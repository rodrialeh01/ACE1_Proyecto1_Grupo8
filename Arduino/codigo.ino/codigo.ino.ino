#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Stepper.h> //libreria para el motor

int stepsPerRevolution = 5;
Stepper stepperA(stepsPerRevolution, A1, A2, A3, A4);// pines
#define VELOCIDAD 300
int botonAbrir = 52;
int botonCerrar = 53;

//configuración ic2
#define LCD_RS 2
#define LCD_RW 1
#define LCD_E  0
#define LCD_D4 4
#define LCD_D5 5
#define LCD_D6 6
#define LCD_D7 7
#define LCD_BACKLIGHT 3

//dimensiones
#define LCD_COLS 16
#define LCD_ROWS 4

//se define que es simulado
#define LCD_I2C_ADDRESS 0x20

//para el menu
int seleccion = 1;
//pines para el menu:
int siguiente = 13;
int anterior = 12;
int enter = 11;

// Objecto lcd
LiquidCrystal_I2C lcd(LCD_I2C_ADDRESS , LCD_RS, LCD_RW, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7, LCD_BACKLIGHT, POSITIVE);

//Para animaciones:
byte Carrito1[8] = {
  B00000,
  B00000,
  B00000,
  B01110,
  B11111,
  B11111,
  B01010,
  B00000
};

byte Carrito2[8] = {
  B00000,
  B00000,
  B01110,
  B11111,
  B11111,
  B01010,
  B00000,
  B00000
};

byte image02[8] = {B11111, B11100, B11100, B11111, B11111, B11111, B10001, B10001};
byte image03[8] = {B00000, B10000, B11110, B11101, B11111, B11111, B01000, B10000};
byte image01[8] = {B00000, B00001, B00111, B00111, B01111, B01111, B00010, B00001};
byte image42[8] = {B00000,
                   B00000,
                   B01110,
                   B11001,
                   B10101,
                   B10011,
                   B01110,
                   B00000
                  };
byte image43[8] = {
  B01110,
  B11001,
  B10101,
  B10011,
  B01110,
  B00000,
  B00000,
  B00000
};

void verificar() {
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
  stepperA.step(stepsPerRevolution / 4);
  delay(1500);
  stepperA.step(-stepsPerRevolution / 4);
}

void setup() {
  lcd.begin(LCD_COLS, LCD_ROWS); //16x4

  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Menu Informacion");
  pinMode(siguiente, INPUT);
  pinMode(anterior, INPUT);
  pinMode(enter, INPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A4, OUTPUT);
  pinMode(botonAbrir, INPUT);
  pinMode(botonCerrar, INPUT);

  lcd.createChar(0, Carrito1);
  lcd.createChar(1, Carrito2);

  lcd.createChar(2, image02);
  lcd.createChar(3, image03);
  lcd.createChar(4, image01);
  lcd.createChar(5, image42);
  lcd.createChar(6, image43);

  Menu();

}
void loop() {

  if (digitalRead(siguiente) == 1) {
    seleccion++;
    Menu();
    delay(100);
    while (digitalRead(siguiente));
  }
  else if (digitalRead(anterior) == 1) {
    seleccion--;
    Menu();
    delay(100);
    while (digitalRead(anterior));
  }
  else if (digitalRead(enter) == 1) {
    visualizar();
    Menu();
    delay(100);
    while (digitalRead(enter));
  }

  //verificar();
}

void Menu() {
  switch (seleccion) {
    case 0:
      seleccion = 1;
      break;
    case 1:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Menu Informacion");
      lcd.setCursor(0, 1);
      lcd.print("> Disponibles");
      lcd.setCursor(0, 2);
      lcd.print("Reservados");
      lcd.setCursor(0, 3);
      lcd.print("Ocupados");
      break;
    case 2:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Menu Informacion");
      lcd.setCursor(0, 1);
      lcd.print("Disponibles");
      lcd.setCursor(0, 2);
      lcd.print("> Reservados");
      lcd.setCursor(0, 3);
      lcd.print("Ocupados");
      break;
    case 3:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Menu Informacion");
      lcd.setCursor(0, 1);
      lcd.print("Disponibles");
      lcd.setCursor(0, 2);
      lcd.print("Reservados");
      lcd.setCursor(0, 3);
      lcd.print("> Ocupados");
      break;
    case 4:
      seleccion = 3;
      break;
  }
}

void visualizar() {
  int x = 32;
  switch (seleccion) {
    case 1:
      lcd.clear();
      for (int i = 1; i < 14; i++) {
        lcd.clear();
        lcd.setCursor(i, 1);
        lcd.write(byte(2));
        lcd.setCursor(i + 1, 1);
        lcd.write(byte(3));
        lcd.setCursor(i - 1, 1);
        lcd.write(byte(4));
        delay(200);
      }

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Total disponible");
      lcd.setCursor(0, 2);
      lcd.print(String(x) + " parqueos");
      delay(1200);
      break;
    case 2:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Total reservado");
      lcd.setCursor(0, 2);
      lcd.print(String(x) + " parqueos");
      delay(1200);
      break;
    case 3:
      lcd.clear();
      for (int i = 1; i < 14; i++) {
        lcd.clear();
        lcd.setCursor(i, 1);
        lcd.write(byte(2));
        lcd.setCursor(i + 1, 1);
        lcd.write(byte(3));
        lcd.setCursor(i - 1, 1);
        lcd.write(byte(4));

        if (i % 2 == 0) {
          lcd.setCursor(i + 2, 1);
          lcd.write(byte(5));
        } else {
          lcd.setCursor(i + 2, 1);
          lcd.write(byte(6));
        }


        delay(200);
      }
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Total ocupado");
      lcd.setCursor(0, 2);
      lcd.print(String(x) + " parqueos");
      delay(1200);
      break;
  }
}
