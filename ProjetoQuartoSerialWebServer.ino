#include <SerialRelay.h>
#include <IRremote.h>
#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h> 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//Pinos

#define RECV_PIN 2 //RemoteControl
#define ONE_WIRE_BUS 3 //DS18B20           
#define PIRFRENTE 5 //PIRFRENTE
#define PIRMOVEL 6 //PIRMOVEL
#define DHTPIN 8 //PINO DIGITAL UTILIZADO PELO DHT22
#define RSPIN 11 //Pino Rele Serial
#define RSCLOCK 12 // Clock Rele Serial

#define GAS A2//MQ2
#define LDR A3 //SensorLuz

//Modulo ReleSerial
#define NumModules 1
SerialRelay relays(RSPIN,RSCLOCK,NumModules);

#define DHTTYPE DHT22 //DEFINE O MODELO DO SENSOR (DHT22 / AM2302)
DHT dht(DHTPIN, DHTTYPE); //PASSA OS PARÂMETROS PARA A FUNÇÃO

// Define uma instancia do oneWire para comunicacao com o sensor
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress sensor1;

//RemoteControl
IRrecv irrecv(RECV_PIN);
decode_results results;
int stat = 1;

//SystemVariables
int qtdParam = 3;
float tempC, tempF, umid, tempCaixa = 0.0;
int qtdLuz, qtdGas = 0;
bool staPirMovel, staPirFrente, staRele1,staRele2, staRele3, staRele4 = false;
String protoc,param[3], rec = "";

void setup(){
  Serial.begin(9600); //INICIALIZA A SERIAL
  
  //DHT22
  dht.begin(); //INICIALIZA A FUNÇÃO

  //DS28B20
  sensors.begin();
  // Localiza e mostra enderecos dos sensores
  Serial.println("Localizando sensores DS18B20...");
  Serial.print("Foram encontrados ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" sensores.");
  if (!sensors.getAddress(sensor1, 0)) 
     Serial.println("Sensores nao encontrados !"); 

  //Limpa Rele Serial
  limpaReleSerial(NumModules);   

  //SensorLuz
  pinMode(LDR, INPUT);

  //GAS MQ2
  pinMode(GAS, INPUT);

  //PIRFrente
  pinMode(PIRFRENTE, INPUT);
  
  //PIRMovel
  pinMode(PIRMOVEL, INPUT);

  //RemoteControl
  irrecv.enableIRIn(); // Start the receiver

  //Interrupcao
  attachInterrupt(digitalPinToInterrupt(RECV_PIN),interrupt,RISING);
}

void loop(){
  lerSerial();
  obtValDHT();
  obtValDS();
  obtLuz();
  obtGas();
  obtPresenca();
  montaProtocolo();
  apresentaProtocolo();
  irrecv.resume(); // Receive the next value
  delay(10);
}

void interrupt(){
  static unsigned long delayEstado;

  if((millis() - delayEstado) > 100){
    delayEstado = millis();
  }

  verificaControle();
}

void obtValDHT(){
  umid = dht.readHumidity();
  tempC = dht.readTemperature();
  tempF = dht.readTemperature(true);
}

void obtValDS(){
  // Le a informacao do sensor
  sensors.requestTemperatures();
  tempCaixa = sensors.getTempC(sensor1);
}

void obtLuz(){  
   int light = analogRead(LDR);
   qtdLuz = map(light,0,1023,0,100);
}

void obtGas(){
  int gas = analogRead(GAS);
  qtdGas = map(gas,0,1023,0,100);
}

void obtPresenca(){
  staPirFrente= digitalRead(PIRFRENTE);
  staPirMovel = digitalRead(PIRMOVEL);
}

void limpaReleSerial(int qtdModulos){
  int numReles = 4;
  for(int i = 1; i <= qtdModulos; i++){
    for(int j = 1; j <= numReles; j++){
      relays.SetRelay(j, SERIAL_RELAY_OFF, i);    
    }
  }
}

void verificaControle(){  
  if (irrecv.decode(&results)) {

    switch(results.value){
      case(0xFFA25D): rele1(); break;//POWER
      case(0xFFE21D): rele2(); break;//THUNDER
      case(0xFF02FD): Serial.println("!-NO ASSIGNMENT FOR UP"); break;//UP
      case(0xFFE01F): Serial.println("!-NO ASSIGNMENT FOR LEFT"); break;//LEFT
      case(0xFFA857): Serial.println("!-NO ASSIGNMENT FOR CENTER"); break;//CENTER
      case(0xFF906F): Serial.println("!-NO ASSIGNMENT FOR RIGHT"); break;//RIGHT
      case(0xFF9867): Serial.println("!-NO ASSIGNMENT FOR DOWN"); break;//DOWN
      case(0xFF30CF): Serial.println("!-NO ASSIGNMENT FOR LETTER A"); break;//LETTER A
      case(0xFF18E7): Serial.println("!-NO ASSIGNMENT FOR LETTER B"); break;//LETTER B
      case(0xFF7A85): Serial.println("!-NO ASSIGNMENT FOR LETTER C"); break;//LETTER C
      case(0xFF10EF): Serial.println("!-NO ASSIGNMENT FOR LETTER D"); break;//LETTER D
      case(0xFF38C7): Serial.println("!-NO ASSIGNMENT FOR LETTER E"); break;//LETTER E
      case(0xFF5AA5): Serial.println("!-NO ASSIGNMENT FOR LETTER F"); break;//LETTER F
      case(0xFF42BD): Serial.println("!-NO ASSIGNMENT FOR LETTER G"); break;//LETTER G
      case(0xFF4AB5): Serial.println("!-NO ASSIGNMENT FOR LETTER H"); break;//LETTER H
      case(0xFF52AD): Serial.println("!-NO ASSIGNMENT FOR LETTER I"); break;//LETTER I
    }
    
    irrecv.resume(); // Receive the next value
  }
}

void rele1(){
  if(!staRele1){
        relays.SetRelay(1, SERIAL_RELAY_ON, 1);
        staRele1 = true;
      }else{
        relays.SetRelay(1, SERIAL_RELAY_OFF, 1);
        staRele1 = false;
      }
}

void rele2(){
  if(!staRele2){
        relays.SetRelay(2, SERIAL_RELAY_ON, 1);
        staRele2 = true;
      }else{
        relays.SetRelay(2, SERIAL_RELAY_OFF, 1);
        staRele2 = false;
      }
}

void rele3(){
  if(!staRele3){
        relays.SetRelay(3, SERIAL_RELAY_ON, 1);
        staRele3 = true;
      }else{
        relays.SetRelay(3, SERIAL_RELAY_OFF, 1);
        staRele3 = false;
      }
}

void rele4(){
  if(!staRele4){
        relays.SetRelay(4, SERIAL_RELAY_ON, 1);
        staRele4 = true;
      }else{
        relays.SetRelay(4, SERIAL_RELAY_OFF, 1);
        staRele4 = false;
      }
}

void montaProtocolo(){
  protoc = "";
  protoc = "#";
  protoc += umid;
  protoc += ":";
  protoc += tempC;
  protoc += ":";
  protoc += tempF;
  protoc += ":";
  protoc += tempCaixa;
  protoc += "|";
  protoc += qtdLuz;
  protoc += ":";
  protoc += qtdGas;
  protoc += "|";
  protoc += staPirMovel;
  protoc += ":";
  protoc += staPirFrente;
  protoc += ":";
  protoc += staRele1;
  protoc += ":";
  protoc += staRele2;
  protoc += ":";
  protoc += staRele3;
  protoc += ":";  
  protoc += staRele4;
  protoc += "#";    
}

void apresentaProtocolo(){
  Serial.println(protoc);
}

void lerSerial(){
  if(Serial.available()){
    //CapturaSerial
    rec = Serial.readString();
    //ConversaoString->Char
    int n = rec.length();
    char mid[n + 1];
    strcpy(mid, rec.c_str());
    n = 0;
    //SplitChar*
    char *token;
    token = strtok(mid, "|");
    while(token != NULL){
      param[n] = token;
      token = strtok(NULL, "|");
      n++;
    }
    interpretarSerial();
  }

  limpaParam();
}

void limpaParam(){
  for (int h = 0; h < qtdParam; h++) {
      param[h] = "";
  }
}

void interpretarSerial(){

  String tipo = param[0];
  String number = param[1];
  String value = param[2];

  if(tipo == "R"){
    if(number == "1"){
      rele1();
    }else if(number == "2"){
      rele2();
    }else if(number == "3"){
      rele3();
    }else if(number == "4"){
      rele4();
    }
    else{
      Serial.println("!Comando Invalido");
    }
  }else{
    Serial.println("!Comando Invalido");
  }
      
}
