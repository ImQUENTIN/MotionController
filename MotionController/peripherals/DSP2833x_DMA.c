/*
*  Created on: 2016-12-8
*      Author: QUENTIN
*      E-mail: qiyuexin@yeah.net
*/
//###########################################################################
//
// FILE:	DSP2833x_DMA.c
//
// TITLE:	DSP2833x DMA Initialization & Support Functions.
//
//###########################################################################
// "*" means to do, and "+" means done.



#include "DSP2833x_Device.h"     // Headerfile Include File
#include "DSP2833x_Examples.h"   // Examples Include File

struct DMA_VARS Dma;


void InitDmaCh1(void)
{
#if(USE_DMA_CH1)
	struct DMACH_SETS mySets = Dma.Sets;

//-----------------------------------------------------------------
//  you can do some specific setting based on the default setting.
	mySets.BurstSets.bsize 		= DMA_BSIZE-1;
	mySets.TransferSets.tsize 	= DMA_TSIZE-1;
	mySets.ModeSets.CONTINUOUS 	= CONT_DISABLE;
//-----------------------------------------------------------------

	DMACHxConfig(&mySets, &Dma.RegsAddr->CH1);
	DMACHxAddrConfig(DMACh1Dst, DMACh1Src, &Dma.RegsAddr->CH1);
//	StartDMACHx( &Dma.RegsAddr->CH1);

	// 	修改中断向量表：
	EALLOW;  // This is needed to write to EALLOW protected registers
	PieVectTable.DINTCH1 = &dma_ch1_isr;		// 7.1
	EDIS;    // This is needed to disable write to EALLOW protected registers
	//	 									    // 1. 外设中断打开。
	PieCtrlRegs.PIEIER7.bit.INTx1 = 1;  		// 2. PIE中断打开。
	IER |= M_INT7;  	                		// 3. CPU核中断打开。
	//										    // 4. 开启总中断，INTM。在最后统一打开，这里不开启。

#endif
}

void InitDmaCh2(void)
{
#if(USE_DMA_CH2)
	struct DMACH_SETS mySets = Dma.Sets;

//-----------------------------------------------------------------
//  you can do some specific setting based on the default setting.
	mySets.BurstSets.bsize 		= DMA_BSIZE-1;
	mySets.TransferSets.tsize 	= DMA_TSIZE-1;
	mySets.ModeSets.CONTINUOUS 	= CONT_DISABLE;
//-----------------------------------------------------------------

	DMACHxConfig(&mySets, &Dma.RegsAddr->CH2);
	DMACHxAddrConfig(DMACh2Dst, DMACh2Src, &Dma.RegsAddr->CH2);
//	StartDMACHx( &Dma.RegsAddr->CH2);

	// 	修改中断向量表：
	EALLOW;  // This is needed to write to EALLOW protected registers
	PieVectTable.DINTCH2 = &dma_ch2_isr;		// 7.2
	EDIS;    // This is needed to disable write to EALLOW protected registers
	//	 									    // 1. 外设中断打开。
	PieCtrlRegs.PIEIER7.bit.INTx2 = 1;  		// 2. PIE中断打开。
	IER |= M_INT7;  	                		// 3. CPU核中断打开。
	//										    // 4. 开启总中断，INTM。在最后统一打开，这里不开启。

#endif
}

void InitDmaCh3(void)
{
#if(USE_DMA_CH3)
	struct DMACH_SETS mySets = Dma.Sets;

//-----------------------------------------------------------------
//  you can do some specific setting based on the default setting.
	mySets.BurstSets.bsize 		= DMA_BSIZE-1;
	mySets.TransferSets.tsize 	= DMA_TSIZE-1;
	mySets.ModeSets.CONTINUOUS 	= CONT_DISABLE;
//-----------------------------------------------------------------

	DMACHxConfig(&mySets, &Dma.RegsAddr->CH3);
	DMACHxAddrConfig(DMACh3Dst, DMACh3Src, &Dma.RegsAddr->CH3);
//	StartDMACHx( &Dma.RegsAddr->CH3);

	// 	修改中断向量表：
	EALLOW;  // This is needed to write to EALLOW protected registers
	PieVectTable.DINTCH3 = &dma_ch3_isr;		// 7.3
	EDIS;    // This is needed to disable write to EALLOW protected registers
	//	 									    // 1. 外设中断打开。
	PieCtrlRegs.PIEIER7.bit.INTx3 = 1;  		// 2. PIE中断打开。
	IER |= M_INT7;  	                		// 3. CPU核中断打开。
	//										    // 4. 开启总中断，INTM。在最后统一打开，这里不开启。

#endif
}

void InitDmaCh4(void)
{
#if(USE_DMA_CH4)
	struct DMACH_SETS mySets = Dma.Sets;

//-----------------------------------------------------------------
//  you can do some specific setting based on the default setting.
	mySets.BurstSets.bsize 		= DMA_BSIZE-1;
	mySets.TransferSets.tsize 	= DMA_TSIZE-1;
	mySets.ModeSets.CONTINUOUS 	= CONT_DISABLE;
//-----------------------------------------------------------------

	DMACHxConfig(&mySets, &Dma.RegsAddr->CH4);
	DMACHxAddrConfig(DMACh4Dst, DMACh4Src, &Dma.RegsAddr->CH4);
//	StartDMACHx( &Dma.RegsAddr->CH4);

	// 	修改中断向量表：
	EALLOW;  // This is needed to write to EALLOW protected registers
	PieVectTable.DINTCH4 = &dma_ch4_isr;		// 7.4
	EDIS;    // This is needed to disable write to EALLOW protected registers
	//	 									    // 1. 外设中断打开。
	PieCtrlRegs.PIEIER7.bit.INTx4 = 1;  		// 2. PIE中断打开。
	IER |= M_INT7;  	                		// 3. CPU核中断打开。
	//										    // 4. 开启总中断，INTM。在最后统一打开，这里不开启。

#endif
}

void InitDmaCh5(void)
{
#if(USE_DMA_CH5)
	struct DMACH_SETS mySets = Dma.Sets;

//-----------------------------------------------------------------
//  you can do some specific setting based on the default setting.
	mySets.BurstSets.bsize 		= DMA_BSIZE-1;
	mySets.TransferSets.tsize 	= DMA_TSIZE-1;
	mySets.ModeSets.CONTINUOUS 	= CONT_DISABLE;
//-----------------------------------------------------------------

	DMACHxConfig(&mySets, &Dma.RegsAddr->CH5);
	DMACHxAddrConfig(DMACh5Dst, DMACh5Src, &Dma.RegsAddr->CH5);
//	StartDMACHx( &Dma.RegsAddr->CH5);

	// 	修改中断向量表：
	EALLOW;  // This is needed to write to EALLOW protected registers
	PieVectTable.DINTCH5 = &dma_ch5_isr;		// 7.5
	EDIS;    // This is needed to disable write to EALLOW protected registers
	//	 									    // 1. 外设中断打开。
	PieCtrlRegs.PIEIER7.bit.INTx5 = 1;  		// 2. PIE中断打开。
	IER |= M_INT5;  	                		// 3. CPU核中断打开。
	//										    // 4. 开启总中断，INTM。在最后统一打开，这里不开启。

#endif
}

void InitDmaCh6(void)
{
#if(USE_DMA_CH6)
	struct DMACH_SETS mySets = Dma.Sets;

//-----------------------------------------------------------------
//  you can do some specific setting based on the default setting.
	mySets.BurstSets.bsize 		= DMA_BSIZE-1;
	mySets.TransferSets.tsize 	= DMA_TSIZE-1;
	mySets.ModeSets.CONTINUOUS 	= CONT_DISABLE;
//-----------------------------------------------------------------

	DMACHxConfig(&mySets, &Dma.RegsAddr->CH6);
	DMACHxAddrConfig(DMACh6Dst, DMACh6Src, &Dma.RegsAddr->CH6);
//	StartDMACHx( &Dma.RegsAddr->CH6);

	// 	修改中断向量表：
	EALLOW;  // This is needed to write to EALLOW protected registers
	PieVectTable.DINTCH6 = &dma_ch6_isr;		// 7.6
	EDIS;    // This is needed to disable write to EALLOW protected registers
	//	 									    // 1. 外设中断打开。
	PieCtrlRegs.PIEIER7.bit.INTx1 = 1;  		// 2. PIE中断打开。
	IER |= M_INT7;  	                		// 3. CPU核中断打开。
	//										    // 4. 开启总中断，INTM。在最后统一打开，这里不开启。

#endif
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////                     common                   //////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////

void InitDmas(void)
{
	Dma.RegsAddr = &DmaRegs;

	DMALoadDefaultSets();
	DMAInitialize();
	InitDmaCh1();
	InitDmaCh2();
	InitDmaCh3();
	InitDmaCh4();
	InitDmaCh5();
	InitDmaCh6();
}

void DMALoadDefaultSets(void)
{
	// Burst Sets
	Dma.Sets.BurstSets.bsize 		= DMA_BSIZE-1;	// 一包有多少个DATASIZE（数据的位数）的数据，(bsize+1)个。
	Dma.Sets.BurstSets.desbstep 	= 2;		// 如果DATASIZE是32，这里偏移地址为2；
	Dma.Sets.BurstSets.srcbstep 	= 2;		// 如果DATASIZE是16，这里偏移地址为1；
												// 不过，即使数据是Uint16类型的仍可以使用DATASIZE=32发送。
	// Transfer Sets
	Dma.Sets.TransferSets.tsize 	= DMA_TSIZE-1;		// 该通道，传输一次发送几包，发送(tsize+1)包
	Dma.Sets.TransferSets.deststep 	= 2;		// 与burst的类似。
	Dma.Sets.TransferSets.srctstep 	= 2;

	// Wrap Sets
	Dma.Sets.WrapSets.deswsize = 0xffff;		// 它的size > TRANSFERSIZE 表示禁用。
	Dma.Sets.WrapSets.srcwsize = 0xffff;		// 同上
	Dma.Sets.WrapSets.deswstep = 0;
	Dma.Sets.WrapSets.srcwstep = 0;

	// Mode Sets
	Dma.Sets.ModeSets.PERINTSEL = DMA_TINT0;		// 同步的中断源，也就是说要想触发一次传输满足两个条件：
	Dma.Sets.ModeSets.PERINTE	= PERINT_ENABLE;	// 1. CONTROL.RUN = 1, 当 CONTINUOUS = 0时，触发完一次会自动关闭；
	Dma.Sets.ModeSets.CHINTMODE	= CHINT_END;		// 2. 中断源的中断信号。
	Dma.Sets.ModeSets.CHINTE	= CHINT_ENABLE;
	Dma.Sets.ModeSets.SYNCE		= SYNC_DISABLE;
	Dma.Sets.ModeSets.SYNCSEL	= SYNC_SRC;

	Dma.Sets.ModeSets.ONESHOT	= ONESHOT_ENABLE;
	Dma.Sets.ModeSets.CONTINUOUS = CONT_DISABLE;
	Dma.Sets.ModeSets.OVRINTE	= OVRFLOW_DISABLE;
	Dma.Sets.ModeSets.DATASIZE	= THIRTYTWO_BIT;	// 32-bit word

	//Use timer0 to start the x-fer.
	//Since this is a static copy use one shot mode, so only one trigger is needed
	//Also using 32-bit mode to decrease x-fer time
}

// This function initializes the DMA to a known state.
//
void DMAInitialize(void)
{
	EALLOW;

//	   Uint16 HARDRESET:1;       // 0      Hard Reset Bit (R=0/W=1)
//	   Uint16 PRIORITYRESET:1;   // 1      Priority Reset Bit (R=0/W=1)
//	   Uint16 rsvd1:14;          // 15:2   (R=0:0)
	DmaRegs.DMACTRL.bit.HARDRESET = 1;	// Perform a hard reset on DMA

//	   Uint16 FREE:1;            // 15     Debug Mode Bit (R/W):
//	                             //        0     halt after current read-write operation
//	                             //        1     continue running
	DmaRegs.DEBUGCTRL.bit.FREE = 1;		// Allow DMA to run free on emulation suspend
	EDIS;
}

void DMACHxConfig(struct DMACH_SETS * dmaSets, volatile struct CH_REGS *ch)
{
	DMACHxBurstConfig(&dmaSets->BurstSets, ch);
	DMACHxTransferConfig(&dmaSets->TransferSets, ch);
	DMACHxWrapConfig(&dmaSets->WrapSets, ch);
	DMACHxModeConfig(&dmaSets->ModeSets, ch);
}
// This function starts DMA Channel 1.
void StartDMACHx(volatile struct CH_REGS *ch)
{
	EALLOW;
	ch->CONTROL.bit.RUN = 1;
	EDIS;
}

void DMACHxAddrConfig(volatile Uint16 *DMA_Dest,volatile Uint16 *DMA_Source, volatile struct CH_REGS *ch)
{
	EALLOW;
	// Set up SOURCE address:

	ch->SRC_BEG_ADDR_SHADOW = (Uint32)DMA_Source;	// Point to beginning of source buffer
	ch->SRC_ADDR_SHADOW =     (Uint32)DMA_Source;

	// Set up DESTINATION address:
	ch->DST_BEG_ADDR_SHADOW = (Uint32)DMA_Dest;	    // Point to beginning of destination buffer
	ch->DST_ADDR_SHADOW =     (Uint32)DMA_Dest;

	EDIS;
}


void DMACHxBurstConfig(struct DMACH_BURST_SETS * burstSets, volatile struct CH_REGS *ch)
{
	EALLOW;

	// Set up BURST registers:
	ch->BURST_SIZE.all = burstSets->bsize;	                // Number of words(X-1) x-ferred in a burst
	ch->SRC_BURST_STEP = burstSets->srcbstep;			    // Increment source addr between each word x-ferred
	ch->DST_BURST_STEP = burstSets->desbstep;              // Increment dest addr between each word x-ferred

	EDIS;
}

void DMACHxTransferConfig(struct DMACH_TRANSFER_SETS * transferSets, volatile struct CH_REGS *ch)
{
	EALLOW;

	// Set up TRANSFER registers:
	ch->TRANSFER_SIZE 	  = transferSets->tsize;            // Number of bursts per transfer, DMA interrupt will occur after completed transfer
	ch->SRC_TRANSFER_STEP = transferSets->srctstep;			// TRANSFER_STEP is ignored when WRAP occurs
	ch->DST_TRANSFER_STEP = transferSets->deststep;			// TRANSFER_STEP is ignored when WRAP occurs

	EDIS;
}

void DMACHxWrapConfig(struct DMACH_WRAP_SETS * wrapSets, volatile struct CH_REGS *ch)
{
	EALLOW;

		// Set up WRAP registers:
	ch->SRC_WRAP_SIZE = wrapSets->srcwsize;				// Wrap source address after N bursts
    ch->SRC_WRAP_STEP = wrapSets->srcwstep;			    // Step for source wrap

	ch->DST_WRAP_SIZE = wrapSets->deswsize;				// Wrap destination address after N bursts
	ch->DST_WRAP_STEP = wrapSets->deswstep;				// Step for destination wrap

	EDIS;
}

void DMACHxModeConfig(struct MODE_BITS * modeSets, volatile struct CH_REGS *ch)
{
	EALLOW;

	// Set up MODE Register:
//	ch->MODE.bit.PERINTSEL 	= modeSets->PERINTSEL;	    // Passed DMA channel as peripheral interrupt source
//	ch->MODE.bit.PERINTE 	= modeSets->PERINTE;       	// Peripheral interrupt enable
//	ch->MODE.bit.ONESHOT 	= modeSets->ONESHOT;       	// Oneshot enable
//	ch->MODE.bit.CONTINUOUS = modeSets->CONTINUOUS;    	// Continous enable
//	ch->MODE.bit.SYNCE 		= modeSets->SYNCE;         	// Peripheral sync enable/disable
//	ch->MODE.bit.SYNCSEL 	= modeSets->SYNCSEL;       	// Sync effects source or destination
//	ch->MODE.bit.OVRINTE 	= modeSets->OVRINTE;        // Enable/disable the overflow interrupt
//	ch->MODE.bit.DATASIZE 	= modeSets->DATASIZE;      	// 16-bit/32-bit data size transfers
//	ch->MODE.bit.CHINTMODE 	= modeSets->CHINTMODE;		// Generate interrupt to CPU at beginning/end of transfer
//	ch->MODE.bit.CHINTE 	= modeSets->CHINTE;        	// Channel Interrupt to CPU enable
	ch->MODE.bit	=  *modeSets;
	// Clear any spurious flags:
	ch->CONTROL.bit.PERINTCLR = 1;  		// Clear any spurious interrupt flags
	ch->CONTROL.bit.SYNCCLR = 1;    		// Clear any spurious sync flags
	ch->CONTROL.bit.ERRCLR = 1; 	     	// Clear any spurious sync error flags

//	// Initialize PIE vector for CPU interrupt:
//	PieCtrlRegs.PIEIER7.bit.INTx1 = 1;              // Enable DMA CHx interrupt in PIE

	EDIS;
}




//===========================================================================
// No more.
//===========================================================================
