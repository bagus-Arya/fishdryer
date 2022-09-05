void GSMpowerOn()
{
  digitalWrite(GSMPowerKey,LOW);
  delay(1000);
  digitalWrite(GSMPowerKey,HIGH);
  delay(2000);
  digitalWrite(GSMPowerKey,LOW);
  delay(3000);
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
  
  clearLCD(0,1,1,1);
  Serial.println(F(""));
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