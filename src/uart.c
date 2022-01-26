#include "uart.h"

void initUart(void) {
	SCON=0x50;
	RCAP2H=(65536-(3456/96))>>8; 
	RCAP2L=(65536-(3456/96))%256;
	T2CON=0x34;
	TI=1;
}

