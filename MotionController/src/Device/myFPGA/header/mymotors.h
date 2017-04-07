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
    uint16_t MBSY:1;    // 0,   Motor Busy, high active.
    uint16_t NSTA:2;    // 2-1, Now Status of DDA
    uint16_t NMSG:1;    // 3,   Next Message
    uint16_t LMTN:1;    // 4,   limit- flag, high active.
    uint16_t LMTP:1;    // 5,   limit+ flag, high active.

    uint16_t rsvd:10;
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
    uint16_t rsvd2:12;
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
    uint16_t INDISPM:1;     // 2,   INxxx Display Mode

    uint16_t rsvd2:13;
  };

union MCONF_REG{
    uint16_t           all;
    struct MCONF_BITS  bit;
};

//---------------------------------------------------------------------------
// Da Register File:
//
struct MYDA_REGS_BITS{
	uint16_t data:16;
	uint16_t addr:3;
	uint16_t reg:3;
	uint16_t z:1;
	uint16_t rw:1;
	uint16_t we:1;		// bit 8,
	uint16_t load:1;		// bit 9,

	uint16_t rsvd:6;
  };

union MYDA_REGS{
	struct MYDA_REGS_BITS bit;
	uint32_t all;
};

//---------------------------------------------------------------------------
// Motor Register File:
//
struct MOTORS_REGS{
    // 0x00~0x0f
    int32_t     INPOS;   // IN Position
    int32_t     INVEL;   // velocity
    int32_t     INACC;   // acceleration
    int32_t     INJERK;  // jerk
    int32_t     NOWPOS;
    int32_t     NOWVEL;
    int32_t     NOWACC;
    int32_t     NOWJERK;
    // 0x10~0x17
    union MYDA_REGS   MYDA;
    uint16_t            rsvdRegs[6];
    // 0x18~0x1f
    uint16_t            FFWP;         // Fifo write Pointer
    uint16_t            FFRP;         // Fifo Read Pointer
    union MSTA_REG      MSTA;         // Motor Status register    
    union MCTL_REG      MCTL;         // Motor Control register
    union MCONF_REG     MCONF;        // Motor Configure register
    uint16_t            rsvdRegs2[2];
    uint16_t 	MTCNT;
  };


extern volatile struct MOTORS_REGS MotorRegs[AXIS_ITEM];

void InitMotors(void);

int M_usedSpace(uint16_t wp, uint16_t rp);
void MR_SetDDA( int axis, int32_t inpos, int32_t invel, int32_t inacc, int32_t injerk);

int M_freeSpace(int axis);
void M_SetDDA( int axis, DDA_VARS_S *dda);
void testMyDAC(void);


#endif /* MOTORS_H_ */
