/*
*  Created on: 2016-12-18
*      Author: QUENTIN
*      E-mail: qiyuexin@yeah.net
*/
//###########################################################################
//
//  FILE:   mymotors.h
//
// TITLE:   FPGA电机部分
//
//###########################################################################


#ifndef MYMOTORS_H_
#define MYMOTORS_H_

//---------------------------------------------------
// Motor Status register bit definitions:
//
struct MSTA_BITS{
    uint16_t MBSY:1;    // 0,   Motor Busy, high active.
    uint16_t NMSG:1;    // 1,   Next Message
    uint16_t LMTN:1;    // 2,   limit- flag, high active.
    uint16_t LMTP:1;    // 3,   limit+ flag, high active.

    uint16_t rsvd:12;
  };

union MSTA_REG{
    uint16_t          all;
    struct MSTA_BITS  bit;
};

//---------------------------------------------------
// Motor Control register bit definitions:
//

struct MCTL_BITS{
    uint16_t RST:1;         // 0,   reset
    uint16_t ENA:1;         // 1,   enable
    uint16_t START:1;       // 2,   start
    uint16_t EDITA:1;       // 3,   Edit allow bit
    enum MMODE_E{			// 4-5, Motion Mode,
    	DDA_MODE=0, 		// 		00-DDA mode, 01-JOG mode.
    	JOG_MODE} MMODE:2;
    uint16_t rsvd2:10;
  };

union MCTL_REG{
    uint16_t          all;
    struct MCTL_BITS  bit;
};

//---------------------------------------------------
// Motor Configure register bit definitions:
//
struct MCONF_BITS{
    uint16_t LIMITNV:1;     // 0,   limit-'s active value
    uint16_t LIMITPV:1;     // 1,   limit+'s active value
    uint16_t LIMITDIS:1;    // 2,   1: limit function is disable
    uint16_t DISPM:1;     	// 3,   INxxx Display Mode
    uint16_t CBUFS:4;     	// 7-4, Command Buffer Size
    uint16_t rsvd2:8;
  };

union MCONF_REG{
    uint16_t           all;
    struct MCONF_BITS  bit;
};

//---------------------------------------------------
// Max Velocity register bit definitions:
//
struct MAXVEL_BITS{
	uint32_t maxvel:23;		// 0-22：最大速度，无符号。
	uint32_t rsvd:9;		// reserved.
};
union MAXVEL_REG{
	uint32_t 			all;
	struct MAXVEL_BITS bit;
};


//---------------------------------------------------
// Max Acceleration register bit definitions:
//
struct MAXACC_BITS{
	uint32_t acc_l:23;		// 0-22：低位，无符号。
	int32_t  acc_h:9;		//  高位，有符号。
};
union MAXACC_REG{
	uint32_t 			all;
	struct MAXACC_BITS bit;
};


//---------------------------------------------------------------------------
// Motor Register File:
//
struct MOTORS_REGS{
    // 0x00~0x0f
    int32_t     INPOS;   // IN Position
    int32_t     INVEL;   // velocity
    int32_t     INACC;   // acceleration
    int32_t     INXX;  	// jerk
    int32_t     NOWPOS;
    int32_t     NOWVEL;
    int32_t     NOWACC;
    int32_t     NOWXX;
    // 0x10~0x17
    int32_t     ENCP;	// Encode Position
    int32_t     ENCV;	// Encode Velocity
    union MAXVEL_REG     MAXVEL;	// Max Velocity
    union MAXACC_REG     MAXACC;	// Max Velocity

    // 0x18~0x1f
    uint16_t            FFWP;         // Fifo write Pointer
    uint16_t            FFRP;         // Fifo Read Pointer
    union MSTA_REG      MSTA;         // Motor Status register
    union MCTL_REG      MCTL;         // Motor Control register
    union MCONF_REG     MCONF;        // Motor Configure register
    uint16_t            rsvdRegs2[2];
    uint16_t 			MTCNT;
  };


extern volatile struct MOTORS_REGS MotorRegs[8];

void InitMotors(void);		// 初始化电机
/* 基本操作 */
void M_ServoOn(int axis);		// 打开电机
void M_ServoOff(int axis);		// 关闭电机
void M_Update(int axis);		// 更新运动指令。

/* 控制模式选择 */
enum MMODE_E M_GetCurMode(int axis);			// 获取当前轴的控制模式

/* PT 模式操作相关 */
void M_SetPvatMode(int axis);				// 设置当前轴为PT模式
int M_GetfreeCmdSize(int axis);			// 获取空闲的DDA命令缓存区的大小
void M_SetPvat( int axis, PVAT_S *dda);	// 输入pvat的数据

/* JOG 模式操作相关 */
void M_SetJogMode(int axis);			// 设置当前轴为JOG模式
void M_SetVad( int axis, int32_t aim_vel);	// 输入vad 的数据


//void testMyDAC(void);


#endif /* MOTORS_H_ */
