/*
*  Created on: 2016-12-18
*      Author: QUENTIN
*      E-mail: qiyuexin@yeah.net
*/
//###########################################################################
//
//  FILE:   mymotors.c
//
// TITLE:   FPGA电机部分
//
//###########################################################################


#include "DSP2833x_Device.h"

// set position
void Mymotor_SetInpos( int32_t inpos, volatile struct MOTORS_REGS *motor )
{
	motor->INPOSL = inpos;
	motor->INPOSH = inpos >> 16;
}

// set velocity
void Mymotor_SetInvel( int32_t invel, volatile struct MOTORS_REGS *motor )
{
	motor->INVELL = invel;
	motor->INVELH = invel >> 16;
}

// set acceleration
void Mymotor_SetInacc( int32_t inacc, volatile struct MOTORS_REGS *motor )
{
	motor->INACCL = inacc;
	motor->INACCH = inacc >> 16;
}

// read now position
int32_t Mymotor_ReadNowpos( volatile struct MOTORS_REGS *motor )
{
	int32_t tmp;
	tmp   = motor->NOWPOSH;
	tmp <<= 16;
	tmp  += motor->NOWPOSL;
	return tmp;
}


// read now velocity
int32_t Mymotor_ReadNowvel( volatile struct MOTORS_REGS *motor )
{
	int32_t tmp;
	tmp   = motor->NOWVELH;
	tmp <<= 16;
	tmp  += motor->NOWVELL;
	return tmp;
}


// read now acceleration
int32_t Mymotor_ReadNowacc( volatile struct MOTORS_REGS *motor )
{
	int32_t tmp;
	tmp   = motor->NOWACCH;
	tmp <<= 16;
	tmp  += motor->NOWACCL;
	return tmp;
}

// active the motor
void Mymotor_Active( volatile struct MOTORS_REGS *motor )
{
	motor->MOTORSTA.bit.active = 1;	// active the motor.
}

// revoke the motor
void Mymotor_Revoke( volatile struct MOTORS_REGS *motor )
{
	motor->MOTORSTA.bit.active = 0;	// Revoke the motor.
}


// loadSets
void Mymotor_LoadSets( volatile struct MOTORS_REGS *motor )
{
	motor->MOTORSTA.bit.start = 1;	// Revoke the motor.
	motor->MOTORSTA.bit.start = 0;	// Revoke the motor.
}

void Mymotor_Init( volatile struct MOTORS_REGS *motor )
{
	motor->MOTORSTA.bit.reset  = 1;		// reset
	motor->MOTORSTA.bit.start  = 0;
	motor->MOTORSTA.bit.active = 1;		// stop the motor.
	Mymotor_SetInpos(0, &Motor1Regs );
	Mymotor_SetInvel(0, &Motor1Regs );
	Mymotor_SetInacc(0, &Motor1Regs );
	motor->MOTORSTA.bit.start  = 1;		// start
}

void testMymotor(void)
{
	volatile int32_t tmp;

	Mymotor_Init( &Motor1Regs );
	tmp = Mymotor_ReadNowpos( &Motor1Regs );
	tmp = Mymotor_ReadNowvel( &Motor1Regs );
	tmp = Mymotor_ReadNowacc( &Motor1Regs );
	tmp = Motor1Regs.MOTORSTA.all;
	Mymotor_SetInpos(64000, &Motor1Regs );
	Mymotor_SetInvel(0, &Motor1Regs );
	Mymotor_SetInacc(0, &Motor1Regs );

	tmp = Mymotor_ReadNowpos( &Motor1Regs );
	tmp = Mymotor_ReadNowvel( &Motor1Regs );
	tmp = Mymotor_ReadNowacc( &Motor1Regs );
	tmp = Motor1Regs.MOTORSTA.all;
}
