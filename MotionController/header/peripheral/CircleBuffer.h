#ifndef _CIRCLE_BUFFER_H_
#define _CIRCLE_BUFFER_H_

typedef struct
{
	int head;
	int tail;
	int block_size;
	int block_number;
	void* dat;
}CIRCLE_BUFFER_S;


int cb_create(CIRCLE_BUFFER_S *buf, int block_size, int block_number);
int cb_release(CIRCLE_BUFFER_S *buf);
int cb_append(CIRCLE_BUFFER_S *buf, void* block_dat);
int cb_get(CIRCLE_BUFFER_S *buf, void* block_dat);
int cb_usedSpace(CIRCLE_BUFFER_S *buf);

#endif //_CIRCLE_BUFFER_H_
