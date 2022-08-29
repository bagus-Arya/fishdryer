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
  // put your main code here, to run repeatedly:
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
  SIM900.println(F("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",\"80\""));
  delay(2000);
  SIM900.println(F("AT+CIPSEND"));
  delay(2000);
  SIM900.println("GET /update?api_key=UUFK0YH7PU4CCVPR&field1="+String(suhu)+"&field2="+String(berat)+"\r\n\x1A");
  delay(9000); 
  SIM900.println(F("AT+CIPCLOSE"));
  delay(2000);
}
