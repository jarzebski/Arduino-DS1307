/*
DS1307.h - Header file for the DS1307 Real-Time Clock

Version: 1.1.0
(c) 2014 Korneliusz Jarzebski
www.jarzebski.pl

This program is free software: you can redistribute it and/or modify
it under the terms of the version 3 GNU General Public License as
published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef DS1307_h
#define DS1307_h

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define DS1307_ADDRESS              (0x68)

#define DS1307_REG_TIME             (0x00)
#define DS1307_REG_CONTROL          (0x07)
#define DS1307_REG_RAM              (0x08)

#ifndef RTCDATETIME_STRUCT_H
#define RTCDATETIME_STRUCT_H
struct RTCDateTime
{
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    uint8_t dayOfWeek;
    uint32_t unixtime;
};
#endif

typedef enum
{
    DS1307_LOW          = 0x00,
    DS1307_HIGH         = 0x80,
    DS1307_1HZ          = 0x10,
    DS1307_4096HZ       = 0x11,
    DS1307_8192HZ       = 0x12,
    DS1307_32768HZ      = 0x13
} ds1307_sqwOut_t;

class DS1307
{
    public:

	bool begin(void);

	void setDateTime(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second);
	void setDateTime(uint32_t t);
	void setDateTime(const char* date, const char* time);
	RTCDateTime getDateTime(void);
	uint8_t isReady(void);

	uint8_t readByte(uint8_t offset);
	uint8_t writeByte(uint8_t offset, uint8_t data);

	void readMemory(uint8_t offset, uint8_t * buff, uint8_t size);
	void writeMemory(uint8_t offset, uint8_t * buff, uint8_t size);

	void clearMemory(void);

	ds1307_sqwOut_t getOutput(void);
	void setOutput(ds1307_sqwOut_t mode);
	void setOutput(bool mode);

	char* dateFormat(const char* dateFormat, RTCDateTime dt);

    private:
	RTCDateTime t;

	char *strDayOfWeek(uint8_t dayOfWeek);
	char *strMonth(uint8_t month);
	char *strAmPm(uint8_t hour, bool uppercase);
	char *strDaySufix(uint8_t day);

	void readPacket(uint8_t offset, uint8_t * buff, uint8_t size);
	void writePacket(uint8_t offset, uint8_t * buff, uint8_t size);

	uint8_t hour12(uint8_t hour24);
	uint8_t bcd2dec(uint8_t bcd);
	uint8_t dec2bcd(uint8_t dec);

	long time2long(uint16_t days, uint8_t hours, uint8_t minutes, uint8_t seconds);
	uint16_t date2days(uint16_t year, uint8_t month, uint8_t day);
	uint8_t daysInMonth(uint16_t year, uint8_t month);
	uint16_t dayInYear(uint16_t year, uint8_t month, uint8_t day);
	bool isLeapYear(uint16_t year);
	uint8_t dow(uint16_t y, uint8_t m, uint8_t d);

	uint32_t unixtime(void);
	uint8_t conv2d(const char* p);

	void writeRegister8(uint8_t reg, uint8_t value);
	uint8_t readRegister8(uint8_t reg);
};

#endif