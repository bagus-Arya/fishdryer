/* www.nyebarilmu.com */
 
//testing module

#define GSMPowerKey  2
#include <SoftwareSerial.h>
SoftwareSerial SIM808(10,11);//TX,RX

#include <ArduinoJson.h>

// CHubungkan TXD untuk Arduino pin 2 
// Hubungkan RXD untuk Arduino pin 3
char c = ' ';
int sentInterval=3000;
unsigned long lastSent=millis();
const char* url="http://ptsv2.com/t/ozsqu-1662125812/post";



void powerUpGSM(){
  pinMode(GSMPowerKey,OUTPUT);
  digitalWrite(GSMPowerKey,LOW);
  delay(1000);
  digitalWrite(GSMPowerKey,HIGH);
  delay(2000);
  digitalWrite(GSMPowerKey,LOW);
  delay(3000);
}

String serilizeJson(String api_key="test",float suhu=20,float berat=900,boolean lampu=false){
  StaticJsonDocument<128> doc;

  JsonObject data = doc.createNestedObject("data");
  data["device"] = "FD001";
  data["weight"] = 50000;
  data["temp"] = 200;
  data["lux"] = 1000000;
  data["relay"] = true;
  
  JsonObject data_location = data.createNestedObject("location");
  data_location["lat"] = "09876543";
  data_location["long"] = "6789098765";
  String output;
  serializeJson(doc, output);
//  String output="{\"api_key\":\""+api_key+"\",\"suhu\":"+(suhu)+",\"berat\":"+String(berat)+",\"lampu\":"+String(lampu)+"}";
  return output;
}

void testGSM(){
  while(true){
     // memastikan pembacaan dari SIM800 dan dikirim datanya ke Arduino(Serial Monitor)
    if (SIM808.available())
    { c = SIM808.read();
      Serial.write(c);}
 
    // memastikan pembacaan dari SIM800 dan dikirim datanya ke Arduino(Serial Monitor)
    if (Serial.available())
    { c = Serial.read();
      SIM808.write(c);  
       }
  }
}

//void resetGSMCharReading(){
//  memset(GSMCharReading,0,sizeof(GSMCharReading));
//  GSMCharPos=0;
//}

String GSMCommand(String commmand,int timeoutProcess=500,int charLastReceivedTimeout=500){
  Serial.println("-----------------------");
  Serial.println("GSM Command");
  Serial.println("-----------------------");
  Serial.println("Typing Command:");
  Serial.println(commmand);
  char GSMCharReading[170];
  memset(GSMCharReading,0,sizeof(GSMCharReading));
  int GSMCharPos=0;
  unsigned long startTime=millis();
  unsigned long currentTime=millis();
  unsigned long charLastReceived=millis();
  SIM808.println(commmand);
  while(true){
    while(SIM808.available()){
      GSMCharReading[GSMCharPos]=SIM808.read();
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

void postReq(String content){
//  Serial.println(content);
  GSMCommand("AT+SAPBR=0,1",3000,3000);
  GSMCommand("AT+SAPBR=3,1,\"Contype\",\"GPRS\"");
  GSMCommand("AT+SAPBR=3,1,\"APN\",\"internet\"");
  GSMCommand("AT+SAPBR=1,1",3000,3000);
  GSMCommand("AT+HTTPINIT");
  GSMCommand("AT+HTTPPARA=\"CID\",1");
  GSMCommand("AT+HTTPPARA=\"URL\",\""+String(url)+"\"",3000,2000);
  GSMCommand("AT+HTTPPARA=\"CONTENT\",\"application/json\"");
  GSMCommand("AT+HTTPDATA=" + String(content.length()) + ",10000");
  GSMCommand(content);
  GSMCommand("AT+HTTPACTION=1",7000,5000);
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
  GSMCommand("AT+HTTPPARA=\"URL\",\""+String(url)+"\"");
  GSMCommand("AT+HTTPACTION=0",7000,5000);
  GSMCommand("AT+HTTPREAD",3000,3000);
}


void setup() 
{
    powerUpGSM();
    Serial.begin(9600); //Menggunakan komunikasi serial pada 9600bps
    delay(1000);
    Serial.println(F("Serial Started"));
    SIM808.begin(9600);  
    delay(1000);
    Serial.println("Serial For GSM Started");
    testGSM();
//    postReq(serilizeJson());
//    getReq();
}

void loop()
{
  
}
