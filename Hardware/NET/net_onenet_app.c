/**
  *********************************************************************************************************
  * @file    net_onenet_app.c
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

#include "net_onenet_app.h"
#include "platform_config.h"
#include "platform_map.h"
#include "hal_rtc.h"
#include "string.h"
#include "radio_rf_app.h"

/**********************************************************************************************************
 @Function			void NET_ONENET_APP_PollExecution(ONENET_ClientsTypeDef* pClient)
 @Description			ONENET_APP_PollExecution				: OneNET逻辑处理
 @Input				pClient							: OneNET客户端实例
 @Return				void
**********************************************************************************************************/
void NET_ONENET_APP_PollExecution(ONENET_ClientsTypeDef* pClient)
{
	switch (pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateEvent)
	{
	case STOP_MODE:
		NET_ONENET_NBIOT_Event_StopMode(pClient);
		break;
	
	case HARDWARE_REBOOT:
		NET_ONENET_NBIOT_Event_HardwareReboot(pClient);
		break;
	
	case REPORT_ERROE:
		NET_ONENET_NBIOT_Event_ReportError(pClient);
		break;
	
	case MODULE_CHECK:
		NET_ONENET_NBIOT_Event_ModuleCheck(pClient);
		break;
	
	case PARAMETER_CONFIG:
		NET_ONENET_NBIOT_Event_ParameterConfig(pClient);
		break;
	
	case ICCID_CHECK:
		NET_ONENET_NBIOT_Event_SimICCIDCheck(pClient);
		break;
	
	case MISC_EQUIP_CONFIG:
		pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
		break;
	
	case ATTACH_CHECK:
		NET_ONENET_NBIOT_Event_AttachCheck(pClient);
		break;
	
	case ATTACH_EXECUTE:
		NET_ONENET_NBIOT_Event_AttachExecute(pClient);
		break;
	
	case ATTACH_INQUIRE:
		NET_ONENET_NBIOT_Event_AttachInquire(pClient);
		break;
	
	case PARAMETER_CHECKOUT:
		NET_ONENET_NBIOT_Event_ParameterCheckOut(pClient);
		break;
	
	case MINIMUM_FUNCTIONALITY:
		NET_ONENET_NBIOT_Event_MinimumFunctionality(pClient);
		break;
	
	case FULL_FUNCTIONALITY:
		NET_ONENET_NBIOT_Event_FullFunctionality(pClient);
		break;
	
	case CDP_SERVER_CHECK:
		pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
		break;
	
	case CDP_SERVER_CONFIG:
		pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
		break;
	
	case NBAND_MODE_CHECK:
		NET_ONENET_NBIOT_Event_NbandModeCheck(pClient);
		break;
	
	case NBAND_MODE_CONFIG:
		NET_ONENET_NBIOT_Event_NbandModeConfig(pClient);
		break;
	
	case SEND_DATA:
		pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
		break;
	
	case RECV_DATA:
		pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
		break;
	
	case SEND_DATA_RA_NORMAL:
		pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
		break;
	
	case RECV_DATA_RA_NORMAL:
		pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
		break;
	
	case EXECUT_DOWNLINK_DATA:
		pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
		break;
	
	case MQTTSN_PROCESS_STACK:
		pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
		break;
	
	case DNS_PROCESS_STACK:
		pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
		break;
	
	case ONENET_PROCESS_STACK:
		NET_ONENET_APP_ProcessExecution(pClient);
		break;
	
	case LISTEN_RUN_CTL:
		pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
		break;
	
	default :
		pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
		break;
	}
}

/**********************************************************************************************************
 @Function			void NET_ONENET_APP_ProcessExecution(ONENET_ClientsTypeDef* pClient)
 @Description			NET_ONENET_APP_ProcessExecution		: OneNET协议逻辑处理
 @Input				pClient							: OneNET客户端实例
 @Return				void
**********************************************************************************************************/
void NET_ONENET_APP_ProcessExecution(ONENET_ClientsTypeDef* pClient)
{
	switch (pClient->ProcessState)
	{
	case ONENET_PROCESSSTATE_INIT:
		NET_ONENET_Event_Init(pClient);
		break;
	
	
	
	
	
	
	
	
	
	}
}

/**********************************************************************************************************
 @Function			static void ONENET_NBIOT_DictateEvent_SetTime(ONENET_ClientsTypeDef* pClient, unsigned int TimeoutSec)
 @Description			ONENET_NBIOT_DictateEvent_SetTime		: 事件运行控制器注入时间(内部使用)
 @Input				pClient							: OneNET客户端实例
					TimeoutSec						: 注入超时时间
 @Return				void
 @attention			事件运行之前判断是否需要注入时间
**********************************************************************************************************/
static void ONENET_NBIOT_DictateEvent_SetTime(ONENET_ClientsTypeDef* pClient, unsigned int TimeoutSec)
{
	Stm32_CalculagraphTypeDef dictateRunTime;
	
	/* It is the first time to execute */
	if (pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateEnable != true) {
		pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateEnable = true;
		pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateTimeoutSec = TimeoutSec;
		Stm32_Calculagraph_CountdownSec(&dictateRunTime, pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateTimeoutSec);
		pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateRunTime = dictateRunTime;
	}
}

/**********************************************************************************************************
 @Function			static void ONENET_DictateEvent_SetTime(ONENET_ClientsTypeDef* pClient, unsigned int TimeoutSec)
 @Description			ONENET_DictateEvent_SetTime			: 事件运行控制器注入时间(内部使用)
 @Input				pClient							: OneNET客户端实例
					TimeoutSec						: 注入超时时间
 @Return				void
 @attention			事件运行之前判断是否需要注入时间
**********************************************************************************************************/
//static void ONENET_DictateEvent_SetTime(ONENET_ClientsTypeDef* pClient, unsigned int TimeoutSec)
//{
//	Stm32_CalculagraphTypeDef dictateRunTime;
//	
//	/* It is the first time to execute */
//	if (pClient->DictateRunCtl.dictateEnable != true) {
//		pClient->DictateRunCtl.dictateEnable = true;
//		pClient->DictateRunCtl.dictateTimeoutSec = TimeoutSec;
//		Stm32_Calculagraph_CountdownSec(&dictateRunTime, pClient->DictateRunCtl.dictateTimeoutSec);
//		pClient->DictateRunCtl.dictateRunTime = dictateRunTime;
//	}
//}

static unsigned char* ONENET_NBIOT_GetDictateFailureCnt(ONENET_ClientsTypeDef* pClient, NBIOT_DictateEventTypeDef dictateNoTimeOut)
{
	unsigned char* dictateFailureCnt;
	
	switch (dictateNoTimeOut)
	{
	case HARDWARE_REBOOT:
		dictateFailureCnt = &pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateRebootFailureCnt;
		break;
	
	case REPORT_ERROE:
		dictateFailureCnt = &pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateReportErrorFailureCnt;
		break;
	
	case MODULE_CHECK:
		dictateFailureCnt = &pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateModuleCheckFailureCnt;
		break;
	
	case PARAMETER_CONFIG:
		dictateFailureCnt = &pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateParameterConfigFailureCnt;
		break;
	
	case ICCID_CHECK:
		dictateFailureCnt = &pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateSimICCIDCheckFailureCnt;
		break;
	
	case ATTACH_CHECK:
		dictateFailureCnt = &pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateAttachCheckFailureCnt;
		break;
	
	case ATTACH_EXECUTE:
		dictateFailureCnt = &pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateAttachExecuteFailureCnt;
		break;
	
	case ATTACH_INQUIRE:
		dictateFailureCnt = &pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateAttachInquireFailureCnt;
		break;
	
	case PARAMETER_CHECKOUT:
		dictateFailureCnt = &pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictatePatameterCheckOutFailureCnt;
		break;
	
	case MINIMUM_FUNCTIONALITY:
		dictateFailureCnt = &pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateMinimumFunctionalityFailureCnt;
		break;
	
	case FULL_FUNCTIONALITY:
		dictateFailureCnt = &pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateFullFunctionalityFailureCnt;
		break;
	
	case NBAND_MODE_CHECK:
		dictateFailureCnt = &pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateNbandModeCheckFailureCnt;
		break;
	
	case NBAND_MODE_CONFIG:
		dictateFailureCnt = &pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateNbandModeConfigFailureCnt;
		break;
	
	default :
		dictateFailureCnt = &pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateRebootFailureCnt;
		break;
	}
	
	return dictateFailureCnt;
}

/**********************************************************************************************************
 @Function			static void ONENET_NBIOT_DictateEvent_FailExecute(ONENET_ClientsTypeDef* pClient, NBIOT_DictateEventTypeDef dictateTimeOut, \
																					  NBIOT_DictateEventTypeDef dictateFail, \
																					  NBIOT_DictateEventTypeDef dictateNoTimeOut)
 @Description			ONENET_NBIOT_DictateEvent_FailExecute	: 事件运行控制器出错执行(内部使用)
 @Input				pClient							: OneNET客户端实例
					dictateTimeOut						: 事假处理错误超时
					dictateFail						: 事件处理错误次数溢出
					dictateNoTimeOut					: 事假处理错误未超时
 @Return				void
**********************************************************************************************************/
static void ONENET_NBIOT_DictateEvent_FailExecute(ONENET_ClientsTypeDef* pClient, NBIOT_DictateEventTypeDef dictateTimeOut, \
																  NBIOT_DictateEventTypeDef dictateFail, \
																  NBIOT_DictateEventTypeDef dictateNoTimeOut)
{
	unsigned char* dictateFailureCnt;
	
	dictateFailureCnt = ONENET_NBIOT_GetDictateFailureCnt(pClient, dictateNoTimeOut);
	
	if (Stm32_Calculagraph_IsExpiredSec(&pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateRunTime) == true) {
		/* Dictate TimeOut */
		pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateEnable = false;
		pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateEvent = dictateTimeOut;
		*dictateFailureCnt += 1;
		if (*dictateFailureCnt > 3) {
			*dictateFailureCnt = 0;
			pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateEvent = dictateFail;
		}
	}
	else {
		/* Dictate isn't TimeOut */
		pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateEvent = dictateNoTimeOut;
	}
}

/**********************************************************************************************************
 @Function			static void ONENET_NBIOT_DictateEvent_SuccessExecute(ONENET_ClientsTypeDef* pClient, NBIOT_DictateEventTypeDef dictateSuccess, \
																						NBIOT_DictateEventTypeDef dictateNoTimeOut)
 @Description			ONENET_NBIOT_DictateEvent_SuccessExecute: 事件运行控制器正确执行(内部使用)
 @Input				pClient							: OneNET客户端实例
					dictateSuccess						: 事假处理正确
					dictateNoTimeOut					: 事假处理错误未超时
 @Return				void
**********************************************************************************************************/
static void ONENET_NBIOT_DictateEvent_SuccessExecute(ONENET_ClientsTypeDef* pClient, NBIOT_DictateEventTypeDef dictateSuccess, \
																	NBIOT_DictateEventTypeDef dictateNoTimeOut)
{
	unsigned char* dictateFailureCnt;
	
	dictateFailureCnt = ONENET_NBIOT_GetDictateFailureCnt(pClient, dictateNoTimeOut);
	
	pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateEnable = false;
	pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateEvent = dictateSuccess;
	*dictateFailureCnt = 0;
}

/**********************************************************************************************************
 @Function			void NET_ONENET_NBIOT_Event_StopMode(ONENET_ClientsTypeDef* pClient)
 @Description			NET_ONENET_NBIOT_Event_StopMode		: 停止模式
 @Input				pClient							: OneNET客户端实例
 @Return				void
 @attention			当30分钟或有数据需要发送时退出停止模式尝试重启NB,开启NB运行
**********************************************************************************************************/
void NET_ONENET_NBIOT_Event_StopMode(ONENET_ClientsTypeDef* pClient)
{
	Stm32_CalculagraphTypeDef dictateRunTime;
	
	/* It is the first time to execute */
	if (pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateEnable != true) {
		pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateEnable = true;
		pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateTimeoutSec = 1800;
		Stm32_Calculagraph_CountdownSec(&dictateRunTime, pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateTimeoutSec);
		pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateRunTime = dictateRunTime;
		/* NBIOT Module Poweroff */
		NBIOT_Neul_NBxx_HardwarePoweroff(pClient->LWM2MStack->NBIotStack);
		/* Init Message Index */
		
		
		
	}
	
	if (Stm32_Calculagraph_IsExpiredSec(&pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateRunTime) == true) {
		/* Dictate TimeOut */
		pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateEnable = false;
		pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
		pClient->ProcessState = ONENET_PROCESSSTATE_INIT;
		pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_ONENET;
	}
	else {
		/* Dictate isn't TimeOut */
		
		
		
	}
}

/**********************************************************************************************************
 @Function			void NET_ONENET_NBIOT_Event_HardwareReboot(ONENET_ClientsTypeDef* pClient)
 @Description			NET_ONENET_NBIOT_Event_HardwareReboot	: 硬件重启
 @Input				pClient							: OneNET客户端实例
 @Return				void
**********************************************************************************************************/
void NET_ONENET_NBIOT_Event_HardwareReboot(ONENET_ClientsTypeDef* pClient)
{
	ONENET_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if (NBIOT_Neul_NBxx_HardwareReboot(pClient->LWM2MStack->NBIotStack, 8000) == NBIOT_OK) {
		/* Dictate execute is Success */
#if NBIOT_PRINT_ERROR_CODE_TYPE
		ONENET_NBIOT_DictateEvent_SuccessExecute(pClient, REPORT_ERROE, HARDWARE_REBOOT);
#else
		ONENET_NBIOT_DictateEvent_SuccessExecute(pClient, MODULE_CHECK, HARDWARE_REBOOT);
#endif
		
#ifdef ONENET_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("NB HDRBT Ok, Baud:%d", NBIOTBaudRate.Baud);
#endif
	}
	else {
		/* Dictate execute is Fail */
		ONENET_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, HARDWARE_REBOOT);
		
#ifdef ONENET_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("NB HDRBT Fail");
#endif
	}
}

/**********************************************************************************************************
 @Function			void NET_ONENET_NBIOT_Event_ReportError(ONENET_ClientsTypeDef* pClient)
 @Description			NET_ONENET_NBIOT_Event_ReportError		: 错误码输出
 @Input				pClient							: OneNET客户端实例
 @Return				void
**********************************************************************************************************/
void NET_ONENET_NBIOT_Event_ReportError(ONENET_ClientsTypeDef* pClient)
{
	ONENET_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if (NBIOT_Neul_NBxx_SetReportTerminationError(pClient->LWM2MStack->NBIotStack, CMEEnable) == NBIOT_OK) {
		/* Dictate execute is Success */
		ONENET_NBIOT_DictateEvent_SuccessExecute(pClient, MODULE_CHECK, REPORT_ERROE);
		
#ifdef ONENET_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("NB ReportErrorCode Set %d Ok", CMEEnable);
#endif
	}
	else {
		/* Dictate execute is Fail */
		ONENET_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, REPORT_ERROE);
		
#ifdef ONENET_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("NB ReportErrorCode Set %d Fail", CMEEnable);
#endif
	}
}

/**********************************************************************************************************
 @Function			void NET_ONENET_NBIOT_Event_ModuleCheck(ONENET_ClientsTypeDef* pClient)
 @Description			NET_ONENET_NBIOT_Event_ModuleCheck		: 模块检测
 @Input				pClient							: OneNET客户端实例
 @Return				void
**********************************************************************************************************/
void NET_ONENET_NBIOT_Event_ModuleCheck(ONENET_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBStatus;
	
	ONENET_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if (((NBStatus = NBIOT_Neul_NBxx_CheckReadManufacturer(pClient->LWM2MStack->NBIotStack)) == NBIOT_OK) && 
	    ((NBStatus = NBIOT_Neul_NBxx_CheckReadManufacturerModel(pClient->LWM2MStack->NBIotStack)) == NBIOT_OK) &&
	    ((NBStatus = NBIOT_Neul_NBxx_CheckReadModuleVersion(pClient->LWM2MStack->NBIotStack)) == NBIOT_OK)) {
		/* Dictate execute is Success */
		ONENET_NBIOT_DictateEvent_SuccessExecute(pClient, PARAMETER_CONFIG, MODULE_CHECK);
		
#ifdef ONENET_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("NB Module Check Ok");
#endif
	}
	else {
		/* Dictate execute is Fail */
		ONENET_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, MODULE_CHECK);
		
#ifdef ONENET_DEBUG_LOG_RF_PRINT
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		Radio_Trf_Debug_Printf_Level2("NB Module Check Fail ECde %d", NBStatus);
	#else
		Radio_Trf_Debug_Printf_Level2("NB Module Check Fail");
	#endif
#endif
	}
}

/**********************************************************************************************************
 @Function			void NET_ONENET_NBIOT_Event_ParameterConfig(ONENET_ClientsTypeDef* pClient)
 @Description			NET_ONENET_NBIOT_Event_ParameterConfig	: 参数配置
 @Input				pClient							: OneNET客户端实例
 @Return				void
**********************************************************************************************************/
void NET_ONENET_NBIOT_Event_ParameterConfig(ONENET_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBStatus;
	
	ONENET_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if ((NBStatus = NBIOT_Neul_NBxx_CheckReadConfigUE(pClient->LWM2MStack->NBIotStack)) == NBIOT_OK) {
		/* Dictate execute is Success */
		ONENET_NBIOT_DictateEvent_SuccessExecute(pClient, ICCID_CHECK, PARAMETER_CONFIG);
		
#ifdef ONENET_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("NB Parameter Config Read Ok");
#endif
	}
	else {
		/* Dictate execute is Fail */
		ONENET_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, PARAMETER_CONFIG);
		
#ifdef ONENET_DEBUG_LOG_RF_PRINT
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		Radio_Trf_Debug_Printf_Level2("NB Parameter Config Read Fail ECde %d", NBStatus);
	#else
		Radio_Trf_Debug_Printf_Level2("NB Parameter Config Read Fail");
	#endif
#endif
		return;
	}
	
	if (pClient->LWM2MStack->NBIotStack->Parameter.nconfig.autoConnect != AutoConnectVal) {
		if (NBIOT_Neul_NBxx_SetConfigUE(pClient->LWM2MStack->NBIotStack, AutoConnect, AutoConnectVal) == NBIOT_OK) {
			/* Dictate execute is Success */
			ONENET_NBIOT_DictateEvent_SuccessExecute(pClient, ICCID_CHECK, PARAMETER_CONFIG);
			
#ifdef ONENET_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("NB %s %d Ok", AutoConnect, AutoConnectVal);
#endif
		}
		else {
			/* Dictate execute is Fail */
			ONENET_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, PARAMETER_CONFIG);
			
#ifdef ONENET_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("NB %s %d Fail", AutoConnect, AutoConnectVal);
#endif
			return;
		}
	}
	
	if (pClient->LWM2MStack->NBIotStack->Parameter.nconfig.crScrambling != CrScramblingVal) {
		if (NBIOT_Neul_NBxx_SetConfigUE(pClient->LWM2MStack->NBIotStack, CrScrambling, CrScramblingVal) == NBIOT_OK) {
			/* Dictate execute is Success */
			ONENET_NBIOT_DictateEvent_SuccessExecute(pClient, ICCID_CHECK, PARAMETER_CONFIG);
			
#ifdef ONENET_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("NB %s %d Ok", CrScrambling, CrScramblingVal);
#endif
		}
		else {
			/* Dictate execute is Fail */
			ONENET_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, PARAMETER_CONFIG);
			
#ifdef ONENET_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("NB %s %d Fail", CrScrambling, CrScramblingVal);
#endif
			return;
		}
	}
	
	if (pClient->LWM2MStack->NBIotStack->Parameter.nconfig.crSiAvoid != CrSiAvoidVal) {
		if (NBIOT_Neul_NBxx_SetConfigUE(pClient->LWM2MStack->NBIotStack, CrSiAvoid, CrSiAvoidVal) == NBIOT_OK) {
			/* Dictate execute is Success */
			ONENET_NBIOT_DictateEvent_SuccessExecute(pClient, ICCID_CHECK, PARAMETER_CONFIG);
			
#ifdef ONENET_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("NB %s %d Ok", CrSiAvoid, CrSiAvoidVal);
#endif
		}
		else {
			/* Dictate execute is Fail */
			ONENET_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, PARAMETER_CONFIG);
			
#ifdef ONENET_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("NB %s %d Fail", CrSiAvoid, CrSiAvoidVal);
#endif
			return;
		}
	}
	
	if (pClient->LWM2MStack->NBIotStack->Parameter.nconfig.combineAttach != CombineAttachVal) {
		if (NBIOT_Neul_NBxx_SetConfigUE(pClient->LWM2MStack->NBIotStack, CombineAttach, CombineAttachVal) == NBIOT_OK) {
			/* Dictate execute is Success */
			ONENET_NBIOT_DictateEvent_SuccessExecute(pClient, ICCID_CHECK, PARAMETER_CONFIG);
			
#ifdef ONENET_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("NB %s %d Ok", CombineAttach, CombineAttachVal);
#endif
		}
		else {
			/* Dictate execute is Fail */
			ONENET_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, PARAMETER_CONFIG);
			
#ifdef ONENET_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("NB %s %d Fail", CombineAttach, CombineAttachVal);
#endif
			return;
		}
	}
	
	if (pClient->LWM2MStack->NBIotStack->Parameter.nconfig.cellReselection != CellReselectionVal) {
		if (NBIOT_Neul_NBxx_SetConfigUE(pClient->LWM2MStack->NBIotStack, CellReselection, CellReselectionVal) == NBIOT_OK) {
			/* Dictate execute is Success */
			ONENET_NBIOT_DictateEvent_SuccessExecute(pClient, ICCID_CHECK, PARAMETER_CONFIG);
			
#ifdef ONENET_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("NB %s %d Ok", CellReselection, CellReselectionVal);
#endif
		}
		else {
			/* Dictate execute is Fail */
			ONENET_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, PARAMETER_CONFIG);
			
#ifdef ONENET_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("NB %s %d Fail", CellReselection, CellReselectionVal);
#endif
			return;
		}
	}
	
	if (pClient->LWM2MStack->NBIotStack->Parameter.nconfig.enableBip != EnableBipVal) {
		if (NBIOT_Neul_NBxx_SetConfigUE(pClient->LWM2MStack->NBIotStack, EnableBip, EnableBipVal) == NBIOT_OK) {
			/* Dictate execute is Success */
			ONENET_NBIOT_DictateEvent_SuccessExecute(pClient, ICCID_CHECK, PARAMETER_CONFIG);
			
#ifdef ONENET_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("NB %s %d Ok", EnableBip, EnableBipVal);
#endif
		}
		else {
			/* Dictate execute is Fail */
			ONENET_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, PARAMETER_CONFIG);
			
#ifdef ONENET_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("NB %s %d Fail", EnableBip, EnableBipVal);
#endif
			return;
		}
	}
}

/**********************************************************************************************************
 @Function			void NET_ONENET_NBIOT_Event_SimICCIDCheck(ONENET_ClientsTypeDef* pClient)
 @Description			NET_ONENET_NBIOT_Event_SimICCIDCheck	: Sim卡检测
 @Input				pClient							: OneNET客户端实例
 @Return				void
**********************************************************************************************************/
void NET_ONENET_NBIOT_Event_SimICCIDCheck(ONENET_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBStatus;
	
	ONENET_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if ((NBStatus = NBIOT_Neul_NBxx_CheckReadICCID(pClient->LWM2MStack->NBIotStack)) == NBIOT_OK) {
		/* Dictate execute is Success */
		ONENET_NBIOT_DictateEvent_SuccessExecute(pClient, FULL_FUNCTIONALITY, ICCID_CHECK);
		
#ifdef ONENET_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("NB ICCID Check Ok");
#endif
	}
	else {
		/* Dictate execute is Fail */
		ONENET_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, ICCID_CHECK);
		
#ifdef ONENET_DEBUG_LOG_RF_PRINT
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		Radio_Trf_Debug_Printf_Level2("NB ICCID Check Fail ECde %d", NBStatus);
	#else
		Radio_Trf_Debug_Printf_Level2("NB ICCID Check Fail");
	#endif
#endif
	}
}

/**********************************************************************************************************
 @Function			void NET_ONENET_NBIOT_Event_FullFunctionality(ONENET_ClientsTypeDef* pClient)
 @Description			NET_ONENET_NBIOT_Event_FullFunctionality: 完整功能
 @Input				pClient							: OneNET客户端实例
 @Return				void
**********************************************************************************************************/
void NET_ONENET_NBIOT_Event_FullFunctionality(ONENET_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBStatus;
	
	ONENET_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if ((NBStatus = NBIOT_Neul_NBxx_CheckReadMinOrFullFunc(pClient->LWM2MStack->NBIotStack)) == NBIOT_OK) {
		/* Dictate execute is Success */
		ONENET_NBIOT_DictateEvent_SuccessExecute(pClient, NBAND_MODE_CHECK, FULL_FUNCTIONALITY);
		
#ifdef ONENET_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("NB FullFunc Check Ok");
#endif
	}
	else {
		/* Dictate execute is Fail */
		ONENET_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, FULL_FUNCTIONALITY);
		
#ifdef ONENET_DEBUG_LOG_RF_PRINT
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		Radio_Trf_Debug_Printf_Level2("NB FullFunc Check Fail ECde %d", NBStatus);
	#else
		Radio_Trf_Debug_Printf_Level2("NB FullFunc Check Fail");
	#endif
#endif
		return;
	}
	
	if (pClient->LWM2MStack->NBIotStack->Parameter.functionality != FullFunc) {
		if ((NBStatus = NBIOT_Neul_NBxx_SetMinOrFullFunc(pClient->LWM2MStack->NBIotStack, FullFunc)) == NBIOT_OK) {
			/* Dictate execute is Success */
			ONENET_NBIOT_DictateEvent_SuccessExecute(pClient, NBAND_MODE_CHECK, FULL_FUNCTIONALITY);
			
#ifdef ONENET_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("NB FullFunc Set Ok");
#endif
		}
		else {
			/* Dictate execute is Fail */
			ONENET_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, FULL_FUNCTIONALITY);
			
#ifdef ONENET_DEBUG_LOG_RF_PRINT
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
 @Function			void NET_ONENET_NBIOT_Event_MinimumFunctionality(ONENET_ClientsTypeDef* pClient)
 @Description			NET_ONENET_NBIOT_Event_MinimumFunctionality	: 最小功能
 @Input				pClient								: OneNET客户端实例
 @Return				void
**********************************************************************************************************/
void NET_ONENET_NBIOT_Event_MinimumFunctionality(ONENET_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBStatus;
	
	ONENET_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if ((NBStatus = NBIOT_Neul_NBxx_CheckReadMinOrFullFunc(pClient->LWM2MStack->NBIotStack)) == NBIOT_OK) {
		/* Dictate execute is Success */
		ONENET_NBIOT_DictateEvent_SuccessExecute(pClient, NBAND_MODE_CONFIG, MINIMUM_FUNCTIONALITY);
		
#ifdef ONENET_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("NB MinFunc Check Ok");
#endif
	}
	else {
		/* Dictate execute is Fail */
		ONENET_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, MINIMUM_FUNCTIONALITY);
		
#ifdef ONENET_DEBUG_LOG_RF_PRINT
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		Radio_Trf_Debug_Printf_Level2("NB MinFunc Check Fail ECde %d", NBStatus);
	#else
		Radio_Trf_Debug_Printf_Level2("NB MinFunc Check Fail");
	#endif
#endif
		return;
	}
	
	if (pClient->LWM2MStack->NBIotStack->Parameter.functionality != MinFunc) {
		if ((NBStatus = NBIOT_Neul_NBxx_SetMinOrFullFunc(pClient->LWM2MStack->NBIotStack, MinFunc)) == NBIOT_OK) {
			/* Dictate execute is Success */
			ONENET_NBIOT_DictateEvent_SuccessExecute(pClient, NBAND_MODE_CONFIG, MINIMUM_FUNCTIONALITY);
			
#ifdef ONENET_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("NB MinFunc Set Ok");
#endif
		}
		else {
			/* Dictate execute is Fail */
			ONENET_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, MINIMUM_FUNCTIONALITY);
			
#ifdef ONENET_DEBUG_LOG_RF_PRINT
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
 @Function			void NET_ONENET_NBIOT_Event_NbandModeCheck(ONENET_ClientsTypeDef* pClient)
 @Description			NET_ONENET_NBIOT_Event_NbandModeCheck	: NBAND模式查询
 @Input				pClient							: OneNET客户端实例
 @Return				void
**********************************************************************************************************/
void NET_ONENET_NBIOT_Event_NbandModeCheck(ONENET_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBStatus;
	
	ONENET_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if ((NBStatus = NBIOT_Neul_NBxx_CheckReadSupportedBands(pClient->LWM2MStack->NBIotStack)) == NBIOT_OK) {
		/* Dictate execute is Success */
		ONENET_NBIOT_DictateEvent_SuccessExecute(pClient, NBAND_MODE_CHECK, NBAND_MODE_CHECK);
		
#ifdef ONENET_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("NB BAND Read %d Ok", pClient->LWM2MStack->NBIotStack->Parameter.band);
#endif
	}
	else {
		/* Dictate execute is Fail */
		ONENET_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, NBAND_MODE_CHECK);
		
#ifdef ONENET_DEBUG_LOG_RF_PRINT
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		Radio_Trf_Debug_Printf_Level2("NB BAND Read Fail ECde %d", NBStatus);
	#else
		Radio_Trf_Debug_Printf_Level2("NB BAND Read Fail");
	#endif
#endif
	}
	
	if (pClient->LWM2MStack->NBIotStack->Parameter.band != ONENET_NBIOT_BAND_TYPE) {
		/* BAND Mode Mast be Config */
		pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateEvent = MINIMUM_FUNCTIONALITY;
	}
	else {
		/* BAND Mode Needn't be Config */
		pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateEvent = ATTACH_CHECK;
	}
}

/**********************************************************************************************************
 @Function			void NET_ONENET_NBIOT_Event_NbandModeConfig(ONENET_ClientsTypeDef* pClient)
 @Description			NET_ONENET_NBIOT_Event_NbandModeConfig	: NBAND模式配置
 @Input				pClient							: OneNET客户端实例
 @Return				void
**********************************************************************************************************/
void NET_ONENET_NBIOT_Event_NbandModeConfig(ONENET_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBStatus;
	
	ONENET_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if ((NBStatus = NBIOT_Neul_NBxx_CheckReadSupportedBands(pClient->LWM2MStack->NBIotStack)) == NBIOT_OK) {
		/* Dictate execute is Success */
		ONENET_NBIOT_DictateEvent_SuccessExecute(pClient, FULL_FUNCTIONALITY, NBAND_MODE_CONFIG);
		
#ifdef ONENET_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("NB BAND Read %d Ok", pClient->LWM2MStack->NBIotStack->Parameter.band);
#endif
	}
	else {
		/* Dictate execute is Fail */
		ONENET_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, NBAND_MODE_CONFIG);
		
#ifdef ONENET_DEBUG_LOG_RF_PRINT
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		Radio_Trf_Debug_Printf_Level2("NB BAND Read Fail ECde %d", NBStatus);
	#else
		Radio_Trf_Debug_Printf_Level2("NB BAND Read Fail");
	#endif
#endif
	}
	
	if (pClient->LWM2MStack->NBIotStack->Parameter.band != ONENET_NBIOT_BAND_TYPE) {
		/* BAND Mode Mast be Config */
		if ((NBStatus = NBIOT_Neul_NBxx_SetSupportedBands(pClient->LWM2MStack->NBIotStack, ONENET_NBIOT_BAND_TYPE)) == NBIOT_OK) {
			/* Dictate execute is Success */
			ONENET_NBIOT_DictateEvent_SuccessExecute(pClient, FULL_FUNCTIONALITY, NBAND_MODE_CONFIG);
			
#ifdef ONENET_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("NB BAND Set %d Ok", ONENET_NBIOT_BAND_TYPE);
#endif
		}
		else {
			/* Dictate execute is Fail */
			ONENET_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, NBAND_MODE_CONFIG);
			
#ifdef ONENET_DEBUG_LOG_RF_PRINT
		#if NBIOT_PRINT_ERROR_CODE_TYPE
			Radio_Trf_Debug_Printf_Level2("NB BAND Set %d Fail ECde %d", ONENET_NBIOT_BAND_TYPE, NBStatus);
		#else
			Radio_Trf_Debug_Printf_Level2("NB BAND Set %d Fail", ONENET_NBIOT_BAND_TYPE);
		#endif
#endif
			return;
		}
	}
	else {
		/* BAND Mode Needn't be Config */
		pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateEvent = FULL_FUNCTIONALITY;
	}
}

/**********************************************************************************************************
 @Function			void NET_ONENET_NBIOT_Event_AttachCheck(ONENET_ClientsTypeDef* pClient)
 @Description			NET_ONENET_NBIOT_Event_AttachCheck		: 注网检查
 @Input				pClient							: OneNET客户端实例
 @Return				void
**********************************************************************************************************/
void NET_ONENET_NBIOT_Event_AttachCheck(ONENET_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBStatus;
	
	ONENET_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if ((NBStatus = NBIOT_Neul_NBxx_CheckReadAttachOrDetach(pClient->LWM2MStack->NBIotStack)) == NBIOT_OK) {
		/* Dictate execute is Success */
		ONENET_NBIOT_DictateEvent_SuccessExecute(pClient, ATTACH_CHECK, ATTACH_CHECK);
		
#ifdef ONENET_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("NB CGATT %d Ok", pClient->LWM2MStack->NBIotStack->Parameter.netstate);
#endif
	}
	else {
		/* Dictate execute is Fail */
		ONENET_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, ATTACH_CHECK);
		
#ifdef ONENET_DEBUG_LOG_RF_PRINT
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		Radio_Trf_Debug_Printf_Level2("NB CGATT %d Fail ECde %d", pClient->LWM2MStack->NBIotStack->Parameter.netstate, NBStatus);
	#else
		Radio_Trf_Debug_Printf_Level2("NB CGATT %d Fail", pClient->LWM2MStack->NBIotStack->Parameter.netstate);
	#endif
#endif
		return;
	}
	
	if (pClient->LWM2MStack->NBIotStack->Parameter.netstate != Attach) {
		pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateEvent = ATTACH_EXECUTE;
	}
	else {
		pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateEvent = ATTACH_INQUIRE;
	}
}

/**********************************************************************************************************
 @Function			void NET_ONENET_NBIOT_Event_AttachExecute(ONENET_ClientsTypeDef* pClient)
 @Description			NET_ONENET_NBIOT_Event_AttachExecute	: 注网进行
 @Input				pClient							: OneNET客户端实例
 @Return				void
**********************************************************************************************************/
void NET_ONENET_NBIOT_Event_AttachExecute(ONENET_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBStatus;
	
	ONENET_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if ((NBStatus = NBIOT_Neul_NBxx_SetAttachOrDetach(pClient->LWM2MStack->NBIotStack, Attach)) == NBIOT_OK) {
		/* Dictate execute is Success */
		ONENET_NBIOT_DictateEvent_SuccessExecute(pClient, ATTACH_INQUIRE, ATTACH_EXECUTE);
		
#ifdef ONENET_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("NB Set CGATT %d Ok", Attach);
#endif
	}
	else {
		/* Dictate execute is Fail */
		ONENET_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, ATTACH_EXECUTE);
		
#ifdef ONENET_DEBUG_LOG_RF_PRINT
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		Radio_Trf_Debug_Printf_Level2("NB Set CGATT %d Fail ECde %d", Attach, NBStatus);
	#else
		Radio_Trf_Debug_Printf_Level2("NB Set CGATT %d Fail", Attach);
	#endif
#endif
	}
}

/**********************************************************************************************************
 @Function			void NET_ONENET_NBIOT_Event_AttachInquire(ONENET_ClientsTypeDef* pClient)
 @Description			NET_ONENET_NBIOT_Event_AttachInquire	: 注网查询
 @Input				pClient							: OneNET客户端实例
 @Return				void
**********************************************************************************************************/
void NET_ONENET_NBIOT_Event_AttachInquire(ONENET_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBStatus;
	
	ONENET_NBIOT_DictateEvent_SetTime(pClient, 60);
	
	if ((NBStatus = NBIOT_Neul_NBxx_CheckReadAttachOrDetach(pClient->LWM2MStack->NBIotStack)) == NBIOT_OK) {
		/* Dictate execute is Success */
		pClient->LWM2MStack->NBIotStack->DictateRunCtl.dictateEvent = ATTACH_INQUIRE;
#ifdef ONENET_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("NB CGATT %d Ok", pClient->LWM2MStack->NBIotStack->Parameter.netstate);
#endif
	}
	else {
		/* Dictate execute is Fail */
		ONENET_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, ATTACH_INQUIRE);
		
#ifdef ONENET_DEBUG_LOG_RF_PRINT
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		Radio_Trf_Debug_Printf_Level2("NB CGATT %d Fail ECde %d", pClient->LWM2MStack->NBIotStack->Parameter.netstate, NBStatus);
	#else
		Radio_Trf_Debug_Printf_Level2("NB CGATT %d Fail", pClient->LWM2MStack->NBIotStack->Parameter.netstate);
	#endif
#endif
		return;
	}
	
	if (pClient->LWM2MStack->NBIotStack->Parameter.netstate != Attach) {
		ONENET_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, ATTACH_INQUIRE);
	}
	else {
		ONENET_NBIOT_DictateEvent_SuccessExecute(pClient, PARAMETER_CHECKOUT, ATTACH_INQUIRE);
	}
}

/**********************************************************************************************************
 @Function			void NET_ONENET_NBIOT_Event_ParameterCheckOut(ONENET_ClientsTypeDef* pClient)
 @Description			NET_ONENET_NBIOT_Event_ParameterCheckOut: 参数检出
 @Input				pClient							: OneNET客户端实例
 @Return				void
**********************************************************************************************************/
void NET_ONENET_NBIOT_Event_ParameterCheckOut(ONENET_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBStatus;
	
	ONENET_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if (((NBStatus = NBIOT_Neul_NBxx_CheckReadIMEI(pClient->LWM2MStack->NBIotStack)) == NBIOT_OK) && 
	    ((NBStatus = NBIOT_Neul_NBxx_CheckReadIMEISV(pClient->LWM2MStack->NBIotStack)) == NBIOT_OK) && 
	    ((NBStatus = NBIOT_Neul_NBxx_CheckReadIMSI(pClient->LWM2MStack->NBIotStack)) == NBIOT_OK) && 
	    ((NBStatus = NBIOT_Neul_NBxx_CheckReadCGPADDR(pClient->LWM2MStack->NBIotStack)) == NBIOT_OK) && 
	    ((NBStatus = NBIOT_Neul_NBxx_CheckReadCGDCONT(pClient->LWM2MStack->NBIotStack)) == NBIOT_OK) && 
	    ((NBStatus = NBIOT_Neul_NBxx_CheckReadRSSI(pClient->LWM2MStack->NBIotStack)) == NBIOT_OK) && 
	    ((NBStatus = NBIOT_Neul_NBxx_CheckReadStatisticsRADIO(pClient->LWM2MStack->NBIotStack)) == NBIOT_OK) && 
	    ((NBStatus = NBIOT_Neul_NBxx_CheckReadStatisticsCELL(pClient->LWM2MStack->NBIotStack)) == NBIOT_OK) && 
	    ((NBStatus = NBIOT_Neul_NBxx_CheckReadDateTime(pClient->LWM2MStack->NBIotStack)) == NBIOT_OK)) {
		/* Dictate execute is Success */
		ONENET_NBIOT_DictateEvent_SuccessExecute(pClient, ONENET_PROCESS_STACK, PARAMETER_CHECKOUT);
		
		pClient->LWM2MStack->NBIotStack->Registered = true;
		
#ifdef ONENET_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("NB Parameter Check Ok");
#endif
	}
	else {
		/* Dictate execute is Fail */
		ONENET_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, PARAMETER_CHECKOUT);
		
#ifdef ONENET_DEBUG_LOG_RF_PRINT
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		Radio_Trf_Debug_Printf_Level2("NB Parameter Check Fail ECde %d", NBStatus);
	#else
		Radio_Trf_Debug_Printf_Level2("NB Parameter Check Fail");
	#endif
#endif
		return;
	}
	
	/* Set System Time */
	RTC_Set_Date(pClient->LWM2MStack->NBIotStack->Parameter.dataTime.year, pClient->LWM2MStack->NBIotStack->Parameter.dataTime.month, pClient->LWM2MStack->NBIotStack->Parameter.dataTime.day);
	RTC_Set_Time(pClient->LWM2MStack->NBIotStack->Parameter.dataTime.hour, pClient->LWM2MStack->NBIotStack->Parameter.dataTime.min,   pClient->LWM2MStack->NBIotStack->Parameter.dataTime.sec);
}

/**********************************************************************************************************
 @Function			void NET_ONENET_Event_Init(ONENET_ClientsTypeDef* pClient)
 @Description			NET_ONENET_Event_Init				: INIT
 @Input				pClient							: OneNET客户端实例
 @Return				void
**********************************************************************************************************/
void NET_ONENET_Event_Init(ONENET_ClientsTypeDef* pClient)
{
#ifdef DegOneNET
	ONENET_StatusTypeDef retError;
	s32 refer;
	
	retError = NBIOT_OneNET_Related_AccessConfig_AckTimeout(pClient, 20);
	
	retError = NBIOT_OneNET_Related_Create_SuiteInstance(pClient, &refer);
	retError = NBIOT_OneNET_Related_Add_LwM2MObject(pClient, refer, 6203, 2, (sc8*)"11", 1, 0);
	retError = NBIOT_OneNET_Related_Send_RegisterRequest(pClient, refer, 3600, 60);
	
	retError = NBIOT_OneNET_Related_Respond_DiscoverRequest(pClient, refer, 50944, 1, 4, (sc8*)"1000", (sc8*)"0x200");
	retError = NBIOT_OneNET_Related_Respond_DiscoverRequest(pClient, refer, 50944, 1, 4, (sc8*)"1000", NULL);
	
	retError = NBIOT_OneNET_Related_Send_DeregisterRequest(pClient, refer);
	retError = NBIOT_OneNET_Related_Del_LwM2MObject(pClient, refer, 6203);
	retError = NBIOT_OneNET_Related_Delete_SuiteInstance(pClient, refer);
	
	retError = retError;
#endif
}




















































/********************************************** END OF FLEE **********************************************/
