// Date and time functions using a DS3231 RTC connected via I2C and Wire lib
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "RTClib.h"

RTC_DS3231 rtc;
LiquidCrystal_I2C lcd(0x27, 16, 2);

byte Clock[8] =
{
    0b11111,
    0b10001,
    0b01010,
    0b00100,
    0b00100,
    0b01010,
    0b10101,
    0b11111
};
byte Calendar[8] =
{
    0b00000,
    0b11111,
    0b11111,
    0b10001,
    0b10011,
    0b10001,
    0b11111,
    0b00000
};
byte Thermometer[8] = //icon for termometer
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

void setup ()
{
    Serial.begin(9600);

    lcd.init (); // initialize the lcd
    lcd.backlight();//To Power ON the back light

    lcd.createChar(1, Clock);
    lcd.createChar(2, Calendar);
    lcd.createChar(3, Thermometer);

#ifndef ESP8266
    while (!Serial); // wait for serial port to connect. Needed for native USB
#endif

    if (! rtc.begin())
    {
        Serial.println("Couldn't find RTC");
        Serial.flush();
        while (1);
    }

    if (rtc.lostPower())
    {
        Serial.println("RTC lost power, let's set the time!");
        // When time needs to be set on a new device, or after a power loss, the
        // following line sets the RTC to the date & time this sketch was compiled
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
        // This line sets the RTC with an explicit date & time, for example to set
        // January 21, 2014 at 3am you would call:
        // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
    }

    // When time needs to be re-set on a previously configured device, the
    // following line sets the RTC to the date & time this sketch was compiled
    // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
}

void loop ()
{
    DateTime now = rtc.now();

    lcd.setCursor(1, 0);
    lcd.write(1);
    lcd.print(" ");

    if(now.hour() > 12)
    {
        long H=now.hour()-12;
        if(H < 10)
        {
            lcd.print("0");
            lcd.print(H, DEC);
        }
    }
    else
    {
        long H=now.hour();
        lcd.print(H, DEC);
    }

    lcd.print(':');
    if(now.minute() < 10)
    {
        lcd.print("0");
        lcd.print(now.minute(), DEC);
    }
    else
    {
        lcd.print(now.minute(), DEC);
    }
    lcd.print(':');
    if(now.second() < 10)
    {
        lcd.print("0");
        lcd.print(now.second(), DEC);
    }
    else
    {
        lcd.print(now.second(), DEC);
    }
    //AM PM srart
    if(now.hour() >= 12)
    {
        lcd.print(" ");
        lcd.print("PM");
    }
    else
    {
        lcd.print(" ");
        lcd.print("AM");
    }
    //AM PM end

    lcd.print("  ");
    // time end

    // date start
    lcd.setCursor(0, 1);
    lcd.write(2);
    if(now.day() < 10)
    {
        lcd.print("0");
        lcd.print(now.day(), DEC);
    }
    else
    {
        lcd.print(now.day(), DEC);
    }
    lcd.print('/');
    if(now.month() < 10)
    {
        lcd.print("0");
        lcd.print(now.month(), DEC);
    }
    else
    {
        lcd.print(now.month(), DEC);
    }
    // lcd.print('/');
    // lcd.print(now.year(), DEC);

    // date end

    lcd.print(" ");

    lcd.print(" ");

    //temperature start
    lcd.write(3);
    lcd.print(rtc.getTemperature());
    lcd.print((char)223);
    lcd.print("C");
    //temperature end
}
