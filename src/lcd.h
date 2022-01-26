#ifndef lcd_h
#define lcd_h
#include<intrins.h>
#include "type.h"
#include "time.h"


void LcdWriteData(char dataW);
void LcdWriteCommand(uchar CMD,uchar AttribC);
void LcdReset(void);
void Display(uchar dd);
void DispOneChar(uchar x,uchar y,uchar Wdata);
void DispOneCharWithNoLocate(uchar x,uchar y,uchar Wdata);
void Putstr(uchar x,uchar y,uchar code *ptr);
void PutDataStr(uchar x,uchar y,uchar *ptr);
void LocateXY(char posx, char posy);

#endif