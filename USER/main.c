/*********************************************************************************
//������
//���ߣ����ƮԾ     ʱ�䣺       �ص㣺
//CPU�ͺţ�         ϵͳ��Ƶ��
//�汾�ţ�V0.0        
*********************************************************************************/
/*********************************************************************************
�ļ�������
*********************************************************************************/
#include "bsp.h"	

/*********************************************************************************
����������
*********************************************************************************/
/*********************************************************************************
��������������
*********************************************************************************/
/*********************************************************************************
�ⲿ����������
*********************************************************************************/
/*********************************************************************************
˽�б���������
*********************************************************************************/ 
/*********************************************************************************
���Ա���������
*********************************************************************************/
/*********************************************************************************
�ڲ�����������
*********************************************************************************/
/*********************************************************************************
���ܴ��붨����
*********************************************************************************/
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*********************************************************************************/

#define USE_ACK	 	1				

u16 mid_globle = 0;

static int gprs_debug_thread(void)	
{			
	u16 len = 0;	
	u16 i = 0;	
	u8 ch = 0;	
	u8 ch_tmp[2] = 0;		
		
	u8 *pRxBuf = 0;				
	u8 RxBuf[UART_BUF_SIZE_MAX] = {0};					
	int RxBuf_Length = 0;	
	u8 message_ID = 0;
	u16 mid = 0;
		
	int buff_cnt = 0;			
	int RxBuf_Len = 0;
	
	
	if(uart_recv_finish_get(NB_COM))		
	{		
		pRxBuf = g_tUart2.pRxBuf;	
		RxBuf_Length = g_tUart2.usRxCount;	

	#if 1		
		if (strstr(pRxBuf, "+NNMI:") != NULL)
		{		

		#if USE_ACK
		
			if (sscanf(pRxBuf, "\r\n+NNMI:%d,%4x%s",&RxBuf_Len,&mid,RxBuf) != 3)					
			{			
				DEBUG_PRINT(DEBUG_ERROR, "NNMI Failed\n");	
				return -1;	
			}	

			mid_globle = mid;	
		
		#else

			if (sscanf(pRxBuf, "\r\n+NNMI:%d,%s",&RxBuf_Len,RxBuf) != 3)					
			{		
				DEBUG_PRINT(DEBUG_ERROR, "NNMI Failed\n");	
				return -1;	
			}
			
		#endif
	
			BufAscToHex((u8 *)RxBuf,RxBuf_Len*2);	
					
			len = strlen((char *)RxBuf);
			
			for (i = 0; i < RxBuf_Len; i++)	
			{	
				printf("%c",RxBuf[i]);			
				comSendChar(DEV_COM,(u8)RxBuf[i]);					
			}
	
			//comSendChar(DEV_COM,(u8)'\r');
		}		
		else
		{		
			for (i = 0; i < RxBuf_Length; i++)		
			{		
				printf("%c",pRxBuf[i]);											
			}
		}


	#else

		for (i = 0; i < RxBuf_Length; i++)		
		{		
			printf("%c",pRxBuf[i]); 										
		}	

	#endif
		
		uart_recv_finish_clr(NB_COM);			
	}
}




static void gprs_func_thread(void)
{	
	u16 len = 0;			
	u16 i = 0;	
			
	u8 *pRxBuf = 0;				
	int RxBuf_Length = 0;			

	u8 message_ID = 1;		
	u16 mid = 0;
	u8 errcode = 0;
	
	u8 uart_buff_1[UART_BUF_SIZE_MAX] = {0};			
	u16 buff_cnt = 0;		
	
	if(uart_recv_finish_get(DEV_COM))		
	{	
		pRxBuf = g_tUart1.pRxBuf;		
		RxBuf_Length = g_tUart1.usRxCount;				

	#if USE_ACK	
	
		//������ɺ󽫻�����������ת����ʮ�����Ƶ��ַ���
		BufStrToASC(pRxBuf,RxBuf_Length);	
		buff_cnt = RxBuf_Length + 6;			
		mid = mid_globle;		
				
		sprintf((char *)uart_buff_1, "AT+NMGS=%d,%02x%04x%02x%04x%s\r",buff_cnt,message_ID,mid,errcode,RxBuf_Length,pRxBuf);	
		
		len = strlen((char *)uart_buff_1);	
	
	#else		

		//������ɺ󽫻�����������ת����ʮ�����Ƶ��ַ���
		BufStrToASC(pRxBuf,RxBuf_Length);	
		buff_cnt = RxBuf_Length + 2;							
					
		sprintf((char *)uart_buff_1, "AT+NMGS=%d,04x%s\r",buff_cnt,RxBuf_Length,pRxBuf);	
		
		len = strlen((char *)uart_buff_1);

	#endif			

		for (i = 0; i < len; i++)	
		{	
			comSendChar(NB_COM,(u8)uart_buff_1[i]);	
		}		

		for (i = 0; i < len; i++)	
		{			
			printf("%c",(u8)uart_buff_1[i]);			
		}							

		uart_recv_finish_clr(DEV_COM);		
	}

}


void Uart_Running_Init(void)
{	
	uart_recv_finish_clr(DEV_COM);
	uart_recv_finish_clr(NB_COM);
	uart_recv_finish_clr(DEBUG_COM);
}


s16 model_sta = 0;

void main(void)
{		
	u8 cnt = 0;
	u8 i ;
		
	disableInterrupts();                                      //�����ж�
		
	RCC_Configuration();
	bsp_Init();						
	//Rtc_Config();
	//Set_Alarm();
	//Pulse_Acquire_Config();	
	//IWDG_INIT(); 
	
    enableInterrupts();  		                                     //�����ж�

	/////////////////////////////////////////////////////////    
	//Read_ACUM_Flow(ADD_FLOW_ADD,&Cal.Water_Data);         //��ȡ��ǰ�ۻ�����
	//Read_Meter_Parameter();                               			    //��ȡˮ�����
	//Read_History_Save_Index();                            			    //��ȡ��ʷ���ݱ�������

	//BC95.Report_Bit = 1;
	//BC95.Start_Process = BC95_RECONNECT;
	
#if 1		
	
	/*�������ģ���Ƿ�����*/	
	while(1)
	{	
		if(bc95_modem_init() == 0)	
		{		
			break;		
		}					
				
		printf("BC95 Connecting......\n");			
					
		bsp_DelayMS(5*SYSTEM_TICKS_PER_SEC);											
	}
			
#endif	
		
	Uart_Running_Init();		
	
	while(1)
	{		
		shell_thread();		
		
		gprs_debug_thread();
			
		gprs_func_thread();						
			
		model_sta = bc95_state_check();	
		
	}		

	while (1)
	{
		//    RTC_GetDate(RTC_Format_BIN, &RTC_DateStr);
		//    RTC_GetTime(RTC_Format_BIN, &RTC_TimeStr);
		//�ϱ�ʧ��2����λ
		if( BC95.FailTimes >= 2 )
		{ 
		Save_Add_Flow(ADD_FLOW_ADD,&Cal.Water_Data);       //���浱ǰˮ��
		WWDG->CR = 0x80;  //���Ź���λ
		}

		IWDG_ReloadCounter();//���ؿ��Ź�������


		if(MeterParameter.DeviceStatus == SLEEP)     //�豸����˯��״̬
		{
		Sleep();
		}  
		else
		{
		Sys_Timer_Process();
		BC95_Process(); 
		}
	}
}

/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*********************************************************************************/
void Sleep(void)
{  
	HistoryData.ReadIndex = 0;
	BC95.Report_Bit = 0;
	BC95.Start_Process = BC95_POWER_DOWN;
	MeterParameter.DeviceRunTiming = 0;

	//  CLK_HaltConfig(CLK_Halt_FastWakeup,ENABLE);   //���ٻ��Ѻ�ʱ��ΪHSI  
	//  PWR_FastWakeUpCmd(ENABLE);                    //������Դ������Ŀ��ٻ���  
	PWR_UltraLowPowerCmd(ENABLE);                 //ʹ�ܵ�Դ�ĵ͹���ģʽ          //�������ڲ��ο���ѹ��ȡֵ��С
	//  CLK_HSICmd(DISABLE);                          //�ر��ڲ�����ʱ��

	halt();
}

/**
*******************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*********************************************************************************/
void IWDG_INIT(void)  //���Ź���ʼ��
{ 
	IWDG_Enable();//�������Ź�
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);//����дԤ��Ƶ�������ؼĴ���
	IWDG_SetPrescaler(IWDG_Prescaler_256);//����IWDGԤ��Ƶֵ
	IWDG_SetReload(0xFF);//��������ֵ1.7s��(255+1)*256/38K = 1.72s
	IWDG_ReloadCounter();//���ؼ�����
}




