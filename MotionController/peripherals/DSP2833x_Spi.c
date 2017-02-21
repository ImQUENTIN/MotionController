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
/*
    SPI ʹ��˵����
    1. ����FIFO����
    SPIA �ڲ�ʹ����FIFO�жϣ�Ĭ�ϵĻ��泤���� 16(Ӳ��) + 16���������32�ֳ���
    ��������Ļ����DEPTH����ͨ���޸ı���SPIA_SWFFTXDEEP, SPIA_SWFFRXDEEP
    ���Զ������ã��������޸ģ�line 118-120, @ my_demo_select.h

    2. ���ڷ��ͽ���SPI��ʹ��
    SPIA ʹ���շ��������ʽ�������ֱ��ʹ�ú��� char Spia_gets(char * msg)
    ����ȡ���յ����ݣ�msg�ĳ���Ӧ����16+SPIA_SWFFRXDEEP��
    ��û�����ݽ��յ�ʱ��Spia_gets ����1������0˵��msg��Ϣ��Ч���Խ�һ������
    ע�⣺Spia_gets ���Զ���msg�����һ��д0��β���������һλ����ֵ��0ʱҪע�⡣

    ���͵Ļ���ʹ��Spia_puts(char * msg);

    3. �����������
    a) ���û�м�ʹ��ջ��棬�����ջ�������֮�󽫲����ٽ����µ����ݣ�ֱ���п���
       �Ļ���������Ϊֹ��
    b) �������һ֡����ʱ�������˷��ͻ������Ĵ�С�������ַ����ضϵ�λ�á�
       ���緵��j�������ͻ�������պ���һ֡���ݿ�����ô����Spia_puts(msg+j);

       ����ѻ����������ʺ�ʹ�á�


 */
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

struct SPI_VARS Spia;		// Spia ����


// Test 1,SPIA , 8-bit word,
void InitSpia(void)
{
	void InitSpiaGpio(void);

	Spia.RegsAddr->SPICCR.bit.SPISWRESET = 0;		// force to reset.

	Spia.RegsAddr 	= &SpiaRegs;
	Spia.Baud		= SPIA_BAUD;		// get from user's configuration.

	cb_create(&Spia.cb_rx, sizeof(Uint16), 30);		// ѭ��������
	cb_create(&Spia.cb_tx, sizeof(Uint16), 30);		// ѭ��������

	InitSpiaGpio();

	// 	�޸��ж�������
	EALLOW;  // This is needed to write to EALLOW protected registers
	PieVectTable.SPIRXINTA = &spia_rx_isr;	// 6.1
	PieVectTable.SPITXINTA = &spia_tx_isr;	// 6.2
	EDIS;    // This is needed to disable write to EALLOW protected registers

	//	 									    // 1. �����жϴ򿪡�
	PieCtrlRegs.PIEIER6.bit.INTx1 = 1;  		// 2. PIE�жϴ򿪡�
	PieCtrlRegs.PIEIER6.bit.INTx2 = 1;          //
	IER |= M_INT6;  	                		// 3. CPU���жϴ򿪡�
	//										    // 4. �������жϣ�INTM�������ͳһ�򿪣����ﲻ������

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
//	   Uint16 TXFFIL:5;          // 4:0    0, Interrupt level, Interrupt occurs when TXFFIL equal TXFFST.
//	   Uint16 TXFFIENA:1;        // 5      1, Interrupt enable
//	   Uint16 TXFFINTCLR:1;      // 6      1, Clear INT flag, high active.
//	   Uint16 TXFFINT:1;         // 7      INT flag
//	   Uint16 TXFFST:5;          // 12:8   FIFO status, the number of stored data.
//	   Uint16 TXFIFOXRESET:1;    // 13     1, FIFO reset,low active.
//	   Uint16 SPIFFENA:1;        // 14     1, FIFO Enable
//	   Uint16 SPIRST:1;          // 15     1, SPI reset rx/tx channels
	Spi->RegsAddr->SPIFFTX.all=0xc028; // SPI reset rx/tx channels, tx FIFO; low active.
//										//  Clear tx Fifo INT flag, Enhancement enable.

//    struct  SPIFFRX_BITS {       // bits   description
//       Uint16 RXFFIL:5;          // 4:0    10,Interrupt level, Interrupt occurs when RXFFIL equal RXFFST.
//       Uint16 RXFFIENA:1;        // 5      1, Interrupt enable
//       Uint16 RXFFINTCLR:1;      // 6      1, Clear INT flag
//       Uint16 RXFFINT:1;         // 7      INT flag
//       Uint16 RXFFST:5;          // 12:8   FIFO status, the number of stored data.
//       Uint16 RXFIFORESET:1;     // 13     1, FIFO reset
//       Uint16 RXFFOVRCLR:1;      // 14     1, Clear overflow
//       Uint16 RXFFOVF:1;         // 15     0, FIFO overflow
	Spi->RegsAddr->SPIFFRX.all=0x0028;//0x2070;	// ** �����˸Ļ���***

//	Spi->RegsAddr->SPIFFTX.bit.TXFFIENA 	= 1;	// tx fifo intterrupt enable.
//	Spi->RegsAddr->SPIFFRX.bit.RXFFIENA 	= 1;	// tx fifo intterrupt enable.


//	// SPI FIFO control register bit definitions:
//	struct  SPIFFCT_BITS {     // bits   description
//	   Uint16 FFTXDLY:8;         // 7:0    FIFO transmit delay
//	   Uint16 rsvd:5;            // 12:8   reserved
//	   Uint16 CDC:1;             // 13     Auto baud mode enable
//	   Uint16 ABDCLR:1;          // 14     Auto baud clear
//	   Uint16 ABD:1;             // 15     Auto baud detect
//	};
	Spi->RegsAddr->SPIFFCT.all=0x0;

   SpiaRegs.SPIFFTX.bit.TXFIFORESET = 1;
   SpiaRegs.SPIFFRX.bit.RXFIFORESET = 1;
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
	Spi->RegsAddr->SPICCR.all =0x000f;   	// Reset on, rising edge, 8-bit char bits


	//----------------------------------------------------------
	// Step3.
	//----------------------------------------------------------
//	   Uint16 SPIINTENA:1;       // 0      Interrupt enable
//	   Uint16 TALK:1;            // 1      Master/Slave transmit enable
//	   Uint16 MASTER_SLAVE:1;    // 2      Network control mode
//	   Uint16 CLK_PHASE:1;       // 3      Clock phase select: 0, no delay; 1,SPICLK signal delayed by one half-cycle;
//	   Uint16 OVERRUNINTENA:1;   // 4      Overrun interrupt enable, overwiten.
//	   Uint16 rsvd:11;           // 15:5   reserved
	Spi->RegsAddr->SPICTL.all =  0x0002;   // Enable slave mode, normal phase, Enables interrupt.


//// �����־λ
//	SpiaRegs.SPISTS.bit.BUFFULL_FLAG = 1;
//	SpiaRegs.SPISTS.bit.INT_FLAG = 1;
//	SpiaRegs.SPISTS.bit.OVERRUN_FLAG = 1;

	//----------------------------------------------------------
	// Step4. Enable interrupt, if needed. And Start up.
	//----------------------------------------------------------
//#if (USE_SPI_INT && !USE_SPI_FIFO)
////	û�п���SPI FIFO���ܣ���RX,TX���ж�
//	Spi->RegsAddr->SPICTL.bit.SPIINTENA 		= 1;
//	Spi->RegsAddr->SPICTL.bit.OVERRUNINTENA 	= 1;
//#elif (USE_SPI_INT)
//	Spi->RegsAddr->SPIFFRX.bit.RXFFIENA = 1;
//	Spi->RegsAddr->SPIFFTX.bit.TXFFIENA = 1;
//#endif

	Spi->RegsAddr->SPIPRI.bit.FREE = 1;			// Set so breakpoints don't disturb xmission
	Spi->RegsAddr->SPICCR.bit.SPISWRESET = 1;  	// Relinquish SPI from Reset

}


// ===================================================================================================
//           Below are supports for circle buffer
// ===================================================================================================

////----------------------------------------------------------
////           supports for Spi_gets
////----------------------------------------------------------
//
//char SPIFFRX_IsNotEmpty( struct SPI_VARS *Spi){	return (Spi->RegsAddr->SPIFFRX.bit.RXFFST > 0) ? 1: 0; }
//char SPIFFRX_Out( struct SPI_VARS *Spi){	return ( (Spi->RegsAddr->SPIRXBUF) & 0x00ff );}
//void SPIFFRX_ClearINT( struct SPI_VARS *Spi){	Spi->RegsAddr->SPIFFRX.bit.RXFFINTCLR = 1;}
//
//char SPIFFRX_IsOn( struct SPI_VARS *Spi){return( Spi->RegsAddr->SPIFFTX.bit.SPIFFENA );}
//void SPIFFRX_TurnOn( struct SPI_VARS *Spi){Spi->RegsAddr->SPIFFTX.bit.SPIFFENA = 1;}
//void SPIFFRX_TurnOff( struct SPI_VARS *Spi){	Spi->RegsAddr->SPIFFTX.bit.SPIFFENA = 0;}
//
//char Spi_IsFifoTxEmpty(struct SPI_VARS *Spi ) {	return ( !SPIFFRX_IsNotEmpty(Spi) && swfifo_IsEmpty( &Spi->swfifoTx) ) ? 1 : 0;}
//
////----------------------------------------------------------
////           supports for Spi_puts
////----------------------------------------------------------
//char SPIFFTX_IsNotFull( struct SPI_VARS *Spi){	return (Spi->RegsAddr->SPIFFTX.bit.TXFFST < 16) ? 1: 0;}
//void SPIFFTX_In( char dat, struct SPI_VARS *Spi){	Spi->RegsAddr->SPITXBUF = dat;}
//void SPIFFTX_ClearINT( struct SPI_VARS *Spi){	Spi->RegsAddr->SPIFFTX.bit.TXFFINTCLR = 1;	}// clear INT flag.
////char SPI_IsMater( struct SPI_VARS *Spi ){	return(Spi->RegsAddr->SPICTL.bit.MASTER_SLAVE);}
////void SPI_SetMater( struct SPI_VARS *Spi ){	Spi->RegsAddr->SPICTL.bit.MASTER_SLAVE = 1;}
////void SPI_SetSlaver( struct SPI_VARS *Spi ){	Spi->RegsAddr->SPICTL.bit.MASTER_SLAVE = 0;}


char Spi_putchar(Uint16 dat, struct SPI_VARS *Spi)
{
	// ��swFIFOΪ�յ�����£�����ʹ��Ӳ��FIFO.

	if(Spi->RegsAddr->SPIFFTX.bit.TXFFST < 16 ){
		Spi->RegsAddr->SPITXBUF = dat;
		return 0;
	}
	return 1;
}


////----------------------------------------------------------
////           supports for Spi_puts
////----------------------------------------------------------
//char Spi_puts(char * msg, struct SPI_VARS *Spi)
//{
//	int i;
//	i = 0;
//
//	while(msg[i] != '\0') {
//			if( Spi_putchar(msg[i], Spi) ){
//				SPIFFTX_ClearINT(Spi);
//				return i;	// FIFO���,�����ַ����ضϵ�λ�á�
//			}
//
//		i++;
//	}
//	SPIFFTX_ClearINT(Spi);	// clear INT flag.
//	return 0;	// normal
//}
//
//// ���溯�����ж���ִ�У�ּ����գ����ͣ����FIFO�����ݡ�
//void Spi_TxFifoFullHandler(struct SPI_VARS *Spi)
//{
//	char tmpData;
//	if( !swfifo_IsEmpty(&Spi->swfifoTx) ) {		// swFIFOTX is not empty, load them to SPI TXFIFO.
//		while(  SPIFFTX_IsNotFull(Spi) &&
//		       !swfifo_IsEmpty(&Spi->swfifoTx) ) {
////			 ��swFIFOTX�ǿգ���SPITXFIFOδ��ʱ������SPITXFIFO�����ݡ�
//			swfifo_Out(&tmpData, &Spi->swfifoTx);	// get from swfifoTx
//			SPIFFTX_In(tmpData,Spi);				// load to SPIFFTX
//		}
//
//		if( !SPIFFTX_IsNotFull(Spi) ) { 	// SPIFFTX is full
//			// swFIFOTX�����Էǿգ�����Ҫ��װһ��SPITXFIFO.
//			SPIFFTX_ClearINT(Spi);		// ��֤��SPITXFIFO��ʱ�����жϡ�
//		} else {
//			// eFifo is empty
//			// do nothing, and exit.
//		}
//	} else {
//		// eFifo is empty
//		// do nothing, and exit.
//	}
//}
//
//// �������ڹ̶������Ķ�ȡ
//char Spi_getchar(char *dat, struct SPI_VARS *Spi)
//{
//	if( !swfifo_IsEmpty(&Spi->swfifoRx) ) {
////		���ȴ� swFIFO��ȡ����
//		swfifo_Out(dat, &Spi->swfifoRx);
//	} else if( SPIFFRX_IsNotEmpty(Spi) ) {
////		û�еĻ����� SPI RXBUF��ȡ
//		*dat = SPIFFRX_Out(Spi);
//	} else {
////		��û�У��Ǿ����û���ˡ�
//		return 1;	// ���ݲ����á�
//	}
//	return 0;
//}
//
//
//// ��ȡȫ���յ�������
//char Spi_gets(char * msg, struct SPI_VARS *Spi)
//{
//	int i=0;
//
////	��ȡ swFIFO�����ݣ�������ȡ�ա�
//	while( !swfifo_IsEmpty( &Spi->swfifoRx ) ) {	// ����ȡ��
//		swfifo_Out( msg+i,&Spi->swfifoRx);
//		i++;
//	}
////	��ȡ SPI RXBUF�����ݣ�������ȡ�ա�
//	while( SPIFFRX_IsNotEmpty(Spi) ) {	// ����ȡ��
//		msg[i] = SPIFFRX_Out(Spi);
//		i++;
//	}
//	if(i)	*(msg+i) = 0;	// β
//
//	// ���rx���ܹرգ���ô������
//	if( SPIFFRX_IsOn(Spi) )
//		SPIFFRX_TurnOn(Spi);
//
//	SPIFFRX_ClearINT(Spi);
//	return i ? 0: 1;	// return 1: ���ݲ����ã�����Ϊ�ա�
//}
//
//void Spi_RxFifoFullHandler(struct SPI_VARS *Spi)
//{
//	char tmpData;
//	// Ӳ����SPI RXFIFO���ˣ��������FIFO����swFIFORX���Ա�֤SPI RXFIFO�����������ݡ�
//	if( !swfifo_IsFull( &Spi->swfifoRx) ) {
//		while( !swfifo_IsFull( &Spi->swfifoRx) &&
//				SPIFFRX_IsNotEmpty( Spi ) ) {
//			// ��SPIRXFIFO�ǿգ���swFIFORXδ��ʱ������swFIFORX�����ݡ�
//			tmpData = SPIFFRX_Out(Spi);
//			swfifo_In(tmpData, &Spi->swfifoRx);
//		}
//		SPIFFRX_ClearINT(Spi);
//	} else {
////			 swFifo is full and SPI_FIFO_RX is full.
////		Ϊ�˷�ֹ����д�������RX���ܹرգ���gets����д�򿪡�
//
////		if(Spi->RegsAddr->SPIFFRX.bit.RXFFOVF)
////			Spi->RegsAddr->SPIFFRX.bit.RXFFOVFCLR = 1;
////		�رմ��ڽ���
//		SPIFFRX_TurnOff(Spi);
//	}
//
//}

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
