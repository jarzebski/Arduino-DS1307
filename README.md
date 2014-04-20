Arduino-DS1307
==============

DS1307 Real-Time Clock.

Features
--------

 * Raw access to time register (see ds1307_simple.ino)
 * Support access for 56-byte RAM (see ds1307_ram.ino)
 * Support for OUT pin (see ds1307_output.ino)
 * Suppoer for SQW pin (see ds1307_sqw.ino)
 * Different methods of formatting dates and times (see ds1307_dateformat.ino)

Date formats
------------

 * "d" - Day of the month, 2 digits with leading zeros
 * "j" - Day of the month without leading zeros
 * "l" - A full textual representation of the day of the week
 * "D" - A textual representation of a day, three letters
 * "w" - Numeric representation of the day of the week 
 * "m" - Numeric representation of a month, with leading zeros
 * "n" - Numeric representation of a month, without leading zeros
 * "F" - A full textual representation of a month, such as January or March
 * "M" - A short textual representation of a month, three letters
 * "Y" - A full numeric representation of a year, 4 digits
 * "y" - A two digit representation of a year
 * "H" - 24-hour format of an hour with leading zeros
 * "G" - 24-hour format of an hour without leading zeros
 * "h" - 12-hour format of an hour with leading zeros
 * "g" - 12-hour format of an hour without leading zeros
 * "A" - Uppercase Ante meridiem and Post meridiem
 * "a" - Lowercase Ante meridiem and Post meridiem
 * "i" - Minutes with leading zeros
 * "s" - Seconds, with leading zeros
 * "U" - Seconds since the Unix Epoch (January 1 1970 00:00:00 GMT)

More info
---------

Tutorials: www.jarzebski.pl/arduino/komponenty/zegar-czasu-rzeczywistego-rtc-ds1307.html

This library use I2C to communicate, 2 pins are required to interface.

Credits
-------

Original Code by JeeLabs http://news.jeelabs.org/code/

First fork by adafruit https://github.com/adafruit/RTClib