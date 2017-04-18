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
		MotorRegs[axis].MCTL.all = 9;	// no reset & EDITA
		MotorRegs[axis].MCONF.bit.DISPM = 1; // INxx ��ʾд�������
		if(axis>2){
			// ��Щ��û��ʹ��
			MotorRegs[axis].MCONF.bit.LIMITNV = 0;
			MotorRegs[axis].MCONF.bit.LIMITPV = 0;
		}
		MotorRegs[axis].MCTL.bit.EDITA = 0;
	}
}


// ������
void M_Arm(int axis){
	MotorRegs[axis].MCTL.bit.ENA = 1;
}

// ��ֹ��
void M_UnArm(int axis){
	MotorRegs[axis].MCTL.bit.ENA = 0;
}

// �򿪵��
void M_ServoOn(int axis){
	MotorRegs[axis].MCTL.bit.START = 1;
}

// �رյ��
void M_ServoOff(int axis){
	MotorRegs[axis].MCTL.bit.START = 0;
}

// ��ȡ��ǰ��Ŀ���ģʽ
enum MMODE_E M_GetCurMode(int axis){
	return MotorRegs[axis].MCTL.bit.MMODE;
}

// ���õ�ǰ��ΪPTģʽ
void M_SetPvatMode(int axis){
	MotorRegs[axis].MCTL.bit.MMODE = DDA_MODE;
}

// ����pvat������
void M_SetPvat( int axis, PVAT_S *dda){
	MotorRegs[axis].INPOS =  dda->aim_pos;
	MotorRegs[axis].INVEL =  dda->start_vel;
	MotorRegs[axis].INACC =  dda->start_acc;
	MotorRegs[axis].INXX  =  dda->min_period;
	MotorRegs[axis].MSTA.bit.NMSG = 1;	// push into the fifo of DDA.
}

// ��ȡ���е�DDA��������Ĵ�С
int M_GetfreeCmdSize(int axis)
{
	int wp = MotorRegs[axis].FFWP >> 3;
	int rp = MotorRegs[axis].FFRP;
	int cmdbufsize = 1 << 1+MotorRegs[axis].MCONF.bit.CBUFS;

	if (wp < rp)	return (rp - wp);
	else			return ( cmdbufsize - (wp - rp));

}

// ���õ�ǰ��ΪJOGģʽ
void M_SetJogMode(int axis){
	MotorRegs[axis].MCTL.bit.MMODE = JOG_MODE;
}

// ����vad ������
void M_SetVad( int axis, VAD_S *vad){
	MotorRegs[axis].INXX  =  vad->start_dec;
	MotorRegs[axis].INACC =  vad->start_acc;
	MotorRegs[axis].INVEL =  vad->aim_vel;
}



//// ����ʣ��ռ�
//int M_usedSpace(uint16_t wp, uint16_t rp)
//{
//	int fwp = wp >> 3;
//	int frp = rp;
//
//	if (fwp < frp) fwp += MRAM_SIZE;
//	return(MRAM_SIZE - (fwp - frp));
//}

