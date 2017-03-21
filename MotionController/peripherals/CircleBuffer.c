/*
 * CircleBuffer.c
 *
 *  Created on: 2017-2-16
 *      Author: Administrator
 */
#include "sysTypes.h"
#include "CircleBuffer.h"
#include "stdlib.h"
#include "string.h"
#include "DSP2833x_Device.h"

int cb_create(CIRCLE_BUFFER_S *buf, int block_size, int block_number)
{
	//cb_release(buf);
	memset(buf, 0, sizeof(CIRCLE_BUFFER_S));
	buf->dat =(int*) malloc(block_size * block_number);
	buf->block_number = block_number;
	buf->block_size = block_size;
	return buf->block_number;
}

int cb_release(CIRCLE_BUFFER_S *buf)
{
	if (buf->dat == 0) return 0;
	memset(buf, 0, sizeof(CIRCLE_BUFFER_S));
	free(buf->dat);
	return 0;
}

int cb_append(CIRCLE_BUFFER_S *buf, void* block_dat)
{
	int tail = (buf->tail + 1) % buf->block_number;
	if (tail == buf->head) return RTN_ERROR;
	memcpy((Uint16 *)(buf->dat) + tail * buf->block_size, (Uint16 *)block_dat, buf->block_size);
	buf->tail = tail;
	return buf->tail;
}

int cb_get(CIRCLE_BUFFER_S *buf, void* block_dat)
{
	int head = (buf->head + 1) % buf->block_number;
	if (buf->head == buf->tail ) return RTN_ERROR;
	memcpy((Uint16 *)block_dat, (Uint16 *)(buf->dat) + head * buf->block_size, buf->block_size);
	buf->head = head;
	return buf->head;
}

// available
int cb_usedSpace(CIRCLE_BUFFER_S *buf)
{
	int tail = buf->tail;
	if (tail < buf->head) tail += buf->block_number;
	return (tail - buf->head);
}



