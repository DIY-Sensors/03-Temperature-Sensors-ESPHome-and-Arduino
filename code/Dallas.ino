 /*
 *     Project    : Dallas DS18B20 to Home Assistant by MQTT
 *     Versie     : 1.0
 *     Datum      : 03-2024
 *     Schrijver  : Ap Matteman
 *
 *     Versie 1.0 : basic 
 *     
 */    

#include <ESP8266WiFi.h>        // For WiFi
#include <PubSubClient.h>       // For MQTT
#include <OneWire.h>            // For the Dallas sensor
#include <DallasTemperature.h>  // For the Dallas sensor
#include "arduino_secrets.h"    // My Passwords

// Dallas sensor
#define ONE_WIRE_BUS D6
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// WiFi
const char *ssid = YourSSID; // Enter your Wi-Fi name
const char *password = YourWiFiPassWord;  // Enter Wi-Fi password

// MQTT Broker
const char *mqtt_broker = YourMQTTserver;
const char *mqtt_username = YourMQTTuser;
const char *mqtt_password = YourMQTTpassword;
const int mqtt_port = 1883;
int WiFiTry = 0;                // nr of times the WiFi is not available
int MQTTtry = 0;                // nr of times the MQTT is not available
WiFiClient espClient;
PubSubClient MQTTclient(espClient);


void Connect2WiFi() {
  //Connect with WiFi network
  WiFiTry = 0;
   WiFi.begin(ssid, password);
   Serial.println("Connecting to WiFi");
  //Try to connect to WiFi for 11 times
  while (WiFi.status() != WL_CONNECTED && WiFiTry < 11) {

    ++WiFiTry;
    // Serial.print("WiFi status: ");; Serial.println(WiFi.status());
    // Serial.print("WiFiTry: ");; Serial.println(WiFiTry);
    delay(1000);
  }
  Serial.println("");
  Serial.print("WiFiTry: ");; Serial.println(WiFiTry);
  Serial.print("Got IP: ");  Serial.println(WiFi.localIP());

}

void Connect2MQTT(){
  //connecting to a mqtt broker
  MQTTtry = 0;
  while ((!MQTTclient.connected() && MQTTtry < 16)) {
      String client_id = "esp32-client-";
      client_id += String(WiFi.macAddress());
      Serial.printf("The client %s connects to the public MQTT broker\n", client_id.c_str());
      if (MQTTclient.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
          Serial.println("Public EMQX MQTT broker connected");
          break;
      } else {
          Serial.print("failed with state ");
          Serial.print(MQTTclient.state());
          delay(2000);
      } 
      ++MQTTtry;
      Serial.print("#");
  }
   // Subscribe, voor iedere Topic een regel. Wildcards werken (momenteel) niet :(
  MQTTclient.subscribe("lora/testlights/power");
  Serial.println("MQTT = Connected");
}

void setup() {
  // put your setup code here, to run once:
  // Set software serial baud to 115200;
  Serial.begin(115200);

  // Dallas sensor
  sensors.begin();         

  //Connect to WiFi and MQTT
  WiFi.begin(ssid, password);
  Connect2WiFi();
  MQTTclient.setServer(mqtt_broker, mqtt_port);
  Connect2MQTT();

}

void loop() {
  // put your main code here, to run repeatedly:
  float Temperature;

  delay(5000);  // Wait 5 seconds
  sensors.requestTemperatures(); 
  Temperature = sensors.getTempCByIndex(0); // For Celcius
  //            sensors.getTempFByIndex(0)  // For Fahrenheit  

  // Writing to serial port 
  Serial.println();
  Serial.print("Temperature: "); Serial.print(Temperature); Serial.print(" *C   ");
  MQTTclient.publish("MQTT/Test/Temperature", String(Temperature).c_str());


}
