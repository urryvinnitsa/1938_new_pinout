//**************************************************
//!file                          : hard_config.h
//**************************************************
#ifndef __hard_config_H__
#define __hard_config_H__
#define VERSION 19
#define TRUE 1
#define FALSE 0
#define _XTAL_FREQ  32000000

#define U_OPORN 4096 // в реф 4096 милливольт


#define BAUDRG 816 //           // from 32MHz  115.2Kbps 68 (BRG16 = 1) 832 = 9600

#define BITSET(var,bitno)   ((var) |= 1 << (bitno))
#define BITCLR(var,bitno)   ((var) &= ~(1 << (bitno)))
#define BITTST1(var,bitno)  ((var) & (1 << (bitno)))
#define BITTST0(var,bitno)  (!((var) & (1 << (bitno))))
#define BITINV(var,bitno)   var=(var^(1 << (bitno)))

// режимы работы фонар€
typedef enum
{
    START_SHOW = 0, //стартовое шоу
    WORK_MODE,// работа в обычном режиме
} STATE_MODE;
//---------------
// занижено входное нормальное завышено
typedef enum {MODE_LOW = 0, MODE_NORM, MODE_HI} MODE_DEVICE;
extern volatile MODE_DEVICE mode;
extern volatile unsigned int U_in;
//---------------
// тип работы - тестовый или рабочий
typedef enum {TEST = 0, WORK} TEST_WORK;
extern volatile TEST_WORK test_work;
//---------------
extern volatile char asc_print;
#define STAB_U2 1500
#define GISTER  150

#define MIN_MK  1000  //4000
#define TERMO_ALARM 2000
#define TERMO_ERROR 1000
typedef  struct
{
    unsigned ms_1: 1; // миллисекундна€ метка
    unsigned sec: 1;  // секундна€ метка
    unsigned min: 1;  // минутна€ метка
    unsigned char cnt_sec; // счетчик секунд дл€ часов
    unsigned char cnt_min; // счетчик минут дл€ часов
    unsigned char cnt_hr;  // счетчик часов
    unsigned int TT;      // количество минут от момента включени€
    unsigned int TAL; // аварийный таймер превышени€ напр€жени€
    unsigned int TTMR; // аварийный таймер превышени€ температуры
} sFlags_;
//------------------------------------------------------
#if 0

1 нога - PIC KIT3
2 нога - ј÷ѕ 1.  онтроль Ќапр€жени€ сети:
если текущее напр€жение ниже 110ј— / 154DC, то на 5 ногу подаЄм У0Ф. ≈сли “екущее напр€жение выше 110ј— / 154DC, то на 5 ногу подаЄм У1Ф. ≈сли “екущее напр€жение выше 265ј— / 371DC, то на 5 ногу подаЄм У0Ф и пишем в пам€ть пиковое напр€жени€.  онтроль не гарантийного случа€.
3 нога Ц ј÷ѕ 2, термо контроль. ≈сли текущее напр€жение выше 105 √рад. ÷ельси€, то на 5 ногу подаЄм У0Ф и записываем в пам€ть недопустимый перегрев УЅѕФ. ≈сли текущее напр€жение ниже 85 √рад. ÷ельси€, то на 5 ногу подаЄм У1Ф. ¬ процессе отладки смотрим напр€жение на термосопротивлении от 20 до 105 градусов — и строим зависимость снижени€ мощности от снижени€ температуры. “акже фиксируем в программе значени€ напр€жений на термосопротивлении:  90— - смотрим текущее состо€ние ног ÷јѕ и уменьшаем текущую мощность в два раза. 95— -  смотрим текущее состо€ние ног ÷јѕ и снова уменьшаем текущую мощность в два раза(итого уменьшили в 4 раза).
ѕри снижении температуры до 90— восстанавливаем плановую мощность и уменьшаем в 2 раза. ѕри снижении температуры до 80— восстанавливаем плановую мощность.
4 нога Ц ј÷ѕ 3, фото контроль. ≈сли напр€жение на фоторезисторе достигло УU деньФ - то на 5 ногу подаЄм У0Ф. ≈сли напр€жение на фоторезисторе достигло УU ночьФ - то на 5 ногу подаЄм У1Ф.
5 нога Ц выход цифровой, неактивное состо€ние У0Ф, активное У1Ф
6 нога Ц выход цифровой, управление мощностью 10 - й разр€д выбора мощности, неактивное состо€ние УZФ, активное У0Ф.
7 нога - выход цифровой, управление мощностью 9 - й разр€д выбора мощности, неактивное состо€ние УZФ, активное У0Ф.
8 нога - У -Ф питани€
9 нога -  варц 8 м√ц
10 нога -  варц 8 м√ц
11 нога - выход цифровой, управление мощностью 8 - й разр€д выбора мощности, неактивное состо€ние УZФ, активное У0Ф.
12 нога - выход цифровой, управление мощностью 7 - й разр€д выбора мощности, неактивное состо€ние УZФ, активное У0Ф.
13 нога - выход цифровой, управление мощностью 6 - й разр€д выбора мощности, неактивное состо€ние УZФ, активное У0Ф.
14 нога - выход цифровой, управление мощностью 5 - й разр€д выбора мощности, неактивное состо€ние УZФ, активное У0Ф.
15 нога - выход цифровой, управление мощностью 4 - й разр€д выбора мощности, неактивное состо€ние УZФ, активное У0Ф.
16 нога - выход цифровой, управление мощностью 3 - й разр€д выбора мощности, неактивное состо€ние УZФ, активное У0Ф.
17 нога - TX
18 нога - RX
19 нога -  У -Ф питани€.
20 нога -  У +Ф питани€
21 нога - выход цифровой, управление мощностью 2 - й разр€д выбора мощности, неактивное состо€ние УZФ, активное У0Ф.
22 нога - выход цифровой, управление мощностью 1 - й разр€д выбора мощности, неактивное состо€ние УZФ, активное У0Ф.
23 нога - выход цифровой, индикатор работы процессора. √енератор импульсов 2√ц.
24 нога - Ќ»
25 нога - Ќ»
26 нога - Ќ»
27 нога - PIC KIT3
28 нога - PIC KIT3

1 RE3
2 RA0
3 RA1
4 RA2
5 RA3
6 RA4
7 RA5
8 VCC
9 RA7
10 RA6
11 RC0
12 RC1
13 RC2
14 RC3
//
15 RC4
16 RC5
17 RC6
18 RC7
19 VCC
20 VDD
21 RB0
22 RB1
23 RB2
24 RB3
25 RB4
26 RB5
27 RB6
28 RB7


#endif


#define DDR_9_DAC TRISB
#define PORT_9_DAC LATB
#define PIN_9_DAC 1

#define DDR_8_DAC TRISB
#define PORT_8_DAC LATB
#define PIN_8_DAC 0

#define DDR_7_DAC TRISC
#define PORT_7_DAC LATC
#define PIN_7_DAC 5

#define DDR_6_DAC TRISC
#define PORT_6_DAC LATC
#define PIN_6_DAC 4

#define DDR_5_DAC TRISC
#define PORT_5_DAC LATC
#define PIN_5_DAC 3

#define DDR_4_DAC TRISC
#define PORT_4_DAC LATC
#define PIN_4_DAC 2

#define DDR_3_DAC TRISC
#define PORT_3_DAC LATC
#define PIN_3_DAC 1

#define DDR_2_DAC TRISC
#define PORT_2_DAC LATC
#define PIN_2_DAC 0

#define DDR_1_DAC TRISA
#define PORT_1_DAC LATA
#define PIN_1_DAC 5

#define DDR_0_DAC TRISA
#define PORT_0_DAC LATA
#define PIN_0_DAC 4

//---------------------------------------------------

#define DDR_LED TRISB
#define PORT_LED LATB
#define PIN_LED 2

#define LED(x) x ? BITSET(PORT_LED, PIN_LED): BITCLR(PORT_LED, PIN_LED);
//---------------------------------------------------

#define DDR_5 TRISB
#define PORT_5 LATB
#define PIN_5 4
#define DDR_5_2 TRISA
#define PORT_5_2 LATA
#define PIN_5_2 3

#define P_5(x) x ? BITSET(PORT_5, PIN_5): BITCLR(PORT_5, PIN_5);
#define P_5_2(x) x ? BITSET(PORT_5_2, PIN_5_2): BITCLR(PORT_5_2, PIN_5_2);
//---------------------------------------------------

#define TEST_DDR TRISB
#define TEST_PORT LATB
#define TEST_PIN 3
#define TEST_INV()  {BITINV(TEST_PORT, TEST_PIN);}

#define CNT_MESS_MAX 80
#define U_LED_SET 50


#define U_IN_MIN 110
#define U_IN_MAX 280
#define U_IN_MAX_MIN 275

#define U0_GISTER 4

#define PERIOD_U2 40
#define U2_STAB 1500
#define U2_GISTER 25
extern volatile unsigned char dac_current;
extern volatile unsigned char my_power;

char fnGetOnOff(void);


#endif //__hard_config_H__
