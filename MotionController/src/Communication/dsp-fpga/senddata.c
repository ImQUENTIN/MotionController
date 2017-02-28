#include "senddata.h"

SENDMODE databuf;     //直接将databuf放入spififo发送中断

ERROR_CODE senddata(uint8_t *dat_buf, short dat_len)
{

	databuf.head = 0x23;

	if(dat_len > 0 || dat_len <= COMMUNICATION_MAX_LEN)
	{
		int i;
		databuf.len = dat_len;
		memcpy(databuf.data,dat_buf,dat_len);
		for(i = 0; i < dat_len; i++)
		{
			databuf.sum = 0;
			databuf.sum += databuf.data[i];
			return RTN_SUCC;
		}
	}
	else{
		return RTN_INVALID_COMMAND;
	}

	return RTN_SUCC;
}
