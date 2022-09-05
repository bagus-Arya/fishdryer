void GSMpowerOn()
{
  digitalWrite(GSMPowerKey,LOW);
  delay(1000);
  digitalWrite(GSMPowerKey,HIGH);
  delay(2000);
  digitalWrite(GSMPowerKey,LOW);
  delay(3000);
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
