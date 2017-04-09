/*
*  Created on: 2016-12-18
*      Author: QUENTIN
*      E-mail: qiyuexin@yeah.net
*/
//###########################################################################
//
//  FILE:   mymotors.c
//
// TITLE:   FPGA�������
//
//###########################################################################


#include "DSP2833x_Device.h"
#include "sysTypes.h"
#include "mymotors.h"
#include <string.h>

#define ONE_CIRCLE 6400


void InitMotors(void)
{
	int axis;
	for(axis=0; axis<AXISNUM; axis++){
		MotorRegs[axis].MCTL.all = 0;
		MotorRegs[axis].MCTL.all = 9;
		MotorRegs[axis].MCONF.bit.INDISPM = 1; // INxx ��ʾд�������
		if(axis>2){
			// ��Щ��û��ʹ��
			MotorRegs[axis].MCONF.bit.LIMITNV = 0;
			MotorRegs[axis].MCONF.bit.LIMITPV = 0;
		}
		MotorRegs[axis].MCTL.bit.EDITA = 0;
	}
}


// �ļ��ڲ�����ʹ��
void MR_SetDDA( int axis, int32_t inpos, int32_t invel, int32_t inacc, int32_t injerk){

	MotorRegs[axis].INPOS =  inpos;
	MotorRegs[axis].INVEL =  invel;
	MotorRegs[axis].INACC =  inacc;
	MotorRegs[axis].INJERK = injerk;

	MotorRegs[axis].MSTA.bit.NMSG = 1;	// push into the fifo of DDA.
}

void M_SetDDA( int axis, DDA_VARS_S *dda){

	MotorRegs[axis].INPOS =  dda->pos;
	MotorRegs[axis].INVEL =  dda->vel;
	MotorRegs[axis].INACC =  dda->acc;
	MotorRegs[axis].INJERK = dda->jerk;
//	if( memcmp((uint16_t *)&MotorRegs[axis].INPOS, (uint16_t *)&dda->pos, 8) )
//		ESTOP0;	//У�鲻һ�£��������
//	else
		MotorRegs[axis].MSTA.bit.NMSG = 1;	// push into the fifo of DDA.
}

// ����ʣ��ռ�
int M_usedSpace(uint16_t wp, uint16_t rp)
{
	int fwp = wp >> 3;
	int frp = rp;

	if (fwp < frp) fwp += MRAM_SIZE;
	return(MRAM_SIZE - (fwp - frp));
}
int M_freeSpace(int axis)
{

	int wp = MotorRegs[axis].FFWP >> 3;
	int  rp = MotorRegs[axis].FFRP;

	if (wp < rp) wp += MRAM_SIZE;
	return (MRAM_SIZE - (wp - rp));

}


