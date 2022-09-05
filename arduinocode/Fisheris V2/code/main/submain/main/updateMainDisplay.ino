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
