#include <WiFi.h>
//#include <WiFiUDP.h>

//WiFiUDP udp;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(230400);
  WiFi.mode(WIFI_MODE_STA);
  Serial.print("Mac Address: ");
  Serial.println(WiFi.macAddress());
  IPAddress ip = WiFi.localIP();

}

void loop() {
// put your main code here, to run repeatedly:
//  udp.beginPacket(host, port);
//  udp.print("hello world");
//  udp.endPacket();
}
