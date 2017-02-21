/*
 *  Created on: 2016-12-2
 *      Author: QUENTIN
 *      E-mail: qiyuexin@yeah.net
 */
//###########################################################################
//
//  FILE:   myIsr.c
//
// TITLE:   我的中断服务函数
//
//###########################################################################
#include "my_project.h"


//============================================================================
//      Peripheral 2. Cpu Timer
//============================================================================
#if( MY_TEST_DEMO == TEST_GPIO_TIMER_LED)
volatile unsigned int timer_int_cnt;
#endif

#if( USE_CPU_TIMER0 )
interrupt void cpu_timer0_isr(void)
{
	CpuTimer0.InterruptCount++;


#if( MY_TEST_DEMO == TEST_GPIO_TIMER_LED)
	//   Functions of TEST_GPIO_TIMER_LED
	if(timer_int_cnt++ >= 12) {
		timer_int_cnt = 0;
		LED2 = LED_OFF;
		CpuTimer0Regs.TCR.bit.TIE	= 0;	// 设置TIE = 0，关闭定时器0中断
		CpuTimer1Regs.TCR.bit.TIE  	= 1;	// 设置TIE = 1，开启定时器1中断
		CpuTimer2Regs.TCR.bit.TIE  	= 0; 	// 设置TIE = 0，关闭定时器2中断
	}

	LED2_TOG = 1;
#endif

#if(USE_DMA)
	extern volatile Uint16 srcBuf[1024];
	int i;
	for(i=0;i<DMA_BSIZE*DMA_TSIZE*2;i++)
		srcBuf[i] = 1000*CpuTimer0.InterruptCount;
#if(USE_DMA_CH1)
	StartDMACHx( &Dma.RegsAddr->CH1);
#endif
#if(USE_DMA_CH2)
	StartDMACHx( &Dma.RegsAddr->CH2);
#endif
#if(USE_DMA_CH3)
	StartDMACHx( &Dma.RegsAddr->CH3);
#endif

#endif
	// Acknowledge this interrupt to receive more interrupts from group 1
	PieCtrlRegs.PIEACK.bit.ACK1 = 1;

}
#endif

#if( USE_CPU_TIMER1 )
interrupt void cpu_timer1_isr(void)
{
	CpuTimer1.InterruptCount++;

#if( MY_TEST_DEMO == TEST_GPIO_TIMER_LED)
	//   Functions of TEST_GPIO_TIMER_LED
	if( timer_int_cnt++ >= 12) {
		timer_int_cnt = 0;
		LED3 = LED_OFF;
		CpuTimer0Regs.TCR.bit.TIE	= 0;	// 设置TIE = 0，关闭定时器0中断
		CpuTimer1Regs.TCR.bit.TIE  	= 0;	// 设置TIE = 0，开启定时器1中断
		CpuTimer2Regs.TCR.bit.TIE  	= 1; 	// 设置TIE = 1，关闭定时器2中断
	}

	LED3_TOG = 1;
#endif


	// The CPU acknowledges the interrupt.

}
#endif

#if( USE_CPU_TIMER2 )
interrupt void cpu_timer2_isr(void)
{
	CpuTimer2.InterruptCount++;

#if( MY_TEST_DEMO == TEST_GPIO_TIMER_LED)
	//   Functions of TEST_GPIO_TIMER_LED
	if( timer_int_cnt++ >= 12) {
		timer_int_cnt = 0;
		LED4 = LED_OFF;
		CpuTimer0Regs.TCR.bit.TIE	= 1;	// 设置TIE = 1，关闭定时器0中断
		CpuTimer1Regs.TCR.bit.TIE  	= 0;	// 设置TIE = 0，开启定时器1中断
		CpuTimer2Regs.TCR.bit.TIE  	= 0; 	// 设置TIE = 0，关闭定时器2中断
	}

	LED4_TOG = 1;
#endif


	// The CPU acknowledges the interrupt.

}
#endif

//============================================================================
//      Peripheral 3. Sci
//============================================================================
#if(USE_SCI_INT && USE_SCI_FIFO)

#if(USE_SCIA)
interrupt void scia_rx_isr(void)
{
	if(Scia.RegsAddr->SCIFFRX.bit.RXFFINT){
		//		RXFIFO 中断，我们设置的TXFFIL=16，所以当TXFFST=16时会触发中断。
		Sci_RxFifoFullHandler(&Scia);
	}
	if(Scia.RegsAddr->SCIFFRX.bit.RXFFOVF){
		//		overflow.
		Sci_RxFifoFullHandler(&Scia);
		Scia.RegsAddr->SCIFFRX.bit.RXFFOVFCLR = 1;
	}
	PieCtrlRegs.PIEACK.bit.ACK9 = 1;
}
interrupt void scia_tx_isr(void)
{
	if( Scia.RegsAddr->SCIFFTX.bit.TXFFINT ) {
		//		TXFIFO 中断，我们设置的TXFFIL=0，所以当TXFFST=0时会触发中断。
		Sci_TxFifoFullHandler(&Scia);
	}
	// Acknowledge this interrupt to receive more interrupts from group 9
	PieCtrlRegs.PIEACK.bit.ACK9 = 1;
}
#endif //(USE_SCIA)

#if(USE_SCIB)
interrupt void scib_rx_isr(void)
{
	if(Scib.RegsAddr->SCIFFRX.bit.RXFFINT){
		//		RXFIFO 中断，我们设置的TXFFIL=16，所以当TXFFST=16时会触发中断。
		Sci_RxFifoFullHandler(&Scib);
	}
	if(Scib.RegsAddr->SCIFFRX.bit.RXFFOVF){
		//		overflow.
		Sci_RxFifoFullHandler(&Scib);
		Scib.RegsAddr->SCIFFRX.bit.RXFFOVFCLR = 1;
	}
	PieCtrlRegs.PIEACK.bit.ACK9 = 1;
}
interrupt void scib_tx_isr(void)
{
	if( Scib.RegsAddr->SCIFFTX.bit.TXFFINT ) {
		//		TXFIFO 中断，我们设置的TXFFIL=0，所以当TXFFST=0时会触发中断。
		Sci_TxFifoFullHandler(&Scib);
	}
	// Acknowledge this interrupt to receive more interrupts from group 9
	PieCtrlRegs.PIEACK.bit.ACK9 = 1;
}
#endif // (USE_SCIB)

#if(USE_SCIC)
interrupt void scic_rx_isr(void)
{
	if(Scic.RegsAddr->SCIFFRX.bit.RXFFINT){
		//		RXFIFO 中断，我们设置的TXFFIL=16，所以当TXFFST=16时会触发中断。
		Sci_RxFifoFullHandler(&Scic);
	}
	if(Scic.RegsAddr->SCIFFRX.bit.RXFFOVF){
		//		overflow.
		Sci_RxFifoFullHandler(&Scic);
		Scic.RegsAddr->SCIFFRX.bit.RXFFOVFCLR = 1;
	}
	PieCtrlRegs.PIEACK.bit.ACK8 = 1;
}
interrupt void scic_tx_isr(void)
{
	if( Scic.RegsAddr->SCIFFTX.bit.TXFFINT ) {
		//		TXFIFO 中断，我们设置的TXFFIL=0，所以当TXFFST=0时会触发中断。
		Sci_TxFifoFullHandler(&Scic);
	}
	// Acknowledge this interrupt to receive more interrupts from group 9
	PieCtrlRegs.PIEACK.bit.ACK8 = 1;
}
#endif // (USE_SCIC)

#endif //(USE_SCI_INT && USE_SCI_FIFO)


//============================================================================
//      Peripheral 4. SPI
//============================================================================
#if(USE_SPIA)
interrupt void spia_rx_isr(void)
{
	int tmp;
	if(Spia.RegsAddr->SPIFFRX.bit.RXFFINT){
		//		RXFIFO 中断，我们设置的TXFFIL=16，所以当TXFFST=16时会触发中断。
		while( SpiaRegs.SPIFFRX.bit.RXFFST){
			tmp = SpiaRegs.SPIRXBUF;
			cb_append(&Spia.cb_rx, &tmp);
		}
		SpiaRegs.SPIFFRX.bit.RXFFINTCLR = 1;
	}
	if(Spia.RegsAddr->SPIFFRX.bit.RXFFOVF){
		//		overflow.
		//		Spi_RxFifoFullHandler(&Spia);
		Spia.RegsAddr->SPIFFRX.bit.RXFFOVFCLR = 1;
	}

	//	char tmp;
	//		if(SpiaRegs.SPISTS.bit.OVERRUN_FLAG)
	//		SpiaRegs.SPISTS.bit.OVERRUN_FLAG = 1;
	//
	//		if(SpiaRegs.SPISTS.bit.INT_FLAG){
	//			tmp  = SpiaRegs.SPIRXBUF;
	//		}

	PieCtrlRegs.PIEACK.bit.ACK6 = 1;
}
interrupt void spia_tx_isr(void)
{
		if( Spia.RegsAddr->SPIFFTX.bit.TXFFINT ) {
	//		TXFIFO 中断，我们设置的TXFFIL=0，所以当TXFFST=0时会触发中断。
//			Spi_TxFifoFullHandler(&Spia);
//			SpiaRegs.SPIFFTX.bit.TXFFINTCLR = 1;
		}

//	if(SpiaRegs.SPISTS.bit.INT_FLAG)
//		SpiaRegs.SPISTS.bit.INT_FLAG = 1;
//	if(SpiaRegs.SPISTS.bit.BUFFULL_FLAG)
//		SpiaRegs.SPISTS.bit.BUFFULL_FLAG = 1;

	// Acknowledge this interrupt to receive more interrupts from group 6
	PieCtrlRegs.PIEACK.bit.ACK6 = 1;
}
#endif // (USE_SPIA)


//============================================================================
//      Peripheral 5. DMA
//============================================================================
#if(USE_DMA_CH1)
interrupt void dma_ch1_isr(void)
{
	// Next two lines for debug only to halt the processor here
	// Remove after inserting ISR Code
	//	   asm ("      ESTOP0");
	//	   for(;;);
	//	StartDMACHx( &Dma.RegsAddr->CH1);
	// Acknowledge this interrupt to receive more interrupts from group 6
	PieCtrlRegs.PIEACK.bit.ACK7 = 1;
}
#endif

#if(USE_DMA_CH2)
interrupt void dma_ch2_isr(void)
{
	// Next two lines for debug only to halt the processor here
	// Remove after inserting ISR Code
	//	   asm ("      ESTOP0");
	//	   for(;;);
	//	StartDMACHx( &Dma.RegsAddr->CH2);
	// Acknowledge this interrupt to receive more interrupts from group 6
	PieCtrlRegs.PIEACK.bit.ACK7 = 1;
}
#endif

#if(USE_DMA_CH3)
interrupt void dma_ch3_isr(void)
{
	// Next two lines for debug only to halt the processor here
	// Remove after inserting ISR Code
	//	   asm ("      ESTOP0");
	//	   for(;;);
	//	StartDMACHx( &Dma.RegsAddr->CH2);
	// Acknowledge this interrupt to receive more interrupts from group 6
	PieCtrlRegs.PIEACK.bit.ACK7 = 1;
}
#endif



//============================================================================
//      Peripheral X. xx
//============================================================================

