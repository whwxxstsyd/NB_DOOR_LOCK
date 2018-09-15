/**
  *********************************************************************************************************
  * @file    net_mqttsn_app.c
  * @author  Movebroad -- KK
  * @version V1.0
  * @date    2018-03-12
  * @brief   
  *********************************************************************************************************
  * @attention
  *
  *
  *
  *********************************************************************************************************
  */

#include "net_mqttsn_app.h"
#include "platform_config.h"
#include "platform_map.h"
#include "hal_rtc.h"
#include "hal_beep.h"
#include "radar_api.h"
#include "hal_qmc5883l.h"
#include "string.h"
#include "radio_rf_app.h"

/**********************************************************************************************************
 @Function			void NET_MQTTSN_APP_PollExecution(MQTTSN_ClientsTypeDef* pClient)
 @Description			MQTTSN_APP_PollExecution				: MqttSN逻辑处理
 @Input				pClient							: MqttSN客户端实例
 @Return				void
**********************************************************************************************************/
void NET_MQTTSN_APP_PollExecution(MQTTSN_ClientsTypeDef* pClient)
{
	switch (pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent)
	{
	case STOP_MODE:
		NET_MQTTSN_NBIOT_Event_StopMode(pClient);
		break;
		
	case HARDWARE_REBOOT:
		NET_MQTTSN_NBIOT_Event_HardwareReboot(pClient);
		break;
	
	case REPORT_ERROE:
		NET_MQTTSN_NBIOT_Event_ReportError(pClient);
		break;
	
	case MODULE_CHECK:
		NET_MQTTSN_NBIOT_Event_ModuleCheck(pClient);
		break;
	
	case PARAMETER_CONFIG:
		NET_MQTTSN_NBIOT_Event_ParameterConfig(pClient);
		break;
	
	case ICCID_CHECK:
		NET_MQTTSN_NBIOT_Event_SimICCIDCheck(pClient);
		break;
	
	case MISC_EQUIP_CONFIG:
		pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
		break;
	
	case ATTACH_CHECK:
		NET_MQTTSN_NBIOT_Event_AttachCheck(pClient);
		break;
	
	case ATTACH_EXECUTE:
		NET_MQTTSN_NBIOT_Event_AttachExecute(pClient);
		break;
	
	case ATTACH_INQUIRE:
		NET_MQTTSN_NBIOT_Event_AttachInquire(pClient);
		break;
	
	case PARAMETER_CHECKOUT:
		NET_MQTTSN_NBIOT_Event_ParameterCheckOut(pClient);
		break;
	
	case MINIMUM_FUNCTIONALITY:
		NET_MQTTSN_NBIOT_Event_MinimumFunctionality(pClient);
		break;
	
	case FULL_FUNCTIONALITY:
		NET_MQTTSN_NBIOT_Event_FullFunctionality(pClient);
		break;
	
	case CDP_SERVER_CHECK:
		pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
		break;
	
	case CDP_SERVER_CONFIG:
		pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
		break;
	
	case NBAND_MODE_CHECK:
		NET_MQTTSN_NBIOT_Event_NbandModeCheck(pClient);
		break;
	
	case NBAND_MODE_CONFIG:
		NET_MQTTSN_NBIOT_Event_NbandModeConfig(pClient);
		break;
	
	case SEND_DATA:
		pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
		break;
	
	case RECV_DATA:
		pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
		break;
	
	case SEND_DATA_RA_NORMAL:
		pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
		break;
	
	case RECV_DATA_RA_NORMAL:
		pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
		break;
	
	case EXECUT_DOWNLINK_DATA:
		pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
		break;
	
	case MQTTSN_PROCESS_STACK:
		NET_MQTTSN_APP_ProcessExecution(pClient);
		break;
	
	case DNS_PROCESS_STACK:
		pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
		break;
	
	case ONENET_PROCESS_STACK:
		pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
		break;
	
	case LISTEN_RUN_CTL:
		pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
		break;
	
	default :
		pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
		break;
	}
}

/**********************************************************************************************************
 @Function			void NET_MQTTSN_APP_ProcessExecution(MQTTSN_ClientsTypeDef* pClient)
 @Description			NET_MQTTSN_APP_ProcessExecution		: MqttSN协议逻辑处理
 @Input				pClient							: MqttSN客户端实例
 @Return				void
**********************************************************************************************************/
void NET_MQTTSN_APP_ProcessExecution(MQTTSN_ClientsTypeDef* pClient)
{
	switch (pClient->SubState)
	{
	case MQTTSN_SUBSTATE_INIT:
		NET_MQTTSN_Event_Init(pClient);
		break;
	
	case MQTTSN_SUBSTATE_DISCONNECT:
		NET_MQTTSN_Event_Disconnect(pClient);
		break;
	
	case MQTTSN_SUBSTATE_ACTIVE:
		NET_MQTTSN_Event_Active(pClient);
		break;
	
	case MQTTSN_SUBSTATE_SLEEP:
		NET_MQTTSN_Event_Sleep(pClient);
		break;
	
	case MQTTSN_SUBSTATE_AWAKE:
		NET_MQTTSN_Event_Aweak(pClient);
		break;
	
	case MQTTSN_SUBSTATE_LOST:
		NET_MQTTSN_Event_Lost(pClient);
		break;
	}
}

/**********************************************************************************************************
 @Function			static void MQTTSN_NBIOT_DictateEvent_SetTime(MQTTSN_ClientsTypeDef* pClient, unsigned int TimeoutSec)
 @Description			MQTTSN_NBIOT_DictateEvent_SetTime		: 事件运行控制器注入时间(内部使用)
 @Input				pClient							: MQTTSN客户端实例
					TimeoutSec						: 注入超时时间
 @Return				void
 @attention			事件运行之前判断是否需要注入时间
**********************************************************************************************************/
static void MQTTSN_NBIOT_DictateEvent_SetTime(MQTTSN_ClientsTypeDef* pClient, unsigned int TimeoutSec)
{
	Stm32_CalculagraphTypeDef dictateRunTime;
	
	/* It is the first time to execute */
	if (pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEnable != true) {
		pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEnable = true;
		pClient->SocketStack->NBIotStack->DictateRunCtl.dictateTimeoutSec = TimeoutSec;
		Stm32_Calculagraph_CountdownSec(&dictateRunTime, pClient->SocketStack->NBIotStack->DictateRunCtl.dictateTimeoutSec);
		pClient->SocketStack->NBIotStack->DictateRunCtl.dictateRunTime = dictateRunTime;
	}
}

/**********************************************************************************************************
 @Function			static void MQTTSN_DictateEvent_SetTime(MQTTSN_ClientsTypeDef* pClient, unsigned int TimeoutSec)
 @Description			MQTTSN_DictateEvent_SetTime			: 事件运行控制器注入时间(内部使用)
 @Input				pClient							: MQTTSN客户端实例
					TimeoutSec						: 注入超时时间
 @Return				void
 @attention			事件运行之前判断是否需要注入时间
**********************************************************************************************************/
static void MQTTSN_DictateEvent_SetTime(MQTTSN_ClientsTypeDef* pClient, unsigned int TimeoutSec)
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

static unsigned char* MQTTSN_NBIOT_GetDictateFailureCnt(MQTTSN_ClientsTypeDef* pClient, NBIOT_DictateEventTypeDef dictateNoTimeOut)
{
	unsigned char* dictateFailureCnt;
	
	switch (dictateNoTimeOut)
	{
	case HARDWARE_REBOOT:
		dictateFailureCnt = &pClient->SocketStack->NBIotStack->DictateRunCtl.dictateRebootFailureCnt;
		break;
	
	case REPORT_ERROE:
		dictateFailureCnt = &pClient->SocketStack->NBIotStack->DictateRunCtl.dictateReportErrorFailureCnt;
		break;
	
	case MODULE_CHECK:
		dictateFailureCnt = &pClient->SocketStack->NBIotStack->DictateRunCtl.dictateModuleCheckFailureCnt;
		break;
	
	case PARAMETER_CONFIG:
		dictateFailureCnt = &pClient->SocketStack->NBIotStack->DictateRunCtl.dictateParameterConfigFailureCnt;
		break;
	
	case ICCID_CHECK:
		dictateFailureCnt = &pClient->SocketStack->NBIotStack->DictateRunCtl.dictateSimICCIDCheckFailureCnt;
		break;
	
	case ATTACH_CHECK:
		dictateFailureCnt = &pClient->SocketStack->NBIotStack->DictateRunCtl.dictateAttachCheckFailureCnt;
		break;
	
	case ATTACH_EXECUTE:
		dictateFailureCnt = &pClient->SocketStack->NBIotStack->DictateRunCtl.dictateAttachExecuteFailureCnt;
		break;
	
	case ATTACH_INQUIRE:
		dictateFailureCnt = &pClient->SocketStack->NBIotStack->DictateRunCtl.dictateAttachInquireFailureCnt;
		break;
	
	case PARAMETER_CHECKOUT:
		dictateFailureCnt = &pClient->SocketStack->NBIotStack->DictateRunCtl.dictatePatameterCheckOutFailureCnt;
		break;
	
	case MINIMUM_FUNCTIONALITY:
		dictateFailureCnt = &pClient->SocketStack->NBIotStack->DictateRunCtl.dictateMinimumFunctionalityFailureCnt;
		break;
	
	case FULL_FUNCTIONALITY:
		dictateFailureCnt = &pClient->SocketStack->NBIotStack->DictateRunCtl.dictateFullFunctionalityFailureCnt;
		break;
	
	case NBAND_MODE_CHECK:
		dictateFailureCnt = &pClient->SocketStack->NBIotStack->DictateRunCtl.dictateNbandModeCheckFailureCnt;
		break;
	
	case NBAND_MODE_CONFIG:
		dictateFailureCnt = &pClient->SocketStack->NBIotStack->DictateRunCtl.dictateNbandModeConfigFailureCnt;
		break;
	
	default :
		dictateFailureCnt = &pClient->SocketStack->NBIotStack->DictateRunCtl.dictateRebootFailureCnt;
		break;
	}
	
	return dictateFailureCnt;
}

/**********************************************************************************************************
 @Function			static void MQTTSN_NBIOT_DictateEvent_FailExecute(MQTTSN_ClientsTypeDef* pClient, NBIOT_DictateEventTypeDef dictateTimeOut, \
																					  NBIOT_DictateEventTypeDef dictateFail, \
																					  NBIOT_DictateEventTypeDef dictateNoTimeOut)
 @Description			MQTTSN_NBIOT_DictateEvent_FailExecute	: 事件运行控制器出错执行(内部使用)
 @Input				pClient							: MQTTSN客户端实例
					dictateTimeOut						: 事假处理错误超时
					dictateFail						: 事件处理错误次数溢出
					dictateNoTimeOut					: 事假处理错误未超时
 @Return				void
**********************************************************************************************************/
static void MQTTSN_NBIOT_DictateEvent_FailExecute(MQTTSN_ClientsTypeDef* pClient, NBIOT_DictateEventTypeDef dictateTimeOut, \
																  NBIOT_DictateEventTypeDef dictateFail, \
																  NBIOT_DictateEventTypeDef dictateNoTimeOut)
{
	unsigned char* dictateFailureCnt;
	
	dictateFailureCnt = MQTTSN_NBIOT_GetDictateFailureCnt(pClient, dictateNoTimeOut);
	
	if (Stm32_Calculagraph_IsExpiredSec(&pClient->SocketStack->NBIotStack->DictateRunCtl.dictateRunTime) == true) {
		/* Dictate TimeOut */
		pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEnable = false;
		pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = dictateTimeOut;
		*dictateFailureCnt += 1;
		if (*dictateFailureCnt > 3) {
			*dictateFailureCnt = 0;
			pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = dictateFail;
		}
	}
	else {
		/* Dictate isn't TimeOut */
		pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = dictateNoTimeOut;
	}
}

/**********************************************************************************************************
 @Function			static void MQTTSN_NBIOT_DictateEvent_SuccessExecute(MQTTSN_ClientsTypeDef* pClient, NBIOT_DictateEventTypeDef dictateSuccess, \
																						NBIOT_DictateEventTypeDef dictateNoTimeOut)
 @Description			MQTTSN_NBIOT_DictateEvent_SuccessExecute: 事件运行控制器正确执行(内部使用)
 @Input				pClient							: MQTTSN客户端实例
					dictateSuccess						: 事假处理正确
					dictateNoTimeOut					: 事假处理错误未超时
 @Return				void
**********************************************************************************************************/
static void MQTTSN_NBIOT_DictateEvent_SuccessExecute(MQTTSN_ClientsTypeDef* pClient, NBIOT_DictateEventTypeDef dictateSuccess, \
																	NBIOT_DictateEventTypeDef dictateNoTimeOut)
{
	unsigned char* dictateFailureCnt;
	
	dictateFailureCnt = MQTTSN_NBIOT_GetDictateFailureCnt(pClient, dictateNoTimeOut);
	
	pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEnable = false;
	pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = dictateSuccess;
	*dictateFailureCnt = 0;
}

/**********************************************************************************************************
 @Function			void NET_MQTTSN_NBIOT_Event_StopMode(MQTTSN_ClientsTypeDef* pClient)
 @Description			NET_MQTTSN_NBIOT_Event_StopMode		: 停止模式
 @Input				pClient							: MqttSN客户端实例
 @Return				void
 @attention			当30分钟或有数据需要发送时退出停止模式尝试重启NB,开启NB运行
**********************************************************************************************************/
void NET_MQTTSN_NBIOT_Event_StopMode(MQTTSN_ClientsTypeDef* pClient)
{
	Stm32_CalculagraphTypeDef dictateRunTime;
#if MQTTSN_MSG_VERSION_STREAM_TYPE == MQTTSN_MSG_VERSION_JSON_STREAM
	static unsigned char MqttSNStatusBasicIndex;
	static unsigned char MqttSNStatusExtendIndex;
	static unsigned char MqttSNInfoWorkIndex;
	static unsigned char MqttSNInfoBasicIndex;
	static unsigned char MqttSNInfoDynamicIndex;
	static unsigned char MqttSNInfoRadarIndex;
	static unsigned char MqttSNInfoResponseIndex;
#endif
#if MQTTSN_MSG_VERSION_STREAM_TYPE == MQTTSN_MSG_VERSION_BYTE_STREAM
	static unsigned char MqttSNByteStreamIndex;
#endif
	
	/* It is the first time to execute */
	if (pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEnable != true) {
		pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEnable = true;
		pClient->SocketStack->NBIotStack->DictateRunCtl.dictateTimeoutSec = 1800;
		Stm32_Calculagraph_CountdownSec(&dictateRunTime, pClient->SocketStack->NBIotStack->DictateRunCtl.dictateTimeoutSec);
		pClient->SocketStack->NBIotStack->DictateRunCtl.dictateRunTime = dictateRunTime;
		/* NBIOT Module Poweroff */
		NBIOT_Neul_NBxx_HardwarePoweroff(pClient->SocketStack->NBIotStack);
		/* Init Message Index */
#if MQTTSN_MSG_VERSION_STREAM_TYPE == MQTTSN_MSG_VERSION_JSON_STREAM
		MqttSNStatusBasicIndex = NET_MqttSN_Message_StatusBasicRear();
		MqttSNStatusExtendIndex = NET_MqttSN_Message_StatusExtendRear();
		MqttSNInfoWorkIndex = NET_MqttSN_Message_InfoWorkRear();
		MqttSNInfoBasicIndex = NET_MqttSN_Message_InfoBasicRear();
		MqttSNInfoDynamicIndex = NET_MqttSN_Message_InfoDynamicRear();
		MqttSNInfoRadarIndex = NET_MqttSN_Message_InfoRadarRear();
		MqttSNInfoResponseIndex = NET_MqttSN_Message_InfoResponseRear();
#endif
#if MQTTSN_MSG_VERSION_STREAM_TYPE == MQTTSN_MSG_VERSION_BYTE_STREAM
		MqttSNByteStreamIndex = NET_MqttSN_Message_SendDataRear();
#endif
	}
	
	if (Stm32_Calculagraph_IsExpiredSec(&pClient->SocketStack->NBIotStack->DictateRunCtl.dictateRunTime) == true) {
		/* Dictate TimeOut */
		pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEnable = false;
		pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
		pClient->SubState = MQTTSN_SUBSTATE_INIT;
		pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_DNS;
	}
	else {
		/* Dictate isn't TimeOut */
#if MQTTSN_MSG_VERSION_STREAM_TYPE == MQTTSN_MSG_VERSION_JSON_STREAM
		if ( (NET_MqttSN_Message_StatusBasicRear() != MqttSNStatusBasicIndex) || 
			(NET_MqttSN_Message_StatusExtendRear() != MqttSNStatusExtendIndex) ||
			(NET_MqttSN_Message_InfoWorkRear() != MqttSNInfoWorkIndex) ||
			(NET_MqttSN_Message_InfoBasicRear() != MqttSNInfoBasicIndex) ||
			(NET_MqttSN_Message_InfoDynamicRear() != MqttSNInfoDynamicIndex) ||
			(NET_MqttSN_Message_InfoRadarRear() != MqttSNInfoRadarIndex) ||
			(NET_MqttSN_Message_InfoResponseRear() != MqttSNInfoResponseIndex) ) {
#endif
#if MQTTSN_MSG_VERSION_STREAM_TYPE == MQTTSN_MSG_VERSION_BYTE_STREAM
		if ( NET_MqttSN_Message_SendDataRear() != MqttSNByteStreamIndex ) {
#endif
			pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEnable = false;
			pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
			pClient->SubState = MQTTSN_SUBSTATE_INIT;
			pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_DNS;
		}
		else {
			pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = STOP_MODE;
			pClient->SubState = MQTTSN_SUBSTATE_INIT;
			pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_MQTTSN;
		}
	}
}

/**********************************************************************************************************
 @Function			void NET_MQTTSN_NBIOT_Event_HardwareReboot(MQTTSN_ClientsTypeDef* pClient)
 @Description			NET_MQTTSN_NBIOT_Event_HardwareReboot	: 硬件重启
 @Input				pClient							: MqttSN客户端实例
 @Return				void
**********************************************************************************************************/
void NET_MQTTSN_NBIOT_Event_HardwareReboot(MQTTSN_ClientsTypeDef* pClient)
{
	MQTTSN_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if (NBIOT_Neul_NBxx_HardwareReboot(pClient->SocketStack->NBIotStack, 8000) == NBIOT_OK) {
		/* Dictate execute is Success */
#if NBIOT_PRINT_ERROR_CODE_TYPE
		MQTTSN_NBIOT_DictateEvent_SuccessExecute(pClient, REPORT_ERROE, HARDWARE_REBOOT);
#else
		MQTTSN_NBIOT_DictateEvent_SuccessExecute(pClient, MODULE_CHECK, HARDWARE_REBOOT);
#endif
		
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT_BEFORE
		Radio_Trf_Debug_Printf_Level2("NB HDRBT Ok, Baud:%d", NBIOTBaudRate.Baud);
#endif
	}
	else {
		/* Dictate execute is Fail */
		MQTTSN_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, HARDWARE_REBOOT);
		
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT_BEFORE
		Radio_Trf_Debug_Printf_Level2("NB HDRBT Fail");
#endif
	}
}

/**********************************************************************************************************
 @Function			void NET_MQTTSN_NBIOT_Event_ReportError(MQTTSN_ClientsTypeDef* pClient)
 @Description			NET_MQTTSN_NBIOT_Event_ReportError		: 错误码输出
 @Input				pClient							: MqttSN客户端实例
 @Return				void
**********************************************************************************************************/
void NET_MQTTSN_NBIOT_Event_ReportError(MQTTSN_ClientsTypeDef* pClient)
{
	MQTTSN_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if ((NBIOT_Neul_NBxx_SetReportTerminationError(pClient->SocketStack->NBIotStack, CMEEnable) == NBIOT_OK)) {
		/* Dictate execute is Success */
		MQTTSN_NBIOT_DictateEvent_SuccessExecute(pClient, MODULE_CHECK, REPORT_ERROE);
		
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT_BEFORE
		Radio_Trf_Debug_Printf_Level2("NB ReportErrorCode Set %d Ok", CMEEnable);
#endif
	}
	else {
		/* Dictate execute is Fail */
		MQTTSN_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, REPORT_ERROE);
		
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT_BEFORE
		Radio_Trf_Debug_Printf_Level2("NB ReportErrorCode Set %d Fail", CMEEnable);
#endif
	}
}

/**********************************************************************************************************
 @Function			void NET_MQTTSN_NBIOT_Event_ModuleCheck(MQTTSN_ClientsTypeDef* pClient)
 @Description			NET_MQTTSN_NBIOT_Event_ModuleCheck		: 模块检测
 @Input				pClient							: MqttSN客户端实例
 @Return				void
**********************************************************************************************************/
void NET_MQTTSN_NBIOT_Event_ModuleCheck(MQTTSN_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBStatus;
	
	MQTTSN_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if (((NBStatus = NBIOT_Neul_NBxx_CheckReadManufacturer(pClient->SocketStack->NBIotStack)) == NBIOT_OK) && 
	    ((NBStatus = NBIOT_Neul_NBxx_CheckReadManufacturerModel(pClient->SocketStack->NBIotStack)) == NBIOT_OK) && 
	    ((NBStatus = NBIOT_Neul_NBxx_CheckReadModuleVersion(pClient->SocketStack->NBIotStack)) == NBIOT_OK)) {
		/* Dictate execute is Success */
		MQTTSN_NBIOT_DictateEvent_SuccessExecute(pClient, PARAMETER_CONFIG, MODULE_CHECK);
		
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT_BEFORE
		Radio_Trf_Debug_Printf_Level2("NB Module Check Ok");
#endif
	}
	else {
		/* Dictate execute is Fail */
		MQTTSN_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, MODULE_CHECK);
		
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT_BEFORE
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		Radio_Trf_Debug_Printf_Level2("NB Module Check Fail ECde %d", NBStatus);
	#else
		Radio_Trf_Debug_Printf_Level2("NB Module Check Fail");
	#endif
#endif
	}
}

/**********************************************************************************************************
 @Function			void NET_MQTTSN_NBIOT_Event_ParameterConfig(MQTTSN_ClientsTypeDef* pClient)
 @Description			NET_MQTTSN_NBIOT_Event_ParameterConfig	: 参数配置
 @Input				pClient							: MqttSN客户端实例
 @Return				void
**********************************************************************************************************/
void NET_MQTTSN_NBIOT_Event_ParameterConfig(MQTTSN_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBStatus;
	
	MQTTSN_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if ((NBStatus = NBIOT_Neul_NBxx_CheckReadConfigUE(pClient->SocketStack->NBIotStack)) == NBIOT_OK) {
		/* Dictate execute is Success */
		MQTTSN_NBIOT_DictateEvent_SuccessExecute(pClient, ICCID_CHECK, PARAMETER_CONFIG);
		
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT_BEFORE
		Radio_Trf_Debug_Printf_Level2("NB Parameter Config Read Ok");
#endif
	}
	else {
		/* Dictate execute is Fail */
		MQTTSN_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, PARAMETER_CONFIG);
		
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT_BEFORE
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		Radio_Trf_Debug_Printf_Level2("NB Parameter Config Read Fail ECde %d", NBStatus);
	#else
		Radio_Trf_Debug_Printf_Level2("NB Parameter Config Read Fail");
	#endif
#endif
		return;
	}
	
	if (pClient->SocketStack->NBIotStack->Parameter.nconfig.autoConnect != AutoConnectVal) {
		if (NBIOT_Neul_NBxx_SetConfigUE(pClient->SocketStack->NBIotStack, AutoConnect, AutoConnectVal) == NBIOT_OK) {
			/* Dictate execute is Success */
			MQTTSN_NBIOT_DictateEvent_SuccessExecute(pClient, ICCID_CHECK, PARAMETER_CONFIG);
			
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT_BEFORE
			Radio_Trf_Debug_Printf_Level2("NB %s %d Ok", AutoConnect, AutoConnectVal);
#endif
		}
		else {
			/* Dictate execute is Fail */
			MQTTSN_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, PARAMETER_CONFIG);
			
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT_BEFORE
			Radio_Trf_Debug_Printf_Level2("NB %s %d Fail", AutoConnect, AutoConnectVal);
#endif
			return;
		}
	}
	
	if (pClient->SocketStack->NBIotStack->Parameter.nconfig.crScrambling != CrScramblingVal) {
		if (NBIOT_Neul_NBxx_SetConfigUE(pClient->SocketStack->NBIotStack, CrScrambling, CrScramblingVal) == NBIOT_OK) {
			/* Dictate execute is Success */
			MQTTSN_NBIOT_DictateEvent_SuccessExecute(pClient, ICCID_CHECK, PARAMETER_CONFIG);
			
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT_BEFORE
			Radio_Trf_Debug_Printf_Level2("NB %s %d Ok", CrScrambling, CrScramblingVal);
#endif
		}
		else {
			/* Dictate execute is Fail */
			MQTTSN_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, PARAMETER_CONFIG);
			
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT_BEFORE
			Radio_Trf_Debug_Printf_Level2("NB %s %d Fail", CrScrambling, CrScramblingVal);
#endif
			return;
		}
	}
	
	if (pClient->SocketStack->NBIotStack->Parameter.nconfig.crSiAvoid != CrSiAvoidVal) {
		if (NBIOT_Neul_NBxx_SetConfigUE(pClient->SocketStack->NBIotStack, CrSiAvoid, CrSiAvoidVal) == NBIOT_OK) {
			/* Dictate execute is Success */
			MQTTSN_NBIOT_DictateEvent_SuccessExecute(pClient, ICCID_CHECK, PARAMETER_CONFIG);
			
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT_BEFORE
			Radio_Trf_Debug_Printf_Level2("NB %s %d Ok", CrSiAvoid, CrSiAvoidVal);
#endif
		}
		else {
			/* Dictate execute is Fail */
			MQTTSN_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, PARAMETER_CONFIG);
			
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT_BEFORE
			Radio_Trf_Debug_Printf_Level2("NB %s %d Fail", CrSiAvoid, CrSiAvoidVal);
#endif
			return;
		}
	}
	
	if (pClient->SocketStack->NBIotStack->Parameter.nconfig.combineAttach != CombineAttachVal) {
		if (NBIOT_Neul_NBxx_SetConfigUE(pClient->SocketStack->NBIotStack, CombineAttach, CombineAttachVal) == NBIOT_OK) {
			/* Dictate execute is Success */
			MQTTSN_NBIOT_DictateEvent_SuccessExecute(pClient, ICCID_CHECK, PARAMETER_CONFIG);
			
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT_BEFORE
			Radio_Trf_Debug_Printf_Level2("NB %s %d Ok", CombineAttach, CombineAttachVal);
#endif
		}
		else {
			/* Dictate execute is Fail */
			MQTTSN_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, PARAMETER_CONFIG);
			
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT_BEFORE
			Radio_Trf_Debug_Printf_Level2("NB %s %d Fail", CombineAttach, CombineAttachVal);
#endif
			return;
		}
	}
	
	if (pClient->SocketStack->NBIotStack->Parameter.nconfig.cellReselection != CellReselectionVal) {
		if (NBIOT_Neul_NBxx_SetConfigUE(pClient->SocketStack->NBIotStack, CellReselection, CellReselectionVal) == NBIOT_OK) {
			/* Dictate execute is Success */
			MQTTSN_NBIOT_DictateEvent_SuccessExecute(pClient, ICCID_CHECK, PARAMETER_CONFIG);
			
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT_BEFORE
			Radio_Trf_Debug_Printf_Level2("NB %s %d Ok", CellReselection, CellReselectionVal);
#endif
		}
		else {
			/* Dictate execute is Fail */
			MQTTSN_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, PARAMETER_CONFIG);
			
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT_BEFORE
			Radio_Trf_Debug_Printf_Level2("NB %s %d Fail", CellReselection, CellReselectionVal);
#endif
			return;
		}
	}
	
	if (pClient->SocketStack->NBIotStack->Parameter.nconfig.enableBip != EnableBipVal) {
		if (NBIOT_Neul_NBxx_SetConfigUE(pClient->SocketStack->NBIotStack, EnableBip, EnableBipVal) == NBIOT_OK) {
			/* Dictate execute is Success */
			MQTTSN_NBIOT_DictateEvent_SuccessExecute(pClient, ICCID_CHECK, PARAMETER_CONFIG);
			
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT_BEFORE
			Radio_Trf_Debug_Printf_Level2("NB %s %d Ok", EnableBip, EnableBipVal);
#endif
		}
		else {
			/* Dictate execute is Fail */
			MQTTSN_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, PARAMETER_CONFIG);
			
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT_BEFORE
			Radio_Trf_Debug_Printf_Level2("NB %s %d Fail", EnableBip, EnableBipVal);
#endif
			return;
		}
	}
}

/**********************************************************************************************************
 @Function			void NET_MQTTSN_NBIOT_Event_SimICCIDCheck(MQTTSN_ClientsTypeDef* pClient)
 @Description			NET_MQTTSN_NBIOT_Event_SimICCIDCheck	: Sim卡检测
 @Input				pClient							: MqttSN客户端实例
 @Return				void
**********************************************************************************************************/
void NET_MQTTSN_NBIOT_Event_SimICCIDCheck(MQTTSN_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBStatus;
	
	MQTTSN_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if ((NBStatus = NBIOT_Neul_NBxx_CheckReadICCID(pClient->SocketStack->NBIotStack)) == NBIOT_OK) {
		/* Dictate execute is Success */
		MQTTSN_NBIOT_DictateEvent_SuccessExecute(pClient, FULL_FUNCTIONALITY, ICCID_CHECK);
		
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT_BEFORE
		Radio_Trf_Debug_Printf_Level2("NB ICCID Check Ok");
#endif
	}
	else {
		/* Dictate execute is Fail */
		MQTTSN_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, ICCID_CHECK);
		
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT_BEFORE
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		Radio_Trf_Debug_Printf_Level2("NB ICCID Check Fail ECde %d", NBStatus);
	#else
		Radio_Trf_Debug_Printf_Level2("NB ICCID Check Fail");
	#endif
#endif
	}
}

/**********************************************************************************************************
 @Function			void NET_MQTTSN_NBIOT_Event_FullFunctionality(MQTTSN_ClientsTypeDef* pClient)
 @Description			NET_MQTTSN_NBIOT_Event_FullFunctionality	: 完整功能
 @Input				pClient								: MqttSN客户端实例
 @Return				void
**********************************************************************************************************/
void NET_MQTTSN_NBIOT_Event_FullFunctionality(MQTTSN_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBStatus;
	
	MQTTSN_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if ((NBStatus = NBIOT_Neul_NBxx_CheckReadMinOrFullFunc(pClient->SocketStack->NBIotStack)) == NBIOT_OK) {
		/* Dictate execute is Success */
		MQTTSN_NBIOT_DictateEvent_SuccessExecute(pClient, NBAND_MODE_CHECK, FULL_FUNCTIONALITY);
		
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT_BEFORE
		Radio_Trf_Debug_Printf_Level2("NB FullFunc Check Ok");
#endif
	}
	else {
		/* Dictate execute is Fail */
		MQTTSN_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, FULL_FUNCTIONALITY);
		
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT_BEFORE
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		Radio_Trf_Debug_Printf_Level2("NB FullFunc Check Fail ECde %d", NBStatus);
	#else
		Radio_Trf_Debug_Printf_Level2("NB FullFunc Check Fail");
	#endif
#endif
		return;
	}
	
	if (pClient->SocketStack->NBIotStack->Parameter.functionality != FullFunc) {
		if ((NBStatus = NBIOT_Neul_NBxx_SetMinOrFullFunc(pClient->SocketStack->NBIotStack, FullFunc)) == NBIOT_OK) {
			/* Dictate execute is Success */
			MQTTSN_NBIOT_DictateEvent_SuccessExecute(pClient, NBAND_MODE_CHECK, FULL_FUNCTIONALITY);
			
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT_BEFORE
			Radio_Trf_Debug_Printf_Level2("NB FullFunc Set Ok");
#endif
		}
		else {
			/* Dictate execute is Fail */
			MQTTSN_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, FULL_FUNCTIONALITY);
			
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT_BEFORE
		#if NBIOT_PRINT_ERROR_CODE_TYPE
			Radio_Trf_Debug_Printf_Level2("NB FullFunc Set Fail ECde %d", NBStatus);
		#else
			Radio_Trf_Debug_Printf_Level2("NB FullFunc Set Fail");
		#endif
#endif
			return;
		}
	}
}

/**********************************************************************************************************
 @Function			void NET_MQTTSN_NBIOT_Event_MinimumFunctionality(MQTTSN_ClientsTypeDef* pClient)
 @Description			NET_MQTTSN_NBIOT_Event_MinimumFunctionality	: 最小功能
 @Input				pClient								: MqttSN客户端实例
 @Return				void
**********************************************************************************************************/
void NET_MQTTSN_NBIOT_Event_MinimumFunctionality(MQTTSN_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBStatus;
	
	MQTTSN_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if ((NBStatus = NBIOT_Neul_NBxx_CheckReadMinOrFullFunc(pClient->SocketStack->NBIotStack)) == NBIOT_OK) {
		/* Dictate execute is Success */
		MQTTSN_NBIOT_DictateEvent_SuccessExecute(pClient, NBAND_MODE_CONFIG, MINIMUM_FUNCTIONALITY);
		
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT_BEFORE
		Radio_Trf_Debug_Printf_Level2("NB MinFunc Check Ok");
#endif
	}
	else {
		/* Dictate execute is Fail */
		MQTTSN_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, MINIMUM_FUNCTIONALITY);
		
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT_BEFORE
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		Radio_Trf_Debug_Printf_Level2("NB MinFunc Check Fail ECde %d", NBStatus);
	#else
		Radio_Trf_Debug_Printf_Level2("NB MinFunc Check Fail");
	#endif
#endif
		return;
	}
	
	if (pClient->SocketStack->NBIotStack->Parameter.functionality != MinFunc) {
		if ((NBStatus = NBIOT_Neul_NBxx_SetMinOrFullFunc(pClient->SocketStack->NBIotStack, MinFunc)) == NBIOT_OK) {
			/* Dictate execute is Success */
			MQTTSN_NBIOT_DictateEvent_SuccessExecute(pClient, NBAND_MODE_CONFIG, MINIMUM_FUNCTIONALITY);
			
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT_BEFORE
			Radio_Trf_Debug_Printf_Level2("NB MinFunc Set Ok");
#endif
		}
		else {
			/* Dictate execute is Fail */
			MQTTSN_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, MINIMUM_FUNCTIONALITY);
			
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT_BEFORE
		#if NBIOT_PRINT_ERROR_CODE_TYPE
			Radio_Trf_Debug_Printf_Level2("NB MinFunc Set Fail ECde %d", NBStatus);
		#else
			Radio_Trf_Debug_Printf_Level2("NB MinFunc Set Fail");
		#endif
#endif
			return;
		}
	}
}

/**********************************************************************************************************
 @Function			void NET_MQTTSN_NBIOT_Event_NbandModeCheck(MQTTSN_ClientsTypeDef* pClient)
 @Description			NET_MQTTSN_NBIOT_Event_NbandModeCheck		: NBAND模式查询
 @Input				pClient								: MqttSN客户端实例
 @Return				void
**********************************************************************************************************/
void NET_MQTTSN_NBIOT_Event_NbandModeCheck(MQTTSN_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBStatus;
	
	MQTTSN_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if ((NBStatus = NBIOT_Neul_NBxx_CheckReadSupportedBands(pClient->SocketStack->NBIotStack)) == NBIOT_OK) {
		/* Dictate execute is Success */
		MQTTSN_NBIOT_DictateEvent_SuccessExecute(pClient, NBAND_MODE_CHECK, NBAND_MODE_CHECK);
		
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT_BEFORE
		Radio_Trf_Debug_Printf_Level2("NB BAND Read %d Ok", pClient->SocketStack->NBIotStack->Parameter.band);
#endif
	}
	else {
		/* Dictate execute is Fail */
		MQTTSN_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, NBAND_MODE_CHECK);
		
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT_BEFORE
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		Radio_Trf_Debug_Printf_Level2("NB BAND Read Fail ECde %d", NBStatus);
	#else
		Radio_Trf_Debug_Printf_Level2("NB BAND Read Fail");
	#endif
#endif
	}
	
	if (pClient->SocketStack->NBIotStack->Parameter.band != MQTTSN_NBIOT_BAND_TYPE) {
		/* BAND Mode Mast be Config */
		pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = MINIMUM_FUNCTIONALITY;
	}
	else {
		/* BAND Mode Needn't be Config */
		pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = ATTACH_CHECK;
	}
}

/**********************************************************************************************************
 @Function			void NET_MQTTSN_NBIOT_Event_NbandModeConfig(MQTTSN_ClientsTypeDef* pClient)
 @Description			NET_MQTTSN_NBIOT_Event_NbandModeConfig		: NBAND模式配置
 @Input				pClient								: MqttSN客户端实例
 @Return				void
**********************************************************************************************************/
void NET_MQTTSN_NBIOT_Event_NbandModeConfig(MQTTSN_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBStatus;
	
	MQTTSN_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if ((NBStatus = NBIOT_Neul_NBxx_CheckReadSupportedBands(pClient->SocketStack->NBIotStack)) == NBIOT_OK) {
		/* Dictate execute is Success */
		MQTTSN_NBIOT_DictateEvent_SuccessExecute(pClient, FULL_FUNCTIONALITY, NBAND_MODE_CONFIG);
		
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT_BEFORE
		Radio_Trf_Debug_Printf_Level2("NB BAND Read %d Ok", pClient->SocketStack->NBIotStack->Parameter.band);
#endif
	}
	else {
		/* Dictate execute is Fail */
		MQTTSN_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, NBAND_MODE_CONFIG);
		
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT_BEFORE
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		Radio_Trf_Debug_Printf_Level2("NB BAND Read Fail ECde %d", NBStatus);
	#else
		Radio_Trf_Debug_Printf_Level2("NB BAND Read Fail");
	#endif
#endif
	}
	
	if (pClient->SocketStack->NBIotStack->Parameter.band != MQTTSN_NBIOT_BAND_TYPE) {
		/* BAND Mode Mast be Config */
		if ((NBStatus = NBIOT_Neul_NBxx_SetSupportedBands(pClient->SocketStack->NBIotStack, MQTTSN_NBIOT_BAND_TYPE)) == NBIOT_OK) {
			/* Dictate execute is Success */
			MQTTSN_NBIOT_DictateEvent_SuccessExecute(pClient, FULL_FUNCTIONALITY, NBAND_MODE_CONFIG);
			
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT_BEFORE
			Radio_Trf_Debug_Printf_Level2("NB BAND Set %d Ok", MQTTSN_NBIOT_BAND_TYPE);
#endif
		}
		else {
			/* Dictate execute is Fail */
			MQTTSN_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, NBAND_MODE_CONFIG);
			
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT_BEFORE
		#if NBIOT_PRINT_ERROR_CODE_TYPE
			Radio_Trf_Debug_Printf_Level2("NB BAND Set %d Fail ECde %d", MQTTSN_NBIOT_BAND_TYPE, NBStatus);
		#else
			Radio_Trf_Debug_Printf_Level2("NB BAND Set %d Fail", MQTTSN_NBIOT_BAND_TYPE);
		#endif
#endif
			return;
		}
	}
	else {
		/* BAND Mode Needn't be Config */
		pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = FULL_FUNCTIONALITY;
	}
}

/**********************************************************************************************************
 @Function			void NET_MQTTSN_NBIOT_Event_AttachCheck(MQTTSN_ClientsTypeDef* pClient)
 @Description			NET_MQTTSN_NBIOT_Event_AttachCheck		: 注网检查
 @Input				pClient							: MqttSN客户端实例
 @Return				void
**********************************************************************************************************/
void NET_MQTTSN_NBIOT_Event_AttachCheck(MQTTSN_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBStatus;
	
	MQTTSN_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if ((NBStatus = NBIOT_Neul_NBxx_CheckReadAttachOrDetach(pClient->SocketStack->NBIotStack)) == NBIOT_OK) {
		/* Dictate execute is Success */
		MQTTSN_NBIOT_DictateEvent_SuccessExecute(pClient, ATTACH_CHECK, ATTACH_CHECK);
		
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("NB CGATT %d Ok", pClient->SocketStack->NBIotStack->Parameter.netstate);
#endif
	}
	else {
		/* Dictate execute is Fail */
		MQTTSN_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, ATTACH_CHECK);
		
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		Radio_Trf_Debug_Printf_Level2("NB CGATT %d Fail ECde %d", pClient->SocketStack->NBIotStack->Parameter.netstate, NBStatus);
	#else
		Radio_Trf_Debug_Printf_Level2("NB CGATT %d Fail", pClient->SocketStack->NBIotStack->Parameter.netstate);
	#endif
#endif
		return;
	}
	
	if (pClient->SocketStack->NBIotStack->Parameter.netstate != Attach) {
		pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = ATTACH_EXECUTE;
	}
	else {
		pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = ATTACH_INQUIRE;
	}
}

/**********************************************************************************************************
 @Function			void NET_MQTTSN_NBIOT_Event_AttachExecute(MQTTSN_ClientsTypeDef* pClient)
 @Description			NET_MQTTSN_NBIOT_Event_AttachExecute	: 注网进行
 @Input				pClient							: MqttSN客户端实例
 @Return				void
**********************************************************************************************************/
void NET_MQTTSN_NBIOT_Event_AttachExecute(MQTTSN_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBStatus;
	
	MQTTSN_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if ((NBStatus = NBIOT_Neul_NBxx_SetAttachOrDetach(pClient->SocketStack->NBIotStack, Attach)) == NBIOT_OK) {
		/* Dictate execute is Success */
		MQTTSN_NBIOT_DictateEvent_SuccessExecute(pClient, ATTACH_INQUIRE, ATTACH_EXECUTE);
		
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("NB Set CGATT %d Ok", Attach);
#endif
	}
	else {
		/* Dictate execute is Fail */
		MQTTSN_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, ATTACH_EXECUTE);
		
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		Radio_Trf_Debug_Printf_Level2("NB Set CGATT %d Fail ECde %d", Attach, NBStatus);
	#else
		Radio_Trf_Debug_Printf_Level2("NB Set CGATT %d Fail", Attach);
	#endif
#endif
	}
}

/**********************************************************************************************************
 @Function			void NET_MQTTSN_NBIOT_Event_AttachInquire(MQTTSN_ClientsTypeDef* pClient)
 @Description			NET_MQTTSN_NBIOT_Event_AttachInquire	: 注网查询
 @Input				pClient							: MqttSN客户端实例
 @Return				void
**********************************************************************************************************/
void NET_MQTTSN_NBIOT_Event_AttachInquire(MQTTSN_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBStatus;
	
	MQTTSN_NBIOT_DictateEvent_SetTime(pClient, 60);
	
	if ((NBStatus = NBIOT_Neul_NBxx_CheckReadAttachOrDetach(pClient->SocketStack->NBIotStack)) == NBIOT_OK) {
		/* Dictate execute is Success */
		pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = ATTACH_INQUIRE;
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("NB CGATT %d Ok", pClient->SocketStack->NBIotStack->Parameter.netstate);
#endif
	}
	else {
		/* Dictate execute is Fail */
		MQTTSN_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, ATTACH_INQUIRE);
		
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		Radio_Trf_Debug_Printf_Level2("NB CGATT %d Fail ECde %d", pClient->SocketStack->NBIotStack->Parameter.netstate, NBStatus);
	#else
		Radio_Trf_Debug_Printf_Level2("NB CGATT %d Fail", pClient->SocketStack->NBIotStack->Parameter.netstate);
	#endif
#endif
		return;
	}
	
	if (pClient->SocketStack->NBIotStack->Parameter.netstate != Attach) {
		MQTTSN_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, ATTACH_INQUIRE);
	}
	else {
		MQTTSN_NBIOT_DictateEvent_SuccessExecute(pClient, PARAMETER_CHECKOUT, ATTACH_INQUIRE);
	}
}

/**********************************************************************************************************
 @Function			void NET_MQTTSN_NBIOT_Event_PatameterCheckOut(MQTTSN_ClientsTypeDef* pClient)
 @Description			NET_MQTTSN_NBIOT_Event_PatameterCheckOut: 参数检出
 @Input				pClient							: MqttSN客户端实例
 @Return				void
**********************************************************************************************************/
void NET_MQTTSN_NBIOT_Event_ParameterCheckOut(MQTTSN_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBStatus;
	
	MQTTSN_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if (((NBStatus = NBIOT_Neul_NBxx_CheckReadIMEI(pClient->SocketStack->NBIotStack)) == NBIOT_OK) && 
	    ((NBStatus = NBIOT_Neul_NBxx_CheckReadIMEISV(pClient->SocketStack->NBIotStack)) == NBIOT_OK) && 
	    ((NBStatus = NBIOT_Neul_NBxx_CheckReadIMSI(pClient->SocketStack->NBIotStack)) == NBIOT_OK) && 
	    ((NBStatus = NBIOT_Neul_NBxx_CheckReadCGPADDR(pClient->SocketStack->NBIotStack)) == NBIOT_OK) && 
	    ((NBStatus = NBIOT_Neul_NBxx_CheckReadCGDCONT(pClient->SocketStack->NBIotStack)) == NBIOT_OK) && 
	    ((NBStatus = NBIOT_Neul_NBxx_CheckReadRSSI(pClient->SocketStack->NBIotStack)) == NBIOT_OK) && 
	    ((NBStatus = NBIOT_Neul_NBxx_CheckReadStatisticsRADIO(pClient->SocketStack->NBIotStack)) == NBIOT_OK) && 
	    ((NBStatus = NBIOT_Neul_NBxx_CheckReadStatisticsCELL(pClient->SocketStack->NBIotStack)) == NBIOT_OK) && 
	    ((NBStatus = NBIOT_Neul_NBxx_CheckReadDateTime(pClient->SocketStack->NBIotStack)) == NBIOT_OK)) {
		/* Dictate execute is Success */
		MQTTSN_NBIOT_DictateEvent_SuccessExecute(pClient, MQTTSN_PROCESS_STACK, PARAMETER_CHECKOUT);
		
		pClient->SocketStack->NBIotStack->Registered = true;
		
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("NB Parameter Check Ok");
#endif
	}
	else {
		/* Dictate execute is Fail */
		MQTTSN_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, PARAMETER_CHECKOUT);
		
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		Radio_Trf_Debug_Printf_Level2("NB Parameter Check Fail ECde %d", NBStatus);
	#else
		Radio_Trf_Debug_Printf_Level2("NB Parameter Check Fail");
	#endif
#endif
		return;
	}
	
	/* Set System Time */
	RTC_Set_Date(pClient->SocketStack->NBIotStack->Parameter.dataTime.year, pClient->SocketStack->NBIotStack->Parameter.dataTime.month, pClient->SocketStack->NBIotStack->Parameter.dataTime.day);
	RTC_Set_Time(pClient->SocketStack->NBIotStack->Parameter.dataTime.hour, pClient->SocketStack->NBIotStack->Parameter.dataTime.min,   pClient->SocketStack->NBIotStack->Parameter.dataTime.sec);
}

/**********************************************************************************************************
 @Function			void NET_MQTTSN_Event_Init(MQTTSN_ClientsTypeDef* pClient)
 @Description			NET_MQTTSN_Event_Init				: INIT
 @Input				pClient							: MqttSN客户端实例
 @Return				void
**********************************************************************************************************/
void NET_MQTTSN_Event_Init(MQTTSN_ClientsTypeDef* pClient)
{
	Stm32_CalculagraphTypeDef dictateRunTime;
	
	MQTTSN_DictateEvent_SetTime(pClient, 30);
	
	/* Creat UDP Socket */
	if (pClient->SocketStack->Open(pClient->SocketStack) == MQTTSN_OK) {
		/* Dictate execute is Success */
		pClient->DictateRunCtl.dictateEnable = false;
		pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = MQTTSN_PROCESS_STACK;
		pClient->SubState = MQTTSN_SUBSTATE_DISCONNECT;
		pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_MQTTSN;
		pClient->DictateRunCtl.dictateInitFailureCnt = 0;
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("MqttSN Creat UDP Ok");
#endif
	}
	else {
		/* Dictate execute is Fail */
		if (Stm32_Calculagraph_IsExpiredSec(&pClient->DictateRunCtl.dictateRunTime) == true) {
			/* Dictate TimeOut */
			pClient->DictateRunCtl.dictateEnable = false;
			pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
			pClient->SubState = MQTTSN_SUBSTATE_INIT;
			pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_DNS;
			pClient->DictateRunCtl.dictateInitFailureCnt++;
			if (pClient->DictateRunCtl.dictateInitFailureCnt > 3) {
				pClient->DictateRunCtl.dictateInitFailureCnt = 0;
				pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = STOP_MODE;
				pClient->SubState = MQTTSN_SUBSTATE_INIT;
				pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_MQTTSN;
			}
		}
		else {
			/* Dictate isn't TimeOut */
			pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = MQTTSN_PROCESS_STACK;
			pClient->SubState = MQTTSN_SUBSTATE_INIT;
			pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_MQTTSN;
		}
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("MqttSN Creat UDP Fail");
#endif
		return;
	}
	
	/* Initiation MQTTSN Status */
	pClient->DictateRunCtl.dictateSubscribeStatus = false;
	/* Set Pingreg Duration */
	pClient->DictateRunCtl.dictateTimeoutSec = TNET_MQTTSN_PINGREG_DURATION;
	Stm32_Calculagraph_CountdownSec(&dictateRunTime, pClient->DictateRunCtl.dictateTimeoutSec);
	pClient->PingTimer = dictateRunTime;
}

/**********************************************************************************************************
 @Function			void NET_MQTTSN_Event_Disconnect(MQTTSN_ClientsTypeDef* pClient)
 @Description			NET_MQTTSN_Event_Disconnect			: DISCONNECT
 @Input				pClient							: MqttSN客户端实例
 @Return				void
**********************************************************************************************************/
void NET_MQTTSN_Event_Disconnect(MQTTSN_ClientsTypeDef* pClient)
{
	MQTTSNPacket_connectData options = MQTTSNPacket_connectData_initializer;
	
	MQTTSN_DictateEvent_SetTime(pClient, 60);
	
	/* Connecting MQTTSN Server */
	options.clientID.cstring = MQTTSN_CLIENT_ID;
	options.duration = TNET_MQTTSN_ACTIVE_DURATION;
	options.cleansession = false;
	if (MQTTSN_Connect(pClient, &options) != MQTTSN_OK) {
		/* Dictate execute is Fail */
		if (Stm32_Calculagraph_IsExpiredSec(&pClient->DictateRunCtl.dictateRunTime) == true) {
			/* Dictate TimeOut */
			pClient->DictateRunCtl.dictateEnable = false;
			pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
			pClient->SubState = MQTTSN_SUBSTATE_INIT;
			pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_DNS;
			pClient->DictateRunCtl.dictateDisconnectFailureCnt++;
			if (pClient->DictateRunCtl.dictateDisconnectFailureCnt > 3) {
				pClient->DictateRunCtl.dictateDisconnectFailureCnt = 0;
				pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = MQTTSN_PROCESS_STACK;
				pClient->SubState = MQTTSN_SUBSTATE_LOST;
				pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_MQTTSN;
			}
		}
		else {
			/* Dictate isn't TimeOut */
			pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = MQTTSN_PROCESS_STACK;
			pClient->SubState = MQTTSN_SUBSTATE_DISCONNECT;
			pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_MQTTSN;
		}
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("MqttSN Connect Server Fail");
#endif
	}
	else {
		/* Dictate execute is Success */
		pClient->DictateRunCtl.dictateEnable = false;
		pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = MQTTSN_PROCESS_STACK;
		pClient->SubState = MQTTSN_SUBSTATE_ACTIVE;
		pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_MQTTSN;
		pClient->DictateRunCtl.dictateDisconnectFailureCnt = 0;
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("MqttSN Connect Server Ok");
#endif
	}
}

/**********************************************************************************************************
 @Function			void NET_MQTTSN_Event_Active(MQTTSN_ClientsTypeDef* pClient)
 @Description			NET_MQTTSN_Event_Active				: ACTIVE
 @Input				pClient							: MqttSN客户端实例
 @Return				void
**********************************************************************************************************/
void NET_MQTTSN_Event_Active(MQTTSN_ClientsTypeDef* pClient)
{
	MQTTSN_DictateEvent_SetTime(pClient, 60);
	
	/* Subscribing Topic */
	if (pClient->DictateRunCtl.dictateSubscribeStatus != true) {
		if (MQTTSN_Subscribe(pClient, MQTTSN_SUBSCRIBE_ID, QOS2, messageHandlerFunction) != MQTTSN_OK) {
			/* Dictate execute is Fail */
			if (Stm32_Calculagraph_IsExpiredSec(&pClient->DictateRunCtl.dictateRunTime) == true) {
				/* Dictate TimeOut */
				pClient->DictateRunCtl.dictateEnable = false;
				pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
				pClient->SubState = MQTTSN_SUBSTATE_INIT;
				pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_DNS;
				pClient->DictateRunCtl.dictateActiveFailureCnt++;
				if (pClient->DictateRunCtl.dictateActiveFailureCnt > 3) {
					pClient->DictateRunCtl.dictateActiveFailureCnt = 0;
					pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = MQTTSN_PROCESS_STACK;
					pClient->SubState = MQTTSN_SUBSTATE_LOST;
					pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_MQTTSN;
				}
			}
			else {
				/* Dictate isn't TimeOut */
				pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = MQTTSN_PROCESS_STACK;
				pClient->SubState = MQTTSN_SUBSTATE_ACTIVE;
				pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_MQTTSN;
			}
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("MqttSN Subscrib %s Fail", MQTTSN_SUBSCRIBE_ID);
#endif
			return;
		}
		else {
			/* Dictate execute is Success */
			pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = MQTTSN_PROCESS_STACK;
			pClient->SubState = MQTTSN_SUBSTATE_ACTIVE;
			pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_MQTTSN;
			pClient->DictateRunCtl.dictateSubscribeStatus = true;
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("MqttSN Subscrib %s Ok", MQTTSN_SUBSCRIBE_ID);
#endif
		}
	}
	
	/* Whether the query has data needs to be sent */
#if MQTTSN_MSG_VERSION_STREAM_TYPE == MQTTSN_MSG_VERSION_JSON_STREAM
	/* OBJECT_TYPE_TMOTES_STATUS_BASIC_PUT */
#if NBMQTTSN_SENDCODE_STATUS_BASIC
	if (NET_MqttSN_Message_StatusBasicisEmpty() != true) {
		pClient->MessageSendCtl.messageStatusBasic = true;
	}
	if (pClient->MessageSendCtl.messageStatusBasic != false) {
		if (NET_MQTTSN_SendPayloadPacket(pClient, OBJECT_TYPE_TMOTES_STATUS_BASIC_PUT) != MQTTSN_OK) {
			/* Dictate execute is Fail */
			if (Stm32_Calculagraph_IsExpiredSec(&pClient->DictateRunCtl.dictateRunTime) == true) {
				/* Dictate TimeOut */
				pClient->DictateRunCtl.dictateEnable = false;
				pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
				pClient->SubState = MQTTSN_SUBSTATE_INIT;
				pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_DNS;
				pClient->DictateRunCtl.dictateActiveFailureCnt++;
				if (pClient->DictateRunCtl.dictateActiveFailureCnt > 3) {
					pClient->DictateRunCtl.dictateActiveFailureCnt = 0;
					pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = MQTTSN_PROCESS_STACK;
					pClient->SubState = MQTTSN_SUBSTATE_LOST;
					pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_MQTTSN;
				}
			}
			else {
				/* Dictate isn't TimeOut */
				pClient->SubState = MQTTSN_SUBSTATE_ACTIVE;
			}
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("MqttSN Send StatusBasic Fail");
#endif
			return;
		}
		else {
			/* Dictate execute is Success */
			pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = MQTTSN_PROCESS_STACK;
			pClient->SubState = MQTTSN_SUBSTATE_ACTIVE;
			pClient->MessageSendCtl.messageStatusBasic = false;
			pClient->SocketStack->NBIotStack->NetStateIdentification = true;
			NET_MqttSN_Message_StatusBasicOffSet();
			/* NB 继续活跃注入时间 */
			TCFG_Utility_Set_Nbiot_IdleLifetime(NBIOT_CONTINUE_LIFETIME);
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("MqttSN Send StatusBasic Ok");
#endif
		}
	}
#endif
	
	/* OBJECT_TYPE_TMOTES_STATUS_EXTEND_PUT */
#if NBMQTTSN_SENDCODE_STATUS_EXTEND
	if (NET_MqttSN_Message_StatusExtendisEmpty() != true) {
		pClient->MessageSendCtl.messageStatusExtend = true;
	}
	if (pClient->MessageSendCtl.messageStatusExtend != false) {
		if (NET_MQTTSN_SendPayloadPacket(pClient, OBJECT_TYPE_TMOTES_STATUS_EXTEND_PUT) != MQTTSN_OK) {
			/* Dictate execute is Fail */
			if (Stm32_Calculagraph_IsExpiredSec(&pClient->DictateRunCtl.dictateRunTime) == true) {
				/* Dictate TimeOut */
				pClient->DictateRunCtl.dictateEnable = false;
				pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
				pClient->SubState = MQTTSN_SUBSTATE_INIT;
				pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_DNS;
				pClient->DictateRunCtl.dictateActiveFailureCnt++;
				if (pClient->DictateRunCtl.dictateActiveFailureCnt > 3) {
					pClient->DictateRunCtl.dictateActiveFailureCnt = 0;
					pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = MQTTSN_PROCESS_STACK;
					pClient->SubState = MQTTSN_SUBSTATE_LOST;
					pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_MQTTSN;
				}
			}
			else {
				/* Dictate isn't TimeOut */
				pClient->SubState = MQTTSN_SUBSTATE_ACTIVE;
			}
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("MqttSN Send StatusExtend Fail");
#endif
			return;
		}
		else {
			/* Dictate execute is Success */
			pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = MQTTSN_PROCESS_STACK;
			pClient->SubState = MQTTSN_SUBSTATE_ACTIVE;
			pClient->MessageSendCtl.messageStatusExtend = false;
			pClient->SocketStack->NBIotStack->NetStateIdentification = true;
			NET_MqttSN_Message_StatusExtendOffSet();
			/* NB 继续活跃注入时间 */
			TCFG_Utility_Set_Nbiot_IdleLifetime(NBIOT_CONTINUE_LIFETIME);
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("MqttSN Send StatusExtend Ok");
#endif
		}
	}
#endif
	
	/* OBJECT_TYPE_TMOTES_INFO_WORK_PUT */
#if NBMQTTSN_SENDCODE_WORK_INFO
	if (NET_MqttSN_Message_InfoWorkisEmpty() != true) {
		pClient->MessageSendCtl.messageInfoWork = true;
	}
	if (pClient->MessageSendCtl.messageInfoWork != false) {
		if (NET_MQTTSN_SendPayloadPacket(pClient, OBJECT_TYPE_TMOTES_INFO_WORK_PUT) != MQTTSN_OK) {
			/* Dictate execute is Fail */
			if (Stm32_Calculagraph_IsExpiredSec(&pClient->DictateRunCtl.dictateRunTime) == true) {
				/* Dictate TimeOut */
				pClient->DictateRunCtl.dictateEnable = false;
				pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
				pClient->SubState = MQTTSN_SUBSTATE_INIT;
				pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_DNS;
				pClient->DictateRunCtl.dictateActiveFailureCnt++;
				if (pClient->DictateRunCtl.dictateActiveFailureCnt > 3) {
					pClient->DictateRunCtl.dictateActiveFailureCnt = 0;
					pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = MQTTSN_PROCESS_STACK;
					pClient->SubState = MQTTSN_SUBSTATE_LOST;
					pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_MQTTSN;
				}
			}
			else {
				/* Dictate isn't TimeOut */
				pClient->SubState = MQTTSN_SUBSTATE_ACTIVE;
			}
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("MqttSN Send InfoWork Fail");
#endif
			return;
		}
		else {
			/* Dictate execute is Success */
			pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = MQTTSN_PROCESS_STACK;
			pClient->SubState = MQTTSN_SUBSTATE_ACTIVE;
			pClient->MessageSendCtl.messageInfoWork = false;
			pClient->SocketStack->NBIotStack->NetStateIdentification = true;
			NET_MqttSN_Message_InfoWorkOffSet();
			/* NB 继续活跃注入时间 */
			TCFG_Utility_Set_Nbiot_IdleLifetime(NBIOT_CONTINUE_LIFETIME);
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("MqttSN Send InfoWork Ok");
#endif
		}
	}
#endif
	
	/* OBJECT_TYPE_TMOTES_INFO_BASIC_PUT */
#if NBMQTTSN_SENDCODE_BASIC_INFO
	if (NET_MqttSN_Message_InfoBasicisEmpty() != true) {
		pClient->MessageSendCtl.messageInfoBasic = true;
	}
	if (pClient->MessageSendCtl.messageInfoBasic != false) {
		if (NET_MQTTSN_SendPayloadPacket(pClient, OBJECT_TYPE_TMOTES_INFO_BASIC_PUT) != MQTTSN_OK) {
			/* Dictate execute is Fail */
			if (Stm32_Calculagraph_IsExpiredSec(&pClient->DictateRunCtl.dictateRunTime) == true) {
				/* Dictate TimeOut */
				pClient->DictateRunCtl.dictateEnable = false;
				pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
				pClient->SubState = MQTTSN_SUBSTATE_INIT;
				pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_DNS;
				pClient->DictateRunCtl.dictateActiveFailureCnt++;
				if (pClient->DictateRunCtl.dictateActiveFailureCnt > 3) {
					pClient->DictateRunCtl.dictateActiveFailureCnt = 0;
					pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = MQTTSN_PROCESS_STACK;
					pClient->SubState = MQTTSN_SUBSTATE_LOST;
					pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_MQTTSN;
				}
			}
			else {
				/* Dictate isn't TimeOut */
				pClient->SubState = MQTTSN_SUBSTATE_ACTIVE;
			}
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("MqttSN Send InfoBasic Fail");
#endif
			return;
		}
		else {
			/* Dictate execute is Success */
			pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = MQTTSN_PROCESS_STACK;
			pClient->SubState = MQTTSN_SUBSTATE_ACTIVE;
			pClient->MessageSendCtl.messageInfoBasic = false;
			pClient->SocketStack->NBIotStack->NetStateIdentification = true;
			NET_MqttSN_Message_InfoBasicOffSet();
			/* NB 继续活跃注入时间 */
			TCFG_Utility_Set_Nbiot_IdleLifetime(NBIOT_CONTINUE_LIFETIME);
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("MqttSN Send InfoBasic Ok");
#endif
		}
	}
#endif
	
	/* OBJECT_TYPE_TMOTES_INFO_DYNAMIC_PUT */
#if NBMQTTSN_SENDCODE_DYNAMIC_INFO
	if (NET_MqttSN_Message_InfoDynamicisEmpty() != true) {
		pClient->MessageSendCtl.messageInfoDynamic = true;
	}
	if (pClient->MessageSendCtl.messageInfoDynamic != false) {
		if (NET_MQTTSN_SendPayloadPacket(pClient, OBJECT_TYPE_TMOTES_INFO_DYNAMIC_PUT) != MQTTSN_OK) {
			/* Dictate execute is Fail */
			if (Stm32_Calculagraph_IsExpiredSec(&pClient->DictateRunCtl.dictateRunTime) == true) {
				/* Dictate TimeOut */
				pClient->DictateRunCtl.dictateEnable = false;
				pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
				pClient->SubState = MQTTSN_SUBSTATE_INIT;
				pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_DNS;
				pClient->DictateRunCtl.dictateActiveFailureCnt++;
				if (pClient->DictateRunCtl.dictateActiveFailureCnt > 3) {
					pClient->DictateRunCtl.dictateActiveFailureCnt = 0;
					pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = MQTTSN_PROCESS_STACK;
					pClient->SubState = MQTTSN_SUBSTATE_LOST;
					pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_MQTTSN;
				}
			}
			else {
				/* Dictate isn't TimeOut */
				pClient->SubState = MQTTSN_SUBSTATE_ACTIVE;
			}
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("MqttSN Send InfoDynamic Fail");
#endif
			return;
		}
		else {
			/* Dictate execute is Success */
			pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = MQTTSN_PROCESS_STACK;
			pClient->SubState = MQTTSN_SUBSTATE_ACTIVE;
			pClient->MessageSendCtl.messageInfoDynamic = false;
			pClient->SocketStack->NBIotStack->NetStateIdentification = true;
			NET_MqttSN_Message_InfoDynamicOffSet();
			/* NB 继续活跃注入时间 */
			TCFG_Utility_Set_Nbiot_IdleLifetime(NBIOT_CONTINUE_LIFETIME);
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("MqttSN Send InfoDynamic Ok");
#endif
		}
	}
#endif
	
	/* OBJECT_TYPE_TMOTES_INFO_RADAR_PUT */
#if NBMQTTSN_SENDCODE_RADAR_INFO
	if (NET_MqttSN_Message_InfoRadarisEmpty() != true) {
		pClient->MessageSendCtl.messageInfoRadar = true;
	}
	if (pClient->MessageSendCtl.messageInfoRadar != false) {
		if (NET_MQTTSN_SendPayloadPacket(pClient, OBJECT_TYPE_TMOTES_INFO_RADAR_PUT) != MQTTSN_OK) {
			/* Dictate execute is Fail */
			if (Stm32_Calculagraph_IsExpiredSec(&pClient->DictateRunCtl.dictateRunTime) == true) {
				/* Dictate TimeOut */
				pClient->DictateRunCtl.dictateEnable = false;
				pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
				pClient->SubState = MQTTSN_SUBSTATE_INIT;
				pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_DNS;
				pClient->DictateRunCtl.dictateActiveFailureCnt++;
				if (pClient->DictateRunCtl.dictateActiveFailureCnt > 3) {
					pClient->DictateRunCtl.dictateActiveFailureCnt = 0;
					pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = MQTTSN_PROCESS_STACK;
					pClient->SubState = MQTTSN_SUBSTATE_LOST;
					pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_MQTTSN;
				}
			}
			else {
				/* Dictate isn't TimeOut */
				pClient->SubState = MQTTSN_SUBSTATE_ACTIVE;
			}
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("MqttSN Send InfoRadar Fail");
#endif
			return;
		}
		else {
			/* Dictate execute is Success */
			pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = MQTTSN_PROCESS_STACK;
			pClient->SubState = MQTTSN_SUBSTATE_ACTIVE;
			pClient->MessageSendCtl.messageInfoRadar = false;
			pClient->SocketStack->NBIotStack->NetStateIdentification = true;
			NET_MqttSN_Message_InfoRadarOffSet();
			/* NB 继续活跃注入时间 */
			TCFG_Utility_Set_Nbiot_IdleLifetime(NBIOT_CONTINUE_LIFETIME);
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("MqttSN Send InfoRadar Ok");
#endif
		}
	}
#endif
	
	/* OBJECT_TYPE_TMOTES_INFO_RESPONSE_PUT */
	if (NET_MqttSN_Message_InfoResponseisEmpty() != true) {
		pClient->MessageSendCtl.messageInfoResponse = true;
	}
	if (pClient->MessageSendCtl.messageInfoResponse != false) {
		if (NET_MQTTSN_SendPayloadPacket(pClient, OBJECT_TYPE_TMOTES_INFO_RESPONSE_PUT) != MQTTSN_OK) {
			/* Dictate execute is Fail */
			if (Stm32_Calculagraph_IsExpiredSec(&pClient->DictateRunCtl.dictateRunTime) == true) {
				/* Dictate TimeOut */
				pClient->DictateRunCtl.dictateEnable = false;
				pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
				pClient->SubState = MQTTSN_SUBSTATE_INIT;
				pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_DNS;
				pClient->DictateRunCtl.dictateActiveFailureCnt++;
				if (pClient->DictateRunCtl.dictateActiveFailureCnt > 3) {
					pClient->DictateRunCtl.dictateActiveFailureCnt = 0;
					pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = MQTTSN_PROCESS_STACK;
					pClient->SubState = MQTTSN_SUBSTATE_LOST;
					pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_MQTTSN;
				}
			}
			else {
				/* Dictate isn't TimeOut */
				pClient->SubState = MQTTSN_SUBSTATE_ACTIVE;
			}
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("MqttSN Send InfoResponse Fail");
#endif
			return;
		}
		else {
			/* Dictate execute is Success */
			pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = MQTTSN_PROCESS_STACK;
			pClient->SubState = MQTTSN_SUBSTATE_ACTIVE;
			pClient->MessageSendCtl.messageInfoResponse = false;
			pClient->SocketStack->NBIotStack->NetStateIdentification = true;
			NET_MqttSN_Message_InfoResponseOffSet();
			/* NB 继续活跃注入时间 */
			TCFG_Utility_Set_Nbiot_IdleLifetime(NBIOT_CONTINUE_LIFETIME);
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("MqttSN Send InfoResponse Ok");
#endif
		}
	}
#endif
	
#if MQTTSN_MSG_VERSION_STREAM_TYPE == MQTTSN_MSG_VERSION_BYTE_STREAM
	/* OBJECT_TYPE_TMOTES_BYTE_STREAM_PUT */
	if (NET_MqttSN_Message_SendDataisEmpty() != true) {
		pClient->MessageSendCtl.messageByteStream = true;
	}
	if (pClient->MessageSendCtl.messageByteStream != false) {
		if (NET_MQTTSN_SendPayloadPacket(pClient, OBJECT_TYPE_TMOTES_BYTE_STREAM_PUT) != MQTTSN_OK) {
			/* Dictate execute is Fail */
			if (Stm32_Calculagraph_IsExpiredSec(&pClient->DictateRunCtl.dictateRunTime) == true) {
				/* Dictate TimeOut */
				pClient->DictateRunCtl.dictateEnable = false;
				pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
				pClient->SubState = MQTTSN_SUBSTATE_INIT;
				pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_DNS;
				pClient->DictateRunCtl.dictateActiveFailureCnt++;
				if (pClient->DictateRunCtl.dictateActiveFailureCnt > 3) {
					pClient->DictateRunCtl.dictateActiveFailureCnt = 0;
					pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = MQTTSN_PROCESS_STACK;
					pClient->SubState = MQTTSN_SUBSTATE_LOST;
					pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_MQTTSN;
				}
			}
			else {
				/* Dictate isn't TimeOut */
				pClient->SubState = MQTTSN_SUBSTATE_ACTIVE;
			}
		}
		else {
			/* Dictate execute is Success */
			pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = MQTTSN_PROCESS_STACK;
			pClient->SubState = MQTTSN_SUBSTATE_ACTIVE;
			pClient->MessageSendCtl.messageByteStream = false;
			pClient->SocketStack->NBIotStack->NetStateIdentification = true;
			NET_MqttSN_Message_SendDataOffSet();
			/* NB 继续活跃注入时间 */
			TCFG_Utility_Set_Nbiot_IdleLifetime(NBIOT_CONTINUE_LIFETIME);
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("MqttSN Send Payload Ok");
#endif
		}
	}
#endif
	
	/* Keep active for Active seconds before to Sleep, so we can send messsage contiguously */
	if (Stm32_Calculagraph_IsExpiredSec(&pClient->DictateRunCtl.dictateRunTime) == true) {
		/* Arrival time for Sleep */
		if (MQTTSN_DisConnect(pClient, TNET_MQTTSN_SLEEP_DURATION) != MQTTSN_OK) {
			/* Dictate execute is Fail */
			pClient->DictateRunCtl.dictateEnable = false;
			pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
			pClient->SubState = MQTTSN_SUBSTATE_INIT;
			pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_DNS;
			pClient->DictateRunCtl.dictateActiveFailureCnt++;
			if (pClient->DictateRunCtl.dictateActiveFailureCnt > 3) {
				pClient->DictateRunCtl.dictateActiveFailureCnt = 0;
				pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = MQTTSN_PROCESS_STACK;
				pClient->SubState = MQTTSN_SUBSTATE_LOST;
				pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_MQTTSN;
			}
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("MqttSN DisConnect Fail");
#endif
		}
		else {
			/* Dictate execute is Success */
			pClient->DictateRunCtl.dictateEnable = false;
			pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = MQTTSN_PROCESS_STACK;
			pClient->SubState = MQTTSN_SUBSTATE_AWAKE;
			pClient->DictateRunCtl.dictateActiveFailureCnt = 0;
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("MqttSN DisConnect Ok");
#endif
		}
	}
	else {
		/* Keep Active */
		MQTTSN_RecvAck(pClient);
		pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = MQTTSN_PROCESS_STACK;
		pClient->SubState = MQTTSN_SUBSTATE_ACTIVE;
	}
}

/**********************************************************************************************************
 @Function			void NET_MQTTSN_Event_Sleep(MQTTSN_ClientsTypeDef* pClient)
 @Description			NET_MQTTSN_Event_Sleep				: SLEEP
 @Input				pClient							: MqttSN客户端实例
 @Return				void
**********************************************************************************************************/
void NET_MQTTSN_Event_Sleep(MQTTSN_ClientsTypeDef* pClient)
{
	MQTTSNPacket_connectData options = MQTTSNPacket_connectData_initializer;
	
	MQTTSN_DictateEvent_SetTime(pClient, 60);
	
	/* Whether the query has data needs to be sent */
#if MQTTSN_MSG_VERSION_STREAM_TYPE == MQTTSN_MSG_VERSION_JSON_STREAM
	if (NET_MqttSN_Message_StatusBasicisEmpty() != true) {
		pClient->MessageSendCtl.messageStatusBasic = true;
	}
	if (NET_MqttSN_Message_StatusExtendisEmpty() != true) {
		pClient->MessageSendCtl.messageStatusExtend = true;
	}
	if (NET_MqttSN_Message_InfoWorkisEmpty() != true) {
		pClient->MessageSendCtl.messageInfoWork = true;
	}
	if (NET_MqttSN_Message_InfoBasicisEmpty() != true) {
		pClient->MessageSendCtl.messageInfoBasic = true;
	}
	if (NET_MqttSN_Message_InfoDynamicisEmpty() != true) {
		pClient->MessageSendCtl.messageInfoDynamic = true;
	}
	if (NET_MqttSN_Message_InfoRadarisEmpty() != true) {
		pClient->MessageSendCtl.messageInfoRadar = true;
	}
	if (NET_MqttSN_Message_InfoResponseisEmpty() != true) {
		pClient->MessageSendCtl.messageInfoResponse = true;
	}
#endif
#if MQTTSN_MSG_VERSION_STREAM_TYPE == MQTTSN_MSG_VERSION_BYTE_STREAM
	if (NET_MqttSN_Message_SendDataisEmpty() != true) {
		pClient->MessageSendCtl.messageByteStream = true;
	}
#endif
	
#if MQTTSN_MSG_VERSION_STREAM_TYPE == MQTTSN_MSG_VERSION_JSON_STREAM
	if ( (pClient->MessageSendCtl.messageStatusBasic != false) || 
		(pClient->MessageSendCtl.messageStatusExtend != false) ||
		(pClient->MessageSendCtl.messageInfoWork != false) ||
		(pClient->MessageSendCtl.messageInfoBasic != false) ||
		(pClient->MessageSendCtl.messageInfoDynamic != false) ||
		(pClient->MessageSendCtl.messageInfoRadar != false) ||
		(pClient->MessageSendCtl.messageInfoResponse != false) ) {
#endif
#if MQTTSN_MSG_VERSION_STREAM_TYPE == MQTTSN_MSG_VERSION_BYTE_STREAM
	if ( pClient->MessageSendCtl.messageByteStream != false ) {
#endif
		options.clientID.cstring = MQTTSN_CLIENT_ID;
		options.duration = TNET_MQTTSN_ACTIVE_DURATION;
		options.cleansession = false;
		if (MQTTSN_Connect(pClient, &options) != MQTTSN_OK) {
			/* Dictate execute is Fail */
			if (Stm32_Calculagraph_IsExpiredSec(&pClient->DictateRunCtl.dictateRunTime) == true) {
				/* Dictate TimeOut */
				pClient->DictateRunCtl.dictateEnable = false;
				pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
				pClient->SubState = MQTTSN_SUBSTATE_INIT;
				pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_DNS;
				pClient->DictateRunCtl.dictateSleepFailureCnt++;
				if (pClient->DictateRunCtl.dictateSleepFailureCnt > 3) {
					pClient->DictateRunCtl.dictateSleepFailureCnt = 0;
					pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = MQTTSN_PROCESS_STACK;
					pClient->SubState = MQTTSN_SUBSTATE_LOST;
					pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_MQTTSN;
				}
			}
			else {
				/* Dictate isn't TimeOut */
				pClient->SubState = MQTTSN_SUBSTATE_SLEEP;
			}
			return;
		}
		else {
			/* Dictate execute is Success */
			pClient->DictateRunCtl.dictateEnable = false;
			pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = MQTTSN_PROCESS_STACK;
			pClient->SubState = MQTTSN_SUBSTATE_ACTIVE;
			pClient->DictateRunCtl.dictateSleepFailureCnt = 0;
			return;
		}
	}
	
	/* If time to Aweak, then send a pingreq */
	if (Stm32_Calculagraph_IsExpiredSec(&pClient->PingTimer) == true) {
		pClient->DictateRunCtl.dictateEnable = false;
		pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = MQTTSN_PROCESS_STACK;
		pClient->SubState = MQTTSN_SUBSTATE_AWAKE;
		pClient->DictateRunCtl.dictateSleepFailureCnt = 0;
	}
	else {
		pClient->DictateRunCtl.dictateEnable = false;
		pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = MQTTSN_PROCESS_STACK;
		pClient->SubState = MQTTSN_SUBSTATE_SLEEP;
		pClient->DictateRunCtl.dictateSleepFailureCnt = 0;
	}
}

/**********************************************************************************************************
 @Function			void NET_MQTTSN_Event_Aweak(MQTTSN_ClientsTypeDef* pClient)
 @Description			NET_MQTTSN_Event_Aweak				: AWAKE
 @Input				pClient							: MqttSN客户端实例
 @Return				void
**********************************************************************************************************/
void NET_MQTTSN_Event_Aweak(MQTTSN_ClientsTypeDef* pClient)
{
	MQTTSNString clientid = MQTTSNString_initializer;
	Stm32_CalculagraphTypeDef dictateRunTime;
	
	MQTTSN_DictateEvent_SetTime(pClient, 60);
	
	/* Pingreqing MQTTSN Server */
	clientid.cstring = MQTTSN_CLIENT_ID;
	if (MQTTSN_Pingreq(pClient, &clientid) != MQTTSN_OK) {
		/* Dictate execute is Fail */
		if (Stm32_Calculagraph_IsExpiredSec(&pClient->DictateRunCtl.dictateRunTime) == true) {
			/* Dictate TimeOut */
			pClient->DictateRunCtl.dictateEnable = false;
			pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
			pClient->SubState = MQTTSN_SUBSTATE_INIT;
			pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_DNS;
			pClient->DictateRunCtl.dictateAweakFailureCnt++;
			if (pClient->DictateRunCtl.dictateAweakFailureCnt > 3) {
				pClient->DictateRunCtl.dictateAweakFailureCnt = 0;
				pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = MQTTSN_PROCESS_STACK;
				pClient->SubState = MQTTSN_SUBSTATE_LOST;
				pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_MQTTSN;
			}
		}
		else {
			/* Dictate isn't TimeOut */
			pClient->SubState = MQTTSN_SUBSTATE_AWAKE;
		}
		return;
	}
	else {
		/* Dictate execute is Success */
		pClient->DictateRunCtl.dictateEnable = false;
		pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = MQTTSN_PROCESS_STACK;
		pClient->SubState = MQTTSN_SUBSTATE_SLEEP;
		pClient->DictateRunCtl.dictateAweakFailureCnt = 0;
	}
	
	/* Set Pingreg Duration */
	pClient->DictateRunCtl.dictateTimeoutSec = TNET_MQTTSN_PINGREG_DURATION;
	Stm32_Calculagraph_CountdownSec(&dictateRunTime, pClient->DictateRunCtl.dictateTimeoutSec);
	pClient->PingTimer = dictateRunTime;
}

/**********************************************************************************************************
 @Function			void NET_MQTTSN_Event_Lost(MQTTSN_ClientsTypeDef* pClient)
 @Description			NET_MQTTSN_Event_Lost				: LOST
 @Input				pClient							: MqttSN客户端实例
 @Return				void
**********************************************************************************************************/
void NET_MQTTSN_Event_Lost(MQTTSN_ClientsTypeDef* pClient)
{
	pClient->DictateRunCtl.dictateEnable = false;
	pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = STOP_MODE;
	pClient->SubState = MQTTSN_SUBSTATE_INIT;
	pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_MQTTSN;
}

/**********************************************************************************************************
 @Function			MQTTSN_StatusTypeDef messageHandlerFunction(MQTTSN_ClientsTypeDef* pClient, MQTTSN_MessageDataTypeDef* messageHandler)
 @Description			messageHandlerFunction		: MQTTSN接收处理
 @Input				pClient					: MqttSN客户端实例
					messageHandler				: 下行消息
 @Return				MQTTSN_StatusTypeDef		: MQTTSN处理状态
**********************************************************************************************************/
MQTTSN_StatusTypeDef messageHandlerFunction(MQTTSN_ClientsTypeDef* pClient, MQTTSN_MessageDataTypeDef* messageHandler)
{
	u16 recvBufOffset = 0;
	u16 recvMagicNum = 0;
	u8 ret = NETIP_OK;
	bool recvEffective = false;
	
#ifdef MQTTSN_DEBUG_LOG_RF_PRINT
	Radio_Trf_Debug_Printf_Level2("MqttSN Recv Filter:%s, topicid:%d", pClient->messageHandlers[0].topicFilter, pClient->messageHandlers[0].topicid);
	Radio_Trf_Debug_Printf_Level2("MqttSN Recv Payload:%.*s", messageHandler->message->payloadlen, messageHandler->message->payload);
#endif
	
	messageHandler->message->payload[messageHandler->message->payloadlen] = '\0';
	for (int i = 0; i < messageHandler->message->payloadlen; i++) {
		if ((messageHandler->message->payload[i] == 'T') && (messageHandler->message->payload[i+1] == 'C') && \
		    (messageHandler->message->payload[i+2] == 'L') && (messageHandler->message->payload[i+3] == 'D')) {
			recvBufOffset = i;
			recvEffective = true;
		}
	}
	
	if (recvEffective != false) {
		/* Find "TCLD" */
		if (messageHandler->message->payload[recvBufOffset + TCLOD_MSGVERSION_OFFSET] == TCLOD_MSGVERSION) {
			if (messageHandler->message->payload[recvBufOffset + TCLOD_MSGID_OFFSET] == TCLOD_CONFIG_SET) {
				BEEP_CtrlRepeat_Extend(1, 300, 0);
				TCFG_EEPROM_SetNBCmdCnt(1 + TCFG_EEPROM_GetNBCmdCnt());
				
				/* 工作模式配置指令 */
				if (strstr((char *)messageHandler->message->payload + recvBufOffset + TCLOD_DATA_OFFSET, "Workmode") != NULL) {
					u16 mode;
					sscanf((char *)messageHandler->message->payload + recvBufOffset + TCLOD_DATA_OFFSET, "{(Workmode):{(val):%hu,(Magic):%hu}}", &mode, &recvMagicNum);
					if (recvMagicNum == TCLOD_MAGIC_NUM) {
						TCFG_SystemData.WorkMode = mode;
						if ((TCFG_SystemData.WorkMode != DEBUG_WORK) && (TCFG_SystemData.WorkMode != NORMAL_WORK)) {
							TCFG_SystemData.WorkMode = NORMAL_WORK;
							TCFG_EEPROM_SetWorkMode(TCFG_SystemData.WorkMode);
							ret = NETIP_ERRORPARAM;
						}
						else {
							TCFG_EEPROM_SetWorkMode(TCFG_SystemData.WorkMode);
						}
					}
					else {
						ret = NETIP_UNKNOWNERROR;
					}
				}
				/* 传感器灵敏度配置指令 */
				else if (strstr((char *)messageHandler->message->payload + recvBufOffset + TCLOD_DATA_OFFSET, "Sense") != NULL) {
					u16 sense;
					sscanf((char *)messageHandler->message->payload + recvBufOffset + TCLOD_DATA_OFFSET, "{(Sense):{(val):%hu,(Magic):%hu}}", &sense, &recvMagicNum);
					if (recvMagicNum == TCLOD_MAGIC_NUM) {
						TCFG_SystemData.Sensitivity = sense;
						if ((TCFG_SystemData.Sensitivity > SENSE_LOWEST) || (TCFG_SystemData.Sensitivity < SENSE_HIGHEST)) {
							TCFG_SystemData.Sensitivity = SENSE_MIDDLE;
							TCFG_EEPROM_SetSavedSensitivity(TCFG_SystemData.Sensitivity);
							ret = NETIP_ERRORPARAM;
						}
						else {
							TCFG_EEPROM_SetSavedSensitivity(TCFG_SystemData.Sensitivity);
						}
					}
					else {
						ret = NETIP_UNKNOWNERROR;
					}
				}
				/* 无线心跳间隔时间配置指令 */
				else if (strstr((char *)messageHandler->message->payload + recvBufOffset + TCLOD_DATA_OFFSET, "RFHeart") != NULL) {
					u16 rfheart;
					sscanf((char *)messageHandler->message->payload + recvBufOffset + TCLOD_DATA_OFFSET, "{(RFHeart):{(val):%hu,(Magic):%hu}}", &rfheart, &recvMagicNum);
					if (recvMagicNum == TCLOD_MAGIC_NUM) {
						TCFG_SystemData.Heartinterval = rfheart;
						if ((TCFG_SystemData.Heartinterval > 120) || (TCFG_SystemData.Heartinterval < 1)) {
							TCFG_SystemData.Heartinterval = HEART_INTERVAL;
							TCFG_EEPROM_SetHeartinterval(TCFG_SystemData.Heartinterval);
							ret = NETIP_ERRORPARAM;
						}
						else {
							TCFG_EEPROM_SetHeartinterval(TCFG_SystemData.Heartinterval);
						}
					}
					else {
						ret = NETIP_UNKNOWNERROR;
					}
				}
				/* 初始化传感器指令 */
				else if (strstr((char *)messageHandler->message->payload + recvBufOffset + TCLOD_DATA_OFFSET, "Background") != NULL) {
					u16 backgroundval;
					sscanf((char *)messageHandler->message->payload + recvBufOffset + TCLOD_DATA_OFFSET, "{(Background):{(val):%hu,(Magic):%hu}}", &backgroundval, &recvMagicNum);
					if (recvMagicNum == TCLOD_MAGIC_NUM) {
						if (((radar_targetinfo.strenth_total_diff > (backgroundval-5)) && (radar_targetinfo.strenth_total_diff < (backgroundval+5))) || (backgroundval == 0)) {
							Radar_InitBackground(TO_SAVE_RADAR_BACKGROUND);
							QMC5883L_InitBackgroud();
						}
						else {
							ret = NETIP_ERRORPARAM;
						}
					}
					else {
						ret = NETIP_UNKNOWNERROR;
					}
				}
				/* Reboot */
				else if (strstr((char *)messageHandler->message->payload + recvBufOffset + TCLOD_DATA_OFFSET, "Reboot") != NULL) {
					BEEP_CtrlRepeat_Extend(2, 500, 250);
					Stm32_System_Software_Reboot();
				}
				/* NewSn */
				else if (strstr((char *)messageHandler->message->payload + recvBufOffset + TCLOD_DATA_OFFSET, "Newsn") != NULL) {
					u32 newsnval;
					sscanf((char *)messageHandler->message->payload + recvBufOffset + TCLOD_DATA_OFFSET, "{(Newsn):{(val):%08x,(Magic):%hu}}", &newsnval, &recvMagicNum);
					if (recvMagicNum == TCLOD_MAGIC_NUM) {
						TCFG_EEPROM_Set_MAC_SN(newsnval);
					}
					else {
						ret = NETIP_UNKNOWNERROR;
					}
				}
				/* Active */
				else if (strstr((char *)messageHandler->message->payload + recvBufOffset + TCLOD_DATA_OFFSET, "Active") != NULL) {
					u16 activeval;
					sscanf((char *)messageHandler->message->payload + recvBufOffset + TCLOD_DATA_OFFSET, "{(Active):{(val):%hu,(Magic):%hu}}", &activeval, &recvMagicNum);
					if (recvMagicNum == TCLOD_MAGIC_NUM) {
						TCFG_EEPROM_SetActiveDevice(activeval);
						if (activeval) {
							DeviceActivedMode = true;
							BEEP_CtrlRepeat_Extend(5,30,70);
						}
						else {
							DeviceActivedMode = false;
							BEEP_CtrlRepeat_Extend(1,500,0);
						}
					}
					else {
						ret = NETIP_UNKNOWNERROR;
					}
				}
				/* RadarDbg */
				else if (strstr((char *)messageHandler->message->payload + recvBufOffset + TCLOD_DATA_OFFSET, "RadarDbg") != NULL) {
					u16 radarDbgval;
					sscanf((char *)messageHandler->message->payload + recvBufOffset + TCLOD_DATA_OFFSET, "{(RadarDbg):{(val):%hu,(Magic):%hu}}", &radarDbgval, &recvMagicNum);
					if (recvMagicNum == TCLOD_MAGIC_NUM) {
						TCFG_EEPROM_SetRadarDbgMode(radarDbgval);
						TCFG_SystemData.RadarDbgMode = TCFG_EEPROM_GetRadarDbgMode();
						#if NBMQTTSN_SENDCODE_DYNAMIC_INFO
						NETMqttSNNeedSendCode.InfoDynamic = 1;
						#endif
					}
					else {
						ret = NETIP_UNKNOWNERROR;
					}
				}
				/* MagMod */
				else if (strstr((char *)messageHandler->message->payload + recvBufOffset + TCLOD_DATA_OFFSET, "MagMod") != NULL) {
					u16 magmodval;
					sscanf((char *)messageHandler->message->payload + recvBufOffset + TCLOD_DATA_OFFSET, "{(MagMod):{(val):%hu,(Magic):%hu}}", &magmodval, &recvMagicNum);
					if (recvMagicNum == TCLOD_MAGIC_NUM) {
						TCFG_EEPROM_SetMagMode(magmodval);
						talgo_set_magmod(magmodval);
					}
					else {
						ret = NETIP_UNKNOWNERROR;
					}
				}
				/* NbHeart */
				else if (strstr((char *)messageHandler->message->payload + recvBufOffset + TCLOD_DATA_OFFSET, "NbHeart") != NULL) {
					u16 nbheartval;
					sscanf((char *)messageHandler->message->payload + recvBufOffset + TCLOD_DATA_OFFSET, "{(NbHeart):{(val):%hu,(Magic):%hu}}", &nbheartval, &recvMagicNum);
					if (recvMagicNum == TCLOD_MAGIC_NUM) {
						TCFG_EEPROM_SetNbiotHeart(nbheartval);
						TCFG_SystemData.NBIotHeart = TCFG_EEPROM_GetNbiotHeart();
					}
					else {
						ret = NETIP_UNKNOWNERROR;
					}
				}
				/* InitRadar */
				else if (strstr((char *)messageHandler->message->payload + recvBufOffset + TCLOD_DATA_OFFSET, "InitRadar") != NULL) {
					u32 i32, j32, k32, m32;
					sscanf((char *)messageHandler->message->payload + recvBufOffset + TCLOD_DATA_OFFSET, \
						"{(InitRadar):{(v23456):%u,(v7890a):%u,(vbcdef):%u,(vg):%u,(Magic):%hu}}", &i32, &j32, &k32, &m32, &recvMagicNum);
					if (recvMagicNum == TCLOD_MAGIC_NUM) {
						Radar_InitBG_Cmd(i32, j32, k32, m32);
						#if NBMQTTSN_SENDCODE_RADAR_INFO
						NETMqttSNNeedSendCode.InfoRadar = 1;
						#endif
					}
					else {
						ret = NETIP_UNKNOWNERROR;
					}
				}
				/* InitMag */
				else if (strstr((char *)messageHandler->message->payload + recvBufOffset + TCLOD_DATA_OFFSET, "InitMag") != NULL) {
					s16 x, y, z;
					sscanf((char *)messageHandler->message->payload + recvBufOffset + TCLOD_DATA_OFFSET, \
						"{(InitMag):{(x):%hd,(y):%hd,(z):%hd,(Magic):%hu}}", &x, &y, &z, &recvMagicNum);
					if (recvMagicNum == TCLOD_MAGIC_NUM) {
						QMC5883L_InitBackgroud_cmd(x, y, z);
					}
					else {
						ret = NETIP_UNKNOWNERROR;
					}
				}
				/* DisRange */
				else if (strstr((char *)messageHandler->message->payload + recvBufOffset + TCLOD_DATA_OFFSET, "DisRange") != NULL) {
					u16 disrangeval;
					sscanf((char *)messageHandler->message->payload + recvBufOffset + TCLOD_DATA_OFFSET, "{(DisRange):{(val):%hu,(Magic):%hu}}", &disrangeval, &recvMagicNum);
					if (recvMagicNum == TCLOD_MAGIC_NUM) {
						tradar_set_distance_range(disrangeval + 4);
						TCFG_EEPROM_SetRadarRange(disrangeval);
						TCFG_SystemData.RadarRange = TCFG_EEPROM_GetRadarRange();
					}
					else {
						ret = NETIP_UNKNOWNERROR;
					}
				}
				/* CarInDelay */
				else if (strstr((char *)messageHandler->message->payload + recvBufOffset + TCLOD_DATA_OFFSET, "InDelay") != NULL) {
					u16 indelayval;
					sscanf((char *)messageHandler->message->payload + recvBufOffset + TCLOD_DATA_OFFSET, "{(InDelay):{(val):%hu,(Magic):%hu}}", &indelayval, &recvMagicNum);
					if (recvMagicNum == TCLOD_MAGIC_NUM) {
						TCFG_EEPROM_SetCarInDelay(indelayval);
						TCFG_SystemData.CarInDelay = TCFG_EEPROM_GetCarInDelay();
					}
					else {
						ret = NETIP_UNKNOWNERROR;
					}
				}
				/* MagCoef */
				else if (strstr((char *)messageHandler->message->payload + recvBufOffset + TCLOD_DATA_OFFSET, "MagCoef") != NULL) {
					short magTempCoefX, magTempCoefY, magTempCoefZ;
					sscanf((char *)messageHandler->message->payload + recvBufOffset + TCLOD_DATA_OFFSET, \
						"{(MagCoef):{(x):%hd,(y):%hd,(z):%hd,(Magic):%hu}}", &magTempCoefX, &magTempCoefY, &magTempCoefZ, &recvMagicNum);
					if (recvMagicNum == TCLOD_MAGIC_NUM) {
						TCFG_SystemData.MagCoefX = magTempCoefX;
						TCFG_SystemData.MagCoefY = magTempCoefY;
						TCFG_SystemData.MagCoefZ = magTempCoefZ;
						TCFG_EEPROM_SetMagTempCoef(TCFG_SystemData.MagCoefX, TCFG_SystemData.MagCoefY, TCFG_SystemData.MagCoefZ);
					}
					else {
						ret = NETIP_UNKNOWNERROR;
					}
				}
				/* SetQmcCoef */
				else if (strstr((char *)messageHandler->message->payload + recvBufOffset + TCLOD_DATA_OFFSET, "SetQmcCoef") != NULL) {
					short magTempCoefX, magTempCoefY, magTempCoefZ;
					QMC5883L_measure_qmc_coef((signed char*)&magTempCoefX, (signed char*)&magTempCoefY, (signed char*)&magTempCoefZ);
					TCFG_SystemData.MagCoefX = magTempCoefX;
					TCFG_SystemData.MagCoefY = magTempCoefY;
					TCFG_SystemData.MagCoefZ = magTempCoefZ;
					TCFG_EEPROM_SetMagTempCoef(TCFG_SystemData.MagCoefX, TCFG_SystemData.MagCoefY, TCFG_SystemData.MagCoefZ);
				}
				/* BeepOff */
				else if (strstr((char *)messageHandler->message->payload + recvBufOffset + TCLOD_DATA_OFFSET, "BeepOff") != NULL) {
					u16 beepoff;
					sscanf((char *)messageHandler->message->payload + recvBufOffset + TCLOD_DATA_OFFSET, "{(BeepOff):{(val):%hu,(Magic):%hu}}", &beepoff, &recvMagicNum);
					if (recvMagicNum == TCLOD_MAGIC_NUM) {
						TCFG_SystemData.BeepCtrlOff = beepoff;
						TCFG_EEPROM_SetBeepOff(TCFG_SystemData.BeepCtrlOff);
					}
					else {
						ret = NETIP_UNKNOWNERROR;
					}
				}
				/* ...... */
			}
			else if (messageHandler->message->payload[recvBufOffset + TCLOD_MSGID_OFFSET] == TCLOD_CONFIG_GET) {
				BEEP_CtrlRepeat_Extend(2, 50, 50);
				/* InfoWork */
				if (strstr((char *)messageHandler->message->payload + recvBufOffset + TCLOD_DATA_OFFSET, "Workinfo") != NULL) {
				#if NBMQTTSN_SENDCODE_WORK_INFO
					NETMqttSNNeedSendCode.InfoWork = 1;
				#endif
				}
				/* InfoBasic */
				else if (strstr((char *)messageHandler->message->payload + recvBufOffset + TCLOD_DATA_OFFSET, "BasicInfo") != NULL) {
				#if NBMQTTSN_SENDCODE_BASIC_INFO
					NETMqttSNNeedSendCode.InfoBasic = 1;
				#endif
				}
				/* InfoDynamic */
				else if (strstr((char *)messageHandler->message->payload + recvBufOffset + TCLOD_DATA_OFFSET, "DynamicInfo") != NULL) {
				#if NBMQTTSN_SENDCODE_DYNAMIC_INFO
					NETMqttSNNeedSendCode.InfoDynamic = 1;
				#endif
				}
				/* ...... */
			}
		}
		else {
			ret = NETIP_NOTSUPPORT;
		}
	}
	else {
		/* Not Valid */
		ret = NETIP_NOTVALID;
	}
	
	NETMqttSNNeedSendCode.InfoResponseErrcode = ret;
	NETMqttSNNeedSendCode.InfoResponse = 1;
	
	return MQTTSN_OK;
}

/**********************************************************************************************************
 @Function			MQTTSN_StatusTypeDef NET_MQTTSN_SendPayloadPacket(MQTTSN_ClientsTypeDef* pClient, NET_MQTTSN_ObjectPacketTypeDef ObjectPacket)
 @Description			NET_MQTTSN_SendPayloadPacket	: MQTTSN发送负载包
 @Input				pClient					: MqttSN客户端实例
					ObjectPacket				: 对象类型
 @Return				MQTTSN_StatusTypeDef		: MQTTSN处理状态
**********************************************************************************************************/
MQTTSN_StatusTypeDef NET_MQTTSN_SendPayloadPacket(MQTTSN_ClientsTypeDef* pClient, NET_MQTTSN_ObjectPacketTypeDef ObjectPacket)
{
	MQTTSN_StatusTypeDef MQTTSNStatus = MQTTSN_OK;
#if MQTTSN_MSG_VERSION_STREAM_TYPE == MQTTSN_MSG_VERSION_JSON_STREAM
	NET_Message_TcldMsgTypeDef *pMsg = (NET_Message_TcldMsgTypeDef*)pClient->DataProcessStack;
#endif
#if MQTTSN_MSG_VERSION_STREAM_TYPE == MQTTSN_MSG_VERSION_BYTE_STREAM
	unsigned char* pMsg = pClient->DataProcessStack;
	unsigned short pMsgLen = 0;
#endif
	MQTTSN_MessageTypeDef message;
	MQTTSN_topicid topic;
	
	memset(pClient->DataProcessStack, 0x0, pClient->DataProcessStack_size);
	
	message.qos		= QOS1;
	message.msgid		= 1;
	message.dup		= 0;
	message.retained	= 0;
	message.payload	= pClient->DataProcessStack;
	
	/* seclect different data and send */
	switch (ObjectPacket)
	{
		case OBJECT_TYPE_TMOTES_STATUS_BASIC_PUT:
		{
#if MQTTSN_MSG_VERSION_STREAM_TYPE == MQTTSN_MSG_VERSION_JSON_STREAM
#if NBMQTTSN_SENDCODE_STATUS_BASIC
			NET_MESSAGE_GET_MAGICNUM(pMsg->MsgHead.MagicNum);
			pMsg->MsgHead.MsgType		=	MSG_JSON;
			pMsg->MsgHead.Version		=	MESSAGE_VERSION;
			pMsg->MsgHead.EncryptMode	=	ENCRYPT_NONE;
			pMsg->MsgHead.MsgId			=	MSGID_PUT;
			
			message.payloadlen			=	NET_Message_Operate_Creat_Json_MoteStatus_Basic(pMsg->pData) + sizeof(NET_Message_TcldHeadTypeDef);
			
			topic.type = MQTTSN_TOPIC_TYPE_PREDEFINED;
			topic.data.id = TOPICID_MOTESTATUS;
			if ((MQTTSNStatus = MQTTSN_Publish(pClient, topic, &message)) != MQTTSN_OK) {
				//Todo
			}
#endif
#endif
			break;
		}
		
		case OBJECT_TYPE_TMOTES_STATUS_EXTEND_PUT:
		{
#if MQTTSN_MSG_VERSION_STREAM_TYPE == MQTTSN_MSG_VERSION_JSON_STREAM
#if NBMQTTSN_SENDCODE_STATUS_EXTEND
			NET_MESSAGE_GET_MAGICNUM(pMsg->MsgHead.MagicNum);
			pMsg->MsgHead.MsgType		=	MSG_JSON;
			pMsg->MsgHead.Version		=	MESSAGE_VERSION;
			pMsg->MsgHead.EncryptMode	=	ENCRYPT_NONE;
			pMsg->MsgHead.MsgId			=	MSGID_PUT;
			
			message.payloadlen			=	NET_Message_Operate_Creat_Json_MoteStatus_Extend(pMsg->pData) + sizeof(NET_Message_TcldHeadTypeDef);
			
			topic.type = MQTTSN_TOPIC_TYPE_PREDEFINED;
			topic.data.id = TOPICID_MOTESTATUS;
			if ((MQTTSNStatus = MQTTSN_Publish(pClient, topic, &message)) != MQTTSN_OK) {
				//Todo
			}
#endif
#endif
			break;
		}
		
		case OBJECT_TYPE_TMOTES_INFO_WORK_PUT:
		{
#if MQTTSN_MSG_VERSION_STREAM_TYPE == MQTTSN_MSG_VERSION_JSON_STREAM
#if NBMQTTSN_SENDCODE_WORK_INFO
			NET_MESSAGE_GET_MAGICNUM(pMsg->MsgHead.MagicNum);
			pMsg->MsgHead.MsgType		=	MSG_JSON;
			pMsg->MsgHead.Version		=	MESSAGE_VERSION;
			pMsg->MsgHead.EncryptMode	=	ENCRYPT_NONE;
			pMsg->MsgHead.MsgId			=	MSGID_PUT;
			
			message.payloadlen			=	NET_Message_Operate_Creat_Json_MoteInfo_Work(pMsg->pData) + sizeof(NET_Message_TcldHeadTypeDef);
			
			topic.type = MQTTSN_TOPIC_TYPE_PREDEFINED;
			topic.data.id = TOPICID_MOTEINFO;
			if ((MQTTSNStatus = MQTTSN_Publish(pClient, topic, &message)) != MQTTSN_OK) {
				//Todo
			}
#endif
#endif
			break;
		}
		
		case OBJECT_TYPE_TMOTES_INFO_BASIC_PUT:
		{
#if MQTTSN_MSG_VERSION_STREAM_TYPE == MQTTSN_MSG_VERSION_JSON_STREAM
#if NBMQTTSN_SENDCODE_BASIC_INFO
			NET_MESSAGE_GET_MAGICNUM(pMsg->MsgHead.MagicNum);
			pMsg->MsgHead.MsgType		=	MSG_JSON;
			pMsg->MsgHead.Version		=	MESSAGE_VERSION;
			pMsg->MsgHead.EncryptMode	=	ENCRYPT_NONE;
			pMsg->MsgHead.MsgId			=	MSGID_PUT;
			
			message.payloadlen			=	NET_Message_Operate_Creat_Json_MoteInfo_Basic(pMsg->pData) + sizeof(NET_Message_TcldHeadTypeDef);
			
			topic.type = MQTTSN_TOPIC_TYPE_PREDEFINED;
			topic.data.id = TOPICID_MOTEINFO;
			if ((MQTTSNStatus = MQTTSN_Publish(pClient, topic, &message)) != MQTTSN_OK) {
				//Todo
			}
#endif
#endif
			break;
		}
		
		case OBJECT_TYPE_TMOTES_INFO_DYNAMIC_PUT:
		{
#if MQTTSN_MSG_VERSION_STREAM_TYPE == MQTTSN_MSG_VERSION_JSON_STREAM
#if NBMQTTSN_SENDCODE_DYNAMIC_INFO
			NET_MESSAGE_GET_MAGICNUM(pMsg->MsgHead.MagicNum);
			pMsg->MsgHead.MsgType		=	MSG_JSON;
			pMsg->MsgHead.Version		=	MESSAGE_VERSION;
			pMsg->MsgHead.EncryptMode	=	ENCRYPT_NONE;
			pMsg->MsgHead.MsgId			=	MSGID_PUT;
			
			message.payloadlen			=	NET_Message_Operate_Creat_Json_MoteInfo_Dynamic(pMsg->pData) + sizeof(NET_Message_TcldHeadTypeDef);
			
			topic.type = MQTTSN_TOPIC_TYPE_PREDEFINED;
			topic.data.id = TOPICID_MOTEINFO;
			if ((MQTTSNStatus = MQTTSN_Publish(pClient, topic, &message)) != MQTTSN_OK) {
				//Todo
			}
#endif
#endif
			break;
		}
		
		case OBJECT_TYPE_TMOTES_INFO_RADAR_PUT:
		{
#if MQTTSN_MSG_VERSION_STREAM_TYPE == MQTTSN_MSG_VERSION_JSON_STREAM
#if NBMQTTSN_SENDCODE_RADAR_INFO
			NET_MESSAGE_GET_MAGICNUM(pMsg->MsgHead.MagicNum);
			pMsg->MsgHead.MsgType		=	MSG_JSON;
			pMsg->MsgHead.Version		=	MESSAGE_VERSION;
			pMsg->MsgHead.EncryptMode	=	ENCRYPT_NONE;
			pMsg->MsgHead.MsgId			=	MSGID_PUT;
			
			message.payloadlen			=	NET_Message_Operate_Creat_Json_MoteInfo_Radar(pMsg->pData) + sizeof(NET_Message_TcldHeadTypeDef);
			
			topic.type = MQTTSN_TOPIC_TYPE_PREDEFINED;
			topic.data.id = TOPICID_MOTEINFO;
			if ((MQTTSNStatus = MQTTSN_Publish(pClient, topic, &message)) != MQTTSN_OK) {
				//Todo
			}
#endif
#endif
			break;
		}
		
		case OBJECT_TYPE_TMOTES_INFO_RESPONSE_PUT:
		{
#if MQTTSN_MSG_VERSION_STREAM_TYPE == MQTTSN_MSG_VERSION_JSON_STREAM
			NET_MESSAGE_GET_MAGICNUM(pMsg->MsgHead.MagicNum);
			pMsg->MsgHead.MsgType		=	MSG_JSON;
			pMsg->MsgHead.Version		=	MESSAGE_VERSION;
			pMsg->MsgHead.EncryptMode	=	ENCRYPT_NONE;
			pMsg->MsgHead.MsgId			=	MSGID_PUT;
			
			message.payloadlen			=	NET_Message_Operate_Creat_Json_MoteInfo_Response(pMsg->pData) + sizeof(NET_Message_TcldHeadTypeDef);
			
			topic.type = MQTTSN_TOPIC_TYPE_PREDEFINED;
			topic.data.id = TOPICID_MOTEINFO;
			if ((MQTTSNStatus = MQTTSN_Publish(pClient, topic, &message)) != MQTTSN_OK) {
				//Todo
			}
#endif
			break;
		}
		
		case OBJECT_TYPE_TMOTES_BYTE_STREAM_PUT:
		{
#if MQTTSN_MSG_VERSION_STREAM_TYPE == MQTTSN_MSG_VERSION_BYTE_STREAM
			NET_MqttSN_Message_SendDataDequeue(pMsg, &pMsgLen);
			
			message.payloadlen			=	pMsgLen;
			
			topic.type = MQTTSN_TOPIC_TYPE_PREDEFINED;
			topic.data.id = TOPICID_STANDARD;
			if ((MQTTSNStatus = MQTTSN_Publish(pClient, topic, &message)) != MQTTSN_OK) {
				//Todo
			}
#endif
			break;
		}
	}
	
	return MQTTSNStatus;
}

/********************************************** END OF FLEE **********************************************/
