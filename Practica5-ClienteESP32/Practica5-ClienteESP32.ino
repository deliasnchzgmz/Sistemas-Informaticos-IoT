/*
  Asignatura: Sistemas Informáticos en IoT
  Practica4 - Practica5-ClienteESP32
  Funcionamiento: Conexion de cliente desde la tarjeta ESP32 al servidor Flask para envio de datos añadidos en este programa a mano (no medidos desde BME280)
  David Martín Gómez
  dmgomez@ing.uc3m.es
*/

#include <WiFi.h>
#include <HTTPClient.h>
#include "SparkFunBME280.h"
#include <string.h>

BME280 mySensor; // Declaración de mySensor para conexión del sensor BME280 a través de I2C

const char* ssid = "Mitelf";
const char* password =  "qwertyuiop";

WiFiClient wifi;

void setup() {
  
  Serial.begin(230400);
  // Wifi Setup
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address set: ");
  Serial.println(WiFi.localIP()); //print LAN IP

  Wire.begin();
  if (mySensor.beginI2C() == false) //Begin communication over I2C
  {
    Serial.println("The sensor did not respond. Please check wiring.");
    while(1); //Freeze
  }


}

void loop() {

  if (WiFi.status() == WL_CONNECTED) {

    HTTPClient http;

    int  temperature = mySensor.readTempC();
    int altitude = mySensor.readFloatAltitudeMeters();
    float humidity = mySensor.readFloatHumidity();
    int pressure = mySensor.readFloatPressure();
    
    http.begin("http://192.168.91.114:5000/sensor_values?temperature=72&pressure=20&altitude=600&humidity=67");

    //String s = "http://192.168.91.114:5000/sensor_values?temperature="+temperature+"&pressure="+to_string(pressure)+"&altitude="+to_string(altitude)+"&humidity="+string to_string(humidity);
    //http.begin("http://192.168.91.114:5000/sensor_values?temperature="+temperature+"&pressure="+pressure+"&altitude="+altitude+"&humidity="+humidity);
    http.addHeader("Content-Type", "text/plain");

    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(response);
    } else {
      Serial.print("Error on sending GET Request: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  } else {
    Serial.println("Error in WiFi connection");
  }
  delay(1000);
}
