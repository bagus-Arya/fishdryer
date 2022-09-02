/* www.nyebarilmu.com */
 
//testing module
 
#include <SoftwareSerial.h>
SoftwareSerial SIM900(11,10);//TX,RX

//#include <ArduinoJson.h>

// CHubungkan TXD untuk Arduino pin 2 
// Hubungkan RXD untuk Arduino pin 3
char c = ' ';
int sentInterval=3000;
unsigned long lastSent=millis();
String api_key="m8A0ZgmbcFal8EIECYz0moPRUYKhsyLtJEp1a8o4|62e802dfb04b30.04414522";
String getURL="http://fishdryer.stiki-indonesia.ac.id/api/store/m8A0ZgmbcFal8EIECYz0moPRUYKhsyLtJEp1a8o4|62e802dfb04b30.04414522/99/100/1";
String postURL="http://fishdryer.stiki-indonesia.ac.id/api/store";


void setup() 
{
    Serial.begin(9600); //Menggunakan komunikasi serial pada 9600bps
    delay(1000);
    Serial.println("Serial Started");
    SIM900.begin(9600);  
    delay(1000);
    Serial.println("Serial For GSM Started");
   testGSM();
//  postReq(postURL,api_key,11,900,1);
  // getReq(getURL);
//set_simcard();
//send_data(22,300);
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
//  lcd.setCursor(2,1);
//  lcd.print("Ready");
}
void send_data(float suhu, float berat){
  SIM900.println(F("AT+CIPSTART=\"TCP\",\"kulkul.instiki.ac.id\",\"80\""));
  delay(2000);
  SIM900.println(F("AT+CIPSEND"));
  delay(2000);
  SIM900.println("GET /kulkul-app/public/api/fishdryer/50.23/30.12\r\n\x1A");
  delay(9000); 
  SIM900.println(F("AT+CIPCLOSE"));
  delay(2000);
}

String contentJson(String api_key,float suhu,float berat,boolean lampu){
//  StaticJsonDocument<192> doc;
//  doc["api_key"] = api_key;
//  doc["suhu"] = suhu;
//  doc["berat"] = berat;
//  doc["lampu"] = lampu;
//  
//  // Add an array.
////  JsonArray data = doc.createNestedArray("data");
////  data.add(48.756080);
////  data.add(2.302038);
//  String test;
//  serializeJsonPretty(doc, test);
  String test="{\"api_key\":\""+api_key+"\",\"suhu\":"+String(suhu)+",\"berat\":"+String(berat)+",\"lampu\":"+String(lampu)+"}";
  return test;
}

void testGSM(){
  while(true){
     // memastikan pembacaan dari SIM800 dan dikirim datanya ke Arduino(Serial Monitor)
    if (SIM900.available())
    { c = SIM900.read();
      Serial.write(c);}
 
    // memastikan pembacaan dari SIM800 dan dikirim datanya ke Arduino(Serial Monitor)
    if (Serial.available())
    { c = Serial.read();
      SIM900.write(c);  
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

void postReq(String url,String api_key,float suhu,float berat,boolean lampu){
  String content=contentJson(api_key,suhu,berat,lampu);
//  Serial.println(content);
  GSMCommand("AT+SAPBR=0,1",3000,3000);
  GSMCommand("AT+SAPBR=3,1,\"Contype\",\"GPRS\"");
  GSMCommand("AT+SAPBR=3,1,\"APN\",\"internet\"");
  GSMCommand("AT+SAPBR=1,1",3000,3000);
  GSMCommand("AT+HTTPINIT");
  GSMCommand("AT+HTTPPARA=\"CID\",1");
  GSMCommand("AT+HTTPPARA=\"URL\",\""+url+"\"");
  GSMCommand("AT+HTTPPARA=\"CONTENT\",\"application/json\"");
  GSMCommand("AT+HTTPDATA=" + String(content.length()) + ",10000");
  GSMCommand(content);
  GSMCommand("AT+HTTPACTION=1",7000,5000);
}
void getReq(String url){
//  String data="{\"Test\":\"HI\"}";
  GSMCommand("AT+SAPBR=0,1",3000,3000);
  GSMCommand("AT+SAPBR=3,1,\"Contype\",\"GPRS\"");
  GSMCommand("AT+SAPBR=3,1,\"APN\",\"internet\"");
  GSMCommand("AT+SAPBR=1,1",5000,5000);
  GSMCommand("AT+HTTPINIT");
  GSMCommand("AT+HTTPPARA=\"CID\",1");
  GSMCommand("AT+HTTPPARA=\"URL\",\""+url+"\"");
  GSMCommand("AT+HTTPACTION=0",7000,5000);
}

void loop()
{
  // getReq(getURL);
  postReq(postURL,api_key,11,900,1);
  delay(1000);
}
