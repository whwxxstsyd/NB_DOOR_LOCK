/**
  *********************************************************************************************************
  * @file    net_dns_app.c
  * @author  Movebroad -- KK
  * @version V1.0
  * @date    2018-05-16
  * @brief   
  *********************************************************************************************************
  * @attention
  *
  *
  *
  *********************************************************************************************************
  */

#include "net_dns_app.h"
#include "hal_rtc.h"
#include "string.h"
#include "radio_rf_app.h"

/**********************************************************************************************************
 @Function			void NET_DNS_APP_PollExecution(DNS_ClientsTypeDef* pClient)
 @Description			DNS_APP_PollExecution				: DNS逻辑处理
 @Input				pClient							: DNS客户端实例
 @Return				void
**********************************************************************************************************/
void NET_DNS_APP_PollExecution(DNS_ClientsTypeDef* pClient)
{
	switch (pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent)
	{
	case STOP_MODE:
		NET_DNS_NBIOT_Event_StopMode(pClient);
		break;
	
	case HARDWARE_REBOOT:
		NET_DNS_NBIOT_Event_HardwareReboot(pClient);
		break;
	
	case REPORT_ERROE:
		NET_DNS_NBIOT_Event_ReportError(pClient);
		break;
	
	case MODULE_CHECK:
		NET_DNS_NBIOT_Event_ModuleCheck(pClient);
		break;
	
	case PARAMETER_CONFIG:
		NET_DNS_NBIOT_Event_ParameterConfig(pClient);
		break;
	
	case ICCID_CHECK:
		NET_DNS_NBIOT_Event_SimICCIDCheck(pClient);
		break;
	
	case MISC_EQUIP_CONFIG:
		pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
		break;
	
	case ATTACH_CHECK:
		NET_DNS_NBIOT_Event_AttachCheck(pClient);
		break;
	
	case ATTACH_EXECUTE:
		NET_DNS_NBIOT_Event_AttachExecute(pClient);
		break;
	
	case ATTACH_INQUIRE:
		NET_DNS_NBIOT_Event_AttachInquire(pClient);
		break;
	
	case PARAMETER_CHECKOUT:
		NET_DNS_NBIOT_Event_ParameterCheckOut(pClient);
		break;
	
	case MINIMUM_FUNCTIONALITY:
		NET_DNS_NBIOT_Event_MinimumFunctionality(pClient);
		break;
	
	case FULL_FUNCTIONALITY:
		NET_DNS_NBIOT_Event_FullFunctionality(pClient);
		break;
	
	case CDP_SERVER_CHECK:
		pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
		break;
	
	case CDP_SERVER_CONFIG:
		pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
		break;
	
	case NBAND_MODE_CHECK:
		NET_DNS_NBIOT_Event_NbandModeCheck(pClient);
		break;
	
	case NBAND_MODE_CONFIG:
		NET_DNS_NBIOT_Event_NbandModeConfig(pClient);
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
		pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
		break;
	
	case DNS_PROCESS_STACK:
		NET_DNS_APP_ProcessExecution(pClient);
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
 @Function			void NET_DNS_APP_ProcessExecution(DNS_ClientsTypeDef* pClient)
 @Description			NET_DNS_APP_ProcessExecution			: DNS协议逻辑处理
 @Input				pClient							: DNS客户端实例
 @Return				void
**********************************************************************************************************/
void NET_DNS_APP_ProcessExecution(DNS_ClientsTypeDef* pClient)
{
	switch (pClient->ProcessState)
	{
	case DNS_PROCESS_CREAT_UDP_SOCKET:
		NET_DNS_Event_CreatUDPSocket(pClient);
		break;
	
	case DNS_PROCESS_SEND_DNS_STRUCT_DATA:
		NET_DNS_Event_SendDnsStructData(pClient);
		break;
	
	case DNS_PROCESS_RECV_DNS_STRUCT_DATA:
		NET_DNS_Event_RecvDnsStructData(pClient);
		break;
	
	case DNS_PROCESS_CLOSE_UDP_SOCKET:
		NET_DNS_Event_CloseUDPSocket(pClient);
		break;
	
	case DNS_PROCESS_OVER_DNS_ANALYSIS:
		NET_DNS_Event_OverDnsAnalysis(pClient);
		break;
	}
}

/**********************************************************************************************************
 @Function			static void DNS_NBIOT_DictateEvent_SetTime(DNS_ClientsTypeDef* pClient, unsigned int TimeoutSec)
 @Description			DNS_NBIOT_DictateEvent_SetTime		: 事件运行控制器注入时间(内部使用)
 @Input				pClient							: DNS客户端实例
					TimeoutSec						: 注入超时时间
 @Return				void
 @attention			事件运行之前判断是否需要注入时间
**********************************************************************************************************/
static void DNS_NBIOT_DictateEvent_SetTime(DNS_ClientsTypeDef* pClient, unsigned int TimeoutSec)
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
 @Function			static void DNS_DictateEvent_SetTime(DNS_ClientsTypeDef* pClient, unsigned int TimeoutSec)
 @Description			DNS_DictateEvent_SetTime				: 事件运行控制器注入时间(内部使用)
 @Input				pClient							: DNS客户端实例
					TimeoutSec						: 注入超时时间
 @Return				void
 @attention			事件运行之前判断是否需要注入时间
**********************************************************************************************************/
static void DNS_DictateEvent_SetTime(DNS_ClientsTypeDef* pClient, unsigned int TimeoutSec)
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

static unsigned char* DNS_NBIOT_GetDictateFailureCnt(DNS_ClientsTypeDef* pClient, NBIOT_DictateEventTypeDef dictateNoTimeOut)
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
 @Function			static void DNS_NBIOT_DictateEvent_FailExecute(DNS_ClientsTypeDef* pClient, NBIOT_DictateEventTypeDef dictateTimeOut, \
																				 NBIOT_DictateEventTypeDef dictateFail, \
																				 NBIOT_DictateEventTypeDef dictateNoTimeOut)
 @Description			DNS_NBIOT_DictateEvent_FailExecute		: 事件运行控制器出错执行(内部使用)
 @Input				pClient							: DNS客户端实例
					dictateTimeOut						: 事假处理错误超时
					dictateFail						: 事件处理错误次数溢出
					dictateNoTimeOut					: 事假处理错误未超时
 @Return				void
**********************************************************************************************************/
static void DNS_NBIOT_DictateEvent_FailExecute(DNS_ClientsTypeDef* pClient, NBIOT_DictateEventTypeDef dictateTimeOut, \
															 NBIOT_DictateEventTypeDef dictateFail, \
															 NBIOT_DictateEventTypeDef dictateNoTimeOut)
{
	unsigned char* dictateFailureCnt;
	
	dictateFailureCnt = DNS_NBIOT_GetDictateFailureCnt(pClient, dictateNoTimeOut);
	
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
 @Function			static void DNS_NBIOT_DictateEvent_SuccessExecute(DNS_ClientsTypeDef* pClient, NBIOT_DictateEventTypeDef dictateSuccess, \
																				    NBIOT_DictateEventTypeDef dictateNoTimeOut)
 @Description			DNS_NBIOT_DictateEvent_SuccessExecute	: 事件运行控制器正确执行(内部使用)
 @Input				pClient							: DNS客户端实例
					dictateSuccess						: 事假处理正确
					dictateNoTimeOut					: 事假处理错误未超时
 @Return				void
**********************************************************************************************************/
static void DNS_NBIOT_DictateEvent_SuccessExecute(DNS_ClientsTypeDef* pClient, NBIOT_DictateEventTypeDef dictateSuccess, \
															    NBIOT_DictateEventTypeDef dictateNoTimeOut)
{
	unsigned char* dictateFailureCnt;
	
	dictateFailureCnt = DNS_NBIOT_GetDictateFailureCnt(pClient, dictateNoTimeOut);
	
	pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEnable = false;
	pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = dictateSuccess;
	*dictateFailureCnt = 0;
}

/**********************************************************************************************************
 @Function			void NET_DNS_NBIOT_Event_StopMode(DNS_ClientsTypeDef* pClient)
 @Description			NET_DNS_NBIOT_Event_StopMode			: 停止模式
 @Input				pClient							: DNS客户端实例
 @Return				void
 @attention			当30分钟或有数据需要发送时退出停止模式尝试重启NB,开启NB运行
**********************************************************************************************************/
void NET_DNS_NBIOT_Event_StopMode(DNS_ClientsTypeDef* pClient)
{
	Stm32_CalculagraphTypeDef dictateRunTime;
#if MQTTSN_MSG_VERSION_STREAM_TYPE == MQTTSN_MSG_VERSION_JSON_STREAM
	static unsigned char DNSMqttSNStatusBasicIndex;
	static unsigned char DNSMqttSNStatusExtendIndex;
	static unsigned char DNSMqttSNInfoWorkIndex;
	static unsigned char DNSMqttSNInfoBasicIndex;
	static unsigned char DNSMqttSNInfoDynamicIndex;
	static unsigned char DNSMqttSNInfoRadarIndex;
	static unsigned char DNSMqttSNInfoResponseIndex;
#endif
#if MQTTSN_MSG_VERSION_STREAM_TYPE == MQTTSN_MSG_VERSION_BYTE_STREAM
	static unsigned char DNSByteStreamIndex;
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
		DNSMqttSNStatusBasicIndex = NET_MqttSN_Message_StatusBasicRear();
		DNSMqttSNStatusExtendIndex = NET_MqttSN_Message_StatusExtendRear();
		DNSMqttSNInfoWorkIndex = NET_MqttSN_Message_InfoWorkRear();
		DNSMqttSNInfoBasicIndex = NET_MqttSN_Message_InfoBasicRear();
		DNSMqttSNInfoDynamicIndex = NET_MqttSN_Message_InfoDynamicRear();
		DNSMqttSNInfoRadarIndex = NET_MqttSN_Message_InfoRadarRear();
		DNSMqttSNInfoResponseIndex = NET_MqttSN_Message_InfoResponseRear();
#endif
#if MQTTSN_MSG_VERSION_STREAM_TYPE == MQTTSN_MSG_VERSION_BYTE_STREAM
		DNSByteStreamIndex = NET_MqttSN_Message_SendDataRear();
#endif
	}
	
	if (Stm32_Calculagraph_IsExpiredSec(&pClient->SocketStack->NBIotStack->DictateRunCtl.dictateRunTime) == true) {
		/* Dictate TimeOut */
		pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEnable = false;
		pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
		pClient->ProcessState = DNS_PROCESS_CREAT_UDP_SOCKET;
		pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_DNS;
	}
	else {
		/* Dictate isn't TimeOut */
#if MQTTSN_MSG_VERSION_STREAM_TYPE == MQTTSN_MSG_VERSION_JSON_STREAM
		if ( (NET_MqttSN_Message_StatusBasicRear() != DNSMqttSNStatusBasicIndex) || 
			(NET_MqttSN_Message_StatusExtendRear() != DNSMqttSNStatusExtendIndex) ||
			(NET_MqttSN_Message_InfoWorkRear() != DNSMqttSNInfoWorkIndex) ||
			(NET_MqttSN_Message_InfoBasicRear() != DNSMqttSNInfoBasicIndex) ||
			(NET_MqttSN_Message_InfoDynamicRear() != DNSMqttSNInfoDynamicIndex) ||
			(NET_MqttSN_Message_InfoRadarRear() != DNSMqttSNInfoRadarIndex) ||
			(NET_MqttSN_Message_InfoResponseRear() != DNSMqttSNInfoResponseIndex) ) {
#endif
#if MQTTSN_MSG_VERSION_STREAM_TYPE == MQTTSN_MSG_VERSION_BYTE_STREAM
		if ( NET_MqttSN_Message_SendDataRear() != DNSByteStreamIndex ) {
#endif
			pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEnable = false;
			pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
			pClient->ProcessState = DNS_PROCESS_CREAT_UDP_SOCKET;
			pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_DNS;
		}
		else {
			pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = STOP_MODE;
			pClient->ProcessState = DNS_PROCESS_CREAT_UDP_SOCKET;
			pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_DNS;
		}
	}
}

/**********************************************************************************************************
 @Function			void NET_DNS_NBIOT_Event_HardwareReboot(DNS_ClientsTypeDef* pClient)
 @Description			NET_DNS_NBIOT_Event_HardwareReboot		: 硬件重启
 @Input				pClient							: DNS客户端实例
 @Return				void
**********************************************************************************************************/
void NET_DNS_NBIOT_Event_HardwareReboot(DNS_ClientsTypeDef* pClient)
{
	DNS_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if (NBIOT_Neul_NBxx_HardwareReboot(pClient->SocketStack->NBIotStack, 8000) == NBIOT_OK) {
		/* Dictate execute is Success */
#if NBIOT_PRINT_ERROR_CODE_TYPE
		DNS_NBIOT_DictateEvent_SuccessExecute(pClient, REPORT_ERROE, HARDWARE_REBOOT);
#else
		DNS_NBIOT_DictateEvent_SuccessExecute(pClient, MODULE_CHECK, HARDWARE_REBOOT);
#endif
		
#ifdef DNS_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("NB HDRBT Ok, Baud:%d", NBIOTBaudRate.Baud);
#endif
	}
	else {
		/* Dictate execute is Fail */
		DNS_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, HARDWARE_REBOOT);
		
#ifdef DNS_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("NB HDRBT Fail");
#endif
	}
}

/**********************************************************************************************************
 @Function			void NET_DNS_NBIOT_Event_ReportError(DNS_ClientsTypeDef* pClient)
 @Description			NET_DNS_NBIOT_Event_ReportError		: 错误码输出
 @Input				pClient							: DNS客户端实例
 @Return				void
**********************************************************************************************************/
void NET_DNS_NBIOT_Event_ReportError(DNS_ClientsTypeDef* pClient)
{
	DNS_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if ((NBIOT_Neul_NBxx_SetReportTerminationError(pClient->SocketStack->NBIotStack, CMEEnable) == NBIOT_OK)) {
		/* Dictate execute is Success */
		DNS_NBIOT_DictateEvent_SuccessExecute(pClient, MODULE_CHECK, REPORT_ERROE);
		
#ifdef DNS_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("NB ReportErrorCode Set %d Ok", CMEEnable);
#endif
	}
	else {
		/* Dictate execute is Fail */
		DNS_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, REPORT_ERROE);
		
#ifdef DNS_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("NB ReportErrorCode Set %d Fail", CMEEnable);
#endif
	}
}

/**********************************************************************************************************
 @Function			void NET_DNS_NBIOT_Event_ModuleCheck(DNS_ClientsTypeDef* pClient)
 @Description			NET_DNS_NBIOT_Event_ModuleCheck		: 模块检测
 @Input				pClient							: DNS客户端实例
 @Return				void
**********************************************************************************************************/
void NET_DNS_NBIOT_Event_ModuleCheck(DNS_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBStatus;
	
	DNS_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if (((NBStatus = NBIOT_Neul_NBxx_CheckReadManufacturer(pClient->SocketStack->NBIotStack)) == NBIOT_OK) && 
	    ((NBStatus = NBIOT_Neul_NBxx_CheckReadManufacturerModel(pClient->SocketStack->NBIotStack)) == NBIOT_OK) && 
	    ((NBStatus = NBIOT_Neul_NBxx_CheckReadModuleVersion(pClient->SocketStack->NBIotStack)) == NBIOT_OK)) {
		/* Dictate execute is Success */
		DNS_NBIOT_DictateEvent_SuccessExecute(pClient, PARAMETER_CONFIG, MODULE_CHECK);
		
#ifdef DNS_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("NB Module Check Ok");
#endif
	}
	else {
		/* Dictate execute is Fail */
		DNS_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, MODULE_CHECK);
		
#ifdef DNS_DEBUG_LOG_RF_PRINT
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		Radio_Trf_Debug_Printf_Level2("NB Module Check Fail ECde %d", NBStatus);
	#else
		Radio_Trf_Debug_Printf_Level2("NB Module Check Fail");
	#endif
#endif
	}
}

/**********************************************************************************************************
 @Function			void NET_DNS_NBIOT_Event_ParameterConfig(DNS_ClientsTypeDef* pClient)
 @Description			NET_DNS_NBIOT_Event_ParameterConfig	: 参数配置
 @Input				pClient							: DNS客户端实例
 @Return				void
**********************************************************************************************************/
void NET_DNS_NBIOT_Event_ParameterConfig(DNS_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBStatus;
	
	DNS_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if ((NBStatus = NBIOT_Neul_NBxx_CheckReadConfigUE(pClient->SocketStack->NBIotStack)) == NBIOT_OK) {
		/* Dictate execute is Success */
		DNS_NBIOT_DictateEvent_SuccessExecute(pClient, ICCID_CHECK, PARAMETER_CONFIG);
		
#ifdef DNS_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("NB Parameter Config Read Ok");
#endif
	}
	else {
		/* Dictate execute is Fail */
		DNS_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, PARAMETER_CONFIG);
		
#ifdef DNS_DEBUG_LOG_RF_PRINT
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
			DNS_NBIOT_DictateEvent_SuccessExecute(pClient, ICCID_CHECK, PARAMETER_CONFIG);
			
#ifdef DNS_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("NB %s %d Ok", AutoConnect, AutoConnectVal);
#endif
		}
		else {
			/* Dictate execute is Fail */
			DNS_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, PARAMETER_CONFIG);
			
#ifdef DNS_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("NB %s %d Fail", AutoConnect, AutoConnectVal);
#endif
			return;
		}
	}
	
	if (pClient->SocketStack->NBIotStack->Parameter.nconfig.crScrambling != CrScramblingVal) {
		if (NBIOT_Neul_NBxx_SetConfigUE(pClient->SocketStack->NBIotStack, CrScrambling, CrScramblingVal) == NBIOT_OK) {
			/* Dictate execute is Success */
			DNS_NBIOT_DictateEvent_SuccessExecute(pClient, ICCID_CHECK, PARAMETER_CONFIG);
			
#ifdef DNS_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("NB %s %d Ok", CrScrambling, CrScramblingVal);
#endif
		}
		else {
			/* Dictate execute is Fail */
			DNS_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, PARAMETER_CONFIG);
			
#ifdef DNS_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("NB %s %d Fail", CrScrambling, CrScramblingVal);
#endif
			return;
		}
	}
	
	if (pClient->SocketStack->NBIotStack->Parameter.nconfig.crSiAvoid != CrSiAvoidVal) {
		if (NBIOT_Neul_NBxx_SetConfigUE(pClient->SocketStack->NBIotStack, CrSiAvoid, CrSiAvoidVal) == NBIOT_OK) {
			/* Dictate execute is Success */
			DNS_NBIOT_DictateEvent_SuccessExecute(pClient, ICCID_CHECK, PARAMETER_CONFIG);
			
#ifdef DNS_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("NB %s %d Ok", CrSiAvoid, CrSiAvoidVal);
#endif
		}
		else {
			/* Dictate execute is Fail */
			DNS_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, PARAMETER_CONFIG);
			
#ifdef DNS_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("NB %s %d Fail", CrSiAvoid, CrSiAvoidVal);
#endif
			return;
		}
	}
	
	if (pClient->SocketStack->NBIotStack->Parameter.nconfig.combineAttach != CombineAttachVal) {
		if (NBIOT_Neul_NBxx_SetConfigUE(pClient->SocketStack->NBIotStack, CombineAttach, CombineAttachVal) == NBIOT_OK) {
			/* Dictate execute is Success */
			DNS_NBIOT_DictateEvent_SuccessExecute(pClient, ICCID_CHECK, PARAMETER_CONFIG);
			
#ifdef DNS_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("NB %s %d Ok", CombineAttach, CombineAttachVal);
#endif
		}
		else {
			/* Dictate execute is Fail */
			DNS_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, PARAMETER_CONFIG);
			
#ifdef DNS_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("NB %s %d Fail", CombineAttach, CombineAttachVal);
#endif
			return;
		}
	}
	
	if (pClient->SocketStack->NBIotStack->Parameter.nconfig.cellReselection != CellReselectionVal) {
		if (NBIOT_Neul_NBxx_SetConfigUE(pClient->SocketStack->NBIotStack, CellReselection, CellReselectionVal) == NBIOT_OK) {
			/* Dictate execute is Success */
			DNS_NBIOT_DictateEvent_SuccessExecute(pClient, ICCID_CHECK, PARAMETER_CONFIG);
			
#ifdef DNS_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("NB %s %d Ok", CellReselection, CellReselectionVal);
#endif
		}
		else {
			/* Dictate execute is Fail */
			DNS_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, PARAMETER_CONFIG);
			
#ifdef DNS_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("NB %s %d Fail", CellReselection, CellReselectionVal);
#endif
			return;
		}
	}
	
	if (pClient->SocketStack->NBIotStack->Parameter.nconfig.enableBip != EnableBipVal) {
		if (NBIOT_Neul_NBxx_SetConfigUE(pClient->SocketStack->NBIotStack, EnableBip, EnableBipVal) == NBIOT_OK) {
			/* Dictate execute is Success */
			DNS_NBIOT_DictateEvent_SuccessExecute(pClient, ICCID_CHECK, PARAMETER_CONFIG);
			
#ifdef DNS_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("NB %s %d Ok", EnableBip, EnableBipVal);
#endif
		}
		else {
			/* Dictate execute is Fail */
			DNS_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, PARAMETER_CONFIG);
			
#ifdef DNS_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("NB %s %d Fail", EnableBip, EnableBipVal);
#endif
			return;
		}
	}
}

/**********************************************************************************************************
 @Function			void NET_NET_NBIOT_Event_SimICCIDCheck(DNS_ClientsTypeDef* pClient)
 @Description			NET_NET_NBIOT_Event_SimICCIDCheck		: Sim卡检测
 @Input				pClient							: DNS客户端实例
 @Return				void
**********************************************************************************************************/
void NET_DNS_NBIOT_Event_SimICCIDCheck(DNS_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBStatus;
	
	DNS_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if ((NBStatus = NBIOT_Neul_NBxx_CheckReadICCID(pClient->SocketStack->NBIotStack)) == NBIOT_OK) {
		/* Dictate execute is Success */
		DNS_NBIOT_DictateEvent_SuccessExecute(pClient, FULL_FUNCTIONALITY, ICCID_CHECK);
		
#ifdef DNS_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("NB ICCID Check Ok");
#endif
	}
	else {
		/* Dictate execute is Fail */
		DNS_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, ICCID_CHECK);
		
#ifdef DNS_DEBUG_LOG_RF_PRINT
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		Radio_Trf_Debug_Printf_Level2("NB ICCID Check Fail ECde %d", NBStatus);
	#else
		Radio_Trf_Debug_Printf_Level2("NB ICCID Check Fail");
	#endif
#endif
	}
}

/**********************************************************************************************************
 @Function			void NET_DNS_NBIOT_Event_FullFunctionality(DNS_ClientsTypeDef* pClient)
 @Description			NET_DNS_NBIOT_Event_FullFunctionality		: 完整功能
 @Input				pClient								: DNS客户端实例
 @Return				void
**********************************************************************************************************/
void NET_DNS_NBIOT_Event_FullFunctionality(DNS_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBStatus;
	
	DNS_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if ((NBStatus = NBIOT_Neul_NBxx_CheckReadMinOrFullFunc(pClient->SocketStack->NBIotStack)) == NBIOT_OK) {
		/* Dictate execute is Success */
		DNS_NBIOT_DictateEvent_SuccessExecute(pClient, NBAND_MODE_CHECK, FULL_FUNCTIONALITY);
		
#ifdef DNS_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("NB FullFunc Check Ok");
#endif
	}
	else {
		/* Dictate execute is Fail */
		DNS_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, FULL_FUNCTIONALITY);
		
#ifdef DNS_DEBUG_LOG_RF_PRINT
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
			DNS_NBIOT_DictateEvent_SuccessExecute(pClient, NBAND_MODE_CHECK, FULL_FUNCTIONALITY);
			
#ifdef DNS_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("NB FullFunc Set Ok");
#endif
		}
		else {
			/* Dictate execute is Fail */
			DNS_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, FULL_FUNCTIONALITY);
			
#ifdef DNS_DEBUG_LOG_RF_PRINT
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
 @Function			void NET_DNS_NBIOT_Event_MinimumFunctionality(DNS_ClientsTypeDef* pClient)
 @Description			NET_DNS_NBIOT_Event_MinimumFunctionality	: 最小功能
 @Input				pClient								: DNS客户端实例
 @Return				void
**********************************************************************************************************/
void NET_DNS_NBIOT_Event_MinimumFunctionality(DNS_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBStatus;
	
	DNS_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if ((NBStatus = NBIOT_Neul_NBxx_CheckReadMinOrFullFunc(pClient->SocketStack->NBIotStack)) == NBIOT_OK) {
		/* Dictate execute is Success */
		DNS_NBIOT_DictateEvent_SuccessExecute(pClient, NBAND_MODE_CONFIG, MINIMUM_FUNCTIONALITY);
		
#ifdef DNS_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("NB MinFunc Check Ok");
#endif
	}
	else {
		/* Dictate execute is Fail */
		DNS_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, MINIMUM_FUNCTIONALITY);
		
#ifdef DNS_DEBUG_LOG_RF_PRINT
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
			DNS_NBIOT_DictateEvent_SuccessExecute(pClient, NBAND_MODE_CONFIG, MINIMUM_FUNCTIONALITY);
			
#ifdef DNS_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("NB MinFunc Set Ok");
#endif
		}
		else {
			/* Dictate execute is Fail */
			DNS_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, MINIMUM_FUNCTIONALITY);
			
#ifdef DNS_DEBUG_LOG_RF_PRINT
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
 @Function			void NET_DNS_NBIOT_Event_NbandModeCheck(DNS_ClientsTypeDef* pClient)
 @Description			NET_DNS_NBIOT_Event_NbandModeCheck			: NBAND模式查询
 @Input				pClient								: DNS客户端实例
 @Return				void
**********************************************************************************************************/
void NET_DNS_NBIOT_Event_NbandModeCheck(DNS_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBStatus;
	
	DNS_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if ((NBStatus = NBIOT_Neul_NBxx_CheckReadSupportedBands(pClient->SocketStack->NBIotStack)) == NBIOT_OK) {
		/* Dictate execute is Success */
		DNS_NBIOT_DictateEvent_SuccessExecute(pClient, NBAND_MODE_CHECK, NBAND_MODE_CHECK);
		
#ifdef DNS_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("NB BAND Read %d Ok", pClient->SocketStack->NBIotStack->Parameter.band);
#endif
	}
	else {
		/* Dictate execute is Fail */
		DNS_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, NBAND_MODE_CHECK);
		
#ifdef DNS_DEBUG_LOG_RF_PRINT
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		Radio_Trf_Debug_Printf_Level2("NB BAND Read Fail ECde %d", NBStatus);
	#else
		Radio_Trf_Debug_Printf_Level2("NB BAND Read Fail");
	#endif
#endif
	}
	
	if (pClient->SocketStack->NBIotStack->Parameter.band != DNS_NBIOT_BAND_TYPE) {
		/* BAND Mode Mast be Config */
		pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = MINIMUM_FUNCTIONALITY;
	}
	else {
		/* BAND Mode Needn't be Config */
		pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = ATTACH_CHECK;
	}
}

/**********************************************************************************************************
 @Function			void NET_DNS_NBIOT_Event_NbandModeConfig(DNS_ClientsTypeDef* pClient)
 @Description			NET_DNS_NBIOT_Event_NbandModeConfig		: NBAND模式配置
 @Input				pClient								: DNS客户端实例
 @Return				void
**********************************************************************************************************/
void NET_DNS_NBIOT_Event_NbandModeConfig(DNS_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBStatus;
	
	DNS_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if ((NBStatus = NBIOT_Neul_NBxx_CheckReadSupportedBands(pClient->SocketStack->NBIotStack)) == NBIOT_OK) {
		/* Dictate execute is Success */
		DNS_NBIOT_DictateEvent_SuccessExecute(pClient, FULL_FUNCTIONALITY, NBAND_MODE_CONFIG);
		
#ifdef DNS_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("NB BAND Read %d Ok", pClient->SocketStack->NBIotStack->Parameter.band);
#endif
	}
	else {
		/* Dictate execute is Fail */
		DNS_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, NBAND_MODE_CONFIG);
		
#ifdef DNS_DEBUG_LOG_RF_PRINT
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		Radio_Trf_Debug_Printf_Level2("NB BAND Read Fail ECde %d", NBStatus);
	#else
		Radio_Trf_Debug_Printf_Level2("NB BAND Read Fail");
	#endif
#endif
	}
	
	if (pClient->SocketStack->NBIotStack->Parameter.band != DNS_NBIOT_BAND_TYPE) {
		/* BAND Mode Mast be Config */
		if ((NBStatus = NBIOT_Neul_NBxx_SetSupportedBands(pClient->SocketStack->NBIotStack, DNS_NBIOT_BAND_TYPE)) == NBIOT_OK) {
			/* Dictate execute is Success */
			DNS_NBIOT_DictateEvent_SuccessExecute(pClient, FULL_FUNCTIONALITY, NBAND_MODE_CONFIG);
			
#ifdef DNS_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("NB BAND Set %d Ok", DNS_NBIOT_BAND_TYPE);
#endif
		}
		else {
			/* Dictate execute is Fail */
			DNS_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, NBAND_MODE_CONFIG);
			
#ifdef DNS_DEBUG_LOG_RF_PRINT
		#if NBIOT_PRINT_ERROR_CODE_TYPE
			Radio_Trf_Debug_Printf_Level2("NB BAND Set %d Fail ECde %d", DNS_NBIOT_BAND_TYPE, NBStatus);
		#else
			Radio_Trf_Debug_Printf_Level2("NB BAND Set %d Fail", DNS_NBIOT_BAND_TYPE);
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
 @Function			void NET_DNS_NBIOT_Event_AttachCheck(DNS_ClientsTypeDef* pClient)
 @Description			NET_DNS_NBIOT_Event_AttachCheck		: 注网检查
 @Input				pClient							: DNS客户端实例
 @Return				void
**********************************************************************************************************/
void NET_DNS_NBIOT_Event_AttachCheck(DNS_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBStatus;
	
	DNS_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if ((NBStatus = NBIOT_Neul_NBxx_CheckReadAttachOrDetach(pClient->SocketStack->NBIotStack)) == NBIOT_OK) {
		/* Dictate execute is Success */
		DNS_NBIOT_DictateEvent_SuccessExecute(pClient, ATTACH_CHECK, ATTACH_CHECK);
		
#ifdef DNS_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("NB CGATT %d Ok", pClient->SocketStack->NBIotStack->Parameter.netstate);
#endif
	}
	else {
		/* Dictate execute is Fail */
		DNS_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, ATTACH_CHECK);
		
#ifdef DNS_DEBUG_LOG_RF_PRINT
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
 @Function			void NET_DNS_NBIOT_Event_AttachExecute(DNS_ClientsTypeDef* pClient)
 @Description			NET_DNS_NBIOT_Event_AttachExecute		: 注网进行
 @Input				pClient							: DNS客户端实例
 @Return				void
**********************************************************************************************************/
void NET_DNS_NBIOT_Event_AttachExecute(DNS_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBStatus;
	
	DNS_NBIOT_DictateEvent_SetTime(pClient, 30);
	
	if ((NBStatus = NBIOT_Neul_NBxx_SetAttachOrDetach(pClient->SocketStack->NBIotStack, Attach)) == NBIOT_OK) {
		/* Dictate execute is Success */
		DNS_NBIOT_DictateEvent_SuccessExecute(pClient, ATTACH_INQUIRE, ATTACH_EXECUTE);
		
#ifdef DNS_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("NB Set CGATT %d Ok", Attach);
#endif
	}
	else {
		/* Dictate execute is Fail */
		DNS_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, ATTACH_EXECUTE);
		
#ifdef DNS_DEBUG_LOG_RF_PRINT
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		Radio_Trf_Debug_Printf_Level2("NB Set CGATT %d Fail ECde %d", Attach, NBStatus);
	#else
		Radio_Trf_Debug_Printf_Level2("NB Set CGATT %d Fail", Attach);
	#endif
#endif
	}
}

/**********************************************************************************************************
 @Function			void NET_DNS_NBIOT_Event_AttachInquire(DNS_ClientsTypeDef* pClient)
 @Description			NET_DNS_NBIOT_Event_AttachInquire		: 注网查询
 @Input				pClient							: DNS客户端实例
 @Return				void
**********************************************************************************************************/
void NET_DNS_NBIOT_Event_AttachInquire(DNS_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBStatus;
	
	DNS_NBIOT_DictateEvent_SetTime(pClient, 60);
	
	if ((NBStatus = NBIOT_Neul_NBxx_CheckReadAttachOrDetach(pClient->SocketStack->NBIotStack)) == NBIOT_OK) {
		/* Dictate execute is Success */
		pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = ATTACH_INQUIRE;
#ifdef DNS_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("NB CGATT %d Ok", pClient->SocketStack->NBIotStack->Parameter.netstate);
#endif
	}
	else {
		/* Dictate execute is Fail */
		DNS_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, ATTACH_INQUIRE);
		
#ifdef DNS_DEBUG_LOG_RF_PRINT
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		Radio_Trf_Debug_Printf_Level2("NB CGATT %d Fail ECde %d", pClient->SocketStack->NBIotStack->Parameter.netstate, NBStatus);
	#else
		Radio_Trf_Debug_Printf_Level2("NB CGATT %d Fail", pClient->SocketStack->NBIotStack->Parameter.netstate);
	#endif
#endif
		return;
	}
	
	if (pClient->SocketStack->NBIotStack->Parameter.netstate != Attach) {
		DNS_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, ATTACH_INQUIRE);
	}
	else {
		DNS_NBIOT_DictateEvent_SuccessExecute(pClient, PARAMETER_CHECKOUT, ATTACH_INQUIRE);
	}
}

/**********************************************************************************************************
 @Function			void NET_DNS_NBIOT_Event_PatameterCheckOut(DNS_ClientsTypeDef* pClient)
 @Description			NET_DNS_NBIOT_Event_PatameterCheckOut	: 参数检出
 @Input				pClient							: DNS客户端实例
 @Return				void
**********************************************************************************************************/
void NET_DNS_NBIOT_Event_ParameterCheckOut(DNS_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBStatus;
	
	DNS_NBIOT_DictateEvent_SetTime(pClient, 30);
	
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
		DNS_NBIOT_DictateEvent_SuccessExecute(pClient, DNS_PROCESS_STACK, PARAMETER_CHECKOUT);
		
		pClient->SocketStack->NBIotStack->Registered = true;
		
#ifdef DNS_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("NB Parameter Check Ok");
#endif
	}
	else {
		/* Dictate execute is Fail */
		DNS_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, PARAMETER_CHECKOUT);
		
#ifdef DNS_DEBUG_LOG_RF_PRINT
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
 @Function			void NET_DNS_Event_CreatUDPSocket(DNS_ClientsTypeDef* pClient)
 @Description			NET_DNS_Event_CreatUDPSocket			: CreatUDPSocket
 @Input				pClient							: DNS客户端实例
 @Return				void
**********************************************************************************************************/
void NET_DNS_Event_CreatUDPSocket(DNS_ClientsTypeDef* pClient)
{
	DNS_DictateEvent_SetTime(pClient, 30);
	
	/* Creat UDP Socket */
	if (pClient->SocketStack->Open(pClient->SocketStack) == DNS_OK) {
		/* Dictate execute is Success */
		pClient->DictateRunCtl.dictateEnable = false;
		pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = DNS_PROCESS_STACK;
		pClient->ProcessState = DNS_PROCESS_SEND_DNS_STRUCT_DATA;
		pClient->DictateRunCtl.dictateCreatUDPSocketFailureCnt = 0;
#ifdef DNS_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("DNS Creat UDP Ok");
#endif
	}
	else {
		/* Dictate execute is Fail */
		if (Stm32_Calculagraph_IsExpiredSec(&pClient->DictateRunCtl.dictateRunTime) == true) {
			/* Dictate TimeOut */
			pClient->DictateRunCtl.dictateEnable = false;
			pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
			pClient->ProcessState = DNS_PROCESS_CREAT_UDP_SOCKET;
			pClient->DictateRunCtl.dictateCreatUDPSocketFailureCnt++;
			if (pClient->DictateRunCtl.dictateCreatUDPSocketFailureCnt > 3) {
				pClient->DictateRunCtl.dictateCreatUDPSocketFailureCnt = 0;
				pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = STOP_MODE;
				pClient->ProcessState = DNS_PROCESS_CREAT_UDP_SOCKET;
			}
		}
		else {
			/* Dictate isn't TimeOut */
			pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = DNS_PROCESS_STACK;
			pClient->ProcessState = DNS_PROCESS_CREAT_UDP_SOCKET;
		}
#ifdef DNS_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("DNS Creat UDP Fail");
#endif
		return;
	}
}

/**********************************************************************************************************
 @Function			void NET_DNS_Event_SendDnsStructData(DNS_ClientsTypeDef* pClient)
 @Description			NET_DNS_Event_SendDnsStructData		: SendDnsStructData
 @Input				pClient							: DNS客户端实例
 @Return				void
**********************************************************************************************************/
void NET_DNS_Event_SendDnsStructData(DNS_ClientsTypeDef* pClient)
{
	unsigned char Sendhostname[DNS_HOSTNAME_SIZE];
	
	if (pClient->AnalysisTick < DNS_ANALYSIS_DATA) {
		/* Have Domain name need to resolution */
		
		DNS_DictateEvent_SetTime(pClient, 30);
		
		/* Serialize dnsDataStructure Command Buffer */
		memset((void*)Sendhostname, 0, sizeof(Sendhostname));
		memcpy(Sendhostname, pClient->AnalysisData[pClient->AnalysisTick].hostname, sizeof(pClient->AnalysisData[pClient->AnalysisTick].hostname));
		DNSSerialize_dnsDataStructure(pClient, Sendhostname);
		
		/* Send dnsDataStructure Command Buffer to DNS Server */
		if (pClient->SocketStack->Write(pClient->SocketStack, (char *)pClient->Sendbuf, pClient->Sendlen) != DNS_OK) {
			/* Dictate execute is Fail */
			if (Stm32_Calculagraph_IsExpiredSec(&pClient->DictateRunCtl.dictateRunTime) == true) {
				/* Dictate TimeOut */
				pClient->DictateRunCtl.dictateEnable = false;
				pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
				pClient->ProcessState = DNS_PROCESS_CREAT_UDP_SOCKET;
				pClient->DictateRunCtl.dictateSendDnsStructDataFailureCnt++;
				if (pClient->DictateRunCtl.dictateSendDnsStructDataFailureCnt > 3) {
					pClient->DictateRunCtl.dictateSendDnsStructDataFailureCnt = 0;
					pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = STOP_MODE;
					pClient->ProcessState = DNS_PROCESS_CREAT_UDP_SOCKET;
				}
			}
			else {
				/* Dictate isn't TimeOut */
				pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = DNS_PROCESS_STACK;
				pClient->ProcessState = DNS_PROCESS_SEND_DNS_STRUCT_DATA;
			}
#ifdef DNS_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("DNS Send %s Fail", pClient->AnalysisData[pClient->AnalysisTick].hostnameAddr);
#endif
			return;
		}
		else {
			/* Dictate execute is Success */
			pClient->DictateRunCtl.dictateEnable = false;
			pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = DNS_PROCESS_STACK;
			pClient->ProcessState = DNS_PROCESS_RECV_DNS_STRUCT_DATA;
			pClient->DictateRunCtl.dictateSendDnsStructDataFailureCnt = 0;
#ifdef DNS_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("DNS Send %s Ok", pClient->AnalysisData[pClient->AnalysisTick].hostnameAddr);
#endif
		}
	}
	else {
		/* None Domain name need to resolution */
		pClient->DictateRunCtl.dictateEnable = false;
		pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = DNS_PROCESS_STACK;
		pClient->ProcessState = DNS_PROCESS_CLOSE_UDP_SOCKET;
		pClient->DictateRunCtl.dictateSendDnsStructDataFailureCnt = 0;
	}
}

/**********************************************************************************************************
 @Function			void NET_DNS_Event_RecvDnsStructData(DNS_ClientsTypeDef* pClient)
 @Description			NET_DNS_Event_RecvDnsStructData		: RecvDnsStructData
 @Input				pClient							: DNS客户端实例
 @Return				void
**********************************************************************************************************/
void NET_DNS_Event_RecvDnsStructData(DNS_ClientsTypeDef* pClient)
{
	int Rlength = 0;													//读取数据长度
	int Rleftlength = 0;												//剩余读取数据长度
	
	if (pClient->AnalysisTick < DNS_ANALYSIS_DATA) {
		/* Have Domain name need to resolution */
		
		DNS_DictateEvent_SetTime(pClient, 60);
		
		/* Recv dnsDataStructure Command Buffer to DNS Server */
		if (pClient->SocketStack->Read(pClient->SocketStack, (char *)pClient->Recvbuf, pClient->Recvbuf_size, &Rlength, &Rleftlength) != DNS_OK) {
			/* Dictate execute is Fail */
			if (Stm32_Calculagraph_IsExpiredSec(&pClient->DictateRunCtl.dictateRunTime) == true) {
				/* Dictate TimeOut */
				pClient->DictateRunCtl.dictateEnable = false;
				pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
				pClient->ProcessState = DNS_PROCESS_CREAT_UDP_SOCKET;
				pClient->DictateRunCtl.dictateRecvDnsStructDataFailureCnt++;
				if (pClient->DictateRunCtl.dictateRecvDnsStructDataFailureCnt > 3) {
					pClient->DictateRunCtl.dictateRecvDnsStructDataFailureCnt = 0;
					pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = STOP_MODE;
					pClient->ProcessState = DNS_PROCESS_CREAT_UDP_SOCKET;
				}
			}
			else {
				/* Dictate isn't TimeOut */
				pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = DNS_PROCESS_STACK;
				pClient->ProcessState = DNS_PROCESS_RECV_DNS_STRUCT_DATA;
			}
#ifdef DNS_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("DNS Wait Recv %s", pClient->AnalysisData[pClient->AnalysisTick].hostnameAddr);
#endif
			return;
		}
		else {
			/* Dictate execute is Success */
			if (Rlength == 0) {
				/* No Recv Data */
				if (Stm32_Calculagraph_IsExpiredSec(&pClient->DictateRunCtl.dictateRunTime) == true) {
					/* Dictate TimeOut */
					pClient->DictateRunCtl.dictateEnable = false;
					pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
					pClient->ProcessState = DNS_PROCESS_CREAT_UDP_SOCKET;
					pClient->DictateRunCtl.dictateRecvDnsStructDataFailureCnt++;
					if (pClient->DictateRunCtl.dictateRecvDnsStructDataFailureCnt > 3) {
						pClient->DictateRunCtl.dictateRecvDnsStructDataFailureCnt = 0;
						pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = STOP_MODE;
						pClient->ProcessState = DNS_PROCESS_CREAT_UDP_SOCKET;
					}
				}
				else {
					/* Dictate isn't TimeOut */
					pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = DNS_PROCESS_STACK;
					pClient->ProcessState = DNS_PROCESS_RECV_DNS_STRUCT_DATA;
				}
				return;
			}
			else {
				/* Is Recv Data */
				pClient->Recvlen = Rlength;
				
				/* Deserialize dnsDataStructure Command Buffer */
				if (DNSDeserialize_dnsDataStructure(pClient, pClient->AnalysisData[pClient->AnalysisTick].hostname) != DNS_OK) {
					/* Deserialize into IP Error */
					pClient->DictateRunCtl.dictateEnable = false;
					pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
					pClient->ProcessState = DNS_PROCESS_CREAT_UDP_SOCKET;
					pClient->DictateRunCtl.dictateRecvDnsStructDataFailureCnt++;
					if (pClient->DictateRunCtl.dictateRecvDnsStructDataFailureCnt > 3) {
						pClient->DictateRunCtl.dictateRecvDnsStructDataFailureCnt = 0;
						pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = STOP_MODE;
						pClient->ProcessState = DNS_PROCESS_CREAT_UDP_SOCKET;
					}
#ifdef DNS_DEBUG_LOG_RF_PRINT
					Radio_Trf_Debug_Printf_Level2("DNS Analysis %s Fail", pClient->AnalysisData[pClient->AnalysisTick].hostnameAddr);
#endif
					return;
				}
				else {
					/* Deserialize into IP OK */
					pClient->DictateRunCtl.dictateEnable = false;
					pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = DNS_PROCESS_STACK;
					pClient->ProcessState = DNS_PROCESS_SEND_DNS_STRUCT_DATA;
					pClient->DictateRunCtl.dictateRecvDnsStructDataFailureCnt = 0;
#ifdef DNS_DEBUG_LOG_RF_PRINT
					Radio_Trf_Debug_Printf_Level2("DNS Analysis %s OK", pClient->AnalysisData[pClient->AnalysisTick].hostnameAddr);
					Radio_Trf_Debug_Printf_Level2("%s : %s", pClient->AnalysisData[pClient->AnalysisTick].hostnameAddr, pClient->AnalysisData[pClient->AnalysisTick].hostIP);
#endif
					pClient->AnalysisTick += 1;
				}
			}
		}
	}
	else {
		/* None Domain name need to resolution */
		pClient->DictateRunCtl.dictateEnable = false;
		pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = DNS_PROCESS_STACK;
		pClient->ProcessState = DNS_PROCESS_CLOSE_UDP_SOCKET;
		pClient->DictateRunCtl.dictateRecvDnsStructDataFailureCnt = 0;
	}
}

/**********************************************************************************************************
 @Function			void NET_DNS_Event_CloseUDPSocket(DNS_ClientsTypeDef* pClient)
 @Description			NET_DNS_Event_CloseUDPSocket			: CloseUDPSocket
 @Input				pClient							: DNS客户端实例
 @Return				void
**********************************************************************************************************/
void NET_DNS_Event_CloseUDPSocket(DNS_ClientsTypeDef* pClient)
{
	DNS_DictateEvent_SetTime(pClient, 30);
	
	/* Close UDP Socket */
	if (pClient->SocketStack->Close(pClient->SocketStack) == DNS_OK) {
		/* Dictate execute is Success */
		pClient->DictateRunCtl.dictateEnable = false;
		pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = DNS_PROCESS_STACK;
		pClient->ProcessState = DNS_PROCESS_OVER_DNS_ANALYSIS;
		pClient->DictateRunCtl.dictateCloseUDPSocketFailureCnt = 0;
#ifdef DNS_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("DNS Close UDP Ok");
#endif
	}
	else {
		/* Dictate execute is Fail */
		if (Stm32_Calculagraph_IsExpiredSec(&pClient->DictateRunCtl.dictateRunTime) == true) {
			/* Dictate TimeOut */
			pClient->DictateRunCtl.dictateEnable = false;
			pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
			pClient->ProcessState = DNS_PROCESS_OVER_DNS_ANALYSIS;
		}
		else {
			/* Dictate isn't TimeOut */
			pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = DNS_PROCESS_STACK;
			pClient->ProcessState = DNS_PROCESS_CLOSE_UDP_SOCKET;
		}
#ifdef DNS_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("DNS Close UDP Fail");
#endif
		return;
	}
}

/**********************************************************************************************************
 @Function			void NET_DNS_Event_OverDnsAnalysis(DNS_ClientsTypeDef* pClient)
 @Description			NET_DNS_Event_OverDnsAnalysis			: OverDnsAnalysis
 @Input				pClient							: DNS客户端实例
 @Return				void
**********************************************************************************************************/
void NET_DNS_Event_OverDnsAnalysis(DNS_ClientsTypeDef* pClient)
{
	MQTTSN_Transport_Init(&MqttSNSocketNetHandler, &NbiotClientHandler, MQTTSN_SERVER_LOCAL_PORT, (char*)DNS_GetHostIP(pClient, (unsigned char*)MQTTSN_SERVER_HOST_NAME), MQTTSN_SERVER_TELE_PORT);
	
	pClient->NetNbiotStack->PollExecution = NET_POLL_EXECUTION_MQTTSN;
	pClient->SocketStack->NBIotStack->DictateRunCtl.dictateEvent = ATTACH_CHECK;
	pClient->ProcessState = DNS_PROCESS_CREAT_UDP_SOCKET;
}

/********************************************** END OF FLEE **********************************************/
