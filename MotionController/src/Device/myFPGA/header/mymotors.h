/*
*  Created on: 2016-12-18
*      Author: QUENTIN
*      E-mail: qiyuexin@yeah.net
*/
//###########################################################################
//
//  FILE:   mymotors.h
//
// TITLE:   FPGA�������
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
    uint16_t rsvd2:14;
  };

union MCONF_REG{
    uint16_t           all;
    struct MCONF_BITS  bit;
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
    uint16_t            rsvdRegs[8];
    // 0x18~0x1f
    uint16_t            FFWP;         // Fifo write Pointer
    uint16_t            FFRP;         // Fifo Read Pointer
    union MSTA_REG      MSTA;         // Motor Status register    
    union MCTL_REG      MCTL;         // Motor Control register
    union MCONF_REG     MCONF;        // Motor Configure register
    uint16_t            rsvdRegs2[3];
  };


extern volatile struct MOTORS_REGS MotorRegs[AXIS_ITEM];

void InitMotors(void);
int M_usedSpace(uint16_t wp, uint16_t rp);
void MR_SetDDA( int axis, DDA_VARS_S *dda);
void M_SetDDA( int axis, int32_t inpos, int32_t invel, int32_t inacc, int32_t injerk);

#endif /* MOTORS_H_ */
