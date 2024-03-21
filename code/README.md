<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
</head>
<body>
  <h1>Temperature and Humidity Sensor Code</h1>
  
  <h2>Arduino Code</h2>
  
  <p>In the Arduino code, we utilize an arduino_secrets.h file to securely store our WiFi passwords and MQTT server connections. The code is structured with separate subroutines for connecting to WiFi and MQTT. While it's common practice to include WiFi and MQTT connection checks in the loop, for simplicity, these checks have been omitted in this example.</p>
  
  <p>All sensor readings are logged to the serial monitor for debugging purposes and published to Home Assistant via MQTT for real-time monitoring and automation.</p>
  
  <h3>Usage</h3>
  
  <p>Ensure you have the necessary libraries installed and update the arduino_secrets.h file with your WiFi and MQTT credentials before uploading the code to your Arduino board.</p>
  
  <hr>
  
  <h2>ESPHome Code</h2>
  
  <p>The ESPHome code provided is a basic configuration to enable the sensors to function. It includes the necessary components to interface with the sensors and publish the readings to Home Assistant.</p>
  
  <h3>Usage</h3>
  
  <p>Simply copy the ESPHome code into your ESPHome configuration file and upload it to your ESP8266 or ESP32 device. Make sure to adjust the configuration to match your specific sensor setup if necessary.</p>
  
  <hr>
  
  <p>Happy coding!</p>
  
</body>
</html>
