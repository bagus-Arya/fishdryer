#include <ArduinoJson.h>
HardwareSerial ESPSerial(1);
long currentTime;
long lastSentTime;
const int sendDelay=10000;
struct GSMCommandOutput{
   String inputCom;
   bool status;
   String outputCom;
};

struct location{
  float latitude;
  float longitude;
};

const String deviceId="GPS001";
const String postUrl="http://ptsv2.com/t/7vvz7-1662298931/post";


GSMCommandOutput GSMCommand(String commmand,int timeoutProcess=500,int charLastReceivedTimeout=500){
  Serial.println(F(""));
  Serial.println(F("---------------------------------------------------------------------------------------------------------------------------"));
  Serial.println(F("-GSM COMMANDS-"));
  while(ESPSerial.available()){
    ESPSerial.read();
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

  ESPSerial.println(commmand);
  while(true){
    while(ESPSerial.available()){
      GSMCharReading[GSMCharPos]=ESPSerial.read();
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

  if(result.indexOf("AT+GPS=11")!=-1 && result.indexOf("OK")!=-1){
    output.status=true;
  }
  else if (result.indexOf("AT+LOCATION=2")!=-1 && result.indexOf("OK")!=-1)
  {
     output.status=true;
  }
  else{
      output.status=false;
  }
  Serial.println("Input Command : " +output.inputCom);
  Serial.println("Status : " +String(output.status));
  Serial.println(F("Output Command ["));
  Serial.println(output.outputCom);
  Serial.println(F("]"));
  Serial.println(F("-GSM COMMANDS END-"));
  Serial.println(F("---------------------------------------------------------------------------------------------------------------------------"));
  Serial.println(F(""));
  return output;
}
String serilizeJson(location cordinate,String device=deviceId){
  StaticJsonDocument<60> doc;
  
  JsonObject location = doc.createNestedObject("location");
  location["latitude"] = cordinate.latitude;
  location["longitude"] = cordinate.longitude;

  String serilize;
  serializeJson(doc, serilize);
  serilize.replace("\"", "\\22");
  return serilize;
}

void postRequest(String content,String url=postUrl){
  GSMCommand("AT+CGATT=1",4000,4000);
  GSMCommand("AT+CGDCONT=1,\"IP\",\"internet\"");
  GSMCommand("AT+CGACT=1,1",3000,3000);
  GSMCommand("AT+HTTPPOST=\""+url+"\",\"application/json\",\""+content+"\"",9000,9000);
}

location getLocation(){
  GSMCommand("AT+GPS=1",300,300);
  GSMCommandOutput gsmResult=GSMCommand("AT+LOCATION=2",500,500);
  int n1=gsmResult.outputCom.indexOf('\n');
  int n2=gsmResult.outputCom.indexOf('\n',n1+1);
  location cordinate;
  if(gsmResult.status){
    String fetch=gsmResult.outputCom.substring(n2+1,gsmResult.outputCom.indexOf('\n',n2+1)-1);
    cordinate.latitude=fetch.substring(0,fetch.indexOf(',')-1).toFloat();
    cordinate.longitude=fetch.substring(fetch.indexOf(',')+1,fetch.length()-1).toFloat();
//    Serial.println("n1:"+String(n1)+"|char:"+gsmResult.outputCom[n1]);
//    Serial.println("n2:"+String(gsmResult.outputCom.indexOf('\n',n2+1))+"|char:"+gsmResult.outputCom[n2]);
//    Serial.println("Latitude:"+cordinate.latitude);
//    Serial.println("Longitude:"+cordinate.longitude);
      postRequest(serilizeJson(cordinate));
  }
  else{
    cordinate.latitude=0;
    cordinate.longitude=0;
  }
  return cordinate;
}



void setup() {
  Serial.begin(115200); //Menggunakan komunikasi serial pada 9600bps
  delay(100);
  Serial.println(F(""));
  Serial.println(F("-Setup Device-"));
  
  ESPSerial.begin(115200,SERIAL_8N1,16,17);  
  delay(100);
  
  Serial.println(F("Checking A9G Module......"));
  while(true){
//    GSMpowerOn();
    ESPSerial.println("AT");
    delay(300);
    String result=ESPSerial.readString();
    if(result.indexOf("OK")!=-1){
      break;
    }
  }
  
  Serial.println(F("-Complete-"));
  Serial.println(F(""));
}

void loop() {
    location cordinate=getLocation();
    Serial.println("Latitude:"+String(cordinate.latitude,6));
    Serial.println("Longitude:"+String(cordinate.longitude,6));
//  Serial.println(F("-Complete-"));
//  Serial.println(F(""));
  delay(1000);
}
