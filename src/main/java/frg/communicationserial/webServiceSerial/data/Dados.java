package frg.communicationserial.webServiceSerial.data;

public class Dados {
	
	private Temperaturas temp;
	
	private Analogicos analog;
	
	private Booleanos boole;

	public Temperaturas getTemp() {
		return temp;
	}

	public void setTemp(Temperaturas temp) {
		this.temp = temp;
	}

	public Analogicos getAnalog() {
		return analog;
	}

	public void setAnalog(Analogicos analog) {
		this.analog = analog;
	}

	public Booleanos getBoole() {
		return boole;
	}

	public void setBoole(Booleanos boole) {
		this.boole = boole;
	}
}
