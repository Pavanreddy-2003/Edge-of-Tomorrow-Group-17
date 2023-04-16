#include <SoftwareSerial.h>
#include<LiquidCrystal.h>
#include <Adafruit_BMP085.h>
#include<Wire.h>
#include <dht.h>
#include <String.h>
#define dht1 A0  
#define gas_sensor A1
#define soil A2
#define ldr A3
dht DHT;
Adafruit_BMP085 bmp;
SoftwareSerial mySerial(12,13);
LiquidCrystal lcd(2,3,4,5,6,7);
int gas_value,temp,humi,water,light,pressure;
String Api_Key= "OQ2R5QXDQKTFP0IS";
 
void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  lcd.begin(16,2);
  bmp.begin();
  lcd.setCursor(0,0);
  lcd.print("Agriculture"); 
  lcd.setCursor(0,1);
  lcd.print("Monitoring System"); 
  Serial.println("Iot based Agriculture MOnitoring System");
  delay(1000);
  lcd.clear();
}
void loop() {
  pressure=bmp.readPressure();
  gas_value=analogRead(gas_sensor);
  DHT.read11(dht1);
  temp=DHT.temperature;
  humi=DHT.humidity;
  water=analogRead(soil);
  light=analogRead(ldr);
  Serial.print("Presuure:");
  Serial.println(pressure);
  Serial.print("Gas:");
  Serial.println(gas_value);
  Serial.print("TEMP:");
  Serial.println(temp);
  Serial.print("Humi:");
  Serial.println(humi);
  Serial.print("Soil:");
  Serial.println(water);
  Serial.print("Light");
  Serial.println(light);
 lcd.setCursor(0,0);
 lcd.print("Temp:"+String(temp)+"");
 lcd.setCursor(9,0); 
 lcd.print("Hum:"+String(humi)+"");
 lcd.setCursor(0,1);
 lcd.print("Soil:"+String(water)+"");
 lcd.setCursor(10,1) ;
 lcd.print("Gas:"+String(gas_value)+"");
 delay(3000);
 Send2thing();
 lcd.clear();
 lcd.home();
 lcd.setCursor(0,0);
 lcd.print("Light:"+String(light)+"");
 lcd.setCursor(0,1);
 lcd.print("Pressure:"+String(pressure)+"");
  if(temp>20)
  {
    sendMessage1();
  }
  if(water>400)
  {
    sendMessage2();
  }
  if(gas_value>250)
  {
    sendMessage3();
  }
}
void sendMessage1()
{
  mySerial.println("AT+CMGF=1");//set the GSM Module in Text mode
  delay(1000);
  ShowSerialData();
  mySerial.println("AT+CMGS=\"+917204670614\"\r");
  delay(1000);
  ShowSerialData();
  mySerial.println("Alert,Temperature HIGH");//the SMS text you want to send
  delay(100);
  ShowSerialData();
  mySerial.println((char)26);
  delay(100);
 
}
void sendMessage2()
{
  mySerial.println("AT+CMGF=1");//set the GSM Module in Text mode
  delay(1000);
  ShowSerialData();
  mySerial.println("AT+CMGS=\"+917204670614\"\r");
  delay(1000);
  ShowSerialData();
  mySerial.println("Alert,Water level LOW");//the SMS text you want to send
  delay(100);
  ShowSerialData();
  mySerial.println((char)26);
  delay(100);
 
}
void sendMessage3()
{
  mySerial.println("AT+CMGF=1");//set the GSM Module in Text mode
  delay(1000);
  ShowSerialData();
  mySerial.println("AT+CMGS=\"+917204670614\"\r");
  delay(1000);
  ShowSerialData();
  mySerial.println("Alert,Gas level HIGH");//the SMS text you want to send
  delay(100);
  ShowSerialData();
  mySerial.println((char)26);
  delay(100);
 
}
void Send2thing()
{
  mySerial.println("AT");
  delay(1000);
 
  mySerial.println("AT+CPIN?");
  delay(1000);
 
  mySerial.println("AT+CREG?");
  delay(1000);
 
  mySerial.println("AT+CGATT?");
  delay(1000);
 
  mySerial.println("AT+CIPSHUT");
  delay(1000);
 
  mySerial.println("AT+CIPSTATUS");
  delay(2000);
 
  mySerial.println("AT+CIPMUX=0");
  delay(2000);
 
  ShowSerialData();
 
  mySerial.println("AT+CSTT=\"airtelgprs.com\"");//start task and setting the APN
  delay(1000);
 
  ShowSerialData();
 
  mySerial.println("AT+CIICR");//bring up wireless connection
  delay(3000);
 
  ShowSerialData();
 
  mySerial.println("AT+CIFSR");//get local IP adress
  delay(2000);
 
  ShowSerialData();
 
  mySerial.println("AT+CIPSPRT=0");
  delay(3000);
 
  ShowSerialData();
 
  mySerial.println("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",\"80\"");//start up the connection
  delay(5000);
 
  ShowSerialData();
 
  mySerial.println("AT+CIPSEND");//begin send data to remote server
  ShowSerialData();
  delay(500);
  String str="GET http://api.thingspeak.com/update?api_key=8JZRE6S2CLYOSW13&field1=" + String(temp);
  str+="&field2=";
  str+=String(humi);
  str+="&field3=";
  str+=String(gas_value);
  str+="&field4=";
  str+=String(water);
  str+="&field5=";
  str+=String(light);
  str+="&field6=";
  str+=String(pressure);
  Serial.println(str);
  mySerial.println(str);//begin send data to remote server
  delay(500);
  ShowSerialData();
 
  mySerial.println((char)26);//sending
  delay(2000);//waitting for reply, important! the time is base on the condition of internet 
  mySerial.println();
 
  ShowSerialData();
 
  mySerial.println("AT+CIPSHUT");//close the connection
  delay(100);
  ShowSerialData();
  Serial.println("   Try again");
}
void ShowSerialData()
{
  while(mySerial.available()!=0)
    Serial.write(mySerial.read());
}