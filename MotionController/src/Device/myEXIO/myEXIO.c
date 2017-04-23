/*
 * myEXIO.c
 *
 *  Created on: 2017-4-20
 *      Author: QIYUEXIN
 *  Contact me: qiyuexin@yeah.net
 */
#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File
#include "myEXIO.h"
#include "sysTypes.h"
#include "mymotors.h"

void InitEXIO(void)
{
	EALLOW;
	/*	EXI1~EXI8 : GPIO2   ~GPIO9
	* 	EXO1~EXO8 : GPIO10  ~GPIO17
	* 	17,16 	15,14,13,12 	11,10,9,8 	7-0
	*   11    	1111			1100		0000£¬0000
	*/
	// By default, all are GPIO Inputs
	GpioCtrlRegs.GPADIR.all = 0x03fc00;	// EXO, Íâ²¿Êä³ö¡£

   // Pull-ups can be enabled or disabled.
   GpioCtrlRegs.GPAPUD.all = 0x03fc00;      // Pullup's enabled GPIO0-GPIO31

	EDIS;

}

void EnterJogMode(int axis){
	MotorRegs[axis].MCTL.all = 3;
	MotorRegs[axis].MCTL.bit.MMODE = JOG_MODE;
	MotorRegs[axis].MCTL.bit.START = 1;
}

int prevZP, prevZN;
int prevYP, prevYN;
int prevXP, prevXN;

void testJog(int axis){

switch(axis){
	case 0:
		/*  Axis0 - X */
		if( prevXP && !M_JXP)			M_SetVad(axis, 6400);	// Key X+ Down
		else if( !prevXP && M_JXP)		M_SetVad(axis, 0);		// Key X+ Up

		if( prevXN && !M_JXN)			M_SetVad(axis,-6400);	// Key X- Down
		else if( !prevXN && M_JXN)		M_SetVad(axis, 0);		// Key X- Up
		prevXP = M_JXP;
		prevXN = M_JXN;
	break;
	case 1:
		/*  Axis1 - Y */
		if( prevYP && !M_JYP)			M_SetVad(axis,6400);	// Key Y+ Down
		else if( !prevYP && M_JYP)		M_SetVad(axis, 0);		// Key Y+ Up

		if( prevYN && !M_JYN)			M_SetVad(axis,-6400);	// Key Y- Down
		else if( !prevYN && M_JYN)		M_SetVad(axis, 0);		// Key Y- Up
		prevYP = M_JYP;
		prevYN = M_JYN;
	break;
	case 2:
		/*  Axis2 - Z */
		if( prevZP && !M_JZP)			M_SetVad(axis,40000);	// Key Z+ Down
		else if( !prevZP && M_JZP)		M_SetVad(axis, 0);		// Key Z+ Up

		if( prevZN && !M_JZN)			M_SetVad(axis,-40000);	// Key Z- Down
		else if( !prevZN && M_JZN)		M_SetVad(axis, 0);		// Key Z- Up
		prevZP = M_JZP;
		prevZN = M_JZN;
	break;
	default: break;
}



}

