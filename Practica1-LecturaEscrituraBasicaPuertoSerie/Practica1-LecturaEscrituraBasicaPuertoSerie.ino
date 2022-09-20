/*
  Asignatura: Sistemas Informáticos en IoT
  Práctica 1 - Lectura - escritura básica por puerto serie
  Funcionamiento: escribir un número en el PC y ver su valor ASCII en Decimal
  David Martín Gómez
  dmgomez@ing.uc3m.es
*/
byte incomingByte = 0;

void setup() {
  Serial.begin(230400); //configuro el puerto a una velocidad (baudrate)
}

void loop() {
  if (Serial.available() > 0 )
  {
    // read the incoming Byte
    incomingByte = Serial.read();
    Serial.print("Value: ");
    Serial.println(incomingByte, DEC);
  }
}
