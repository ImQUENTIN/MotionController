/*
*  Created on: 2016-12-2
*      Author: QUENTIN
*      E-mail: qiyuexin@yeah.net
*/
//###########################################################################
//
//  FILE:   myPeripherals_config.h
//
// TITLE:   我的片内外设配置文件
//
//###########################################################################

#ifndef QYX_PERIPHERALS_CONFIG_H_
#define QYX_PERIPHERALS_CONFIG_H_


#ifdef __cplusplus
extern "C" {
#endif



//############################################################################
//
//		   PART x: 设备篇（目标板配置及选择）
//
//############################################################################

//----------------------------------------------------------
// Step1. Choose your target device. (only choose one)
//----------------------------------------------------------
//  这个去DSP2833x_Device.h.h 文件中定义


//############################################################################
//
//		   PART x: 时钟篇
//
//############################################################################
// 简单地说DSP的外部时钟源（XCLKIN）先经PLL倍频（在PLLCR里配置），再经DIV分频
// 输出CLKIN，最后经过CPU core 输出成SYSCLKOUT，这里的SYSCLKOUT与CLKIN频率相同。
// 这里有个重要的原则：不管前面怎么配置我们系统系统主频最高，即150Mhz。
// 所以，SYSCLKOUT = 150Mhz 但是也有芯片（比如F28332最大支持100Mhz频率）
//
//============================================================================
//      Section 1: System Clock, 系统时钟。
//============================================================================
//----------------------------------------------------------
// Step1. configure your CLKIN frequency.
//----------------------------------------------------------
// 定义你的晶振频率，一般使用30Mhz晶振。
#define OSCCLK_MHZ		30

//----------------------------------------------------------
// Step2. Choose SYSCLKOUT Frequency by DIVSEL and PLLCR.
//----------------------------------------------------------
//        PLLSTS[DIVSEL]   SYSCLKOUT
//        0, 1             OSCCLK*PLLCR/4
//        2                OSCCLK*PLLCR/2
// OSCCLK 是晶振的频率，你可以通过上面的公式，修改DIVSEL和PLLCR的值从而达到你想要
// 的系统主频的要求，一般我们希望系统主频为150Mhz，而晶振为30Mhz，所以，
// DIVSEL = 2, PLLCR = 10.
//  TMS320F28335,334,235,234 最大可以支持150Mhz的主频；
//  TMS320F25232,332 最大可以支持100Mhz的主频。
//  这里会根据你选定的晶振来配置最大的SYS_CLK的主频。

#if(DSP28_28335)

#if(OSCCLK_MHZ == 30)
// 30Mhz*10/2 = 150Mhz.
#define DSP28_DIVSEL  		2
#define DSP28_PLLCR   		10
#define SYSCLKOUT_MHZ		150		// 150 MHz CPU Freq (30 MHz input freq) by DEFAULT
#define CPU_RATE    		6.667L	// support for DelayUs.
#endif

#elif(DSP28_28332)
#if(OSCCLK_MHZ == 30)
// 30Mhz*6/2 = 90Mhz.
#define DSP28_DIVSEL  		2
#define DSP28_PLLCR   		6
#define SYSCLKOUT_MHZ		90		// 90 MHz CPU Freq (30 MHz input freq)
#define CPU_RATE    		11.111L	// support for DelayUs.
#endif

#else
#error no target device defined.
#endif

//----------------------------------------------------------
// Step3. Calculate the SYSCLK's frequency.
//          ** DO NOT CHANGE HERE **
//----------------------------------------------------------
#define SYSCLK_HZ		(SYSCLKOUT_MHZ*1E6)		// 90Mhz

//============================================================================
//      Section 2: Peripheral clock Frequency, 外设时钟频率
//============================================================================
// 此外，还有两个很重要的时钟源：LSPCLK（低速外设时钟）和HSPCLK（高速设备时钟）。
// Prescale  -   Frequency (1,2,4,,14)
//  000      -      /1
//  001      -      /2
//  010      -      /4
//  ...      -      /6,8,12
//  111      -      /14
// 默认的情况下：  LSPCLK = SYSCLKOUT/4 = 37.5Mhz;
//   (150Mhz)      HSPCLK = SYSCLKOUT/2 = 75Mhz;
//  TMS320F28335,334,235,234 最大可以支持150Mhz的HSPCLK, 75Mhz的LSPCLK；
//  TMS320F25232,332 最大可以支持100Mhz的HSPCLK, 50Mhz的LSPCLK；

//----------------------------------------------------------
// Step1. choose HISPCP & LOSPCP for Peripheral clock.
//----------------------------------------------------------
#if(DSP28_28335 && (OSCCLK_MHZ == 30) )
#define DSP28_HISPCP	1	// 1->75Mhz	 , while SYSCLK_HZ = 150MHZ.
#define DSP28_LOSPCP	2	// 2->37.5Mhz, while SYSCLK_HZ = 150MHZ.
#elif(DSP28_28332 && (OSCCLK_MHZ == 30) )
#define DSP28_HISPCP	0	// 0->90MHZ, while SYSCLK_HZ = 90MHZ.
#define DSP28_LOSPCP	1	// 1->45MHZ, while SYSCLK_HZ = 90MHZ.
#else
#error No suitable combination of crystals
#endif

//----------------------------------------------------------
// Step2. calculate the HSPCLK&LSPCLK's frequency.
//          ** DO NOT CHANGE HERE **
//----------------------------------------------------------
#if( DSP28_HISPCP != 0)
#define HSPCLK_HZ 		( SYSCLK_HZ / (2*DSP28_HISPCP) )
#else
#define HSPCLK_HZ 		SYSCLK_HZ
#endif
#if( DSP28_LOSPCP != 0)
#define LSPCLK_HZ 		( SYSCLK_HZ / (2*DSP28_LOSPCP) )
#else
#define LSPCLK_HZ 		SYSCLK_HZ
#endif


//############################################################################
//
//		   PART II: （片内）外设篇
//
//############################################################################
// 注意：
// 1. 如下下面的#define被注释掉了，说明该定义在file：“my_demo_select.h”里.
// 2. 如果设备使用到了中断（默认开启），该中断的服务函数定义在"myIsr.c"里。
//


//============================================================================
//      Peripheral 1: cpu timer, 默认开启中断
//============================================================================
//----------------------------------------------------------
// Step1. Select the Cpu Timers modules you use.
//----------------------------------------------------------
#ifndef USE_CPU_TIMER0
#define USE_CPU_TIMER0  0	// 是否使用Cpu Timer0
#endif
#ifndef USE_CPU_TIMER1
#define USE_CPU_TIMER1  0	// 是否使用Cpu Timer1
#endif
#ifndef USE_CPU_TIMER2
#define USE_CPU_TIMER2  0	// 是否使用Cpu Timer2
#endif


//----------------------------------------------------------
// Step3. configure the Cpu Timer period(us) .
//----------------------------------------------------------
#if !defined(CPU_TIMER0_PRD) & USE_CPU_TIMER0
#define 	CPU_TIMER0_PRD	500000
#endif

#if !defined(CPU_TIMER1_PRD) & USE_CPU_TIMER1
#define 	CPU_TIMER1_PRD	500000
#endif

#if !defined(CPU_TIMER2_PRD) & USE_CPU_TIMER2
#define 	CPU_TIMER2_PRD	500000
#endif



//============================================================================
//      Peripheral 2: SCI(UART) : 默认使用中断模式，使用FIFO进行收发控制。
//============================================================================
// 说明： 为了是Sci效率最大化，新增了软件FIFO，即swFIFO.默认的情况是只要使用
//        相应的FIFO就会开启swFIFO功能，于此同时，基于两个FIFO的puts即gets函数
//        可以使用，十分方便。当然也需要中断（即INT）的支持。
//
// 警告： 如果关闭swFIFO或者关闭INT中断，gets及puts函数运行结果不予保证，
//        如果你确实需要如此，建议不使用该工程，因为这与工程建立的初衷相悖。

//----------------------------------------------------------
// Step1. Select the SCI modules you use.
//----------------------------------------------------------
#ifndef USE_SCIA
#define USE_SCIA        0   // 是否使用 SCIA
#endif
#ifndef USE_SCIB
#define USE_SCIB        0   // 是否使用 SCIB
#endif
#ifndef USE_SCIC
#define USE_SCIC        0   // 是否使用 SCIC
#endif

///////////////////////////////////////////////////////////////////
///               Here are default cofigurations                ///
///                  Do not change them here.                   ///
///////////////////////////////////////////////////////////////////
#if( USE_SCIA || USE_SCIB || USE_SCIC)

// 默认波特率设为9600bps
#ifndef DEFAULT_SCIBAUD
#define DEFAULT_SCIBAUD 9600
#endif


// 警告： 如果关闭swFIFO或者关闭INT中断，gets及puts函数运行结果不予保证，
//        如果你确实需要如此，建议不使用该工程，因为这与工程建立的初衷相悖。
//
//        而之所以给以以下选择，是为了你选取该工程的部分代码时更方便，所有
//        修改（新增）的关键代码都会使用宏定义保护起来。

#ifndef USE_SCI_INT
#define USE_SCI_INT     1   // ** MUST BE 1, DO NOT CHANGE **
#endif
#ifndef USE_SCI_FIFO
#define USE_SCI_FIFO 	1   // ** MUST BE 1, DO NOT CHANGE **
#endif

#endif

///////////////////////  NO MORE  /////////////////////////////////////

//----------------------------------------------------------
// Step2. Choose a GPIO as SCI interface.
//----------------------------------------------------------
	/*
	    Pin Assignment:
	    SCIRXDA:    GPIO28/GPIO36
	    SCITXDA:    GPIO29/GPIO35
	    SCIRXDB:    GPIO11/GPIO15/GPIO19/GPIO23
	    SCITXDB:    GPIO9 /GPIO14/GPIO18/GPIO22
	    SCIRXDC:    GPIO62  // No choose.
	    SCITXDC:    GPIO63  // No choose.
	 */

#if ( USE_SCIA )
// Choose SCIRXDA pin, only choose one.

#if( !defined(USE_GPIO36_AS_SCIRXDA) && !defined(USE_GPIO28_AS_SCIRXDA) )
#define USE_GPIO36_AS_SCIRXDA   1		// func:1, test ok.
#define USE_GPIO28_AS_SCIRXDA   0		// func:1, test ok.
#endif
// Choose SCITXDA pin, only choose one.
#if( !defined(USE_GPIO35_AS_SCITXDA) && !defined(USE_GPIO29_AS_SCITXDA) )
#define USE_GPIO35_AS_SCITXDA   1		// func:1, test ok.
#define USE_GPIO29_AS_SCITXDA   0		// func:1, test ok.
#endif

#endif	// ( USE_SCIA )

#if ( USE_SCIB )

// Choose SCIRXDB pin, only choose one.
#if(!defined(USE_GPIO11_AS_SCIRXDB) && !defined(USE_GPIO15_AS_SCIRXDB) &&\
	!defined(USE_GPIO19_AS_SCIRXDB) && !defined(USE_GPIO23_AS_SCIRXDB) )
#define USE_GPIO11_AS_SCIRXDB   1		// func:2, test ok.
#define USE_GPIO15_AS_SCIRXDB   0		// func:2, test ok.
#define USE_GPIO19_AS_SCIRXDB   0		// func:2, no test.
#define USE_GPIO23_AS_SCIRXDB   0		// func:3, no test.
#endif

// Choose SCITXDB pin, only choose one.
#if(!defined(USE_GPIO9_AS_SCITXDB) && !defined(USE_GPIO14_AS_SCITXDB) &&\
	!defined(USE_GPIO18_AS_SCITXDB) && !defined(USE_GPIO22_AS_SCITXDB) )
#define USE_GPIO9_AS_SCITXDB    1		// func:2, test ok.
#define USE_GPIO14_AS_SCITXDB   0		// func:2, test ok.
#define USE_GPIO18_AS_SCITXDB   0		// func:2, no test.
#define USE_GPIO22_AS_SCITXDB   0		// func:3, no test.
#endif

#endif // ( USE_SCIB )

//----------------------------------------------------------
// Step3. Configure the uart baud.
//----------------------------------------------------------
// data format is fixed : 8-data, 1 stop bit, no parity.
//

// Configure the baud of SCIA, if used.
#if(USE_SCIA && !defined(SCIA_BAUD))
#define SCIA_BAUD DEFAULT_SCIBAUD  // change the DEFAULT_BAUD here.
#endif

// Configure the baud of SCIB, if used.
#if(USE_SCIB && !defined(SCIB_BAUD))
#define SCIB_BAUD DEFAULT_SCIBAUD // change the DEFAULT_BAUD here.
#endif

// Configure the baud of SCIC, if used.
#if(USE_SCIC && !defined(SCIC_BAUD))
#define SCIC_BAUD DEFAULT_SCIBAUD // change the DEFAULT_BAUD here.
#endif

//----------------------------------------------------------
// Step4. Advanced sets
//----------------------------------------------------------
#if(USE_SCI_FIFO)

#if(USE_SCIA)
#ifndef SCIA_SWFFTXDEEP
#define SCIA_SWFFTXDEEP 16	// 设置软件FIFO的TX空间大小
#endif

#ifndef SCIA_SWFFRXDEEP
#define SCIA_SWFFRXDEEP 16	// 设置软件FIFO的RX空间大小
#endif
#endif // #if(USE_SCIA)

#if(USE_SCIB)
#ifndef SCIB_SWFFTXDEEP
#define SCIB_SWFFTXDEEP 16	// 设置软件FIFO的TX空间大小
#endif

#ifndef SCIB_SWFFRXDEEP
#define SCIB_SWFFRXDEEP 16	// 设置软件FIFO的RX空间大小
#endif
#endif // #if(USE_SCIB)

#if(USE_SCIC)
#ifndef SCIC_SWFFTXDEEP
#define SCIC_SWFFTXDEEP 16	// 设置软件FIFO的TX空间大小
#endif

#ifndef SCIC_SWFFRXDEEP
#define SCIC_SWFFRXDEEP 16	// 设置软件FIFO的RX空间大小
#endif
#endif // #if(USE_SCIC)


#endif	// (USE_SCI_FIFO)


//============================================================================
//      Peripheral 2: SPI : 默认使用中断模式，使用FIFO进行收发控制。
//============================================================================
//----------------------------------------------------------
// Step1. Select the SPI modules you use.
//----------------------------------------------------------
// F28335 只有一个SPI(SPI_A)
#ifndef USE_SPIA
#define USE_SPIA        0   // 是否使用 SPIA
#endif


///////////////////////////////////////////////////////////////////
///               Here are default cofigurations                ///
///                  Do not change them here.                   ///
///////////////////////////////////////////////////////////////////
#if( USE_SPIA || USE_SPIB || USE_SPIC)
// 警告： 如果关闭swFIFO或者关闭INT中断，gets及puts函数运行结果不予保证，
//        如果你确实需要如此，建议不使用该工程，因为这与工程建立的初衷相悖。
//
//        而之所以给以以下选择，是为了你选取该工程的部分代码时更方便，所有
//        修改（新增）的关键代码都会使用宏定义保护起来。

#ifndef USE_SPI_INT
#define USE_SPI_INT     1   // ** MUST BE 1, DO NOT CHANGE **
#endif
#ifndef USE_SPI_FIFO
#define USE_SPI_FIFO 	1   // ** MUST BE 1, DO NOT CHANGE **
#endif

// 默认波特率设为500Khz
#ifndef DEFAULT_SPIBAUD
#define DEFAULT_SPIBAUD 500000
#endif
#endif // ( USE_SPIA || USE_SPIB || USE_SPIC)

///////////////////////  NO MORE  /////////////////////////////////////

//----------------------------------------------------------
// Step2. Choose a GPIO as SPI interface.
//----------------------------------------------------------
#if ( USE_SPIA )

// Choose SPISOMI pin, only choose one.
#if( !defined(USE_GPIO16_AS_SPISIMOA) && !defined(USE_GPIO54_AS_SPISIMOA) )
#define USE_GPIO16_AS_SPISIMOA   0      // func:2
#define USE_GPIO54_AS_SPISIMOA   1      // func:2
#endif
#if( !defined(USE_GPIO17_AS_SPISOMIA) && !defined(USE_GPIO55_AS_SPISOMIA) )
#define USE_GPIO17_AS_SPISOMIA   0      // func:2
#define USE_GPIO55_AS_SPISOMIA   1      // func:2
#endif
#if( !defined(USE_GPIO18_AS_SPICLKA) && !defined(USE_GPIO56_AS_SPICLKA) )
#define USE_GPIO18_AS_SPICLKA    0      // func:2
#define USE_GPIO56_AS_SPICLKA    1      // func:2
#endif
#if( !defined(USE_GPIO19_AS_SPISTEA) && !defined(USE_GPIO57_AS_SPISTEA) )
#define USE_GPIO19_AS_SPISTEA    0      // func:2, SPI slave transmit-enable
#define USE_GPIO57_AS_SPISTEA    1      // func:2
#endif

#endif // ( USE_SPIA )


//----------------------------------------------------------
// Step3. Configure the SPI baud.
//----------------------------------------------------------
// data format is fixed : 8-bit,
//

// Configure the baud of SPIA, if used.
#if(USE_SPIA && !defined(SPIA_BAUD))
#define SPIA_BAUD DEFAULT_SPIBAUD  // change the DEFAULT_BAUD here.
#endif

//----------------------------------------------------------
// Step4. Advanced sets
//----------------------------------------------------------
#if(USE_SPI_FIFO)

#if(USE_SPIA)
#ifndef SPIA_SWFFTXDEEP
#define SPIA_SWFFTXDEEP 16	// 设置软件FIFO的TX空间大小
#endif

#ifndef SPIA_SWFFRXDEEP
#define SPIA_SWFFRXDEEP 16	// 设置软件FIFO的RX空间大小
#endif
#endif // #if(USE_SPIA)

#endif	// (USE_SCI_FIFO)

//============================================================================
//      Peripheral X: DMA
//============================================================================
//----------------------------------------------------------
// Step1. Select the DMA channels you use.
//----------------------------------------------------------
#ifndef USE_DMA_CH1
#define USE_DMA_CH1		0
#endif
#ifndef USE_DMA_CH2
#define USE_DMA_CH2		0
#endif
#ifndef USE_DMA_CH3
#define USE_DMA_CH3		0
#endif
#ifndef USE_DMA_CH4
#define USE_DMA_CH4		0
#endif
#ifndef USE_DMA_CH5
#define USE_DMA_CH5		0
#endif
#ifndef USE_DMA_CH6
#define USE_DMA_CH6		0
#endif

//----------------------------------------------------------
// Step2. Configure the DMA channels you choose.
//----------------------------------------------------------
// 1. 具体的配置参数请参考 Dma_defines.h
// 2. 去DMA.c文件中完成相应的配置。

// DATASIZE = 32, 这样既提高效率也不影响16位数据的传输。
#ifndef DMA_BSIZE
#define DMA_BSIZE		8	// 一包数据里，32-bit 数据的个数， 或者两倍的16-bit数据的个数。
#endif
#ifndef DMA_TSIZE
#define DMA_TSIZE		1	// 一次传输，传输几包。
#endif

//============================================================================
//      Peripheral X: XINTF External Interface
//============================================================================
//----------------------------------------------------------
// Step1. Select the zones you use.
//----------------------------------------------------------
#ifndef USE_XINTF_ZONE0
#define USE_XINTF_ZONE0		0
#endif
#ifndef USE_XINTF_ZONE6
#define USE_XINTF_ZONE6		0
#endif
#ifndef USE_XINTF_ZONE7
#define USE_XINTF_ZONE7		0
#endif

#if( USE_XINTF_ZONE0 || USE_XINTF_ZONE6 || USE_XINTF_ZONE7 )
#define USE_XINTF 	1


#else
#define USE_XINTF	0
#endif



//============================================================================
//      Peripheral X:
//============================================================================
//----------------------------------------------------------
// Step1. Select the SCI modules you use.
//----------------------------------------------------------

//----------------------------------------------------------
// Step2. Choose a GPIO as SCI interface.
//----------------------------------------------------------

//----------------------------------------------------------
// Step3. Configure the uart baud.
//----------------------------------------------------------

//----------------------------------------------------------
// Step4. Advanced sets
//----------------------------------------------------------







#ifdef __cplusplus
}
#endif /* extern "C" */


#endif /* MYPERIPHERALS_CONFIG_H_ */
