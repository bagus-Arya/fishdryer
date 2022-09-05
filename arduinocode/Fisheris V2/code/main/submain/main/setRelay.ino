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
