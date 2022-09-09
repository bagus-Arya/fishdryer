#include <LiquidCrystal_I2C.h>
#include <BH1750.h>
//#include <Wire.h> 



LiquidCrystal_I2C lcd(0x27, 20, 4);
BH1750 lightMeter;

void setup()
{
  //  Wire.begin();
	lcd.begin();
  lightMeter.begin();
	lcd.backlight();
  lcd.setCursor(1,0);//colom,baris
	lcd.print("Hello, world!");
}

void loop()
{
  lcd.clear();
  float lux = lightMeter.readLightLevel();
  lcd.setCursor(1,0);//colom,baris
  lcd.print("Lux:"+String(lux)+" lx");
	// Do nothing here...
}
