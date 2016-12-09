/*
*  Created on: 2016-12-6
*      Author: QUENTIN
*      E-mail: qiyuexin@yeah.net
*/
//###########################################################################
//
// FILE:	mySwfifo.c
//
// TITLE:	My Software FIFO Initialization & Support Functions.
//
//###########################################################################

#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File

//----------------------------------------------------------
//            swFifo
//----------------------------------------------------------

void swfifoReset( struct SWFIFO *swfifo )
{
	//swfifo->Buffer  = 0;
	swfifo->RdP = 0;
	swfifo->WrP = 0;
	swfifo->Sta = 0;
}

// return val: 0, not full; 1: full.
uint8_t swfifo_In( int8_t dat, struct SWFIFO *swFifo)
{
	// 打开FIFO 中断
	if( swFifo->Sta >= swFifo->Deep) {
		return 1;	// no space room.
	}

	// 否则，eFIFO还有存储空间可以存入,存进去。
	swFifo->Buffer[swFifo->WrP++] = dat;

	if(swFifo->WrP >= swFifo->Deep)
		swFifo->WrP = 0;	// 循环指针

	swFifo->Sta++;		// add a new data.
	if( swFifo->Sta >= swFifo->Deep)
		return 1;	// no space room.
	else
		return 0;	// not full.
}

// return val: 0, not empty; 1: empty.
uint8_t swfifo_Out( int8_t *dat, struct SWFIFO *swFifo)
{
	if( 0 == swFifo->Sta) {
		return 1;	// no more new data.
	}

	// SCI  FIFO is not full.
	*dat = swFifo->Buffer[swFifo->RdP++];
	if(swFifo->RdP >= swFifo->Deep)
		swFifo->RdP = 0;	// 循环指针

	swFifo->Sta --;	// delete a data.
	return 0;	// still have data.
}

uint8_t swfifo_IsEmpty(struct SWFIFO *swFifo)
{
	return (swFifo->Sta == 0) ? 1 : 0;
}

uint8_t swfifo_IsFull(struct SWFIFO *swFifo)
{
	return (swFifo->Sta >= swFifo->Deep) ? 1 : 0;
}

