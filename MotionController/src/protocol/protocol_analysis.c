#include "protocol_analysis.h" ///< doxygen

#define HEAD_0   0x55
#define HEAD_1   0xaa

static byte  cmd_buf[COMMUNICATION_MAX_LEN];
static short cmd_ptr = 0;
static byte  protocol_len = 0;

static byte  dat_buf[COMMUNICATION_MAX_LEN];
static short dat_ptr = 0;


static byte check_sum(byte* pdat, short len)
{
	int i;
	byte sum = 0;
	for (i = 0; i < len; i++)
		sum += pdat[i];

	return sum;
}

/**
 * \brief 		分析协议函数
 * \param[in]
 */
short analysis(byte ch)
{
	if (cmd_ptr <= 0) {
		if (ch == HEAD_0) {
			cmd_buf[cmd_ptr++] = ch;
			return RTN_ERROR;
		}
	}else if (cmd_ptr == 1) {
		if (ch == HEAD_1) {
			cmd_buf[cmd_ptr++] = ch;
		}else{
			cmd_ptr = 0;
		}
		return RTN_ERROR;
	}else if (cmd_ptr == 2) {
		protocol_len = ch;
		cmd_buf[cmd_ptr] = ch;	cmd_ptr++;
		return RTN_ERROR;
	}else{
		// 防止越界
		if (cmd_ptr >= COMMUNICATION_MAX_LEN) {
			cmd_ptr = 0;
			return RTN_ERROR;
		}

		//
		cmd_buf[cmd_ptr++] = ch;
		if (cmd_ptr == protocol_len + 4) {
			if (ch == check_sum(cmd_buf, cmd_ptr -1)) {
				dat_ptr = cmd_ptr - 4;
				memcpy(dat_buf, &cmd_buf[3], dat_ptr);
				cmd_ptr = 0;
				return RTN_OK;
			}
			cmd_ptr = 0;
		}
	}
	return RTN_ERROR;
}

short get_cmd_buf(byte* pdat, short* dat_len, short max_len)
{
	if (dat_ptr == 0) return RTN_ERROR;

	*dat_len = dat_ptr;
	memcpy(pdat, dat_buf, dat_ptr);
	dat_ptr = 0;
	return RTN_OK;
}
