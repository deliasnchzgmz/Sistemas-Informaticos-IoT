/*
  Asignatura: Sistemas Informáticos en IoT
  Practica6 - Prueba 2. Método POST en cliente ESP32
  Funcionamiento: prueba de método POST en cliente ESP32 para envío de datos a servidor Flask en PC
  David Martín Gómez
  dmgomez@ing.uc3m.es
*/

#include <WiFi.h>
#include <Wire.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>

//const char* ssid = "UC3M-LABS";
//const char* password =  "Uc3M.L4b.2020";

const char* ssid = "Mitelf";
const char* password =  "qwertyuiop";

float temperature, altitude, pressure, humidity;

WiFiClient wifi;

void setup()
{
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

}

void loop() {

  if (WiFi.status() == WL_CONNECTED) {

    // Read Sensors
	  temperature = 27.7;
	  pressure = 12.3;
	  altitude = 615;
	  humidity = 7.09;

    StaticJsonDocument<200> doc;

    doc["temperature"] = temperature;
    doc["pressure"] = pressure;
    doc["altitude"] = altitude;
    doc["humidity"] = humidity;

    String json_string;
	
    serializeJson(doc, json_string);

    Serial.print(json_string);

    HTTPClient http;
	
    //http.begin("http://192.168.125.39:5000/sensor_values");   //"http://10.118.68.110:5000/sensor_values"
    http.begin("http://192.168.91.114:5000/sensor_values");
    http.addHeader("Content-Type", "application/json");
    
    int httpResponseCode = http.POST(json_string);
    
	if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(response);
    } else {
      Serial.print("Error on sending POST Request: ");
      Serial.println(httpResponseCode);
    }
    
	http.end();
  
  } else {
    Serial.println("Error in WiFi connection");
  }
  delay(3000);
}
