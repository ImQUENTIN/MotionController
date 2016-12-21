/*
*  Created on: 2016-12-6
*      Author: QUENTIN
*      E-mail: qiyuexin@yeah.net
*/
//###########################################################################
//
// FILE:	DSP2833x_Spi.c
//
// TITLE:	DSP2833x SPI Initialization & Support Functions.
//
//###########################################################################
// "*" means to do, and "+" means done.
// + spia_config
// + Spi_putchar
// + Spi_puts


#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File

// ========================================================================================
//       The following is added by QUENTIN.
// ========================================================================================
void ConfigSpi(struct SPI_VARS *Spi);
void InitSpiFifo( struct SPI_VARS *Spi);

#if(USE_SPIA)
struct SPI_VARS Spia;
uint8_t SPIA_SWFFTXBUF[SPIA_SWFFTXDEEP]={0};		// SPI Software FIFO Tx Buffer.
uint8_t SPIA_SWFFRXBUF[SPIA_SWFFRXDEEP]={0};		// SPI Software FIFO Rx Buffer.


// Test 1,SPIA , 8-bit word,
void InitSpia(void)
{
	void InitSpiaGpio(void);

	Spia.RegsAddr->SPICCR.bit.SPISWRESET = 0;		// force to reset.

	Spia.RegsAddr 	= &SpiaRegs;
	Spia.Baud		= SPIA_BAUD;		// get from user's configuration.

	swfifoReset(&Spia.swfifoTx);
	swfifoReset(&Spia.swfifoRx);
	Spia.swfifoTx.Buffer = SPIA_SWFFTXBUF;		// tx buffer addr
	Spia.swfifoTx.Deep   = SPIA_SWFFTXDEEP;		// tx buffer size
	Spia.swfifoRx.Buffer = SPIA_SWFFRXBUF;		// Rx buffer addr
	Spia.swfifoRx.Deep   = SPIA_SWFFRXDEEP;		// Rx buffer size

#if DSP28_SPIA
	InitSpiaGpio();
#else
#error hardware is not support for 'SPIA'.
#endif

#if(USE_SPI_INT)
	// 	修改中断向量表：
	EALLOW;  // This is needed to write to EALLOW protected registers
	PieVectTable.SPIRXINTA = &spia_rx_isr;	// 6.1
	PieVectTable.SPITXINTA = &spia_tx_isr;	// 6.2
	EDIS;    // This is needed to disable write to EALLOW protected registers

	//	 									    // 1. 外设中断打开。
	PieCtrlRegs.PIEIER6.bit.INTx1 = 1;  		// 2. PIE中断打开。
	PieCtrlRegs.PIEIER6.bit.INTx2 = 1;          //
	IER |= M_INT6;  	                		// 3. CPU核中断打开。
	//										    // 4. 开启总中断，INTM。在最后统一打开，这里不开启。
#endif

    // Make sure this peripheral clock is enabled
	EALLOW;
	SysCtrlRegs.PCLKCR0.bit.SPIAENCLK = 1;   // SPI-A
	EDIS;
	InitSpiFifo(&Spia);
	ConfigSpi(&Spia);


}
#endif

void InitSpis(void){
#if(USE_SPIA)
	InitSpia();
#endif
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////                     common                   //////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////


// Initalize the SPI FIFO
void InitSpiFifo( struct SPI_VARS *Spi)
{
	// Initialize SPI FIFO registers

//	struct  SPIFFTX_BITS {       // bit    description
//	   Uint16 TXFFIL:5;          // 4:0    Interrupt level, Interrupt occurs when TXFFIL equal TXFFST.
//	   Uint16 TXFFIENA:1;        // 5      Interrupt enable
//	   Uint16 TXFFINTCLR:1;      // 6      Clear INT flag, high active.
//	   Uint16 TXFFINT:1;         // 7      INT flag
//	   Uint16 TXFFST:5;          // 12:8   FIFO status, the number of stored data.
//	   Uint16 TXFIFOXRESET:1;    // 13     FIFO reset,low active.
//	   Uint16 SPIFFENA:1;        // 14     FIFO Enable
//	   Uint16 SPIRST:1;          // 15     SPI reset rx/tx channels
	Spi->RegsAddr->SPIFFTX.all=0xE040;	// 0xC028 // SPI reset rx/tx channels, tx FIFO; low active.
	//  Clear tx Fifo INT flag, Enhancement enable.

//    struct  SPIFFRX_BITS {       // bits   description
//       Uint16 RXFFIL:5;          // 4:0    Interrupt level, Interrupt occurs when RXFFIL equal RXFFST.
//       Uint16 RXFFIENA:1;        // 5      Interrupt enable
//       Uint16 RXFFINTCLR:1;      // 6      Clear INT flag
//       Uint16 RXFFINT:1;         // 7      INT flag
//       Uint16 RXFFST:5;          // 12:8   FIFO status, the number of stored data.
//       Uint16 RXFIFORESET:1;     // 13     FIFO reset
//       Uint16 RXFFOVRCLR:1;      // 14     Clear overflow
//       Uint16 RXFFOVF:1;         // 15     FIFO overflow
	Spi->RegsAddr->SPIFFRX.all=0x2050;	// ** 别忘了改回来***

#if(USE_SPI_INT)
	Spi->RegsAddr->SPIFFTX.bit.TXFFIENA 	= 1;	// tx fifo intterrupt enable.
	Spi->RegsAddr->SPIFFRX.bit.RXFFIENA 	= 1;	// tx fifo intterrupt enable.
#elif( USE_SPIA || USE_SPIB || USE_SPIC )
	#error no INT support for spi.
#endif

//	// SPI FIFO control register bit definitions:
//	struct  SPIFFCT_BITS {     // bits   description
//	   Uint16 FFTXDLY:8;         // 7:0    FIFO transmit delay
//	   Uint16 rsvd:5;            // 12:8   reserved
//	   Uint16 CDC:1;             // 13     Auto baud mode enable
//	   Uint16 ABDCLR:1;          // 14     Auto baud clear
//	   Uint16 ABD:1;             // 15     Auto baud detect
//	};
	Spi->RegsAddr->SPIFFCT.all=0x0;

}


void ConfigSpi(struct SPI_VARS *Spi)
{

	//----------------------------------------------------------
	// Step1. Configure the baud.
	//----------------------------------------------------------
//	For SPIBRR = 3 to 127  :	SPI Baud Rate = LSPCLK / (SPIBRR+1)
//	For SPIBRR = 0, 1, or 2:	SPI Baud Rate = LSPCLK / 4
	Uint16 tmp = (Uint16)( LSPCLK_HZ / (Spi->Baud )  - 1 );
	Spi->RegsAddr->SPIBRR	= tmp;

	//----------------------------------------------------------
	// Step2.
	//----------------------------------------------------------
	// 		Name				  // bit    description
//	   Uint16 SPICHAR:4;         // 3:0    Character length control
//	   Uint16 SPILBK:1;          // 4      Loop-back enable/disable
//	   Uint16 rsvd1:1;           // 5      reserved
//	   Uint16 CLKPOLARITY:1;     // 6      Clock polarity: 0,Data is output on rising edge and input on falling edge.
//	   Uint16 SPISWRESET:1;      // 7      SPI SW Reset
//	   Uint16 rsvd2:8;           // 15:8   reserved
	Spi->RegsAddr->SPICCR.all =0x0007;   	// Reset on, rising edge, 8-bit char bits


	//----------------------------------------------------------
	// Step3.
	//----------------------------------------------------------
//	   Uint16 SPIINTENA:1;       // 0      Interrupt enable
//	   Uint16 TALK:1;            // 1      Master/Slave transmit enable
//	   Uint16 MASTER_SLAVE:1;    // 2      Network control mode
//	   Uint16 CLK_PHASE:1;       // 3      Clock phase select: 0, no delay; 1,SPICLK signal delayed by one half-cycle;
//	   Uint16 OVERRUNINTENA:1;   // 4      Overrun interrupt enable, overwiten.
//	   Uint16 rsvd:11;           // 15:5   reserved
	Spi->RegsAddr->SPICTL.all = 0x0002;   // Enable slave mode, normal phase, Enables interrupt.


//// 清楚标志位
//	SpiaRegs.SPISTS.bit.BUFFULL_FLAG = 1;
//	SpiaRegs.SPISTS.bit.INT_FLAG = 1;
//	SpiaRegs.SPISTS.bit.OVERRUN_FLAG = 1;

	//----------------------------------------------------------
	// Step4. Enable interrupt, if needed. And Start up.
	//----------------------------------------------------------
#if (USE_SPI_INT && !USE_SPI_FIFO)
//	没有开启SPI FIFO功能，打开RX,TX的中断
	Spi->RegsAddr->SPICTL.bit.SPIINTENA 		= 1;
	Spi->RegsAddr->SPICTL.bit.OVERRUNINTENA 	= 1;
#endif

	Spi->RegsAddr->SPIPRI.bit.FREE = 1;                // Set so breakpoints don't disturb xmission
	Spi->RegsAddr->SPICCR.bit.SPISWRESET = 1;  // Relinquish SPI from Reset

}

// ===================================================================================================
//           Below are supports for swFIFO
// ===================================================================================================


//----------------------------------------------------------
//           supports for Spi_gets
//----------------------------------------------------------

uint8_t SPIFFRX_IsNotEmpty( struct SPI_VARS *Spi){	return (Spi->RegsAddr->SPIFFRX.bit.RXFFST > 0) ? 1: 0; }
uint8_t SPIFFRX_Out( struct SPI_VARS *Spi){	return ( (Spi->RegsAddr->SPIRXBUF) & 0x00ff );}
void SPIFFRX_ClearINT( struct SPI_VARS *Spi){	Spi->RegsAddr->SPIFFRX.bit.RXFFINTCLR = 1;}

uint8_t SPIFFRX_IsOn( struct SPI_VARS *Spi){return( Spi->RegsAddr->SPIFFTX.bit.SPIFFENA );}
void SPIFFRX_TurnOn( struct SPI_VARS *Spi){Spi->RegsAddr->SPIFFTX.bit.SPIFFENA = 1;}
void SPIFFRX_TurnOff( struct SPI_VARS *Spi){	Spi->RegsAddr->SPIFFTX.bit.SPIFFENA = 0;}

uint8_t Spi_IsFifoTxEmpty(struct SPI_VARS *Spi ) {	return ( !SPIFFRX_IsNotEmpty(Spi) && swfifo_IsEmpty( &Spi->swfifoTx) ) ? 1 : 0;}

//----------------------------------------------------------
//           supports for Spi_puts
//----------------------------------------------------------
uint8_t SPIFFTX_IsNotFull( struct SPI_VARS *Spi){	return (Spi->RegsAddr->SPIFFTX.bit.TXFFST < 16) ? 1: 0;}
void SPIFFTX_In( uint8_t dat, struct SPI_VARS *Spi){	Spi->RegsAddr->SPITXBUF = dat;}
void SPIFFTX_ClearINT( struct SPI_VARS *Spi){	Spi->RegsAddr->SPIFFTX.bit.TXFFINTCLR = 1;	}// clear INT flag.
//uint8_t SPI_IsMater( struct SPI_VARS *Spi ){	return(Spi->RegsAddr->SPICTL.bit.MASTER_SLAVE);}
//void SPI_SetMater( struct SPI_VARS *Spi ){	Spi->RegsAddr->SPICTL.bit.MASTER_SLAVE = 1;}
//void SPI_SetSlaver( struct SPI_VARS *Spi ){	Spi->RegsAddr->SPICTL.bit.MASTER_SLAVE = 0;}


uint8_t Spi_putchar(int8_t dat, struct SPI_VARS *Spi)
{
	// 在swFIFO为空的情况下，优先使用硬件FIFO.
	if( SPIFFTX_IsNotFull(Spi) && swfifo_IsEmpty(&Spi->swfifoTx) )
		SPIFFTX_In(dat, Spi);
	else	// SPI FIFO 满了 或者swFIFO非空 存入swFIFO.
		if( !swfifo_IsFull(&Spi->swfifoTx) )
			swfifo_In(dat, &Spi->swfifoTx);
		else
			return 1; 	// 当两个FIFO都满时，新来的数据不能存入，返回溢出标志。
	return 0;	// 正常
}


//----------------------------------------------------------
//           supports for Spi_puts
//----------------------------------------------------------
uint8_t Spi_puts(int8_t * msg, struct SPI_VARS *Spi)
{
	int i;
	i = 0;

	while(msg[i] != '\0') {
			if( Spi_putchar(msg[i], Spi) ){
				SPIFFTX_ClearINT(Spi);
				return i;	// FIFO溢出,返回字符串截断的位置。
			}

		i++;
	}
	SPIFFTX_ClearINT(Spi);	// clear INT flag.
	return 0;	// normal
}

// 下面函数在中断中执行，旨在清空（发送）软件FIFO的数据。
void Spi_TxFifoFullHandler(struct SPI_VARS *Spi)
{
	int8_t tmpData;
	if( !swfifo_IsEmpty(&Spi->swfifoTx) ) {		// swFIFOTX is not empty, load them to SPI TXFIFO.
		while(  SPIFFTX_IsNotFull(Spi) &&
		       !swfifo_IsEmpty(&Spi->swfifoTx) ) {
//			 当swFIFOTX非空，且SPITXFIFO未满时，就往SPITXFIFO送数据。
			swfifo_Out(&tmpData, &Spi->swfifoTx);	// get from swfifoTx
			SPIFFTX_In(tmpData,Spi);				// load to SPIFFTX
		}

		if( !SPIFFTX_IsNotFull(Spi) ) { 	// SPIFFTX is full
			// swFIFOTX可能仍非空，还需要加装一次SPITXFIFO.
			SPIFFTX_ClearINT(Spi);		// 保证当SPITXFIFO空时进入中断。
		} else {
			// eFifo is empty
			// do nothing, and exit.
		}
	} else {
		// eFifo is empty
		// do nothing, and exit.
	}
}

// 可以用于固定个数的读取
uint8_t Spi_getchar(int8_t *dat, struct SPI_VARS *Spi)
{
	if( !swfifo_IsEmpty(&Spi->swfifoRx) ) {
//		优先从 swFIFO中取数据
		swfifo_Out(dat, &Spi->swfifoRx);
	} else if( SPIFFRX_IsNotEmpty(Spi) ) {
//		没有的话，从 SPI RXBUF中取
		*dat = SPIFFRX_Out(Spi);
	} else {
//		还没有，那就真的没有了。
		return 1;	// 数据不可用。
	}
	return 0;
}


// 读取全部收到的数据
uint8_t Spi_gets(int8_t * msg, struct SPI_VARS *Spi)
{
	int i=0;

//	先取 swFIFO的数据，并将其取空。
	while( !swfifo_IsEmpty( &Spi->swfifoRx ) ) {	// 将其取空
		swfifo_Out( msg+i,&Spi->swfifoRx);
		i++;
	}
//	再取 SPI RXBUF的数据，并将其取空。
	while( SPIFFRX_IsNotEmpty(Spi) ) {	// 将其取空
		msg[i] = SPIFFRX_Out(Spi);
		i++;
	}
	if(i)	*(msg+i) = 0;	// 尾

	// 如果rx功能关闭，那么打开它。
	if( SPIFFRX_IsOn(Spi) )
		SPIFFRX_TurnOn(Spi);

	SPIFFRX_ClearINT(Spi);
	return i ? 0: 1;	// return 1: 数据不可用，数据为空。
}

void Spi_RxFifoFullHandler(struct SPI_VARS *Spi)
{
	uint8_t tmpData;
	// 硬件的SPI RXFIFO满了，存入软件FIFO（即swFIFORX）以保证SPI RXFIFO继续接收数据。
	if( !swfifo_IsFull( &Spi->swfifoRx) ) {
		while( !swfifo_IsFull( &Spi->swfifoRx) &&
				SPIFFRX_IsNotEmpty( Spi ) ) {
			// 当SPIRXFIFO非空，且swFIFORX未满时，就往swFIFORX送数据。
			tmpData = SPIFFRX_Out(Spi);
			swfifo_In(tmpData, &Spi->swfifoRx);
		}
		SPIFFRX_ClearINT(Spi);
	} else {
//			 swFifo is full and SPI_FIFO_RX is full.
//		为了防止被重写，这里把RX功能关闭，在gets里重写打开。

//		if(Spi->RegsAddr->SPIFFRX.bit.RXFFOVF)
//			Spi->RegsAddr->SPIFFRX.bit.RXFFOVFCLR = 1;
//		关闭串口接收
		SPIFFRX_TurnOff(Spi);
	}

}

// ========================================================================================
//       no more.
// ========================================================================================

//---------------------------------------------------------------------------
// Example: InitSpiGpio:
//---------------------------------------------------------------------------
// This function initializes GPIO pins to function as SPI pins
//
// Each GPIO pin can be configured as a GPIO pin or up to 3 different
// peripheral functional pins. By default all pins come up as GPIO
// inputs after reset.
//
// Caution:
// For each SPI peripheral
// Only one GPIO pin should be enabled for SPISOMO operation.
// Only one GPIO pin should be enabled for SPISOMI operation.
// Only one GPIO pin should be enabled for SPICLKA operation.
// Only one GPIO pin should be enabled for SPISTEA operation.
// Comment out other unwanted lines.
void InitSpiaGpio(void)
{

	EALLOW;

#if (USE_GPIO16_AS_SPISIMOA)
	GpioCtrlRegs.GPAPUD.bit.GPIO16 = 0;   // Enable pull-up on GPIO16 (SPISIMOA)
	GpioCtrlRegs.GPAQSEL2.bit.GPIO16 = 3; // Asynch input GPIO16 (SPISIMOA)
	GpioCtrlRegs.GPAMUX2.bit.GPIO16 = 1; // Configure GPIO16 as SPISIMOA

#elif(USE_GPIO54_AS_SPISIMOA)
	GpioCtrlRegs.GPBPUD.bit.GPIO54 = 0;   // Enable pull-up on GPIO54 (SPISIMOA)
	GpioCtrlRegs.GPBQSEL2.bit.GPIO54 = 3; // Asynch input GPIO16 (SPISIMOA)
	GpioCtrlRegs.GPBMUX2.bit.GPIO54 = 1; // Configure GPIO54 as SPISIMOA
#elif(USE_SPIA)
#error pin SPISIMOA is not defined.
#endif

#if(USE_GPIO17_AS_SPISOMIA)
	GpioCtrlRegs.GPAPUD.bit.GPIO17 = 0;   // Enable pull-up on GPIO17 (SPISOMIA)
	GpioCtrlRegs.GPAQSEL2.bit.GPIO17 = 3; // Asynch input GPIO17 (SPISOMIA)
	GpioCtrlRegs.GPAMUX2.bit.GPIO17 = 1; // Configure GPIO17 as SPISOMIA

#elif(USE_GPIO55_AS_SPISOMIA)
	GpioCtrlRegs.GPBPUD.bit.GPIO55 = 0;   // Enable pull-up on GPIO55 (SPISOMIA)
	GpioCtrlRegs.GPBQSEL2.bit.GPIO55 = 3; // Asynch input GPIO17 (SPISOMIA)
	GpioCtrlRegs.GPBMUX2.bit.GPIO55 = 1; // Configure GPIO55 as SPISOMIA
#elif(USE_SPIA)
#error pin SPISOMIA is not defined.
#endif

#if(USE_GPIO18_AS_SPICLKA)
	GpioCtrlRegs.GPAPUD.bit.GPIO18 = 0;   // Enable pull-up on GPIO18 (SPICLKA)
	GpioCtrlRegs.GPAQSEL2.bit.GPIO18 = 3; // Asynch input GPIO18 (SPICLKA)
	GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 1; // Configure GPIO18 as SPICLKA

#elif(USE_GPIO56_AS_SPICLKA)
	GpioCtrlRegs.GPBPUD.bit.GPIO56 = 0;   // Enable pull-up on GPIO56 (SPICLKA)
	GpioCtrlRegs.GPBQSEL2.bit.GPIO56 = 3; // Asynch input GPIO18 (SPICLKA)
	GpioCtrlRegs.GPBMUX2.bit.GPIO56 = 1; // Configure GPIO56 as SPICLKA
#elif(USE_SPIA)
#error pin SPICLKA is not defined.
#endif


#if(USE_GPIO19_AS_SPISTEA)
	GpioCtrlRegs.GPAPUD.bit.GPIO19 = 0;   // Enable pull-up on GPIO19 (SPISTEA)
	GpioCtrlRegs.GPAQSEL2.bit.GPIO19 = 3; // Asynch input GPIO19 (SPISTEA)
	GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 1; // Configure GPIO19 as SPISTEA

#elif(USE_GPIO57_AS_SPISTEA)
	GpioCtrlRegs.GPBPUD.bit.GPIO57 = 0;   // Enable pull-up on GPIO57 (SPISTEA)
	GpioCtrlRegs.GPBQSEL2.bit.GPIO57 = 3; // Asynch input GPIO19 (SPISTEA)
	GpioCtrlRegs.GPBMUX2.bit.GPIO57 = 1; // Configure GPIO57 as SPISTEA
#elif(USE_SPIA)
#error pin SPISTEA is not defined.
#endif

	EDIS;
}

//===========================================================================
// End of file.
//===========================================================================
