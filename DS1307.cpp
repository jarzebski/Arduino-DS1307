/*
DS1307.cpp - Class file for the DS1307 Real-Time Clock

Version: 1.0.0
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

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>
#include "DS1307.h"

const uint8_t daysInMonth [] PROGMEM = { 31,28,31,30,31,30,31,31,30,31,30,31 };

bool DS1307::begin(void)
{
    Wire.begin();

    t.year = 0;
    t.month = 0;
    t.day = 0;
    t.hour = 0;
    t.minute = 0;
    t.second = 0;
    t.dayOfWeek = 0;
    t.unixtime = 0;

    return true;
}

void DS1307::setDateTime(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second)
{
    Wire.beginTransmission(DS1307_ADDRESS);

    #if ARDUINO >= 100
        Wire.write(DS1307_REG_TIME);
    #else
        Wire.send(DS1307_REG_TIME);
    #endif

    #if ARDUINO >= 100
        Wire.write(dec2bcd(second));
        Wire.write(dec2bcd(minute));
        Wire.write(dec2bcd(hour));
        Wire.write(dec2bcd(0));
        Wire.write(dec2bcd(day));
        Wire.write(dec2bcd(month));
        Wire.write(dec2bcd(year-2000));
    #else
        Wire.send(dec2bcd(second));
        Wire.send(dec2bcd(minute));
        Wire.send(dec2bcd(hour));
        Wire.send(dec2bcd(0));
        Wire.send(dec2bcd(day));
        Wire.send(dec2bcd(month));
        Wire.send(dec2bcd(year-2000));
    #endif

    #if ARDUINO >= 100
        Wire.write(DS1307_REG_TIME);
    #else
        Wire.send(DS1307_REG_TIME);
    #endif

    Wire.endTransmission();
}

void DS1307::setDateTime(uint32_t t)
{
    t -= 946681200;

    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;

    second = t % 60;
    t /= 60;

    minute = t % 60;
    t /= 60;

    hour = t % 24;
    uint16_t days = t / 24;
    uint8_t leap;

    for (year = 0; ; ++year)
    {
        leap = year % 4 == 0;
        if (days < 365 + leap)
        {
            break;
        }
        days -= 365 + leap;
    }

    for (month = 1; ; ++month) 
    {
        uint8_t daysPerMonth = pgm_read_byte(daysInMonth + month - 1);

        if (leap && month == 2)
        {
            ++daysPerMonth;
        }

        if (days < daysPerMonth)
        {
            break;
        }
        days -= daysPerMonth;
    }

    day = days + 1;

    setDateTime(year+2000, month, day, hour, minute, second);
}

void DS1307::setDateTime(const char* date, const char* time)
{
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;

    year = conv2d(date + 9);

    switch (date[0])
    {
        case 'J': month = date[1] == 'a' ? 1 : month = date[2] == 'n' ? 6 : 7; break;
        case 'F': month = 2; break;
        case 'A': month = date[2] == 'r' ? 4 : 8; break;
        case 'M': month = date[2] == 'r' ? 3 : 5; break;
        case 'S': month = 9; break;
        case 'O': month = 10; break;
        case 'N': month = 11; break;
        case 'D': month = 12; break;
    }

    day = conv2d(date + 4);
    hour = conv2d(time);
    minute = conv2d(time + 3);
    second = conv2d(time + 6);

    setDateTime(year+2000, month, day, hour, minute, second);
}

char* DS1307::dateFormat(const char* dateFormat)
{
    char buffer[255];

    buffer[0] = 0;

    char helper[10];

    if (t.unixtime == 0)
    {
        getDateTime();
    }

    while (*dateFormat != '\0')
    {
        switch (dateFormat[0])
        {
            // Day decoder
            case 'd':
                sprintf(helper, "%02d", t.day); 
                strcat(buffer, (const char *)helper); 
                break;
            case 'j':
                sprintf(helper, "%d", t.day);
                strcat(buffer, (const char *)helper);
                break;
            case 'l':
                strcat(buffer, (const char *)strDayOfWeek(t.dayOfWeek));
                break;
            case 'D':
                strncat(buffer, strDayOfWeek(t.dayOfWeek), 3);
                break;
            case 'w':
                sprintf(helper, "%d", t.dayOfWeek);
                strcat(buffer, (const char *)helper);
                break;

            // Month decoder
            case 'm':
                sprintf(helper, "%02d", t.month);
                strcat(buffer, (const char *)helper);
                break;
            case 'n':
                sprintf(helper, "%d", t.month);
                strcat(buffer, (const char *)helper);
                break;
            case 'F':
                strcat(buffer, (const char *)strMonth(t.month));
                break;
            case 'M':
                strncat(buffer, (const char *)strMonth(t.month), 3);
                break;

            // Year decoder
            case 'Y':
                sprintf(helper, "%d", t.year); 
                strcat(buffer, (const char *)helper); 
                break;
            case 'y': sprintf(helper, "%02d", t.year-2000);
                strcat(buffer, (const char *)helper);
                break;

            // Hour decoder
            case 'H':
                sprintf(helper, "%02d", t.hour);
                strcat(buffer, (const char *)helper);
                break;
            case 'G':
                sprintf(helper, "%d", t.hour);
                strcat(buffer, (const char *)helper);
                break;
            case 'h':
                sprintf(helper, "%02d", hour12(t.hour));
                strcat(buffer, (const char *)helper);
                break;
            case 'g':
                sprintf(helper, "%d", hour12(t.hour));
                strcat(buffer, (const char *)helper);
                break;
            case 'A':
                strcat(buffer, (const char *)strAmPm(t.hour, true));
                break;
            case 'a':
                strcat(buffer, (const char *)strAmPm(t.hour, false));
                break;

            // Minute decoder
            case 'i': 
                sprintf(helper, "%02d", t.minute);
                strcat(buffer, (const char *)helper);
                break;

            // Second decoder
            case 's':
                sprintf(helper, "%02d", t.second); 
                strcat(buffer, (const char *)helper); 
                break;

            // Misc decoder
            case 'U': 
                sprintf(helper, "%lu", t.unixtime);
                strcat(buffer, (const char *)helper);
                break;

            default: 
                strncat(buffer, dateFormat, 1);
                break;
        }
        dateFormat++;
    }

    return buffer;
}

RTCDateTime DS1307::getDateTime(void)
{
    int values[7];

    Wire.beginTransmission(DS1307_ADDRESS);
    #if ARDUINO >= 100
        Wire.write(DS1307_REG_TIME);
    #else
        Wire.send(DS1307_REG_TIME);
    #endif
    Wire.endTransmission();

    Wire.requestFrom(DS1307_ADDRESS, 7);

    while(!Wire.available()) {};

    for (int i = 6; i >= 0; i--)
    {
        #if ARDUINO >= 100
        values[i] = bcd2dec(Wire.read());
        #else
        values[i] = bcd2dec(Wire.receive());
        #endif
    }

    Wire.endTransmission();

    t.year = values[0] + 2000;
    t.month = values[1];
    t.day = values[2];
    t.dayOfWeek = values[3];
    t.hour = values[4];
    t.minute = values[5];
    t.second = values[6];
    t.unixtime = unixtime();

    return t;
}

uint8_t DS1307::isReady(void) 
{
    Wire.beginTransmission(DS1307_ADDRESS);

    #if ARDUINO >= 100
        Wire.write(DS1307_REG_TIME);
    #else
        Wire.send(DS1307_REG_TIME);
    #endif

    Wire.requestFrom(DS1307_ADDRESS, 1);

    while(!Wire.available()) {};

    #if ARDUINO >= 100
        uint8_t ss = Wire.read();
    #else
        uint8_t ss = Wire.receive();
    #endif

    Wire.endTransmission();

    return !(ss>>7);
}

uint8_t DS1307::readByte(uint8_t offset)
{
    uint8_t value; 

    Wire.beginTransmission(DS1307_ADDRESS);

    #if ARDUINO >= 100
        Wire.write(DS1307_REG_RAM + offset);
    #else
        Wire.send(DS1307_REG_RAM + offset);
    #endif

    Wire.endTransmission();

    Wire.requestFrom(DS1307_ADDRESS, 1);

    while (!Wire.available()) {}

    #if ARDUINO >= 100
        value = Wire.read();
    #else
        value = Wire.receive();
    #endif

    Wire.endTransmission();

    return value;
}

uint8_t DS1307::writeByte(uint8_t offset, uint8_t data)
{
    Wire.beginTransmission(DS1307_ADDRESS);

    #if ARDUINO >= 100
        Wire.write(DS1307_REG_RAM + offset);
        Wire.write(data);
    #else
        Wire.send(DS1307_REG_RAM + offset);
        Wire.send(data);
    #endif

    Wire.endTransmission();

    return 1;
}

void DS1307::clearMemory(void)
{
    for (uint8_t offset = 0; offset < 56; offset++)
    {
        writeByte(offset, 0);
    }
}

void DS1307::setSQWRate(ds1307_sqwRate_t rate)
{
    uint8_t value;
    value = readRegister8(DS1307_REG_CONTROL);
    value &= 0b00010000;
    value |= rate;
    writeRegister8(DS1307_REG_CONTROL, value);
}

ds1307_sqwRate_t DS1307::getSQWRate(void)
{
    uint8_t value;
    value = readRegister8(DS1307_REG_CONTROL);
    value &= 0b00000011;
    return (ds1307_sqwRate_t)value;
}

void DS1307::enableSQW(void)
{
    uint8_t value;
    value = readRegister8(DS1307_REG_CONTROL);
    value &= 0b00000011;
    value |= 0b00010000;
    writeRegister8(DS1307_REG_CONTROL, value);
}

void DS1307::disableSQW(void)
{
    uint8_t value;
    value = readRegister8(DS1307_REG_CONTROL);
    value &= 0b00000011;
    writeRegister8(DS1307_REG_CONTROL, value);
}

void DS1307::setOutput(bool enable)
{
    uint8_t value;
    value = (enable << 7);
    writeRegister8(DS1307_REG_CONTROL, value);
}

uint8_t DS1307::bcd2dec(uint8_t bcd)
{
    return ((bcd / 16) * 10) + (bcd % 16);
}

uint8_t DS1307::dec2bcd(uint8_t dec)
{
    return ((dec / 10) * 16) + (dec % 10);
}

char *DS1307::strDayOfWeek(int dayOfWeek)
{
    switch (dayOfWeek) {
        case 0:
            return "Sunday";
            break;
        case 1:
            return "Monday";
            break;
        case 2:
            return "Tuesday";
            break;
        case 3:
            return "Wednesday";
            break;
        case 4:
            return "Thrusday";
            break;
        case 5:
            return "Friday";
            break;
        case 6:
            return "Saturday";
            break;
        default:
            return "Unknown";
    }
}

char *DS1307::strMonth(int month)
{
    switch (month) {
        case 1:
            return "January";
            break;
        case 2:
            return "February";
            break;
        case 3:
            return "March";
            break;
        case 4:
            return "April";
            break;
        case 5:
            return "May";
            break;
        case 6:
            return "June";
            break;
        case 7:
            return "July";
            break;
        case 8:
            return "August";
            break;
        case 9:
            return "September";
            break;
        case 10:
            return "October";
            break;
        case 11:
            return "November";
            break;
        case 12:
            return "December";
            break;
        default:
            return "Unknown";
    }
}

char *DS1307::strAmPm(int hour, bool uppercase)
{
    if (hour < 12)
    {
        if (uppercase)
        {
            return "AM";
        } else
        {
            return "am";
        }
    } else
    {
        if (uppercase)
        {
            return "PM";
        } else
        {
            return "pm";
        }
    }
}

uint8_t DS1307::hour12(uint8_t hour24)
{
    if (hour24 == 0)
    {
        return 12;
    }

    if (hour24 > 12) 
    {
       return (hour24 - 12);
    }

    return hour24;
}

long DS1307::time2long(uint16_t days, uint8_t hours, uint8_t minutes, uint8_t seconds)
{
    return ((days * 24L + hours) * 60 + minutes) * 60 + seconds;
}

uint16_t DS1307::date2days(void)
{
    uint16_t year = t.year - 2000;
    uint16_t days = t.day;

    for (uint8_t i = 1; i < t.month; ++i)
    {
        days += pgm_read_byte(daysInMonth + i - 1);
    }

    if ((t.month > 2) && (year % 4 == 0))
    {
        ++days;
    }

    return days + 365 * year + (year + 3) / 4 - 1;
}

uint32_t DS1307::unixtime(void)
{
    uint32_t u;

    u = time2long(date2days(), t.hour, t.minute, t.second);
    u += 946681200;

    return u;
}

uint8_t DS1307::conv2d(const char* p)
{
    uint8_t v = 0;

    if ('0' <= *p && *p <= '9')
    {
        v = *p - '0';
    }

    return 10 * v + *++p - '0';
}

void DS1307::writeRegister8(uint8_t reg, uint8_t value)
{
    Wire.beginTransmission(DS1307_ADDRESS);
    #if ARDUINO >= 100
        Wire.write(reg);
        Wire.write(value);
    #else
        Wire.send(reg);
        Wire.send(value);
    #endif
    Wire.endTransmission();
}

uint8_t DS1307::readRegister8(uint8_t reg)
{
    uint8_t value;
    Wire.beginTransmission(DS1307_ADDRESS);
    #if ARDUINO >= 100
        Wire.write(reg);
    #else
        Wire.send(reg);
    #endif
    Wire.endTransmission();

    Wire.requestFrom(DS1307_ADDRESS, 1);
    while(!Wire.available()) {};
    #if ARDUINO >= 100
        value = Wire.read();
    #else
        value = Wire.receive();
    #endif;
    Wire.endTransmission();

    return value;
}
