#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Stepper.h> //libreria para el motor

int stepsPerRevolution = 18;
Stepper stepperA(stepsPerRevolution, A1, A2, A3, A4);// pines
Stepper stepperB(stepsPerRevolution, A5, A6, A7, A8);// pines
#define VELOCIDAD 300
int botonAbrir = 52;
int botonCerrar = 53;
int Disponible = 16;
int Ocupado = 0;

int t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15, t16 = 0;

//configuración ic2
#define LCD_RS 2
#define LCD_RW 1
#define LCD_E  0
#define LCD_D4 4
#define LCD_D5 5
#define LCD_D6 6
#define LCD_D7 7
#define LCD_BACKLIGHT 3
#define BOCINA 2
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

int angulo = 0;
char estado, estado1 ;
void abrirSalida() {
  stepperA.setSpeed(40);
  stepperA.step(stepsPerRevolution / 4);
  delay(4000);
  stepperA.step(-stepsPerRevolution / 4);
}
void abrirEntrada() {
  stepperB.setSpeed(40);
  stepperB.step(stepsPerRevolution / 4);
  delay(4000);
  stepperB.step(-stepsPerRevolution / 4);
}


void setup() {
  lcd.begin(LCD_COLS, LCD_ROWS); //16x4

  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Menu Informacion");
  pinMode(7, OUTPUT);
  pinMode(siguiente, INPUT);
  pinMode(anterior, INPUT);
  pinMode(enter, INPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(A4, OUTPUT);
  pinMode(botonAbrir, INPUT);
  pinMode(botonCerrar, INPUT);
  pinMode(53, INPUT);
  pinMode(9, OUTPUT);
  pinMode(A5, OUTPUT);
  pinMode(A6, OUTPUT);
  pinMode(A7, OUTPUT);
  pinMode(A8, OUTPUT);
  pinMode(BOCINA, OUTPUT);

  for (int i = 22; i <= 37; i++) {
    pinMode(i, INPUT);
  }

  for (int i = 38; i <= 53; i++) {
    pinMode(i, OUTPUT);
  }

  lcd.createChar(0, Carrito1);
  lcd.createChar(1, Carrito2);

  lcd.createChar(2, image02);
  lcd.createChar(3, image03);
  lcd.createChar(4, image01);
  lcd.createChar(5, image42);
  lcd.createChar(6, image43);

  Serial.begin(9600); //BLUETOOTH
  Serial1.begin(9600); //API

  if (digitalRead(22) == HIGH ) {
    t1 = 1;
  }
  if (digitalRead(23) == HIGH ) {
    t2 = 1;
  }
  if (digitalRead(24) == HIGH ) {
    t3 = 1;
  }
  if (digitalRead(25) == HIGH ) {
    t4 = 1;
  }
  if (digitalRead(26) == HIGH ) {
    t5 = 1;
  }
  if (digitalRead(27) == HIGH ) {
    t6 = 1;
  }
  if (digitalRead(28) == HIGH ) {
    t7 = 1;
  }
  if (digitalRead(29) == HIGH ) {
    t8 = 1;
  }
  if (digitalRead(30) == HIGH ) {
    t9 = 1;
  }
  if (digitalRead(31) == HIGH ) {
    t10 = 1;
  }
  if (digitalRead(32) == HIGH ) {
    t11 = 1;
  }
  if (digitalRead(33) == HIGH ) {
    t12 = 1;
  }
  if (digitalRead(34) == HIGH ) {
    t13 = 1;
  }
  if (digitalRead(35) == HIGH ) {
    t14 = 1;
  }
  if (digitalRead(36) == HIGH ) {
    t15 = 1;
  }
  if (digitalRead(37) == HIGH ) {
    t16 = 1;
  }


  Menu();

}
void loop() {
  if (Serial.available() > 0) {
    estado = char(Serial.read());

    if (estado == '1') {
      abrirSalida();
    }
    else if (estado == '0') {
      abrirEntrada();
    }
  }

  if (Serial1.available() > 0) {
    Reservado();
  }


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
  Estacionar();
  verif();
  Reservado();
}

void Reservado() {
  /*estado1 = char(Serial.read());
    if (estado1 == 'A') {
    digitalWrite(38, HIGH);
    delay(5000);
    digitalWrite(38, LOW);
    Serial1.println('0');
    }
    if (estado1 == 'B') {
    digitalWrite(39, HIGH);
    delay(5000);
    digitalWrite(39, LOW);
    }
    if (estado1 == 'C') {
    digitalWrite(40, HIGH);
    delay(5000);
    digitalWrite(40, LOW);
    }
    if (estado1 == 'D') {
    digitalWrite(41, HIGH);
    delay(5000);
    digitalWrite(41, LOW);
    }
    if (estado1 == 'E') {
    digitalWrite(42, HIGH);
    delay(5000);
    digitalWrite(42, LOW);
    }
    if (estado1 == 'F') {
    digitalWrite(43, HIGH);
    delay(5000);
    digitalWrite(43, LOW);
    }
    if (estado1 == 'G') {
    digitalWrite(44, HIGH);
    delay(5000);
    digitalWrite(44, LOW);
    }
    if (estado1 == 'H') {
    digitalWrite(45, HIGH);
    delay(5000);
    digitalWrite(45, LOW);
    }
    if (estado1 == 'I') {
    digitalWrite(46, HIGH);
    delay(5000);
    digitalWrite(46, LOW);
    }
    if (estado1 == 'J') {
    digitalWrite(47, HIGH);
    delay(5000);
    digitalWrite(47, LOW);
    }
    if (estado1 == 'K') {
    digitalWrite(48, HIGH);
    delay(5000);
    digitalWrite(48, LOW);
    }
    if (estado1 == 'L') {
    digitalWrite(49, HIGH);
    delay(5000);
    digitalWrite(49, LOW);
    }
    if (estado1 == 'M') {
    digitalWrite(50, HIGH);
    delay(5000);
    digitalWrite(50, LOW);
    }
    if (estado1 == 'N') {
    digitalWrite(51, HIGH);
    delay(5000);
    digitalWrite(51, LOW);
    }
    if (estado1 == 'O') {
    digitalWrite(52, HIGH);
    delay(5000);
    digitalWrite(52, LOW);
    }
    if (estado1 == 'P') {
    digitalWrite(53, HIGH);
    delay(5000);
    digitalWrite(53, LOW);
    }
    if (estado1 == 'Q') {
    digitalWrite(2, HIGH);
    }
    if (estado1 == 'R') {
    digitalWrite(2, LOW);
    }*/
  char ReaderFromNode; // Store current character
  ReaderFromNode = (char) Serial1.read();
  digitalWrite(39,HIGH);
  if(ReaderFromNode=='o'){
    digitalWrite(38, HIGH);
    delay(100); 
  }
  if(ReaderFromNode=='f'){
    digitalWrite(38, LOW);
    delay(100); 
  }
  
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
  int x = 0;
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
      lcd.print(String(Disponible) + " parqueos");
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
      lcd.print(String(Ocupado) + " parqueos");
      delay(1200);
      break;
  }
}

void Estacionar() {
  Disponible = 16;
  Ocupado = 0;
  for (int i = 22; i <= 37; i++) {
    if (digitalRead(i) == HIGH) {
      Disponible--;
      Ocupado++;
    }
  }
}

void verif() {
  if (t1 != digitalRead(22)) {
    if (t1 == 0) {
      Serial1.println('A');
      t1 = 1;
    }
    else {
      Serial1.println('a');
      t1 = 0;
    }
  }
  if (t2 != digitalRead(23)) {
    if (t2 == 0) {
      Serial1.println('B');
      t2 = 1;
    }
    else {
      Serial1.println('b');
      t2 = 0;
    }
  }
  if (t3 != digitalRead(24)) {
    if (t3 == 0) {
      Serial1.println('C');
      t3 = 1;
    }
    else {
      Serial1.println('c');
      t3 = 0;
    }
  }
  if (t4 != digitalRead(25)) {
    if (t4 == 0) {
      Serial1.println('D');
      t4 = 1;
    }
    else {
      Serial1.println('d');
      t4 = 0;
    }
  }
  if (t5 != digitalRead(26)) {
    if (t5 == 0) {
      Serial1.println('E');
      t5 = 1;
    }
    else {
      Serial1.println('e');
      t5 = 0;
    }
  }
  if (t6 != digitalRead(27)) {
    if (t6 == 0) {
      Serial1.println('F');
      t6 = 1;
    }
    else {
      Serial1.println('f');
      t6 = 0;
    }
  }
  if (t7 != digitalRead(28)) {
    if (t7 == 0) {
      Serial1.println('G');
      t7 = 1;
    }
    else {
      Serial1.println('g');
      t7 = 0;
    }
  }
  if (t8 != digitalRead(29)) {
    if (t8 == 0) {
      Serial1.println('H');
      t8 = 1;
    }
    else {
      Serial1.println('h');
      t8 = 0;
    }
  }
  if (t9 != digitalRead(30)) {
    if (t9 == 0) {
      Serial1.println('I');
      t9 = 1;
    }
    else {
      Serial1.println('i');
      t9 = 0;
    }
  }
  if (t10 != digitalRead(31)) {
    if (t10 == 0) {
      Serial1.println('J');
      t10 = 1;
    }
    else {
      Serial1.println('j');
      t10 = 0;
    }
  }
  if (t11 != digitalRead(32)) {
    if (t11 == 0) {
      Serial1.println('K');
      t11 = 1;
    }
    else {
      Serial1.println('k');
      t11 = 0;
    }
  }
  if (t12 != digitalRead(33)) {
    if (t12 == 0) {
      Serial1.println('L');
      t12 = 1;
    }
    else {
      Serial1.println('l');
      t12 = 0;
    }
  }
  if (t13 != digitalRead(34)) {
    if (t13 == 0) {
      Serial1.println('M');
      t13 = 1;
    }
    else {
      Serial1.println('m');
      t13 = 0;
    }
  }
  if (t14 != digitalRead(35)) {
    if (t14 == 0) {
      Serial1.println('N');
      t14 = 1;
    }
    else {
      Serial1.println('n');
      t14 = 0;
    }
  }
  if (t15 != digitalRead(36)) {
    if (t15 == 0) {
      Serial1.println('O');
      t15 = 1;
    }
    else {
      Serial1.println('o');
      t15 = 0;
    }
  }
  if (t16 != digitalRead(37)) {
    if (t16 == 0) {
      Serial1.println('P');
      t16 = 1;
    }
    else {
      Serial1.println('p');
      t16 = 0;
    }
  }

}
