/*
  DS1307: Real-Time Clock. Date Format
  Read more: www.jarzebski.pl/arduino/komponenty/zegar-czasu-rzeczywistego-rtc-ds1307.html
  GIT: https://github.com/jarzebski/Arduino-DS1307
  Web: http://www.jarzebski.pl
  (c) 2014 by Korneliusz Jarzebski
*/

#include <Wire.h>
#include <DS1307.h>

DS1307 clock;

void setup()
{
  Serial.begin(9600);

  // Initialize DS1307
  Serial.println("Initialize DS1307");;
  clock.begin();

  // If date not set
  if (!clock.isReady())
  {
    // Set sketch compiling time
    clock.setDateTime(__DATE__, __TIME__);

    // Set from UNIX timestamp
    // clock.setDateTime(1397408400);

    // Manual (YYYY, MM, DD, HH, II, SS
    // clock.setDateTime(2014, 4, 13, 19, 21, 00);
  }
}

void loop()
{
  Serial.println(clock.dateFormat("d-m-Y H:i:s"));
  Serial.println(clock.dateFormat("d F Y H:i:s"));
  Serial.println(clock.dateFormat("j M y, h:ia (l)"));
  Serial.println("");
  delay(1000);
}
