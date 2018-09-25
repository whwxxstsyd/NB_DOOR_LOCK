#ifndef _SHELL_H_
#define _SHELL_H_		


typedef struct
{
	u8 *p_cmd;
	void (*func)(void);
}SHELL_CMD_FUNC;		


void shell_thread(void);	


#endif

