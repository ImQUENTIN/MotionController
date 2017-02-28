#ifndef _SENDDATA_H_
#define _SENDDATA_H_

#include"sysTypes.h"

ERROR_CODE senddata(uint8_t *dat_buf, short dat_len);

typedef struct {
	char head;
	char len;
	char* data;
	char sum;

}SENDMODE;

#endif
