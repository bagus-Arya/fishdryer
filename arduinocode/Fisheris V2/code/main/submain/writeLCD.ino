//custom char
const byte tempIcon[] = {
  B00100,
  B01010,
  B01010,
  B01010,
  B01010,
  B10101,
  B10001,
  B01110
};
const byte weightIcon[] = {
  B11111,
  B01110,
  B00100,
  B01110,
  B11001,
  B10101,
  B10001,
  B01110
};
const byte signalIcon[] = {
  B00000,
  B00001,
  B00001,
  B00011,
  B00111,
  B01111,
  B11111,
  B11111
};
const byte luxIcon[] = {
  B00000,
  B00000,
  B10101,
  B01110,
  B11011,
  B01110,
  B10101,
  B00000
};
const byte relayIcon[] = {
  B00000,
  B01010,
  B01010,
  B01010,
  B11111,
  B11111,
  B01110,
  B00100
};

void writeLCD(String msg,int row=0,int collum=0){
  lcd.setCursor(collum,row);//collum,row
  lcd.print(msg);
}

void writeLCDIcon(char *icon,int row=0,int collum=0){
  int id;
  if(icon=="tempIcon"){
    id=0;
    lcd.createChar(id, tempIcon);
    lcd.setCursor(collum,row);//collum,row
    lcd.write(id);

  }  
 else if(icon=="weightIcon"){
    id=1;
    lcd.createChar(id, weightIcon);
    lcd.setCursor(collum,row);//collum,row
    lcd.write(id);

 }
 else if(icon=="signalIcon"){
    id=2;
    lcd.createChar(id, signalIcon);
    lcd.setCursor(collum,row);//collum,row
    lcd.write(id);

 }
 else if(icon=="luxIcon"){
    id=3;
    lcd.createChar(id, luxIcon);
    lcd.setCursor(collum,row);//collum,row
    lcd.write(id);

 }
 else if(icon=="relayIcon"){
    id=4;
    lcd.createChar(id, relayIcon);
    lcd.setCursor(collum,row);//collum,row
    lcd.write(id);
 }
}