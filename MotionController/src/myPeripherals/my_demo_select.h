/*
 * my_demo_select.h
 *
 *  Created on: 2016-12-3
 *      Author: Administrator
 */

#ifndef MY_DEMO_SELECT_H_
#define MY_DEMO_SELECT_H_


//============================================================================
//		    ���еĲ���ģʽ
//============================================================================
/* ALL TEST DEMO */
#define EMPTY_PROJECT       	0       // �յĹ���
#define TEST_GPIO_TIMER_LED 	1       // ����GPIO,����Timer������LED������
#define TEST_SCIA           	2       // ����SCIA����uartͨ�š�
#define TEST_SCIB           	22      // ����SCIB���˶����ư忨�ӿڡ�
#define TEST_SCIC           	23      // ����SCIA����uartͨ�š�
#define TEST_SPIA           	3       // ����SPI
#define TEST_DMA				4		// ����DMA
#define TEST_XINTF				5		// ����XINTF


//###########################################################################
//						 DEMO SELECT
//###########################################################################
#define MY_TEST_DEMO TEST_SPIA		// ����ģʽ��ѡ��



#if( MY_TEST_DEMO == TEST_GPIO_TIMER_LED)
//============================================================================
//      Example 1. GPIO ����ģʽ
//============================================================================
// ʵ������ 3��LED��������˸���Բ�ͬ��Ƶ�ʡ�ÿ��LED��һ��Cpu Timer���ơ�
// ʹ��GPIO����LED�����ӣ�����YH��DSP�����塣������Ǹÿ��Ű壬�޸������
// �ܽź궨�壬�����ˡ�
//

// ʹ�õ�������
#define USE_CPU_TIMER0  1	// ʹ��Cpu Timer0
#define USE_CPU_TIMER1  1	// ʹ��Cpu Timer1
#define USE_CPU_TIMER2  1	// ʹ��Cpu Timer2

/* ˵��
    Ӳ���ܽ�ͼ��
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
//      Example X. SCIA ����ģʽ
//============================================================================
// ʵ������ Ĭ�ϲ���swFIFO���������⵽���ַ���
//

#define USE_SCIA        		1   		// ʹ�� SCIA
// ���Զ˿ڣ�ok.
//#define USE_GPIO28_AS_SCIRXDA 	1			// RX - GPIO28
//#define USE_GPIO29_AS_SCITXDA 	1			// TX - GPIO29
#define SCIA_BAUD 				115200	 	    // SCIA BAUD = 115200

//============================================================================
//      Example X. SCIB ����ģʽ
//============================================================================
#elif( MY_TEST_DEMO == TEST_SCIB )
#define SCIB_BAUD 				256000
#define USE_GPIO19_AS_SCIRXDB 1
#define USE_GPIO18_AS_SCITXDB 1

//============================================================================
//      Example X. SCIC ����ģʽ
//============================================================================
#elif( MY_TEST_DEMO == TEST_SCIC )
#define USE_SCIC        		1   		// ʹ�� SCIC
#define SCIC_BAUD 				256000



//============================================================================
//      Example X. SPI ����ģʽ
//============================================================================
// ʵ������
//
#elif( MY_TEST_DEMO == TEST_SPIA )
#define USE_SPIA 		1
// #define USE_GPIO19_AS_SPISTEA    1  // jzs �İ��Ӹ���ע�͵�


//============================================================================
//      Example X. DMA ����ģʽ
//============================================================================
// ʵ������
//
#elif( MY_TEST_DEMO == TEST_DMA )
#define USE_DMA   		1
#define USE_DMA_CH1		1
#define USE_CPU_TIMER0  1



#elif( MY_TEST_DEMO == TEST_XINTF )
//============================================================================
//      Example X. XXX ����ģʽ
//============================================================================
// ʵ������
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
//      Example X. XXX ����ģʽ
//============================================================================
// ʵ������
//
#endif /* MY_TEST_DEMO*/


#endif /* MY_DEMO_SELECT_H_ */
