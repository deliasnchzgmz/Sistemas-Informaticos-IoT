#define ANALOG 4
#define DIGITAL 2
#define THRESHOLD 800



#define NOTE_A4  440 //nota LA4
#define BUZZER 18 //Conecto el pin 18 al buzzer

void measureGas(){
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
  Serial.print("Preheating...\n");
  delay(20000);
}

void loop() {

  measureGas();
  delay(5000);

}
