#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File

Uint16    *DualRamStart = (Uint16 *) 0x004000;
void sendResult()
{
	int i ;
	for(i = 0; i < 0xFFFF; i++)
	{
		*(DualRamStart + i) = i;
		if(*(DualRamStart + i) != i)
		{
			while(1);
		}
		if(*DualRamStart == 0x5000)
		 {
             while(1);
		 }
	}
}
