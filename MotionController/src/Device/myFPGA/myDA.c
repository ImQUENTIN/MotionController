/*
 * DA.c
 *
 *  Created on: 2017-3-31
 *      Author: QIYUEXIN
 *  Contact me: qiyuexin@yeah.net
 */
#include "DSP2833x_Device.h"
#include "myDA.h"

void testMyDAC(void){
	union MYDA_REGS dac_cfg;
#if 0
	while(1){
	myDaRegs.low = 0x8000;
	myDaRegs.high = 0x0300;
	}
#else
	int i;

	// Step 1. Configure the power register
	dac_cfg.all = 0;
	dac_cfg.bit.DAC.all = 0x10000f;
	dac_cfg.bit.we = 1; // write
	myDaRegs = dac_cfg;

	// Step 2. Configure the output range
	dac_cfg.all = 0;
	dac_cfg.bit.DAC.all = 0x080004;
	dac_cfg.bit.we = 1; // write
	myDaRegs = dac_cfg;

	// Step 3. Configure the output range
//	dac_cfg.bit.DAC.all = 0x080004;
//	dac_cfg.bit.we = 1; // write
//	myDaRegs = dac_cfg;
//


	// Step 4. Configure the DAC register
	dac_cfg.all = 0;
	dac_cfg.bit.DAC.all = 0x004f00;
	dac_cfg.bit.we = 1; // write
	dac_cfg.bit.load = 1; // write
	myDaRegs = dac_cfg;

	dac_cfg.all = 0;
	dac_cfg.bit.DAC.all = 0x008f00;
	dac_cfg.bit.we = 1; // write
	dac_cfg.bit.load = 1; // write
	myDaRegs = dac_cfg;

	while(1)
		myDaRegs = dac_cfg;

	dac_cfg.all = 0;



	ESTOP0;

	// test read function:
	dac_cfg.bit.DAC.all = 0x800000;
	dac_cfg.bit.we = 1; // write
	myDaRegs = dac_cfg;

	for(i=0;i<200;i++);
	dac_cfg.bit.DAC.all = 0x180000;
	dac_cfg.bit.we = 1; // write
	myDaRegs = dac_cfg;

	dac_cfg = myDaRegs;
#endif
}
