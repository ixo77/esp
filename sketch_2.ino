#include <OneWire.h>
#include <DallasTemperature.h>
#include <SPI.h>
#include <Wire.h>
#include <math.h>
//#include <ESP8266WiFi.h>
//#include "Adafruit_MQTT.h"
//#include "Adafruit_MQTT_Client.h"
//#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
//#define ONE_WIRE_BUS 10 // номер пина к которому подключен DS18B20
/************************* WiFi Access Point *********************************/
/*
#define WLAN_SSID       "esp" //Wi-Fi- сеть 
#define WLAN_PASS       "98805060"  // пароль сети

/************************* Adafruit.io Setup *********************************/
/*
#define AIO_SERVER      "io.adafruit.com"       // адрес сервера mqtt
#define AIO_SERVERPORT  1883                   // порт сервера
#define AIO_USERNAME    "...your AIO username (see https://accounts.adafruit.com)..." //логин устройства
#define AIO_KEY         "...your AIO key..."  //праоль (для закрытых подключений)

/************ Global State (you don't need to change this!) ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
/*WiFiClient client;
// or... use WiFiFlientSecure for SRT
//WiFiClientSecure client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/****************************** Feeds ***************************************/
/*
// Setup a feed called 'photocell' for publishing.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
Adafruit_MQTT_Publish photocell = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/photocell");

// Setup a feed called 'onoff' for subscribing to changes.
Adafruit_MQTT_Subscribe onoffbutton = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/onoff");



*/



#define TEMPERATURE_PRECISION 12 //разрядность измерения

#define OLED_RESET 13
Adafruit_SSD1306 display(OLED_RESET);

#define ONE_WIRE_BUS_1 14 // левый терм
#define ONE_WIRE_BUS_2 12 // правый терм
#define ONE_WIRE_BUS_3 10 // мокрый терм

//#define PIN_FF 16 // реле нагрева
#define PIN_LL 16 // реле вентелятора

OneWire oneWire_left(ONE_WIRE_BUS_1);
OneWire oneWire_right(ONE_WIRE_BUS_2);
OneWire oneWire_wet(ONE_WIRE_BUS_3);

DallasTemperature sensor_left(&oneWire_left);
DallasTemperature sensor_right(&oneWire_right);
DallasTemperature sensor_wet(&oneWire_wet);

float LT; // текущая температура левого датчика
float RT; // температура сухого грудусника
float WT; // разница температур сухого и мокрого
float RTW;
float TT; // разница температур правого и левого термометров
float mm;
float wet; // влажность воздуха
String vod;
void setup()
{
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  sensor_left.begin();
  sensor_right.begin();
  sensor_wet.begin();
  mm=1;
//pinMode(PIN_FF, OUTPUT);
pinMode(PIN_LL, OUTPUT);
/*
    WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
*/
}
void loop()
{
  Serial.setDebugOutput(true);
  display.clearDisplay();
  sensor_left.requestTemperatures();
  sensor_right.requestTemperatures();
  sensor_wet.requestTemperatures();
 
LT = sensor_left.getTempCByIndex(0);
RTW = sensor_wet.getTempCByIndex(0);
RT = sensor_right.getTempCByIndex(0) ;
WT = RT-RTW;
wet = (((pow(10,(10.79574*(1-273.16/(273.15+RT-WT))-5.028*log10((273.15+RT-WT)/273.16)+1.50475*pow(10,-4)*(1-pow(10,-8.2969*((273.15+RT-WT)/273.16-1)))+0.42873*pow(10,-3)*(pow(10,(4.76955*(1-273.16/(273.15+RT-WT))))-1)+0.78614)))-795*pow(10,-6)*1000*(RT-(RT-WT))*(1+0.00115*(RT-WT)))/(pow(10,(10.79574*(1-273.16/(273.15+RT))-5.028*log10((273.15+RT)/273.16)+1.50475*pow(10,-4)*(1-pow(10,-8.2969*((273.15+RT)/273.16-1)))+0.42873*pow(10,-3)*(pow(10,(4.76955*(1-273.16/(273.15+RT))))-1)+0.78614))))*100 ;

TT = RT - LT;
TT = fabs(TT);
if (TT >= 2) {
  if (digitalRead(PIN_LL)==LOW) {
    digitalWrite(PIN_LL, HIGH);
  }
else if (digitalRead(PIN_LL)==HIGH) {
  digitalWrite(PIN_LL, LOW);
}
}
 


  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Temp 1");
  display.setTextColor(WHITE); // 'inverted' text
  display.println(String(RT , 1));
  display.println("Temp vot");
  display.println(String(RTW , 1));
  display.println("VOD");
  display.println(String(wet , 1));
  display.setCursor(40, 0);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println("Temp 2");
  display.setCursor(40, 8);  
  display.println(String(LT , 1));
  display.setCursor(40, 32);
  display.println(String(TT , 1));
    
  display.display();

  if (mm == 1) {
    mm=2;
      display.setCursor(50, 50);
  display.setTextSize(1);
    display.println(":");
  }  
  else 
   mm=1;
          display.setCursor(50, 50);
  display.setTextSize(1);
    display.println("");
 
  delay(250);

  
}
