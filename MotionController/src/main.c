 /*
 *  Created on: 2016-12-1
 *      Author: QUENTIN
 *      E-mail: qiyuexin@yeah.net
 */
//###########################################################################
//
// FILE:  main.c
//
//
// TITLE:   用DSP点亮发光二极管实验
//
//###########################################################################
//
//    根据在RAM中调试的需要，这个项目配置成"boot to SARAM".2833x引导模式
//    表如下显示. 常用的还有"boot to Flash"模式，当程序在RAM调试完善后就
//    可以将代码烧进Flash中并使用"boot to Flash"引导模式.
//
//       $Boot_Table:
//
//         GPIO87   GPIO86     GPIO85   GPIO84
//          XA15     XA14       XA13     XA12
//           PU       PU         PU       PU
//        ==========================================
//            1        1          1        1    Jump to Flash
//            1        1          1        0    SCI-A boot
//            1        1          0        1    SPI-A boot
//            1        1          0        0    I2C-A boot
//            1        0          1        1    eCAN-A boot
//            1        0          1        0    McBSP-A boot
//            1        0          0        1    Jump to XINTF x16
//            1        0          0        0    Jump to XINTF x32
//            0        1          1        1    Jump to OTP
//            0        1          1        0    Parallel GPIO I/O boot
//            0        1          0        1    Parallel XINTF boot
//            0        1          0        0    Jump to SARAM	    <- "boot to SARAM"
//            0        0          1        1    Branch to check boot mode
//            0        0          1        0    Boot to flash, bypass ADC cal
//            0        0          0        1    Boot to SARAM, bypass ADC cal
//            0        0          0        0    Boot to SCI-A, bypass ADC cal
//                                              Boot_Table_End$
//
// 功能描述:
//
//    本例程配置 CPU定时器0为500ms周期, 每次周期中断取反 GPIO66
//    取反后LED就在亮和灭之间转换. 为了便于观察，本例还增加了一个计数变量CpuTimer0.InterruptCount
//    每次中断计数变量+1；
//
//       Watch Variables:
//          CpuTimer0.InterruptCount
//
//       Monitor the GPIO32 LED blink on (for 500 msec) and off (for 500 msec) on the 2833x eZdsp.
//
//###########################################################################

#include "my_project.h"


//===========================================================================
// No more.
//===========================================================================

int main(void)
{
	InitPeripherals();	// as you see.
	ExecuteMyTask();
	return 0;
}

//===========================================================================
// No more.
//===========================================================================
