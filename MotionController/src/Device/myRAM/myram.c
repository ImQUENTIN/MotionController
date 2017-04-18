/*
 * myram.c
 *
 *  Created on: 2017-3-17
 *      Author: QIYUEXIN
 *  Contact me: qiyuexin@yeah.net
 */
#include "DSP2833x_Device.h"     	// DSP2833x Headerfile Include File
#include "sysTypes.h"
#include "CircleBuffer.h"
#include "string.h"
#include <math.h>
#include "mymotors.h"

#define DDA_RAM_SIZE 		0x80000/AXISNUM/8

// �˶�����EXTRAM�õ�zone6��
#pragma DATA_SECTION(cmd_ram,"EXTRAM_DATA");

//8 words;
CMD_VARS_S cmd_ram[AXISNUM][DDA_RAM_SIZE];	// 8k
CIRCLE_BUFFER_S cmd_buf[AXISNUM];

void EXTRAM_init(void) {
	int i;
	for (i = 0; i < AXISNUM; i++) {
		memset(&cmd_buf[i], 0, sizeof(CIRCLE_BUFFER_S));
		cmd_buf[i].dat = (int*) cmd_ram[i];
		cmd_buf[i].block_number = DDA_RAM_SIZE;
		cmd_buf[i].block_size = sizeof(CMD_VARS_S);
		cmd_buf[i].head = 0;
		cmd_buf[i].tail = 0;
	}

}
double constrain(double src, double min, double max) {
	return (src > max) ? max : (src < min) ? min : src;
}
void testPlot(void) {
	double STEP_X = (2.1 * 6400) / 100;   //���������������λmm
	double A = 30, B = 2 * 3.141593 / 80, D;	// sin ����
	double step_vel = 1e4, max_vel = 64e3;		// �ȶ������ٶ�

	double x, y, v, px, py, vx, vy, x_prev = 0, y_prev = 0, t_sec = 0;

	int x_start = 1, x_end = 100; // ��λmm

	PVAT_S pvat[AXISNUM];
	static int32_t pre_pos[AXISNUM];
	const int time_scale = 50;
	int axis = 0;

	D = A;

	for (x = x_start; x < x_end + 1; x+=1) {
		y = A * sin(B * x) + D;
		// ���������ٶ�
		vx = (x - x_prev) * STEP_X;		// delta x
		vy = (y - y_prev) * STEP_X;		// delta y
		v = sqrt(vx * vx + vy * vy);		// distance
		t_sec = (v / step_vel);			// time, seconds

		vx = vx / t_sec;					// vel x
		vy = vy / t_sec;					// vel y

		// ѹ��RAM
		memset(&pvat[0], 0, sizeof(PVAT_S) * AXISNUM);
		pre_pos[0] = pvat[0].aim_pos;
		pre_pos[1] = pvat[1].aim_pos;

		px = x * STEP_X;
		py = y * STEP_X;
		// ��������
		if(px>0) px = px + 0.5; else px = px - 0.5;
		if(py>0) py = py + 0.5; else py = py - 0.5;

		// ѹ��RAM
		pvat[0].aim_pos = (int32_t) px;
		pvat[0].start_vel = (int32_t) vx; //
		pvat[0].min_period = t_sec*1e6*time_scale;			// used as period:us

		pvat[1].aim_pos = (int32_t) py;
		pvat[1].start_vel = (int32_t) vy;
		pvat[1].min_period = t_sec*1e6*time_scale;			// used as period:us

		cb_append(&cmd_buf[0], &pvat[0]);
		cb_append(&cmd_buf[1], &pvat[1]);

		x_prev = x;
		y_prev = y;
	}



	for (axis = 0; axis < 2; axis++) {
		MotorRegs[axis].MCTL.all = 0;	// ��λ���
		MotorRegs[axis].MCTL.all = 3;
		//	INxxx ��ʾDSPд�������
		MotorRegs[axis].MCTL.bit.EDITA = 1;
		MotorRegs[axis].MCONF.bit.DISPM = 1;
		MotorRegs[axis].MCTL.bit.EDITA = 0;

		while (RTN_ERROR != cb_get(&cmd_buf[axis], &pvat[axis])) {
			// ȡ��axis, ѹ��DDA
			M_SetPvat(axis, &pvat[axis]);
		}
	}

	//	INxxx ��ʾ��һ��DDA�������
//	MotorRegs[axis].MCTL.bit.EDITA = 1;
//	MotorRegs[axis].MCONF.bit.INDISPM = 0;
//	MotorRegs[axis].MCTL.bit.EDITA = 0;

	for (axis = 0; axis < 2; axis++) {
		MotorRegs[axis].MCTL.bit.START = 1;
	}

	// ����ģʽ
	while(MotorRegs[0].FFRP != MotorRegs[0].FFWP>>3){

//			while( MotorRegs[0].MSTA.bit.MBSY);// && abs(MotorRegs[0].INPOS - MotorRegs[0].NOWPOS) < 500);
			// ͬʱ����
//			for (axis = 0; axis < 2; axis++) {
//				MotorRegs[axis].MCTL.bit.START = 1;
//			}
//
//			// ͬʱֹͣ
//			for (axis = 0; axis < 2; axis++) {
//				MotorRegs[axis].MCTL.bit.START = 0;
//			}
//
//			ESTOP0;	// stop here.
		}


	// test clear limit bit
{
		MotorRegs[0].MSTA.bit.LMTN = 1;
		MotorRegs[0].MSTA.bit.LMTP = 1;
	}

}

void EXTRAM_test(void) {
	uint16_t i, j;
	static PVAT_S tmp, tmp2;
	for (i = 0; i < DDA_RAM_SIZE - 1; i++) {
		// д��
		tmp.aim_pos = i;
		tmp.start_vel = i + 1;
		tmp.start_acc = i + 2;
		tmp.min_period = i + 3;
		for (j = 0; j < AXISNUM; j++)
			if (RTN_ERROR == cb_append(&cmd_buf[j], &tmp))
				asm("ESTOP0");
//		if( i == 8191) asm("ESTOP0");
	}

	for (; i < DDA_RAM_SIZE + 16; i++) {
		// ��⸲��д��
		tmp.aim_pos = i;
		tmp.start_vel = i + 1;
		tmp.start_acc = i + 2;
		tmp.min_period = i + 3;
		for (j = 0; j < AXISNUM; j++) {
			if (RTN_ERROR == cb_append(&cmd_buf[j], &tmp)) {
				cb_get(&cmd_buf[j], &tmp2);
				cb_append(&cmd_buf[j], &tmp);
			}
		}

	}
	//*****************************
	// ����ֱ�Ӳ鿴 cmd_ram ���ݡ�
	//*****************************

}

