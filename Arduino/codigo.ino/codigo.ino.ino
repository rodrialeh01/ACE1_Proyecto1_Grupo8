#include <LiquidCrystal.h>
#include <Stepper.h> //libreria para el motor
int stepsPerRevolution = 5;
Stepper stepperA(stepsPerRevolution, A1, A2, A3, A4);// pines
#define COLS 16
#define ROWS 4
#define VELOCIDAD 300
LiquidCrystal lcd(2, 3, 8, 9, 10, 11);
int botonAbrir = 52;
int botonCerrar = 53;

void verificar(){
  /*if (digitalRead(botonAbrir) == LOW) {
    stepperA.setSpeed(10);
    stepsPerRevolution = 5;
      stepperA.step(stepsPerRevolution);

  }else if (digitalRead(botonCerrar) == LOW){
    stepperA.setSpeed(10);
    delay(1500);
    stepsPerRevolution = 5;
    stepperA.step(-stepsPerRevolution);
  }*/
  stepperA.setSpeed(10);
   stepperA.step(stepsPerRevolution/4);
   delay(1500);
   stepperA.step(-stepsPerRevolution/4);
}
void setup()
{
  lcd.begin(COLS,ROWS);
  lcd.print("INFORMACION ----");
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A4, OUTPUT);
  pinMode(botonAbrir, INPUT);
  pinMode(botonCerrar, INPUT);
}

void loop()
{
 
  verificar();
  
   
}
