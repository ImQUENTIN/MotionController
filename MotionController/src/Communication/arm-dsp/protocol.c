
#include "procotol.h"

static word cmd_buf[COMMUNICATION_MAX_LEN];
static short cmd_ptr = 0;
static short protocol_len = 0;

static word check_sum(word* pdat, short len)
{
	int i;
	word sum = 0;
	for (i = 0; i < len; i++)
		sum += pdat[i];
 	return sum;
}


ERROR_CODE protocol(word chc, word *dat_buf, short *dat_len)
{
	if (cmd_ptr <= 0) {
		if (chc == MSG_HEAD) {
			cmd_buf[cmd_ptr++] = chc;
			return RTN_ERROR;
		}

	}else if (cmd_ptr == 1) {
		protocol_len = chc;
		cmd_buf[cmd_ptr++] = chc;
		return RTN_ERROR;
	}else{
		// ·ÀÖ¹Ô½½ç
		if (cmd_ptr >= COMMUNICATION_MAX_LEN) {
			cmd_ptr = 0;
			return RTN_ERROR;
		}
		//

		cmd_buf[cmd_ptr++] = chc;
		if (cmd_ptr == protocol_len + 3) {
			if (chc == check_sum(cmd_buf+2, protocol_len)) {
				memcpy(dat_buf, &cmd_buf[2], protocol_len);
				cmd_ptr = 0;
				*dat_len = protocol_len;
				return RTN_SUCC;
			}
			cmd_ptr = 0;
		}
	}
	return RTN_ERROR;
}


