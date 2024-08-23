//**************************************************
//!file                          : m_adc.h
//**************************************************
#ifndef __adc_H__
#define __adc_H__
#include <stdbool.h>
void fnAdcInit(void);
void ADC_GetConversion(void);
unsigned int fnGet1500(void);
void fnGetU_IN(void);
typedef struct
{
    unsigned int u_in;
    signed int termo;
    unsigned int umk;
    bool u_max;
} u_adc_t;
extern volatile u_adc_t u_adc;
extern volatile  unsigned char m_dac;
typedef struct
{
    bool termo_alarm;
    bool termo_error;
} termo_t;
extern volatile  termo_t termo;

//
#define FVR_4096 0xC3 // opora 4096 mv
#define FVR_1024 0xC1 // opora 1024 mv
#define HCS_U0 0 // 0 chan u_input
#define HCS_U1 1 // 1 chan u_termo
#define HCS_UMK 0x1F //chann fvr
#define VDD_VREF 0
#define VFR_VREF 3
#define ZNAM (unsigned long)1048576

signed int ADC_GetTermo(void);
#define KOEFF (float)9.964
#define PERIOD_U0 40

#endif //__adc_H__


