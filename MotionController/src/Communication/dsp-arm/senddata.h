#ifndef _SENDDATA_H_
#define _SENDDATA_H_

#include"sysTypes.h"
#include"my_project.h"

ERROR_CODE senddata(int *dat_buf, short dat_len);

typedef struct {
	char head;
	short len;
	int* data;
	char sum;

}SENDMODE;

#endif
