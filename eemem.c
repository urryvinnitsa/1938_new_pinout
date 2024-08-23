//!**************************************************
//! ����                    : eemem.c
//!**************************************************

#include <xc.h>
#include <stdint.h>
#include "eemem.h"
#include "hard_config.h"
volatile un_t un;
volatile un_tt_t un_tt;
//--------------------------------------------------------
unsigned char fnCalcCrc(unsigned char *mas, unsigned char len)
{
    unsigned char i, data, crc, fb, st_byt;
    st_byt = 0;
    crc = 0;
    do
    {
        data = *mas++;
        for (i = 0; i < 8; i++) // ������� ����� � �����
        {
            fb = crc  ^ data;
            fb &= 1;
            crc >>= 1;
            data >>= 1;
            if (fb == 1)
            {
                crc ^= 0x8c;    // �������, dallas
            }
        }
        st_byt++;
    }
    while (st_byt != len);  // ������� ������ � �������
    return crc;
}
//--------------------------------------------------------#define ADR_TT_1 180
void fnWriteDataEeprom(void)
{
    int i;
    un.line.crc = fnCalcCrc((uint8_t *)un.arr, ARR_SIZE - 1);
    for (i = 0; i < ARR_SIZE; i++)
    {
        CLRWDT();
        EEPROM_WRITE((uint8_t *)(ADR_1 + i), un.arr[i]);
    }
    // __delay_ms(30);
    for (i = 0; i < ARR_SIZE; i++)
    {
        CLRWDT();
        EEPROM_WRITE((uint8_t *)(ADR_2 + i), un.arr[i]);
    }
    //       __delay_ms(30);
    for (i = 0; i < ARR_SIZE; i++)
    {
        CLRWDT();
        EEPROM_WRITE((uint8_t *)(ADR_3 + i), un.arr[i]);
    }
}
//--------------------------------------------------------//
void fnWriteDataTTEeprom(void)
{
    int i;
    un_tt.line.crc = fnCalcCrc((uint8_t *)un_tt.arr, ARR_SIZE_TT - 1);
    for (i = 0; i < ARR_SIZE_TT; i++)
    {
        CLRWDT();
        EEPROM_WRITE((uint8_t *)(ADR_TT_1 + i), un_tt.arr[i]);
    }
    for (i = 0; i < ARR_SIZE; i++)
    {
        CLRWDT();
        EEPROM_WRITE((uint8_t *)(ADR_TT_2 + i), un_tt.arr[i]);
    }
    for (i = 0; i < ARR_SIZE; i++)
    {
        CLRWDT();
        EEPROM_WRITE((uint8_t *)(ADR_TT_3 + i), un_tt.arr[i]);
    }
}
//--------------------------------------------------------
unsigned char fnGetDataEeprom(void)
{
    unsigned char cRez = 0;
    int i;
    for (i = 0; i < ARR_SIZE; i++)
    {
        un.arr[i] = EEPROM_READ((uint8_t *)(ADR_1 + i));
    }
    if (fnCalcCrc((uint8_t *)un.arr, ARR_SIZE - 1) == un.arr[ARR_SIZE - 1])
    {
        if (un.line.serial[0] != 0 && un.line.serial[1] != 0)
        {
            BITSET(cRez, 0);
        }
    }
    //
    for (i = 0; i < ARR_SIZE; i++)
    {
        un.arr[i] = EEPROM_READ((uint8_t *)(ADR_2 + i));
    }
    if (fnCalcCrc((uint8_t *)un.arr, ARR_SIZE - 1) == un.arr[ARR_SIZE - 1])
    {
        if (un.line.serial[0] != 0 && un.line.serial[1] != 0)
        {
            BITSET(cRez, 1);
        }
    }
    //
    for (i = 0; i < ARR_SIZE; i++)
    {
        un.arr[i] = EEPROM_READ((uint8_t *)(ADR_3 + i));
    }
    if (fnCalcCrc((uint8_t *)un.arr, ARR_SIZE - 1) == un.arr[ARR_SIZE - 1])
    {
        if (un.line.serial[0] != 0 && un.line.serial[1] != 0)
        {
            BITSET(cRez, 2);
        }
    }
    //
    if (!cRez) // ������ �� �������
    {
        // default
        /*    fnSetDataEeprom();*/
        return cRez;
    }
    if (cRez == 7)
    {
        return 7;
    }
    else if (cRez != 7)// ���� ����� ������
    {
        if (BITTST1(cRez, 0)) // ������ �� ����� ������ �����
        {
            for (i = 0; i < ARR_SIZE; i++)
            {
                un.arr[i] = EEPROM_READ((uint8_t *)(ADR_1 + i));
            }
        }
        if (BITTST1(cRez, 1)) // ������ �� ����� ������ �����
        {
            for (i = 0; i < ARR_SIZE; i++)
            {
                un.arr[i] = EEPROM_READ((uint8_t *)(ADR_2 + i));
            }
        }
        if (BITTST1(cRez, 2)) // ������ �� ����� ������ �����
        {
            for (i = 0; i < ARR_SIZE; i++)
            {
                un.arr[i] = EEPROM_READ((uint8_t *)(ADR_3 + i));
            }
        }
        fnWriteDataEeprom();//���������� ����� ������
        return cRez;
    }
    return cRez;
}
//--------------------------------------------------------
unsigned char fnGetDataEepromTT(void)
{
    unsigned char cRez = 0;
    int i;
    for (i = 0; i < ARR_SIZE_TT; i++)
    {
        un_tt.arr[i] = EEPROM_READ((uint8_t *)(ADR_TT_1 + i));
    }
    if (fnCalcCrc((uint8_t *)un_tt.arr, ARR_SIZE_TT - 1) == un_tt.arr[ARR_SIZE_TT - 1])
    {
        if (un_tt.line.TP != 0)
        {
            BITSET(cRez, 0);
        }
    }
    //
    for (i = 0; i < ARR_SIZE_TT; i++)
    {
        un_tt.arr[i] = EEPROM_READ((uint8_t *)(ADR_TT_2 + i));
    }
    if (fnCalcCrc((uint8_t *)un_tt.arr, ARR_SIZE_TT - 1) == un_tt.arr[ARR_SIZE_TT - 1])
    {
        if (un_tt.line.TP != 0)
        {
            BITSET(cRez, 1);
        }
    }
    //
    for (i = 0; i < ARR_SIZE_TT; i++)
    {
        un_tt.arr[i] = EEPROM_READ((uint8_t *)(ADR_TT_3 + i));
    }
    if (fnCalcCrc((uint8_t *)un_tt.arr, ARR_SIZE_TT - 1) == un_tt.arr[ARR_SIZE_TT - 1])
    {
        if (un_tt.line.TP != 0)
        {
            BITSET(cRez, 2);
        }
    }
    //
    if (!cRez) // ������ �� �������
    {
        return cRez;
    }
    if (cRez == 7)
    {
        return 7;
    }
    else if (cRez != 7)// ���� ����� ������
    {
        if (BITTST1(cRez, 0)) // ������ �� ����� ������ �����
        {
            for (i = 0; i < ARR_SIZE_TT; i++)
            {
                un_tt.arr[i] = EEPROM_READ((uint8_t *)(ADR_TT_1 + i));
            }
        }
        if (BITTST1(cRez, 1)) // ������ �� ����� ������ �����
        {
            for (i = 0; i < ARR_SIZE_TT; i++)
            {
                un_tt.arr[i] = EEPROM_READ((uint8_t *)(ADR_TT_2 + i));
            }
        }
        if (BITTST1(cRez, 2)) // ������ �� ����� ������ �����
        {
            for (i = 0; i < ARR_SIZE_TT; i++)
            {
                un_tt.arr[i] = EEPROM_READ((uint8_t *)(ADR_TT_3 + i));
            }
        }
        fnWriteDataTTEeprom();//���������� ����� ������
        return cRez;
    }
    return cRez;
}
//--------------------------------------------------------


