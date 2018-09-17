/*********************************************************************************
//概述：
//作者：随风飘跃     时间：       地点：
//CPU型号：         系统主频：
//版本号：V0.0        
*********************************************************************************/
/*********************************************************************************
文件包含区
*********************************************************************************/
#include "bsp.h"	

/*********************************************************************************
常量定义区
*********************************************************************************/
/*********************************************************************************
公共变量定义区
*********************************************************************************/
/*********************************************************************************
外部变量声明区
*********************************************************************************/
/*********************************************************************************
私有变量定义区
*********************************************************************************/ 
/*********************************************************************************
测试变量定义区
*********************************************************************************/
/*********************************************************************************
内部函数定义区
*********************************************************************************/
/*********************************************************************************
功能代码定义区
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

static int gprs_debug_thread(void)	
{			
	u16 len = 0;	
	u16 i = 0;	
	u8 ch = 0;	
	u8 ch_tmp[2] = 0;		
		
	u8 *pRxBuf = 0;				
	u8 RxBuf[UART_BUF_SIZE_MAX] = {0};					
	int RxBuf_Length = 0;				
		
	int buff_cnt = 0;			
	int RxBuf_Len = 0;	

	int year = 0,day = 0,month = 0;	
	
	
	if(uart_recv_finish_get(NB_COM))		
	{		
		pRxBuf = g_tUart2.pRxBuf;	
		RxBuf_Length = g_tUart2.usRxCount;	
			
		if (strstr(pRxBuf, "+NNMI:") != NULL)
		{	
			if (sscanf(pRxBuf, "\r\n+NNMI:%d,%s",&RxBuf_Len,RxBuf) != 2)					
			{	
				DEBUG_PRINT(DEBUG_ERROR, "NNMI Failed\n");	
				return -1;
			}
	
			BufAscToHex((u8 *)RxBuf,RxBuf_Len*2);	
					
			len = strlen((char *)RxBuf);
			
			for (i = 0; i < RxBuf_Len; i++)	
			{	
				printf("%c",RxBuf[i]);		
				comSendChar(DEV_COM,(u8)RxBuf[i]);					
			}

			comSendChar(DEV_COM,(u8)'\r');
		}		
		else
		{		
			for (i = 0; i < RxBuf_Length; i++)		
			{		
				printf("%c",pRxBuf[i]);											
			}
		}			

		uart_recv_finish_clr(NB_COM);			
	}
}




//u16 buff_cnt = 0;		
//u8 uart_buff[UART_BUF_SIZE_MAX] = {0};		

static void gprs_func_thread(void)
{	
	u16 len = 0;			
	u16 i = 0;	
			
	u8 *pRxBuf = 0;				
	int RxBuf_Length = 0;			
		
	u8 uart_buff_1[UART_BUF_SIZE_MAX] = {0};			
	u16 buff_cnt = 0;		
	
	if(uart_recv_finish_get(DEV_COM))		
	{	
		pRxBuf = g_tUart1.pRxBuf;		
		RxBuf_Length = g_tUart1.usRxCount;				
				
		//接收完成后将缓冲区的数据转换成十六进制的字符串
		BufStrToASC(pRxBuf,RxBuf_Length);	
		buff_cnt = RxBuf_Length + 2;
				
		sprintf((char *)uart_buff_1, "AT+NMGS=%d,%04x%s\r",buff_cnt,RxBuf_Length,pRxBuf);	
		
		len = strlen((char *)uart_buff_1);
		
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
		
	disableInterrupts();                                      //关总中断
		
	RCC_Configuration();
	bsp_Init();						
	//Rtc_Config();
	//Set_Alarm();
	//Pulse_Acquire_Config();	
	//IWDG_INIT(); 
	
    enableInterrupts();  	                                     //开总中断

	/////////////////////////////////////////////////////////    
	//Read_ACUM_Flow(ADD_FLOW_ADD,&Cal.Water_Data);         //读取当前累积流量
	//Read_Meter_Parameter();                               			    //读取水表参数
	//Read_History_Save_Index();                            			    //读取历史数据保存索引

	//BC95.Report_Bit = 1;
	//BC95.Start_Process = BC95_RECONNECT;

#if 1		

	/*开机检测模块是否正常*/	
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

	//  CLK_HaltConfig(CLK_Halt_FastWakeup,ENABLE);   //快速唤醒后时钟为HSI  
	//  PWR_FastWakeUpCmd(ENABLE);                    //开启电源管理里的快速唤醒  
	PWR_UltraLowPowerCmd(ENABLE);                 //使能电源的低功耗模式          //开启后内部参考电压获取值变小
	//  CLK_HSICmd(DISABLE);                          //关闭内部高速时钟

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
void IWDG_INIT(void)  //看门狗初始化
{ 
	IWDG_Enable();//启动看门狗
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);//允许写预分频器和重载寄存器
	IWDG_SetPrescaler(IWDG_Prescaler_256);//设置IWDG预分频值
	IWDG_SetReload(0xFF);//设置重载值1.7s：(255+1)*256/38K = 1.72s
	IWDG_ReloadCounter();//重载计数器
}
/*********************************************************************************
 Function:      //
 Description:   //独立看门狗复位
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
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

/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
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

