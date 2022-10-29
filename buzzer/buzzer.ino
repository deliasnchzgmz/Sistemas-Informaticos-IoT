#define NOTE_A4  440 //nota LA4
#define BUZZER 18 //Conecto el pin 18 al buzzer
#define alarmButton 2
volatile bool enable = true;

void sound(bool isEnabled){ // Si el sonido está activado, suena, si no emite sonido
  if (isEnabled){
    //Serial.print("Sound ON \n");
    tone(BUZZER, NOTE_A4, 1000);
  }else{
    tone(BUZZER, 0, 1000);
  }
}

void ISR_enable_sound(void){
  Serial.print("Pressed \n");
  if(enable){
    enable = false;
  }else{
    enable = true;
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(230400); 
  pinMode(BUZZER, OUTPUT); //Configuro el pin 18 como salida para el buzzer
  pinMode(alarmButton, INPUT); //Pin 2 como entrada en modo pullup
  attachInterrupt(digitalPinToInterrupt(alarmButton), ISR_enable_sound, HIGH);
  //Habilito la interrupción para el pin 2, lo conecto con la ISR 
  //HIGH  es para indicar que la interrupción se dispara cuando el pin está a 0
}

void loop() {
  // put your main code here, to run repeatedly:
  sound(enable);
}
