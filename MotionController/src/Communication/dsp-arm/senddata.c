#include "senddata.h"



ERROR_CODE senddata(word cmd,word mark,word *dat_buf, word dat_len)
{
	int str[COMMUNICATION_MAX_LEN];
	str[0] = 0x23;
	str[2] = cmd;
	str[3] = mark;

	if(dat_len > 0 || dat_len <= COMMUNICATION_MAX_LEN)
	{
		int i, j;
		str[1] = dat_len;
		str[dat_len + 4] = 0;
		memcpy(str+4, &dat_buf, dat_len);
		for(i = 0; i < dat_len; i++)
		{
			str[dat_len + 4] += str[4+i];
		}
		for(j = 0; j < (dat_len+5); j++){
		cb_append(&Spia.cb_tx, &str[j]);
		}
		return RTN_SUCC;
	}
	else{
		return RTN_INVALID_COMMAND;
	}

}
