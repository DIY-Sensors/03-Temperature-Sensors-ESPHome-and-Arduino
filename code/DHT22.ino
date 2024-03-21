 /*
 *     Project    : DHT22 to Home Assistant by MQTT
 *     Versie     : 1.0
 *     Datum      : 03-2024
 *     Schrijver  : Ap Matteman
 *
 *     Versie 1.0 : basic 
 *     
 */    

#include <ESP8266WiFi.h>         // For WiFi
#include <PubSubClient.h>        // For MQTT
#include <DHT.h>                 // For DHT22 sensor
#include "arduino_secrets.h"     // My Passwords

// DHT22 Sensor
#define DHTTYPE DHT22
uint8_t DHTPin = D5; // DTH22 sensor - Inside temp & humidity
DHT dht(DHTPin, DHTTYPE);

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

  pinMode(DHTPin, INPUT);
  dht.begin();            

  //Connect to WiFi and MQTT
  WiFi.begin(ssid, password);
  Connect2WiFi();
  MQTTclient.setServer(mqtt_broker, mqtt_port);
  Connect2MQTT();

}

void loop() {
  // put your main code here, to run repeatedly:
  float Temperature;
  float Humidity;
  delay(5000);  // Wait 5 seconds
  
  Temperature = dht.readTemperature(); // Gets the values of the temperature
  Humidity = dht.readHumidity(); // Gets the values of the humidity
  // Writing to serial port 
  Serial.println();
  Serial.print("Temperature: "); Serial.print(Temperature); Serial.print(" *C   ");
  Serial.print("Humidity: "); Serial.print(Humidity); Serial.print(" %");
  MQTTclient.publish("MQTT/Test/Temperature", String(Temperature).c_str());
  MQTTclient.publish("MQTT/Test/Humidity", String(Humidity).c_str());



}
