/*
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

// 运动卡的EXTRAM用的zone6，EXTFLASH用的zone7, FPGA用的zone0.

// 运动卡的EXTRAM用的zone6，
#pragma DATA_SECTION(EXTRAM,"EXTRAM_DATA");
volatile Uint16 EXTRAM[0x8000];		// 片外RAM


// 运动卡的FPGA用的zone0，
// #pragma DATA_SECTION(EXTFPGA,"Motor2RegsFiles");
 volatile Uint16 EXTFPGA[0x20];	// 片外FPGA, 0x1000,4kB.


// Configure DMA Channel
#pragma DATA_SECTION(srcBuf,"DMARAML4");
volatile Uint16  srcBuf[1024];

#if( USE_DMA_CH1 )
// ch1 -> 读flash：srcBuf到dst0Buf
volatile Uint16 *DMACh1Dst = EXTFPGA;
volatile Uint16 *DMACh1Src = srcBuf;
#endif

#if( USE_DMA_CH2 )
// ch2 -> 写RAM：srcBuf到dst6Buf
volatile Uint16 *DMACh2Dst = EXTRAM;
volatile Uint16 *DMACh2Src = srcBuf;
#endif

#if( USE_DMA_CH3 )
// ch3 -> 写flash：srcBuf到dst7Buf
volatile Uint16 *DMACh3Dst = EXTFLASH;
volatile Uint16 *DMACh3Src = srcBuf;
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

	InitCpuTimers();   	// Peripheral 2: Cpu timer 初始化，if used.
	InitScis();			// Peripheral 3: SCI 初始化，if used.
	InitSpis();			// Peripheral 4: SPI 初始化，if used.
	InitXintf();		// initializes the External Interface the default reset state.
	InitDmas();			// Peripheral 5: DMA 初始化，if used.

	InitMotors();		// 初始化电机


	// StartDMACHx( &Dma.RegsAddr->CH1);
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



// 此次任务内容
void ExecuteMyTask(void)
{
#if( MY_TEST_DEMO == TEST_SCIA || MY_TEST_DEMO == TEST_SCIB || MY_TEST_DEMO == TEST_SCIC )
	TestSci();
#endif

#if( MY_TEST_DEMO == TEST_SPIA )
	TestSpi();
#endif

#if( MY_TEST_DEMO == TEST_DMA || MY_TEST_DEMO == TEST_XINTF)
	TestXintf();
	FlashSST39_Init();
	EXTFPGA_Test();
	while(1);
#endif


}
