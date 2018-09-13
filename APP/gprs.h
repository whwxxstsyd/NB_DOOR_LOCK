#ifndef _GPRS_H_
#define _GPRS_H_

/* 定义下面这句话, 将把收到的字符发送到调试串口1 */
#define BC95_TO_DEBUG_EN

#define BC95_TMR_ID				(TMR_COUNT - 1)
#define BC95_CHK_TMR_ID		(TMR_COUNT - 2)	


#define AT_CR		'\r'
#define AT_LF		'\n'


s16 bc95_modem_init(void);

s16 bc95_state_check(void);



#endif

