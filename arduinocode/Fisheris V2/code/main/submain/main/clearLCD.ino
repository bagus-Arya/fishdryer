void clearLCD(){
  lcd.clear();
}

void clearLCD(int row,int collumStart,int collumEnd){
  for(int i=collumStart;i<=collumEnd;i++){
    lcd.setCursor(i,row);
    lcd.print(" ");
  }
}

void clearLCD(boolean row1,boolean row2,boolean row3,boolean row4){
  if(row1){
    lcd.setCursor(0,0);
    lcd.print("                    ");
  }
  if(row2){
    lcd.setCursor(0,1);
    lcd.print("                    ");
  }
  if(row3){
    lcd.setCursor(0,2);
    lcd.print("                    ");
  }
  if(row4){
    lcd.setCursor(0,3);
    lcd.print("                    ");
  }
}