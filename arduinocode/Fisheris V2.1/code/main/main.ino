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
const String postTargetUrl="https://risetndev.com/api/logdata/store";
const String postBypassUrl="http://fishdryer.stiki-indonesia.ac.id/api/bypasstls";
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
const String latitude="90.000000";
const String longitude="180.000000";

// Sending Timer
unsigned long curretTime;
unsigned long pastSentTime;
const int sendDelay =3000;

void writeLCD(String msg,int row=0,int collum=0){
  lcd.setCursor(collum,row);//collum,row
  lcd.print(msg);
}

void writeLCDIcon(char *icon,int row=0,int collum=0){
  int id;
  if(icon=="tempIcon"){
    id=0;
    lcd.createChar(id, tempIcon);
    lcd.setCursor(collum,row);//collum,row
    lcd.write(id);

  }  
 else if(icon=="weightIcon"){
    id=1;
    lcd.createChar(id, weightIcon);
    lcd.setCursor(collum,row);//collum,row
    lcd.write(id);

 }
 else if(icon=="signalIcon"){
    id=2;
    lcd.createChar(id, signalIcon);
    lcd.setCursor(collum,row);//collum,row
    lcd.write(id);

 }
 else if(icon=="luxIcon"){
    id=3;
    lcd.createChar(id, luxIcon);
    lcd.setCursor(collum,row);//collum,row
    lcd.write(id);

 }
 else if(icon=="relayIcon"){
    id=4;
    lcd.createChar(id, relayIcon);
    lcd.setCursor(collum,row);//collum,row
    lcd.write(id);
 }
}

void clearLCD(){
  lcd.clear();
}

void clearLCD(int row,int collumStart,int collumEnd){
  for(int i=collumStart;i<=collumEnd;i++){
    lcd.setCursor(i,row);
    lcd.print(" ");
  }
}

void clearLCD(boolean row1,boolean row2,boolean row3,boolean row4){
  if(row1){
    lcd.setCursor(0,0);
    lcd.print("                    ");
  }
  if(row2){
    lcd.setCursor(0,1);
    lcd.print("                    ");
  }
  if(row3){
    lcd.setCursor(0,2);
    lcd.print("                    ");
  }
  if(row4){
    lcd.setCursor(0,3);
    lcd.print("                    ");
  }
}

void setupLoadCell(){
  clearLCD(0,1,1,1);
  Serial.println(F(""));
  Serial.println(F("-SET UP LOADCELL-"));
  writeLCD("--SETUP LOADCELL--",1,1);
  
  Serial.println("Set Default Calibration Factor : " +String(calibration_factor));
  writeLCD("Calibration Factor",2,1);
  writeLCD(String(calibration_factor),3,((20-String(calibration_factor).length())/2));
  scale.set_scale(calibration_factor);
  delay(2000);
  clearLCD(0,0,1,1);
  
  Serial.println(F("Set Tare......."));
  writeLCD("Setup Tare",2,5);
  
  Serial.println(F("In 5....."));
  writeLCD("In 5.....",3,6);
  delay(1000);
  Serial.println(F("In 4.... "));
  writeLCD("In 4.... ",3,6);
  delay(1000);
  Serial.println(F("In 3...  "));
  writeLCD("In 3...  ",3,6);
  delay(1000);
  Serial.println(F("In 2..   "));
  writeLCD("In 2..   ",3,6);
  delay(1000);
  Serial.println(F("In 1.    "));
  writeLCD("In 1.    ",3,6);
  delay(1000);
  Serial.println(F("Tare Set"));
  clearLCD(0,0,0,1);
  writeLCD("Tare Set",3,6);
  scale.tare();
  delay(1000);
  
  clearLCD(0,1,1,1);
  Serial.println(F(""));
}

void setupLCD(){
  Serial.println(F(""));
  Serial.println(F("-SET UP LCD-"));
  lcd.begin();
  lcd.backlight();
  clearLCD();
  Serial.println(F("Setup Complete"));
  Serial.println(F(""));
}

void setupLightMeter(){
  clearLCD(0,1,1,1);
  Serial.println(F(""));
  
  Serial.println(F("-SET UP  LIGHTMETER-"));
  writeLCD("-SET UP  LIGHTMETER-",1,0);
  lightMeter.begin();
  
  Serial.println("Waiting Sensor To Get Value.......");
  writeLCD("Getting Value...",2,2);
  delay(1000);
  
  while(lux==0){
    getLux();
  }
  
  Serial.println("Got Value : "+String(lux)+" lx");
  clearLCD(0,0,1,1);
  writeLCD("Got Value",2,6);
  clearLCD(0,0,0,1);
  writeLCD(String(lux)+" lx",3,((20-(String(lux)+" lx").length())/2));
  delay(1000);
  
  clearLCD(0,1,1,1);
  Serial.println(F(""));
}

void setupTemperatureSensor(){
  clearLCD(0,1,1,1);
  Serial.println(F(""));
  
  Serial.println(F("-SET UP TEMPERATURE-"));
  writeLCD("-SET UP TEMPERATURE-",1,0);
  sensors.begin();//setup temp sensor
  
  Serial.println("Waiting Sensor To Get Value.......");
  writeLCD("Getting Value...",2,2);
  delay(2000);
  
  while(temp==0){
    getTemperature();
  }
  
  Serial.println("Got Value : "+String(temp)+" C");
  clearLCD(0,0,1,1);
  writeLCD("Got Value",2,6);
  clearLCD(0,0,0,1);
  writeLCD(String(temp)+" C",3,((20-(String(temp)+" C").length())/2));
  delay(1000);
  
  clearLCD(0,1,1,1);
  Serial.println(F(""));
}

void setupRelay(){
  clearLCD(0,1,1,1);
  Serial.println(F(""));
  
  Serial.println(F("-SET UP RELAY-"));
  writeLCD("-SET UP RELAY-",1,3);
  pinMode(RELAY,OUTPUT);
  
  Serial.println("Listen To The Relay Sound.......");
  writeLCD("Listen Relay Sound",2,1);
  delay(2000);
  
  for (int i = 1; i <= 4; i++)
  {
    clearLCD(0,0,0,1);
    if (i % 2){
      relayStat=true;
      digitalWrite(RELAY,relayStat);
      Serial.println("relay status : " +String((relayStat)?"ON":"OFF"));
      writeLCD("Relay : " +String((relayStat)?"ON":"OFF"),3,((20-("Relay : " +String((relayStat)?"ON":"OFF")).length())/2));
    }
    else{
      relayStat=false;
      digitalWrite(RELAY,relayStat);
      Serial.println("relay status : " +String((relayStat)?"ON":"OFF"));
      writeLCD("Relay : " +String((relayStat)?"ON":"OFF"),3,((20-("Relay : " +String((relayStat)?"ON":"OFF")).length())/2));
    }
    delay(1000);
    
  }
  digitalWrite(RELAY,false);
  clearLCD(0,1,1,1);
  Serial.println(F(""));
}

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

String serilizeJson(String deviceId=deviceId,float weight=weight,float temp=temp,float lux=lux,boolean relayStat=relayStat,String latitude=latitude,String longitude=longitude){
  StaticJsonDocument<200> doc;

  doc["url"] = postTargetUrl;
  JsonObject dump_data = doc["dump"].createNestedObject("data");
  dump_data["device"] = deviceId;
  dump_data["weight"] = weight;
  dump_data["temp"] = temp;
  dump_data["lux"] = lux;
  dump_data["relay"] = relayStat;
  
  JsonObject dump_data_location = dump_data.createNestedObject("location");
  dump_data_location["latitude"] = latitude;
  dump_data_location["longitude"] = longitude;
  String serilize;
  serializeJson(doc, serilize);
  return serilize;
}

void GSMpowerOn()
{
  digitalWrite(GSMPowerKey,LOW);
  delay(1000);
  digitalWrite(GSMPowerKey,HIGH);
  delay(2000);
  digitalWrite(GSMPowerKey,LOW);
  delay(3000);
}

GSMCommandOutput GSMCommand(String commmand,boolean printStr=true,int timeoutProcess=500,int charLastReceivedTimeout=500){
  if (printStr!=false)
  {
      Serial.println(F(""));
      Serial.println(F("---------------------------------------------------------------------------------------------------------------------------"));
      Serial.println(F("-GSM COMMANDS-"));
  }
  while(Serial3.available()){
    Serial3.read();
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
  else if(result.indexOf("AT+SAPBR=2,1")!=-1 && result.indexOf("+SAPBR: 1,1,")!=-1){
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
  else if (result.indexOf("AT+HTTPACTION=1")!=-1 && result.indexOf("+HTTPACTION:")!=-1 && ((result.indexOf("200")!=-1 || result.indexOf("201")!=-1)))
  {
     output.status=true;
  }
  else if (result.indexOf("AT+HTTPACTION=0")!=-1 && result.indexOf("+HTTPACTION:")!=-1 && ((result.indexOf("200")!=-1 || result.indexOf("201")!=-1)))
  {
     output.status=true;
  }
  else if (result.indexOf("AT+HTTPREAD")!=-1 && result.indexOf("+HTTPREAD")!=-1)
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
  if (printStr!=false)
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

void setupATComToSend(){
  GSMCommand("AT+SAPBR=0,1",true,1000,1000);
  GSMCommand("AT+SAPBR=3,1,\"Contype\",\"GPRS\"");
  GSMCommand("AT+SAPBR=3,1,\"APN\",\"internet\"");
  GSMCommand("AT+SAPBR=1,1",true,3000,3000);
  GSMCommand("AT+HTTPINIT");
  GSMCommand("AT+HTTPPARA=\"CID\",1");
  GSMCommand("AT+HTTPPARA=\"URL\",\""+postBypassUrl+"\"",true,3000,2000);
  GSMCommand("AT+HTTPPARA=\"CONTENT\",\"application/json\"");
}

void setupGSM(){
  clearLCD(0,1,1,1);
  Serial.println(F(""));
  
  Serial.println(F("-SET UP GSM MODEM-"));
  writeLCD("-SET UP GSM MODEM-",1,1);
  Serial3.begin(9600);
  pinMode(GSMPowerKey,OUTPUT);
  
  Serial.println("Waiting Modem To Respond.......");
  writeLCD("Wait Modem Respond",2,1);
  

  while(true){
    GSMpowerOn();
    Serial3.println("AT");
    delay(300);
    String result=Serial3.readString();
    if(result.indexOf("OK")!=-1){
      break;
    }
  }
  
  Serial.println("Modem Responded");
  clearLCD(0,0,1,1);
  writeLCD("Modem Responded",2,2);
  clearLCD(0,0,0,1);
  writeLCD("AT=OK",3,7);
  delay(1000);

  Serial.println("Connecting To Internet");
  clearLCD(0,0,1,1);
  writeLCD("Connecting Internet",2,1);
  clearLCD(0,0,0,1);
  writeLCD("Waiting To Connect",3,1);

  setupATComToSend();
  while(true){
    GSMCommandOutput checkConnection=GSMCommand("AT+SAPBR=2,1");
    if (checkConnection.status){
      break;
    }
    else{
      setupATComToSend();
    }
  }
  
  Serial.println("Connected");
  clearLCD(0,0,0,1);
  writeLCD("Connected",3,5);
  delay(1000);

  clearLCD(0,1,1,1);
  Serial.println(F(""));
}

void postReq(String content){
  GSMCommandOutput checkConnection=GSMCommand("AT+SAPBR=2,1");
  if (checkConnection.status==false){
    setupATComToSend();
  }
  GSMCommand("AT+HTTPDATA=" + String(content.length()) + ",10000");
  GSMCommand(content);
  GSMCommandOutput checkDataSent = GSMCommand("AT+HTTPACTION=1",true,7000,5000);
  clearLCD(0,0,0,1);
  if(checkDataSent.status!=0){
    writeLCD("-Data Sent-",3,5);
  }else{
    writeLCD("-Failed to Send-",3,2);
  }
  GSMCommand("AT+HTTPREAD",3000,3000);
}

void getReq(){
//  String data="{\"Test\":\"HI\"}";
  GSMCommand("AT+SAPBR=0,1",3000,3000);
  GSMCommand("AT+SAPBR=3,1,\"Contype\",\"GPRS\"");
  GSMCommand("AT+SAPBR=3,1,\"APN\",\"internet\"");
  GSMCommand("AT+SAPBR=1,1",3000,3000);
  GSMCommand("AT+HTTPINIT");
  GSMCommand("AT+HTTPPARA=\"CID\",1");
  GSMCommand("AT+HTTPPARA=\"URL\",\""+getUrl+"\"");
  GSMCommand("AT+HTTPACTION=0",7000,5000);
  GSMCommand("AT+HTTPREAD",3000,3000);
}

int getSimSignal(){
  GSMCommandOutput dataResult=GSMCommand("AT+CSQ",false);
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

void updateMainDisplay(){
  Serial.println(F(""));

  Serial.println(F("Module Status"));

  
  String strTmlTemp;
  String strTmlSignal;
  String strTmlWeight;
  String strTmlLux;
  float fltTmlTemp=getTemperature();
  int fltTmlSignalStat=getSimSignal();
  float fltTmlWeight=getWeight();
  float fltTmlLux=getLux();

  
  Serial.println("temp : " + String(fltTmlTemp)+" C"); 
  if(fltTmlTemp>=100){
    strTmlTemp=":"+String(fltTmlTemp,0)+"C ";
  }
  else if(fltTmlTemp>=10)
  {
    strTmlTemp=":"+String(fltTmlTemp,1)+"C" ;
  } 
  else
  {
    strTmlTemp=":"+String(fltTmlTemp,1)+" C ";
  } 
  clearLCD(1,1,8);
  writeLCDIcon("tempIcon",1,1);
  writeLCD(strTmlTemp.substring(0, 6),1,2);

  String strTmlRelayStat;
  boolean fltTmlRelayStat=getAndUpdateRelayStat();
  Serial.println("relay status : " +String((fltTmlRelayStat)?"ON":"OFF"));
  if(fltTmlRelayStat){
    strTmlTemp=":ON ";
  }
  else
  {
    strTmlTemp=":OFF ";
  } 
  clearLCD(1,9,14);
  writeLCDIcon("relayIcon",1,9);
  writeLCD(strTmlTemp.substring(0, 5),1,10);

 
  Serial.println("GSM signal : " +String(fltTmlSignalStat));
  if(fltTmlSignalStat>9){
    strTmlSignal=":"+String(fltTmlSignalStat);
  }
  else
  {
    strTmlSignal=":0"+String(fltTmlSignalStat);
  } 
  clearLCD(1,15,19);
  writeLCDIcon("signalIcon",1,15);
  writeLCD(strTmlSignal.substring(0, 5),1,16);

  

  Serial.println("weight : " + String(fltTmlWeight)+" Gram");
  if (fltTmlWeight>=10000)
  {
    strTmlWeight=":"+String(fltTmlWeight/1000,1)+"Kg ";
  }
  else if(fltTmlWeight>=1000){
    strTmlWeight=":"+String(fltTmlWeight/1000,2)+"Kg ";
  }
  else if (fltTmlWeight>=100)
  {
    strTmlWeight=":"+String(int(fltTmlWeight))+"g ";
  } 
  else{
    strTmlWeight=":"+String(int(fltTmlWeight))+"g ";
  }

  Serial.println("Lux : " + String(fltTmlLux)+" Lx");
  if (fltTmlLux>=100000)
  {
    strTmlLux=":"+String(fltTmlLux/1000,1)+"KLx";
  }
  else if (fltTmlLux>=10000)
  {
    strTmlLux=":"+String(fltTmlLux/1000,1)+"KLx";
  }
  else if(fltTmlLux>=1000){
    strTmlLux=":"+String(fltTmlLux/1000,1)+"KLx";
  }
  else if (fltTmlLux>=100)
  {
    strTmlLux=":"+String(int(fltTmlLux))+"Lx";
  } 
  else{
    strTmlLux=":"+String(int(fltTmlLux))+"Lx";
  }
  int middleling=(20-(strTmlWeight.length()+strTmlLux.length())-1)/2;
  clearLCD(0,0,1,0);
  writeLCDIcon("weightIcon",2,middleling);
  writeLCD(strTmlWeight,2,1+middleling);
  writeLCDIcon("luxIcon",2,1+middleling+strTmlWeight.length());
  writeLCD(strTmlLux,2,1+1+middleling+strTmlWeight.length());

  String strSentTimer="Send Every : "+String(sendDelay/1000)+" S";
  Serial.println(strSentTimer);
  clearLCD(0,0,0,1);
  writeLCD(strSentTimer,3,(20-1-strSentTimer.length())/2);
  Serial.println(F(""));
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
