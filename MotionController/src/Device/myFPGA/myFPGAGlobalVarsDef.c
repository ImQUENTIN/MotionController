
/*
*  Created on: 2016-12-18
*      Author: QUENTIN
*      E-mail: qiyuexin@yeah.net
*/
//###########################################################################
//
//  FILE:   myFPGAGlobalVarsDef.c
//
// TITLE:   FPGA �����ڴ�ӳ���������
//
//###########################################################################


#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File

volatile struct MOTORS_REGS MotorRegs[AXIS_ITEM];

#pragma DATA_SECTION(MotorRegs,"MotorRegsFiles");


