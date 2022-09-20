/*
  Asignatura: Sistemas Informáticos en IoT
  Práctica 2 - Tarjeta ESP32 configurada como servidor
  Funcionamiento: conexión básica a una red WiFi, configuración como servidor y conexión de clientes
  David Martín Gómez
  dmgomez@ing.uc3m.es
*/

#include <WiFi.h>
#include <Servo.h>

Servo servo;
int servoPin = 4;

// Check limit values
int checkLimits(int min_val, int max_val, int value)
{
  if (value <= min_val)
    return min_val;
  if (value >= max_val)
    return max_val;
  return value;
}

const char* ssid = "UC3M-LABS"; // usuario universidad
const char* password = "Uc3M.L4b.2020"; // contraseña universidad

WiFiServer server(80);

void setup()
{
  Serial.begin(230400);
  servo.attach(servoPin);//attach servoPin, which is Pin 4
  Serial.println();
  Serial.print("Connecting to network: ");
  Serial.println(ssid);
  WiFi.disconnect(true);  //disconnect form wifi to set new wifi connection
  WiFi.mode(WIFI_STA); //init wifi mode
  
  WiFi.begin(ssid,password); //connect to wifi
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address set: ");
  Serial.println(WiFi.localIP()); //print LAN IP
  
  server.begin();
}

void loop() {
  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("New Client.");          // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        //Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.print("Click <a href=\"/H\">here</a> to print in the Serial Monitor: Hello World<br>");
            client.print("Click <a href=\"/M\">here</a> to print in the Serial Monitor: I am learning<br>");
            client.print("Click <a href=\"/L\">here</a> to print in the Serial Monitor: Internet of Things<br>");


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

        // Check to see if the client request was "GET /H" or "GET /M" or "GET /L":
        if (currentLine.endsWith("GET /H")) {
          /*Serial.println("------------");
          Serial.println("------------");
          Serial.println("Hello World");
          Serial.println("------------");
          Serial.println("------------");*/
          servo.write(checkLimits(0, 180, 180));
          sleep(2);
        }
        if (currentLine.endsWith("GET /M")) {
          /*Serial.println("------------");
          Serial.println("------------");
          Serial.println("I am learning");
          Serial.println("------------");
          Serial.println("------------");*/
          servo.write(checkLimits(0, 180, 90));
          sleep(2);
        }
        if (currentLine.endsWith("GET /L")) {
          /*Serial.println("------------");
          Serial.println("------------");
          Serial.println("Internet of Things");
          Serial.println("------------");
          Serial.println("------------");*/
          servo.write(checkLimits(0, 180, 0));
          sleep(2);
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}
