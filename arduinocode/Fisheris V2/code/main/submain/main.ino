//LIBRARIES
#include <LiquidCrystal_I2C.h>
#include <BH1750.h>
#include "HX711.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ArduinoJson.h>

//INCREASE BUFFER
// #define _SS_MAX_RX_BUFF 256

// DEVICE ID
const String deviceId="FD001";

//PINS HX711
#define DOUT  A1
#define CLK  A0

//GSMPowerKey
#define GSMPowerKey  2

//PINS DALLASTEMP
#define ONE_WIRE_BUS A7

//PINS RELAY
#define RELAY 7

//MODULE INITIALIZE
LiquidCrystal_I2C lcd(0x27, 20, 4);
BH1750 lightMeter;
HX711 scale(DOUT, CLK);
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Sending Timer
unsigned long curretTime;
unsigned long pastSentTime;
const int sendDelay =30000;

float getTemperature(){
  sensors.requestTemperatures();
  temp=sensors.getTempCByIndex(0);
  return temp;
}

float getLux(){
  lux=lightMeter.readLightLevel();
  return lux;
}

float getWeight(){
  weight=scale.get_units(), 4;
  if (weight<0){
    weight=0.0;
  }
  return weight;
}

boolean getAndUpdateRelayStat(){
  relayStat=(getLux()<relayOffSens)?true:false;
  digitalWrite(RELAY,relayStat);
  return relayStat;
}

String serilizeJson(String device="FD001",float weight=50000,float temp=200,float lux=1000000,boolean relay=true,String latitude="09876543",String longitude="6789098765"){
  StaticJsonDocument<150> doc;

  JsonObject data = doc.createNestedObject("data");
  data["device"] = device;
  data["weight"] = weight;
  data["temp"] = temp;
  data["lux"] = lux;
  data["relay"] = relay;
  
  JsonObject data_location = data.createNestedObject("location");
  data_location["lat"] = latitude;
  data_location["long"] = longitude;
  String serilize;
  serializeJson(doc, serilize);
//  String output="{\"api_key\":\""+api_key+"\",\"suhu\":"+(suhu)+",\"berat\":"+String(berat)+",\"lampu\":"+String(lampu)+"}";
  return serilize;
}

void setup()
{
  Serial.begin(9600);
  Serial.println(F("DRYFISHER V2.0 Begin...."));
  
  setupLCD();//setup lcd
  // delay(1000);
  
  writeLCD("DRYFISHER V2.0",0,3);//row,collum
  // delay(1000);

  setupRelay();
  // delay(1000);
  
  setupLightMeter();
  // delay(1000);
  
  setupLoadCell();//setup loadcell
  // delay(1000);
  
  setupTemperatureSensor();
  // delay(1000);
  setupGSM();
  clearLCD(0,1,1,1);
  // postReq(serilizeJson());
  curretTime=millis();
  pastSentTime=millis();
}

void loop()
{
  if(curretTime-pastSentTime>sendDelay){
    Serial.println(F(""));
    Serial.println("Sending The Message");
    
    clearLCD(0,0,0,1);
    writeLCD("Sending Data....",3,2);

    postReq(serilizeJson(deviceId,weight,temp,lux,relayStat));

    pastSentTime=millis();
    Serial.println(F(""));
  }
  curretTime=millis();
  updateMainDisplay();
}