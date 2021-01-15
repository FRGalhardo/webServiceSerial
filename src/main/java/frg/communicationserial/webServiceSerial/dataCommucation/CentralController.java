package frg.communicationserial.webServiceSerial.dataCommucation;

import java.util.Scanner;

import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import com.google.gson.Gson;

import frg.communicationserial.webServiceSerial.data.Analogicos;
import frg.communicationserial.webServiceSerial.data.Booleanos;
import frg.communicationserial.webServiceSerial.data.Dados;
import frg.communicationserial.webServiceSerial.data.Temperaturas;
import jssc.SerialPort;
import jssc.SerialPortException;

@RestController
public class CentralController {

	static Scanner sc;
	static String[] arr = null;
	static SerialPort sport = null;

	public static void iniciarSerial() throws SerialPortException {
		try {
			sport = new SerialPort("/dev/ttyUSB2");

			sport.openPort();

			sport.setParams(9600, 8, 1, 0);

		} catch (SerialPortException ex) {
			if (sport.isOpened()) {
				sport.closePort();
			}
			System.out.println(ex);
		}
	}

	@RequestMapping("/input/ligarRele1")
	public void ligarRele1() throws SerialPortException {
		sport.writeString("R|1");
	}

	@RequestMapping("/input/ligarRele2")
	public void ligarRele2() throws SerialPortException {
		sport.writeString("R|2");
	}

	@RequestMapping("/input/ligarRele3")
	public void ligarRele3() throws SerialPortException {
		sport.writeString("R|3");
	}

	@RequestMapping("/input/ligarRele4")
	public void ligarRele4() throws SerialPortException {
		sport.writeString("R|4");
	}

	@RequestMapping("/output")
	public String recebeDados() throws SerialPortException {
		String dadosSujo = sport.readString(128);

		int lastHash = dadosSujo.lastIndexOf("#");
		int firstOfLasthash = lastHash - 44;

		String dadosLimpos = dadosSujo.substring(firstOfLasthash, lastHash);
		dadosLimpos = dadosLimpos.replace("#", "");
		dadosLimpos = dadosLimpos.replace(" ", "");

		String[] tempArray = dadosLimpos.split("\\|");
		String[] temps = tempArray[0].split(":");
		String[] analogicStats = tempArray[1].split(":");
		String[] booleanStats = tempArray[2].split(":");
		
		Dados dd = new Dados();
		
		Temperaturas tmp = new Temperaturas();
		Analogicos ana =  new Analogicos();
		Booleanos boo =  new Booleanos();
		
		tmp.setUmidade((int)Float.parseFloat(temps[0]));
		tmp.setTempCelsius((int)Float.parseFloat(temps[1]));
		tmp.setTempFahrenheit((int)Float.parseFloat(temps[2]));
		tmp.setTempCaixaCelsius((int)Float.parseFloat(temps[3]));
		
		ana.setLuz(Integer.parseInt(analogicStats[0]));
		ana.setGas(Integer.parseInt(analogicStats[1]));
		
		boo.setPirMovel(conversaoBooleana(booleanStats[0]));
		boo.setPirFrente(conversaoBooleana(booleanStats[1]));
		boo.setRele1(conversaoBooleana(booleanStats[2]));
		boo.setRele2(conversaoBooleana(booleanStats[3]));
		boo.setRele3(conversaoBooleana(booleanStats[4]));
		boo.setRele4(conversaoBooleana(booleanStats[5]));
		
		dd.setTemp(tmp);
		dd.setAnalog(ana);
		dd.setBoole(boo);
		
		Gson gson = new Gson();
		String retJson = gson.toJson(dd);
		
		/*System.out.println(tmp.getUmidade() + " " + 
		                   tmp.getTempCelsius() + " " + 
				           tmp.getTempFahrenheit() + " " + 
		                   tmp.getTempCaixaCelsius()  + "\n" + 
						   ana.getLuz() + " " + 
		                   ana.getGas() + "\n" + 
					       boo.isPirMovel() + " " + 
		                   boo.isPirFrente() + " " +  
					       boo.isRele1() + " " + 
		                   boo.isRele2() + " " + 
					       boo.isRele3() + " " + 
		                   boo.isRele4());*/

		return retJson;
	}
	
	private boolean conversaoBooleana(String value) {
		boolean retorno = false;
		
		if("1".equals(value))
			retorno = true;
		
		return retorno;

	}

}
