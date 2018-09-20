/**
  *********************************************************************************************************
  * @file    onenetconfig.c
  * @author  Movebroad -- KK
  * @version V1.0
  * @date    2018-08-31
  * @brief   
  *********************************************************************************************************
  * @attention
  *
  *
  *
  *********************************************************************************************************
  */
	
#include "onenet_config.h"
#include "onenet_func.h"	
#include "net_platform_config.h"	


unsigned char OneNET_SendBuf[ONENET_BUFFER_SIZE];
unsigned char OneNET_RecvBuf[ONENET_BUFFER_SIZE];
unsigned char OneNET_DataStack[ONENET_DATASTACK_SIZE];

/**********************************************************************************************************
 @Function			void OneNET_Client_Init(ONENET_ClientsTypeDef* pClient, ONENET_LWM2MTransportTypeDef* NetSock, NET_NBIOT_ClientsTypeDef* NetNbiotStack)
 @Description			OneNET_Client_Init						: ��ʼ��OneNET�ͻ���
 @Input				pClient								: OneNET�ͻ���ʵ��
					NetSock								: OneNET Transport Э��ջ
					NetNbiotStack							: NET NBIOT Э��ջ
 @Return				void
**********************************************************************************************************/
void OneNET_Client_Init(ONENET_ClientsTypeDef* pClient, ONENET_LWM2MTransportTypeDef* NetSock, NET_NBIOT_ClientsTypeDef* NetNbiotStack)
{
	pClient->Sendbuf									= OneNET_SendBuf;
	pClient->Recvbuf									= OneNET_RecvBuf;
	pClient->Sendbuf_size								= sizeof(OneNET_SendBuf);
	pClient->Recvbuf_size								= sizeof(OneNET_RecvBuf);
	pClient->Sendlen									= 0;
	pClient->Recvlen									= 0;
	pClient->DataProcessStack							= OneNET_DataStack;
	pClient->DataProcessStack_size						= sizeof(OneNET_DataStack);
	
	pClient->Command_Timeout_Sec						= ONENET_COMMAND_TIMEOUT_SEC;
	pClient->Command_Failure_Cnt						= ONENET_COMMAND_FAILURE_CNT;
	
	pClient->DictateRunCtl.dictateEnable				= false;
	pClient->DictateRunCtl.dictateTimeoutSec			= 0;
	pClient->DictateRunCtl.dictateInitFailureCnt		= 0;	
	
	
	
	
	
	
	pClient->ProcessState								= ONENET_PROCESSSTATE_INIT;
	pClient->LWM2MStack									= NetSock;
	pClient->NetNbiotStack								= NetNbiotStack;
}








/********************************************** END OF FLEE **********************************************/

