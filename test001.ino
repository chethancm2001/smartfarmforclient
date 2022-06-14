#include "DHT.h"
#include <ESP8266WiFi.h>

DHT dht = DHT(D1, DHT11); 
const int sensor_pin = A0;
const char *ssid =  "miniproject";     // replace with your wifi ssid and wpa2 key
const char *pass =  "miniproject";
const char *server = "api.thingspeak.com";
String apiKey = "PP7IAXNKATEUD1MF"; 
WiFiClient client;

void setup()
{
Serial.begin(9600);
pinMode(A0,INPUT);
dht.begin();
Serial.println("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) 
     {
            delay(500);
            Serial.print(".");
     }
      Serial.println("");
      Serial.println("WiFi connected");

}

void loop()
{

delay(5000);
float moisture_percentage;
moisture_percentage = ( 100.00 - ( (analogRead(sensor_pin)/1023.00) * 100.00 ) );

float humidity = dht.readHumidity(); 
float temperature = dht.readTemperature(); 

Serial.print("Humidity=");
Serial.print(humidity);
Serial.print("  ");
Serial.print("temperature=");
Serial.print(temperature);
Serial.print("    ");
Serial.print("mositure of soil  = ");
Serial.print(moisture_percentage);  
Serial.print("   ");

if (isnan(humidity) || isnan(temperature)) 
                 {
                     Serial.println("Failed to read from DHT sensor!");
                      return;
                 }
 
                         if (client.connect(server,80))   //   "184.106.153.149" or api.thingspeak.com
                      {  
                            
                             String postStr = apiKey;
                             postStr +="&field1=";
                             postStr += String(temperature);
                             postStr +="&field2=";
                             postStr += String(humidity);
                             postStr +="&field3=";
                             postStr += String(moisture_percentage);
                             postStr += "\r\n\r\n";
 
                             client.print("POST /update HTTP/1.1\n");
                             client.print("Host: api.thingspeak.com\n");
                             client.print("Connection: close\n");
                             client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
                             client.print("Content-Type: application/x-www-form-urlencoded\n");
                             client.print("Content-Length: ");
                             client.print(postStr.length());
                             client.print("\n\n");
                             client.print(postStr);
 
                             Serial.print("Temperature: ");
                             Serial.print(temperature);
                             Serial.print(" degrees Celcius, Humidity: ");
                             Serial.print(humidity);
                             Serial.println("%. Send to Thingspeak.");
                             Serial.print("soil moisture");
                             Serial.println(moisture_percentage);
                        }
          client.stop();
 
          Serial.println("Waiting...");





}
