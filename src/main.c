#include <stdio.h>
#include <intrins.h>
#include <STRING.H>
#include "uart.h"
#include "type.h"
#include "keyscan.h"
#include "lcd.h"
#include "msg.h"

uchar key_Value = 0xff;
bool Key_process();

char adminPwd[] = "1234";

#define PwdLength 4
char userPwd[PwdLength + 1] = {'\0'};
char userInput[PwdLength + 1] = {'\0'};
char reUserInput[PwdLength + 1] = {'\0'};
uchar len = 0;

sbit LEDGREEN = P2 ^ 0;
sbit LEDRED = P2 ^ 1;

void MyLcdInit()
{
    LcdReset();
    Putstr(0, 0, hello);
    LocateXY(0, 1);
}

void press(int r, int c, char *s)
{
    printf("[press] Key(R%d,C%d) is pressed, value is %s\n", r, c, s);
}

void logInput()
{
    if (len == 1)
    {
        MyLcdInit();
    }
    PutDataStr(0, 1, userInput);
    printf("[logInput] Now userInput buffer is: %s\n", userInput);
}

void number(char n)
{
    if (len < PwdLength)
    {
        userInput[len++] = n + '0';
    }
    else
    {
        printf("[number] userInput buffer is full!\n");
    }
    userInput[len] = '\0';

    logInput();
}

void back(void)
{
    // 1, 2, 3, 4, \0
    // 0, 1, 2, 3, 4
    if (len > 0)
    {
        len--;
    }
    userInput[len] = '\0';
    LcdReset();
    Putstr(0, 0, hello);
    logInput();
}

void enter(void)
{
    logInput();
}

void getInput()
{
    bool flag = TRUE;
    bool clear = TRUE;
    do
    {
        if (key_Value != 0xff)
        {
            flag = Key_process();
            key_Value = 0xff;
        }
    } while (flag);
    printf("[getInput] Get input: %s\n", userInput);
}

void clearInput()
{
    len = 0;
    userInput[len] = '\0';
}

bool setPwd(void)
{
    printf("[setPwd] Start!\n");
    MyLcdInit();
    Putstr(0, 1, setYourPwd);

    clearInput();
    getInput();
    strcpy(reUserInput, userInput);

    MyLcdInit();
    Putstr(0, 1, enterAgain);

    clearInput();
    getInput();

    if (strcmp(reUserInput, userInput) == 0)
    {

        if (len != 4)
        {
            MyLcdInit();
            Putstr(0, 1, pwdTooShort);
            printf("[setPwd] Passowrd length not enough!\n");
            time(3000);

            return FALSE;
        }

        if (strcmp(userInput, adminPwd) == 0)
        {
            MyLcdInit();
            Putstr(0, 1, pwdCannotUse);
            printf("[setPwd] Can not use admin pwd as user pwd!\n");
            time(3000);

            return FALSE;
        }

        strcpy(userPwd, userInput);
        printf("[setPwd] password set ok, is: %s\n", userPwd);
        clearInput();
        return TRUE;
    }
    MyLcdInit();
    Putstr(0, 1, pwdSetFail);
    printf("[setPwd] Two times inputs are not equal.\n");
    time(2000);
    clearInput();
    return FALSE;
}

void main(void)
{
    initUart();
    printf("Hello World!\n");
    printf("YuZhenxi-201904020209\n");

    TMOD = 0x10;
    TH1 = -10000 >> 8;
    TL1 = -10000 % 256;
    TCON = 0x40;
    IE = 0x88;
    key_Value = 0xff;

    LEDGREEN = 0;
    LEDRED = 1;
    time(1000);
    LEDGREEN = 1;
    LEDRED = 0;
    time(1000);
    LEDGREEN = 0;
    LEDRED = 1;

    MyLcdInit();

    printf("[main] Init Complate!\n");

    while (TRUE)
    {
        if (setPwd())
            break;
    }
    MyLcdInit();
    Putstr(0, 1, pwdSetOk);
    printf("[main] Password set ok!\n");
    time(3000);
    Putstr(0, 1, please);
    while (TRUE)
    {
        MyLcdInit();
        Putstr(0, 1, please);

        getInput();
        if (strcmp(userPwd, userInput) == 0)
        {
            LEDGREEN = 1;
            MyLcdInit();
            Putstr(0, 1, ok);
            printf("[main] Password right!\n");
            time(3000);
            LEDGREEN = 0;
        }
        else if (strcmp(adminPwd, userInput) == 0)
        {
            printf("[main] Admin Mode, you can set new password!\n");
            MyLcdInit();
            Putstr(0, 1, adminMode);
            time(2000);
            MyLcdInit();
            Putstr(0, 1, setYourPwd);

            if (setPwd())
            {
                MyLcdInit();
                Putstr(0, 1, pwdSetOk);
                printf("[main] Password set ok!\n");
                time(2000);
                MyLcdInit();
            }
            else
            {
                MyLcdInit();
                Putstr(0, 1, pwdSetFail);
                printf("[main] Password set fail, back to user mode!\n");
                time(2000);
                MyLcdInit();
            }
        }
        else
        {

            printf("[main] Password wrong!\n");
            MyLcdInit();
            Putstr(0, 1, fail);

            LEDRED = 0;
            time(3000);
            LEDRED = 1;

            MyLcdInit();
        }
        clearInput();
    }
}

void timer1int(void) interrupt 3
{
    EA = 0;
    TR1 = 0;
    TH1 = -10000 >> 8;
    TL1 = -10000 % 256;
    TR1 = 1;
    key_Value = keyscan();
    EA = 1;
}

bool Key_process()
{
    bool flag = TRUE;
    switch (key_Value)
    {
    case 0x11:
        press(1, 1, "1");
        number(1);
        break;
    case 0x21:
        press(1, 2, "2");
        number(2);
        break;
    case 0x41:
        press(1, 3, "3");
        number(3);
        break;

    // 2
    case 0x12:
        press(2, 1, "4");
        number(4);
        break;
    case 0x22:
        press(2, 2, "5");
        number(5);
        break;
    case 0x42:
        press(2, 3, "6");
        number(6);
        break;

    // 3
    case 0x14:
        press(3, 1, "7");
        number(7);
        break;
    case 0x24:
        press(3, 2, "8");
        number(8);
        break;
    case 0x44:
        press(3, 3, "9");
        number(9);
        break;

    // 4
    case 0x18:
        press(4, 1, "Back");
        back();
        break;
    case 0x28:
        press(4, 2, "0");
        number(0);
        break;
    case 0x48:
        press(4, 3, "Enter");
        flag = FALSE;
        break;
    }
    return flag;
}
