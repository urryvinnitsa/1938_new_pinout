
//!**************************************************
//! file                           : main.c
//!**************************************************
#include <xc.h>
#include <stdio.h>

#include "hard_config.h"
#include "m_adc.h"
#include "m_time.h"
#include "dac.h"
#include "eemem.h"
//------------------------------------------------------------------------------
#pragma config FOSC = HS    // Oscillator Selection (INTOSC oscillator: I/O function on CLKIN pin)  
#pragma config WDTE = ON       // Watchdog Timer Enable (WDT disabled)  
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)  
#pragma config MCLRE = OFF       // MCLR Pin Function Select (MCLR/VPP pin function is MCLR)  
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)  
#pragma config CPD = OFF        // Data Memory Code Protection (Data memory code protection is disabled)  
#pragma config BOREN = ON       // Brown-out Reset Enable (Brown-out Reset enabled)  
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)  
#pragma config IESO = OFF       // Internal/External Switchover (Internal/External Switchover mode is enabled)  
#pragma config FCMEN = OFF       // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is enabled)  
// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)  
#pragma config VCAPEN = OFF     // Voltage Regulator Capacitor Enable (All VCAP pin functionality is disabled)  
#pragma config PLLEN = ON       // PLL Enable (4x PLL enabled)  
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)  
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)  
#pragma config LVP = OFF        // Low-Voltage Programming Enable (High-voltage on MCLR/VPP must be used for programming) 
//------------------------------------------------------------------------------
void fnPOwerOn(void);
void fnStartShow(void);
void fnWorkMode(void);
void fnLedInit(void);
void fnUinCut(void);
//------------------------------------------------------------------------------
static STATE_MODE curr_mode = START_SHOW;// текущий режим работы
unsigned int rez;
volatile MODE_DEVICE mode;
volatile TEST_WORK test_work;
volatile unsigned int tm0, tm1, tm2, tm3;
static unsigned int iShowTime;
static unsigned char power_old = 0;// для индикации мощности
volatile unsigned char dac_current;
unsigned int serg = 0;
typedef enum {razr_0, razr_1, razr_2, razr_3, razr_4, razr_5, razr_n} RAZR_TEST;
static RAZR_TEST razr_test = razr_0;
//
static char pair = 0;
static unsigned int iCntLed = 0;
volatile unsigned char my_power;
static unsigned int iCntPower = 0;
static unsigned int iCntDel = 0;
static char on_off = 0;
//--------------------------------------------------------------------------------
char fnGetOnOff(void)
{
    return on_off;
}
int main()
{
    OSCCON = 0b11110000;
    nWPUEN = 0;
    WPUB = 0;
    // ---------------------------
    //
    TRISA = 7;
    PORTA = 0;
    ANSELA = 3;
    //
    TRISC = 0b11000000; // ?6 ?7 USART, ?4 ? ?5 ????????, ??????? ??? ??????
    PORTC = 0;
    TRISB = 0xC0;//0b11000000; // b0-b5 b6-b7 programm
    PORTB = 0x00;
    ANSELB = 0;
    LATB = 0x00;
    //--------------------------------
    SPBRGH = BAUDRG >> 8; //0;
    SPBRGL  = (unsigned char)((unsigned int)BAUDRG & 0x00FF);  //0x40;//  = BAUDRG;// 115200
    BAUDCON = 0;
    BRG16 = 1;
    TXSTA   = 0b00100100;
    RCSTA   = 0b10010000;
    RCIE = 1;
    GIE = 1;
    //
    WDTCONbits.WDTPS = 0b01011; // 2sec
    WDTCONbits.SWDTEN = 1;
    CLRWDT();
    //
    __delay_ms(100);
    printf("\r\nversion %d \r\n", VERSION);
    fnAdcInit();
    ADC_GetConversion();
    printf("start  u_in = %d u_term = %d u_mk = %d   \r\n ",
           u_adc.u_in, u_adc.termo, u_adc.umk);
#if 0
    do
    {
        ADC_GetConversion();
        __delay_ms(1);
    }
    while (u_adc.umk < MIN_MK);
#endif
    fnDacInit();
    fnSetDac(0x30F);
    BITCLR(DDR_5, PIN_5);
    BITCLR(DDR_5_2, PIN_5_2);
    BITSET(TEST_DDR, TEST_PIN);
    fnInitTime();
    dac_current = 0x00;
    rez = fnGetDataEeprom();
    if (rez == 0)
    {
        test_work = TEST;
    }
    else
    {
        test_work = WORK;
    }
    __delay_ms(100);
    //--------------------------------------------------------------------------------
    while (1)
    {
        if (sFlags.ms_1 == 1)
        {
            sFlags.ms_1 = 0;
            iCntLed++;
            if (iCntLed >= 500)
            {
                iCntLed = 0;
                pair ^= 1;
                LED(pair);
            }
            fnGetU_IN();
            fnUinCut();
            u_adc.termo = ADC_GetTermo();
            CLRWDT();
            if (test_work == TEST)
            {
                iCntDel++;
                if (iCntDel >= 10)
                {
                    iCntDel = 0;
                    iCntPower++;
                    if (iCntPower >= 0x0400)
                    {
                        iCntPower = 0;
                    }
                    // fnSetDac(iCntPower);
                }
                //---------------------------------------------------------
                if (sFlags.sec == 1)
                {
                    sFlags.sec = 0;
                    if (asc_print == 0)
                    {
                        //                        printf(" u_in = %d u_term = %d u_mk = %d dac = %d  \r\n ",
                        //                               u_adc.u_in, u_adc.termo, u_adc.umk, m_dac);
                    }
                }
            }
            //---------------------------------------------------------
        }//if (sFlags.ms_1 == 1)
    }// end while 1
    //--------------------------------------------------------------------------------
    return 0;
}
//--------------------------------------------------
void fnUinCut(void)
{
    if (u_adc.u_in <= 101)
    {
        on_off = 0;
    }
    if (u_adc.u_in >= 265)
    {
        on_off = 0;
    }
    if (on_off == 0)
    {
        if (u_adc.u_in > 110 && u_adc.u_in < 255)
        {
            on_off = 1;
        }
    }
    if (on_off == 1)
    {
        P_5(1);
        P_5_2(1);
    }
    else
    {
        P_5(0);
        P_5_2(0);
    }
}
//--------------------------------------------------


//
void putch(unsigned char byte)
{
    while (TXIF == 0);
    TXREG = byte;
}
//--------------------------------------------------
void fnStartShow(void)
{
    //  ШОУ (только при включении фонаря, то есть при подаче на него напряжения)
    //Происходит нарастание яркости фонаря.
    //Через 1500мс устанавливаем мощность РТ3. (Какие ключи VT2-VT7 будут открыты, а какие закрыты будем выбирать в таблице загрузки данных). Если РТ3 не активен, то он заменяется РТ2. Если РТ2 не активен, то он заменяется РТ1. Если РТ1 не активен, то он заменяется Р1.
    //Через 1500мс устанавливаем мощность РТ2. (Какие ключи VT2-VT7 будут открыты, а какие закрыты будем выбирать в таблице загрузки данных). Если РТ2 не активен, то он заменяется РТ1. Если РТ1 не активен, то он заменяется Р1.
    //Через 1500мс устанавливаем мощность РТ1. (Какие ключи VT2-VT7 будут открыты, а какие закрыты будем выбирать в таблице загрузки данных). Если РТ1 не активен, то он заменяется Р1.
    //Через 1500мс устанавливаем мощность Р1. (Какие ключи VT2-VT7 будут открыты, а какие закрыты будем выбирать в таблице загрузки данных).
    iShowTime++;
    if (iShowTime == 1500)
    {
        if (power_old  != un.line.PT3)
        {
            power_old  = un.line.PT3;
        }
    }
    else if (iShowTime == 3000)
    {
        if (power_old  != un.line.PT2)
        {
            power_old  = un.line.PT2;
        }
    }
    else if (iShowTime == 4500)
    {
        if (power_old  != un.line.PT1)
        {
            power_old  = un.line.PT1;
        }
    }
    else if (iShowTime == 6000)
    {
        if (power_old  != un.line.P1)
        {
            power_old  = un.line.P1;
        }
    }
    else if (iShowTime >= 7500)
    {
        curr_mode = WORK_MODE;
        iShowTime = 0;
        printf("new mode WORK_MODE\r\n");
    }
    //    if (fnTestU(START_SHOW) == 0)return;
    //    if (fnTestTermo(START_SHOW) == 0)return;
}
//---------------------------------------------------------------------------
void fnWorkMode(void)
{
    //    //Основная работа:
    //    //Работа фонаря на полной мощности от момента включения до первого снижения мощности определяется по нижеприведённой формуле.
    //Рекомендация:
    //    это от момента включения и до 23: 00: 00 мощность 100 % .
    //    if (TT < (TMIN - (TMAX - TMIN) / D1 - (TMAX - (TMAX - TMIN) / D2 - TP) / 2))
    //        то устанавливаем мощность Р1.
    //        //Работа фонаря от первого снижения мощности до второго снижения мощности. Рекомендация: это 23:00:00
    //        и до 01: 00: 00 устанавливаем мощность 50 % от максимальной.
    //        if (TT < (TMIN - (TMAX - TMIN) / D1 - (TMAX - (TMAX - TMIN) / D2 - TP) / 2) + T1)
    //            то устанавливаем мощность РТ1.
    //            //Работа фонаря от второго снижения мощности до первого подъема мощности. Рекомендация: это от 01:00:00
    //            до 04: 00: 00 устанавливаем мощность 25 % .
    //            if (TT < (TMIN - (TMAX - TMIN) / D1 - (TMAX - (TMAX - TMIN) / D2 - TP) / 2) + T2)
    //                то устанавливаем мощность РТ2.
    //                //Работа фонаря от 04:00:00 до 06:00:00 устанавливаем мощность 50%.
    //                if (TT < (TMIN - (TMAX - TMIN) / D1 - (TMAX - (TMAX - TMIN) / D2 - TP) / 2) + T3)
    //                    то устанавливаем мощность РТ3.
    //                    Если условие не выполняется, то есть время ТТ стало больше или равно, то устанавливаем мощность Р1.
    if (sFlags.min) // новое значение минут
    {
        sFlags.min = 0;
        if (sFlags.TT <= tm0)
        {
            if (power_old  != un.line.P1)
            {
                power_old  = un.line.P1;
            }
        }
        else if ((sFlags.TT > tm0) && (sFlags.TT <= tm1))
        {
            if (power_old  != un.line.PT1)
            {
                power_old  = un.line.PT1;
            }
        }
        else if ((sFlags.TT > tm1) && (sFlags.TT <= tm2))
        {
            if (power_old  != un.line.PT2)
            {
                power_old  = un.line.PT2;
            }
        }
        else if ((sFlags.TT > tm2) && (sFlags.TT <= tm3))
        {
            if (power_old  != un.line.PT3)
            {
                power_old  = un.line.PT3;
            }
        }
        else if (sFlags.TT > tm3)
        {
            if (power_old  != un.line.P1)
            {
                power_old  = un.line.P1;
            }
        }
    }
}
//-------------------------------------------------------


