// Temperature functions using a DS3231 RTC connected via I2C and Wire lib
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "RTClib.h"

RTC_DS3231 rtc;
LiquidCrystal_I2C lcd(0x27, 16, 2);

unsigned long blinkTime = 0;
String placeHolder = "                ";

byte Thermometer[8] =  //icon for termometer
  {
    0b00100,
    0b01010,
    0b01010,
    0b01110,
    0b01110,
    0b11111,
    0b11111,
    0b01110
  };
byte degreeCentigrade[8] = {
  0b11000,
  0b11000,
  0b00110,
  0b01001,
  0b01000,
  0b01000,
  0b01001,
  0b00110
};

void setup() {
  Serial.begin(9600);

  lcd.init();       // initialize the lcd
  lcd.backlight();  //To Power ON the back light

  lcd.createChar(3, Thermometer);
  lcd.createChar(4, degreeCentigrade);

#ifndef ESP8266
  while (!Serial)
    ;  // wait for serial port to connect. Needed for native USB
#endif

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1)
      ;
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, let's set the time!");
  }
}

void loop() {
  DateTime now = rtc.now();



  lcd.setCursor(0, 0);
  if (millis() - blinkTime > 3500) {
    lcd.print("ROOM TEMPERATURE");
    blinkTime = millis();
  }
  if (millis() - blinkTime > 2900) {
    lcd.print(placeHolder);
  }

  lcd.setCursor(4, 1);
  //temperature start
  lcd.write(3);
  float Temperature = (rtc.getTemperature());
  lcd.print(Temperature);
  lcd.write(4);
  //temperature end
}
