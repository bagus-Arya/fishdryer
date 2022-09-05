float calibration_factor = 31.41; //HX711 CALIBRATION FACTOR

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