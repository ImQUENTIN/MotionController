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

#define AXIS_ITEM 8


//---------------------------------------------------
// Motor Status register bit definitions:
//
struct MSTA_BITS{
    uint16_t MBSY:1;        // Motor Busy
    uint16_t NSTA:2;		// Now Status of DDA
    uint16_t NMSG:1;		// Next Message
    uint16_t rsvd:12;
	};

union MSTA_REG{
	uint16_t              all;
	struct MSTA_BITS  bit;
};

//---------------------------------------------------
// Motor Control register bit definitions:
//
struct MCTL_BITS{
	uint16_t RST:1;         // reset
	uint16_t ENA:1;         // enable
	uint16_t PAUSE:1;       // PAUSE MODE

    uint16_t rsvd:4;
    uint16_t EDITA:1;      // Edit allow bit
    uint16_t rsvd2:8;
	};

union MCTL_REG{
	uint16_t              all;
	struct MCTL_BITS  bit;
};

//---------------------------------------------------------------------------
// Motor Register File:
//
struct MOTORS_REGS{
		
		int32_t			INPOS;   // IN Position
        int32_t			INVEL;   // velocity
		int32_t			INACC;   // acceleration
		int32_t			INJERK;  // jerk

		int32_t			NOWPOS;
		int32_t			NOWVEL;
		int32_t			NOWACC;
		int32_t			NOWJERK;

		uint16_t            rsvdRegs[8];
        uint16_t            FFWP;           // Fifo write Pointer
        uint16_t            FFRP;           // Fifo Read Pointer
        uint16_t            rsvdRegs2[4];
        union MCTL_REG	    MCTL;	        // Motor Control register
        union MSTA_REG	    MSTA;	        // Motor Status register
	};


extern volatile struct MOTORS_REGS MotorRegs[AXIS_ITEM];

// DDA 设置变量
struct DDA_SET_VARS{
	int32_t inpos;
	int32_t invel;
	int32_t inacc;
	int32_t injerk;
};

// DDA 信息变量
struct DDA_INFO_VARS{
	int32_t nowpos;
	int32_t nowvel;
	int32_t nowacc;
	int32_t nowjerk;
};

// test the motor.
void testMymotor(void);

#endif /* MOTORS_H_ */
