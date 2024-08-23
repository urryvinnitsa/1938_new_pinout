//!**************************************************
//! file                           : eemem.h
//!**************************************************
#ifndef __eemem_H__
#define __eemem_H__


#define ADR_1 16
#define ADR_2 64
#define ADR_3 112

#define ADR_TT_1 192
#define ADR_TT_2 208
#define ADR_TT_3 224


unsigned char fnCalcCrc(unsigned char *mas, unsigned char len);

#define ARR_SIZE 39
typedef union
{
    struct
    {
        unsigned char serial[16];
        unsigned int t_min; //2
        unsigned int t_max; //4
        unsigned int T1; //6
        unsigned int T2; //8
        unsigned int T3; //10
        unsigned int D1; //12
        unsigned int D2; //14
        unsigned int TP; //16
        unsigned char P1; //17
        unsigned char PT1; //18
        unsigned char PT2; //19
        unsigned char PT3; //20
        unsigned char iCntAlarm;//21
        unsigned char iUAlarm;//22
        unsigned char crc;//23
    } line;
    unsigned char arr[ARR_SIZE];
} un_t;

extern volatile un_t un;

#define ARR_SIZE_TT 3
typedef union
{
    struct
    {
        unsigned int TP;
        unsigned char crc;//23
    } line;
    unsigned char arr[ARR_SIZE_TT];
} un_tt_t;
extern volatile un_tt_t un_tt;
void fnWriteDataEeprom(void);
unsigned char fnGetDataEeprom(void);
void fnWriteDataTTEeprom(void);
unsigned char fnGetDataEepromTT(void);
unsigned char fnCalcCrc(unsigned char *mas, unsigned char len);

#endif //__eemem_H__
