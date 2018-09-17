/**
  *********************************************************************************************************
  * @file    nbiotconfig.c
  * @author  Movebroad -- KK
  * @version V1.0
  * @date    2018-03-03
  * @brief   
  *********************************************************************************************************
  * @attention
  *
  *
  *
  *********************************************************************************************************
  */

#include "nbiotconfig.h"
#include "nbiotfunc.h"
#include "nbiottransport.h"
#include "platform_config.h"
#include "platform_map.h"
#include "string.h"

unsigned char NBIOT_SendBuf[NBIOT_BUFFER_SIZE];
unsigned char NBIOT_RecvBuf[NBIOT_BUFFER_SIZE];
unsigned char NBIOT_DataStack[NBIOT_DATASTACK_SIZE];

NBIOT_BaudrateTypeDef NBIOTBaudRate;

/**********************************************************************************************************
 @Function			void NBIOT_Client_Init(NBIOT_ClientsTypeDef* pClient, NBIOT_ATCmdTypeDef* ATCmdStack, NET_NBIOT_ClientsTypeDef* NetNbiotStack)
 @Description			NBIOT_Client_Init						: 初始化NBIOT客户端
 @Input				pClient								: NBIOT客户端实例
					ATCmdStack							: NBIOT AT 协议栈
					NetNbiotStack							: NET NBIOT 协议栈
 @Return				void
**********************************************************************************************************/
void NBIOT_Client_Init(NBIOT_ClientsTypeDef* pClient, NBIOT_ATCmdTypeDef* ATCmdStack, NET_NBIOT_ClientsTypeDef* NetNbiotStack)
{
	pClient->Sendbuf										= NBIOT_SendBuf;
	pClient->Recvbuf										= NBIOT_RecvBuf;
	pClient->Sendbuf_size									= sizeof(NBIOT_SendBuf);
	pClient->Recvbuf_size									= sizeof(NBIOT_RecvBuf);
	pClient->Sendlen										= 0;
	pClient->Recvlen										= 0;
	pClient->DataProcessStack								= NBIOT_DataStack;
	pClient->DataProcessStack_size							= sizeof(NBIOT_DataStack);
	
	pClient->Command_Timeout_Msec								= NBIOT_COMMAND_TIMEOUT_MSEC;
	pClient->Command_Failure_Cnt								= NBIOT_COMMAND_FAILURE_CNT;
	
	/* 事件运行控制器 */
	pClient->DictateRunCtl.dictateCoapRANormalEnable				= false;
	pClient->DictateRunCtl.dictateEnable						= false;
	pClient->DictateRunCtl.dictateTimeoutSec					= 0;
	pClient->DictateRunCtl.dictateRebootFailureCnt				= 0;
	pClient->DictateRunCtl.dictateModuleCheckFailureCnt			= 0;
	pClient->DictateRunCtl.dictateParameterConfigFailureCnt		= 0;
	pClient->DictateRunCtl.dictateSimICCIDCheckFailureCnt			= 0;
	pClient->DictateRunCtl.dictateMiscEquipConfigFailureCnt		= 0;
	pClient->DictateRunCtl.dictateAttachCheckFailureCnt			= 0;
	pClient->DictateRunCtl.dictateAttachExecuteFailureCnt			= 0;
	pClient->DictateRunCtl.dictateAttachInquireFailureCnt			= 0;
	pClient->DictateRunCtl.dictatePatameterCheckOutFailureCnt		= 0;
	pClient->DictateRunCtl.dictateFullFunctionalityFailureCnt		= 0;
	pClient->DictateRunCtl.dictateMinimumFunctionalityFailureCnt	= 0;
	pClient->DictateRunCtl.dictateCDPServerCheckFailureCnt			= 0;
	pClient->DictateRunCtl.dictateCDPServerConfigFailureCnt		= 0;
	pClient->DictateRunCtl.dictateNbandModeCheckFailureCnt			= 0;
	pClient->DictateRunCtl.dictateNbandModeConfigFailureCnt		= 0;
	pClient->DictateRunCtl.dictateSendDataFailureCnt				= 0;
	pClient->DictateRunCtl.dictateRecvDataFailureCnt				= 0;
	pClient->DictateRunCtl.dictateSendDataRANormalFailureCnt		= 0;
	pClient->DictateRunCtl.dictateRecvDataRANormalFailureCnt		= 0;
	pClient->DictateRunCtl.dictateListenRunCtlFailureCnt			= 0;
	pClient->DictateRunCtl.dictateEvent						= HARDWARE_REBOOT;
	
	/* 事件运行监听器 */
#if NBCOAP_LISTEN_IDLE_TYPE == NBCOAP_LISTEN_IDLE_ENABLE
	pClient->ListenRunCtl.ListenEnterIdle.listenEnable				= false;
	pClient->ListenRunCtl.ListenEnterIdle.listenStatus				= false;
	pClient->ListenRunCtl.ListenEnterIdle.listenTimereachSec			= NBCOAP_LISTEN_ENTER_IDLE_SEC;
	pClient->ListenRunCtl.ListenEnterIdle.EventCtl.eventEnable			= false;
	pClient->ListenRunCtl.ListenEnterIdle.EventCtl.eventTimeoutSec		= 0;
	pClient->ListenRunCtl.ListenEnterIdle.EventCtl.eventFailureCnt		= 0;
#endif
#if NBCOAP_LISTEN_PARAMETER_TYPE == NBCOAP_LISTEN_PARAMETER_ENABLE
	pClient->ListenRunCtl.ListenEnterParameter.listenEnable			= false;
	pClient->ListenRunCtl.ListenEnterParameter.listenStatus			= false;
	pClient->ListenRunCtl.ListenEnterParameter.listenTimereachSec		= NBCOAP_LISTEN_ENTER_PARAMETER_SEC;
	pClient->ListenRunCtl.ListenEnterParameter.EventCtl.eventEnable		= false;
	pClient->ListenRunCtl.ListenEnterParameter.EventCtl.eventTimeoutSec	= 0;
	pClient->ListenRunCtl.ListenEnterParameter.EventCtl.eventFailureCnt	= 0;
#endif
	pClient->ListenRunCtl.listenEvent								= NBCOAP_LISTEN_DEFAULT_BOOTMODE;
	
	pClient->Registered										= false;
	pClient->NetStateIdentification							= false;
	pClient->ATCmdStack										= ATCmdStack;
	pClient->NetNbiotStack									= NetNbiotStack;
	
	Stm32_EventRunningTime_Init(&pClient->ConnectTimeMS);
	Stm32_EventRunningTime_Init(&pClient->IdleTimeMS);
}

/**********************************************************************************************************
 @Function			void NBIOT_COAP_RA_NORMAL_SET_STATE(NBIOT_ClientsTypeDef* pClient, bool enable)
 @Description			NBIOT_COAP_RA_NORMAL_SET_STATE			: 设置COAP发送RA包或Normal包状态
 @Input				pClient								: NBIOT客户端实例
					enable								: 状态
 @Return				void
**********************************************************************************************************/
void NBIOT_COAP_RA_NORMAL_SET_STATE(NBIOT_ClientsTypeDef* pClient, bool enable)
{
	pClient->DictateRunCtl.dictateCoapRANormalEnable				= enable;
}

/**********************************************************************************************************
 @Function			bool NBIOT_COAP_RA_NORMAL_GET_STATE(NBIOT_ClientsTypeDef* pClient)
 @Description			NBIOT_COAP_RA_NORMAL_GET_STATE			: 读取COAP发送RA包或Normal包状态
 @Input				pClient								: NBIOT客户端实例
 @Return				enable								: 状态
**********************************************************************************************************/
bool NBIOT_COAP_RA_NORMAL_GET_STATE(NBIOT_ClientsTypeDef* pClient)
{
	return pClient->DictateRunCtl.dictateCoapRANormalEnable;
}

/********************************************** END OF FLEE **********************************************/

