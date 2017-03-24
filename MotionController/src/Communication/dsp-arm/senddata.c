#include "senddata.h"
#include <string.h>

#define SEND_CH(d)  ch = d; cb_append(&Spia.cb_tx, &ch)

// 弃用占用大片内存方式
ERROR_CODE senddata(word cmd, word mark, word *dat_buf, word dat_len) {
	word ch, checksum = 0;

	if (dat_len <= 0 || dat_len > COMMUNICATION_MAX_LEN)
		return RTN_INVALID_COMMAND;

	SEND_CH(0x23);
	// cmd[0]
	SEND_CH(dat_len+2);
	// cmd[1]

	// 开始计算校验位
	SEND_CH(cmd);
	// cmd[2]
	checksum += ch;			// checksum

	SEND_CH(mark);
	// cmd[3]
	checksum += ch;			// checksum

	while (dat_len--) {
		SEND_CH(*dat_buf++);
		// dat
		checksum += ch;			// checksum
	}

	SEND_CH(checksum);
	// cmd[n], checksum
	return RTN_SUCC;
}
