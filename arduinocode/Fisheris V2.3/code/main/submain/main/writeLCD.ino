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
