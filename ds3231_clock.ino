  // Date and time functions using a DS3231 RTC connected via I2C and Wire lib
  #include <Wire.h>
  #include <LiquidCrystal_I2C.h>
  #include "RTClib.h"

  RTC_DS3231 rtc;
  LiquidCrystal_I2C lcd(0x27, 16, 2);
  char daysOfTheWeek[7][12] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

  byte Clock[8] =
  {
    0b10001,
    0b01110,
    0b10101,
    0b10101,
    0b10111,
    0b10001,
    0b01110,
    0b00000
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
  byte degreeCentigrade[8] = 
  {
    0b11000,
    0b11000,
    0b00110,
    0b01001,
    0b01000,
    0b01000,
    0b01001,
    0b00110
  };
  byte daySign[8] = {
    0b00000,
    0b10001,
    0b01110,
    0b01010,
    0b01110,
    0b10001,
    0b00000,
    0b00000
}; 


  void setup ()
  {
      Serial.begin(9600);

      lcd.init (); // initialize the lcd
      lcd.backlight();//To Power ON the back light

      lcd.createChar(1, Clock);
      lcd.createChar(2, Calendar);
      lcd.createChar(3, Thermometer);
      lcd.createChar(4, degreeCentigrade); 
      lcd.createChar(5, daySign);     

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
          rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));//auto update from computer time
          // rtc.adjust(DateTime(2023, 3, 21, 15, 24, 0));// to set the time manually
      }
  }

  void loop ()
  {
      DateTime now = rtc.now();

      lcd.setCursor(0, 0);
      lcd.write(1);
      // lcd.print(" ");

      if(now.hour() == 0)
      {
        lcd.print("12");
      }
      else if(now.hour() > 12)
      {
          int Hour=now.hour()-12;
          if(Hour < 10)
          {
            lcd.print("0");
            lcd.print(Hour, DEC);
          }
          else{
            lcd.print(Hour, DEC);          
          }
      }
      else
      {
        int Hour=now.hour();
        if(Hour < 10)
          {
            lcd.print("0");
            lcd.print(Hour, DEC);
          }
          else{
            lcd.print(Hour, DEC);          
          }
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
        // lcd.print(" ");
        lcd.print("PM");
      }
      else
      {
        // lcd.print(" ");
        lcd.print("AM");
      }
      //AM PM end
      // time end

      lcd.print(" ");
      //temperature start
      lcd.write(3);
      int Temperature=(rtc.getTemperature());
      lcd.print(Temperature);
      lcd.write(4);
      //temperature end

      ///////////////////////////////////////////////////////////////////////////////////////

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
      lcd.print('/');
      lcd.print(now.year(), DEC);
      // date end

      lcd.print(" ");
      //Print days name start
      lcd.write(5);   
      lcd.print(daysOfTheWeek[now.dayOfTheWeek()]);      
      //Print days name end

      
  }

