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