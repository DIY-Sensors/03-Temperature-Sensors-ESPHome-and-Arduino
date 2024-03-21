 /*
 *     Project    : BME680
 to Home Assistant by MQTT
 *     Versie     : 1.0
 *     Datum      : 03-2024
 *     Schrijver  : Ap Matteman
 *
 *     Versie 1.0 : basic 
 *     
 */

#include <ESP8266WiFi.h>         // For WiFi
#include <PubSubClient.h>        // For MQTT
#include <Wire.h>                // For the BME680
#include <SPI.h>                 // For the BME680
#include <Adafruit_Sensor.h>     // For the BME680
#include <Adafruit_BME680.h>     // For the BME680
#include "arduino_secrets.h"     // My Passwords

#define SEALEVELPRESSURE_HPA (1013.25)


// Connecting:
// Board:                         SDA         SCL
//   ESP8266 ESP-01:..............GPIO0/D5    GPIO2/D3
//   NodeMCU 1.0, WeMos D1 Mini...GPIO4/D2    GPIO5/D1


// For the BME680 sensor
Adafruit_BME680 BME680; // I2C




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


 if (!BME680.begin()) {
    Serial.println("Couldn't find sensor!");
    while (1);
  }          

  // Set up oversampling and filter initialization for the BME680
  BME680.setTemperatureOversampling(BME680_OS_8X);
  BME680.setHumidityOversampling(BME680_OS_2X);
  BME680.setPressureOversampling(BME680_OS_4X);
  BME680.setIIRFilterSize(BME680_FILTER_SIZE_3);
  BME680.setGasHeater(320, 150); // 320*C for 150 ms

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
  float Pressure;
  float Gas;
  float Altitude;
  delay(5000);  // Wait 5 seconds
 
  Temperature = BME680.temperature;
  Humidity = BME680.humidity;
  Pressure = BME680.pressure / 100.0;
  Gas = BME680.gas_resistance;
  Altitude = BME680.readAltitude(SEALEVELPRESSURE_HPA);

  // Writing to serial port 
  Serial.println();
  Serial.print("Temperature: "); Serial.print(Temperature); Serial.print(" *C   ");
  Serial.print("Humidity: "); Serial.print(Humidity); Serial.print(" %  ");
  Serial.print("Pressure: "); Serial.print(Pressure); Serial.print(" hPa  ");
  Serial.print("Gas: "); Serial.print(Gas); Serial.print(" KOhms  ");
  Serial.print("Altitude: "); Serial.print(Altitude); Serial.print(" m  ");
  MQTTclient.publish("MQTT/Test/Temperature", String(Temperature).c_str());
  MQTTclient.publish("MQTT/Test/Humidity", String(Humidity).c_str());
  MQTTclient.publish("MQTT/Test/Pressure", String(Pressure).c_str());
  MQTTclient.publish("MQTT/Test/Gas", String(Gas).c_str());
  MQTTclient.publish("MQTT/Test/Altitude", String(Altitude).c_str());

}
