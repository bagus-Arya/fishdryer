void postReq(String content){
  GSMCommand("AT+SAPBR=0,1",3000,3000);
  GSMCommand("AT+SAPBR=3,1,\"Contype\",\"GPRS\"");
  GSMCommand("AT+SAPBR=3,1,\"APN\",\"internet\"");
  GSMCommand("AT+SAPBR=1,1",3000,3000);
  GSMCommand("AT+HTTPINIT");
  GSMCommand("AT+HTTPPARA=\"CID\",1");
  GSMCommand("AT+HTTPPARA=\"URL\",\""+postUrl+"\"",3000,2000);
  GSMCommand("AT+HTTPPARA=\"CONTENT\",\"application/json\"");
  GSMCommand("AT+HTTPDATA=" + String(content.length()) + ",10000");
  GSMCommand(content);
  GSMCommandOutput checkOutput = GSMCommand("AT+HTTPACTION=1",7000,5000);
  clearLCD(0,0,0,1);
  if(checkOutput.status!=0){
    
    writeLCD("-Data Sent-",3,5);
  }else{
    writeLCD("-Failed to Send-",3,2);
  }
  GSMCommand("AT+HTTPREAD",3000,3000);
}