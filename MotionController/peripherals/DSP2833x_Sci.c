/*
*  Created on: 2016-12-1
*      Author: QUENTIN
*      E-mail: qiyuexin@yeah.net
*/
//###########################################################################
//
// FILE:	DSP2833x_Sci.c
//
// TITLE:	DSP2833x SCI Initialization & Support Functions.
//
//###########################################################################
// "*" means to do, and "+" means done.
// + scia_config
// + Sci_putchar
// + Sci_puts



#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File


// ========================================================================================
//       The following is added by QUENTIN.
// ========================================================================================




void ConfigSci(struct SCI_VARS *Sci);
void InitSciFifo( struct SCI_VARS *Sci);


// -----------------------------------------------------------------------------------------
//   SCIA
// -----------------------------------------------------------------------------------------
#if(USE_SCIA)

struct SCI_VARS Scia;	// SCI A

char SCIA_SWFFTXBUF[SCIA_SWFFTXDEEP];		// SCI Software FIFO Tx Buffer.
char SCIA_SWFFRXBUF[SCIA_SWFFRXDEEP];		// SCI Software FIFO Rx Buffer.

// Test 1,SCIA  DLB, 8-bit word, baud rate 0x000F, default, 1 STOP bit, no parity
void InitScia(void)
{
  void InitSciaGpio();

	Scia.RegsAddr 	= &SciaRegs;
	Scia.Baud		= SCIA_BAUD;			// get from user's configuration.

	swfifoReset(&Scia.swfifoTx);
	swfifoReset(&Scia.swfifoRx);
	Scia.swfifoTx.Buffer = SCIA_SWFFTXBUF;		// tx buffer addr
	Scia.swfifoTx.Deep   = SCIA_SWFFTXDEEP;		// tx buffer size
	Scia.swfifoRx.Buffer = SCIA_SWFFRXBUF;		// Rx buffer addr
	Scia.swfifoRx.Deep   = SCIA_SWFFRXDEEP;		// Rx buffer size

#if DSP28_SCIA
	InitSciaGpio();
#else
#error hardware is not support for 'SCIA'.
#endif


#if(USE_SCI_INT)
	// 	�޸��ж�������
	EALLOW;  // This is needed to write to EALLOW protected registers
	PieVectTable.SCIRXINTA = &scia_rx_isr;
	PieVectTable.SCITXINTA = &scia_tx_isr;
	EDIS;    // This is needed to disable write to EALLOW protected registers

	//	 									    // 1. �����жϴ򿪡�
	PieCtrlRegs.PIEIER9.bit.INTx1 = 1;  		// 2. PIE�жϴ򿪡�
	PieCtrlRegs.PIEIER9.bit.INTx2 = 1;
	IER |= M_INT9;  	                		// 3. CPU���жϴ򿪡�
	//										    // 4. �������жϣ�INTM�������ͳһ�򿪣����ﲻ������
#endif

	// Make sure this peripheral clock is enabled
	EALLOW;
	SysCtrlRegs.PCLKCR0.bit.SCIAENCLK = 1;   // SCI-A
	EDIS;

	InitSciFifo(&Scia);
	ConfigSci(&Scia);


}

#endif // (USE_SCIA)



// -----------------------------------------------------------------------------------------
//   SCIB
// -----------------------------------------------------------------------------------------
#if(USE_SCIB)

struct SCI_VARS Scib;	// SCI B

char SCIB_SWFFTXBUF[SCIB_SWFFTXDEEP];		// SCI Software FIFO Tx Buffer.
char SCIB_SWFFRXBUF[SCIB_SWFFRXDEEP];		// SCI Software FIFO Rx Buffer.

// Test 1,SCIA  DLB, 8-bit word, baud rate 0x000F, default, 1 STOP bit, no parity
void InitScib(void)
{
	void InitScibGpio(void);
	Scib.RegsAddr 	= &ScibRegs;
	Scib.Baud		= SCIB_BAUD;		// get from user's configuration.

	swfifoReset(&Scib.swfifoTx);
	swfifoReset(&Scib.swfifoRx);
	Scib.swfifoTx.Buffer = SCIB_SWFFTXBUF;		// tx buffer addr
	Scib.swfifoTx.Deep   = SCIB_SWFFTXDEEP;		// tx buffer size
	Scib.swfifoRx.Buffer = SCIB_SWFFRXBUF;		// Rx buffer addr
	Scib.swfifoRx.Deep   = SCIB_SWFFRXDEEP;		// Rx buffer size

#if DSP28_SCIB
	InitScibGpio();
#else
#error hardware is not support for 'SCIB'.
#endif


#if(USE_SCI_INT)
	// 	�޸��ж�������
	EALLOW;  // This is needed to write to EALLOW protected registers
	PieVectTable.SCIRXINTB = &scib_rx_isr;
	PieVectTable.SCITXINTB = &scib_tx_isr;
	EDIS;    // This is needed to disable write to EALLOW protected registers

	//	 									    // 1. �����жϴ򿪡�
	PieCtrlRegs.PIEIER9.bit.INTx3 = 1;  		// 2. PIE�жϴ򿪡�
	PieCtrlRegs.PIEIER9.bit.INTx4 = 1;
	IER |= M_INT9;  	                		// 3. CPU���жϴ򿪡�
	//										    // 4. �������жϣ�INTM�������ͳһ�򿪣����ﲻ������
#endif

	// Make sure this peripheral clock is enabled
	EALLOW;
	SysCtrlRegs.PCLKCR0.bit.SCIBENCLK = 1;   // SCI-B
	EDIS;
	InitSciFifo(&Scib);
	ConfigSci(&Scib);

}

#endif // (USE_SCIB)



// -----------------------------------------------------------------------------------------
//   SCIC
// -----------------------------------------------------------------------------------------
#if(USE_SCIC)

struct SCI_VARS Scic;	// SCI C

char SCIC_SWFFTXBUF[SCIC_SWFFTXDEEP];		// SCI Software FIFO Tx Buffer.
char SCIC_SWFFRXBUF[SCIC_SWFFRXDEEP];		// SCI Software FIFO Rx Buffer.

// Test 1,SCIC  DLB, 8-bit word, baud rate 0x000F, default, 1 STOP bit, no parity
void InitScic(void)
{
	void InitScicGpio(void);
	Scic.RegsAddr 	= &ScicRegs;
	Scic.Baud		= SCIC_BAUD;		// get from user's configuration.

	swfifoReset(&Scic.swfifoTx);
	swfifoReset(&Scic.swfifoRx);
	Scic.swfifoTx.Buffer = SCIC_SWFFTXBUF;		// tx buffer addr
	Scic.swfifoTx.Deep   = SCIC_SWFFTXDEEP;		// tx buffer size
	Scic.swfifoRx.Buffer = SCIC_SWFFRXBUF;		// Rx buffer addr
	Scic.swfifoRx.Deep   = SCIC_SWFFRXDEEP;		// Rx buffer size

#if DSP28_SCIC
	InitScicGpio();
#else
#error hardware is not support for 'SCIC'.
#endif


#if(USE_SCI_INT)
	// 	�޸��ж�������
	EALLOW;  // This is needed to write to EALLOW protected registers
	PieVectTable.SCIRXINTC = &scic_rx_isr; 	// 8.5
	PieVectTable.SCITXINTC = &scic_tx_isr;	// 8.6
	EDIS;    // This is needed to disable write to EALLOW protected registers

	//	 									    // 1. �����жϴ򿪡�
	PieCtrlRegs.PIEIER8.bit.INTx5 = 1;  		// 2. PIE�жϴ򿪡�
	PieCtrlRegs.PIEIER8.bit.INTx6 = 1;
	IER |= M_INT8;  	                		// 3. CPU���жϴ򿪡�
	//										    // 4. �������жϣ�INTM�������ͳһ�򿪣����ﲻ������
#endif

	// Make sure this peripheral clock is enabled
	EALLOW;
	SysCtrlRegs.PCLKCR0.bit.SCICENCLK = 1;   // SCI-C
	EDIS;
	InitSciFifo(&Scic);
	ConfigSci(&Scic);
}

#endif // (USE_SCIC)


void InitScis(void)
{
#if( USE_SCIA )
	InitScia();
#endif
#if( USE_SCIB )
	InitScib();
#endif
#if( USE_SCIC )
	InitScic();
#endif
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////                     common                   //////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
char SCIFFTX_IsNotFull( struct SCI_VARS *Sci)
{
	return (Sci->RegsAddr->SCIFFTX.bit.TXFFST < 16) ? 1: 0;
}
char SCIFFTX_IsEmpty( struct SCI_VARS *Sci)
{
	return (Sci->RegsAddr->SCIFFTX.bit.TXFFST == 0) ? 1: 0;
}
void SCIFFTX_In( char dat, struct SCI_VARS *Sci)
{
	Sci->RegsAddr->SCITXBUF = dat;
}
void SCIFFTX_ClearINT( struct SCI_VARS *Sci)
{
	Sci->RegsAddr->SCIFFTX.bit.TXFFINTCLR = 1;	       // clear INT flag.
}
char Sci_IsFifoTxEmpty(struct SCI_VARS *Sci )
{
	return ( SCIFFTX_IsEmpty(Sci) && swfifo_IsEmpty( &Sci->swfifoTx) ) ? 1 : 0;
}

char SCIFFRX_IsNotEmpty( struct SCI_VARS *Sci)
{
	return (Sci->RegsAddr->SCIFFRX.bit.RXFFST > 0) ? 1: 0;
}
char SCIFFRX_Out( struct SCI_VARS *Sci)
{
	return ( Sci->RegsAddr->SCIRXBUF.bit.RXDT );
}
void SCIFFRX_ClearINT( struct SCI_VARS *Sci)
{
	Sci->RegsAddr->SCIFFRX.bit.RXFFINTCLR = 1;
}
char SCIRX_IsOn( struct SCI_VARS *Sci)
{
	return( Sci->RegsAddr->SCICTL1.bit.RXENA );
}
void SCIRX_TurnOn( struct SCI_VARS *Sci)
{
	Sci->RegsAddr->SCICTL1.bit.RXENA = 1;
}
void SCIRX_TurnOff( struct SCI_VARS *Sci)
{
	Sci->RegsAddr->SCICTL1.bit.RXENA = 0;
}


char Sci_putchar(char dat, struct SCI_VARS *Sci)
{
	// ��swFIFOΪ�յ�����£�����ʹ��Ӳ��FIFO.
	if( SCIFFTX_IsNotFull(Sci) && swfifo_IsEmpty(&Sci->swfifoTx) )
		SCIFFTX_In(dat, Sci);
	else	// SCI FIFO ���� ����swFIFO�ǿ� ����swFIFO.
		if( !swfifo_IsFull(&Sci->swfifoTx) )
			swfifo_In(dat, &Sci->swfifoTx);
		else
			return 1; 	// ������FIFO����ʱ�����������ݲ��ܴ��룬���������־��
	return 0;	// ����
}


//----------------------------------------------------------
//           supports for Sci_puts
//----------------------------------------------------------
char Sci_puts(char * msg, struct SCI_VARS *Sci)
{
	int i;
	i = 0;

	while(msg[i] != '\0') {
//	// �˲�������Ϊǿ�Ʒ��ͣ�������֮��ȴ��пռ���ٷ��͡�
//#if(USE_SCI_FIFO & USE_SCI_INT)	// SCI FIFO �����жϵĴ��䷽ʽ����Ҫ swFIFO��֧�֡�
//
//		//	FIFO���˿��Դ�������FIFO�ռ䣬�ÿռ����ݻ���Sci��SCI FIFO�����ݷ�����ɺ󣬴����жϣ�
//		//	Ȼi�����ж��﷢�ͳ�ȥ�����Ըù�����Ҫ�жϵ�֧�֡�
//		if(Sci->RegsAddr->SCIFFTX.bit.TXFFST > 15 ||
//		    Sci->swFifo.TxSta)	{
////			 ���ʹ�������FIFO�������FIFO���ȣ��˾�Ϊ�˱�֤�ַ���˳�򲻱䡣
////			���TXFIFO�����ݻ���SCIFIFO���ʱ�����ж��Զ�����SCITXFIFO.
////			Ӳ��SCI TXFIFO�����ˣ�ʹ�����TXFIFO
//			if(Sci->swFifo.TxSta < Sci->swFifo.fifoTxDeep)
//				Sci_InSwfifoTx(msg[i],&Sci->swFifo);	// �пռ�Ļ�ֱ�Ӵ��롣
//			else {
////				 swFIFO�ռ����꣬��ʱ����ȴ�SCIFIFOTX���жϰ�swFIFO��һ�������ݣ�16words��ȡ��,
////				Ϊ��֤SCIFIFOTX�ж�˳�����룬CLR���жϱ�־λ��
//				Sci->RegsAddr->SCIFFTX.bit.TXFFINTCLR = 1;	// clear INT flag.
//				while(Sci->swFifo.TxSta == Sci->swFifo.fifoTxDeep);	// �ȴ�swFIFO�����ݱ�ȡ�ߡ�
//				Sci_InSwfifoTx(msg[i],&Sci->swFifo);	// swFIFO�пռ��ˣ����Դ��롣
//			}
//		} else //Sci_putchar(msg[i], Sci);
//#else
//		while ( !Sci->RegsAddr->SCICTL2.bit.TXRDY ) ;		// TX��λ�Ĵ���û�в�����ɡ�
//#endif
		if( Sci_putchar(msg[i], Sci) ) {
			SCIFFTX_ClearINT(Sci);
			return i;	// FIFO���,�����ַ����ضϵ�λ�á�
		}

		i++;
	}
	SCIFFTX_ClearINT(Sci);	// clear INT flag.
	return 0;	// normal
}

// ���溯�����ж���ִ�У�ּ����գ����ͣ����FIFO�����ݡ�
void Sci_TxFifoFullHandler(struct SCI_VARS *Sci)
{
	char tmpData;
	if( !swfifo_IsEmpty(&Sci->swfifoTx) ) {		// swFIFOTX is not empty, load them to SCI TXFIFO.
		while(  SCIFFTX_IsNotFull(Sci) &&
		        !swfifo_IsEmpty(&Sci->swfifoTx) ) {
//			 ��swFIFOTX�ǿգ���SCITXFIFOδ��ʱ������SCITXFIFO�����ݡ�
			swfifo_Out(&tmpData, &Sci->swfifoTx);	// get from swfifoTx
			SCIFFTX_In(tmpData,Sci);				// load to SCIFFTX
		}

		if( !SCIFFTX_IsNotFull(Sci) ) { 	// SCIFFTX is full
			// swFIFOTX�����Էǿգ�����Ҫ��װһ��SCITXFIFO.
			SCIFFTX_ClearINT(Sci);		// ��֤��SCITXFIFO��ʱ�����жϡ�
		} else {
			// eFifo is empty
			// do nothing, and exit.
		}
	} else {
		// eFifo is empty
		// do nothing, and exit.
	}
}

////----------------------------------------------------------
////                      Sci_RxswFifo
////----------------------------------------------------------

//----------------------------------------------------------
//           supports for Sci_gets
//----------------------------------------------------------

// �������ڹ̶������Ķ�ȡ
char Sci_getchar(char *dat, struct SCI_VARS *Sci)
{
	if( !swfifo_IsEmpty(&Sci->swfifoRx) ) {
//		���ȴ� swFIFO��ȡ����
		swfifo_Out(dat, &Sci->swfifoRx);
	} else if( SCIFFRX_IsNotEmpty(Sci) ) {
//		û�еĻ����� SCI RXBUF��ȡ
		*dat = SCIFFRX_Out(Sci);
	} else {
//		��û�У��Ǿ����û���ˡ�
		return 1;	// ���ݲ����á�
	}
	return 0;
}


// ��ȡȫ���յ�������
char Sci_gets(char * msg, struct SCI_VARS *Sci)
{
	int i=0;

//	��ȡ swFIFO�����ݣ�������ȡ�ա�
	while( !swfifo_IsEmpty( &Sci->swfifoRx ) ) {	// ����ȡ��
		swfifo_Out( msg+i,&Sci->swfifoRx);
		i++;
	}
//	��ȡ SCI RXBUF�����ݣ�������ȡ�ա�
	while( SCIFFRX_IsNotEmpty(Sci) ) {	// ����ȡ��
		msg[i] = SCIFFRX_Out(Sci);
		i++;
	}

	if(i) *(msg+i) = 0;	// β

	// ���rx���ܹرգ���ô������
	if( !SCIRX_IsOn(Sci) )
		SCIRX_TurnOn(Sci);

	SCIFFRX_ClearINT(Sci);
	return i ? 0: 1;	// return 1: ���ݲ����ã�����Ϊ�ա�
}

void Sci_RxFifoFullHandler(struct SCI_VARS *Sci)
{
	char tmpData;
	// Ӳ����SCI RXFIFO���ˣ��������FIFO����swFIFORX���Ա�֤SCI RXFIFO�����������ݡ�
	if( !swfifo_IsFull( &Sci->swfifoRx) ) {
		while( !swfifo_IsFull( &Sci->swfifoRx) &&
		       SCIFFRX_IsNotEmpty( Sci ) ) {
			// ��SCIRXFIFO�ǿգ���swFIFORXδ��ʱ������swFIFORX�����ݡ�
			tmpData = SCIFFRX_Out(Sci);
			swfifo_In(tmpData, &Sci->swfifoRx);
		}
		SCIFFRX_ClearINT(Sci);
	} else {
//			 swFifo is full and SCI_FIFO_RX is full.
//		Ϊ�˷�ֹ����д�������RX���ܹرգ���gets����д�򿪡�

//		if(Sci->RegsAddr->SCIFFRX.bit.RXFFOVF)
//			Sci->RegsAddr->SCIFFRX.bit.RXFFOVFCLR = 1;
//		�رմ��ڽ���
		SCIRX_TurnOff(Sci);
	}

}


void swfifoClear( struct SCI_VARS *Sci )
{
	Sci->swfifoTx.Sta    = 0;					// tx status
	Sci->swfifoTx.WrP    = 0;					// tx write point
	Sci->swfifoTx.RdP    = 0;					// tx read point

	Sci->swfifoRx.Sta    = 0;					// Rx status
	Sci->swfifoRx.WrP    = 0;					// Rx write point
	Sci->swfifoRx.RdP    = 0;					// Rx read point
}

// Initalize the SCI FIFO
void InitSciFifo( struct SCI_VARS *Sci)
{
//	struct  SCIFFTX_BITS {       // bit    description
//	   Uint16 TXFFIL:5;          // 4:0    Interrupt level, Interrupt occurs when TXFFIL equal TXFFST.
//	   Uint16 TXFFIENA:1;        // 5      Interrupt enable
//	   Uint16 TXFFINTCLR:1;      // 6      Clear INT flag, high active.
//	   Uint16 TXFFINT:1;         // 7      INT flag
//	   Uint16 TXFFST:5;          // 12:8   FIFO status, the number of stored data.
//	   Uint16 TXFIFOXRESET:1;    // 13     FIFO reset,low active.
//	   Uint16 SCIFFENA:1;        // 14     FIFO Enable
//	   Uint16 SCIRST:1;          // 15     SCI reset rx/tx channels

	Sci->RegsAddr->SCIFFTX.all=0xE040;	// SCI reset rx/tx channels, tx FIFO; low active.
	//  Clear tx Fifo INT flag, Enhancement enable.

//    struct  SCIFFRX_BITS {       // bits   description
//       Uint16 RXFFIL:5;          // 4:0    Interrupt level, Interrupt occurs when RXFFIL equal RXFFST.
//       Uint16 RXFFIENA:1;        // 5      Interrupt enable
//       Uint16 RXFFINTCLR:1;      // 6      Clear INT flag
//       Uint16 RXFFINT:1;         // 7      INT flag
//       Uint16 RXFFST:5;          // 12:8   FIFO status, the number of stored data.
//       Uint16 RXFIFORESET:1;     // 13     FIFO reset
//       Uint16 RXFFOVFCLR:1;      // 14     Clear overflow
//       Uint16 RXFFOVF:1;         // 15     FIFO overflow
	Sci->RegsAddr->SCIFFRX.all=0x2050;	//

#if(USE_SCI_INT)
	Sci->RegsAddr->SCIFFTX.bit.TXFFIENA 	= 1;	// tx fifo intterrupt enable.
	Sci->RegsAddr->SCIFFRX.bit.RXFFIENA 	= 1;	// tx fifo intterrupt enable.
#endif

//	// SCI FIFO control register bit definitions:
//	struct  SCIFFCT_BITS {     // bits   description
//	   Uint16 FFTXDLY:8;         // 7:0    FIFO transmit delay
//	   Uint16 rsvd:5;            // 12:8   reserved
//	   Uint16 CDC:1;             // 13     Auto baud mode enable
//	   Uint16 ABDCLR:1;          // 14     Auto baud clear
//	   Uint16 ABD:1;             // 15     Auto baud detect
//	};
	Sci->RegsAddr->SCIFFCT.all=0x0;

}


void ConfigSci(struct SCI_VARS *Sci)
{
	Uint16 tmp;
	//----------------------------------------------------------
	// Step1. Configure the baud.
	//----------------------------------------------------------
	//	baud = LSPCK / ( 8*(BAUD_RATE_REG + 1) )) ;
	//	=> BBR = LSPCK/(8*baud) - 1;
	tmp = (Uint16)( LSPCLK_HZ / (8* Sci->Baud )  - 1 );
	Sci->RegsAddr->SCILBAUD	= tmp&0x00ff;
	Sci->RegsAddr->SCIHBAUD	= (tmp>>8)&0x00ff;

	//----------------------------------------------------------
	// Step2. Configure the Data Format.
	//----------------------------------------------------------
	// 		Name				  // bit    description
	//	Uint16 SCICHAR:3;         // 2:0    Character length control
	//	Uint16 ADDRIDLE_MODE:1;   // 3      ADDR/IDLE Mode control
	//	Uint16 LOOPBKENA:1;       // 4      Loop Back enable or not.
	//	Uint16 PARITYENA:1;       // 5      Parity enable or not.
	//	Uint16 PARITY:1;          // 6      Even or Odd Parity
	//	Uint16 STOPBITS:1;        // 7      Two or One Stop Bit
	//	Uint16 rsvd1:8;           // 15:8   reserved
	Sci->RegsAddr->SCICCR.all =0x0007;   	// 1 stop bit,  No loopback
//										 	// No parity,8 char bits,
//										 	// async mode, idle-line protocol

	//----------------------------------------------------------
	// Step3. Configure the Sci work mode and enable Tx,Rx.
	//----------------------------------------------------------
	// 	struct  SCICTL1_BITS {       // bit    description
	// 	   Uint16 RXENA:1;           // 0      SCI receiver enable
	// 	   Uint16 TXENA:1;           // 1      SCI transmitter enable
	// 	   Uint16 SLEEP:1;           // 2      SCI sleep
	// 	   Uint16 TXWAKE:1;          // 3      Transmitter wakeup method
	// 	   Uint16 rsvd:1;            // 4      reserved
	// 	   Uint16 SWRESET:1;         // 5      Software reset, low active.
	// 	   Uint16 RXERRINTENA:1;     // 6      Recieve Error interrupt enable
	// 	   Uint16 rsvd1:9;           // 15:7   reserved
	Sci->RegsAddr->SCICTL1.all =0x0003;  	// enable TX, RX, internal SCICLK,
	//								    	// Disable RX ERR, SLEEP, TXWAKE

	//	struct  SCICTL2_BITS {       // bit    description
	//	   Uint16 TXINTENA:1;        // 0      Transmit interrupt enable
	//	   Uint16 RXBKINTENA:1;      // 1      Receiver-buffer break enable
	//	   Uint16 rsvd:4;            // 5:2    reserved
	//	   Uint16 TXEMPTY:1;         // 6      *Transmitter empty flag
	//	   Uint16 TXRDY:1;           // 7      Transmitter ready flag
	//	   Uint16 rsvd1:8;           // 15:8   reserved
	Sci->RegsAddr->SCICTL2.all =0x0003;		//	Tx and Rx Buffer Interrupt enable
	//								    	//  clear flag.

	//----------------------------------------------------------
	// Step4. Enable interrupt, if needed. And Start up.
	//----------------------------------------------------------
#if (USE_SCI_INT && !USE_SCI_FIFO)
//	û�п���SCI FIFO���ܣ���RX,TX���ж�
	Sci->RegsAddr->SCICTL2.bit.TXINTENA 	= 1;
	Sci->RegsAddr->SCICTL2.bit.RXBKINTENA 	= 1;	// ��RX��������10��buad clock �ĵ͵�ƽʱ��
//													// Ҳ������жϣ�����ͨ��SCIRXST.BRKDT�鿴��
// ��������BRKDT��־����ʹ�������λ��SCICTL1.SWRESET������Ӳ��ϵͳ��λ-_-|||��
#endif
	Sci->RegsAddr->SCICTL1.bit.SWRESET = 1;  // Relinquish SCI from Reset

}


// ========================================================================================
//       no more.
// ========================================================================================


//---------------------------------------------------------------------------
// Example: InitSciGpio:
//---------------------------------------------------------------------------
// This function initializes GPIO pins to function as SCI pins
//
// Each GPIO pin can be configured as a GPIO pin or up to 3 different
// peripheral functional pins. By default all pins come up as GPIO
// inputs after reset.
//
// Caution:
// Only one GPIO pin should be enabled for SCITXDA/B operation.
// Only one GPIO pin shoudl be enabled for SCIRXDA/B operation.
// Comment out other unwanted lines.


#if DSP28_SCIA
void InitSciaGpio()
{
	EALLOW;

#if ( USE_GPIO36_AS_SCIRXDA )
	GpioCtrlRegs.GPBPUD.bit.GPIO36 = 0;    // Enable pull-up for GPIO36 (SCIRXDA)
	GpioCtrlRegs.GPBQSEL1.bit.GPIO36 = 3;  // Asynch input GPIO36 (SCIRXDA)
	GpioCtrlRegs.GPBMUX1.bit.GPIO36 = 1;   // Configure GPIO36 for SCIRXDA operation
#elif ( USE_GPIO28_AS_SCIRXDA )
	GpioCtrlRegs.GPAPUD.bit.GPIO28 = 0;    // Enable pull-up for GPIO28 (SCIRXDA)
	GpioCtrlRegs.GPAQSEL2.bit.GPIO28 = 3;  // Asynch input GPIO28 (SCIRXDA)
	GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 1;   // Configure GPIO28 for SCIRXDA operation
#elif (USE_SCIA)
#error pin SCIRXDA is not defined.
#endif

#if ( USE_GPIO35_AS_SCITXDA )
	GpioCtrlRegs.GPBPUD.bit.GPIO35 = 0;	   // Enable pull-up for GPIO35 (SCITXDA)
	GpioCtrlRegs.GPBMUX1.bit.GPIO35 = 1;   // Configure GPIO35 for SCITXDA operation
#elif ( USE_GPIO29_AS_SCITXDA )
	GpioCtrlRegs.GPAPUD.bit.GPIO29 = 0;	   // Enable pull-up for GPIO29 (SCITXDA)
	GpioCtrlRegs.GPAMUX2.bit.GPIO29 = 1;   // Configure GPIO29 for SCITXDA operation
#elif (USE_SCIA)
#error pin SCITXDA is not defined.
#endif

	EDIS;
}
#endif

#if DSP28_SCIB
void InitScibGpio(void)
{
	EALLOW;

#if ( USE_GPIO11_AS_SCIRXDB )
	GpioCtrlRegs.GPAPUD.bit.GPIO11  = 0;   // Enable pull-up for GPIO11 (SCIRXDB)
	GpioCtrlRegs.GPAQSEL1.bit.GPIO11 = 3;  // Asynch input GPIO11 (SCIRXDB)
	GpioCtrlRegs.GPAMUX1.bit.GPIO11 = 2;   // Configure GPIO11 for SCIRXDB operation
#elif ( USE_GPIO15_AS_SCIRXDB )
	GpioCtrlRegs.GPAPUD.bit.GPIO15 = 0;    // Enable pull-up for GPIO15 (SCIRXDB)
	GpioCtrlRegs.GPAQSEL1.bit.GPIO15 = 3;  // Asynch input GPIO15 (SCIRXDB)
	GpioCtrlRegs.GPAMUX1.bit.GPIO15 = 2;   // Configure GPIO15 for SCIRXDB operation
#elif ( USE_GPIO19_AS_SCIRXDB )
	GpioCtrlRegs.GPAPUD.bit.GPIO19 = 0;    // Enable pull-up for GPIO19 (SCIRXDB)
	GpioCtrlRegs.GPAQSEL2.bit.GPIO19 = 3;  // Asynch input GPIO19 (SCIRXDB)
	GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 2;   // Configure GPIO19 for SCIRXDB operation
#elif ( USE_GPIO23_AS_SCIRXDB )
	GpioCtrlRegs.GPAPUD.bit.GPIO23 = 0;    // Enable pull-up for GPIO23 (SCIRXDB)
	GpioCtrlRegs.GPAQSEL1.bit.GPIO23 = 3;  // Asynch input GPIO23 (SCIRXDB)
	GpioCtrlRegs.GPAMUX1.bit.GPIO23 = 3;   // Configure GPIO23 for SCIRXDB operation
#elif (USE_SCIB)
#error pin SCIRXDB is not defined.
#endif

#if ( USE_GPIO9_AS_SCITXDB )
	GpioCtrlRegs.GPAPUD.bit.GPIO9 = 0;      // Enable pull-up for GPIO9  (SCITXDB)
	GpioCtrlRegs.GPAMUX1.bit.GPIO9 = 2;     // Configure GPIO9 for SCITXDB operation
#elif ( USE_GPIO14_AS_SCITXDB )
	GpioCtrlRegs.GPAPUD.bit.GPIO14 = 0;     // Enable pull-up for GPIO14  (SCITXDB)
	GpioCtrlRegs.GPAMUX1.bit.GPIO14 = 2;    // Configure GPIO14 for SCITXDB operation
#elif ( USE_GPIO18_AS_SCITXDB )
	GpioCtrlRegs.GPAPUD.bit.GPIO18 = 0;     // Enable pull-up for GPIO18  (SCITXDB)
	GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 2;    // Configure GPIO18 for SCITXDB operation
	GpioCtrlRegs.GPAQSEL2.bit.GPIO18 = 3;  // Asynch input GPIO19 (SCIRXDB)
#elif ( USE_GPIO22_AS_SCITXDB )
	GpioCtrlRegs.GPAPUD.bit.GPIO22 = 0;     // Enable pull-up for GPIO22  (SCITXDB)
	GpioCtrlRegs.GPAMUX1.bit.GPIO22 = 2;    // Configure GPIO22 for SCITXDB operation
#elif (USE_SCIB)
#error pin SCITXDB is not defined.
#endif

	EDIS;
}
#endif // if DSP28_SCIB 

#if DSP28_SCIC
void InitScicGpio()
{
	EALLOW;

	/* Enable internal pull-up for the selected pins */
// Pull-ups can be enabled or disabled disabled by the user.
// This will enable the pullups for the specified pins.

	GpioCtrlRegs.GPBPUD.bit.GPIO62 = 0;    // Enable pull-up for GPIO62 (SCIRXDC)
	GpioCtrlRegs.GPBPUD.bit.GPIO63 = 0;	   // Enable pull-up for GPIO63 (SCITXDC)

	/* Set qualification for selected pins to asynch only */
// Inputs are synchronized to SYSCLKOUT by default.
// This will select asynch (no qualification) for the selected pins.

	GpioCtrlRegs.GPBQSEL2.bit.GPIO62 = 3;  // Asynch input GPIO62 (SCIRXDC)

	/* Configure SCI-C pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be SCI functional pins.

	GpioCtrlRegs.GPBMUX2.bit.GPIO62 = 1;   // Configure GPIO62 for SCIRXDC operation
	GpioCtrlRegs.GPBMUX2.bit.GPIO63 = 1;   // Configure GPIO63 for SCITXDC operation

	EDIS;
}
#endif // if DSP28_SCIC 


//===========================================================================
// End of file.
//===========================================================================
