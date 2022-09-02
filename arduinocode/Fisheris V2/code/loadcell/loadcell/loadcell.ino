#include "HX711.h"
#define DOUT  A1
#define CLK  A0
HX711 scale(DOUT, CLK);
float calibration_factor = 31.41;
int GRAM;
boolean showVal=false;
void setup() {
  Serial.begin(9600);
  Serial.println("Set Default Calibration Factor : " +String(calibration_factor));
  scale.set_scale(calibration_factor);
  scale.tare();
  delay(1000);
  Serial.println(F("Set Tare......."));
  Serial.println(F("In 5......."));
  delay(1000);
  Serial.println(F("In 4......."));
  delay(1000);
  Serial.println(F("In 3......."));
  delay(1000);
  Serial.println(F("In 2......."));
  delay(1000);
  Serial.println(F("In 1......."));
  delay(1000);
  Serial.println(F("Tare Set"));
  scale.tare();
}

void loop() {
  scale.set_scale(calibration_factor);
  GRAM = scale.get_units(), 4;
  Serial.print("Reading: ");
  Serial.print(GRAM);
  Serial.print(" Gram");
  Serial.print(" calibration_factor: ");
  Serial.print(calibration_factor);
  Serial.println();
  delay(100);
}
