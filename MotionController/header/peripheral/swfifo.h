/*
*  Created on: 2016-12-6
*      Author: QUENTIN
*      E-mail: qiyuexin@yeah.net
*/
//###########################################################################
//
// FILE:	mySwfifo.h
//
// TITLE:	My Software FIFO
//
//###########################################################################


#ifndef SWFIFO_H_
#define SWFIFO_H_


// FIFO 的最大深度可以支持到256，目前觉得够用了。
struct SWFIFO {
	 char 	*Buffer;		// Software FIFO Buffer
	uint16_t	Sta:8;			// fifo Status, 即目前FIFO数据的个数。
	uint16_t	WrP:8;			// write point.
	uint16_t	RdP:8;			// Read Point.
	uint16_t	Deep:8;			// FIFO DEEP, get from user.
};

extern void swfifoReset( struct SWFIFO *swfifo );
extern char swfifo_In( char dat, struct SWFIFO *swFifo);
extern char swfifo_Out( char *dat, struct SWFIFO *swFifo);
extern char swfifo_IsEmpty(struct SWFIFO *swFifo);	// 1:empty; 0:not.
extern char swfifo_IsFull(struct SWFIFO *swFifo);		// 1: full; 0:not.

#endif /* MYSWFIFO_H_ */
