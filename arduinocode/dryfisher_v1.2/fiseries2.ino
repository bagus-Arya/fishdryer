// library loadcell
#include "HX711.h"
#include <Wire.h>

// library ds18b20
#include <OneWire.h>
#include <DallasTemperature.h>

// library LCDI2C
#include <LiquidCrystal_I2C.h>

// SIM900
#include <SoftwareSerial.h>
SoftwareSerial SIM900(11,10);//TX,RX

// Relay
#define relay 7
int relayStat = 0;

// Lampu
#define sensorPin A5
int sensorValue = 0;

// HX711 circuit wiring
#define LOADCELL_DOUT_PIN A1
#define LOADCELL_SCK_PIN A0
const float calibration_factor = 242.01;

long int currentTime=millis();
long int pastTime=millis();
const int sendingDelay=10000;

// LCD wiring
LiquidCrystal_I2C lcd(0x3F, 16, 2);

HX711 scale;

// DS18B20 wiring
#define ONE_WIRE_BUS 2
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

float suhu=0, berat=0;

char idAlat[] = "F001";
char postURL[] = "http://203.142.74.174:3280/api/logdata/store";

String contentJson(String idAlat,float suhu,float berat,boolean lampu){
  String test="{\"data\":{\"device\":\""+idAlat+"\",\"suhu\":"+String(suhu)+",\"berat\":"+String(berat)+",\"lampu\":"+String(lampu)+"}}";
  return test;
}

String GSMCommand(String commmand,int timeoutProcess=500,int charLastReceivedTimeout=500){
  Serial.println("-----------------------");
  Serial.println("GSM Command");
  Serial.println("-----------------------");
  Serial.println("Typing Command:");
  Serial.println(commmand);
  char GSMCharReading[150];
  memset(GSMCharReading,0,sizeof(GSMCharReading));
  int GSMCharPos=0;
  unsigned long startTime=millis();
  unsigned long currentTime=millis();
  unsigned long charLastReceived=millis();
  SIM900.println(commmand);
  while(true){
    while(SIM900.available()){
      GSMCharReading[GSMCharPos]=SIM900.read();
      GSMCharPos++;
      charLastReceived=millis();
      if(GSMCharPos>=sizeof(GSMCharReading)){break;}
    }
    if(GSMCharPos>=sizeof(GSMCharReading)){break;}
    if(millis()-startTime>timeoutProcess){break;}
    if(millis()-charLastReceived>charLastReceivedTimeout){break;}
  }
  String result=String(GSMCharReading);
  Serial.println("");
  Serial.println("Result:");
  Serial.print(result);
  Serial.println("-----------------------");
  return result;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  SIM900.begin(9600);
  sensors.begin(); // sensor ds18b20
  pinMode(relay,OUTPUT);
  digitalWrite(relay, HIGH);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("-DryFISH- Berat");
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(calibration_factor);       
  scale.tare();
}

void postReq(String url,String idAlat,float suhu,float berat,boolean lampu){
  String content = contentJson(idAlat,suhu,berat,lampu);
//  Serial.println(content);
  GSMCommand("AT+SAPBR=0,1",3000,3000);
  GSMCommand("AT+SAPBR=3,1,\"Contype\",\"GPRS\"");
  GSMCommand("AT+SAPBR=3,1,\"APN\",\"internet\"");
  GSMCommand("AT+SAPBR=1,1",3000,3000);
  GSMCommand(F("AT+HTTPINIT"));
  GSMCommand(F("AT+HTTPPARA=\"CID\",1"));
  GSMCommand("AT+HTTPPARA=\"URL\",\""+url+"\"");
  GSMCommand("AT+HTTPPARA=\"CONTENT\",\"application/vnd.api+json\"");
  GSMCommand("AT+HTTPDATA=" + String(content.length()) + ",10000");
  GSMCommand(content);
  GSMCommand("AT+HTTPACTION=1",7000,5000);
  GSMCommand(F("AT+GMR"));
  GSMCommand(F("AT+HTTPREAD"));
}


void loop() {
  // put your main code here, to run repeatedly:
  get_berat();
  get_suhu();
  lampu();
//  currentTime=millis();
//  if(currentTime-pastTime>sendingDelay){
//    Serial.println("Sending Data...........");
//    delay(1000);
      postReq(postURL,idAlat,suhu,berat,relayStat);
//    pastTime=millis();
//  }
}

void get_suhu(){
  sensors.requestTemperatures();
  suhu=sensors.getTempCByIndex(0);
  lcd.setCursor(1,1);
  lcd.print(suhu,1);
  lcd.print(" C  ");
}
void get_berat(){
  berat= scale.get_units(), 4;
  if(berat<=0.1){
    lcd.setCursor(13, 1);
    lcd.print("   ");
    berat=0.0;
    lcd.setCursor(9, 1);
    lcd.print(berat,1);
    lcd.print(" g");
  }
  
  else if(berat<1000){
    lcd.setCursor(13, 1);
    lcd.print("   ");
    lcd.setCursor(9, 1);
    lcd.print(berat,1);
    lcd.print(" g");
  }
  else if(berat>=1000){
    lcd.setCursor(13, 1);
    lcd.print("   ");
    float hasil=berat/1000;
    lcd.setCursor(9, 1);
    lcd.print(hasil);
    lcd.print(" kg");
  }
}
void lampu(){
  sensorValue=analogRead(sensorPin); //Membaca nilai analog dari pin A5
//  Serial.print("Nilai Sensor:");
//  Serial.println(sensorValue); //Mencetak hasil pada monitor serial
    if((sensorValue <= 800)){ //voltage<=1 && sensorValue <=108 Ambang batas yang saya gunakan disini adalah bernilai 1
      digitalWrite (7, LOW); // Lampu menyala
      relayStat=1;
      }
    else{
      digitalWrite(7, HIGH);
      relayStat=0;
    } 
}