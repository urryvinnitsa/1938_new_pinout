//!**************************************************
//! file                           : m_time.c
//!**************************************************

#include <xc.h>
#include <stdio.h>
#include "m_time.h"
#include "hard_config.h"
#include "eemem.h"
#define MS_10_FREQ _XTAL_FREQ/100/4
volatile sFlags_ sFlags;
volatile char asc_print;
#define FTMR1_IN  (_XTAL_FREQ/4)
#define FTMR1_CALC  65536- (FTMR1_IN/1000);
#define FTMR2_CALC  256- (2000000/28800);
union
{
    unsigned char arr[2];
    unsigned int i;
} un_i;

volatile unsigned char  inbuf[IN_BUF_SIZE];
static unsigned char        qin = 0;

static char             flag_rx_waiting_for_stop_bit;
static char             flag_rx_off;
static char             rx_mask;
static char             flag_rx_ready;
static char             timer_rx_ctr;
static char             timer_tx_ctr;
static char             bits_left_in_rx;
static char             bits_left_in_tx;
static char             rx_num_of_bits;
//static char             tx_num_of_bits;
static unsigned int     internal_rx_buffer;


unsigned char get_rx_pin_status(void)
{
    return  BITTST1(PORTB, TEST_PIN);
}

//--------------------------------------------------------------
void interrupt isr(void)// свободнобегущий таймер с алгоритмом Ѕрезенхема
{
    unsigned char data;
    unsigned char i;
    static unsigned char len = 0;
    static unsigned char a = 0;
    typedef enum {STATE_WAIT, STATE_PREF, STATE_ANSW} STATE_UART;
    static STATE_UART state_uart = STATE_WAIT;
    char mask, start_bit, flag_in;
    if (T0IE && T0IF)
    {
        T0IF = 0;
        // дл€ коррекции
        static signed long siTMR0Count = MS_10_FREQ;   // 10 ms
        static unsigned int cSEC = 100;
        siTMR0Count -= 2048; // делитель на 256 умножаем на 8 (делитель на 0таймер)
        if (siTMR0Count <= 0)
        {
            siTMR0Count += MS_10_FREQ;
            cSEC--;
            if (0 == cSEC)
            {
                if (mode == MODE_LOW)
                {
                    //    printf("mode low %d \r\n ",U_in);
                }
                if (mode == MODE_NORM)
                {
                    //      printf("mode norm %d \r\n ",U_in);
                }
                if (mode == MODE_HI)
                {
                    //     printf("mode hi %d \r\n ",U_in);
                }
                cSEC = 100;
                sFlags.sec = 1;
                sFlags.cnt_sec++;
                if (sFlags.cnt_sec > 1);//59)
                {
                    sFlags.TT++;
                    sFlags.min = 1;
                    sFlags.cnt_sec = 0;
                    sFlags.cnt_min++;
                    if (sFlags.cnt_min > 59)
                    {
                        sFlags.cnt_min = 0;
                        sFlags.cnt_hr++;
                        if (sFlags.cnt_hr > 23)
                        {
                            sFlags.cnt_hr = 0;
                        }
                    }
                }
                //
            } // if (cSEC..)
        } // if (<=)
    }// end if(T0IE && T0IF)
    if (TMR1IE && TMR1IF)
    {
        TMR1IF = 0;
        TMR1L = un_i.arr[0];
        TMR1H = un_i.arr[1];;
        sFlags.ms_1 = 1;
        //        TEST_INV();
    }
    if (TMR2IE && TMR2IF)
    {
        TMR2IF = 0;
        TMR2 += FTMR2_CALC;
        //------------------------
        if (flag_rx_off == FALSE)
        {
            if (flag_rx_waiting_for_stop_bit)
            {
                if (--timer_rx_ctr == 0)
                {
                    flag_rx_waiting_for_stop_bit = FALSE;
                    flag_rx_ready = FALSE;
                    internal_rx_buffer &= 0xFF;
                    if (internal_rx_buffer != 0xC2)
                    {
                        inbuf[qin] = internal_rx_buffer;
                        if (++qin >= IN_BUF_SIZE)
                        {
                            qin = 0;
                        }
                    }
                }
            }
            else        // rx_test_busy
            {
                if (flag_rx_ready == FALSE)
                {
                    start_bit = get_rx_pin_status();
                    // Test for Start Bit
                    if (start_bit == 0)
                    {
                        flag_rx_ready = TRUE;
                        internal_rx_buffer = 0;
                        timer_rx_ctr = 4;
                        bits_left_in_rx = rx_num_of_bits;
                        rx_mask = 1;
                    }
                }
                else    // rx_busy
                {
                    if (--timer_rx_ctr == 0)
                    {
                        // rcv
                        timer_rx_ctr = 3;
                        flag_in = get_rx_pin_status();
                        if (flag_in)
                        {
                            internal_rx_buffer |= rx_mask;
                        }
                        rx_mask <<= 1;
                        if (--bits_left_in_rx == 0)
                        {
                            flag_rx_waiting_for_stop_bit = TRUE;
                        }
                    }
                }
            }
        }
    }
    if (RCIF)
    {
        RCIF = 0;
        data = RCREG;
        while (TXIF == 0);
        TXREG = data;
        switch (state_uart)
        {
        case STATE_WAIT:
            if (data == 'Q')
            {
                state_uart = STATE_PREF;
            }
            else
            {
                state_uart = STATE_WAIT;
            }
            break;
        case STATE_PREF:
            if (data == 'W')
            {
                printf("QE");
                asc_print = 1;
            }
            if (data == 'S')
            {
                putch(32);
                for (i = 0; i < 16; i++)
                {
                    printf("%02X", un.line.serial[i]);
                }
            }
            if (data == 'R')
            {
                putch(ARR_SIZE);
                for (i = 0; i < ARR_SIZE; i++)
                {
                    putch(un.arr[i]);
                }
                state_uart = STATE_WAIT;
            }
            if (data == 'D')
            {
                state_uart = STATE_ANSW;
                len = 0;
            }
            break;
        case STATE_ANSW:
            if (len == 0)
            {
                len = data;
                a = 0;
            }
            else
            {
                un.arr[a++] = data;
                if (a == len)
                {
                    // test ks ?
                    fnWriteDataEeprom();// записываем
                    sFlags.TAL = 0;
                    state_uart = STATE_WAIT;
                }
            }
            break;
        }//end switch
    }  //if(RCIF)
} //
//--------------------------------------------------------------
void fnInitTime(void)
{
    sFlags.cnt_sec = 0;
    sFlags.cnt_min = 0;
    sFlags.cnt_hr = 0;
    sFlags.TT = 0;
    sFlags.TAL = 0;
    sFlags.TTMR = 0;
    TMR0 = 0;
    T0IE = 1;
    OPTION_REG = 0b11010010;
    //// ----- таймер 1 установки
    T1CKPS0 = 0; // T1CON
    T1CKPS1 = 0; // устанавливаем делители
    T1OSCEN = 0; //выключаем внутренний генератор
    TMR1CS0 = 0; //внутренн€€ тактова€ частота на таймер
    TMR1CS1 = 0; //внутренн€€ тактова€ частота на таймер
    un_i.i = FTMR1_CALC;
    TMR1L = un_i.arr[0];
    TMR1H = un_i.arr[1];
    PIE1 = 0;
    TMR1IE = 1;
    TMR1GE = 0;
    TMR1ON = 1;
    TMR1IF = 0;
    //--------
    TMR2IE = 1;
    T2CKPS0     = 1;
    T2CKPS1     = 0;
    T2OUTPS0     = 0;
    T2OUTPS1    = 0;
    T2OUTPS2    = 0;
    T2OUTPS3    = 0;
    TMR2IF = 0;
    TMR2 = FTMR2_CALC;
    TMR2ON = 1;
    flag_rx_ready = FALSE;
    flag_rx_waiting_for_stop_bit = FALSE;
    flag_rx_off = FALSE;
    rx_num_of_bits = 10;
    //--------
    PEIE = 1;   /* разрешаем прерывани€ периферии */
    asc_print = 0;
    ei();   // включить прерывани€
}
//--------------------------------------------------------------







