/*
  Asignatura: Sistemas Informáticos en IoT
  Practica6 - Prueba 2. Método POST en cliente ESP32
  Funcionamiento: prueba de método POST en cliente ESP32 para envío de datos a servidor Flask en PC
  David Martín Gómez
  dmgomez@ing.uc3m.es

  GND -> GND
  3.3 -> 3.3
  SDA (verde) -> G21
  SCL (azul) -> G22
*/

#include <WiFi.h>
#include <Wire.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include "SparkFunBME280.h"
#include <SPI.h>
#include <MFRC522.h>

BME280 mySensor;

#define ANALOG 32
#define THRESHOLD 500

#define NOTE_A4  440 //nota LA4
#define BUZZER 17 //Conecto el pin 17 al buzzer

#define ANALOG 32
#define DIGITAL 2
#define THRESHOLD 800
#define NOTE_A4  440 //nota LA4
#define BUZZER 17 //Conecto el pin 17 al buzzer
#define SS_PIN 5
#define RST_PIN 27
#define LED_RFID 14

//Variables RFID
byte nuidPICC[4] = {0, 0, 0, 0};
MFRC522::MIFARE_Key key;
MFRC522 rfid = MFRC522(SS_PIN, RST_PIN);
String rfid_key = "";

//const char* ssid = "DIGIFIBRA-bdD7";
//const char* password =  "xTPy4PHuY3";
const char* ssid = "Mitelf";
const char* password =  "qwertyuiop";

float temperature, pressure, humidity;
int gas;

WiFiClient wifi;

int measureGas() {
  int m = analogRead(ANALOG);

  if (m > THRESHOLD ) {
    tone(BUZZER, NOTE_A4, 500);
  }

  return m;
}

String readRFID() {
  String valores = "";
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
  // Look for new 1 cards
  if ( ! rfid.PICC_IsNewCardPresent()){
    return "";
  }
  // Verify if the NUID has been readed
  if (  !rfid.PICC_ReadCardSerial())
    return "";
  // Store NUID into nuidPICC array
  for (byte i = 0; i < 4; i++) {
    nuidPICC[i] = rfid.uid.uidByte[i];
  }
  for (byte i = 0; i < rfid.uid.size; i++) {
    valores.concat(String(rfid.uid.uidByte[i] < 0x10 ? " 0" : " "));
    valores.concat(String(rfid.uid.uidByte[i], DEC));
  }
  // Halt PICC
  rfid.PICC_HaltA();
  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
  return valores;
}

void setup()
{
  Serial.begin(230400);
  //Serial.println("Reading basic values from BME280");
  Wire.begin();
  pinMode(LED_RFID, OUTPUT);

  if (mySensor.beginI2C() == false) //Begin communication over I2C
  {
    Serial.println("The sensor did not respond. Please check wiring.");
    while (1); //Freeze
  }

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

  //Init RFID
  SPI.begin();
  rfid.PCD_Init();

}

void loop() {

  if (WiFi.status() == WL_CONNECTED) {

    // Read Sensors
    temperature = mySensor.readTempC();
    pressure = mySensor.readFloatPressure();
    humidity = mySensor.readFloatHumidity();
    gas = measureGas();
    rfid_key = readRFID();
    if (rfid_key.isEmpty()){
      digitalWrite(LED_RFID, LOW);
    }else{
      digitalWrite(LED_RFID, HIGH);
    }

    delay(50);

    StaticJsonDocument<200> doc;

    doc["measure"] = "proyecto_final";
    doc["temperature"] = temperature;
    doc["pressure"] = pressure;
    doc["humidity"] = humidity;
    doc["gas"] = gas;
    doc["rfid"] = rfid_key;

    String json_string;

    serializeJson(doc, json_string);

    Serial.print(json_string);

    HTTPClient http;

    //http.begin("http://192.168.125.39:5000/sensor_values");   //"http://10.118.68.110:5000/sensor_values"
    http.begin("http://192.168.21.114:5000/sensor_values");
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
  delay(500);
}
