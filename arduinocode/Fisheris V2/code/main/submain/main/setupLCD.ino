void setupLCD(){
  Serial.println(F(""));
  Serial.println(F("-SET UP LCD-"));
  lcd.begin();
  lcd.backlight();
  clearLCD();
  Serial.println(F("Setup Complete"));
  Serial.println(F(""));
}