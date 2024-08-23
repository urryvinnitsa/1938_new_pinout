
#include <xc.h>
#include <stdio.h>
#include "m_adc.h"
#include "hard_config.h"
#include "dac.h"
#include "eemem.h"
#include "m_time.h"
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>

#define ACQ_US_DELAY 40
volatile u_adc_t u_adc;
static unsigned char m_dac_old = 0;
volatile  unsigned char m_dac;
volatile  termo_t termo;
static unsigned char   st_peregrev = 0;
#define SIZE_TABLE 13



//const unsigned int Table_D[SIZE_TABLE] =   {0, 18,  36,  53, 78,  99,  103, 110,  132, 148};// posled 100% 293.4
//const unsigned int Table_D[SIZE_TABLE] =   {0, 11,  22,  37, 53, 71,  85, 98,  109, 121};// posled 50% 140.
//const unsigned int Table_D[SIZE_TABLE] =   {0, 9,  19,  33, 42, 61,  74, 88,  105, 119};// parall 50% 81.0
const unsigned int Table_U[SIZE_TABLE] = { 150, 160, 170, 180, 190, 200, 210, 220, 230, 240, 250, 260, 270};
const unsigned int Table_D[SIZE_TABLE] =   {0,  18,  28,  43,  53,  72, 80,   93,  107, 128, 150, 160, 179};// parall 100% //169.5

unsigned int arr_in_u[CNT_MESS_MAX];

//const unsigned int Table_D[SIZE_TABLE] = {0, 12, 28, 44, 56, 70, 82, 94, 106, 118};
void fnTermoError(void);
void fnTermoAlarm(void);
unsigned int  fnGetTable(unsigned int iU);


#define APP_SIZE                29
#define d_t 1.35 //1.5824
const uint16_t n10k[APP_SIZE]  =   //in adc
{
    //vishai
    3980, 3940,//-30 - -25
    3890, 3826, 3748, 3653,//-20 - -5
    3541, 3409, 3259, 3091, 2908, 2712, //0 - 25
    2508, 2300, 2093, 1890, 1696, 1513, 1343, 1188, //30 - 65
    1047, 921,  809,  710,  623,  546,  480,  421,  371//70-110
};
const int16_t temper[APP_SIZE]  =   //in adc
{
    //vishai
    -30, -25,//-30 - -25
    -20, -15, -10, -5,//-20 - -5
    0, 5, 10, 15, 20, 25, //0 - 25
    30, 35, 40, 45, 50, 55, 60, 65, //30 - 65
    70, 75,  80, 85, 90, 95,  100, 105,  110//70-110
};
//--------------------------------------------------
void fnAdcInit(void)
{
    // cтавим опору 4096
    FVRCON = FVR_4096;
    // опорное для ацп - FVR
    ADCON1bits.ADPREF = VFR_VREF;
    // set the ADC to the options selected in the User Interface
    // GO_nDONE stop; ADON enabled; CHS AN0;
    ADCON0 = 0x01;
    // ADFM left; ADPREF VDD; ADCS FOSC/64;
    ADCON1 = 0xE3;
    // TRIGSEL no_auto_trigger;
    //  ADCON2 = 0x00;
    // ADRESL 0;
    ADRESL = 0x00;
    // ADRESH 0;
    ADRESH = 0x00;
}
//--------------------------------------------------
void  fnGetU_IN(void)
{
    float f;
    unsigned int temp, i;
    unsigned long lRez;
    static unsigned int cnt = 0;
    static unsigned int cnt2 = 0;
    static unsigned long lValueCurr = 0;
    static  unsigned long z = 0; //  переменная для суммы
    static unsigned int z_old;//  переменная для старого значения
    // select the A/D channel
    ADCON0bits.CHS = HCS_U0;
    // Turn on the ADC module
    ADCON0bits.ADON = 1;
    // Acquisition time delay8
    __delay_us(ACQ_US_DELAY);
    // Start the conversion
    ADCON0bits.GO_nDONE = 1;
    // Wait for the conversion to finish
    while (ADCON0bits.GO_nDONE)
    {
    }
    // Conversion finished, return the result
    ADCON0bits.ADON = 0;
    temp = ((unsigned int)((ADRESH << 8) + ADRESL));
    lRez = (unsigned long)temp * U_OPORN;
    lRez += 512; // убираем ошибку целочисленного деления
    lRez /= 1024;
    if (cnt == 0)
    {
        z_old = lRez;
        z = z_old;
    }
    else
    {
        z_old += lRez;
        z_old /= 2;
        z += z_old; // просуммировали;
    }
    cnt++;
    if (cnt >= PERIOD_U0)
    {
        cnt = 0;
        z /= PERIOD_U0;
        f = (float)z / KOEFF;
        f /= 1.4313; ///1.784;
        u_adc.u_in = (unsigned int)f;
    }
}
//--------------------------------------------------
signed int  ADC_GetTermo(void)
{
    unsigned char i;
    unsigned int temp;
    signed int termo;
    int temp_prev;
    int temp_next;
    int diffD;
    int diffT;
    int deltaT;
    float lRez;
    ADCON0bits.CHS = HCS_U1;
    // Turn on the ADC module
    ADCON0bits.ADON = 1;
    // Acquisition time delay8
    __delay_us(ACQ_US_DELAY);
    // Start the conversion
    ADCON0bits.GO_nDONE = 1;
    // Wait for the conversion to finish
    while (ADCON0bits.GO_nDONE)
    {
    }
    // Conversion finished, return the result
    ADCON0bits.ADON = 0;
    temp = ((unsigned int)((ADRESH << 8) + ADRESL));
    lRez = (unsigned long)temp * U_OPORN;
    lRez += 512; // убираем ошибку целочисленного деления
    lRez /= 1024;
    temp = lRez;// / d_t;
    if (temp < n10k[APP_SIZE - 1])
    {
        return 150;
    }
    if (temp > n10k[0])
    {
        return -35;
    }
    for (i = 1; i < APP_SIZE; i++)
    {
        if (n10k[i] < temp)
        {
            break;
        }
    }
    temp_prev = n10k[i - 1];
    temp_next = n10k[i];
    diffD = temp_prev - temp_next; //18
    diffT = n10k[i - 1] - temp;// 1
    deltaT = temp_prev - temp_next; //10
    lRez = 5.0 * (float)diffT;
    lRez /= (float)deltaT;
    lRez += temper[i - 1];
    termo = lRez;
    return termo;
}
//--------------------------------------------------
void  ADC_GetConversion(void)
{
#if 0
    float f;
    unsigned int temp, i;
    unsigned long lRez;
    static unsigned int cnt = 0;
    static unsigned int cnt2 = 0;
    static unsigned long lValueCurr = 0;
    static unsigned long iValueCurr = 0;
    static  unsigned long z = 0; //  переменная для суммы
    static unsigned int z_old;//  переменная для старого значения
    signed int gist;
    float dx;
    unsigned int dac_start;
#if 0
    // select the A/D channel
    ADCON0bits.CHS = HCS_U0;
    // Turn on the ADC module
    ADCON0bits.ADON = 1;
    // Acquisition time delay8
    __delay_us(ACQ_US_DELAY);
    // Start the conversion
    ADCON0bits.GO_nDONE = 1;
    // Wait for the conversion to finish
    while (ADCON0bits.GO_nDONE)
    {
    }
    // Conversion finished, return the result
    ADCON0bits.ADON = 0;
    temp = ((unsigned int)((ADRESH << 8) + ADRESL));
    lRez = (unsigned long)temp * U_OPORN;
    lRez += 512; // убираем ошибку целочисленного деления
    lRez /= 1024;
    if (cnt == 0)
    {
        z_old = lRez;
        z = z_old;
    }
    else
    {
        z_old += lRez;
        z_old /= 2;
        z += z_old; // просуммировали;
    }
    cnt++;
    if (cnt >= PERIOD_U0)
    {
        cnt = 0;
        z /= PERIOD_U0;
        f = (float)z / KOEFF;
        f /= 1.184;
        if (u_adc.u_max == 1)
        {
            if (f  <=   U_IN_MAX_MIN)
            {
                if (u_adc.u_max)
                {
                    u_adc.u_max = 0;
                    //   BITSET(TRISB, 7);
                }
            }
            else
            {
                return;
            }
        }
        else
        {
            if (f  >=   U_IN_MAX)
            {
                //                fnSetKey(0);
                //                fnSetDac(0x0);
                u_adc.u0 = (unsigned int)f;
                u_adc.u_max = 1;
                //         BITCLR(TRISB, 7);
                //         BITCLR(LATB, 7);
                return;
            }
        }
        u_adc.u0 = (unsigned int)f;
        gist = (unsigned int)f - u_adc.u0;
        //        if(abs(gist) > 4)
        //        {
        u_adc.u_max = 0;
        //            printf("new u = %d \r\n",(unsigned int)f);
        //!!!!!!!!!!!!!!!!!!!!!!!!
        dac_start =  fnGetTable(u_adc.u0);
        if (((20.0 + u_adc.u1) * 0.4) > (float)dac_start)
        {
            m_dac = dac_start;
        }
        else
        {
            m_dac = dac_start - (20.0 + u_adc.u1) * 0.4;
        }
        //--------------------------------------------------
        if (u_adc.u1 > 85 && u_adc.u1 < 90)
        {
            m_dac += 50;
            if (m_dac > 255)
            {
                m_dac = 255;
            }
        }
        if (u_adc.u1 >= 90 && u_adc.u1 < 95)
        {
            m_dac += 100;
            if (m_dac > 255)
            {
                m_dac = 255;
            }
        }
        if (u_adc.u1 > 95)
        {
            //             BITCLR(TRISB, 7);
            //             BITCLR(LATB, 7);
            st_peregrev = 1;
        }
        if (u_adc.u1 < 80  && st_peregrev == 1)
        {
            st_peregrev = 0;
            //           BITSET(TRISB, 7);
        }
        fnSetDac(m_dac);
#endif
        //!!!!!!!!!!!!!!!!!!!!!!!!
        //            if (termo.termo_error == 0 && termo.termo_alarm == 0)
        //            {
        //                fnGetTable(u_adc.u0);
        //            }
        //
    }
}
//------------------------------------------
ADCON0bits.CHS = HCS_U1;
// Turn on the ADC module
ADCON0bits.ADON = 1;
// Acquisition time delay8
__delay_us(ACQ_US_DELAY);
// Start the conversion
ADCON0bits.GO_nDONE = 1;
// Wait for the conversion to finish
while (ADCON0bits.GO_nDONE)
{
}
// Conversion finished, return the result
ADCON0bits.ADON = 0;
temp = ((unsigned int)((ADRESH << 8) + ADRESL));
lRez = (unsigned long)temp * U_OPORN;
lRez += 512; // убираем ошибку целочисленного деления
lRez /= 1024;
temp = lRez / d_t;
//    u_adc.u1 = lRez;

//     printf("lrez %d\r\n", temp);
for (i = 1; i < (APP_SIZE); i++)
{
    if (temp >= n10k[i])
    {
        u_adc.u1 = ((10 * 5 * (n10k[i - 1] - temp) / (n10k[i - 1] - n10k[i])) + 10 * (i - 1) * 5 - 10 * 30) / 10;
        //             printf("u_adc.u1 %d\r\n", u_adc.u1);
        break;
    }
}

//    if (u_adc.u1 <= TERMO_ERROR)
//    {
//        termo.termo_error = 1;
//        fnTermoError();
//    }
//    else if (u_adc.u1 > TERMO_ERROR && u_adc.u1 <= TERMO_ALARM)
//    {
//        termo.termo_alarm = 1;
//        fnTermoAlarm();
//    }
//    else // проверяем,был ли взведен еррор ранее
//    {
//        if (termo.termo_error == 1)
//        {
//            TRISA = 7;
//            termo.termo_error = 0;
//        }
//        termo.termo_alarm = 0;
//    }
//-------------------------------------
// cтавим опору 1024
FVRCON = FVR_1024;
// опорное для ацп - питание
ADCON1bits.ADPREF = VDD_VREF;
// select the A/D channel
ADCON0bits.CHS = HCS_UMK;
// Turn on the ADC module
ADCON0bits.ADON = 1;
// Acquisition time delay8
__delay_us(ACQ_US_DELAY);
// Start the conversion
ADCON0bits.GO_nDONE = 1;
// Wait for the conversion to finish
while (ADCON0bits.GO_nDONE)
{
}
temp = ((unsigned int)((ADRESH << 8) + ADRESL));
lRez = (unsigned long)ZNAM / (unsigned long)temp;
u_adc.umk = (unsigned int)lRez;
// возвращаем
FVRCON = FVR_4096;
ADCON1bits.ADPREF = VFR_VREF;
#if 0
if (u_adc.umk < MIN_MK) // записываемся и уходим
{
    // printf("write TT %d\r\n", sFlags.TT);
    un_tt.line.TP = sFlags.TT;
    fnWriteDataTTEeprom();
    __delay_ms(1000);
}
#endif
#endif
}
//--------------------------------------------------
unsigned int fnGetTable(unsigned int iU)
{
    unsigned char i;
    unsigned int m_dac_prev;
    unsigned int diffU;
    unsigned int diffD;
    unsigned int deltaU;
    float f;
    for (i = 0; i < SIZE_TABLE; i++)
    {
        if (Table_U[i] > iU)
        {
            break;
        }
    }
    if (i)
    {
        m_dac_prev = Table_D[i - 1];
        m_dac = Table_D[i];
        diffD = m_dac - m_dac_prev; //18
        diffU = iU - Table_U[i - 1];// 1
        deltaU = Table_U[i] - Table_U[i - 1]; //10
        f = diffU * (float)diffD;
        f += deltaU / 2;
        f /= (float)deltaU;
        f += m_dac_prev;
        m_dac = (unsigned char)f;
    }
    else
    {
        m_dac = 0;
    }
    return m_dac;
    //
    //    if (m_dac_old != m_dac)
    //    {
    //        m_dac_old = m_dac;
    //       // fnSetDac(m_dac);
    //    }
}
//------------------------------------------
void fnTermoError(void)
{
    //    TRISA = 3;
    //    LATA2 = 0;
}
//------------------------------------------
void fnTermoAlarm(void)
{
    //    fnSetKey(0);
    //    fnSetDac(0xFF);
}
//------------------------------------------

