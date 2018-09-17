#ifndef _SHELL_H_
#define _SHELL_H_		

#define MAX_CMD_LEN 64	
#define SHELL_PROMPT "~# "	









	
typedef struct
{
	u8 *p_cmd;
	void (*func)(void);
}SHELL_CMD_FUNC;		


void shell_thread(void);	


#endif

