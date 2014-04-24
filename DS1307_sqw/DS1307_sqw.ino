/*
  DS1307: Real-Time Clock. SQW Example
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

  // Set date time if not set. This is required for SQW work.
  if (!clock.isReady())
  {
    // Set sketch compiling time
    clock.setDateTime(__DATE__, __TIME__);
  }

  // Enable output as rate to 1Hz
  clock.setOutput(DS1307_1HZ);

  switch (clock.getOutput())
  {
    case DS1307_LOW:     Serial.println("SQW = LOW"); break;
    case DS1307_HIGH:    Serial.println("SQW = HIGH"); break;
    case DS1307_1HZ:     Serial.println("SQW = 1Hz"); break;
    case DS1307_4096HZ:  Serial.println("SQW = 4096Hz"); break;
    case DS1307_8192HZ:  Serial.println("SQW = 8192Hz"); break;
    case DS1307_32768HZ: Serial.println("SQW = 32768Hz"); break;
    default: Serial.println("SQW = Unknown"); break; }
}

void loop()
{
}
