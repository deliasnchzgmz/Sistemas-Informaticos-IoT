/*
  Asignatura: Sistemas Informáticos en IoT
  Practica6 - Prueba 1. Estructura JSON en ESP32
  Funcionamiento: prueba básica con estructura de datos JSON en ESP32
  David Martín Gómez
  dmgomez@ing.uc3m.es
*/

#include <ArduinoJson.h>

float temperature, pressure, altitude, humidity;

void setup()
{
  Serial.begin(230400);

}

void loop() {

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

  String json_string, json_string_pretty;

  serializeJson(doc, json_string);

  serializeJsonPretty(doc, json_string_pretty);


  Serial.println(json_string);  
  Serial.println("          -----           ");  
  Serial.println(json_string_pretty);

  delay(3000);
}
