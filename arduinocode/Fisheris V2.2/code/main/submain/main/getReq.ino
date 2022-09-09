void getReq(){
//  String data="{\"Test\":\"HI\"}";
  GSMCommand("AT+SAPBR=0,1",3000,3000);
  GSMCommand("AT+SAPBR=3,1,\"Contype\",\"GPRS\"");
  GSMCommand("AT+SAPBR=3,1,\"APN\",\"internet\"");
  GSMCommand("AT+SAPBR=1,1",3000,3000);
  GSMCommand("AT+HTTPINIT");
  GSMCommand("AT+HTTPPARA=\"CID\",1");
  GSMCommand("AT+HTTPPARA=\"URL\",\""+getUrl+"\"");
  GSMCommand("AT+HTTPACTION=0",7000,5000);
  GSMCommand("AT+HTTPREAD",3000,3000);
}