#include "time.h"

void time(uint ucMs) {
	#define DELAYTIMES 239
	byte ucCounter;
	while (ucMs!=0) {
		for (ucCounter=0;ucCounter<DELAYTIMES;ucCounter++){};
		ucMs--;
	}
}
