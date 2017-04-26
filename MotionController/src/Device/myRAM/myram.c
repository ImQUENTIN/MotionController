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

// 运动卡的EXTRAM用的zone6，
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


void EXTRAM_test(void) {
	uint16_t i, j;
	static PVAT_S tmp, tmp2;
	for (i = 0; i < DDA_RAM_SIZE - 1; i++) {
		// 写满
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
		// 检测覆盖写入
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
	// 可以直接查看 cmd_ram 数据。
	//*****************************
}


