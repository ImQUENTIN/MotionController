/*
 * myram.c
 *
 *  Created on: 2017-3-17
 *      Author: QIYUEXIN
 *  Contact me: qiyuexin@yeah.net
 */

#include "sysTypes.h"
#include "CircleBuffer.h"
#include "string.h"

#define DDA_RAM_SIZE 		0x80000/AXISNUM/8

// 运动卡的EXTRAM用的zone6，
#pragma DATA_SECTION(dda_stored,"EXTRAM_DATA");


//8 words;
DDA_VARS_S dda_stored[AXISNUM][DDA_RAM_SIZE];	// 8k
CIRCLE_BUFFER_S ram_dda[AXISNUM];

void EXTRAM_init(void)
{
	int i;
	for( i=0; i<AXISNUM; i++){
		memset(&ram_dda[i], 0, sizeof(CIRCLE_BUFFER_S));
		ram_dda[i].dat = (int*)dda_stored[i];
		ram_dda[i].block_number = DDA_RAM_SIZE;
		ram_dda[i].block_size = sizeof(DDA_VARS_S);
		ram_dda[i].head =0;
		ram_dda[i].tail =0;
	}

}

void EXTRAM_test(void)
{
	uint16_t i,j;
	static DDA_VARS_S test[AXISNUM][16],tmp,tmp2;
	for( i=0; i<DDA_RAM_SIZE-1; i++){
		// 写满
		tmp.pos = i;
		tmp.vel = i+1;
		tmp.acc = i+2;
		tmp.jerk = i+3;
		for(j=0;j<AXISNUM;j++)
			if( RTN_ERROR == cb_append(&ram_dda[j], &tmp))
				asm("ESTOP0");
//		if( i == 8191) asm("ESTOP0");
	}

	for( ;i<DDA_RAM_SIZE+16; i++){
		// 检测覆盖写入
		tmp.pos = i;
		tmp.vel = i+1;
		tmp.acc = i+2;
		tmp.jerk = i+3;
		for(j=0;j<AXISNUM;j++){
			if (RTN_ERROR == cb_append(&ram_dda[j], &tmp)){
				cb_get(&ram_dda[j], &tmp2);
				cb_append(&ram_dda[j], &tmp);
			}
		}

	}
	//*****************************
	// 可以直接查看 dda_stored 数据。
	//*****************************
//	for( i=0; i<16; i++){
//		for(j=0;j<AXISNUM;j++)
//		cb_get(&ram_dda[j], &test[j][i]);
//	}
}

