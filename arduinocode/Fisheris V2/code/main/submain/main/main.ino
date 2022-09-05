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

//Struct GSM
struct GSMCommandOutput{
   String inputCom;
   bool status;
   String outputCom;
};

//MODULE INITIALIZE
LiquidCrystal_I2C lcd(0x27, 20, 4);
BH1750 lightMeter;
HX711 scale(DOUT, CLK);
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

float calibration_factor = 31.41; //HX711 CALIBRATION FACTOR

//URL HTTP
const String postUrl="http://ptsv2.com/t/7vvz7-1662298931/post";
const String getUrl="http://ptsv2.com/t/7vvz7-1662298931/post";

//custom char
const byte tempIcon[] = {
  B00100,
  B01010,
  B01010,
  B01010,
  B01010,
  B10101,
  B10001,
  B01110
};
const byte weightIcon[] = {
  B11111,
  B01110,
  B00100,
  B01110,
  B11001,
  B10101,
  B10001,
  B01110
};
const byte signalIcon[] = {
  B00000,
  B00001,
  B00001,
  B00011,
  B00111,
  B01111,
  B11111,
  B11111
};
const byte luxIcon[] = {
  B00000,
  B00000,
  B10101,
  B01110,
  B11011,
  B01110,
  B10101,
  B00000
};
const byte relayIcon[] = {
  B00000,
  B01010,
  B01010,
  B01010,
  B11111,
  B11111,
  B01110,
  B00100
};

//Module Variable
int simSignal;
float weight,temp,lux;
boolean relayStat=0;
int relayOffSens=100;

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

GSMCommandOutput GSMCommand(String commmand,int timeoutProcess=500,int charLastReceivedTimeout=500){
  if (commmand.indexOf("AT+CSQ")==-1)
  {
      Serial.println(F(""));
      Serial.println(F("---------------------------------------------------------------------------------------------------------------------------"));
      Serial.println(F("-GSM COMMANDS-"));
  }
 
  GSMCommandOutput output;
  String result;
  boolean status=false;
  char GSMCharReading[500];
  memset(GSMCharReading,0,sizeof(GSMCharReading));
  int GSMCharPos=0;
  unsigned long startTime=millis();
  unsigned long currentTime=millis();
  unsigned long charLastReceived=millis();

  Serial3.println(commmand);
  while(true){
    while(Serial3.available()){
      GSMCharReading[GSMCharPos]=Serial3.read();
      GSMCharPos++;
      charLastReceived=millis();
      if(GSMCharPos>=sizeof(GSMCharReading)){break;}
    }
    if(GSMCharPos>=sizeof(GSMCharReading)){break;}
    if(millis()-startTime>timeoutProcess){break;}
    if(millis()-charLastReceived>charLastReceivedTimeout){break;}
  }
  result=String(GSMCharReading);
  
  output.inputCom=commmand;
  output.outputCom=result;

  if(result.indexOf("AT+SAPBR=0,1")!=-1 && result.indexOf("OK")!=-1){
    output.status=true;
  }
  else if (result.indexOf("AT+SAPBR=3,1")!=-1 && result.indexOf("OK")!=-1)
  {
     output.status=true;
  }
  else if (result.indexOf("AT+SAPBR=1,1")!=-1 && result.indexOf("OK")!=-1)
  {
     output.status=true;
  }
  else if (result.indexOf("AT+HTTPINIT")!=-1 && result.indexOf("OK")!=-1)
  {
     output.status=true;
  }
  else if (result.indexOf("AT+HTTPPARA=\"CID\",1")!=-1 && result.indexOf("OK")!=-1)
  {
     output.status=true;
  }
  else if (result.indexOf("AT+HTTPPARA=\"URL\",")!=-1 && result.indexOf("OK")!=-1)
  {
     output.status=true;
  }
  else if (result.indexOf("AT+HTTPPARA=\"CONTENT\",\"application/json\"")!=-1 && result.indexOf("OK")!=-1)
  {
     output.status=true;
  }
  else if (result.indexOf("AT+HTTPDATA=")!=-1 && result.indexOf("DOWNLOAD")!=-1)
  {
     output.status=true;
  }
  else if (result.indexOf("AT+HTTPACTION=1")!=-1 && result.indexOf("+HTTPACTION:")!=-1)
  {
     output.status=true;
  }
  else if (result.indexOf("AT+HTTPACTION=0")!=-1 && result.indexOf("+HTTPACTION:")!=-1)
  {
     output.status=true;
  }
  else if (result.indexOf("AT+HTTPREAD")!=-1 && result.indexOf("+HTTPREAD")!=-1 && ((result.indexOf("200")!=-1 || result.indexOf("201")!=-1)))
  {
     output.status=true;
  }
   else if (result.indexOf("AT+CSQ")!=-1 && result.indexOf("+CSQ:")!=-1)
  {
     output.status=true;
  }
  else{
    if(result.indexOf("AT")==-1 && result.indexOf("OK")!=-1){
      output.status=true;
    }
    else{
      output.status=false;
    }
  }
  if (commmand.indexOf("AT+CSQ")==-1)
  {
      Serial.println("Input Command : " +output.inputCom);
      Serial.println("Status : " +String(output.status));
      Serial.println(F("Output Command ["));
      Serial.println(output.outputCom);
      Serial.println(F("]"));
      Serial.println(F("-GSM COMMANDS END-"));
      Serial.println(F("---------------------------------------------------------------------------------------------------------------------------"));
      Serial.println(F(""));
  }
 
  return output;
}

int getSimSignal(){
  GSMCommandOutput dataResult=GSMCommand("AT+CSQ");
  if(dataResult.status==0){return 0;}
  
  String signal=dataResult.outputCom;
  signal.trim();
  // Serial.println("1:"+signal);
  // Serial.println("2:"+(signal.substring(signal.indexOf('+CSQ:')+2, signal.indexOf(','))));
  int safeVar=(signal.substring(signal.indexOf('+CSQ:')+1, signal.indexOf(','))).toInt();
  if (!safeVar) {
    return 0;
  }
  simSignal=safeVar;
  return simSignal;
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
