/*
  Asignatura: Sistemas Informáticos en IoT
  Práctica 1 - Ejercicio con servomotor: solución
  Enunciado del ejercicio en la transparencia 17 de la práctica 1 
  David Martín Gómez            
  dmgomez@ing.uc3m.es                                                                                                                                                            
*/

#include <Servo.h>

Servo servo;
int servoPin = 4;

byte incomingByte = 0;
int number = 0;
String total;

// Check limit values
int checkLimits(int min_val, int max_val, int value)
{
  if (value <= min_val)
    return min_val;
  if (value >= max_val)
    return max_val;
  return value;
}

void setup() {
  Serial.begin(230400);
  servo.attach(servoPin);//attach servoPin, which is Pin 4
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
    if (incomingByte == '\n')
    {
      number = total.toInt();
      Serial.print("Value: ");
      Serial.println(number);
      // clear the string for new input
      total = "";
    }
  }
  servo.write(checkLimits(0, 180, number));
  sleep(5);
}
