package frg.communicationserial.webServiceSerial;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;

import frg.communicationserial.webServiceSerial.dataCommucation.CentralController;
import jssc.SerialPortException;

@SpringBootApplication
public class WebServiceSerialApplication {

	public static void main(String[] args) throws SerialPortException {
		
		CentralController cc = new CentralController();
		
		cc.iniciarSerial();
		
		SpringApplication.run(WebServiceSerialApplication.class, args);
	}

}
