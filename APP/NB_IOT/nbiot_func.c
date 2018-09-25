#include "nbiot_func.h"	
#include "basic_fun.h"
#include "include.h"	
#include "bsp.h"			


static void BC95_PrintRxData(uint8_t _ch)
{		
#ifdef BC95_TO_DEBUG_EN
		comSendChar(DEBUG_COM, _ch);		/* ½«½ÓÊÕµ½Êý¾Ý´òÓ¡µ½µ÷ÊÔ´®¿Ú1 */
#endif
}


static s16 bc95_send_at(const u8 *p_cmd)	
{		
	s16 i = 0;
	u16 p_cmd_len = strlen(p_cmd);
	
	if (NULL == p_cmd || p_cmd_len > UART2_TX_BUF_SIZE)	
	{	
		GPRS_DEBUG(DEBUG_ERROR, "param error\n");
		return NBIOT_ERROR;	
	}	
	
	uart_recv_finish_clr(NB_COM);	/* ÇåÁã´®¿Ú½ÓÊÕ»º³åÇø */	

	comSendChar(NB_COM,(u8)'A');
	comSendChar(NB_COM,(u8)'T');	
		
	comSendBuf(NB_COM, (uint8_t *)p_cmd, p_cmd_len);	
			
	comSendChar(NB_COM,(u8)'\r');		
	
	return NBIOT_OK;	
}


void bc95_time_delay(uint16_t _usTimeOut,u16 count)
{
	if (_usTimeOut > 0)
	{	
		bsp_StartTimer(BC95_TMR_ID, _usTimeOut);		/* Ê¹ÓÃÈí¼þ¶¨Ê±Æ÷×÷Îª³¬Ê±¿ØÖÆ */
	}	

	while (1)
	{
		bsp_Idle(); 			/* CPU¿ÕÏÐÖ´ÐÐµÄ²Ù×÷£¬ ¼û bsp.c ºÍ bsp.h ÎÄ¼þ */
					
		if (_usTimeOut > 0)
		{	
			if (bsp_CheckTimer(BC95_TMR_ID))
			{			
				break;
			}
		}					
	}

}

static int8_t bc95_read_onebyte(uint8_t *byte, uint16_t _usTimeOut)
{
	NBIOT_StatusTypeDef NBStatus = NBIOT_OK;
	uint8_t ucData;
	uint16_t pos = 0;	
	uint8_t status = 0; 	/* ½ÓÊÕ×´Ì¬ */

	/* _usTimeOut == 0 ±íÊ¾ÎÞÏÞµÈ´ý */
	if (_usTimeOut > 0)
	{	
		bsp_StartTimer(BC95_TMR_ID, _usTimeOut);		/* Ê¹ÓÃÈí¼þ¶¨Ê±Æ÷×÷Îª³¬Ê±¿ØÖÆ */
	}	
	
	while (1)
	{
		bsp_Idle(); 			/* CPU¿ÕÏÐÖ´ÐÐµÄ²Ù×÷£¬ ¼û bsp.c ºÍ bsp.h ÎÄ¼þ */
		
		if (_usTimeOut > 0)
		{	
			if (bsp_CheckTimer(BC95_TMR_ID))
			{
				NBStatus = NBIOT_CMD_TIMEOUT;
				break;	
			}
		}				
			
		if (comGetChar(NB_COM, byte))	
		{	
			//BC95_PrintRxData(ucData);		/* ½«½ÓÊÕµ½Êý¾Ý´òÓ¡µ½µ÷ÊÔ´®¿Ú1 */
				
			//bsp_StartTimer(BC95_TMR_ID, _usTimeOut);			

			break;
		}	
	}
	
	return NBStatus;
}


static s16 bc95_recv_ret_line(char *p_buff, uint16_t len, uint16_t timeout)
{	
	s16 i = 0;
	u8 ch[2] = {0};
	s16 ret = 0;	

	if (NULL == p_buff)
	{
		GPRS_DEBUG(DEBUG_ERROR, "param error\n");
		return -1;
	}	

	if ((ret = bc95_read_onebyte((u8 *)&ch[0], timeout)) != 0)
	{			
		GPRS_DEBUG(DEBUG_ERROR, "gprs_serial_read_onebyte error\n");
		return ret;
	}
	
	/* ½ÓÊÕÆðÊ¼×Ö·û */
	while(1)
	{			
		if ((ret = bc95_read_onebyte((u8 *)&ch[1], timeout)) != 0)
		{
			GPRS_DEBUG(DEBUG_ERROR, "gprs_serial_read_onebyte error\n");
			return ret;
		}	

		if ('\r' == ch[0] && '\n' == ch[1])
		{
			break; // ÕÒµ½ÆðÊ¼×Ö·û
		}
		else
		{
			ch[0] = ch[1];
		}
	}
	
	/* ½ÓÊÕÊµ¼ÊÊý¾Ý */
	i = 0;
	while (i < len - 1)
	{		
		if ((ret = bc95_read_onebyte((u8 *)&p_buff[i], timeout)) != 0)
		{	
			GPRS_DEBUG(DEBUG_ERROR, "gprs_serial_read_onebyte error\n");
			return ret;
		}		
		
		if ('\r' == p_buff[i] || '>' == p_buff[i])
		{	
			break;
		}
		i++;
	}
	ch[0] = p_buff[i];
	if ('>' == p_buff[i])
	{
		i++;
	}	
	p_buff[i] = '\0';

	/* ½ÓÊÕ½áÊø×Ö·û */
	while(1)
	{	
		if ((ret = bc95_read_onebyte((u8 *)&ch[1], timeout)) != 0)
		{		
			GPRS_DEBUG(DEBUG_ERROR, "gprs_serial_read_onebyte error\n");
			return ret;
		}

		if ('\r' == ch[0] && '\n' == ch[1])
		{
			break; // ÕÒµ½½áÊø×Ö·û
		}
		else if ('>' == ch[0] && ' ' == ch[1])
		{	
			break;
		}
		else
		{
			ch[0] = ch[1];
		}
	}

	return i;
}

	

static u8* bc95_check_cmd(const u8 *raw_data,const u8 *right_data)
{	
	u8 *strx = 0;	
 		
	strx = strstr((const u8*)raw_data,(const u8*)right_data);	
	
	return (u8*)strx;
}



static s16 bc95_cmd_recv_back(u8 *recv_back,u16 len,u16 count)
{
	while(1)	
	{
		if(uart_recv_finish_get(COM2))
		{
			break;		
		}
	}	
	
	recv_back = g_tUart2.pRxBuf;		

	uart_recv_finish_clr(COM2);			
}



static s16 bc95_cmd_recv_back2(u8 *recv_back,u16 len,u16 count)
{
	s16 ret = 0;		
	
	while (count--)		
	{	
		if ((ret = bc95_recv_ret_line(recv_back, len, SYSTEM_TICKS_PER_SEC)) < 0)
		{	
			if (-2 == ret)
			{	
				GPRS_DEBUG(DEBUG_ERROR, "bc95_cmd_recv_back time_out\n");
				return NBIOT_ERROR;
			}	
			
			GPRS_DEBUG(DEBUG_INFO, "bc95_cmd_recv_back error\n");
				
			continue;
		}	
		else
		{			
			break;	
		}	
	}	

	if(count == 0)
	{		
		GPRS_DEBUG(DEBUG_ERROR, "bc95_cmd_recv_back count_out\n");	
		return NBIOT_ERROR;
	}
	else 
	{	
		return NBIOT_OK;		
	}
}
	



static s16 bc95_send_cmd(const u8 *cmd,const u8 *ack,u16 waittime,u16 count)//n*0ms
{	
	u8 str_raw[64]  = {0};
	s16 ret = 0;

	bc95_send_at(cmd);
	
	/*·¢ËÍAT*/
	while (count--)	
	{	
		if ((ret = bc95_recv_ret_line(str_raw, sizeof(str_raw), waittime)) < 0)
		{					
			if (-2 == ret)
			{		
				GPRS_DEBUG(DEBUG_ERROR, "bc95_recv_ret_line timeout\n");
				return -1;
			}

			GPRS_DEBUG(DEBUG_ERROR, "bc95_recv_ret_line error\n");

			continue;
		}		
		
		if (bc95_check_cmd(str_raw,ack))			
		{	
			break;
		}
		else
		{		
			GPRS_DEBUG(DEBUG_ERROR, "bc95_check_cmd error str=%s\n", str_raw);
		}	
	}

	if (0 == count)
	{	
		GPRS_DEBUG(DEBUG_ERROR, "bc95_send_cmd timeout\n");
		return -1;
	}
	else
	{	
		return 0;	
	}
} 


















static s16 neul_bc95_reboot(void)
{			
	if(bc95_send_cmd("+NRB","REBOOTING",SYSTEM_TICKS_PER_SEC,2))	
		return -1;	
	else			
    	return 0;	
}


static s16 neul_bc95_set_band(int band)//ÉèÖÃÆµ¶Î
{	
	u8 wbuf[64] = {0};
    u8 *p = " +NBAND=";	
	
    if (band == 8 || band == 5 || band == 20)
    {	
        sprintf((char*)wbuf, "%s%d ", p, band);
    }	
    else
    {	
        //error band for bc95
        return -1;
    }
			
	if( bc95_send_cmd(wbuf,"OK",SYSTEM_TICKS_PER_SEC,2) != 0)		
		return -1;
	
    return 0;
}


static s16 neul_bc95_reset_netstat(void)	
{
	if(bc95_send_cmd("+CGATT=1","OK",SYSTEM_TICKS_PER_SEC,2))
	{	
		GPRS_DEBUG(DEBUG_ERROR, "+CGATT=1 return failed\n");
		return -1;	
	}
}

static s16 neul_bc95_get_netstat(void)
{
	u8 recv_back[64] = {0};
	
	bc95_send_at("+CGATT?");

	bc95_cmd_recv_back(recv_back,sizeof(recv_back),2);
		
	GPRS_DEBUG(DEBUG_NOTICE, "+CGATT? 1st str=%s\n", recv_back);		

	if(bc95_check_cmd(recv_back,"+CGATT:1") == 0)
	{	
		GPRS_DEBUG(DEBUG_NOTICE, "+CGATT? 1st failed\n");		
		return -1;				
	}
			
	bc95_cmd_recv_back(recv_back,sizeof(recv_back),2);			
				
	GPRS_DEBUG(DEBUG_NOTICE, "+CGATT? 2nd str=%s\n", recv_back);

	if(bc95_check_cmd(recv_back,"OK") == 0)
	{		
		GPRS_DEBUG(DEBUG_NOTICE, "+CGATT? 2nd failed\n");	
		return -1;				
	}	

	
#if 0
	if(bc95_send_cmd("+CGATT?","+CGATT:1",SYSTEM_TICKS_PER_SEC,2))
	{				
		GPRS_DEBUG(DEBUG_ERROR, "+CGATT:1 return failed\n");
		//neul_bc95_reboot();			
		return -1;	
	}
#endif 
		
	return 0;    
}		


static s16 neul_bc95_get_csq(void)
{	
	u8 recv_back[64] = {0};
	char *cmd = "+CSQ";		
	int value[2] = 0;			
	
	bc95_send_at(cmd);			
	
	bc95_cmd_recv_back(recv_back,sizeof(recv_back),2);

	GPRS_DEBUG(DEBUG_NOTICE, "+CSQ 1st str=%s\n", recv_back);	

	sscanf((u8*)recv_back, "\r+CSQ:%d,%d",&value[0],&value[1]);	
			
    if(value[0] >= 31)
	{	
		bc95_send_cmd("+CGATT=0","OK",SYSTEM_TICKS_PER_SEC,2);		
		//neul_bc95_get_netstat();			
		
		return -1;
    }
	
	bc95_cmd_recv_back(recv_back,sizeof(recv_back),2);	
			
	GPRS_DEBUG(DEBUG_NOTICE, "+CSQ 2nd str=%s\n", recv_back);

	if(bc95_check_cmd(recv_back,"OK") == 0)	
	{	
		return -1;				
	}	
	
	return 0;
}

	
static s16 neul_bc95_get_cereg(void)
{
	u8 *cmd = "+CEREG?";
	int ret = 0;
	int buf=0;	
	u8 recv_back[64] = {0};
	
#if 1	
		
	bc95_send_at(cmd);		

	bc95_cmd_recv_back(recv_back,sizeof(recv_back),2);
	
	GPRS_DEBUG(DEBUG_NOTICE, "+CEREG? 1st str=%s\n", recv_back);	

	if(bc95_check_cmd(recv_back,"+CEREG:0,1") == 0)
	{		
		bc95_send_cmd("+CGATT=0","OK",SYSTEM_TICKS_PER_SEC,2);	
		//neul_bc95_get_netstat();		
		
		return -1;			
	}	
	
	bc95_cmd_recv_back(recv_back,sizeof(recv_back),2);
			
	GPRS_DEBUG(DEBUG_NOTICE, "+CEREG? 2nd str=%s\n", recv_back);	
	
	if(bc95_check_cmd(recv_back,"OK") == 0)							
	{					
		return -1;			
	}	

#endif			
		
	//if(bc95_send_cmd("+CEREG?","OK",SYSTEM_TICKS_PER_SEC*2,5) != 0)		 
	//	return -1;	
	
#if 0		
	sscanf((char*)USART2_RX_BUF, "\r+CEREG:%d,%d\r",&ret,&buf);
	if(!(ret == 0 && buf == 1))
	{		
		bc95_send_cmd("AT+CGATT=0\r","OK",SYSTEM_TICKS_PER_SEC*2,1);	
		neul_bc95_get_netstat();
	}
	return buf;
#endif	

	return 0;	
}

		
static s16 neul_bc95_set_cdpserver(const char *ipaddr,const char *port)
{	
	char *cmd = "+NCDP=";	
	char *cmd2 = "+NCDP?";	
	u8 wbuf[64] = {0};
	u8 recv_back[64] = {0};		
			
	sprintf((char*)wbuf,"%s%s,%s", cmd, ipaddr,port);
				
	if(bc95_send_cmd(wbuf,"OK",SYSTEM_TICKS_PER_SEC,2))
	{			
		GPRS_DEBUG(DEBUG_ERROR, "++NCDP=ip,port return failed\n");
		return -1;	
	}			

	bc95_send_at(cmd2);
	
	bc95_cmd_recv_back(recv_back,sizeof(recv_back),2);
					
	GPRS_DEBUG(DEBUG_NOTICE, "+NCDP? 1st str=%s\n", recv_back);		

	bc95_cmd_recv_back(recv_back,sizeof(recv_back),2);
				
	GPRS_DEBUG(DEBUG_NOTICE, "+NCDP? 2nd str=%s\n", recv_back);				
	
	GPRS_DEBUG(DEBUG_NOTICE, "ÔÆÆ½Ì¨µØÖ·ÉèÖÃÍê³É:%s,%s\r\n",ipaddr,port);					
	
	return 0;					
}


static s16 neul_bc95_get_time(void)
{
	u8	year,mon,date,hour,min,sec;
	char *cmd = "+CCLK?";		
	u8 recv_back[64] = {0};	

	bc95_send_at(cmd);			
	
	bc95_cmd_recv_back(recv_back,sizeof(recv_back),2);
	
	GPRS_DEBUG(DEBUG_NOTICE, "+CCLK? 1st str=%s\n", recv_back);	

	bc95_cmd_recv_back(recv_back,sizeof(recv_back),2);		
			
	GPRS_DEBUG(DEBUG_NOTICE, "+CCLK? 1st str=%s\n", recv_back);
		
	return 0;
}






#define RESET_COUNT 6	/*´óÔ¼Ò»·ÖÖÓ*/
	
static s16 err_cnt[4] = 0;		

static void bc95_err_cnt(s16 *err_cnt)
{
	(*err_cnt)++;
	printf("State Check Times:%d\n",(*err_cnt));
	
	if((*err_cnt) >= RESET_COUNT)
	{	
		neul_bc95_reboot();
		(*err_cnt) = 0;		
	}
}

s16 bc95_modem_init(void)
{	
	NBIOT_StatusTypeDef NBStatus = NBIOT_OK;

	/*·¢ËÍ"AT"*/		
	if(bc95_send_cmd("","OK",SYSTEM_TICKS_PER_SEC,6))			
	{			
		GPRS_DEBUG(DEBUG_ERROR, "AT CMD ERR\n");
		NBStatus = NBIOT_ERROR;	
		return NBStatus;	
	}		

	bsp_DelayMS(5);

	/*·¢ËÍ"+CMEE=1"*/	
	if(bc95_send_cmd("+CMEE=1","OK",SYSTEM_TICKS_PER_SEC,2))
	{	
		GPRS_DEBUG(DEBUG_ERROR, "+CMEE=1 CMD ERR\n");
		return -1;
	}		

	bsp_DelayMS(5);

	/*·¢ËÍ"+NBAND?"*/
	if(bc95_send_cmd("+NBAND?","+NBAND:5",SYSTEM_TICKS_PER_SEC,2))	
	{	
		neul_bc95_set_band(5);	
		if(bc95_send_cmd("+NBAND?","+NBAND:5",SYSTEM_TICKS_PER_SEC,2))	
		{	
			GPRS_DEBUG(DEBUG_ERROR, "+NBAND? CMD ERR\n");		
			
			bc95_err_cnt(&err_cnt[0]);

			return -1;		
		}		
	}			

	err_cnt[0] = 0;
	
	bsp_DelayMS(5);

	/*·¢ËÍ"+CGATT=1"*/	
	if(bc95_send_cmd("+CGATT=1","OK",SYSTEM_TICKS_PER_SEC,2))
	{		
		GPRS_DEBUG(DEBUG_ERROR, "+CGATT=1 CMD ERR\n");
		return -1;	
	}

	bsp_DelayMS(5);
	
	/*·¢ËÍ"+CGATT?"*/	
	if(neul_bc95_get_netstat())	
	{		
		GPRS_DEBUG(DEBUG_ERROR, "+CGATT? CMD ERR\n");	
		
		bc95_err_cnt(&err_cnt[1]);
		
		return -1;
	}

	err_cnt[1] = 0;

	bsp_DelayMS(5);	

	/*·¢ËÍ"+CSQ"*/							
	if(neul_bc95_get_csq())
	{		
		GPRS_DEBUG(DEBUG_ERROR, "+CSQ CMD ERR\n");	
		
		bc95_err_cnt(&err_cnt[2]);
		
		return -1;		
	}	

	err_cnt[2] = 0;	

	bsp_DelayMS(5);

	/*·¢ËÍ"+NSMI=1"*/
	if(bc95_send_cmd("+NSMI=1","OK",SYSTEM_TICKS_PER_SEC,2))	
	{						
		GPRS_DEBUG(DEBUG_ERROR, "+NSMI=1 CMD ERR\n");	
		return -1;		
	}

	bsp_DelayMS(5);

	/*·¢ËÍ"+NNMI=1"*/	
	if(bc95_send_cmd("+NNMI=1","OK",SYSTEM_TICKS_PER_SEC,2))	
	{		
		GPRS_DEBUG(DEBUG_ERROR, "+NNMI=1 CMD ERR\n");
		return -1;		
	}		

	bsp_DelayMS(5);

	/*·¢ËÍ"+CEREG?"*/
	if(neul_bc95_get_cereg())
	{	
		GPRS_DEBUG(DEBUG_ERROR, "+CEREG? CMD ERR\n");
		
		bc95_err_cnt(&err_cnt[3]);			
		
		return -1;		
	}	

	err_cnt[3] = 0;		

	bsp_DelayMS(5);	

	/*·¢ËÍ"+NCDP="*/				
	if(neul_bc95_set_cdpserver(CDP_IP_ADDRESS,CDP_IP_PORT))	
	{		
		GPRS_DEBUG(DEBUG_ERROR, "+NCDP= CMD ERR\n");
		return -1;				
	}	

	bsp_DelayMS(5);	

	/*·¢ËÍ"+CCLK?"*/		
	neul_bc95_get_time();								

	bsp_DelayMS(5);							

	printf("gprs init success\n");			

	/*¿ªÆôÒ»¸öBC95×´Ì¬¼ì²âµÄÈí¼þ¶¨Ê±Æ÷,ÖÜÆÚ10s*/
	bsp_StartAutoTimer(BC95_CHK_TMR_ID,5*SYSTEM_TICKS_PER_SEC);		
	
	return 0;

}



s16 bc95_state_check(void)
{
	static s16 ret = 1;
	static s16 err_cnt = 0;

	if (bsp_CheckTimer(BC95_CHK_TMR_ID))	
	{	
		ret = 1;
		
		/*·¢ËÍ"CGATT=1"*/
		if(neul_bc95_get_netstat())
		{		
			GPRS_DEBUG(DEBUG_ERROR, "+CGATT=1 STATE CHK ERR\n");
			neul_bc95_reset_netstat();	
			ret = 0;
		}			
		
#if 0	
		
		/*·¢ËÍ"+CEREG?"*/	
		if(neul_bc95_get_cereg())
		{		
			GPRS_DEBUG(DEBUG_ERROR, "+CEREG? STATE CHK ERR\n");		
			ret = 0;
		}	
		
#endif

		/*Èç¹ûÄ£¿éÍøÂç³öÏÖ´íÎó,³¬¹ý12´ÎÖØÆôÄ£¿é*/
		if(ret == 0)	
		{					
			err_cnt++;	
			printf("State Check Times:%d\n",err_cnt);
			
			if(err_cnt >= RESET_COUNT)
			{	
				neul_bc95_reboot();
				err_cnt = 0;	
			}
		}		
		else
		{		
			err_cnt = 0;
		}

	}	

	
	return ret;

}




































/**********************************************************************************************************
 @Function			static NBIOT_StatusTypeDef NBIOT_Neul_NBxx_DictateEvent_GetError(NBIOT_ClientsTypeDef* pClient)
 @Description			NBIOT_Neul_NBxx_DictateEvent_GetError	: äº‹ä»¶è¿è¡ŒæŽ§åˆ¶å™¨èŽ·å–é”™è¯¯ç (å†…éƒ¨ä½¿ç”¨)
 @Input				pClient							: NBIOTå®¢æˆ·ç«¯å®žä¾‹
 @Return				NBIOT_StatusTypeDef					: NBIOTå¤„ç†çŠ¶æ€
**********************************************************************************************************/
static s16 NBIOT_Neul_NBxx_DictateEvent_GetError(u8* recv_back)
{		
	NBIOT_StatusTypeDef NBStatus = NBIOT_OK;	
	
	int retErrorCode;
	
	if (sscanf((const char*)recv_back, "+CME ERROR: %d", &retErrorCode) > 0) {
		NBStatus = (NBIOT_StatusTypeDef)retErrorCode;
	}
	
	return NBStatus;
}










/**********************************************************************************************************
 @Function			NBIOT_StatusTypeDef NBIOT_Neul_NBxx_HardwareReboot(NBIOT_ClientsTypeDef* pClient, u32 rebootTimeoutMS)
 @Description			NBIOT_Neul_NBxx_HardwareReboot			: Ó²¼þÖØÆôNBÄ£¿é
 @Input				pClient								: NBIOT¿Í»§¶ËÊµÀý
					rebootTimeoutMS						: ÖØÆôµÈ´ý³¬Ê±Ê±¼ä
 @Return				NBIOT_StatusTypeDef						: NBIOT´¦Àí×´Ì¬
**********************************************************************************************************/
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_HardwareReboot(NBIOT_ClientsTypeDef* pClient, u32 rebootTimeoutMS)
{
	return NBIOT_OK;	
}


/**********************************************************************************************************
 @Function			NBIOT_StatusTypeDef NBIOT_Neul_NBxx_HardwarePoweroff(NBIOT_ClientsTypeDef* pClient)
 @Description			NBIOT_Neul_NBxx_HardwareReboot			: Ó²¼þ¶Ïµç
 @Input				pClient								: NBIOT¿Í»§¶ËÊµÀý
 @Return				NBIOT_StatusTypeDef						: NBIOT´¦Àí×´Ì¬
**********************************************************************************************************/
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_HardwarePoweroff(NBIOT_ClientsTypeDef* pClient)	
{
	return NBIOT_OK;
}



	

/**********************************************************************************************************
 @Function			NBIOT_StatusTypeDef NBIOT_Neul_NBxx_SetReportTerminationError(NBIOT_ClientsTypeDef* pClient, NBIOT_ReportErrorTypeDef enable)
 @Description			NBIOT_Neul_NBxx_SetReportTerminationError	: ÉèÖÃCMEE
 @Input				pClient								: NBIOT¿Í»§¶ËÊµÀý
					enable								: CMEDisable						= 0
														  CMEEnable						= 1
 @Return				NBIOT_StatusTypeDef				: NBIOT´¦Àí×´Ì¬
**********************************************************************************************************/
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_SetReportTerminationError(NBIOT_ClientsTypeDef* pClient, NBIOT_ReportErrorTypeDef enable)
{
	NBIOT_StatusTypeDef NBStatus = NBIOT_OK;
	u8 cmd[64] = {0};
			
	sprintf((char *)cmd, "+CMEE=%d", enable);		

	/*·¢ËÍ"+CMEE=1"*/	
	if(bc95_send_cmd(cmd,"OK",SYSTEM_TICKS_PER_SEC,2))
	{		
		GPRS_DEBUG(DEBUG_ERROR, "+CMEE=1 CMD ERR\n");
		NBStatus = 	NBIOT_ERROR;
	}	

	return NBStatus;			
}


/**********************************************************************************************************
 @Function			NBIOT_StatusTypeDef NBIOT_Neul_NBxx_SetSupportedBands(NBIOT_ClientsTypeDef* pClient, NBIOT_BandTypeDef bands)
 @Description			NBIOT_Neul_NBxx_SetSupportedBands			: ÉèÖÃBand
 @Input				pClient								: NBIOT¿Í»§¶ËÊµÀý
					bands								: ChinaTelecom						= 5
														  ChinaMobile						= 8
														  ChinaUnicom						= 20
 @Return				NBIOT_StatusTypeDef						: NBIOT´¦Àí×´Ì¬
**********************************************************************************************************/
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_SetSupportedBands(NBIOT_ClientsTypeDef* pClient, NBIOT_BandTypeDef bands)
{
	NBIOT_StatusTypeDef NBStatus = NBIOT_OK;
	u8 cmd[64] = {0};
	
	sprintf((char *)cmd, "+NBAND=%d", bands);
	
	/*·¢ËÍ"+NBAND=5"*/		
	if(bc95_send_cmd(cmd,"OK",SYSTEM_TICKS_PER_SEC,2))
	{	
		GPRS_DEBUG(DEBUG_ERROR, "+NBAND CMD ERR\n");
		NBStatus = 	NBIOT_ERROR;	
	}
	
	return NBStatus;
}



/**********************************************************************************************************
 @Function			NBIOT_StatusTypeDef NBIOT_Neul_NBxx_CheckReadSupportedBands(NBIOT_ClientsTypeDef* pClient)
 @Description			NBIOT_Neul_NBxx_CheckReadSupportedBands		: ²éÑ¯Band
 @Input				pClient								: NBIOT¿Í»§¶ËÊµÀý
 @Return				NBIOT_StatusTypeDef						: NBIOT´¦Àí×´Ì¬
**********************************************************************************************************/
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_CheckReadSupportedBands(NBIOT_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBIOT_OK;
	int bands = 0;
	u8 recv_back[64] = {0};

	bc95_send_at("+NBAND?");
	
	bc95_cmd_recv_back(recv_back,sizeof(recv_back),2);	
			
	GPRS_DEBUG(DEBUG_NOTICE, "+NBAND? 1st str=%s\n", recv_back);		

	if (sscanf((const char*)recv_back, "%*[^+NBAND]%*[^:]:%d", &bands) <= 0) 
	{	
		NBStatus = NBIOT_ERROR;
	}
	else 
	{		
		if ((bands == NBand_2100MHz) || (bands == NBand_1800MHz) || (bands == NBand_850MHz) || (bands == NBand_900MHz) || (bands == NBand_800MHz) || (bands == NBand_700MHz)) 
		{	
			/*¼ÇÂ¼µ±Ç°µÄbandºÅ*/	
			pClient->Parameter.band = (NBIOT_BandTypeDef)bands;
		}
		else 
		{	
			NBStatus = NBIOT_ERROR;
		}
	}

	return NBStatus;
}





/**********************************************************************************************************
 @Function			NBIOT_StatusTypeDef NBIOT_Neul_NBxx_QuerySendMessageCOAPPayload(NBIOT_ClientsTypeDef* pClient)
 @Description			NBIOT_Neul_NBxx_QuerySendMessageCOAPPayload	: COAP²éÑ¯ÏûÏ¢ÒÑËÍ´ïNB
 @Input				pClient								: NBIOT¿Í»§¶ËÊµÀý
 @Return				NBIOT_StatusTypeDef						: NBIOT´¦Àí×´Ì¬
**********************************************************************************************************/
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_QuerySendMessageCOAPPayload(NBIOT_ClientsTypeDef* pClient)
{		
	NBIOT_StatusTypeDef NBStatus = NBIOT_OK;
	u8 recv_back[64] = {0};
	int value[3] = 0;	

	bc95_send_at("+NQMGS");
	
	bc95_cmd_recv_back(recv_back,sizeof(recv_back),2);	
		
	GPRS_DEBUG(DEBUG_NOTICE, "+NQMGS 1st str=%s\n", recv_back);	
	
	if(sscanf((u8*)recv_back, "%*[^PENDING]%*[^=]=%d%*[^SENT]%*[^=]=%d%*[^ERROR]%*[^=]=%d",&value[0],&value[1],&value[2]) <= 0)	
	{	
 		GPRS_DEBUG(DEBUG_ERROR, "+NQMGS 1st return failed\n");	
		return NBIOT_ERROR;
	}
#ifdef NBIOT_PRINT_ERROR_CODE_TYPE
	else 
	{	
		NBStatus = NBIOT_Neul_NBxx_DictateEvent_GetError(recv_back);
		return NBStatus;		
	}
#endif

	bc95_cmd_recv_back(recv_back,sizeof(recv_back),2);	
			
	GPRS_DEBUG(DEBUG_NOTICE, "+NQMGS 2nd str=%s\n", recv_back);
			
	if(bc95_check_cmd(recv_back,"OK") == 0)	
	{		
		GPRS_DEBUG(DEBUG_ERROR, "+NQMGS 2nd return failed\n");	
		return NBIOT_ERROR;			
	}			
	
	return NBIOT_OK;	
}



/**********************************************************************************************************
 @Function			NBIOT_StatusTypeDef NBIOT_Neul_NBxx_SetAttachOrDetach(NBIOT_ClientsTypeDef* pClient, NBIOT_NetstateTypeDef attdet)
 @Description			NBIOT_Neul_NBxx_SetAttachOrDetach			: ÉèÖÃÖÕ¶ËÈëÍøÍËÍø
 @Input				pClient								: NBIOT¿Í»§¶ËÊµÀý
					attdet								: Detach / Attach
 @Return				NBIOT_StatusTypeDef						: NBIOT´¦Àí×´Ì¬
**********************************************************************************************************/
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_SetAttachOrDetach(NBIOT_ClientsTypeDef* pClient, NBIOT_NetstateTypeDef attdet)
{
	NBIOT_StatusTypeDef NBStatus = NBIOT_OK;
	u8 cmd[64] = {0};
	
	sprintf((char *)cmd, "+CGATT=%d", attdet);	
	
	/*·¢ËÍ"+CGATT"*/		
	if(bc95_send_cmd(cmd,"OK",SYSTEM_TICKS_PER_SEC,2))
	{	
		GPRS_DEBUG(DEBUG_ERROR, "+CGATT CMD ERR\n");
		NBStatus =	NBIOT_ERROR;
	}

	return NBStatus;
}


	
/**********************************************************************************************************
 @Function			NBIOT_StatusTypeDef NBIOT_Neul_NBxx_CheckReadAttachOrDetach(NBIOT_ClientsTypeDef* pClient)
 @Description			NBIOT_Neul_NBxx_CheckReadAttachOrDetach		: ²éÑ¯ÖÕ¶ËÈëÍøÍËÍø
 @Input				pClient									: NBIOT¿Í»§¶ËÊµÀý
 @Return			NBIOT_StatusTypeDef						: NBIOT´¦Àí×´Ì¬
**********************************************************************************************************/
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_CheckReadAttachOrDetach(NBIOT_ClientsTypeDef* pClient)	
{	
	NBIOT_StatusTypeDef NBStatus = NBIOT_OK;
	int netval = 0;
	u8 recv_back[64] = {0};
			
	bc95_send_at("+CGATT?");
	
	bc95_cmd_recv_back(recv_back,sizeof(recv_back),2);
	
	GPRS_DEBUG(DEBUG_NOTICE, "+CGATT? 1st str=%s\n", recv_back);		
		
	if(sscanf((const char*)recv_back, "%*[^+CGATT]%*[^:]:%d", &netval) <= 0)
	{	
		GPRS_DEBUG(DEBUG_NOTICE, "+CGATT? 1st failed\n");		
		NBStatus = NBIOT_ERROR;			
	}
	else 
	{
		if ((netval == Attach) || (netval == Detach)) 
		{	
			/*¼ÇÂ¼ÍøÂç×´Ì¬*/	
			pClient->Parameter.netstate = (NBIOT_NetstateTypeDef)netval;
		}	
		else 
		{
			NBStatus = NBIOT_ERROR;
		}
	}

	return NBStatus;
}



/**********************************************************************************************************
 @Function			NBIOT_StatusTypeDef NBIOT_Neul_NBxx_CheckReadRSSI(NBIOT_ClientsTypeDef* pClient)
 @Description			NBIOT_Neul_NBxx_CheckReadRSSI				: ¼ì³öRSSIºÅ
 @Input				pClient								: NBIOT¿Í»§¶ËÊµÀý
 @Return				NBIOT_StatusTypeDef						: NBIOT´¦Àí×´Ì¬
**********************************************************************************************************/
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_CheckReadRSSI(NBIOT_ClientsTypeDef* pClient)	
{
	NBIOT_StatusTypeDef NBStatus = NBIOT_OK;
	u8 recv_back[64] = {0};
	int value = 0;	
	
	bc95_send_at("+CSQ");		

	bc95_cmd_recv_back(recv_back,sizeof(recv_back),2);

	GPRS_DEBUG(DEBUG_NOTICE, "+CSQ 1st str=%s\n", recv_back);	

	if (sscanf((const char*)recv_back, "%*[^+CSQ]%*[^:]:%d,%*d", &value) <= 0) 
	{	
		GPRS_DEBUG(DEBUG_NOTICE, "+CSQ? 1st failed\n");	
		NBStatus = NBIOT_ERROR;
	}
	else
	{	
		if(value >= 31)	
		{	
			NBStatus = NBIOT_ERROR;
	    }
	}

	return NBStatus;
}




/**********************************************************************************************************
 @Function			NBIOT_StatusTypeDef NBIOT_Neul_NBxx_SetSentMessageIndications(NBIOT_ClientsTypeDef* pClient, NBIOT_OpenOrCloseFuncTypeDef state)
 @Description			NBIOT_Neul_NBxx_SetSentMessageIndications	: ÉèÖÃNSMI¹¦ÄÜÊÇ·ñ¿ªÆô
 @Input				pClient								: NBIOT¿Í»§¶ËÊµÀý
					state								: CloseFunc / OpenFunc
 @Return				NBIOT_StatusTypeDef						: NBIOT´¦Àí×´Ì¬
**********************************************************************************************************/
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_SetSentMessageIndications(NBIOT_ClientsTypeDef* pClient, NBIOT_OpenOrCloseFuncTypeDef state)
{
	NBIOT_StatusTypeDef NBStatus = NBIOT_OK;
	u8 cmd[64] = {0};
	
	sprintf((char *)cmd, "+NSMI=%d", state);	
		
	/*·¢ËÍ"+NSMI"*/		
	if(bc95_send_cmd(cmd,"OK",SYSTEM_TICKS_PER_SEC,2))
	{		
		GPRS_DEBUG(DEBUG_ERROR, "+NSMI CMD ERR\n");
		NBStatus =	NBIOT_ERROR;
	}

	return NBStatus;	
}


/**********************************************************************************************************
 @Function			NBIOT_StatusTypeDef NBIOT_Neul_NBxx_SetNewMessageIndications(NBIOT_ClientsTypeDef* pClient, NBIOT_OpenOrCloseFuncTypeDef state)
 @Description			NBIOT_Neul_NBxx_SetNewMessageIndications	: ÉèÖÃNNMI¹¦ÄÜÊÇ·ñ¿ªÆô
 @Input				pClient								: NBIOT¿Í»§¶ËÊµÀý
					state								: CloseFunc / OpenFunc
 @Return				NBIOT_StatusTypeDef						: NBIOT´¦Àí×´Ì¬
**********************************************************************************************************/
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_SetNewMessageIndications(NBIOT_ClientsTypeDef* pClient, NBIOT_OpenOrCloseFuncTypeDef state)
{	
	NBIOT_StatusTypeDef NBStatus = NBIOT_OK;
	u8 cmd[64] = {0};	
	
	sprintf((char *)cmd, "+NNMI=%d", state);	
		
	/*·¢ËÍ"+NNMI"*/		
	if(bc95_send_cmd(cmd,"OK",SYSTEM_TICKS_PER_SEC,2))
	{		
		GPRS_DEBUG(DEBUG_ERROR, "+NNMI CMD ERR\n");
		NBStatus =	NBIOT_ERROR;	
	}

	return NBStatus;	

}
	

/**********************************************************************************************************
 @Function			NBIOT_StatusTypeDef NBIOT_Neul_NBxx_CheckReadAreaCode(NBIOT_ClientsTypeDef* pClient)
 @Description			NBIOT_Neul_NBxx_CheckReadAreaCode		: ¼ì³ö»ùÕ¾ÇøÓòÂë
 @Input				pClient									: NBIOT¿Í»§¶ËÊµÀý
 @Return			NBIOT_StatusTypeDef						: NBIOT´¦Àí×´Ì¬
**********************************************************************************************************/
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_CheckReadAreaCode(NBIOT_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBIOT_OK;
	u8 cmd[64] = {0};	
	u8 recv_back[64] = {0};
	int value[2] = 0;	

	/*·¢ËÍ"+CEREG=2"*/	
	sprintf((char *)cmd, "+CEREG=2");	
		
	if(bc95_send_cmd(cmd,"OK",SYSTEM_TICKS_PER_SEC,2))
	{		
		GPRS_DEBUG(DEBUG_ERROR, "+CEREG=2 CMD ERR\n");
		NBStatus =	NBIOT_ERROR;	
		goto exit;
	}

	/*·¢ËÍ"+CEREG?"*/		
	bc95_send_at("+CEREG?");				
	
	bc95_cmd_recv_back(recv_back,sizeof(recv_back),2);

	GPRS_DEBUG(DEBUG_NOTICE, "+CEREG? 1st str=%s\n", recv_back);	
	
	if (sscanf((const char*)recv_back, "%*[^+CEREG]%*[^:]:%*d,%*d,%hx,%x,%*d", &value[0], &value[1]) <= 0) 
	{	
		GPRS_DEBUG(DEBUG_NOTICE, "+CEREG? 1st failed\n");	
		NBStatus = NBIOT_ERROR;
		goto exit;	
	}

	/*·¢ËÍ"+CEREG=0"*/	
	sprintf((char *)cmd, "+CEREG=0");	
		
	if(bc95_send_cmd(cmd,"OK",SYSTEM_TICKS_PER_SEC,2))
	{			
		GPRS_DEBUG(DEBUG_ERROR, "+CEREG=0 CMD ERR\n");
		NBStatus =	NBIOT_ERROR;	
		goto exit;
	}

	
exit:
	return NBStatus;
	
}




/**********************************************************************************************************
 @Function			NBIOT_StatusTypeDef NBIOT_Neul_NBxx_SetCDPServer(NBIOT_ClientsTypeDef* pClient, const char *host, unsigned short port)
 @Description			NBIOT_Neul_NBxx_SetCDPServer			: ÉèÖÃCDP·þÎñÆ÷
 @Input				pClient									: NBIOT¿Í»§¶ËÊµÀý
					host										: CDP Server Address	
					port										: CDP Server Port
 @Return			NBIOT_StatusTypeDef						: NBIOT´¦Àí×´Ì¬
**********************************************************************************************************/
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_SetCDPServer(NBIOT_ClientsTypeDef* pClient, const char *host, unsigned short port)
{
	NBIOT_StatusTypeDef NBStatus = NBIOT_OK;
	u8 cmd[64] = {0};	
		
	sprintf((char *)cmd, "+NCDP=%s,%d", host, port);
		
	/*·¢ËÍ"+NNMI"*/		
	if(bc95_send_cmd(cmd,"OK",SYSTEM_TICKS_PER_SEC,2))
	{		
		GPRS_DEBUG(DEBUG_ERROR, "+NNMI CMD ERR\n");
		NBStatus =	NBIOT_ERROR;	
	}

	return NBStatus;
}


/**********************************************************************************************************
 @Function			NBIOT_StatusTypeDef NBIOT_Neul_NBxx_CheckReadCDPServer(NBIOT_ClientsTypeDef* pClient)
 @Description			NBIOT_Neul_NBxx_CheckReadCDPServer			: ²éÑ¯CDP·þÎñÆ÷
 @Input				pClient								: NBIOT¿Í»§¶ËÊµÀý
 @Return				NBIOT_StatusTypeDef						: NBIOT´¦Àí×´Ì¬
**********************************************************************************************************/
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_CheckReadCDPServer(NBIOT_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBIOT_OK;	
	u8 recv_back[64] = {0};

	memset((void *)&pClient->Parameter.cdpserver, 0x0, sizeof(pClient->Parameter.cdpserver));
		
	bc95_send_at("+NCDP?");

	bc95_cmd_recv_back(recv_back,sizeof(recv_back),2);	

	GPRS_DEBUG(DEBUG_NOTICE, "+NCDP? 1st str=%s\n", recv_back);		

	if (sscanf((const char*)recv_back, "%*[^+NCDP]%*[^:]:%[^,],%hu", (char *)&pClient->Parameter.cdpserver.CDPServerHost, &pClient->Parameter.cdpserver.CDPServerPort) <= 0) {
		GPRS_DEBUG(DEBUG_NOTICE, "+NCDP? 1st failed\n");		
		NBStatus = NBIOT_ERROR;
	}		

	return NBStatus;
}




/**********************************************************************************************************
 @Function			NBIOT_StatusTypeDef NBIOT_Neul_NBxx_CheckReadDateTime(NBIOT_ClientsTypeDef* pClient)
 @Description			NBIOT_Neul_NBxx_CheckReadDateTime			: ¼ì³öDateTime»ùÕ¾Ê±¼ä
 @Input				pClient								: NBIOT¿Í»§¶ËÊµÀý
 @Return				NBIOT_StatusTypeDef						: NBIOT´¦Àí×´Ì¬
**********************************************************************************************************/
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_CheckReadDateTime(NBIOT_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBIOT_OK;
	int zoneTime;
	u8	year,month,day,hour,min,sec;	
	u8 recv_back[64] = {0};			

	bc95_send_at("+CCLK?");			
	
	bc95_cmd_recv_back(recv_back,sizeof(recv_back),2);
	
	GPRS_DEBUG(DEBUG_NOTICE, "+CCLK? 1st str=%s\n", recv_back);	

	if ( sscanf((const char*)recv_back, 
		"%*[^+CCLK]%*[^:]:%d/%d/%d,%d:%d:%d+%d",
		&year, &month, &day, &hour, &min, &sec, &zoneTime) <= 0)
	{
		NBStatus = NBIOT_ERROR;
	}	
	else
	{	
		NBStatus = NBIOT_OK;	
	}

	return NBStatus;

}




/**********************************************************************************************************
 @Function			NBIOT_StatusTypeDef NBIOT_Neul_NBxx_CheckReadIMEI(NBIOT_ClientsTypeDef* pClient)
 @Description			NBIOT_Neul_NBxx_CheckReadIMEI				: ¼ì³öIMEIºÅ
 @Input				pClient								: NBIOT¿Í»§¶ËÊµÀý
 @Return				NBIOT_StatusTypeDef						: NBIOT´¦Àí×´Ì¬
**********************************************************************************************************/
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_CheckReadIMEI(NBIOT_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBIOT_OK;
	u8 recv_back[64] = {0};		
	
	bc95_send_at("+CGSN=1");			

	bc95_cmd_recv_back(recv_back,sizeof(recv_back),2);	
	
	GPRS_DEBUG(DEBUG_NOTICE, "+CGSN=1 1st str=%s\n", recv_back);	
	
	memset((void *)pClient->Parameter.imei, 0x0, sizeof(pClient->Parameter.imei));
	
	if (sscanf((const char*)recv_back, "%*[^+CGSN]%*[^:]:%[^\r]", pClient->Parameter.imei) <= 0) 
	{
		NBStatus = NBIOT_ERROR;	
	}	
	
	return NBStatus;
}


/**********************************************************************************************************
 @Function			NBIOT_StatusTypeDef NBIOT_Neul_NBxx_CheckReadIMEISV(NBIOT_ClientsTypeDef* pClient)
 @Description			NBIOT_Neul_NBxx_CheckReadIMEISV			: ¼ì³öIMEISVºÅ
 @Input				pClient								: NBIOT¿Í»§¶ËÊµÀý
 @Return				NBIOT_StatusTypeDef						: NBIOT´¦Àí×´Ì¬
**********************************************************************************************************/
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_CheckReadIMEISV(NBIOT_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBIOT_OK;
	u8 recv_back[64] = {0};			
	
	bc95_send_at("+CGSN=2");

	bc95_cmd_recv_back(recv_back,sizeof(recv_back),2);	
		
	GPRS_DEBUG(DEBUG_NOTICE, "+CGSN=2 1st str=%s\n", recv_back);	

	memset((void *)pClient->Parameter.imeisv, 0x0, sizeof(pClient->Parameter.imeisv));
		
	if (sscanf((const char*)recv_back, "%*[^+CGSN]%*[^:]:%[^\r]", pClient->Parameter.imeisv) <= 0) {
		NBStatus = NBIOT_ERROR;
	}
	
	return NBStatus;
}




/**********************************************************************************************************
 @Function			NBIOT_StatusTypeDef NBIOT_Neul_NBxx_CheckReadIMSI(NBIOT_ClientsTypeDef* pClient)
 @Description			NBIOT_Neul_NBxx_CheckReadIMSI				: ¼ì³öIMSIºÅ
 @Input				pClient								: NBIOT¿Í»§¶ËÊµÀý
 @Return				NBIOT_StatusTypeDef						: NBIOT´¦Àí×´Ì¬
**********************************************************************************************************/

NBIOT_StatusTypeDef NBIOT_Neul_NBxx_CheckReadIMSI(NBIOT_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBIOT_OK;
	u8 recv_back[64] = {0};		
	
	bc95_send_at("+CIMI");			
	
	bc95_cmd_recv_back(recv_back,sizeof(recv_back),2);		
			
	GPRS_DEBUG(DEBUG_NOTICE, "+CIMI 1st str=%s\n", recv_back);	
	
	memset((void *)pClient->Parameter.imsi, 0x0, sizeof(pClient->Parameter.imsi));
	
	if (sscanf((const char*)recv_back, "\r\n%[^\r]", pClient->Parameter.imsi) <= 0) {
		NBStatus = NBIOT_ERROR;
	}
	
	return NBStatus;
}




/**********************************************************************************************************
 @Function			NBIOT_StatusTypeDef NBIOT_Neul_NBxx_CheckReadCGPADDR(NBIOT_ClientsTypeDef* pClient)
 @Description			NBIOT_Neul_NBxx_CheckReadCGPADDR			: ¼ì³öCGPADDRµØÖ·
 @Input				pClient								: NBIOT¿Í»§¶ËÊµÀý
 @Return				NBIOT_StatusTypeDef						: NBIOT´¦Àí×´Ì¬
**********************************************************************************************************/
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_CheckReadCGPADDR(NBIOT_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBIOT_OK;
	u8 recv_back[64] = {0};			
	
	bc95_send_at("+CGPADDR");			
	
	bc95_cmd_recv_back(recv_back,sizeof(recv_back),2);		
				
	GPRS_DEBUG(DEBUG_NOTICE, "+CGPADDR 1st str=%s\n", recv_back);	

	memset((void *)pClient->Parameter.cgpaddr, 0x0, sizeof(pClient->Parameter.cgpaddr));
	
	if(	sscanf((const char*)recv_back, "%*[^+CGPADDR]%*[^,],%[^\r]", pClient->Parameter.cgpaddr) <= 0)
	{		
		NBStatus = NBIOT_ERROR;
	}
	
	return NBStatus;
}




/**********************************************************************************************************
 @Function			NBIOT_StatusTypeDef NBIOT_Neul_NBxx_CheckReadCGDCONT(NBIOT_ClientsTypeDef* pClient)
 @Description			NBIOT_Neul_NBxx_CheckReadCGDCONT			: ¼ì³öCGDCONTÍøÖ·
 @Input				pClient								: NBIOT¿Í»§¶ËÊµÀý
 @Return				NBIOT_StatusTypeDef						: NBIOT´¦Àí×´Ì¬
**********************************************************************************************************/

NBIOT_StatusTypeDef NBIOT_Neul_NBxx_CheckReadCGDCONT(NBIOT_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBIOT_OK;
	u8 recv_back[64] = {0};			
	
	bc95_send_at("+CGDCONT?");				
	
	bc95_cmd_recv_back(recv_back,sizeof(recv_back),2);		
				
	GPRS_DEBUG(DEBUG_NOTICE, "+CGDCONT? 1st str=%s\n", recv_back);	
	
	memset((void *)pClient->Parameter.cgdcont, 0x0, sizeof(pClient->Parameter.cgdcont));
			
	sscanf((const char*)recv_back, "%*[^+CGDCONT]%*[^,],%*[^,],%[^,]", pClient->Parameter.cgdcont);
	
	return NBStatus;
}

/**********************************************************************************************************
 @Function			NBIOT_StatusTypeDef NBIOT_Neul_NBxx_CheckReadStatisticsRADIO(NBIOT_ClientsTypeDef* pClient)
 @Description			NBIOT_Neul_NBxx_CheckReadStatisticsRADIO	: ¼ì³ö»ùÕ¾Á¬½Ó²ÎÊýRADIO
 @Input				pClient								: NBIOT¿Í»§¶ËÊµÀý
 @Return				NBIOT_StatusTypeDef						: NBIOT´¦Àí×´Ì¬
**********************************************************************************************************/
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_CheckReadStatisticsRADIO(NBIOT_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBIOT_OK;
	u8 recv_back[256] = {0};					
	
	bc95_send_at("+NUESTATS");					
	
	bc95_cmd_recv_back(recv_back,sizeof(recv_back),2);		

	GPRS_DEBUG(DEBUG_NOTICE, "+NUESTATS 1st str=%s\n", recv_back);	
	
	if ( sscanf((const char*)recv_back, 
		"%*[^S]Signal power:%d%*[^T]Total power:%d%*[^T]TX power:%d%*[^T]TX time:%d%*[^R]RX time:%d%*[^C]Cell ID:%d%*[^E]ECL:%d%*[^S]SNR:%d%*[^E]EARFCN:%d%*[^P]PCI:%d%*[^R]RSRQ:%d", 
		&pClient->Parameter.statisticsRADIO.Signalpower,
		&pClient->Parameter.statisticsRADIO.Totalpower,
		&pClient->Parameter.statisticsRADIO.TXpower,
		&pClient->Parameter.statisticsRADIO.TXtime,
		&pClient->Parameter.statisticsRADIO.RXtime,
		&pClient->Parameter.statisticsRADIO.CellID,
		&pClient->Parameter.statisticsRADIO.ECL,
		&pClient->Parameter.statisticsRADIO.SNR,
		&pClient->Parameter.statisticsRADIO.EARFCN,
		&pClient->Parameter.statisticsRADIO.PCI,
		&pClient->Parameter.statisticsRADIO.RSRQ) <= 0) {
		NBStatus = NBIOT_ERROR;	
	}

	
	return NBStatus;
}



/**********************************************************************************************************
 @Function			NBIOT_StatusTypeDef NBIOT_Neul_NBxx_CheckReadStatisticsCELL(NBIOT_ClientsTypeDef* pClient)
 @Description			NBIOT_Neul_NBxx_CheckReadStatisticsCELL		: ¼ì³ö»ùÕ¾Á¬½Ó²ÎÊýCELL
 @Input				pClient								: NBIOT¿Í»§¶ËÊµÀý
 @Return				NBIOT_StatusTypeDef						: NBIOT´¦Àí×´Ì¬
**********************************************************************************************************/
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_CheckReadStatisticsCELL(NBIOT_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBIOT_OK;
	u8 recv_back[64] = {0};	

	bc95_send_at("+NUESTATS=CELL");					

	bc95_cmd_recv_back(recv_back,sizeof(recv_back),2);		

	GPRS_DEBUG(DEBUG_NOTICE, "+NUESTATS 1st str=%s\n", recv_back);	
	
	if ( sscanf((const char*)recv_back, 		
		"%*[^NUESTATS]%*[^:]:CELL,%d,%d,%d,%d,%d,%d,%d", 
		&pClient->Parameter.statisticsCELL.earfcn, 
		&pClient->Parameter.statisticsCELL.physicalcellID, 
		&pClient->Parameter.statisticsCELL.primarycell, 
		&pClient->Parameter.statisticsCELL.rsrp, 
		&pClient->Parameter.statisticsCELL.rsrq, 
		&pClient->Parameter.statisticsCELL.rssi, 
		&pClient->Parameter.statisticsCELL.snr) <= 0) {
		NBStatus = NBIOT_ERROR;
	}
	
	return NBStatus;
}



/**********************************************************************************************************
 @Function			NBIOT_StatusTypeDef NBIOT_Neul_NBxx_CheckReadICCID(NBIOT_ClientsTypeDef* pClient)
 @Description			NBIOT_Neul_NBxx_CheckReadICCID			: ¼ì³öICCID¿¨ºÅ
 @Input				pClient								: NBIOT¿Í»§¶ËÊµÀý
 @Return				NBIOT_StatusTypeDef						: NBIOT´¦Àí×´Ì¬
**********************************************************************************************************/
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_CheckReadICCID(NBIOT_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBIOT_OK;
	u8 recv_back[64] = {0};		
	
	bc95_send_at("+NCCID");			

	bc95_cmd_recv_back(recv_back,sizeof(recv_back),2);		
	
	GPRS_DEBUG(DEBUG_NOTICE, "+NCCID 1st str=%s\n", recv_back);	
		
	memset((void *)pClient->Parameter.iccid, 0x0, sizeof(pClient->Parameter.iccid));
			
	if (sscanf((const char*)recv_back, "%*[^+NCCID]%*[^:]:%[^\r]", pClient->Parameter.iccid) <= 0) {
		NBStatus = NBIOT_ERROR;
	}		
		
	return NBStatus;
}




/**********************************************************************************************************
 @Function			NBIOT_StatusTypeDef NBIOT_Neul_NBxx_CheckReadManufacturer(NBIOT_ClientsTypeDef* pClient)
 @Description			NBIOT_Neul_NBxx_CheckReadManufacturer		: ¼ì³öÖÆÔìÉÌ±êÊ¶
 @Input				pClient								: NBIOT¿Í»§¶ËÊµÀý
 @Return				NBIOT_StatusTypeDef						: NBIOT´¦Àí×´Ì¬
**********************************************************************************************************/
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_CheckReadManufacturer(NBIOT_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBIOT_OK;
	u8 recv_back[64] = {0};			
	
	bc95_send_at("+CGMI");		
	
	bc95_cmd_recv_back(recv_back,sizeof(recv_back),2);			
			
	GPRS_DEBUG(DEBUG_NOTICE, "+CGMI 1st str=%s\n", g_tUart2.pRxBuf);			
		
	memset((void *)pClient->Parameter.manufacturer, 0x0, sizeof(pClient->Parameter.manufacturer));
				
	if (sscanf((const char*)g_tUart2.pRxBuf, "\r\n%[^\r]", pClient->Parameter.manufacturer) <= 0) {
		NBStatus = NBIOT_ERROR;
	}
	
	return NBStatus;
}





/**********************************************************************************************************
 @Function			NBIOT_StatusTypeDef NBIOT_Neul_NBxx_CheckReadManufacturerModel(NBIOT_ClientsTypeDef* pClient)
 @Description			NBIOT_Neul_NBxx_CheckReadManufacturerModel	: ¼ì³öÖÆÔìÉÌÐÍºÅ
 @Input				pClient								: NBIOT¿Í»§¶ËÊµÀý
 @Return				NBIOT_StatusTypeDef						: NBIOT´¦Àí×´Ì¬
**********************************************************************************************************/
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_CheckReadManufacturerModel(NBIOT_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBIOT_OK;
	u8 recv_back[64] = {0};	
	
	bc95_send_at("+CGMM");			

	bc95_cmd_recv_back(recv_back,sizeof(recv_back),2);			
		
	GPRS_DEBUG(DEBUG_NOTICE, "+CGMM 1st str=%s\n", recv_back);	
			
	memset((void *)pClient->Parameter.manufacturermode, 0x0, sizeof(pClient->Parameter.manufacturermode));
		
	if (sscanf((const char*)recv_back, "\r\n%[^\r]", pClient->Parameter.manufacturermode) <= 0) {
		NBStatus = NBIOT_ERROR;
	}	
	
	return NBStatus;
}



/**********************************************************************************************************
 @Function			NBIOT_StatusTypeDef NBIOT_Neul_NBxx_CheckReadModuleVersion(NBIOT_ClientsTypeDef* pClient)
 @Description			NBIOT_Neul_NBxx_CheckReadModuleVersion		: ¼ì³öÄ£×éÈí¼þ°æ±¾ºÅ
 @Input				pClient								: NBIOT¿Í»§¶ËÊµÀý
 @Return				NBIOT_StatusTypeDef						: NBIOT´¦Àí×´Ì¬
**********************************************************************************************************/
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_CheckReadModuleVersion(NBIOT_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBIOT_OK;
	u8 recv_back[64] = {0};		
	
	bc95_send_at("+CGMR");			

	bc95_cmd_recv_back(recv_back,sizeof(recv_back),2);			
				
	GPRS_DEBUG(DEBUG_NOTICE, "+CGMR 1st str=%s\n", recv_back);	
	
	memset((void *)recv_back, 0x0, sizeof(pClient->Parameter.modelversion));
			
	if (sscanf((const char*)recv_back, "%*[^SECURITY]%*[^,],%*11s%[^\r]", pClient->Parameter.modelversion) <= 0) {
		NBStatus = NBIOT_ERROR;	
	}
	
	return NBStatus;
}




/**********************************************************************************************************
 @Function			NBIOT_StatusTypeDef NBIOT_Neul_NBxx_CheckReadNewMessageIndications(NBIOT_ClientsTypeDef* pClient)
 @Description			NBIOT_Neul_NBxx_CheckReadNewMessageIndications	: ²éÑ¯NNMI¹¦ÄÜÊÇ·ñ¿ªÆô
 @Input				pClient									: NBIOT¿Í»§¶ËÊµÀý
 @Return				NBIOT_StatusTypeDef							: NBIOT´¦Àí×´Ì¬
**********************************************************************************************************/
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_CheckReadNewMessageIndications(NBIOT_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBIOT_OK;
	int nnmival = 0;
	u8 recv_back[64] = {0};		
	
	bc95_send_at("+NNMI?");				

	bc95_cmd_recv_back(recv_back,sizeof(recv_back),2);			

	GPRS_DEBUG(DEBUG_NOTICE, "+NNMI? 1st str=%s\n", recv_back);	
	
	if (sscanf((const char*)recv_back, "%*[^+NNMI]%*[^:]:%d", &nnmival) <= 0) {
		NBStatus = NBIOT_ERROR;
	}	
	else {
		if ((nnmival == OpenFunc) || (nnmival == CloseFunc)) {
			pClient->Parameter.nnmistate = (NBIOT_OpenOrCloseFuncTypeDef)nnmival;
		}
		else {
			NBStatus = NBIOT_ERROR;
		}
	}
	
	return NBStatus;
}

	


/**********************************************************************************************************
 @Function			NBIOT_StatusTypeDef NBIOT_Neul_NBxx_CheckReadSentMessageIndications(NBIOT_ClientsTypeDef* pClient)
 @Description			NBIOT_Neul_NBxx_CheckReadSentMessageIndications	: ²éÑ¯NSMI¹¦ÄÜÊÇ·ñ¿ªÆô
 @Input				pClient									: NBIOT¿Í»§¶ËÊµÀý
 @Return				NBIOT_StatusTypeDef							: NBIOT´¦Àí×´Ì¬
**********************************************************************************************************/
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_CheckReadSentMessageIndications(NBIOT_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBIOT_OK;
	int nsmival = 0;
	u8 recv_back[64] = {0};	
	
	bc95_send_at("+NSMI?");

	bc95_cmd_recv_back(recv_back,sizeof(recv_back),2);			

	GPRS_DEBUG(DEBUG_NOTICE, "+NSMI? 1st str=%s\n", recv_back);	
	
	if (sscanf((const char*)recv_back, "%*[^+NSMI]%*[^:]:%d", &nsmival) <= 0) {
		NBStatus = NBIOT_ERROR;
	}
	else {
		if ((nsmival == OpenFunc) || (nsmival == CloseFunc)) {
			pClient->Parameter.nsmistate = (NBIOT_OpenOrCloseFuncTypeDef)nsmival;
		}
		else {
			NBStatus = NBIOT_ERROR;
		}
	}

	return NBStatus;
}



/**********************************************************************************************************
 @Function			NBIOT_StatusTypeDef NBIOT_Neul_NBxx_CheckReadConfigUE(NBIOT_ClientsTypeDef* pClient)
 @Description			NBIOT_Neul_NBxx_CheckReadConfigUE			: ²éÑ¯NCONFIGÄ£Ê½
 @Input				pClient								: NBIOT¿Í»§¶ËÊµÀý
 @Return				NBIOT_StatusTypeDef						: NBIOT´¦Àí×´Ì¬
**********************************************************************************************************/
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_CheckReadConfigUE(NBIOT_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBIOT_OK;
	u8 recv_back[128] = {0};		

	bc95_send_at("+NCONFIG?");	

	bc95_cmd_recv_back(recv_back,sizeof(recv_back),2);			
	
	GPRS_DEBUG(DEBUG_NOTICE, "+NCONFIG? 1st str=%s\n", recv_back);	
	
	if (strstr((const char*)recv_back, "+NCONFIG:AUTOCONNECT,TRUE")) {
		pClient->Parameter.nconfig.autoConnect = NConfigTrue;
	}
	else if (strstr((const char*)recv_back, "+NCONFIG:AUTOCONNECT,FALSE")) {
		pClient->Parameter.nconfig.autoConnect = NConfigFalse;
	}
	else {
		NBStatus = NBIOT_ERROR;
		goto exit;
	}
	
	if (strstr((const char*)recv_back, "+NCONFIG:CR_0354_0338_SCRAMBLING,TRUE")) {
		pClient->Parameter.nconfig.crScrambling = NConfigTrue;
	}
	else if (strstr((const char*)recv_back, "+NCONFIG:CR_0354_0338_SCRAMBLING,FALSE")) {
		pClient->Parameter.nconfig.crScrambling = NConfigFalse;
	}
	else {
		NBStatus = NBIOT_ERROR;
		goto exit;
	}
	
	if (strstr((const char*)recv_back, "+NCONFIG:CR_0859_SI_AVOID,TRUE")) {
		pClient->Parameter.nconfig.crSiAvoid = NConfigTrue;
	}
	else if (strstr((const char*)recv_back, "+NCONFIG:CR_0859_SI_AVOID,FALSE")) {
		pClient->Parameter.nconfig.crSiAvoid = NConfigFalse;			
	}
	else {
		NBStatus = NBIOT_ERROR;
		goto exit;
	}
	
	if (strstr((const char*)recv_back, "+NCONFIG:COMBINE_ATTACH,TRUE")) {
		pClient->Parameter.nconfig.combineAttach = NConfigTrue;
	}
	else if (strstr((const char*)recv_back, "+NCONFIG:COMBINE_ATTACH,FALSE")) {
		pClient->Parameter.nconfig.combineAttach = NConfigFalse;
	}
	else {
		NBStatus = NBIOT_ERROR;
		goto exit;
	}
	
	if (strstr((const char*)recv_back, "+NCONFIG:CELL_RESELECTION,TRUE")) {
		pClient->Parameter.nconfig.cellReselection = NConfigTrue;
	}
	else if (strstr((const char*)recv_back, "+NCONFIG:CELL_RESELECTION,FALSE")) {
		pClient->Parameter.nconfig.cellReselection = NConfigFalse;
	}
	else {
		NBStatus = NBIOT_ERROR;
		goto exit;
	}
	
	if (strstr((const char*)recv_back, "+NCONFIG:ENABLE_BIP,TRUE")) {
		pClient->Parameter.nconfig.enableBip = NConfigTrue;
	}
	else if (strstr((const char*)recv_back, "+NCONFIG:ENABLE_BIP,FALSE")) {
		pClient->Parameter.nconfig.enableBip = NConfigFalse;
	}
	else {
		NBStatus = NBIOT_ERROR;
		goto exit;
	}

	
exit:
	return NBStatus;
}






/**********************************************************************************************************
 @Function			NBIOT_StatusTypeDef NBIOT_Neul_NBxx_SetConfigUE(NBIOT_ClientsTypeDef* pClient, const char *ncmd, NBIOT_NConfigTypeDef state)
 @Description			NBIOT_Neul_NBxx_SetConfigUE				: ÅäÖÃNCONFIGÄ£Ê½
 @Input				pClient								: NBIOT¿Í»§¶ËÊµÀý
					ncmd									: NeulÅäÖÃÏî
					state								: NeulÅäÖÃÄ£Ê½
 @Return				NBIOT_StatusTypeDef						: NBIOT´¦Àí×´Ì¬
**********************************************************************************************************/
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_SetConfigUE(NBIOT_ClientsTypeDef* pClient, const char *ncmd, NBIOT_NConfigTypeDef state)
{
	NBIOT_StatusTypeDef NBStatus = NBIOT_OK;
	u8 cmd[64] = {0};
	
	sprintf((char *)cmd, "AT+NCONFIG=%s,%s\r", ncmd, (state == NConfigTrue)?"TRUE":"FALSE");

	/*·¢ËÍ"+NCONFIG="*/
	if(bc95_send_cmd(cmd,"OK",SYSTEM_TICKS_PER_SEC,2))
	{		
		GPRS_DEBUG(DEBUG_ERROR, "+NCONFIG= CMD ERR\n");
		NBStatus = 	NBIOT_ERROR;
	}		
	
	return NBStatus;
}



