#include "lcd.h"
#include<intrins.h>
#include<stdio.h>
#define uchar unsigned char
uchar xdata Lcd1602CmdPort _at_ 0xFFF0;
uchar xdata Lcd1602StatusPort _at_ 0xFFF1;
uchar xdata Lcd1602WdataPort _at_ 0xFFF2;
uchar xdata Lcd1602RdataPort _at_ 0xFFF3;
#define uchar unsigned char
#define BUSY 0x80

void Putstr(uchar x, uchar y, uchar code *ptr)
{
    uchar i, l = 0;
    while (ptr[l] > 31)
    {
        l++;
    };
    for (i = 0; i < l; i++)
    {
        DispOneChar(x++, y, ptr[i]);

        if (x == 16)
        {
            x = 0;
            y ^= 1;
        }
    }
}

void PutDataStr(uchar x, uchar y, uchar *ptr)
{
	uchar i, l = 0;
    while (ptr[l] > 31)
    {
        l++;
    };
    for (i = 0; i < l; i++)
    {
        DispOneChar(x++, y, ptr[i]);

        if (x == 16)
        {
            x = 0;
            y ^= 1;
        }
    }
}

void Display(uchar dd)
{
    uchar i, l;
    for (i = 0; i < 16; i++)
    {
        DispOneChar(i, l, dd++);
        dd &= 0x7f;
        if (dd < 32)
            dd = 32;
    }
}

void LocateXY(char posx, char posy)
{
    uchar temp;
    temp = posx & 0xf;
    posy &= 0x1;
    if (posy)
        temp |= 0x40;
    temp |= 0x80;
    LcdWriteCommand(temp, 0);
}

void DispOneChar(uchar x, uchar y, uchar Wdata)
{
    LocateXY(x, y);
    LcdWriteData(Wdata);
}

void LcdReset(void)
{
    time(400);
    LcdWriteCommand(0x38, 0);
    while (Lcd1602StatusPort & BUSY)
        ;
    LcdWriteCommand(0x08, 1);
    LcdWriteCommand(0x06, 1);
    LcdWriteCommand(0x0f, 1);
    LcdWriteCommand(0x01, 1);
}

void LcdWriteCommand(uchar CMD, uchar AttribC)
{
    if (AttribC)
        while (Lcd1602StatusPort & BUSY)
            ;
    Lcd1602CmdPort = CMD;
}

void LcdWriteData(char dataW)
{
    while (Lcd1602StatusPort & BUSY);
    Lcd1602WdataPort = dataW;
}
