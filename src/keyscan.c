#include "keyscan.h"

uchar keyscan(void)
{
	uchar readkey,rereadkey;
	uchar x_temp,y_temp;
	
	KEY_PORT=0x0f;
	x_temp=KEY_PORT&0x0f;
	if (x_temp==0x0f) return(0xff);
	KEY_PORT=0xf0;
	y_temp=KEY_PORT&0xf0;
	readkey=x_temp|y_temp;
	time(10);
	KEY_PORT=0x0f;
	x_temp=KEY_PORT&0x0f;
	if (x_temp==0x0f) return(0xff);
	KEY_PORT=0xf0;
	y_temp=KEY_PORT&0xf0;
	rereadkey=x_temp+y_temp;
	if(readkey==rereadkey) return (~rereadkey);
	
	return (0xff);
}
