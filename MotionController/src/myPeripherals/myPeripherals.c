﻿/*
*  Created on: 2016-12-1
*      Author: QUENTIN
*      E-mail: qiyuexin@yeah.net
*/
//###########################################################################
//
//  FILE:   myPeripherals.c
//
// TITLE:   外设的初始化以及功能实现
//
//###########################################################################
// Peripheral 1: Cpu Timer is ok. @16:14 2016/12/2
// Peripheral 2: SCI ( UART )
// "*" means to do, and "+" means done.
// * uart_tx
// * uart_rx
// * uart_config

#include "my_project.h"

#if(USE_DMA_CH1)
	// Configure DMA Channel
	volatile Uint16 srcBuf[1024];
	volatile Uint16 dstBuf[1024];
	volatile Uint16 *DMACh1Dst = dstBuf;
	volatile Uint16 *DMACh1Src = srcBuf;
#endif

void InitPeripherals(void)
{
	// ==============================================================================
	//          Step 1. Initialize System Control:
	// ==============================================================================
	// PLL, WatchDog, enable Peripheral Clocks
	// This example function is found in the DSP2833x_SysCtrl.c file.
	InitSysCtrl();


	// ==============================================================================
	//          Step 2. Clear all interrupts and initialize PIE vector table.
	// ==============================================================================
	// 部分寄存器名字说明：
	// PIE: Peripheral Interrupt Expansion.
	// IER: Interrupt Enable Register
	// IFR: Interrupt Flag Register
	DINT;	// Disable CPU interrupts

	// 默认把PIEIER与PIEIFR全部清零。
	InitPieCtrl();

	// 默认把CPU核的IER与IFR全部清零。
	IER = 0x0000;
	IFR = 0x0000;

	// PieVectTable 结构体的定义在文件“DSP2833x_PieVect.h”里，去那里寻找对应的中断指针。
	// 重新定义中断向量表，如果你使用了中断，可以仿照以下格式定义自己中断服务函数。
	// 各外设的中断向量表重定义放到了Step3里。
	InitPieVectTable();	// 初始化中断向量表，这是必要的，即使你没有使用中断。这样做可以避免PIE引起的错误。

	// ==============================================================================
	//          Step 3. Initialize the Peripherals you used.
	// ==============================================================================
	InitGpio();     	// 把GPIO0~95设置为： 1，普通IO口，不使用特殊功能；2，输入方向；
//						// 3，CLK与SYSCLKOUT同步；4，使用上拉电阻；
	InitMyTask();   	// 初始化任务
	InitCpuTimers();   	// Peripheral 2: Cpu timer 初始化，if used.
	InitScis();			// Peripheral 3: SCI 初始化，if used.
	InitSpis();			// Peripheral 4: SPI 初始化，if used.
	InitDmas();			// Peripheral 5: DMA 初始化，if used.

	//
	// InitXintf();	// initializes the External Interface the default reset state.

	// InitAdc();			// Initializes ADC to a known state.

	// InitECan();			// Initialize eCAN-A module
	// InitECap();			// Initialize the eCAP(s) to a known state.
	// InitEPwm();			// Initializes the ePWM(s) to a known state.
	// InitEQep();	// Initializes the eQEP(s) to a known state.
	// InitI2C();	// initializes the I2C to a known state.
	// InitMcbsp();	// initializes the McBSP to a known state.


// ==============================================================================
// 			Step 4. Enable interrupts:
// ==============================================================================


	// Enable global Interrupts and higher priority real-time debug events:
	EnableInterrupts();


}



// 此次demo的任务（功能）初始化
void InitMyTask(void)
{
	//---------------- 1. GPIO 测试模式 ---------------------------------------------
#if( MY_TEST_DEMO == TEST_GPIO_TIMER_LED)
	EALLOW;

	/* output */
	LED2_DIR = 1;
	LED3_DIR = 1;
	LED4_DIR = 1;

	/* all off. */
	LED2    = LED_OFF;
	LED3    = LED_OFF;
	LED4    = LED_OFF;

	EDIS;
#elif( MY_TEST_DEMO == TEST_DMA )


	int i;
	for(i=0;i<1024;i++){
		dstBuf[i] = 0;
		srcBuf[i] = i;
	}


#endif

}



// 此次任务内容
void ExecuteMyTask(void)
{
#if( MY_TEST_DEMO == TEST_SCIA || MY_TEST_DEMO == TEST_SCIB || MY_TEST_DEMO == TEST_SCIC )
	TestSci();
#endif

#if( MY_TEST_DEMO == TEST_SPIA )
	TestSpi();
#endif

#if( MY_TEST_DEMO == TEST_DMA )
	while(1);
#endif

}