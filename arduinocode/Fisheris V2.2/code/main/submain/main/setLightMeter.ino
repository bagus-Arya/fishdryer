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