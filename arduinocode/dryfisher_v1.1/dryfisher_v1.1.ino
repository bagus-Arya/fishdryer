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
// Lampu
int sensorPin = A5;
int sensorValue = 0;
// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = A1;
const int LOADCELL_SCK_PIN = A0;  
LiquidCrystal_I2C lcd(0x3F, 16, 2);
HX711 scale;
// DS18B20 wiring
#define ONE_WIRE_BUS 2
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
float Celcius=0, berat=0;
int stateLampu;
char c = ' ';
int sentInterval=3000;
unsigned long lastSent=millis();

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
  scale.set_scale(327);      
  scale.tare(357); 
}

void loop() {
  // put your main code here, to run repeatedly:
  get_berat();
  get_suhu();
  lampu();
  getReq("http://fishdryer.stiki-indonesia.ac.id/api/store/m8A0ZgmbcFal8EIECYz0moPRUYKhsyLtJEp1a8o4|62e802dfb04b30.04414522/"+String(Celcius)+"/"+String(berat)+"/"+String(stateLampu));
  Serial.print("Send Data Success"); 
}

void get_suhu(){
  sensors.requestTemperatures();
  Celcius=sensors.getTempCByIndex(0);
  Serial.print("Celcius:");
  Serial.print(Celcius);
  lcd.setCursor(2,1);
  lcd.print(Celcius);
  delay(500);
  return Celcius;
}

void get_berat(){
  Serial.print("Berat");
  berat= scale.get_units(5);
  if(berat<=0.1){
    berat=0.0;
  }
  Serial.println(berat,1);
  if(berat<1000){
    lcd.setCursor(9, 1);
    lcd.print(berat,1);
    lcd.print(" g");
    Serial.println(berat,1);
  }
  delay(500);
  if(berat>=1000){
    float hasil=berat/1000;
    lcd.setCursor(9, 1);
    lcd.print(hasil);
    lcd.print(" kg");
    Serial.println(hasil);
  }
  return berat;
}

void lampu(){
  sensorValue=analogRead(sensorPin); //Membaca nilai analog dari pin A5
  Serial.print("Nilai Sensor:");
  Serial.println(sensorValue); //Mencetak hasil pada monitor serial
//  float voltage =sensorValue * (5.0/1023.0); // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V)
//  Serial.print("V:");
//  Serial.println(voltage);
    if((sensorValue <= 850)){ //voltage<=1 && sensorValue <=108 Ambang batas yang saya gunakan disini adalah bernilai 1
      digitalWrite (7, LOW); // Lampu menyala
      stateLampu = 1;
      }
    else{
      digitalWrite(7, HIGH);
      stateLampu = 0;
    }
  delay(500);  
}

void getReq(String url){
  GSMCommand("AT+SAPBR=0,1",3000,3000);
  GSMCommand("AT+SAPBR=3,1,\"Contype\",\"GPRS\"");
  GSMCommand("AT+SAPBR=3,1,\"APN\",\"internet\"");
  GSMCommand("AT+SAPBR=1,1",5000,5000);
  GSMCommand("AT+HTTPINIT");
  GSMCommand("AT+HTTPPARA=\"CID\",1");
  GSMCommand("AT+HTTPPARA=\"URL\",\""+url+"\"");
  GSMCommand("AT+HTTPACTION=0",7000,5000);
}

///////////
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
// Lampu
int sensorPin = A5;
int sensorValue = 0;
// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = A1;
const int LOADCELL_SCK_PIN = A0;  
LiquidCrystal_I2C lcd(0x3F, 16, 2);
HX711 scale;
// DS18B20 wiring
#define ONE_WIRE_BUS 2
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
float Celcius=0, berat=0;

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
  scale.set_scale(327);      
  scale.tare(357); 
  set_simcard();
}

void set_simcard(){
  SIM900.println(F("AT+CREG=1")); //mengaktifkan registrasi jaringan
  delay(100);
  Serial.println(SIM900.readString());
  SIM900.println(F("AT+CGATT=1")); //masuk ke gprs servis
  delay(100);
  Serial.println(SIM900.readString());
  SIM900.println(F("AT+CIPSHUT")); //menonaktifkan gprs
  delay(100);
  Serial.println(SIM900.readString());
  SIM900.println(F("AT+CIPMUX=0"));//mengaktifkan single IP koneksi
  delay(100);
  Serial.println(SIM900.readString());
  SIM900.println(F("AT+CSTT=\"internet""\"")); //setting APN kartu sim xl
  delay(500);
  SIM900.println(F("AT+CSTT?")); //memastikan setting APN benar
  delay(5000);
  Serial.println(SIM900.readString());
  SIM900.println(F("AT+CIICR")); //memulai koneksi GPRS
  delay(10000);
  Serial.println(SIM900.readString());
  SIM900.println(F("AT+CIFSR")); //request IP
  delay(5000);
  Serial.println(SIM900.readString());
  lcd.setCursor(2,1);
  lcd.print("Ready");
}

void loop() {
  get_berat();
  get_suhu();
  lampu();
  send_data(Celcius, berat);
  Serial.print("Send Data Success"); 
}

void get_suhu(){
  sensors.requestTemperatures();
  Celcius=sensors.getTempCByIndex(0);
  Serial.print("Celcius:");
  Serial.print(Celcius);
  lcd.setCursor(2,1);
  lcd.print(Celcius);
  delay(500);
  return Celcius;
}
void get_berat(){
  Serial.print("Berat");
  berat= scale.get_units(5);
  if(berat<=0.1){
    berat=0.0;
  }
  Serial.println(berat,1);
  if(berat<1000){
    lcd.setCursor(9, 1);
    lcd.print(berat,1);
    lcd.print(" g");
    Serial.println(berat,1);
  }
  delay(500);
  if(berat>=1000){
    float hasil=berat/1000;
    lcd.setCursor(9, 1);
    lcd.print(hasil);
    lcd.print(" kg");
    Serial.println(hasil);
  }
  return berat;
}
void lampu(){
  sensorValue=analogRead(sensorPin); //Membaca nilai analog dari pin A5
  Serial.print("Nilai Sensor:");
  Serial.println(sensorValue); //Mencetak hasil pada monitor serial
//  float voltage =sensorValue * (5.0/1023.0); // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V)
//  Serial.print("V:");
//  Serial.println(voltage);
    if((sensorValue <= 850)){ //voltage<=1 && sensorValue <=108 Ambang batas yang saya gunakan disini adalah bernilai 1
      digitalWrite (7, LOW); // Lampu menyala
      }
    else{
      digitalWrite(7, HIGH);
    }
  delay(500);  
}
void send_data(float suhu, float berat){
  SIM900.println(F("AT+CIPSTART=\"TCP\",\"fishdryer.stiki-indonesia.ac.id\",80\r\n"));
  delay(2000);
  SIM900.println(F("AT+CIPSEND"));
  delay(2000);
  SIM900.println("POST /api/store HTTP/1.1\r\nHost:fishdryer.stiki-indonesia.ac.id\r\nAccept: application/vnd.api+json\r\nContent-Length: 224\r\nContent-Type: application/vnd.api+json\r\n\r\n{\r\n\"api_key\":\"m8A0ZgmbcFal8EIECYz0moPRUYKhsyLtJEp1a8o4|62e802dfb04b30.04414522\",\r\n\"suhu\":\""+String(suhu)+"\",\r\n\"berat\":"+String(berat)+",\r\n\"lampu\":\"1\"\r\n}\r\n\x1A");
  delay(9000); 
  SIM900.println(F("AT+CIPCLOSE"));
  delay(2000);
}