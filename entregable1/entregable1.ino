/*
  Get basic environmental readings from the BME280
  By: Nathan Seidle
  SparkFun Electronics
  Date: March 9th, 2018
  License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).

  Feel like supporting our work? Buy a board from SparkFun!
  https://www.sparkfun.com/products/14348 - Qwiic Combo Board
  https://www.sparkfun.com/products/13676 - BME280 Breakout Board
  
  This example shows how to read humidity, pressure, and current temperature from the BME280 over I2C.

  Hardware connections:
  BME280 -> Arduino
  GND -> GND
  3.3 -> 3.3
  SDA -> A4
  SCL -> A5
*/

#include <WiFi.h>
#include <Wire.h>
#include <Servo.h>
#include "SparkFunBME280.h"
BME280 mySensor;
Servo servo;
int servoPin = 4;

//const char* ssid = "UC3M-LABS"; //conectar tarjetaESP32 y PC a la misma red
//const char* password =  "Uc3M.L4b.2020"; //conectar tarjetaESP32 y PC a la misma red

const char* ssid = "DIGIFIBRA-bdD7";
const char* password =  "xTPy4PHuY3";

WiFiServer server(80);

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
  
  Serial.println("Reading basic values from BME280");
  servo.attach(servoPin);
  
  Wire.begin();

  if (mySensor.beginI2C() == false) //Begin communication over I2C
  {
    Serial.println("The sensor did not respond. Please check wiring.");
    while(1); //Freeze
  }

    // Inicio del servidor de datos en tarjeta ESP32
  server.begin();
}

void loop()
{

  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,

    double temp_min = 25;
    double temp_actual = mySensor.readTempC();
    double temp_max = 32;
    double angulo = (180/(temp_max-temp_min))*(temp_actual-temp_min);
    servo.write(angulo);
    
    Serial.println("New Client.");          // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<table>");
              client.println("<tr>");
                client.println("<td>");
                 client.println("Presi&oacute;n");
                client.println("</td>");
                
                client.println("<td>");                
                 client.println(mySensor.readFloatPressure(), 0);                                 
                 client.println(" hPa");
                client.println("</td>");
              client.println("</tr>");

              client.println("<tr>");
                client.println("<td>");
                 client.println("Temperatura");
                client.println("</td>");
                
                client.println("<td>");                
                 client.println(mySensor.readTempC(), 2);                                 
                 client.println(" &ordm;C");
                client.println("</td>");
              client.println("</tr>");

              client.println("<tr>");
                client.println("<td>");
                 client.println("Altitud");
                client.println("</td>");
                
                client.println("<td>");                
                 client.println(mySensor.readFloatAltitudeMeters(), 1);                                 
                 client.println(" m");
                client.println("</td>");
              client.println("</tr>");

              client.println("<tr>");
                client.println("<td>");
                 client.println("Humedad");
                client.println("</td>");
                
                client.println("<td>");                
                 client.println(mySensor.readFloatHumidity(), 0);                                 
                 client.println(" %");
                client.println("</td>");
              client.println("</tr>");

              client.println("<tr>");
                client.println("<td>");
                 client.println("&Aacute;ngulo servomotor");
                client.println("</td>");
                
                client.println("<td>");                
                 client.println(angulo, 1);                                 
                 client.println(" &ordm;");
                client.println("</td>");
              client.println("</tr>");
            client.println("</table>");
            client.println("</html>");
            
            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}
