/*
 * my_demo_select.h
 *
 *  Created on: 2016-12-3
 *      Author: Administrator
 */

#ifndef MY_DEMO_SELECT_H_
#define MY_DEMO_SELECT_H_


//============================================================================
//		    所有的测试模式
//============================================================================
/* ALL TEST DEMO */
#define EMPTY_PROJECT       	0       // 空的工程
#define TEST_GPIO_TIMER_LED 	1       // 测试GPIO,基于Timer，利用LED看现象。
#define TEST_SCIA           	2       // 测试SCIA，即uart通信。
#define TEST_SCIB           	22      // 测试SCIB，运动控制板卡接口。
#define TEST_SCIC           	23      // 测试SCIA，即uart通信。
#define TEST_SPIA           	3       // 测试SPI
#define TEST_DMA				4		// 测试DMA
#define TEST_XINTF				5		// 测试XINTF


//###########################################################################
//						 DEMO SELECT
//###########################################################################
#define MY_TEST_DEMO TEST_SPIA		// 测试模式的选择



#if( MY_TEST_DEMO == TEST_GPIO_TIMER_LED)
//============================================================================
//      Example 1. GPIO 测试模式
//============================================================================
// 实验现象： 3个LED灯轮流闪烁，以不同的频率。每个LED由一个Cpu Timer控制。
// 使用GPIO点亮LED的例子，基于YH的DSP开发板。如果不是该开放板，修改下面的
// 管脚宏定义，就行了。
//

// 使用到的外设
#define USE_CPU_TIMER0  1	// 使用Cpu Timer0
#define USE_CPU_TIMER1  1	// 使用Cpu Timer1
#define USE_CPU_TIMER2  1	// 使用Cpu Timer2

/* 说明
    硬件管脚图：
        LED2 -> D15 -> GPIO64
        LED3 -> D14 -> GPIO65
        LED4 -> D13 -> GPIO66
 */
#define LED_ON      1
#define LED_OFF     0

//-----------------------------------------------------------
//  Step 1. Choose your LED Port
//-----------------------------------------------------------
#define LED2 	    GpioDataRegs.GPCDAT.bit.GPIO64
#define LED3 	    GpioDataRegs.GPCDAT.bit.GPIO65
#define LED4 	    GpioDataRegs.GPCDAT.bit.GPIO66

#define LED2_DIR    GpioCtrlRegs.GPCDIR.bit.GPIO64
#define LED3_DIR    GpioCtrlRegs.GPCDIR.bit.GPIO65
#define LED4_DIR    GpioCtrlRegs.GPCDIR.bit.GPIO66

#define LED2_TOG	GpioDataRegs.GPCTOGGLE.bit.GPIO64
#define LED3_TOG	GpioDataRegs.GPCTOGGLE.bit.GPIO65
#define LED4_TOG	GpioDataRegs.GPCTOGGLE.bit.GPIO66

//-----------------------------------------------------------
//  Step 2. Choose your LED Flicker frequency
//-----------------------------------------------------------
#define 	CPU_TIMER0_PRD	200000
#define 	CPU_TIMER1_PRD	400000
#define 	CPU_TIMER2_PRD	800000


#elif( MY_TEST_DEMO == TEST_SCIA )
//============================================================================
//      Example X. SCIA 测试模式
//============================================================================
// 实验现象： 默认测试swFIFO，会输出检测到的字符串
//

#define USE_SCIA        		1   		// 使用 SCIA
// 测试端口，ok.
//#define USE_GPIO28_AS_SCIRXDA 	1			// RX - GPIO28
//#define USE_GPIO29_AS_SCITXDA 	1			// TX - GPIO29
#define SCIA_BAUD 				115200	 	    // SCIA BAUD = 115200

//============================================================================
//      Example X. SCIB 测试模式
//============================================================================
#elif( MY_TEST_DEMO == TEST_SCIB )
#define SCIB_BAUD 				256000
#define USE_GPIO19_AS_SCIRXDB 1
#define USE_GPIO18_AS_SCITXDB 1

//============================================================================
//      Example X. SCIC 测试模式
//============================================================================
#elif( MY_TEST_DEMO == TEST_SCIC )
#define USE_SCIC        		1   		// 使用 SCIC
#define SCIC_BAUD 				256000



//============================================================================
//      Example X. SPI 测试模式
//============================================================================
// 实验现象：
//
#elif( MY_TEST_DEMO == TEST_SPIA )
#define USE_SPIA 		1
// #define USE_GPIO19_AS_SPISTEA    1  // jzs 的板子该行注释掉


//============================================================================
//      Example X. DMA 测试模式
//============================================================================
// 实验现象：
//
#elif( MY_TEST_DEMO == TEST_DMA )
#define USE_DMA   		1
#define USE_DMA_CH1		1
#define USE_CPU_TIMER0  1



#elif( MY_TEST_DEMO == TEST_XINTF )
//============================================================================
//      Example X. XXX 测试模式
//============================================================================
// 实验现象：
//
#define USE_CPU_TIMER0  	1

#define USE_DMA   			1
#define USE_DMA_CH1			1
#define USE_DMA_CH2			1
//#define USE_DMA_CH3			1

#define USE_XINTF_ZONE0		1
#define USE_XINTF_ZONE6		1
#define USE_XINTF_ZONE7		1


//#elif( MY_TEST_DEMO == TEST_SCIA_FIFO )

//============================================================================
//      Example X. XXX 测试模式
//============================================================================
// 实验现象：
//
#endif /* MY_TEST_DEMO*/


#endif /* MY_DEMO_SELECT_H_ */
