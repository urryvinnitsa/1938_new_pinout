//**************************************************
//!file                          : hard_config.h
//**************************************************
#ifndef __hard_config_H__
#define __hard_config_H__
#define VERSION 19
#define TRUE 1
#define FALSE 0
#define _XTAL_FREQ  32000000

#define U_OPORN 4096 // � ��� 4096 ����������


#define BAUDRG 816 //           // from 32MHz  115.2Kbps 68 (BRG16 = 1) 832 = 9600

#define BITSET(var,bitno)   ((var) |= 1 << (bitno))
#define BITCLR(var,bitno)   ((var) &= ~(1 << (bitno)))
#define BITTST1(var,bitno)  ((var) & (1 << (bitno)))
#define BITTST0(var,bitno)  (!((var) & (1 << (bitno))))
#define BITINV(var,bitno)   var=(var^(1 << (bitno)))

// ������ ������ ������
typedef enum
{
    START_SHOW = 0, //��������� ���
    WORK_MODE,// ������ � ������� ������
} STATE_MODE;
//---------------
// �������� ������� ���������� ��������
typedef enum {MODE_LOW = 0, MODE_NORM, MODE_HI} MODE_DEVICE;
extern volatile MODE_DEVICE mode;
extern volatile unsigned int U_in;
//---------------
// ��� ������ - �������� ��� �������
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
    unsigned ms_1: 1; // �������������� �����
    unsigned sec: 1;  // ��������� �����
    unsigned min: 1;  // �������� �����
    unsigned char cnt_sec; // ������� ������ ��� �����
    unsigned char cnt_min; // ������� ����� ��� �����
    unsigned char cnt_hr;  // ������� �����
    unsigned int TT;      // ���������� ����� �� ������� ���������
    unsigned int TAL; // ��������� ������ ���������� ����������
    unsigned int TTMR; // ��������� ������ ���������� �����������
} sFlags_;
//------------------------------------------------------
#if 0

1 ���� - PIC KIT3
2 ���� - ��� 1. �������� ���������� ����:
���� ������� ���������� ���� 110�� / 154DC, �� �� 5 ���� ����� �0�. ���� ������� ���������� ���� 110�� / 154DC, �� �� 5 ���� ����� �1�. ���� ������� ���������� ���� 265�� / 371DC, �� �� 5 ���� ����� �0� � ����� � ������ ������� ����������. �������� �� ������������ ������.
3 ���� � ��� 2, ����� ��������. ���� ������� ���������� ���� 105 ����. �������, �� �� 5 ���� ����� �0� � ���������� � ������ ������������ �������� ��ϔ. ���� ������� ���������� ���� 85 ����. �������, �� �� 5 ���� ����� �1�. � �������� ������� ������� ���������� �� ������������������ �� 20 �� 105 �������� � � ������ ����������� �������� �������� �� �������� �����������. ����� ��������� � ��������� �������� ���������� �� ������������������:  90� - ������� ������� ��������� ��� ��� � ��������� ������� �������� � ��� ����. 95� -  ������� ������� ��������� ��� ��� � ����� ��������� ������� �������� � ��� ����(����� ��������� � 4 ����).
��� �������� ����������� �� 90� ��������������� �������� �������� � ��������� � 2 ����. ��� �������� ����������� �� 80� ��������������� �������� ��������.
4 ���� � ��� 3, ���� ��������. ���� ���������� �� ������������� �������� �U ����� - �� �� 5 ���� ����� �0�. ���� ���������� �� ������������� �������� �U ����� - �� �� 5 ���� ����� �1�.
5 ���� � ����� ��������, ���������� ��������� �0�, �������� �1�
6 ���� � ����� ��������, ���������� ��������� 10 - � ������ ������ ��������, ���������� ��������� �Z�, �������� �0�.
7 ���� - ����� ��������, ���������� ��������� 9 - � ������ ������ ��������, ���������� ��������� �Z�, �������� �0�.
8 ���� - � -� �������
9 ���� - ����� 8 ���
10 ���� - ����� 8 ���
11 ���� - ����� ��������, ���������� ��������� 8 - � ������ ������ ��������, ���������� ��������� �Z�, �������� �0�.
12 ���� - ����� ��������, ���������� ��������� 7 - � ������ ������ ��������, ���������� ��������� �Z�, �������� �0�.
13 ���� - ����� ��������, ���������� ��������� 6 - � ������ ������ ��������, ���������� ��������� �Z�, �������� �0�.
14 ���� - ����� ��������, ���������� ��������� 5 - � ������ ������ ��������, ���������� ��������� �Z�, �������� �0�.
15 ���� - ����� ��������, ���������� ��������� 4 - � ������ ������ ��������, ���������� ��������� �Z�, �������� �0�.
16 ���� - ����� ��������, ���������� ��������� 3 - � ������ ������ ��������, ���������� ��������� �Z�, �������� �0�.
17 ���� - TX
18 ���� - RX
19 ���� -  � -� �������.
20 ���� -  � +� �������
21 ���� - ����� ��������, ���������� ��������� 2 - � ������ ������ ��������, ���������� ��������� �Z�, �������� �0�.
22 ���� - ����� ��������, ���������� ��������� 1 - � ������ ������ ��������, ���������� ��������� �Z�, �������� �0�.
23 ���� - ����� ��������, ��������� ������ ����������. ��������� ��������� 2��.
24 ���� - ��
25 ���� - ��
26 ���� - ��
27 ���� - PIC KIT3
28 ���� - PIC KIT3

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
