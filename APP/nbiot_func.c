#include "nbiot_func.h"	
#include "basic_fun.h"
#include "include.h"	
#include "bsp.h"			


static void BC95_PrintRxData(uint8_t _ch)
{		
#ifdef BC95_TO_DEBUG_EN
		comSendChar(DEBUG_COM, _ch);		/* 将接收到数据打印到调试串口1 */
#endif
}


static s16 bc95_send_at(const u8 *p_cmd)	
{		
	s16 i = 0;
	
	if (NULL == p_cmd)
	{	
		GPRS_DEBUG(DEBUG_ERROR, "param error\n");
		return -1;
	}	

	
	uart_recv_finish_clr(NB_COM);	/* 清零串口接收缓冲区 */	

	comSendChar(NB_COM,(u8)'A');
	comSendChar(NB_COM,(u8)'T');
		
	comSendBuf(NB_COM, (uint8_t *)p_cmd, strlen(p_cmd));
			
	comSendChar(NB_COM,(u8)'\r');		
	
	return 0;	
}


void bc95_time_delay(uint16_t _usTimeOut,u16 count)
{
	if (_usTimeOut > 0)
	{	
		bsp_StartTimer(BC95_TMR_ID, _usTimeOut);		/* 使用软件定时器作为超时控制 */
	}	

	while (1)
	{
		bsp_Idle(); 			/* CPU空闲执行的操作， 见 bsp.c 和 bsp.h 文件 */
					
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
	uint8_t ucData;
	uint16_t pos = 0;
	int8_t ret;
	uint8_t status = 0; 	/* 接收状态 */

	/* _usTimeOut == 0 表示无限等待 */
	if (_usTimeOut > 0)
	{	
		bsp_StartTimer(BC95_TMR_ID, _usTimeOut);		/* 使用软件定时器作为超时控制 */
	}	
	
	while (1)
	{
		bsp_Idle(); 			/* CPU空闲执行的操作， 见 bsp.c 和 bsp.h 文件 */
		
		if (_usTimeOut > 0)
		{	
			if (bsp_CheckTimer(BC95_TMR_ID))
			{
				ret = -1;	/* 超时 */
				break;
			}
		}				
			
		if (comGetChar(NB_COM, byte))	
		{	
			//BC95_PrintRxData(ucData);		/* 将接收到数据打印到调试串口1 */
				
			//bsp_StartTimer(BC95_TMR_ID, _usTimeOut);			
					
			ret = 0;

			break;
		}	
	}
	
	return ret;
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
	
	/* 接收起始字符 */
	while(1)
	{			
		if ((ret = bc95_read_onebyte((u8 *)&ch[1], timeout)) != 0)
		{
			GPRS_DEBUG(DEBUG_ERROR, "gprs_serial_read_onebyte error\n");
			return ret;
		}	

		if ('\r' == ch[0] && '\n' == ch[1])
		{
			break; // 找到起始字符
		}
		else
		{
			ch[0] = ch[1];
		}
	}
	
	/* 接收实际数据 */
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

	/* 接收结束字符 */
	while(1)
	{	
		if ((ret = bc95_read_onebyte((u8 *)&ch[1], timeout)) != 0)
		{		
			GPRS_DEBUG(DEBUG_ERROR, "gprs_serial_read_onebyte error\n");
			return ret;
		}

		if ('\r' == ch[0] && '\n' == ch[1])
		{
			break; // 找到结束字符
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
	s16 ret = 0;		
	
	while (count--)		
	{			
		if ((ret = bc95_recv_ret_line(recv_back, len, SYSTEM_TICKS_PER_SEC)) < 0)
		{	
			if (-2 == ret)
			{	
				GPRS_DEBUG(DEBUG_ERROR, "gprs recv time out\n");
				return -1;
			}	
			
			GPRS_DEBUG(DEBUG_INFO, "bc95_recv_ret_line error\n");
				
			continue;
		}	
		else
		{			
			break;	
		}	
	}	

	if(count == 0)
	{	
		GPRS_DEBUG(DEBUG_ERROR, "bc95_cmd_recv_back timeout\n");	
		return -1;
	}
	else 
	{	
		return 0;	
	}
}
	



static s16 bc95_send_cmd(const u8 *cmd,const u8 *ack,u16 waittime,u16 count)//n*0ms
{	
	u8 str_raw[64]  = {0};
	s16 ret = 0;

	uart_recv_finish_clr(NB_COM);

	bc95_send_at(cmd);
	
	/*发送AT*/
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


static s16 neul_bc95_set_band(int band)//设置频段
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

	uart_recv_finish_clr(NB_COM);	
			
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
	
	uart_recv_finish_clr(NB_COM);		

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
		
	uart_recv_finish_clr(NB_COM);			

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
	
	GPRS_DEBUG(DEBUG_NOTICE, "云平台地址设置完成:%s,%s\r\n",ipaddr,port);					
	
	return 0;					
}


static s16 neul_bc95_get_time(void)
{
	u8	year,mon,date,hour,min,sec;
	char *cmd = "+CCLK?";		
	u8 recv_back[64] = {0};	

	uart_recv_finish_clr(NB_COM);	

	bc95_send_at(cmd);			
	
	bc95_cmd_recv_back(recv_back,sizeof(recv_back),2);
	
	GPRS_DEBUG(DEBUG_NOTICE, "+CCLK? 1st str=%s\n", recv_back);	

	bc95_cmd_recv_back(recv_back,sizeof(recv_back),2);		
			
	GPRS_DEBUG(DEBUG_NOTICE, "+CCLK? 1st str=%s\n", recv_back);
		
	return 0;
}






#define RESET_COUNT 6	/*大约一分钟*/
	
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

	/*发送"AT"*/		
	if(bc95_send_cmd("","OK",SYSTEM_TICKS_PER_SEC,6))			
	{			
		GPRS_DEBUG(DEBUG_ERROR, "AT CMD ERR\n");
		NBStatus = NBIOT_ERROR;	
		return NBStatus;	
	}		

	bsp_DelayMS(5);

	/*发送"+CMEE=1"*/	
	if(bc95_send_cmd("+CMEE=1","OK",SYSTEM_TICKS_PER_SEC,2))
	{	
		GPRS_DEBUG(DEBUG_ERROR, "+CMEE=1 CMD ERR\n");
		return -1;
	}		

	bsp_DelayMS(5);

	/*发送"+NBAND?"*/
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

	/*发送"+CGATT=1"*/	
	if(bc95_send_cmd("+CGATT=1","OK",SYSTEM_TICKS_PER_SEC,2))
	{		
		GPRS_DEBUG(DEBUG_ERROR, "+CGATT=1 CMD ERR\n");
		return -1;	
	}

	bsp_DelayMS(5);
	
	/*发送"+CGATT?"*/	
	if(neul_bc95_get_netstat())	
	{		
		GPRS_DEBUG(DEBUG_ERROR, "+CGATT? CMD ERR\n");	
		
		bc95_err_cnt(&err_cnt[1]);
		
		return -1;
	}

	err_cnt[1] = 0;

	bsp_DelayMS(5);	

	/*发送"+CSQ"*/							
	if(neul_bc95_get_csq())
	{		
		GPRS_DEBUG(DEBUG_ERROR, "+CSQ CMD ERR\n");	
		
		bc95_err_cnt(&err_cnt[2]);
		
		return -1;		
	}	

	err_cnt[2] = 0;	

	bsp_DelayMS(5);

	/*发送"+NSMI=1"*/
	if(bc95_send_cmd("+NSMI=1","OK",SYSTEM_TICKS_PER_SEC,2))	
	{						
		GPRS_DEBUG(DEBUG_ERROR, "+NSMI=1 CMD ERR\n");	
		return -1;		
	}

	bsp_DelayMS(5);

	/*发送"+NNMI=1"*/	
	if(bc95_send_cmd("+NNMI=1","OK",SYSTEM_TICKS_PER_SEC,2))	
	{		
		GPRS_DEBUG(DEBUG_ERROR, "+NNMI=1 CMD ERR\n");
		return -1;		
	}		

	bsp_DelayMS(5);

	/*发送"+CEREG?"*/	
	if(neul_bc95_get_cereg())
	{	
		GPRS_DEBUG(DEBUG_ERROR, "+CEREG? CMD ERR\n");
		
		bc95_err_cnt(&err_cnt[3]);			
		
		return -1;		
	}	

	err_cnt[3] = 0;		

	bsp_DelayMS(5);	

	/*发送"+NCDP="*/				
	if(neul_bc95_set_cdpserver(CDP_IP_ADDRESS,CDP_IP_PORT))	
	{		
		GPRS_DEBUG(DEBUG_ERROR, "+NCDP= CMD ERR\n");
		return -1;				
	}	

	bsp_DelayMS(5);	

	/*发送"+CCLK?"*/		
	neul_bc95_get_time();								

	bsp_DelayMS(5);							

	printf("gprs init success\n");			

	/*开启一个BC95状态检测的软件定时器,周期10s*/
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
		
		/*发送"CGATT=1"*/
		if(neul_bc95_get_netstat())
		{		
			GPRS_DEBUG(DEBUG_ERROR, "+CGATT=1 STATE CHK ERR\n");
			neul_bc95_reset_netstat();	
			ret = 0;
		}			
		
#if 0	
		
		/*发送"+CEREG?"*/	
		if(neul_bc95_get_cereg())
		{		
			GPRS_DEBUG(DEBUG_ERROR, "+CEREG? STATE CHK ERR\n");		
			ret = 0;
		}	
		
#endif

		/*如果模块网络出现错误,超过12次重启模块*/
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
 @Description			NBIOT_Neul_NBxx_DictateEvent_GetError	: 浜嬩欢杩愯鎺у埗鍣ㄨ幏鍙栭敊璇爜(鍐呴儴浣跨敤)
 @Input				pClient							: NBIOT瀹㈡埛绔疄渚�
 @Return				NBIOT_StatusTypeDef					: NBIOT澶勭悊鐘舵��
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
 @Function			NBIOT_StatusTypeDef NBIOT_Neul_NBxx_SetReportTerminationError(NBIOT_ClientsTypeDef* pClient, NBIOT_ReportErrorTypeDef enable)
 @Description			NBIOT_Neul_NBxx_SetReportTerminationError	: 设置CMEE
 @Input				pClient								: NBIOT客户端实例
					enable								: CMEDisable						= 0
														  CMEEnable						= 1
 @Return				NBIOT_StatusTypeDef				: NBIOT处理状态
**********************************************************************************************************/
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_SetReportTerminationError(NBIOT_ClientsTypeDef* pClient, NBIOT_ReportErrorTypeDef enable)
{
	NBIOT_StatusTypeDef NBStatus = NBIOT_OK;
	u8 cmd[64] = {0};
			
	sprintf((char *)cmd, "+CMEE=%d", enable);		

	/*发送"+CMEE=1"*/	
	if(bc95_send_cmd(cmd,"OK",SYSTEM_TICKS_PER_SEC,2))
	{		
		GPRS_DEBUG(DEBUG_ERROR, "+CMEE=1 CMD ERR\n");
		NBStatus = 	NBIOT_ERROR;
	}	

	return NBStatus;			
}


/**********************************************************************************************************
 @Function			NBIOT_StatusTypeDef NBIOT_Neul_NBxx_SetSupportedBands(NBIOT_ClientsTypeDef* pClient, NBIOT_BandTypeDef bands)
 @Description			NBIOT_Neul_NBxx_SetSupportedBands			: 设置Band
 @Input				pClient								: NBIOT客户端实例
					bands								: ChinaTelecom						= 5
														  ChinaMobile						= 8
														  ChinaUnicom						= 20
 @Return				NBIOT_StatusTypeDef						: NBIOT处理状态
**********************************************************************************************************/
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_SetSupportedBands(NBIOT_ClientsTypeDef* pClient, NBIOT_BandTypeDef bands)
{
	NBIOT_StatusTypeDef NBStatus = NBIOT_OK;
	u8 cmd[64] = {0};
	
	sprintf((char *)cmd, "+NBAND=%d", bands);
	
	/*发送"+NBAND=5"*/		
	if(bc95_send_cmd(cmd,"OK",SYSTEM_TICKS_PER_SEC,2))
	{	
		GPRS_DEBUG(DEBUG_ERROR, "+NBAND CMD ERR\n");
		NBStatus = 	NBIOT_ERROR;	
	}
	
	return NBStatus;
}



/**********************************************************************************************************
 @Function			NBIOT_StatusTypeDef NBIOT_Neul_NBxx_CheckReadSupportedBands(NBIOT_ClientsTypeDef* pClient)
 @Description			NBIOT_Neul_NBxx_CheckReadSupportedBands		: 查询Band
 @Input				pClient								: NBIOT客户端实例
 @Return				NBIOT_StatusTypeDef						: NBIOT处理状态
**********************************************************************************************************/
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_CheckReadSupportedBands(NBIOT_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBIOT_OK;
	int bands = 0;
	char *cmd = "+NBAND?";			
	u8 recv_back[64] = {0};

	uart_recv_finish_clr(NB_COM);		

	bc95_send_at(cmd);
	
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
			/*记录当前的band号*/	
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
 @Description			NBIOT_Neul_NBxx_QuerySendMessageCOAPPayload	: COAP查询消息已送达NB
 @Input				pClient								: NBIOT客户端实例
 @Return				NBIOT_StatusTypeDef						: NBIOT处理状态
**********************************************************************************************************/
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_QuerySendMessageCOAPPayload(NBIOT_ClientsTypeDef* pClient)
{		
	NBIOT_StatusTypeDef NBStatus = NBIOT_OK;
	char *cmd = "+NQMGS";		
	u8 recv_back[64] = {0};
	int value[3] = 0;	

	uart_recv_finish_clr(NB_COM);		

	bc95_send_at(cmd);
	
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
 @Description			NBIOT_Neul_NBxx_SetAttachOrDetach			: 设置终端入网退网
 @Input				pClient								: NBIOT客户端实例
					attdet								: Detach / Attach
 @Return				NBIOT_StatusTypeDef						: NBIOT处理状态
**********************************************************************************************************/
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_SetAttachOrDetach(NBIOT_ClientsTypeDef* pClient, NBIOT_NetstateTypeDef attdet)
{
	NBIOT_StatusTypeDef NBStatus = NBIOT_OK;
	u8 cmd[64] = {0};
	
	sprintf((char *)cmd, "+CGATT=%d", attdet);	
	
	/*发送"+CGATT"*/		
	if(bc95_send_cmd(cmd,"OK",SYSTEM_TICKS_PER_SEC,2))
	{	
		GPRS_DEBUG(DEBUG_ERROR, "+CGATT CMD ERR\n");
		NBStatus =	NBIOT_ERROR;
	}

	return NBStatus;
}


	
/**********************************************************************************************************
 @Function			NBIOT_StatusTypeDef NBIOT_Neul_NBxx_CheckReadAttachOrDetach(NBIOT_ClientsTypeDef* pClient)
 @Description			NBIOT_Neul_NBxx_CheckReadAttachOrDetach		: 查询终端入网退网
 @Input				pClient									: NBIOT客户端实例
 @Return			NBIOT_StatusTypeDef						: NBIOT处理状态
**********************************************************************************************************/
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_CheckReadAttachOrDetach(NBIOT_ClientsTypeDef* pClient)	
{	
	NBIOT_StatusTypeDef NBStatus = NBIOT_OK;
	int netval = 0;
	u8 recv_back[64] = {0};

	uart_recv_finish_clr(NB_COM);	
			
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
			/*记录网络状态*/	
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
 @Description			NBIOT_Neul_NBxx_CheckReadRSSI				: 检出RSSI号
 @Input				pClient								: NBIOT客户端实例
 @Return				NBIOT_StatusTypeDef						: NBIOT处理状态
**********************************************************************************************************/
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_CheckReadRSSI(NBIOT_ClientsTypeDef* pClient)	
{
	NBIOT_StatusTypeDef NBStatus = NBIOT_OK;
	u8 recv_back[64] = {0};
	int value = 0;	
	
	uart_recv_finish_clr(NB_COM);		

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
 @Description			NBIOT_Neul_NBxx_SetSentMessageIndications	: 设置NSMI功能是否开启
 @Input				pClient								: NBIOT客户端实例
					state								: CloseFunc / OpenFunc
 @Return				NBIOT_StatusTypeDef						: NBIOT处理状态
**********************************************************************************************************/
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_SetSentMessageIndications(NBIOT_ClientsTypeDef* pClient, NBIOT_OpenOrCloseFuncTypeDef state)
{
	NBIOT_StatusTypeDef NBStatus = NBIOT_OK;
	u8 cmd[64] = {0};
	
	sprintf((char *)cmd, "+NSMI=%d", state);	
		
	/*发送"+NSMI"*/		
	if(bc95_send_cmd(cmd,"OK",SYSTEM_TICKS_PER_SEC,2))
	{		
		GPRS_DEBUG(DEBUG_ERROR, "+NSMI CMD ERR\n");
		NBStatus =	NBIOT_ERROR;
	}

	return NBStatus;	
}


/**********************************************************************************************************
 @Function			NBIOT_StatusTypeDef NBIOT_Neul_NBxx_SetNewMessageIndications(NBIOT_ClientsTypeDef* pClient, NBIOT_OpenOrCloseFuncTypeDef state)
 @Description			NBIOT_Neul_NBxx_SetNewMessageIndications	: 设置NNMI功能是否开启
 @Input				pClient								: NBIOT客户端实例
					state								: CloseFunc / OpenFunc
 @Return				NBIOT_StatusTypeDef						: NBIOT处理状态
**********************************************************************************************************/
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_SetNewMessageIndications(NBIOT_ClientsTypeDef* pClient, NBIOT_OpenOrCloseFuncTypeDef state)
{	
	NBIOT_StatusTypeDef NBStatus = NBIOT_OK;
	u8 cmd[64] = {0};	
	
	sprintf((char *)cmd, "+NNMI=%d", state);	
		
	/*发送"+NNMI"*/		
	if(bc95_send_cmd(cmd,"OK",SYSTEM_TICKS_PER_SEC,2))
	{		
		GPRS_DEBUG(DEBUG_ERROR, "+NNMI CMD ERR\n");
		NBStatus =	NBIOT_ERROR;	
	}

	return NBStatus;	

}
	

/**********************************************************************************************************
 @Function			NBIOT_StatusTypeDef NBIOT_Neul_NBxx_CheckReadAreaCode(NBIOT_ClientsTypeDef* pClient)
 @Description			NBIOT_Neul_NBxx_CheckReadAreaCode		: 检出基站区域码
 @Input				pClient									: NBIOT客户端实例
 @Return			NBIOT_StatusTypeDef						: NBIOT处理状态
**********************************************************************************************************/
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_CheckReadAreaCode(NBIOT_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBIOT_OK;
	u8 cmd[64] = {0};	
	u8 recv_back[64] = {0};
	int value[2] = 0;	

	/*发送"+CEREG=2"*/	
	sprintf((char *)cmd, "+CEREG=2");	
		
	if(bc95_send_cmd(cmd,"OK",SYSTEM_TICKS_PER_SEC,2))
	{		
		GPRS_DEBUG(DEBUG_ERROR, "+CEREG=2 CMD ERR\n");
		NBStatus =	NBIOT_ERROR;	
		goto exit;
	}

	/*发送"+CEREG?"*/		
	uart_recv_finish_clr(NB_COM);		

	bc95_send_at("+CEREG?");				
	
	bc95_cmd_recv_back(recv_back,sizeof(recv_back),2);

	GPRS_DEBUG(DEBUG_NOTICE, "+CEREG? 1st str=%s\n", recv_back);	
	
	if (sscanf((const char*)recv_back, "%*[^+CEREG]%*[^:]:%*d,%*d,%hx,%x,%*d", &value[0], &value[1]) <= 0) 
	{	
		GPRS_DEBUG(DEBUG_NOTICE, "+CEREG? 1st failed\n");	
		NBStatus = NBIOT_ERROR;
		goto exit;	
	}

	/*发送"+CEREG=0"*/	
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
 @Description			NBIOT_Neul_NBxx_SetCDPServer			: 设置CDP服务器
 @Input				pClient									: NBIOT客户端实例
					host										: CDP Server Address	
					port										: CDP Server Port
 @Return			NBIOT_StatusTypeDef						: NBIOT处理状态
**********************************************************************************************************/
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_SetCDPServer(NBIOT_ClientsTypeDef* pClient, const char *host, unsigned short port)
{
	NBIOT_StatusTypeDef NBStatus = NBIOT_OK;
	u8 cmd[64] = {0};	
		
	sprintf((char *)cmd, "+NCDP=%s,%d", host, port);
		
	/*发送"+NNMI"*/		
	if(bc95_send_cmd(cmd,"OK",SYSTEM_TICKS_PER_SEC,2))
	{		
		GPRS_DEBUG(DEBUG_ERROR, "+NNMI CMD ERR\n");
		NBStatus =	NBIOT_ERROR;	
	}

	return NBStatus;
}


/**********************************************************************************************************
 @Function			NBIOT_StatusTypeDef NBIOT_Neul_NBxx_CheckReadCDPServer(NBIOT_ClientsTypeDef* pClient)
 @Description			NBIOT_Neul_NBxx_CheckReadCDPServer			: 查询CDP服务器
 @Input				pClient								: NBIOT客户端实例
 @Return				NBIOT_StatusTypeDef						: NBIOT处理状态
**********************************************************************************************************/
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_CheckReadCDPServer(NBIOT_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBIOT_OK;	
	u8 recv_back[64] = {0};

	memset((void *)&pClient->Parameter.cdpserver, 0x0, sizeof(pClient->Parameter.cdpserver));
	
	NBIOT_Neul_NBxx_ATCmd_SetCmdStack(pClient, (unsigned char*)"AT+NCDP?\r", strlen("AT+NCDP?\r"), "OK", "ERROR");
	

	uart_recv_finish_clr(NB_COM);		
	
	bc95_send_at("+NCDP?");		

	bc95_cmd_recv_back(recv_back,sizeof(recv_back),2);

	
		
	if (sscanf((const char*)pClient->ATCmdStack->ATRecvbuf, "%*[^+NCDP]%*[^:]:%[^,],%hu", (char *)&pClient->Parameter.cdpserver.CDPServerHost, &pClient->Parameter.cdpserver.CDPServerPort) <= 0) {
		NBStatus = NBIOT_ERROR;
	}

	
	return NBStatus;
}




/**********************************************************************************************************
 @Function			NBIOT_StatusTypeDef NBIOT_Neul_NBxx_CheckReadDateTime(NBIOT_ClientsTypeDef* pClient)
 @Description			NBIOT_Neul_NBxx_CheckReadDateTime			: 检出DateTime基站时间
 @Input				pClient								: NBIOT客户端实例
 @Return				NBIOT_StatusTypeDef						: NBIOT处理状态
**********************************************************************************************************/
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_CheckReadDateTime(NBIOT_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBIOT_OK;
	int zoneTime;
	u8	year,month,day,hour,min,sec;	
	u8 recv_back[64] = {0};			

	uart_recv_finish_clr(NB_COM);	

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
 @Function			NBIOT_StatusTypeDef NBIOT_Neul_NBxx_HardwareReboot(NBIOT_ClientsTypeDef* pClient, u32 rebootTimeoutMS)
 @Description			NBIOT_Neul_NBxx_HardwareReboot			: 硬件重启NB模块
 @Input				pClient								: NBIOT客户端实例
					rebootTimeoutMS						: 重启等待超时时间
 @Return				NBIOT_StatusTypeDef						: NBIOT处理状态
**********************************************************************************************************/
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_HardwareReboot(NBIOT_ClientsTypeDef* pClient, u32 rebootTimeoutMS)
{
			 
}


/**********************************************************************************************************
 @Function			NBIOT_StatusTypeDef NBIOT_Neul_NBxx_HardwarePoweroff(NBIOT_ClientsTypeDef* pClient)
 @Description			NBIOT_Neul_NBxx_HardwareReboot			: 硬件断电
 @Input				pClient								: NBIOT客户端实例
 @Return				NBIOT_StatusTypeDef						: NBIOT处理状态
**********************************************************************************************************/
NBIOT_StatusTypeDef NBIOT_Neul_NBxx_HardwarePoweroff(NBIOT_ClientsTypeDef* pClient)	
{
	
}


	



