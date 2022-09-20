/*
  Asignatura: Sistemas Informáticos en IoT
  Práctica 1 - Prueba de unión de dígitos en tarjeta ESP32 a través del puerto serie
  Funcionamiento: escribir digitos de un número positivo o negativo en el PC, se envía a la tarjeta ESP32 y se devuelve el número completo al PC
  David Martín Gómez
  dmgomez@ing.uc3m.es
*/

byte incomingByte = 0;
int number = 0;
String total;

void setup() {
  Serial.begin(230400);
}

void loop() {

  // wait till receive data (only one byte)
  while (Serial.available() > 0 )
  {
    // read the incoming Byte
    incomingByte = Serial.read();
	
    // add to string only if it is digit or if it is negative sign
    if (isdigit(incomingByte) || incomingByte == '-')
    {
      total += (char)incomingByte;
    }
	
    // end receiving with \n
    if (incomingByte == '\n') // significado \n --> pulsar tecla intro en PC
    {
      number = total.toInt();
      Serial.print("Value: ");
      Serial.println(number);
      // clear the string for new input
      total = "";
    }
  }
}
