//!**************************************************
//! file                           : dac.c
//!**************************************************
#include <xc.h>
#include "hard_config.h"
#include "dac.h"

#define SIZE_DAC 10
//---------------------------------------------------------------

void fnDacInit(void)
{
    // tris-0
    BITCLR(DDR_0_DAC, PIN_0_DAC);
    BITCLR(DDR_1_DAC, PIN_1_DAC);
    BITCLR(DDR_2_DAC, PIN_2_DAC);
    BITCLR(DDR_3_DAC, PIN_3_DAC);
    BITCLR(DDR_4_DAC, PIN_4_DAC);
    BITCLR(DDR_5_DAC, PIN_5_DAC);
    BITCLR(DDR_6_DAC, PIN_6_DAC);
    BITCLR(DDR_7_DAC, PIN_7_DAC);
    BITCLR(DDR_8_DAC, PIN_8_DAC);
    BITCLR(DDR_9_DAC, PIN_9_DAC);
    //
    BITCLR(PORT_0_DAC, PIN_0_DAC);
    BITCLR(PORT_1_DAC, PIN_1_DAC);
    BITCLR(PORT_2_DAC, PIN_2_DAC);
    BITCLR(PORT_3_DAC, PIN_3_DAC);
    BITCLR(PORT_4_DAC, PIN_4_DAC);
    BITCLR(PORT_5_DAC, PIN_5_DAC);
    BITCLR(PORT_6_DAC, PIN_6_DAC);
    BITCLR(PORT_7_DAC, PIN_7_DAC);
    BITCLR(PORT_8_DAC, PIN_8_DAC);
    BITCLR(PORT_9_DAC, PIN_9_DAC);
}
////---------------------------------------------------------------
void fnSetDac(unsigned int value)
{
    unsigned int mask = 1;
    do
    {
        if (!(value & mask))
        {
            switch (mask)
            {
            case 1:
                BITCLR(DDR_0_DAC, PIN_0_DAC);
                BITCLR(PORT_0_DAC, PIN_0_DAC);
                break;
            case 2:
                BITCLR(DDR_1_DAC, PIN_1_DAC);
                BITCLR(PORT_1_DAC, PIN_1_DAC);
                break;
            case 4:
                BITCLR(DDR_2_DAC, PIN_2_DAC);
                BITCLR(PORT_2_DAC, PIN_2_DAC);
                break;
            case 8:
                BITCLR(DDR_3_DAC, PIN_3_DAC);
                BITCLR(PORT_3_DAC, PIN_3_DAC);
                break;
            case 16:
                BITCLR(DDR_4_DAC, PIN_4_DAC);
                BITCLR(PORT_4_DAC, PIN_4_DAC);
                break;
            case 32:
                BITCLR(DDR_5_DAC, PIN_5_DAC);
                BITCLR(PORT_5_DAC, PIN_5_DAC);
                break;
            case 64:
                BITCLR(DDR_6_DAC, PIN_6_DAC);
                BITCLR(PORT_6_DAC, PIN_6_DAC);
                break;
            case 128:
                BITCLR(DDR_7_DAC, PIN_7_DAC);
                BITCLR(PORT_7_DAC, PIN_7_DAC);
                break;
            case 256:
                BITCLR(DDR_8_DAC, PIN_8_DAC);
                BITCLR(PORT_8_DAC, PIN_8_DAC);
                break;
            case 512:
                BITCLR(DDR_9_DAC, PIN_9_DAC);
                BITCLR(PORT_9_DAC, PIN_9_DAC);
                break;
            }
        }
        else
        {
            switch (mask)
            {
            case 1:
                BITSET(DDR_0_DAC, PIN_0_DAC);
                break;
            case 2:
                BITSET(DDR_1_DAC, PIN_1_DAC);
                break;
            case 4:
                BITSET(DDR_2_DAC, PIN_2_DAC);
                break;
            case 8:
                BITSET(DDR_3_DAC, PIN_3_DAC);
                break;
            case 16:
                BITSET(DDR_4_DAC, PIN_4_DAC);
                break;
            case 32:
                BITSET(DDR_5_DAC, PIN_5_DAC);
                break;
            case 64:
                BITSET(DDR_6_DAC, PIN_6_DAC);
                break;
            case 128:
                BITSET(DDR_7_DAC, PIN_7_DAC);
                break;
            case 256:
                BITSET(DDR_8_DAC, PIN_8_DAC);
                break;
            case 512:
                BITSET(DDR_9_DAC, PIN_9_DAC);
                break;
            }
        }
        mask <<= 1;
    }
    while (mask != 0x0400);
}
////---------------------------------------------------------------
//
//




