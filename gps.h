//!**************************************************
//! file                           : gps.h
//!**************************************************
#ifndef __gps_H__
#define __gps_H__
#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    uint8_t year;
    uint8_t month;
    uint8_t day;
    int8_t hour;
    uint8_t minute;
    uint8_t sec;
} date_time_t;

volatile extern date_time_t date_time_g;
volatile extern date_time_t date_time_l;
extern volatile float d_lon;
signed char fnGetTimeZone(float lon);
char fnGPS(void);
int dayOfWeek(int d, int m, int y);
bool isDaylightTime(int day, int month, int year, int hour) ;
void fnSetLocalTime(void);
#endif // __gps_H__



