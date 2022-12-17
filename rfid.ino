// La librería SPI.h nos permitirá establecer la comunicación entre la tarjeta ESP32 y el módulo.
//y la librería MFRC522.h nos permitirá dialogar con el módulo

//SDA --> G5
//SCK --> G18
//MOSI --> G23
//MISO --> G19
//GND --> GND
//RST --> G27
//3V3 --> 3V3

#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 5
#define RST_PIN 27

//Variables
byte nuidPICC[4] = {0, 0, 0, 0};
String content = "";

MFRC522::MIFARE_Key key;
MFRC522 rfid = MFRC522(SS_PIN, RST_PIN);

void setup() {
  //Init Serial USB
  Serial.begin(230400);
  SPI.begin();
  rfid.PCD_Init();

  Serial.println("Tap an RFID/NFC tag on the RFID-RC522 reader");
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

void loop() {  ////Read RFID card
  /*for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
  // Look for new 1 cards
  if ( ! rfid.PICC_IsNewCardPresent())
    return;
  // Verify if the NUID has been readed
  if (  !rfid.PICC_ReadCardSerial())
    return;
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
  Serial.println(valores);
  Serial.println();
  valores = "";*/
  
  content = readRFID();
  if(content.equals("")){
    return;
  }else{
    Serial.println(content);
    Serial.println();
    content = "";
  }
}
