/*
  DS1307: Real-Time Clock. RAM Access Example. Memory dump
  Read more: www.jarzebski.pl/arduino/komponenty/zegar-czasu-rzeczywistego-rtc-ds1307.html
  GIT: https://github.com/jarzebski/Arduino-DS1307
  Web: http://www.jarzebski.pl
  (c) 2014 by Korneliusz Jarzebski
*/

#include <Wire.h>
#include <DS1307.h>

DS1307 clock;

uint8_t buf[56];
char tmp[16];

void setup()
{
  Serial.begin(9600);
  
  // Initialize DS1307
  Serial.println("Initialize DS1307");
  clock.begin();

  // Fill memory
  Serial.println("Filling memory");

  for (byte i = 0; i < 56; i++)
  {
    buf[i] = i;
  }

  clock.writeMemory(0, buf, 56);

  for (byte i = 0; i < 56; i++)
  {
    buf[i] = 0;
  }    

  Serial.println("... Done");
  Serial.println();  
}

void loop()
{
  // Read a memory
  Serial.println("Reading memory");
  Serial.println();
  Serial.print("        ");

  for (byte i = 0; i < 16; i++)
  {
      sprintf(tmp, "0x%.2X ", i, 2);
      Serial.print(tmp);
  }

  Serial.println();
  Serial.println("---------------------------------------------------------------------------------------");

  clock.readMemory(0, buf, 56);
  
  for (byte j = 0; j < 4; j++)
  {
    sprintf(tmp, " 0x%.2X : ", (j*16), 2);

    Serial.print(tmp);

    for (byte i = 0; i < 16; i++)
    {
      if ((j*16 + i) > 55)
      {
        break;
      }

      sprintf(tmp, "0x%.2X ", buf[j*16 + i], 2);

      Serial.print(tmp);
    }

    Serial.println();
  }

  delay(5000);
}
