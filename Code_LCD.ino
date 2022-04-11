#include <DS1302.h>
#include <LiquidCrystal.h>
ASLKHDJKAJSHDKJHASDasdasd
#define DELAY_TIME 300
// Chan tin hieu cho LCD 16x2
#define rs 9
#define en 8
#define d4 13
#define d5 12
#define d6 11
#define d7 10

// Chan tin hieu cho DS1302
#define sclk 6
#define dat 5
#define ce 7

// Chan tin hieu cho cam bien nhiet do LM35
#define tempSensor A0

// Chan tin hieu cho loa buzzer
int buzzer = 4;

// Chan tin hieu cho button
int btnOK = 3;
int btnAdj = 2;
int btnMode = 1;

// Bien su dung cho thay doi thoi gian
int h = 0;
int m = 0;
int s = 0;

int mode = 1;
int subMode = 1;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

DS1302 rtc(ce, dat, sclk);

byte degreeChar[] = {
  B00111,
  B00101,
  B00111,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};

//---------------------------------Ham setup-----------------------------------//
void setup() {
  pinMode(btnOK, INPUT_PULLUP);
  pinMode(btnAdj, INPUT_PULLUP);
  pinMode(btnMode, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, LOW);
  lcd.begin(16,2);
  lcd.createChar(0, degreeChar);
  // Thiet lap thoi gian ban dau la 00:00:00
  rtc.setTime(0, 0, 0);
}




//---------------------------------Ham loop-----------------------------------//
void loop() {

  if (digitalRead(btnMode) == LOW) {
    mode++;
    if (mode == 4) {
      mode = 1;
    }
    subMode = 1;
    h = 0, m = 0, s = 0;
    lcd.clear();
    delay(DELAY_TIME);
  }
  if (mode == 1) { // Hien thi clock
    displayClock();
  }
  if (mode == 2) { // Chinh clock
    updateClock();
  }
  if (mode == 3) { // Hen gio
    setAlarm();
  }
}

//-------------------------------------Ham displayClock-------------------------------------//
void displayClock() {
  //Hien thi thoi gian
  lcd.setCursor(0, 0);
  lcd.print("Time: ");
  lcd.print(rtc.getTimeStr());
  //Hien thi nhiet do
  lcd.setCursor(0, 1);
  lcd.print("Temp: ");
  printTemp();
}
int readTemp() {
  int vout = analogRead(tempSensor);
  float vol = vout * 5.0 / 1024.0;
  int temp = (int) (vol * 100.0);
  return temp;
}

void printTemp() {
  //Doc nhiet do
  int temp = readTemp();
  //Hien thi ra LCD
  lcd.print(temp);
  lcd.print(char(0));
  lcd.print("C");
}

//---------------------------------Ham updateClock-------------------------------------------//
// Cho phep thiet lap thoi gian
void updateClock() {
   
  if (digitalRead(btnOK) == LOW) {
    subMode++;
    if (subMode > 5) {
      subMode = 1;
    }
    lcd.clear();
    delay(DELAY_TIME);
  }
  if (subMode == 1) {
    lcd.setCursor(0, 0);
    lcd.print("Mode: CHINH GIO");
    lcd.setCursor(3, 1);
    lcd.print("TIEP TUC?");
  }
  if (subMode == 2) {
    lcd.setCursor(0, 0);
    lcd.print("Chinh Gio");
    lcd.setCursor(0, 1);
    lcd.print("--> ");

    if (digitalRead(btnAdj) == LOW) {
      h++;
      if (h == 24) {
        h = 0;
      }
      delay(DELAY_TIME);
    }
    lcd.print(h);
    lcd.print(" ");
  }
  if (subMode == 3) {
    lcd.setCursor(0, 0);
    lcd.print("Chinh Phut");
    lcd.setCursor(0, 1);
    lcd.print("--> ");
    if (digitalRead(btnAdj) == LOW) {
      m++;
      if (m == 60) {
        m = 0;
      }
      delay(DELAY_TIME);
    }
    lcd.print(m);
    lcd.print(" ");
  }
  if (subMode == 4) {
    lcd.setCursor(0, 0);
    lcd.print("Chinh Giay");
    lcd.setCursor(0, 1);
    lcd.print("--> ");
    if (digitalRead(btnAdj) == LOW) {
      s++;
      if (s == 60) {
        s = 0;
      }
      delay(DELAY_TIME);
    }
    lcd.print(s);
    lcd.print(" ");
  }
  if (subMode == 5) {
    lcd.setCursor(0, 0);
    lcd.print("UPDATING");
    for (int i = 0; i < 8; i++) {
      lcd.print(".");
      delay(200);
    }
    rtc.setTime(h, m, s);
    delay(DELAY_TIME);
    lcd.clear();
    mode = 1;
  }
}

//---------------------------------Ham setAlarm-------------------------------------------//
// Cho phep hen gio
void setAlarm() {
  lcd.setCursor(5, 0);
  lcd.print("HEN GIO");
}
