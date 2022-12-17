/*
  Asignatura: Sistemas Informáticos en IoT
  Practica6 - Prueba 2. Método POST en cliente ESP32
  Funcionamiento: prueba de método POST en cliente ESP32 para envío de datos a servidor Flask en PC
  David Martín Gómez
  dmgomez@ing.uc3m.es

//TEMP
  GND -> GND
  3.3 -> 3.3
  SDA (verde) -> G21
  SCL (azul) -> G22

//GAS
//vcc --> 5v
//GND --> GND
//AD --> G32

//RFID
//SDA --> G5
//SCK --> G18
//MOSI --> G23
//MISO --> G19
//GND --> GND
//RST --> G27
//3V3 --> 3V3


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
#define DIGITAL 2
#define THRESHOLD 800
#define NOTE_A4  440 //nota LA4
#define BUZZER 18 //Conecto el pin 18 al buzzer
#define SS_PIN 5
#define RST_PIN 27

//const char* ssid = "UC3M-LABS";
//const char* password =  "Uc3M.L4b.2020";

//const char* ssid = "AndroidMARTA";
//const char* password =  "Anacris66";

const char* ssid = "ONO4948,";
const char* password =  "HakunaMatata7777";

//Variables RFID
byte nuidPICC[4] = {0, 0, 0, 0};

MFRC522::MIFARE_Key key;
MFRC522 rfid = MFRC522(SS_PIN, RST_PIN);


float temperature, altitude, pressure, humidity;
String rfid_key = "";
int gas;


WiFiClient wifi;

int measureGas(){
  int m = analogRead(ANALOG);

  if(m>THRESHOLD ){
    tone(BUZZER, NOTE_A4, 500);
  }
  Serial.print("\nGas Sensor: ");
  Serial.print(m);
  return m;
}

String readRFID(){
  String valores = "";
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
  // Look for new 1 cards
  if ( ! rfid.PICC_IsNewCardPresent())
    return "";
  // Verify if the NUID has been readed
  if (  !rfid.PICC_ReadCardSerial())
    return "";
  // Store NUID into nuidPICC array
  for (byte i = 0; i < 4; i++) {
    nuidPICC[i] = rfid.uid.uidByte[i];
  }
  Serial.print(F("RFID In dec: "));
  for (byte i = 0; i < rfid.uid.size; i++){
    valores.concat(String(rfid.uid.uidByte[i] < 0x10 ? " 0" : " "));
    valores.concat(String(rfid.uid.uidByte[i],DEC));
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
  Serial.println("Reading basic values from BME280");
  pinMode(ANALOG, INPUT);
  pinMode(BUZZER, OUTPUT); //Configuro el pin 18 como salida para el buzzer 
  Serial.print("Preheating...\n");
  delay(20000);
  Wire.begin();
/*
  if (mySensor.beginI2C() == false) //Begin communication over I2C
  {
    Serial.println("The sensor did not respond. Please check wiring.");
    while(1); //Freeze
  }*/

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

  Serial.println("Tap an RFID/NFC tag on the RFID-RC522 reader");
}

void loop() {

  if (WiFi.status() == WL_CONNECTED) {

    // Read Sensors
    temperature = 27.7;
    //temperature = mySensor.readTempC();
    pressure = 12.3;
    //pressure = mySensor.readFloatPressure();
    altitude = 615;
    //altitude = mySensor.readFloatAltitudeFeet();
    humidity = 7.09;
    //humidity = mySensor.readFloatHumidity();

    gas = measureGas();

    rfid_key = readRFID();
    Serial.println(rfid_key);
    
    //delay(50);

    StaticJsonDocument<200> doc;

    doc["measure"] = "BME280";
    doc["temperature"] = temperature;
    doc["pressure"] = pressure;
    doc["altitude"] = altitude;
    doc["humidity"] = humidity;
    doc["gas"] = gas;

    if(rfid_key.equals("")){
      doc["keys"] = "no key";
    }else{
      doc["keys"] = rfid_key;
      rfid_key = "";
    }

    String json_string;
  
    serializeJson(doc, json_string);

    Serial.print(json_string);

    HTTPClient http;
  
    //http.begin("http://192.168.125.39:5000/sensor_values");   //"http://10.118.68.110:5000/sensor_values"
    http.begin("http://192.168.0.27:5000/sensor_values");
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
