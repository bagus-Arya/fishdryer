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
  delay(1000);
  Serial.println("Set Tare and Waiting To Enter char 't'");
  while(true){
    if (Serial.available()) {
      char temp = Serial.read();
      if(temp == 't'){
        scale.tare();
        Serial.println("Tare Set~");
        break;
      }
    }
  }
  Serial.println("-------------------------------------------------------------------------------");
  Serial.println("Setting Button Configuration : ");
  Serial.println("Increase calibration factor a,s,d,f => 10,100,1000,10000");
  Serial.println("Decrease calibration factor z,x,c,v => 10,100,1000,10000");
  Serial.println("Press 'm' to show menu");
  Serial.println("Press 'q' to show a value");
  Serial.println("Press 'w' to toogle repeatedly view value");
  Serial.println("Press 't' to Reset Tare");
  Serial.println("-------------------------------------------------------------------------------");
  Serial.println("");
  
//  long zero_factor = scale.read_average();
//  Serial.print("Zero factor: ");
//  Serial.println(zero_factor);
  delay(2000);
}

void loop() {
  scale.set_scale(calibration_factor);
  GRAM = scale.get_units(), 4;

  if(showVal!=false){
    Serial.print("Reading: ");
    Serial.print(GRAM);
    Serial.print(" Gram");
    Serial.print(" calibration_factor: ");
    Serial.print(calibration_factor);
    Serial.println();
  }
    

  if (Serial.available()) {
    char temp = Serial.read();
    if (temp == '+' || temp == 'a')
      calibration_factor += 0.1;
    else if (temp == '-' || temp == 'z')
      calibration_factor -= 0.1;
    else if (temp == 's')
      calibration_factor += 10;
    else if (temp == 'x')
      calibration_factor -= 10;
    else if (temp == 'd')
      calibration_factor += 100;
    else if (temp == 'c')
      calibration_factor -= 100;
    else if (temp == 'f')
      calibration_factor += 1000;
    else if (temp == 'v')
      calibration_factor -= 1000;
    else if (temp == 't'){
      scale.tare();
      Serial.println("Tare Set~");
    }
    else if (temp == 'm'){
      Serial.println("-------------------------------------------------------------------------------");
      Serial.println("Setting Button Configuration : ");
      Serial.println("Increase calibration factor a,s,d,f => 10,100,1000,10000");
      Serial.println("Decrease calibration factor z,x,c,v => 10,100,1000,10000");
      Serial.println("Press 'm' to show menu");
      Serial.println("Press 'q' to show a value");
      Serial.println("Press 'w' to toogle repeatedly view value");
      Serial.println("Press 't' to Reset Tare");
      Serial.println("-------------------------------------------------------------------------------");
      Serial.println("");
    }
      
    else if (temp == 'q'){
      Serial.print("Reading: ");
      Serial.print(GRAM);
      Serial.print(" Gram");
      Serial.print(" calibration_factor: ");
      Serial.print(calibration_factor);
      Serial.println();
    }
      
    else if (temp == 'w'){
      if(showVal==true){
        showVal=false;
        Serial.println("Toogle:Off");
      }
        
      else
        showVal=true;
        Serial.println("Toogle:On");
    }
      
  }
}
