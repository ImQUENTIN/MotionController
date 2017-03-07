#include "senddata.h"

SENDMODE databuf;     //ֱ�ӽ�databuf����spififo�����ж�

ERROR_CODE senddata(int *dat_buf, short dat_len)
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

		}
		memcpy(&Spia.cb_tx,&databuf,(dat_len+2));
		return RTN_SUCC;
	}
	else{
		return RTN_INVALID_COMMAND;
	}

}
