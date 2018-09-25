      
/**@file 
 * @note tiansixinxi. All Right Reserved.
 * @brief  
 * 
 * @author   madongfang
 * @date     2016-7-22
 * @version  V1.0.0
 * 
 * @note ///Description here 
 * @note History:        
 * @note     <author>   <time>    <version >   <desc>
 * @note  
 * @warning  
 */


#include "bsp.h"
#include "shell.h"


#define MAX_CMD_LEN 64	
#define SHELL_PROMPT "~# "	


static u8 shell_line[MAX_CMD_LEN] = {0};
static u8 line_position = 0;	


static void list_all_cmd(void);
static void at(void);		


static SHELL_CMD_FUNC cmd_fun[] = 
{	
	{"list", list_all_cmd},
	{"AT", at},		
	{NULL, NULL}
};

	

static void list_all_cmd(void)
{	
	u8 i = 0;

	printf("\nsupport commond:\n");
		
	for (i = 0; cmd_fun[i].p_cmd != NULL; i++)
	{		
		printf(cmd_fun[i].p_cmd);
		printf("\n");	
	}
	
	return;
}



static void at(void)
{		
	u8 len = 0;
	u8 cmd[4] = {0};
	u8 param[64] = {0};	
	u16 i = 0;			
			
	sscanf(shell_line, "%s %s", cmd, param);
			
	comSendChar(NB_COM,(u8)'A');	
	comSendChar(NB_COM,(u8)'T');	
			
	for (i = 0; param[i] != '\0'; i++)
	{	
		comSendChar(NB_COM,(u8)param[i]);
	}		
		
	comSendChar(NB_COM,(u8)'\r');								
	

}





void shell_cmd_run(void)
{	
	u8 i = 0;
	u8 cmd[MAX_CMD_LEN] = {0};
	
	memset(cmd, 0, sizeof(cmd));
	if (sscanf(shell_line, "%s", cmd) != 1)
	{	
		printf("shell commond error\r");
	}	
	
	for (i = 0; cmd_fun[i].p_cmd != NULL; i++)
	{
		if (strcmp(cmd, cmd_fun[i].p_cmd) == 0)
		{	
			cmd_fun[i].func();
			return;			
		}	
	}	
	
	printf("unknown commond:cmd=%s,len=%d\n", cmd, strlen(cmd));
	
	list_all_cmd();	
	
	return;
}



/**		  
 * @brief	 自定义shell功能，该函数永远不返回
 * @param 无
 * @return 无	  
 */	
void shell_thread(void)	
{	
	u8 ch = 0;		
					
	while(comGetChar(DEBUG_COM,(u8 *)&ch) != 0)	
	{	
		if (ch == '\n')
		{	
			shell_line[line_position] = 0;
			printf("\n");
			
			if (line_position > 0)
			{
				shell_cmd_run();
				memset(shell_line, 0, sizeof(shell_line));
				line_position = 0;
				printf("\n");
			}
			
			printf(SHELL_PROMPT);
				
			continue;
		}	
	
		if (line_position < MAX_CMD_LEN - 1)
		{	
			shell_line[line_position] = ch;
			line_position++;				
			printf("%c", ch);		
		}
	}
}
	

