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
   MOTOR      : origin = 0x004000, length = 0x000100    // Motor1 ~ 8
   //NEXT      : origin = 0x004100, length = 0x000100    // Motor1 ~ 8

 }


SECTIONS
{
//   REM EXTFPGA_DATA     : > ZONE0B,     PAGE = 1		// added by QUENTIN.
   MotorRegsFiles  : > MOTOR,     PAGE = 1

}


/*
//===========================================================================
// End of file.
//===========================================================================
*/
