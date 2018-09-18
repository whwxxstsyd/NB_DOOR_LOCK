/**
  *********************************************************************************************************
  * @file    net_pcp_app.c
  * @author  Movebroad -- KK
  * @version V1.0
  * @date    2018-07-18
  * @brief   
  *********************************************************************************************************
  * @attention
  *
  *
  *
  *********************************************************************************************************
  */	

#include "net_pcp_app.h"
#include "platform_config.h"
#include "platform_map.h"
#include "string.h"
#include "radio_rf_app.h"




/**********************************************************************************************************
 @Function			static void PCP_NBIOT_DictateEvent_SetTime(PCP_ClientsTypeDef* pClient, unsigned int TimeoutSec)
 @Description			PCP_NBIOT_DictateEvent_SetTime		: 事件运行控制器注入时间(内部使用)
 @Input				pClient							: PCP客户端实例
					TimeoutSec						: 注入超时时间
 @Return				void
 @attention			事件运行之前判断是否需要注入时间
**********************************************************************************************************/
static void PCP_NBIOT_DictateEvent_SetTime(PCP_ClientsTypeDef* pClient, unsigned int TimeoutSec)
{
	Stm32_CalculagraphTypeDef dictateRunTime;
	
	/* It is the first time to execute */
	if (pClient->DictateRunCtl.dictateEnable != true) {
		pClient->DictateRunCtl.dictateEnable = true;
		pClient->DictateRunCtl.dictateTimeoutSec = TimeoutSec;
		
		Stm32_Calculagraph_CountdownSec(&dictateRunTime, pClient->DictateRunCtl.dictateTimeoutSec);
		pClient->DictateRunCtl.dictateRunTime = dictateRunTime;
	}
}

/**********************************************************************************************************
 @Function			PCP_StatusTypeDef NET_PCP_NBIOT_Event_StopMode(PCP_ClientsTypeDef* pClient)
 @Description			NET_PCP_NBIOT_Event_StopMode			: 停止模式
 @Input				pClient							: PCP客户端实例
 @Return				void
**********************************************************************************************************/
static PCP_StatusTypeDef NET_PCP_NBIOT_Event_StopMode(PCP_ClientsTypeDef* pClient)
{
	PCP_StatusTypeDef PCPStatus = PCP_OK;
	Stm32_CalculagraphTypeDef dictateRunTime;
	static unsigned char PcpSendMessageIndex;
	static unsigned char PcpRecvMessageIndex;
	
	/* It is the first time to execute */
	if (pClient->DictateRunCtl.dictateEnable != true) {
		pClient->DictateRunCtl.dictateEnable = true;
		pClient->DictateRunCtl.dictateTimeoutSec = 1800;
		Stm32_Calculagraph_CountdownSec(&dictateRunTime, pClient->DictateRunCtl.dictateTimeoutSec);
		pClient->DictateRunCtl.dictateRunTime = dictateRunTime;
		pClient->UpgradeExecution.upgradeStatus = PCP_UPGRADE_FAILED;
		/* Send Message Index */
		PcpSendMessageIndex = NET_PCP_Message_SendDataRear();
		/* Recv Message Index */
		PcpRecvMessageIndex = NET_PCP_Message_RecvDataRear();
#ifdef PCP_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("PCP Enter Stop");
#endif
	}
	
	if (Stm32_Calculagraph_IsExpiredSec(&pClient->DictateRunCtl.dictateRunTime) == true) {
		/* Dictate TimeOut */
		pClient->DictateRunCtl.dictateEnable = false;
		pClient->DictateRunCtl.dictateEvent = PCP_EVENT_READY;
		pClient->UpgradeExecution.upgradeStatus = PCP_UPGRADE_STANDBY;
		pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_COAP;
	}
	else {
		/* Dictate isn't TimeOut */
		if ((NET_PCP_Message_SendDataRear() != PcpSendMessageIndex) || (NET_PCP_Message_RecvDataRear() != PcpRecvMessageIndex)) {
			/* Have new pcp message need to execute */
			pClient->DictateRunCtl.dictateEnable = false;
			pClient->DictateRunCtl.dictateEvent = PCP_EVENT_READY;
			pClient->UpgradeExecution.upgradeStatus = PCP_UPGRADE_STANDBY;
			pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_COAP;
		}
		else {
			pClient->DictateRunCtl.dictateEvent = PCP_EVENT_STOP;
			pClient->UpgradeExecution.upgradeStatus = PCP_UPGRADE_STANDBY;
			pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_COAP;
		}
	}
	
	return PCPStatus;
}

/**********************************************************************************************************
 @Function			PCP_StatusTypeDef NET_PCP_NBIOT_Event_Initialized(PCP_ClientsTypeDef* pClient)
 @Description			NET_PCP_NBIOT_Event_Initialized		: 初始化PCP
 @Input				pClient							: PCP客户端实例
 @Return				void
**********************************************************************************************************/
static PCP_StatusTypeDef NET_PCP_NBIOT_Event_Initialized(PCP_ClientsTypeDef* pClient)
{
	PCP_StatusTypeDef PCPStatus = PCP_OK;
	
	memset((void*)pClient->Sendbuf, 0x0, pClient->Sendbuf_size);
	memset((void*)pClient->Recvbuf, 0x0, pClient->Recvbuf_size);
	
	pClient->Sendlen = 0;
	pClient->Recvlen = 0;
	memset((void*)pClient->DataProcessStack, 0x0, pClient->DataProcessStack_size);
	
	pClient->DictateRunCtl.dictateEnable = false;
	pClient->DictateRunCtl.dictateEvent = PCP_EVENT_READY;
	pClient->DictateRunCtl.dictateInitializedFailureCnt = 0;
	pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_COAP;
	
	return PCPStatus;
}

/**********************************************************************************************************
 @Function			PCP_StatusTypeDef NET_PCP_NBIOT_Event_Ready(PCP_ClientsTypeDef* pClient)
 @Description			NET_PCP_NBIOT_Event_Ready			: PCP准备中
 @Input				pClient							: PCP客户端实例
 @Return				void
**********************************************************************************************************/
static PCP_StatusTypeDef NET_PCP_NBIOT_Event_Ready(PCP_ClientsTypeDef* pClient)
{		
	PCP_StatusTypeDef PCPStatus = PCP_OK;
	
	/* 平台----->设备 */
	if (NET_PCP_Message_RecvDataisEmpty() != true) {											//接收到平台下发设备数据
		pClient->DictateRunCtl.dictateEnable = false;
		pClient->DictateRunCtl.dictateEvent = PCP_EVENT_EXECUTE;
		pClient->DictateRunCtl.dictateReadyFailureCnt = 0;
		pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_COAP;
	}
	/* 设备----->平台 */
	else if (NET_PCP_Message_SendDataisEmpty() != true) {									//需要发送数据到平台
		pClient->DictateRunCtl.dictateEnable = false;
		pClient->DictateRunCtl.dictateEvent = PCP_EVENT_FRAME_SEND;
		pClient->DictateRunCtl.dictateReadyFailureCnt = 0;
		pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_COAP;
	}
	/* 设备--ActiveUpload-->平台 */
	else if ((pClient->UpgradeExecution.upgradeStatus == PCP_UPGRADE_QUERYVERSION) || 
		    (pClient->UpgradeExecution.upgradeStatus == PCP_UPGRADE_DOWNLOAD) || 
		    (pClient->UpgradeExecution.upgradeStatus == PCP_UPGRADE_ASSEMBLE) || 
		    (pClient->UpgradeExecution.upgradeStatus == PCP_UPGRADE_INSTALL)) {					//需要主动发送数据到平台
		pClient->DictateRunCtl.dictateEnable = false;
		pClient->DictateRunCtl.dictateEvent = PCP_EVENT_ACTIVEUPLOAD;
		pClient->DictateRunCtl.dictateReadyFailureCnt = 0;
		pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_COAP;
	}
	/* 平台--x-->设备 */
	else {																						//等待数据交互
		pClient->DictateRunCtl.dictateEnable = false;
		pClient->DictateRunCtl.dictateEvent = PCP_EVENT_READY;
		pClient->DictateRunCtl.dictateReadyFailureCnt = 0;	
		pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_COAP;
	}
	
	return PCPStatus;
}

/**********************************************************************************************************
 @Function			PCP_StatusTypeDef NET_PCP_NBIOT_Event_Recv(PCP_ClientsTypeDef* pClient)
 @Description			NET_PCP_NBIOT_Event_Recv				: PCP接收数据
 @Input				pClient							: PCP客户端实例
 @Return				void
**********************************************************************************************************/
static PCP_StatusTypeDef NET_PCP_NBIOT_Event_Recv(PCP_ClientsTypeDef* pClient)
{
	PCP_StatusTypeDef PCPStatus = PCP_OK;
	
	PCP_NBIOT_DictateEvent_SetTime(pClient, 60);
	
	if (NBIOT_Neul_NBxx_CheckReadCONDataStatus(pClient->CoAPStack->NBIotStack) == NBIOT_OK) {
		/* Dictate execute is Success */
		pClient->DictateRunCtl.dictateEvent = PCP_EVENT_FRAME_RECV;
	}
	else {
		/* Dictate execute is Fail */
		if (Stm32_Calculagraph_IsExpiredSec(&pClient->DictateRunCtl.dictateRunTime) == true) {
			/* Dictate TimeOut */
			pClient->DictateRunCtl.dictateEnable = false;
			pClient->DictateRunCtl.dictateEvent = PCP_EVENT_READY;
			pClient->DictateRunCtl.dictateRecvFailureCnt++;
			pClient->CoAPStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
			pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_COAP;
			if (pClient->DictateRunCtl.dictateRecvFailureCnt > 3) {
				pClient->DictateRunCtl.dictateRecvFailureCnt = 0;
				pClient->DictateRunCtl.dictateEvent = PCP_EVENT_STOP;
			}
		}
		else {
			/* Dictate isn't TimeOut */
			pClient->DictateRunCtl.dictateEvent = PCP_EVENT_FRAME_RECV;
			pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_PCP;
		}
		goto exit;
	}
	
	if (pClient->CoAPStack->NBIotStack->Parameter.condatastate == SendSussess) {
		/* Send Data To Server Success */
		if (NBIOT_Neul_NBxx_QueryReadMessageCOAPPayload(pClient->CoAPStack->NBIotStack) == NBIOT_OK) {
			/* Dictate execute is Success */
			pClient->DictateRunCtl.dictateEvent = PCP_EVENT_FRAME_RECV;
		}
		else {
			/* Dictate execute is Fail */
			if (Stm32_Calculagraph_IsExpiredSec(&pClient->DictateRunCtl.dictateRunTime) == true) {
				/* Dictate TimeOut */
				pClient->DictateRunCtl.dictateEnable = false;
				pClient->DictateRunCtl.dictateEvent = PCP_EVENT_READY;
				pClient->DictateRunCtl.dictateRecvFailureCnt++;
				pClient->CoAPStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
				pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_COAP;
				if (pClient->DictateRunCtl.dictateRecvFailureCnt > 3) {
					pClient->DictateRunCtl.dictateRecvFailureCnt = 0;
					pClient->DictateRunCtl.dictateEvent = PCP_EVENT_STOP;
				}
			}
			else {
				/* Dictate isn't TimeOut */
				pClient->DictateRunCtl.dictateEvent = PCP_EVENT_FRAME_RECV;
				pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_PCP;
			}
			goto exit;
		}
		
		/* 检查是否有下行数据 */
		if (pClient->CoAPStack->NBIotStack->Parameter.coapReadMessage.buffered != 0) {
			/* Has Data Need Receive */
			for (int index = 0; index < pClient->CoAPStack->NBIotStack->Parameter.coapReadMessage.buffered; index++) {
				/* 读取负载数据 */
				if ((PCPStatus = pClient->CoAPStack->Read(pClient->CoAPStack, (char *)pClient->Recvbuf, (u16 *)&pClient->Recvlen)) == PCP_OK) {
					/* Dictate execute is Success */
					pClient->DictateRunCtl.dictateEvent = pClient->DictateRunCtl.dictateEvent;
				}
				else {
					/* Dictate execute is Fail */
					if (Stm32_Calculagraph_IsExpiredSec(&pClient->DictateRunCtl.dictateRunTime) == true) {
						/* Dictate TimeOut */
						pClient->DictateRunCtl.dictateEnable = false;
						pClient->DictateRunCtl.dictateEvent = PCP_EVENT_READY;
						pClient->DictateRunCtl.dictateRecvFailureCnt++;
						pClient->CoAPStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
						pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_COAP;
						if (pClient->DictateRunCtl.dictateRecvFailureCnt > 3) {
							pClient->DictateRunCtl.dictateRecvFailureCnt = 0;
							pClient->DictateRunCtl.dictateEvent = PCP_EVENT_STOP;
						}
					}
					else {
						/* Dictate isn't TimeOut */
						pClient->DictateRunCtl.dictateEvent = PCP_EVENT_FRAME_RECV;
						pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_PCP;
					}
					goto exit;
				}
				
				NET_PCP_Message_RecvDataEnqueue(pClient->Recvbuf, pClient->Recvlen);
#ifdef PCP_DEBUG_LOG_RF_PRINT
				Radio_Trf_Debug_Printf_Level2("PCP Recv Data Ok");
#endif
			}
		}
		
		NET_PCP_Message_SendDataOffSet();
		pClient->DictateRunCtl.dictateEnable = false;
		pClient->DictateRunCtl.dictateEvent = PCP_EVENT_READY;
		pClient->DictateRunCtl.dictateRecvFailureCnt = 0;
		pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_PCP;
		
	#if NBCOAP_LISTEN_IDLE_TYPE == NBCOAP_LISTEN_IDLE_ENABLE
		NET_COAP_NBIOT_Listen_Enable_EnterIdleMode(pClient->CoAPStack->NBIotStack);
	#endif
	#if NBCOAP_LISTEN_PARAMETER_TYPE == NBCOAP_LISTEN_PARAMETER_ENABLE
		NET_COAP_NBIOT_Listen_Enable_EnterParameter(pClient->CoAPStack->NBIotStack);
	#endif
		
#ifdef PCP_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("PCP Send Ok");
#endif
	}
	else {
		/* Not yet Send Data Success */
		if (Stm32_Calculagraph_IsExpiredSec(&pClient->DictateRunCtl.dictateRunTime) == true) {
			/* Dictate TimeOut */
			pClient->DictateRunCtl.dictateEnable = false;
			pClient->DictateRunCtl.dictateEvent = PCP_EVENT_READY;
			pClient->DictateRunCtl.dictateRecvFailureCnt++;
			pClient->CoAPStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
			pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_COAP;
			if (pClient->DictateRunCtl.dictateRecvFailureCnt > 3) {
				pClient->DictateRunCtl.dictateRecvFailureCnt = 0;
				pClient->DictateRunCtl.dictateEvent = PCP_EVENT_STOP;
			}
		}
		else {
			/* Dictate isn't TimeOut */
			pClient->DictateRunCtl.dictateEvent = PCP_EVENT_FRAME_RECV;
			pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_PCP;
#ifdef PCP_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("PCP Wait Send Ok");
#endif
		}
	}
	
exit:
	return PCPStatus;
}

/**********************************************************************************************************
 @Function			PCP_StatusTypeDef NET_PCP_NBIOT_Event_Send(PCP_ClientsTypeDef* pClient)
 @Description			NET_PCP_NBIOT_Event_Send				: PCP发送数据
 @Input				pClient							: PCP客户端实例
 @Return				void
**********************************************************************************************************/
static PCP_StatusTypeDef NET_PCP_NBIOT_Event_Send(PCP_ClientsTypeDef* pClient)
{
	PCP_StatusTypeDef PCPStatus = PCP_OK;
	NBIOT_StatusTypeDef NBStatus = NBIOT_OK;
	
	PCP_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	/* Data packets need to be sent*/
	if (NET_PCP_Message_SendDataDequeue(pClient->Sendbuf, (unsigned short *)&pClient->Sendlen) == true) {
		/* Connect Check */
		if ((NBStatus = NBIOT_Neul_NBxx_CheckReadAttachOrDetach(pClient->CoAPStack->NBIotStack)) == NBIOT_OK) {
			/* Dictate execute is Success */			
			pClient->DictateRunCtl.dictateEvent = PCP_EVENT_FRAME_SEND;
#ifdef PCP_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("PCP CGATT %d", pClient->CoAPStack->NBIotStack->Parameter.netstate);
#endif
		}
		else {
			/* Dictate execute is Fail */
			if (Stm32_Calculagraph_IsExpiredSec(&pClient->DictateRunCtl.dictateRunTime) == true) {
				/* Dictate TimeOut */
				pClient->DictateRunCtl.dictateEnable = false;
				pClient->DictateRunCtl.dictateEvent = PCP_EVENT_READY;
				pClient->DictateRunCtl.dictateSendFailureCnt++;
				pClient->CoAPStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
				pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_COAP;
				if (pClient->DictateRunCtl.dictateSendFailureCnt > 3) {
					pClient->DictateRunCtl.dictateSendFailureCnt = 0;
					pClient->DictateRunCtl.dictateEvent = PCP_EVENT_STOP;
				}
			}
			else {
				/* Dictate isn't TimeOut */
				pClient->DictateRunCtl.dictateEvent = PCP_EVENT_FRAME_SEND;
				pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_PCP;
			}
#ifdef PCP_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("PCP CGATT %d Fail ECde %d", pClient->CoAPStack->NBIotStack->Parameter.netstate, NBStatus);
#endif
			goto exit;
		}
		
		if (pClient->CoAPStack->NBIotStack->Parameter.netstate != Attach) {
			if (Stm32_Calculagraph_IsExpiredSec(&pClient->DictateRunCtl.dictateRunTime) == true) {
				pClient->DictateRunCtl.dictateEnable = false;
				pClient->DictateRunCtl.dictateEvent = PCP_EVENT_READY;
				pClient->DictateRunCtl.dictateSendFailureCnt++;
				pClient->CoAPStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
				pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_COAP;
				if (pClient->DictateRunCtl.dictateSendFailureCnt > 3) {
					pClient->DictateRunCtl.dictateSendFailureCnt = 0;
					pClient->DictateRunCtl.dictateEvent = PCP_EVENT_STOP;
				}
			}
			else {
				pClient->DictateRunCtl.dictateEvent = PCP_EVENT_FRAME_SEND;
				pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_PCP;
			}
			goto exit;
		}
		
		/* 发送负载数据 */
		if ((PCPStatus = pClient->CoAPStack->Write(pClient->CoAPStack, (char *)pClient->Sendbuf, pClient->Sendlen)) != PCP_OK) {
			/* Dictate execute is Fail */
			if (Stm32_Calculagraph_IsExpiredSec(&pClient->DictateRunCtl.dictateRunTime) == true) {
				/* Dictate TimeOut */
				pClient->DictateRunCtl.dictateEnable = false;
				pClient->DictateRunCtl.dictateEvent = PCP_EVENT_READY;
				pClient->DictateRunCtl.dictateSendFailureCnt++;
				pClient->CoAPStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
				pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_COAP;
				if (pClient->DictateRunCtl.dictateSendFailureCnt > 3) {
					pClient->DictateRunCtl.dictateSendFailureCnt = 0;
					pClient->DictateRunCtl.dictateEvent = PCP_EVENT_STOP;
				}
			}
			else {
				/* Dictate isn't TimeOut */
				pClient->DictateRunCtl.dictateEvent = PCP_EVENT_FRAME_SEND;
				pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_PCP;
			}
#ifdef PCP_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("PCP Send Payload Fail ECde %d", PCPStatus);
#endif
			goto exit;
		}
		else {
			/* Send Data Success */
			pClient->DictateRunCtl.dictateEnable = false;
			pClient->DictateRunCtl.dictateEvent = PCP_EVENT_FRAME_RECV;
			pClient->DictateRunCtl.dictateSendFailureCnt = 0;
			pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_PCP;
#ifdef PCP_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("PCP Send Payload Ok");
#endif
		}
	}
	/* No packets need to be sent */
	else {
		pClient->DictateRunCtl.dictateEnable = false;
		pClient->DictateRunCtl.dictateEvent = PCP_EVENT_READY;
		pClient->DictateRunCtl.dictateSendFailureCnt = 0;
		pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_PCP;
	}
	
exit:
	return PCPStatus;
}

/**********************************************************************************************************
 @Function			PCP_StatusTypeDef NET_PCP_NBIOT_Event_Execute(PCP_ClientsTypeDef* pClient)
 @Description			NET_PCP_NBIOT_Event_Execute			: PCP数据处理
 @Input				pClient							: PCP客户端实例
 @Return				void
**********************************************************************************************************/
static PCP_StatusTypeDef NET_PCP_NBIOT_Event_Execute(PCP_ClientsTypeDef* pClient)
{
	PCP_StatusTypeDef PCPStatus = PCP_OK;
	
	/* 读取接收区数据 */
	if (NET_PCP_Message_RecvDataDequeue(pClient->Recvbuf, (unsigned short *)&pClient->Recvlen) != true) {
		PCPStatus = PCP_Frame_None;
		pClient->DictateRunCtl.dictateEnable = false;
		pClient->DictateRunCtl.dictateEvent = PCP_EVENT_READY;
		pClient->DictateRunCtl.dictateExecuteFailureCnt = 0;
		pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_COAP;
		goto exit;
	}
	
	/* 检测数据帧并校验数据帧 */
	if ((PCPStatus = PCP_Func_FrameCheck(pClient->Recvbuf, pClient->Recvlen)) != PCP_OK) {
		NET_PCP_Message_RecvDataOffSet();
		pClient->DictateRunCtl.dictateEnable = false;
		pClient->DictateRunCtl.dictateEvent = PCP_EVENT_READY;
		pClient->DictateRunCtl.dictateExecuteFailureCnt = 0;
		pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_COAP;
		goto exit;
	}
	
	/* 判断不同消息码处理不同命令 */
	if ((PCPStatus = PCP_Func_SelectMessageExecuteCmd(pClient)) != PCP_OK) {
		NET_PCP_Message_RecvDataOffSet();
		pClient->DictateRunCtl.dictateEnable = false;
		pClient->DictateRunCtl.dictateEvent = PCP_EVENT_READY;
		pClient->DictateRunCtl.dictateExecuteFailureCnt = 0;
		pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_COAP;
		goto exit;
	}
	
	/* 命令处理完成 */
	pClient->DictateRunCtl.dictateEnable = false;
	pClient->DictateRunCtl.dictateEvent = PCP_EVENT_READY;
	pClient->DictateRunCtl.dictateExecuteFailureCnt = 0;
	pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_COAP;
	
exit:
	return PCPStatus;
}
	
/**********************************************************************************************************
 @Function			PCP_StatusTypeDef NET_PCP_NBIOT_Event_ActiveUpload(PCP_ClientsTypeDef* pClient)
 @Description			NET_PCP_NBIOT_Event_ActiveUpload		: PCP主动上传数据
 @Input				pClient							: PCP客户端实例
 @Return				void
**********************************************************************************************************/
static PCP_StatusTypeDef NET_PCP_NBIOT_Event_ActiveUpload(PCP_ClientsTypeDef* pClient)
{
	PCP_StatusTypeDef PCPStatus = PCP_OK;
	
	/* 判断不同主动上传数据码处理不同命令 */
	if ((PCPStatus = PCP_Func_SelectUpgradeStatusExecuteCmd(pClient)) != PCP_OK) {
		pClient->DictateRunCtl.dictateEnable = false;
		pClient->DictateRunCtl.dictateEvent = PCP_EVENT_READY;
		pClient->DictateRunCtl.dictateActiveUploadFailureCnt = 0;
		pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_COAP;
		goto exit;
	}
	
	/* 命令处理完成 */
	pClient->DictateRunCtl.dictateEnable = false;
	pClient->DictateRunCtl.dictateEvent = PCP_EVENT_READY;
	pClient->DictateRunCtl.dictateActiveUploadFailureCnt = 0;
	pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_COAP;
	
exit:
	return PCPStatus;
}






PCP_ClientsTypeDef			PCPClientHandler;				//PCP Clinet Handler

	

/**********************************************************************************************************
 @Function			void NET_PCP_APP_PollExecution(PCP_ClientsTypeDef* pClient)
 @Description			PCP_APP_PollExecution				: PCP逻辑处理
 @Input				pClient							: PCP客户端实例
 @Return				void
**********************************************************************************************************/
void NET_PCP_APP_PollExecution(PCP_ClientsTypeDef* pClient)
{
	switch (pClient->DictateRunCtl.dictateEvent)
	{	
		case PCP_EVENT_STOP:
			NET_PCP_NBIOT_Event_StopMode(pClient);
			break;	
		
		case PCP_EVENT_INITIALIZED:
			NET_PCP_NBIOT_Event_Initialized(pClient);
			break;
		
		case PCP_EVENT_READY:
			NET_PCP_NBIOT_Event_Ready(pClient);
			break;
		
		case PCP_EVENT_FRAME_RECV:
			NET_PCP_NBIOT_Event_Recv(pClient);
			break;
		
		case PCP_EVENT_FRAME_SEND:
			NET_PCP_NBIOT_Event_Send(pClient);
			break;
		
		case PCP_EVENT_EXECUTE:
			NET_PCP_NBIOT_Event_Execute(pClient);
			break;
		
		case PCP_EVENT_ACTIVEUPLOAD:
			NET_PCP_NBIOT_Event_ActiveUpload(pClient);
			break;
	}
}






/********************************************** END OF FLEE **********************************************/

