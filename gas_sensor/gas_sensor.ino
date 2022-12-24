
#include <WiFi.h>
#include <Wire.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>

#define ANALOG 32
#define DIGITAL 2
#define THRESHOLD 500

#define NOTE_A4  440 //nota LA4
#define BUZZER 18 //Conecto el pin 18 al buzzer

//const char* ssid = "MiFibra-BD86";
//const char* password =  "L4r93473";

const char* ssid = "Mitelf";
const char* password =  "qwertyuiop";

int gas;

int measureGas(){
  int m = analogRead(ANALOG);

  if(m>THRESHOLD ){
    tone(BUZZER, NOTE_A4, 500);
  }
  Serial.print("\nGas Sensor: ");
  Serial.print(m);
  return m;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(230400); 
  pinMode(ANALOG, INPUT);
  pinMode(BUZZER, OUTPUT); //Configuro el pin 18 como salida para el buzzer 
  
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
    gas = measureGas();

    StaticJsonDocument<200> doc;

    doc["measure"] = "gas_sensor";
    doc["gas"] = gas;
    String json_string;
  
    serializeJson(doc, json_string);

    Serial.print(json_string);

    HTTPClient http;
  
    //http.begin("http://192.168.125.39:5000/sensor_values");   //"http://10.118.68.110:5000/sensor_values"
    http.begin("http://192.168.158.114:5000/sensor_values");
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


}
