
/*
*  Created on: 2016-12-18
*      Author: QUENTIN
*      E-mail: qiyuexin@yeah.net
*/
//###########################################################################
//
//  FILE:   myFPGAGlobalVarsDef.c
//
// TITLE:   FPGA 部分内存映射变量定义
//
//###########################################################################


#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File

#pragma DATA_SECTION(Motor1Regs,"Motor1RegsFiles");
volatile struct MOTORS_REGS Motor1Regs;

#pragma DATA_SECTION(Motor2Regs,"Motor2RegsFiles");
volatile struct MOTORS_REGS Motor2Regs;

#pragma DATA_SECTION(Motor3Regs,"Motor3RegsFiles");
volatile struct MOTORS_REGS Motor3Regs;

#pragma DATA_SECTION(Motor4Regs,"Motor4RegsFiles");
volatile struct MOTORS_REGS Motor4Regs;

#pragma DATA_SECTION(Motor5Regs,"Motor5RegsFiles");
volatile struct MOTORS_REGS Motor5Regs;

#pragma DATA_SECTION(Motor6Regs,"Motor6RegsFiles");
volatile struct MOTORS_REGS Motor6Regs;

#pragma DATA_SECTION(Motor7Regs,"Motor7RegsFiles");
volatile struct MOTORS_REGS Motor7Regs;

#pragma DATA_SECTION(Motor8Regs,"Motor8RegsFiles");
volatile struct MOTORS_REGS Motor8Regs;



