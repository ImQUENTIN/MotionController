/*
*  Created on: 2016-12-18
*      Author: QUENTIN
*      E-mail: qiyuexin@yeah.net
*/
//###########################################################################
//
//  FILE:   FPGA_maps.cmd
//
// TITLE:   FPGA 内存分配图
//
//###########################################################################


MEMORY
{
 PAGE 0:    /* Program Memory */

 PAGE 1:    /* Data Memory */
   //ZONE0B	  : origin = 0x004000, length = 0x001000	// 用作与FPGA通信, 4KB, added by QUENTIN.
   MOTOR1     : origin = 0x004110, length = 0x000010    // Motor1
   MOTOR2     : origin = 0x004120, length = 0x000010    // Motor2
   MOTOR3     : origin = 0x004130, length = 0x000010    // Motor3
   MOTOR4     : origin = 0x004140, length = 0x000010    // Motor4
   MOTOR5     : origin = 0x004150, length = 0x000010    // Motor5
   MOTOR6     : origin = 0x004160, length = 0x000010    // Motor6
   MOTOR7     : origin = 0x004170, length = 0x000010    // Motor7
   MOTOR8     : origin = 0x004180, length = 0x000010    // Motor8

 }


SECTIONS
{
//   REM EXTFPGA_DATA     : > ZONE0B,     PAGE = 1		// added by QUENTIN.
   Motor1RegsFiles  : > MOTOR1,     PAGE = 1
   Motor2RegsFiles  : > MOTOR2,     PAGE = 1
   Motor3RegsFiles  : > MOTOR3,     PAGE = 1
   Motor4RegsFiles  : > MOTOR4,     PAGE = 1
   Motor5RegsFiles  : > MOTOR5,     PAGE = 1
   Motor6RegsFiles  : > MOTOR6,     PAGE = 1
   Motor7RegsFiles  : > MOTOR7,     PAGE = 1
   Motor8RegsFiles  : > MOTOR8,     PAGE = 1
   
}


/*
//===========================================================================
// End of file.
//===========================================================================
*/
