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

#include <Wire.h>
#include <Servo.h>
#include "SparkFunBME280.h"
BME280 mySensor;
Servo servo;
int servoPin = 4;

void setup()
{
  Serial.begin(230400);
  Serial.println("Reading basic values from BME280");
  servo.attach(servoPin);
  
  Wire.begin();

  if (mySensor.beginI2C() == false) //Begin communication over I2C
  {
    Serial.println("The sensor did not respond. Please check wiring.");
    while(1); //Freeze
  }
}

void loop()
{
  double temp_min = 26;
  double temp_actual = mySensor.readTempC();
  double temp_max = 32;
  
  Serial.print("Temp min: ");
  Serial.print(temp_min);

  Serial.print(" Temp_actual: ");
  Serial.print(temp_actual);

  Serial.print(" Temp max");
  Serial.print(temp_max);

  double angulo = (180/(temp_max-temp_min))*(temp_actual-temp_min);

  Serial.print(" Ángulo: ");
  Serial.print(angulo);
  Serial.println();

  servo.write(angulo);

  delay(50);
  sleep(5);
}
