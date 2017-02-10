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
/* 
    SPI 使用说明：
    1. 关于FIFO长度
    SPIA 内部使用了FIFO中断，默认的缓存长度是 16(硬件) + 16（软件），32字长。
    其中软件的缓存的DEPTH可以通过修改变量SPIA_SWFFTXDEEP, SPIA_SWFFRXDEEP
    来自定义设置，在这里修改：line 118-120, @ my_demo_select.h 
    
    2. 关于发送接收SPI的使用
    SPIA 使用收发缓存的形式，你可以直接使用函数 uint8_t Spia_gets(int8_t * msg)
    来读取接收的数据，msg的长度应大于16+SPIA_SWFFRXDEEP。
    当没有数据接收到时，Spia_gets 返回1；返回0说明msg信息有效可以进一步处理。
    注意：Spia_gets 会自动在msg的最后一个写0封尾，所以最后一位的数值是0时要注意。
    
    发送的话，使用Spia_puts(int8_t * msg);
    
    3. 缓存满的情况
    a) 如果没有即使清空缓存，当接收缓存满了之后将不会再接收新的数据，直到有空闲
       的缓存存放数据为止。
    b) 如果发送一帧数据时，超出了发送缓存区的大小，返回字符串截断的位置。
       比如返回j，当发送缓存区清空后，下一帧数据可以这么发送Spia_puts(msg+j);
       
       建议把缓存器调大，适合使用。
    

 */


	InitPeripherals();	// as you see.
	ExecuteMyTask();
	return 0;
}

//===========================================================================
// No more.
//===========================================================================
