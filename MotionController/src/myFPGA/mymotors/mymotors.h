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

// bit-0(R/W): rst
// bit-1(R/W): active
// bit-2(R/W): start
// bit-3( R ): busy
// bit-4:15  : reserved.
//    `define INPL_ADDR       'h1      // inpos(R/W), LSbyte Register
//    `define INPH_ADDR       'h2      // inpos(R/W), MSbyte Register
//    `define INVL_ADDR       'h3      // invel(R/W), LSbyte Register
//    `define INVH_ADDR       'h4      // invel(R/W), MSbyte Register
//    `define INAL_ADDR       'h5      // inacc(R/W), LSbyte Register
//    `define INAH_ADDR       'h6      // inacc(R/W), MSbyte Register
//    `define NOWPL_ADDR      'h7      // nowpos(R), LSbyte Register
//    `define NOWPH_ADDR      'h8      // nowpos(R), MSbyte Register
//    `define NOWVL_ADDR      'h9      // nowvel(R), LSbyte Register
//    `define NOWVH_ADDR      'ha      // nowvel(R), MSbyte Register
//    `define NOWAL_ADDR      'hb      // nowacc(R), LSbyte Register
//    `define NOWAH_ADDR      'hc      // nowacc(R), MSbyte Register
//    `define rsvdd_ADDR      'hd      // reserved zone d
//    `define rsvde_ADDR      'he      // reserved zone e
//    `define rsvdf_ADDR      'hf      // reserved zone f

//---------------------------------------------------
// Motor status register bit definitions:
//
struct MOTORSTA_BITS{
	uint16_t reset:1;
    uint16_t active:1;
    uint16_t start:1;
    uint16_t busy:1;        // read only.
    uint16_t rsvd:12;
	};

union MOTORSTA_REG{
	uint16_t              all;
	struct MOTORSTA_BITS  bit;
};

//---------------------------------------------------------------------------
// SCI Register File:
//
struct MOTORS_REGS{
		union MOTORSTA_REG	MOTORSTA;	// Motor status register
		uint16_t			INPOSH;
		uint16_t			INPOSL;
        uint16_t			INVELH;
		uint16_t			INVELL;
        uint16_t			INACCH;
		uint16_t			INACCL;
        uint16_t			NOWPOSH;
		uint16_t			NOWPOSL;
        uint16_t			NOWVELH;
		uint16_t			NOWVELL;
        uint16_t			NOWACCH;
		uint16_t			NOWACCL;
	};

extern volatile struct MOTORS_REGS Motor1Regs;
extern volatile struct MOTORS_REGS Motor2Regs;
extern volatile struct MOTORS_REGS Motor3Regs;
extern volatile struct MOTORS_REGS Motor4Regs;
extern volatile struct MOTORS_REGS Motor5Regs;
extern volatile struct MOTORS_REGS Motor6Regs;
extern volatile struct MOTORS_REGS Motor7Regs;
extern volatile struct MOTORS_REGS Motor8Regs;

// set position
void Mymotor_SetInpos( int32_t inpos, volatile struct MOTORS_REGS *motor );
// set velocity
void Mymotor_SetInvel( int32_t invel, volatile struct MOTORS_REGS *motor );
// set acceleration
void Mymotor_SetInacc( int32_t inacc, volatile struct MOTORS_REGS *motor );
// read now position
int32_t Mymotor_ReadNowpos( volatile struct MOTORS_REGS *motor );
// read now velocity
int32_t Mymotor_ReadNowvel( volatile struct MOTORS_REGS *motor );
// read now acceleration
int32_t Mymotor_ReadNowacc( volatile struct MOTORS_REGS *motor );

// test the motor.
void testMymotor(void);

#endif /* MOTORS_H_ */
