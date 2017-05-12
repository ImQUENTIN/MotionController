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
extern COMMAND_S gCmd;

#if( USE_CPU_TIMER0 )
interrupt void cpu_timer0_isr(void)			//1ms
{
	int axis;
	static Uint32 preCount;

	CpuTimer0.InterruptCount++;

	if( CpuTimer0.InterruptCount - preCount >= 10 ){
		/*  period = 10ms */
		preCount = CpuTimer0.InterruptCount;
		for (axis = 0; axis < 3; axis++) {

			if(MotorRegs[axis].MCTL.bit.MMODE == JOG_MODE)
				testJog(axis);
		}
	}

//	FPGA_Space();

	for (axis = 0; axis < 3; axis++) {
//		if ((gCmd.mark >> axis) & 0x01) {
		// gcmd.mark 会随着上位机命令而改变，这里是需要检测以及激活的电机：
		if( MotorRegs[axis].MCTL.bit.ENA){
			// 这里做限位开关的检测即可，pt发送我移到到main里了。
			if( MotorRegs[axis].MSTA.bit.LMTN || MotorRegs[axis].MSTA.bit.LMTP ){
				//
				Stop(AXIS_ALL);		// Turn off all the Servomotors.
				break;
			}
			// end
		}

	}


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
extern word recordtx[100], itx;
extern word testtx[16];
interrupt void spia_rx_isr(void)
{
	int tmp;
	if(Spia.RegsAddr->SPISTS.bit.INT_FLAG){
		//		RXFIFO 中断，我们设置的TXFFIL=1，所以当TXFFST=1时会触发中断。
//		while( SpiaRegs.SPIFFRX.bit.RXFFST){

			// 发送
			if( RTN_ERROR != cb_get(&Spia.cb_tx, &tmp) ){

			}
				else
					tmp = 0xeeee;

			SpiaRegs.SPITXBUF = tmp;
			recordtx[itx++] = tmp;
			if(itx>=30) itx = 0;

			tmp = SpiaRegs.SPIRXBUF;
			cb_append(&Spia.cb_rx, &tmp);
//			tmp = testtx[i];
//			if(++i >= 16) i=0;

//		}
//		SpiaRegs.SPIFFRX.bit.RXFFINTCLR = 1;
	}
	if(Spia.RegsAddr->SPISTS.bit.OVERRUN_FLAG)
		SpiaRegs.SPISTS.bit.OVERRUN_FLAG = 1;
//	if(Spia.RegsAddr->SPIFFRX.bit.RXFFOVF){
//		//		overflow.
//		//		Spi_RxFifoFullHandler(&Spia);
//		Spia.RegsAddr->SPIFFRX.bit.RXFFOVFCLR = 1;
//	}

	PieCtrlRegs.PIEACK.bit.ACK6 = 1;
}

interrupt void spia_tx_isr(void)
{
//	int tmp;

	if( Spia.RegsAddr->SPIFFTX.bit.TXFFINT ) {
		//		TXFIFO 中断，我们设置的TXFFIL=0，所以当TXFFST=0时会触发中断。
		if( cb_usedSpace(&Spia.cb_tx) > 0 ) {
			// 循环缓冲区有数据
//			while ( RTN_ERROR != cb_get(&Spia.cb_tx , &tmp)
//					&& SpiaRegs.SPIFFTX.bit.TXFFST < 16){
//
//				SpiaRegs.SPITXBUF = tmp;
//				txbuf[i++] = tmp;
//			}
			SpiaRegs.SPIFFTX.bit.TXFFINTCLR = 1;
		} else {
			// 循环缓冲区 为空
			// do nothing, and exit.
		}
	}
	// Acknowledge this interrupt to receive more interrupts from group 6
	PieCtrlRegs.PIEACK.bit.ACK6 = 1;
}
#endif // (USE_SPIA)


//============================================================================
//      Peripheral X. xx
//============================================================================

