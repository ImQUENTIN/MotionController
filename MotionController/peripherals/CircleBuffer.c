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


int cb_create(CIRCLE_BUFFER_S *buf, int block_size, int block_number)
{
	//cb_release(buf);
	buf->dat =(int*) malloc(block_size * block_number);
	memset(buf, 0, sizeof(CIRCLE_BUFFER_S));
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
	int tail;
	tail = (buf->tail + 1) % buf->block_number;
	if (tail == buf->head) return RTN_ERROR;
	memcpy((int *)(buf->dat) + buf->tail * buf->block_size, block_dat, buf->block_size);
	buf->tail = tail;
	return buf->tail;
}
//buf->block_size * buf->tail
int cb_get(CIRCLE_BUFFER_S *buf, void* block_dat)
{
//	int head;
	if (buf->head == buf->tail ) return RTN_ERROR;
//	buf->busy = 1;
//	head = (buf->head + 1) % buf->block_number;
	memcpy(block_dat, (int *)buf->dat + buf->head * buf->block_size, buf->block_size);
	buf->head = (buf->head + 1) % buf->block_number;
//	buf->busy = 0;
	return buf->head;
}


// available
int cb_usedSpace(CIRCLE_BUFFER_S *buf)
{
	int tail = buf->tail;
	if (tail < buf->head) tail += buf->block_number;
	return (tail - buf->head);
}



