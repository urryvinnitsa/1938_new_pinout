//!**************************************************
//! Файл                    : gps.c
//!**************************************************
#include <xc.h>
#include "gps.h"
#include "hard_config.h"
#include "m_time.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
volatile date_time_t date_time_g;
volatile date_time_t date_time_l;
uint8_t arr_temp[15];
volatile float d_lon;

//#define IN_BUF_SIZE     256
//extern volatile unsigned char  inbuf[IN_BUF_SIZE];


//
// копируем в временой массив значение между указанными запятыми в том случае, когда они корректные
// arr_temp
char fnFindComa(uint8_t adr1, uint8_t adr2, uint8_t *arr)
{
    uint8_t start, stop;
    uint8_t i, a;
    start = 0;
    for (i = 0; i < IN_BUF_SIZE; i++)
    {
        if (inbuf[i] == ',')
        {
            start++;
            if (adr1 == start)
            {
                start = i;
                break;
            }
        }
    }
    stop = 0;
    for (i = 0; i < IN_BUF_SIZE; i++)
    {
        if (inbuf[i] == ',')
        {
            stop++;
            if (adr2 == stop)
            {
                stop = i;
                break;
            }
        }
    }
    //---------------------------------
    if (stop - start < 6)
    {
        return 0;
    }
    memset(arr, 0, sizeof(arr));
    //printf("start %d stop %d \r\n",start,stop);
    start++;
    a = 0;
    for (i = start; i < stop; i++)
    {
        arr[a++] = inbuf[i];
    }
    //printf("test %s \r\n",arr);
    return 1;
}
//---------------------------------------------------------
void fnSetLocalTime(void)
{
    date_time_l.year = date_time_g.year;
    date_time_l.month = date_time_g.month;
    date_time_l.day = date_time_g.day;
    date_time_l.hour = date_time_g.hour;
    date_time_l.minute = date_time_g.minute;
    date_time_l.sec = date_time_g.sec;
    int8_t zone = fnGetTimeZone(d_lon);
    date_time_l.hour += zone;
    if (date_time_l.hour > 24)
    {
        date_time_l.hour -= 24;
    }
    if (date_time_l.hour < 0)
    {
        date_time_l.hour += 24;
    }
    if (isDaylightTime(date_time_l.day, date_time_l.month, date_time_l.year + 2000, date_time_l.hour) == false)
    {
        date_time_l.hour += 1;
    }
    if (date_time_l.hour > 24)
    {
        date_time_l.hour -= 24;
    }
    if (date_time_l.hour < 0)
    {
        date_time_l.hour += 24;
    }
}
//---------------------------------------------------------
char fnGPS(void)
{
    uint8_t arr[10];
    uint8_t i, s, a;
    uint32_t whole_part;
    uint32_t fractional_part;
    uint16_t desN;
    if (fnFindComa(1, 2, &arr_temp) == 1)
    {
        arr[0] = arr_temp[0];
        arr[1] = arr_temp[1];
        arr[2] = 0;
        date_time_g.hour = atoi(arr);
        arr[0] = arr_temp[2];
        arr[1] = arr_temp[3];
        arr[2] = 0;
        date_time_g.minute = atoi(arr);
        arr[0] = arr_temp[4];
        arr[1] = arr_temp[5];
        arr[2] = 0;
        date_time_g.sec = atoi(arr);
        //printf("time %02d:%02d:%02d \r\n",date_time_g.hour,date_time_g.minute,date_time_g.sec);
    }
    if (fnFindComa(9, 10, &arr_temp) == 1)
    {
        arr[0] = arr_temp[0];
        arr[1] = arr_temp[1];
        arr[2] = 0;
        date_time_g.day = atoi(arr);
        arr[0] = arr_temp[2];
        arr[1] = arr_temp[3];
        arr[2] = 0;
        date_time_g.month = atoi(arr);
        arr[0] = arr_temp[4];
        arr[1] = arr_temp[5];
        arr[2] = 0;
        date_time_g.year = atoi(arr);
        //printf("date %02d-%02d-%02d \r\n",date_time_g.day,date_time_g.month,date_time_g.year);
    }
    if (fnFindComa(5, 6, &arr_temp) == 1)
    {
        memset(arr, 0, 9);
        //---------------------------
        for (i = 0; i < 10; i++)
        {
            if (arr_temp[i] == '.')
            {
                break;
            }
            arr[i] = arr_temp[i];
        }
        whole_part = atoi(arr);
        //
        s = 0;
        memset(arr, 0, 9);
        i++;
        for (a = i; a < i + 8; a++)
        {
            if (arr_temp[a] == 0)
            {
                arr[s] = 0;
                break;
            }
            arr[s++] = arr_temp[a];
        }
        //------------------------
        fractional_part = atoi(arr);
        desN = whole_part / 100;
        d_lon = whole_part - (desN * 100);
        d_lon += (float)fractional_part / 1000000.0;
        d_lon /= (float)60.0;
        d_lon += (float)desN;
    }
    return 0;
}
//---------------------------------------------------------
signed char fnGetTimeZone(float lon)
{
    lon = (lon < 0) ? (lon + 360.) : lon;
    if (lon < 7.5)
    {
        return 0;
    }
    else if (lon < 22.5)
    {
        return 1;
    }
    else if (lon < 37.5)
    {
        return 2;
    }
    else if (lon < 52.5)
    {
        return 3;
    }
    else if (lon < 67.5)
    {
        return 4;
    }
    else if (lon < 82.5)
    {
        return 5;
    }
    else if (lon < 97.5)
    {
        return 6;
    }
    else if (lon < 112.5)
    {
        return 7;
    }
    else if (lon < 127.5)
    {
        return 8;
    }
    else if (lon < 142.5)
    {
        return 9;
    }
    else if (lon < 157.5)
    {
        return 10;
    }
    else if (lon < 172.5)
    {
        return 11;
    }
    else if (lon < 180.0)
    {
        return 12;
    }
    else if (lon < 187.5)
    {
        return -12;
    }
    else if (lon < 202.5)
    {
        return -11;
    }
    else if (lon < 217.5)
    {
        return -10;
    }
    else if (lon < 232.5)
    {
        return -9;
    }
    else if (lon < 247.5)
    {
        return -8;
    }
    else if (lon < 262.5)
    {
        return -7;
    }
    else if (lon < 277.5)
    {
        return -6;
    }
    else if (lon < 292.5)
    {
        return -5;
    }
    else if (lon < 307.5)
    {
        return -4;
    }
    else if (lon < 322.5)
    {
        return -3;
    }
    else if (lon < 337.5)
    {
        return -2;
    }
    else if (lon < 352.5)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}
//---------------------------------------------------------
int dayOfWeek(int d, int m, int y)
{
    static int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
    y -= m < 3;
    return (y + y / 4 - y / 100 + y / 400 + t[m - 1] + d) % 7;
}  // end of dayOfWeek

#if 0
// http://en.wikipedia.org/wiki/Determination_of_the_day_of_the_week
// Devised by Tomohiko Sakamoto in 1993, it is accurate for any Gregorian date.
// Returns 0 = Sunday, 1 = Monday, etc.// http://en.wikipedia.org/wiki/Determination_of_the_day_of_the_week
// Devised by Tomohiko Sakamoto in 1993, it is accurate for any Gregorian date.
// Returns 0 = Sunday, 1 = Monday, etc.
int dayOfWeek(int d, int m, int y)
{
    static int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
    y -= m < 3;
    return (y + y / 4 - y / 100 + y / 400 + t[m - 1] + d) % 7;
}  // end of dayOfWeek

#endif
// DST = Daylight Savings Time
bool isDaylightTime(int day, int month, int year, int hour)
{
    int firstSundayInApril;
    int firstSundayInOctober;
    for (firstSundayInApril = 1; firstSundayInApril <= 31; firstSundayInApril++)
        if (dayOfWeek(firstSundayInApril, 4, year) == 0)
            break;
    for (firstSundayInOctober = 1; firstSundayInOctober <= 31; firstSundayInOctober++)
        if (dayOfWeek(firstSundayInOctober, 10, year) == 0)
            break;
    // May to September: not DST
    if (month >= 5 && month <= 9)
        return false;
    // January to March, and November to December: is DST
    if (month <= 3 || month >= 11)
        return true;
    // In April, if not yet first Sunday, still DST
    if (month == 4 && day < firstSundayInApril)
        return true;
    // In April, on first Sunday, still DST before 2 am
    if (month == 4 && day == firstSundayInApril && hour < 2)
        return true;
    // In October, if after first Sunday, is DST
    if (month == 10 && day > firstSundayInOctober)
        return true;
    // In October, on first Sunday, is DST after 2 am
    if (month == 10 && day == firstSundayInOctober && hour >= 2)
        return true;
    // some date in April or October that did not pass the above tests
    return false;
} // end of isDaylightTime

//// here to process incoming serial data after a terminator received
//void process_data (char * data)
//{
//  // for now just display it
//  Serial.println (data);
//
//  MatchState ms;
//  ms.Target (data);
//
//  char hour [5];
//  char mins [5];
//  char secs [5];
//  char valid [5];
//  char day [5];
//  char month [5];
//  char year [5];
//
//  char result = ms.Match ("^$GPRMC,(%d%d)(%d%d)(%d%d)%.%d+,(%a),.-,.-,.-,.-,.-,.-,(%d%d)(%d%d)(%d%d)");
//  //                                   HH    MM    SS    ms valid   lat  long spd crs  DD   MM    YY
//
//  if (result != REGEXP_MATCHED)
//      return;
//
//  ms.GetCapture (hour, 0);
//  ms.GetCapture (mins, 1);
//  ms.GetCapture (secs, 2);
//  ms.GetCapture (valid, 3);
//  ms.GetCapture (day, 4);
//  ms.GetCapture (month, 5);
//  ms.GetCapture (year, 6);
//
//  int iHour = atoi (hour);
//  int iDay = atoi (day);
//
//  // make time local
//  iHour += DST_TIME_OFFSET;
//
//  // if past midnight with time-zone offset adjust the day so the DST calculations are correct
//  if (iHour >= 24)
//      iDay++;
//  else if (iHour < 0)
//      iDay--;
//
//  // allow for daylight savings
//  if (isDaylightTime (iDay, atoi (month), atoi (year) + 2000, iHour))
//      iHour++;
//
//  // pull into range
//  if (iHour >= 24)
//      iHour -= 24;
//  else
//      if (iHour < 0)
//          iHour += 24;
//
//  // work out AM/PM
//  boolean pm = false;
//  if (iHour >= 12)
//      pm = true;
//
//  if (iHour > 12)
//      iHour -= 12;
//
//  char buf [8];
//  sprintf (buf, "%02i%s%s", iHour, mins, secs);
//
//  // send all 6 digits
//  for (byte digit = 0; digit < 6; digit++)
//  {
//      byte c = buf [digit];
//      if (c == '0' && digit == 0)
//          c = 0xF;  // code for a blank
//      else
//          c -= '0';
//      if (digit == 1 || digit == 3)
//          c |= 0x80;  // decimal place
//      sendByte (8 - digit, c);
//  }
//
//  sendByte (2, 0xF | ((valid [0] == 'A') ? 0x80 : 0));  // space, add dot if valid
//
//  if (pm)
//      sendByte (1, 0xE);  // P
//  else
//      sendByte (1, 0xF);
//}  // end of process_data
//
//
//
//#endif