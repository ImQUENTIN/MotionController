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
    uint16_t rsvd:15;
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
	uint16_t START:1;       // start motion

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
		
		uint16_t			INPL;   // now Position Low 16-bit Register
        uint16_t			INPH;   // In Position High 16-bit Register
        uint16_t			INVL;   // velocity
        uint16_t			INVH;   
		uint16_t			INAL;   // acceleration
        uint16_t			INAH;
		uint16_t			INJL;   // jerk
        uint16_t			INJH;
		
        uint16_t			NOWPL;  // Now Position
        uint16_t			NOWPH;
		uint16_t			NOWVL;
        uint16_t			NOWVH;
		uint16_t			NOWAL;
        uint16_t			NOWAH;
		uint16_t			NOWJL;
        uint16_t			NOWJH;

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
