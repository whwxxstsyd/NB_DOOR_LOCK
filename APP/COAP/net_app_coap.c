#include "net_app_coap.h"
#include "net_platform_config.h"		
#include "nbiot_config.h"	



/**********************************************************************************************************
 @Function			void NET_COAP_APP_PollExecution(NBIOT_ClientsTypeDef* pClient)
 @Description			COAP_APP_PollExecution				: COAP逻辑处理
 @Input				pClient							: NBIOT客户端实例
 @Return				void
**********************************************************************************************************/
void NET_APP_COAP_PollExecution(NBIOT_ClientsTypeDef* pClient)
{	
	switch (pClient->DictateRunCtl.dictateEvent)
	{	
		case STOP_MODE:
			NET_NBIOT_COAP_Event_StopMode(pClient);
			break;			
			
		case HARDWARE_REBOOT:
			NET_NBIOT_COAP_Event_HardwareReboot(pClient);
			break;
		
		case REPORT_ERROE:
			NET_NBIOT_COAP_Event_ReportError(pClient);
			break;
		
		case MODULE_CHECK:
			NET_NBIOT_COAP_Event_ModuleCheck(pClient);
			break;
		
		case PARAMETER_CONFIG:
			NET_NBIOT_COAP_Event_ParameterConfig(pClient);
			break;
		
		case ICCID_CHECK:
			NET_NBIOT_COAP_Event_SimICCIDCheck(pClient);
			break;
		
		case MISC_EQUIP_CONFIG:
			NET_NBIOT_COAP_Event_MiscEquipConfig(pClient);
			break;
		
		case ATTACH_CHECK:
			NET_NBIOT_COAP_Event_AttachCheck(pClient);
			break;
		
		case ATTACH_EXECUTE:
			NET_NBIOT_COAP_Event_AttachExecute(pClient);
			break;
		
		case ATTACH_INQUIRE:
			NET_NBIOT_COAP_Event_AttachInquire(pClient);
			break;
		
		case PARAMETER_CHECKOUT:
			NET_NBIOT_COAP_Event_ParameterCheckOut(pClient);
			break;
		
		case MINIMUM_FUNCTIONALITY:
			//NET_NBIOT_COAP_Event_MinimumFunctionality(pClient);
			break;
		
		case FULL_FUNCTIONALITY:		
			//NET_NBIOT_COAP_Event_FullFunctionality(pClient);
			break;
		
		case CDP_SERVER_CHECK:
			NET_NBIOT_COAP_Event_CDPServerCheck(pClient);
			break;
		
		case CDP_SERVER_CONFIG:
			NET_NBIOT_COAP_Event_CDPServerConfig(pClient);
			break;	
		
		case NBAND_MODE_CHECK:
			NET_NBIOT_COAP_Event_NbandModeCheck(pClient);
			break;
		
		case NBAND_MODE_CONFIG:
			NET_NBIOT_COAP_Event_NbandModeConfig(pClient);
			break;
		
		case SEND_DATA:
			NET_NBIOT_COAP_Event_SendData(pClient);
			break;
		
		case RECV_DATA:
			NET_NBIOT_COAP_Event_RecvData(pClient);
			break;
		
		case SEND_DATA_RA_NORMAL:
			//NET_NBIOT_COAP_Event_SendDataRANormal(pClient);
			break;
		
		case RECV_DATA_RA_NORMAL:
			//NET_NBIOT_COAP_Event_RecvDataRANormal(pClient);
			break;
		
		case EXECUT_DOWNLINK_DATA:	
			//NET_NBIOT_COAP_Event_ExecutDownlinkData(pClient);
			break;
		
		case MQTTSN_PROCESS_STACK:
			pClient->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
			break;
		
		case DNS_PROCESS_STACK:
			pClient->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
			break;			
		
		case ONENET_PROCESS_STACK:
			pClient->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
			break;
			
		case LISTEN_RUN_CTL:
			//NET_NBIOT_COAP_Listen_PollExecution(pClient);
			break;
		
		default :
			pClient->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
			break;
	}
}




/**********************************************************************************************************
 @Function			static void NBIOT_COAP_DictateEvent_SetTime(NBIOT_ClientsTypeDef* pClient, u16 TimeoutSec)
 @Description			NBIOT_COAP_DictateEvent_SetTime		: 事件运行控制器注入时间(内部使用)
 @Input				pClient							: NBIOT客户端实例
					TimeoutSec						: 注入超时时间
 @Return				void
 @attention			事件运行之前判断是否需要注入时间
**********************************************************************************************************/
static void NBIOT_COAP_DictateEvent_SetTime(NBIOT_ClientsTypeDef* pClient, u32 TimeoutSec)
{	
	Stm8_CalculagraphTypeDef dictateRunTime;
	
	/* It is the first time to execute */
	if (pClient->DictateRunCtl.dictateEnable != TRUE) {
		pClient->DictateRunCtl.dictateEnable = TRUE;	
		pClient->DictateRunCtl.dictateTimeoutSec = TimeoutSec;
		Stm8_Calculagraph_CountdownSec(&dictateRunTime, pClient->DictateRunCtl.dictateTimeoutSec);
		pClient->DictateRunCtl.dictateRunTime = dictateRunTime;
	}
}




/**********************************************************************************************************
 @Function			static void NBIOT_COAP_GetDictateFailureCnt(NBIOT_ClientsTypeDef* pClient, 
 															NBIOT_DictateEventTypeDef dictateNoTimeOut)
 															
 @Description			NBIOT_COAP_DictateEvent_FailExecute	: 事件运行控制器出错执行(内部使用)
 @Input				pClient								: NBIOT客户端实例	
					dictateNoTimeOut						: 事假处理错误未超时
 @Return			void
**********************************************************************************************************/

static u8* NBIOT_COAP_GetDictateFailureCnt(NBIOT_ClientsTypeDef* pClient, NBIOT_DictateEventTypeDef dictateNoTimeOut)
{
	u8* dictateFailureCnt;
		
	switch (dictateNoTimeOut)
	{
	case HARDWARE_REBOOT:
		dictateFailureCnt = &pClient->DictateRunCtl.dictateRebootFailureCnt;
		break;
	
	case REPORT_ERROE:
		dictateFailureCnt = &pClient->DictateRunCtl.dictateReportErrorFailureCnt;
		break;
	
	case MODULE_CHECK:
		dictateFailureCnt = &pClient->DictateRunCtl.dictateModuleCheckFailureCnt;
		break;
	
	case PARAMETER_CONFIG:
		dictateFailureCnt = &pClient->DictateRunCtl.dictateParameterConfigFailureCnt;
		break;
	
	case ICCID_CHECK:
		dictateFailureCnt = &pClient->DictateRunCtl.dictateSimICCIDCheckFailureCnt;
		break;
	
	case FULL_FUNCTIONALITY:
		dictateFailureCnt = &pClient->DictateRunCtl.dictateFullFunctionalityFailureCnt;
		break;
	
	case MINIMUM_FUNCTIONALITY:
		dictateFailureCnt = &pClient->DictateRunCtl.dictateMinimumFunctionalityFailureCnt;
		break;
	
	case CDP_SERVER_CHECK:
		dictateFailureCnt = &pClient->DictateRunCtl.dictateCDPServerCheckFailureCnt;
		break;
	
	case CDP_SERVER_CONFIG:
		dictateFailureCnt = &pClient->DictateRunCtl.dictateCDPServerConfigFailureCnt;
		break;
	
	case NBAND_MODE_CHECK:
		dictateFailureCnt = &pClient->DictateRunCtl.dictateNbandModeCheckFailureCnt;
		break;
	
	case NBAND_MODE_CONFIG:
		dictateFailureCnt = &pClient->DictateRunCtl.dictateNbandModeConfigFailureCnt;
		break;
	
	case MISC_EQUIP_CONFIG:
		dictateFailureCnt = &pClient->DictateRunCtl.dictateMiscEquipConfigFailureCnt;
		break;
	
	case ATTACH_CHECK:
		dictateFailureCnt = &pClient->DictateRunCtl.dictateAttachCheckFailureCnt;
		break;
	
	case ATTACH_EXECUTE:
		dictateFailureCnt = &pClient->DictateRunCtl.dictateAttachExecuteFailureCnt;
		break;
	
	case ATTACH_INQUIRE:
		dictateFailureCnt = &pClient->DictateRunCtl.dictateAttachInquireFailureCnt;
		break;
	
	case PARAMETER_CHECKOUT:
		dictateFailureCnt = &pClient->DictateRunCtl.dictatePatameterCheckOutFailureCnt;
		break;
	
	case SEND_DATA:
		dictateFailureCnt = &pClient->DictateRunCtl.dictateSendDataFailureCnt;
		break;
	
	case RECV_DATA:
		dictateFailureCnt = &pClient->DictateRunCtl.dictateRecvDataFailureCnt;
		break;
	
	case SEND_DATA_RA_NORMAL:
		dictateFailureCnt = &pClient->DictateRunCtl.dictateSendDataRANormalFailureCnt;
		break;
	
	case RECV_DATA_RA_NORMAL:
		dictateFailureCnt = &pClient->DictateRunCtl.dictateRecvDataRANormalFailureCnt;
		break;
	
	default :
		dictateFailureCnt = &pClient->DictateRunCtl.dictateRebootFailureCnt;
		break;
	}
	
	return dictateFailureCnt;

}






/**********************************************************************************************************
 @Function			static void NBIOT_COAP_DictateEvent_FailExecute(NBIOT_ClientsTypeDef* pClient, NBIOT_DictateEventTypeDef dictateTimeOut, \
																				    NBIOT_DictateEventTypeDef dictateFail, \
																				    NBIOT_DictateEventTypeDef dictateNoTimeOut)
 @Description			NBIOT_COAP_DictateEvent_FailExecute	: 事件运行控制器出错执行(内部使用)
 @Input				pClient							: NBIOT客户端实例
					dictateTimeOut					: 事假处理错误超时
					dictateFail						: 事件处理错误次数溢出
					dictateNoTimeOut					: 事假处理错误未超时
 @Return				void
**********************************************************************************************************/
static void NBIOT_COAP_DictateEvent_FailExecute(NBIOT_ClientsTypeDef* pClient, NBIOT_DictateEventTypeDef dictateTimeOut,
															    NBIOT_DictateEventTypeDef dictateFail,	
															    NBIOT_DictateEventTypeDef dictateNoTimeOut)
{
	u8* dictateFailureCnt;
	
	dictateFailureCnt = NBIOT_COAP_GetDictateFailureCnt(pClient, dictateNoTimeOut);
			
	if (Stm8_Calculagraph_IsExpiredSec(&pClient->DictateRunCtl.dictateRunTime) == TRUE) {
		/* Dictate TimeOut */
		pClient->DictateRunCtl.dictateEnable = FALSE;
		pClient->DictateRunCtl.dictateEvent = dictateTimeOut;
		*dictateFailureCnt += 1;
		if (*dictateFailureCnt > 3) {
			*dictateFailureCnt = 0;
			pClient->DictateRunCtl.dictateEvent = dictateFail;
		}
	}
	else {
		/* Dictate isn't TimeOut */
		pClient->DictateRunCtl.dictateEvent = dictateNoTimeOut;
	}

	pClient->DictateRunCtl.dictateEvent = dictateNoTimeOut;

}

/**********************************************************************************************************
 @Function			static void NBIOT_COAP_DictateEvent_SuccessExecute(NBIOT_ClientsTypeDef* pClient, NBIOT_DictateEventTypeDef dictateSuccess, \
																	NBIOT_DictateEventTypeDef dictateNoTimeOut)	
 @Description			NBIOT_COAP_DictateEvent_SuccessExecute	: 事件运行控制器正确执行(内部使用)
 @Input				pClient									: NBIOT客户端实例
					dictateSuccess							: 事假处理正确跳转
					dictateNoTimeOut							: 事假处理错误未超时
 @Return			void
**********************************************************************************************************/
static void NBIOT_COAP_DictateEvent_SuccessExecute(NBIOT_ClientsTypeDef* pClient, 
																	NBIOT_DictateEventTypeDef dictateSuccess,
																  	NBIOT_DictateEventTypeDef dictateNoTimeOut)
{	
	u8* dictateFailureCnt;
		
	dictateFailureCnt = NBIOT_COAP_GetDictateFailureCnt(pClient, dictateNoTimeOut);
	
	pClient->DictateRunCtl.dictateEnable = FALSE;	
	pClient->DictateRunCtl.dictateEvent = dictateSuccess;
	
	*dictateFailureCnt = 0;		
}



	






/**********************************************************************************************************
 @Function			void NET_COAP_NBIOT_Event_StopMode(NBIOT_ClientsTypeDef* pClient)
 @Description			NET_COAP_NBIOT_Event_StopMode			: 停止模式
 @Input				pClient							: NBIOT客户端实例
 @Return				void
 @attention			当30分钟或有数据需要发送时退出停止模式尝试重启NB,开启NB运行
**********************************************************************************************************/
void NET_NBIOT_COAP_Event_StopMode(NBIOT_ClientsTypeDef* pClient)
{
	
}





/**********************************************************************************************************
 @Function			void NET_COAP_NBIOT_Event_HardwareReboot(NBIOT_ClientsTypeDef* pClient)
 @Description			NET_COAP_NBIOT_Event_HardwareReboot	: 硬件重启
 @Input				pClient							: NBIOT客户端实例
 @Return				void
**********************************************************************************************************/
void NET_NBIOT_COAP_Event_HardwareReboot(NBIOT_ClientsTypeDef* pClient)
{	
	NBIOT_COAP_DictateEvent_SetTime(pClient, 30);
	
	if (NBIOT_Neul_NBxx_HardwareReboot(pClient, 8000) == NBIOT_OK) {
		/* Dictate execute is Success */
		
		NBIOT_COAP_DictateEvent_SuccessExecute(pClient, MODULE_CHECK, HARDWARE_REBOOT);
		
		/* Get IdleTime */				
		//NBIOT_COAP_GetIdleTime(pClient, TRUE);
				
#ifdef COAP_DEBUG_LOG_PRINT			
		//GPRS_DEBUG(DEBUG_INFO, "NB HDRBT OK, Baud:%d", NBIOTBaudRate.Baud);
#endif
	}
	else {
		/* Dictate execute is Fail */
		NBIOT_COAP_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, HARDWARE_REBOOT);
		
#ifdef COAP_DEBUG_LOG_PRINT
		GPRS_DEBUG(DEBUG_INFO, "NB HDRBT Fail");
#endif
	}
}








/**********************************************************************************************************
 @Function			void NET_NBIOT_COAP_Event_ReportError(NBIOT_ClientsTypeDef* pClient)
 @Description			NET_NBIOT_COAP_Event_ReportError		: 错误码输出
 @Input				pClient							: NBIOT客户端实例
 @Return				void
**********************************************************************************************************/
void NET_NBIOT_COAP_Event_ReportError(NBIOT_ClientsTypeDef* pClient)
{	
	NBIOT_COAP_DictateEvent_SetTime(pClient, 30);

	if (NBIOT_Neul_NBxx_SetReportTerminationError(pClient, CMEEnable) == NBIOT_OK) 
	{	
		/* Dictate execute is Success */
		NBIOT_COAP_DictateEvent_SuccessExecute(pClient, MODULE_CHECK, REPORT_ERROE);
		
#ifdef COAP_DEBUG_LOG_PRINT
		GPRS_DEBUG(DEBUG_INFO, "NB ReportErrorCode Set %d OK", CMEEnable);
#endif
	}
	else 
	{	
		/* Dictate execute is Fail */		
		NBIOT_COAP_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, REPORT_ERROE);
				
#ifdef COAP_DEBUG_LOG_PRINT
		GPRS_DEBUG(DEBUG_INFO, "NB ReportErrorCode Set %d Fail", CMEEnable);
#endif
	}
}




/**********************************************************************************************************
 @Function			void NET_NBIOT_COAP_Event_NbandModeCheck(NBIOT_ClientsTypeDef* pClient)
 @Description			NET_NBIOT_COAP_Event_NbandModeCheck	: NBAND模式查询
 @Input				pClient							: NBIOT客户端实例
 @Return				void
**********************************************************************************************************/
void NET_NBIOT_COAP_Event_NbandModeCheck(NBIOT_ClientsTypeDef* pClient)
{	
	NBIOT_StatusTypeDef NBStatus = NBStatus;	

	NBIOT_COAP_DictateEvent_SetTime(pClient, 30);
	
	if ((NBStatus = NBIOT_Neul_NBxx_CheckReadSupportedBands(pClient)) == NBIOT_OK) 
	{
		/* Dictate execute is Success */
		NBIOT_COAP_DictateEvent_SuccessExecute(pClient, NBAND_MODE_CHECK, NBAND_MODE_CHECK);
		
#ifdef COAP_DEBUG_LOG_PRINT
		GPRS_DEBUG(DEBUG_INFO, "Coap BAND Read %d OK", pClient->Parameter.band);
#endif
	}
	else 
	{
		/* Dictate execute is Fail */
		NBIOT_COAP_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, NBAND_MODE_CHECK);
			
#ifdef COAP_DEBUG_LOG_PRINT	
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		GPRS_DEBUG(DEBUG_INFO, "Coap BAND Read Fail ECde %d", NBStatus);
	#else
		GPRS_DEBUG(DEBUG_INFO, "Coap BAND Read Fail");
	#endif
#endif
	}
	
	if (pClient->Parameter.band != NBIOT_COAP_BAND_TYPE) {
		/* BAND Mode Mast be Config */
		pClient->DictateRunCtl.dictateEvent = MINIMUM_FUNCTIONALITY;
	}
	else {
		/* BAND Mode Needn't be Config */
		pClient->DictateRunCtl.dictateEvent = MISC_EQUIP_CONFIG;
	}
}


/**********************************************************************************************************
 @Function			void NET_NBIOT_COAP_Event_NbandModeConfig(NBIOT_ClientsTypeDef* pClient)
 @Description			NET_NBIOT_COAP_Event_NbandModeConfig	: NBAND模式配置
 @Input				pClient							: NBIOT客户端实例
 @Return				void
**********************************************************************************************************/
void NET_NBIOT_COAP_Event_NbandModeConfig(NBIOT_ClientsTypeDef* pClient)
{	
	NBIOT_StatusTypeDef NBStatus = NBStatus;	

	NBIOT_COAP_DictateEvent_SetTime(pClient, 30);
	
	if ((NBStatus = NBIOT_Neul_NBxx_CheckReadSupportedBands(pClient)) == NBIOT_OK) 
	{
		/* Dictate execute is Success */
		NBIOT_COAP_DictateEvent_SuccessExecute(pClient, FULL_FUNCTIONALITY, NBAND_MODE_CONFIG);
		
#ifdef COAP_DEBUG_LOG_PRINT
		GPRS_DEBUG(DEBUG_INFO, "Coap BAND Read %d OK", pClient->Parameter.band);
#endif
	}
	else 
	{	
		/* Dictate execute is Fail */
		NBIOT_COAP_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, NBAND_MODE_CONFIG);
			
#ifdef COAP_DEBUG_LOG_PRINT
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		GPRS_DEBUG(DEBUG_INFO, "Coap BAND Read Fail ECde %d", NBStatus);
	#else	
		GPRS_DEBUG(DEBUG_INFO, "Coap BAND Read Fail");
	#endif
#endif
	}
	
	if (pClient->Parameter.band != NBIOT_COAP_BAND_TYPE)
	{
		/* BAND Mode Mast be Config */
		if ((NBStatus = NBIOT_Neul_NBxx_SetSupportedBands(pClient, NBIOT_COAP_BAND_TYPE)) == NBIOT_OK)
		{
			/* Dictate execute is Success */
			NBIOT_COAP_DictateEvent_SuccessExecute(pClient, FULL_FUNCTIONALITY, NBAND_MODE_CONFIG);
				
#ifdef COAP_DEBUG_LOG_PRINT
			GPRS_DEBUG(DEBUG_INFO, "Coap BAND Set %d OK", NBIOT_COAP_BAND_TYPE);
#endif
		}
		else 
		{
			/* Dictate execute is Fail */
			NBIOT_COAP_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, NBAND_MODE_CONFIG);
				
#ifdef COAP_DEBUG_LOG_PRINT
		#if NBIOT_PRINT_ERROR_CODE_TYPE
			GPRS_DEBUG(DEBUG_INFO, "Coap BAND Set %d Fail ECde %d", NBIOT_COAP_BAND_TYPE, NBStatus);
		#else
			GPRS_DEBUG(DEBUG_INFO, "Coap BAND Set %d Fail", NBIOT_COAP_BAND_TYPE);
		#endif
#endif
			return;
		}
	}
	else {
		/* BAND Mode Needn't be Config */
		pClient->DictateRunCtl.dictateEvent = FULL_FUNCTIONALITY;
	}
}






/**********************************************************************************************************
 @Function			void NET_NBIOT_COAP_Event_AttachExecute(NBIOT_ClientsTypeDef* pClient)
 @Description			NET_NBIOT_COAP_Event_AttachExecute		: 注网进行
 @Input				pClient							: NBIOT客户端实例
 @Return				void
**********************************************************************************************************/
void NET_NBIOT_COAP_Event_AttachExecute(NBIOT_ClientsTypeDef* pClient)
{	
	NBIOT_StatusTypeDef NBStatus = NBStatus;

	NBIOT_COAP_DictateEvent_SetTime(pClient, 30);
	
	if ((NBStatus = NBIOT_Neul_NBxx_SetAttachOrDetach(pClient, Attach)) == NBIOT_OK)
	{
		/* Dictate execute is Success */
		NBIOT_COAP_DictateEvent_SuccessExecute(pClient, ATTACH_INQUIRE, ATTACH_EXECUTE);
		
#ifdef COAP_DEBUG_LOG_PRINT
		GPRS_DEBUG(DEBUG_INFO, "Coap Set CGATT %d OK", Attach);
#endif
	}
	else
	{
		/* Dictate execute is Fail */
		NBIOT_COAP_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, ATTACH_EXECUTE);
		
#ifdef COAP_DEBUG_LOG_PRINT
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		GPRS_DEBUG(DEBUG_INFO, "Coap Set CGATT %d Fail ECde %d", Attach, NBStatus);
	#else
		GPRS_DEBUG(DEBUG_INFO, "Coap Set CGATT %d Fail", Attach);
	#endif
#endif
	}
}



/**********************************************************************************************************
 @Function			void NET_NBIOT_COAP_Event_AttachInquire(NBIOT_ClientsTypeDef* pClient)
 @Description			NET_NBIOT_COAP_Event_AttachInquire		: 注网查询
 @Input				pClient							: NBIOT客户端实例
 @Return				void
**********************************************************************************************************/
void NET_NBIOT_COAP_Event_AttachInquire(NBIOT_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBStatus;	

	NBIOT_COAP_DictateEvent_SetTime(pClient, 60);
	
	if ((NBStatus = NBIOT_Neul_NBxx_CheckReadAttachOrDetach(pClient)) == NBIOT_OK) 
	{
		/* Dictate execute is Success */
		pClient->DictateRunCtl.dictateEvent = ATTACH_INQUIRE;
#ifdef COAP_DEBUG_LOG_PRINT
		GPRS_DEBUG(DEBUG_INFO, "Coap CGATT %d OK", pClient->Parameter.netstate);
#endif
	}
	else 
	{
		/* Dictate execute is Fail */
		NBIOT_COAP_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, ATTACH_INQUIRE);
		
#ifdef COAP_DEBUG_LOG_PRINT
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		GPRS_DEBUG(DEBUG_INFO, "Coap CGATT %d Fail ECde %d", pClient->Parameter.netstate, NBStatus);
	#else
		GPRS_DEBUG(DEBUG_INFO, "Coap CGATT %d Fail", pClient->Parameter.netstate);
	#endif
#endif
		return;
	}
	
	if (pClient->Parameter.netstate != Attach) 
	{
		NBIOT_COAP_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, ATTACH_INQUIRE);
	}
	else 
	{
		/* 注网成功 */
		NBIOT_COAP_DictateEvent_SuccessExecute(pClient, PARAMETER_CHECKOUT, ATTACH_INQUIRE);
		
		/* Get ConnectTime */		
		//NBIOT_COAP_GetConnectTime(pClient, TRUE);
	}
}




/**********************************************************************************************************
 @Function			void NET_NBIOT_COAP_Event_PatameterCheckOut(NBIOT_ClientsTypeDef* pClient)
 @Description			NET_NBIOT_COAP_Event_PatameterCheckOut	: 参数检出
 @Input				pClient							: NBIOT客户端实例
 @Return				void
**********************************************************************************************************/
void NET_NBIOT_COAP_Event_ParameterCheckOut(NBIOT_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBStatus;

	NBIOT_COAP_DictateEvent_SetTime(pClient, 30);
	
	if (((NBStatus = NBIOT_Neul_NBxx_CheckReadIMEI(pClient)) == NBIOT_OK) && 
	    ((NBStatus = NBIOT_Neul_NBxx_CheckReadIMEISV(pClient)) == NBIOT_OK) && 
	    ((NBStatus = NBIOT_Neul_NBxx_CheckReadIMSI(pClient)) == NBIOT_OK) && 
	    ((NBStatus = NBIOT_Neul_NBxx_CheckReadCGPADDR(pClient)) == NBIOT_OK) && 
	    ((NBStatus = NBIOT_Neul_NBxx_CheckReadCGDCONT(pClient)) == NBIOT_OK) && 
	    ((NBStatus = NBIOT_Neul_NBxx_CheckReadRSSI(pClient)) == NBIOT_OK) && 
	    ((NBStatus = NBIOT_Neul_NBxx_CheckReadStatisticsRADIO(pClient)) == NBIOT_OK) && 
	    ((NBStatus = NBIOT_Neul_NBxx_CheckReadStatisticsCELL(pClient)) == NBIOT_OK) && 
	    ((NBStatus = NBIOT_Neul_NBxx_CheckReadDateTime(pClient)) == NBIOT_OK)) 
	{
		/* Dictate execute is Success */
		NBIOT_COAP_DictateEvent_SuccessExecute(pClient, NBCOAP_SENDMODE_TYPE, PARAMETER_CHECKOUT);
		
		pClient->Registered = TRUE;		
		
#ifdef COAP_DEBUG_LOG_PRINT
		GPRS_DEBUG(DEBUG_INFO, "Coap Parameter Check OK");
#endif
	}
	else 
	{
		/* Dictate execute is Fail */
		NBIOT_COAP_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, PARAMETER_CHECKOUT);
		
#ifdef COAP_DEBUG_LOG_PRINT
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		GPRS_DEBUG(DEBUG_INFO, "Coap Parameter Check Fail ECde %d", NBStatus);
	#else
		GPRS_DEBUG(DEBUG_INFO, "Coap Parameter Check Fail");
	#endif
#endif
		return;
	}

	
	/* Set System Time */
	//RTC_Set_Date(pClient->Parameter.dataTime.year, pClient->Parameter.dataTime.month, pClient->Parameter.dataTime.day);
	//RTC_Set_Time(pClient->Parameter.dataTime.hour, pClient->Parameter.dataTime.min,   pClient->Parameter.dataTime.sec);
}






/**********************************************************************************************************
 @Function			void NET_COAP_NBIOT_Event_MinimumFunctionality(NBIOT_ClientsTypeDef* pClient)
 @Description			NET_COAP_NBIOT_Event_MinimumFunctionality	: 最小功能
 @Input				pClient								: NBIOT客户端实例
 @Return				void
**********************************************************************************************************/
void NET_NBIOT_COAP_Event_MinimumFunctionality(NBIOT_ClientsTypeDef* pClient)
{	
	NBIOT_StatusTypeDef NBStatus = NBStatus;
	
	NBIOT_COAP_DictateEvent_SetTime(pClient, 30);
	
	if ((NBStatus = NBIOT_Neul_NBxx_CheckReadMinOrFullFunc(pClient)) == NBIOT_OK) {
		/* Dictate execute is Success */
		COAP_NBIOT_DictateEvent_SuccessExecute(pClient, CDP_SERVER_CONFIG, MINIMUM_FUNCTIONALITY);
		
#ifdef COAP_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("Coap MinFunc Check Ok");
#endif
	}
	else {
		/* Dictate execute is Fail */
		COAP_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, MINIMUM_FUNCTIONALITY);
		
#ifdef COAP_DEBUG_LOG_RF_PRINT
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		Radio_Trf_Debug_Printf_Level2("Coap MinFunc Check Fail ECde %d", NBStatus);
	#else
		Radio_Trf_Debug_Printf_Level2("Coap MinFunc Check Fail");
	#endif
#endif
		return;
	}
	
	if (pClient->Parameter.functionality != MinFunc) {
		if ((NBStatus = NBIOT_Neul_NBxx_SetMinOrFullFunc(pClient, MinFunc)) == NBIOT_OK) {
			/* Dictate execute is Success */
			COAP_NBIOT_DictateEvent_SuccessExecute(pClient, CDP_SERVER_CONFIG, MINIMUM_FUNCTIONALITY);
			
#ifdef COAP_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("Coap MinFunc Set Ok");
#endif
		}
		else {
			/* Dictate execute is Fail */
			COAP_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, MINIMUM_FUNCTIONALITY);
			
#ifdef COAP_DEBUG_LOG_RF_PRINT
		#if NBIOT_PRINT_ERROR_CODE_TYPE
			Radio_Trf_Debug_Printf_Level2("Coap MinFunc Set Fail ECde %d", NBStatus);
		#else
			Radio_Trf_Debug_Printf_Level2("Coap MinFunc Set Fail");
		#endif
#endif
			return;
		}
	}
}



/**********************************************************************************************************
 @Function			void NET_COAP_NBIOT_Event_FullFunctionality(NBIOT_ClientsTypeDef* pClient)
 @Description			NET_COAP_NBIOT_Event_FullFunctionality	: 完整功能
 @Input				pClient							: NBIOT客户端实例
 @Return				void
**********************************************************************************************************/
void NET_NBIOT_COAP_Event_FullFunctionality(NBIOT_ClientsTypeDef* pClient)
{		
	NBIOT_StatusTypeDef NBStatus = NBStatus;
	
	NBIOT_COAP_DictateEvent_SetTime(pClient, 30);
	
	if ((NBStatus = NBIOT_Neul_NBxx_CheckReadMinOrFullFunc(pClient)) == NBIOT_OK) {
		/* Dictate execute is Success */
		NBIOT_COAP_DictateEvent_SuccessExecute(pClient, CDP_SERVER_CHECK, FULL_FUNCTIONALITY);
		
#ifdef COAP_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("Coap FullFunc Check Ok");
#endif
	}
	else {
		/* Dictate execute is Fail */
		COAP_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, FULL_FUNCTIONALITY);
		
#ifdef COAP_DEBUG_LOG_RF_PRINT
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		Radio_Trf_Debug_Printf_Level2("Coap FullFunc Check Fail ECde %d", NBStatus);
	#else
		Radio_Trf_Debug_Printf_Level2("Coap FullFunc Check Fail");
	#endif
#endif
		return;
	}
	
	if (pClient->Parameter.functionality != FullFunc) {
		if ((NBStatus = NBIOT_Neul_NBxx_SetMinOrFullFunc(pClient, FullFunc)) == NBIOT_OK) {
			/* Dictate execute is Success */
			COAP_NBIOT_DictateEvent_SuccessExecute(pClient, CDP_SERVER_CHECK, FULL_FUNCTIONALITY);
			
#ifdef COAP_DEBUG_LOG_RF_PRINT
			Radio_Trf_Debug_Printf_Level2("Coap FullFunc Set Ok");
#endif
		}
		else {
			/* Dictate execute is Fail */
			COAP_NBIOT_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, FULL_FUNCTIONALITY);
			
#ifdef COAP_DEBUG_LOG_RF_PRINT
		#if NBIOT_PRINT_ERROR_CODE_TYPE
			Radio_Trf_Debug_Printf_Level2("Coap FullFunc Set Fail ECde %d", NBStatus);
		#else
			Radio_Trf_Debug_Printf_Level2("Coap FullFunc Set Fail");
		#endif
#endif
			return;
		}
	}
}






/**********************************************************************************************************
 @Function			void NET_NBIOT_COAP_Event_CDPServerCheck(NBIOT_ClientsTypeDef* pClient)
 @Description			NET_NBIOT_COAP_Event_CDPServerCheck	: CDP服务器查询
 @Input				pClient							: NBIOT客户端实例
 @Return				void
**********************************************************************************************************/

void NET_NBIOT_COAP_Event_CDPServerCheck(NBIOT_ClientsTypeDef* pClient)
{	
	NBIOT_StatusTypeDef NBStatus = NBStatus;

	NBIOT_COAP_DictateEvent_SetTime(pClient, 30);
			
	if ((NBStatus = NBIOT_Neul_NBxx_CheckReadCDPServer(pClient)) == NBIOT_OK) 
	{
		/* Dictate execute is Success */
		NBIOT_COAP_DictateEvent_SuccessExecute(pClient, CDP_SERVER_CHECK, CDP_SERVER_CHECK);
		
#ifdef COAP_DEBUG_LOG_PRINT
		GPRS_DEBUG(DEBUG_INFO,"Coap CDP Read %s:%d OK", pClient->Parameter.cdpserver.CDPServerHost, pClient->Parameter.cdpserver.CDPServerPort);
#endif
	}
	else 
	{
		/* Dictate execute is Fail */
		NBIOT_COAP_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, CDP_SERVER_CHECK);
		
#ifdef COAP_DEBUG_LOG_PRINT
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		GPRS_DEBUG(DEBUG_INFO,"Coap CDP Read Fail ECde %d", NBStatus);
	#else
		GPRS_DEBUG(DEBUG_INFO,"Coap CDP Read Fail");
	#endif
#endif
	}
	
	//if ((strcmp((const char*)pClient->Parameter.cdpserver.CDPServerHost, TCFG_EEPROM_Get_ServerIP_String()) != 0) || (pClient->Parameter.cdpserver.CDPServerPort != TCFG_EEPROM_GetServerPort())) 
	{
		/* CDP Server Mast be Config */
	//	pClient->DictateRunCtl.dictateEvent = MINIMUM_FUNCTIONALITY;
	}
	//else
	{		
		/* CDP Server Needn't be Config */
		pClient->DictateRunCtl.dictateEvent = NBAND_MODE_CHECK;
	}
}



/**********************************************************************************************************
 @Function			void NET_COAP_NBIOT_Event_SimICCIDCheck(NBIOT_ClientsTypeDef* pClient)
 @Description			NET_COAP_NBIOT_Event_SimICCIDCheck		: Sim卡检测
 @Input				pClient							: NBIOT客户端实例
 @Return				void
**********************************************************************************************************/
void NET_NBIOT_COAP_Event_SimICCIDCheck(NBIOT_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBStatus;

	NBIOT_COAP_DictateEvent_SetTime(pClient, 30);
	
	if ((NBStatus = NBIOT_Neul_NBxx_CheckReadICCID(pClient)) == NBIOT_OK) 
	{
		/* Dictate execute is Success */
		NBIOT_COAP_DictateEvent_SuccessExecute(pClient, FULL_FUNCTIONALITY, ICCID_CHECK);
				
#ifdef COAP_DEBUG_LOG_PRINT
		GPRS_DEBUG(DEBUG_INFO,"NB ICCID Check OK");
#endif
	}
	else 
	{
		/* Dictate execute is Fail */	
		NBIOT_COAP_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, ICCID_CHECK);
		
#ifdef COAP_DEBUG_LOG_PRINT
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		GPRS_DEBUG(DEBUG_INFO,"NB ICCID Check Fail ECde %d", NBStatus);
	#else
		GPRS_DEBUG(DEBUG_INFO,"NB ICCID Check Fail");
	#endif
#endif
	}	
}




/**********************************************************************************************************
 @Function			void NET_COAP_NBIOT_Event_CDPServerConfig(NBIOT_ClientsTypeDef* pClient)
 @Description			NET_COAP_NBIOT_Event_CDPServerConfig	: CDP服务器配置
 @Input				pClient							: NBIOT客户端实例
 @Return				void
**********************************************************************************************************/
void NET_NBIOT_COAP_Event_CDPServerConfig(NBIOT_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBStatus;

	NBIOT_COAP_DictateEvent_SetTime(pClient, 30);	
		
	if ((NBStatus = NBIOT_Neul_NBxx_CheckReadCDPServer(pClient)) == NBIOT_OK)
	{
		/* Dictate execute is Success */
		NBIOT_COAP_DictateEvent_SuccessExecute(pClient, NBAND_MODE_CONFIG, CDP_SERVER_CONFIG);
		
#ifdef COAP_DEBUG_LOG_PRINT
		GPRS_DEBUG(DEBUG_INFO,"Coap CDP Read %s:%d OK", pClient->Parameter.cdpserver.CDPServerHost, pClient->Parameter.cdpserver.CDPServerPort);
#endif
	}
	else 
	{
		/* Dictate execute is Fail */
		NBIOT_COAP_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, CDP_SERVER_CONFIG);
		
#ifdef COAP_DEBUG_LOG_PRINT
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		GPRS_DEBUG(DEBUG_INFO,"Coap CDP Read Fail ECde %d", NBStatus);
	#else
		GPRS_DEBUG(DEBUG_INFO,"Coap CDP Read Fail");
	#endif
#endif
	}


#if 0

	if ((strcmp((const char*)pClient->Parameter.cdpserver.CDPServerHost, TCFG_EEPROM_Get_ServerIP_String()) != 0) || (pClient->Parameter.cdpserver.CDPServerPort != TCFG_EEPROM_GetServerPort())) 
	{
		/* CDP Server Mast be Config */
		if ((NBStatus = NBIOT_Neul_NBxx_SetCDPServer(pClient, TCFG_EEPROM_Get_ServerIP_String(), TCFG_EEPROM_GetServerPort())) == NBIOT_OK) 
		{
			/* Dictate execute is Success */
			NBIOT_COAP_DictateEvent_SuccessExecute(pClient, NBAND_MODE_CONFIG, CDP_SERVER_CONFIG);
			
#ifdef COAP_DEBUG_LOG_PRINT
			GPRS_DEBUG(DEBUG_INFO,"Coap CDP Set %s:%d OK", TCFG_EEPROM_Get_ServerIP_String(), TCFG_EEPROM_GetServerPort());
#endif
		}
		else 
		{
			/* Dictate execute is Fail */	
			NBIOT_COAP_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, CDP_SERVER_CONFIG);
			
#ifdef COAP_DEBUG_LOG_PRINT
		#if NBIOT_PRINT_ERROR_CODE_TYPE
			GPRS_DEBUG(DEBUG_INFO,"Coap CDP Set %s:%d Fail ECde %d", TCFG_EEPROM_Get_ServerIP_String(), TCFG_EEPROM_GetServerPort(), NBStatus);
		#else
			GPRS_DEBUG(DEBUG_INFO,"Coap CDP Set %s:%d Fail", TCFG_EEPROM_Get_ServerIP_String(), TCFG_EEPROM_GetServerPort());
		#endif
#endif
			return;
		}
	}
	else 
	{
		/* CDP Server Needn't be Config */
		pClient->DictateRunCtl.dictateEvent = NBAND_MODE_CONFIG;
	}

#else

	pClient->DictateRunCtl.dictateEvent = NBAND_MODE_CONFIG;	
	
#endif

}



/**********************************************************************************************************
 @Function			void NET_COAP_NBIOT_Event_ModuleCheck(NBIOT_ClientsTypeDef* pClient)
 @Description			NET_COAP_NBIOT_Event_ModuleCheck		: 模块检测
 @Input				pClient							: NBIOT客户端实例
 @Return				void
**********************************************************************************************************/
void NET_NBIOT_COAP_Event_ModuleCheck(NBIOT_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBStatus;	

	NBIOT_COAP_DictateEvent_SetTime(pClient, 30);
		
	if (((NBStatus = NBIOT_Neul_NBxx_CheckReadManufacturer(pClient)) == NBIOT_OK) && 
	    ((NBStatus = NBIOT_Neul_NBxx_CheckReadManufacturerModel(pClient)) == NBIOT_OK) &&
	    ((NBStatus = NBIOT_Neul_NBxx_CheckReadModuleVersion(pClient)) == NBIOT_OK)) {
		/* Dictate execute is Success */
		NBIOT_COAP_DictateEvent_SuccessExecute(pClient, PARAMETER_CONFIG, MODULE_CHECK);
				
#ifdef COAP_DEBUG_LOG_PRINT
		GPRS_DEBUG(DEBUG_INFO,"NB Module Check OK");
#endif
	}
	else {
		/* Dictate execute is Fail */	
		NBIOT_COAP_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, MODULE_CHECK);
		
#ifdef COAP_DEBUG_LOG_PRINT
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		GPRS_DEBUG(DEBUG_INFO,"NB Mo	dule Check Fail ECde %d", NBStatus);
	#else
		GPRS_DEBUG(DEBUG_INFO,"NB Module Check Fail");
	#endif
#endif
	}
}




/**********************************************************************************************************
 @Function			void NET_COAP_NBIOT_Event_AttachCheck(NBIOT_ClientsTypeDef* pClient)
 @Description			NET_COAP_NBIOT_Event_AttachCheck		: 注网检查
 @Input				pClient							: NBIOT客户端实例
 @Return				void
**********************************************************************************************************/
void NET_NBIOT_COAP_Event_AttachCheck(NBIOT_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBStatus;
	
	NBIOT_COAP_DictateEvent_SetTime(pClient, 30);
	
	if ((NBStatus = NBIOT_Neul_NBxx_CheckReadAttachOrDetach(pClient)) == NBIOT_OK) {
		/* Dictate execute is Success */
		NBIOT_COAP_DictateEvent_SuccessExecute(pClient, ATTACH_CHECK, ATTACH_CHECK);
		
#ifdef COAP_DEBUG_LOG_PRINT
		GPRS_DEBUG(DEBUG_INFO,"Coap CGATT %d OK", pClient->Parameter.netstate);
#endif
	}
	else {
		/* Dictate execute is Fail */	
		NBIOT_COAP_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, ATTACH_CHECK);
		
#ifdef COAP_DEBUG_LOG_PRINT
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		GPRS_DEBUG(DEBUG_INFO,"Coap CGATT %d Fail ECde %d", pClient->Parameter.netstate, NBStatus);
	#else
		GPRS_DEBUG(DEBUG_INFO,"Coap CGATT %d Fail", pClient->Parameter.netstate);
	#endif
#endif
		return;
	}
	
	if (pClient->Parameter.netstate != Attach) {
		pClient->DictateRunCtl.dictateEvent = ATTACH_EXECUTE;
	}
	else {
		pClient->DictateRunCtl.dictateEvent = ATTACH_INQUIRE;
	}
}



/**********************************************************************************************************
 @Function			void NET_COAP_NBIOT_Event_MiscEquipConfig(NBIOT_ClientsTypeDef* pClient)
 @Description			NET_COAP_NBIOT_Event_MiscEquipConfig	: 其他配置
 @Input				pClient							: NBIOT客户端实例
 @Return				void
**********************************************************************************************************/
void NET_NBIOT_COAP_Event_MiscEquipConfig(NBIOT_ClientsTypeDef* pClient)
{	
	NBIOT_StatusTypeDef NBStatus = NBStatus;
	
	NBIOT_COAP_DictateEvent_SetTime(pClient, 30);
	
	if (((NBStatus = NBIOT_Neul_NBxx_CheckReadNewMessageIndications(pClient)) == NBIOT_OK) &&
	    ((NBStatus = NBIOT_Neul_NBxx_CheckReadSentMessageIndications(pClient)) == NBIOT_OK)) {
		/* Dictate execute is Success */	
		NBIOT_COAP_DictateEvent_SuccessExecute(pClient, ATTACH_CHECK, MISC_EQUIP_CONFIG);
		
#ifdef COAP_DEBUG_LOG_PRINT
		GPRS_DEBUG(DEBUG_INFO,"Coap MiscEquip Read OK");
#endif
	}
	else {
		/* Dictate execute is Fail */
		NBIOT_COAP_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, MISC_EQUIP_CONFIG);
		
#ifdef COAP_DEBUG_LOG_PRINT
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		GPRS_DEBUG(DEBUG_INFO,"Coap MiscEquip Read Fail ECde %d", NBStatus);
	#else
		GPRS_DEBUG(DEBUG_INFO,"Coap MiscEquip Read Fail");
	#endif
#endif
		return;
	}
	
	if (pClient->Parameter.nnmistate != CloseFunc) {
		if ((NBStatus = NBIOT_Neul_NBxx_SetNewMessageIndications(pClient, CloseFunc)) == NBIOT_OK) {
			/* Dictate execute is Success */
			NBIOT_COAP_DictateEvent_SuccessExecute(pClient, ATTACH_CHECK, MISC_EQUIP_CONFIG);
			
#ifdef COAP_DEBUG_LOG_PRINT
			GPRS_DEBUG(DEBUG_INFO,"Coap NNMI Set %d OK", CloseFunc);
#endif
		}
		else {
			/* Dictate execute is Fail */
			NBIOT_COAP_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, MISC_EQUIP_CONFIG);
			
#ifdef COAP_DEBUG_LOG_PRINT
		#if NBIOT_PRINT_ERROR_CODE_TYPE
			GPRS_DEBUG(DEBUG_INFO,"Coap NNMI Set %d Fail ECde %d", CloseFunc, NBStatus);
		#else
			GPRS_DEBUG(DEBUG_INFO,"Coap NNMI Set %d Fail", CloseFunc);
		#endif
#endif
			return;
		}
	}
	
	if (pClient->Parameter.nsmistate != CloseFunc) {
		if ((NBStatus = NBIOT_Neul_NBxx_SetSentMessageIndications(pClient, CloseFunc)) == NBIOT_OK) {
			/* Dictate execute is Success */
			NBIOT_COAP_DictateEvent_SuccessExecute(pClient, ATTACH_CHECK, MISC_EQUIP_CONFIG);
			
#ifdef COAP_DEBUG_LOG_PRINT
			GPRS_DEBUG(DEBUG_INFO,"Coap NSMI Set %d OK", CloseFunc);
#endif
		}
		else {	
			/* Dictate execute is Fail */
			NBIOT_COAP_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, MISC_EQUIP_CONFIG);
			
#ifdef COAP_DEBUG_LOG_PRINT
		#if NBIOT_PRINT_ERROR_CODE_TYPE
			GPRS_DEBUG(DEBUG_INFO,"Coap NSMI Set %d Fail ECde %d", CloseFunc, NBStatus);
		#else
			GPRS_DEBUG(DEBUG_INFO,"Coap NSMI Set %d Fail", CloseFunc);
		#endif
#endif
			return;
		}
	}
}




/**********************************************************************************************************
 @Function			void NET_COAP_NBIOT_Event_ParameterConfig(NBIOT_ClientsTypeDef* pClient)
 @Description			NET_COAP_NBIOT_Event_ParameterConfig	: 参数配置
 @Input				pClient							: NBIOT客户端实例
 @Return				void
**********************************************************************************************************/
void NET_NBIOT_COAP_Event_ParameterConfig(NBIOT_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBStatus;
	
	NBIOT_COAP_DictateEvent_SetTime(pClient, 30);
	
	if ((NBStatus = NBIOT_Neul_NBxx_CheckReadConfigUE(pClient)) == NBIOT_OK) {
		/* Dictate execute is Success */
		NBIOT_COAP_DictateEvent_SuccessExecute(pClient, ICCID_CHECK, PARAMETER_CONFIG);
		
#ifdef COAP_DEBUG_LOG_PRINT
		GPRS_DEBUG(DEBUG_INFO,"NB Parameter Config Read OK");
#endif
	}
	else {
		/* Dictate execute is Fail */
		NBIOT_COAP_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, PARAMETER_CONFIG);
		
#ifdef COAP_DEBUG_LOG_PRINT
	#if NBIOT_PRINT_ERROR_CODE_TYPE
		GPRS_DEBUG(DEBUG_INFO,"NB Parameter Config Read Fail ECde %d", NBStatus);
	#else
		GPRS_DEBUG(DEBUG_INFO,"NB Parameter Config Read Fail");
	#endif
#endif
		return;
	}
	
	if (pClient->Parameter.nconfig.autoConnect != AutoConnectVal) {
		if (NBIOT_Neul_NBxx_SetConfigUE(pClient, AutoConnect, AutoConnectVal) == NBIOT_OK) {
			/* Dictate execute is Success */
			NBIOT_COAP_DictateEvent_SuccessExecute(pClient, ICCID_CHECK, PARAMETER_CONFIG);
			
#ifdef COAP_DEBUG_LOG_PRINT
			GPRS_DEBUG(DEBUG_INFO,"NB %s %d OK", AutoConnect, AutoConnectVal);
#endif
		}
		else {
			/* Dictate execute is Fail */
			NBIOT_COAP_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, PARAMETER_CONFIG);
			
#ifdef COAP_DEBUG_LOG_PRINT
			GPRS_DEBUG(DEBUG_INFO,"NB %s %d Fail", AutoConnect, AutoConnectVal);
#endif
			return;
		}
	}
	
	if (pClient->Parameter.nconfig.crScrambling != CrScramblingVal) {
		if (NBIOT_Neul_NBxx_SetConfigUE(pClient, CrScrambling, CrScramblingVal) == NBIOT_OK) {
			/* Dictate execute is Success */
			NBIOT_COAP_DictateEvent_SuccessExecute(pClient, ICCID_CHECK, PARAMETER_CONFIG);
			
#ifdef COAP_DEBUG_LOG_PRINT
			GPRS_DEBUG(DEBUG_INFO,"NB %s %d OK", CrScrambling, CrScramblingVal);
#endif
		}
		else {
			/* Dictate execute is Fail */
			NBIOT_COAP_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, PARAMETER_CONFIG);
			
#ifdef COAP_DEBUG_LOG_PRINT
			GPRS_DEBUG(DEBUG_INFO,"NB %s %d Fail", CrScrambling, CrScramblingVal);
#endif
			return;
		}
	}
	
	if (pClient->Parameter.nconfig.crSiAvoid != CrSiAvoidVal) {
		if (NBIOT_Neul_NBxx_SetConfigUE(pClient, CrSiAvoid, CrSiAvoidVal) == NBIOT_OK) {
			/* Dictate execute is Success */
			NBIOT_COAP_DictateEvent_SuccessExecute(pClient, ICCID_CHECK, PARAMETER_CONFIG);
			
#ifdef COAP_DEBUG_LOG_PRINT
			GPRS_DEBUG(DEBUG_INFO,"NB %s %d OK", CrSiAvoid, CrSiAvoidVal);
#endif
		}
		else {
			/* Dictate execute is Fail */
			NBIOT_COAP_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, PARAMETER_CONFIG);
			
#ifdef COAP_DEBUG_LOG_PRINT
			GPRS_DEBUG(DEBUG_INFO,"NB %s %d Fail", CrSiAvoid, CrSiAvoidVal);
#endif
			return;
		}
	}
	
	if (pClient->Parameter.nconfig.combineAttach != CombineAttachVal) {
		if (NBIOT_Neul_NBxx_SetConfigUE(pClient, CombineAttach, CombineAttachVal) == NBIOT_OK) {
			/* Dictate execute is Success */
			NBIOT_COAP_DictateEvent_SuccessExecute(pClient, ICCID_CHECK, PARAMETER_CONFIG);
			
#ifdef COAP_DEBUG_LOG_PRINT
			GPRS_DEBUG(DEBUG_INFO,"NB %s %d OK", CombineAttach, CombineAttachVal);
#endif
		}
		else {
			/* Dictate execute is Fail */
			NBIOT_COAP_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, PARAMETER_CONFIG);
			
#ifdef COAP_DEBUG_LOG_PRINT
			GPRS_DEBUG(DEBUG_INFO,"NB %s %d Fail", CombineAttach, CombineAttachVal);
#endif
			return;
		}
	}
	
	if (pClient->Parameter.nconfig.cellReselection != CellReselectionVal) {
		if (NBIOT_Neul_NBxx_SetConfigUE(pClient, CellReselection, CellReselectionVal) == NBIOT_OK) {
			/* Dictate execute is Success */
			NBIOT_COAP_DictateEvent_SuccessExecute(pClient, ICCID_CHECK, PARAMETER_CONFIG);
			
#ifdef COAP_DEBUG_LOG_PRINT
			GPRS_DEBUG(DEBUG_INFO,"NB %s %d OK", CellReselection, CellReselectionVal);
#endif
		}
		else {
			/* Dictate execute is Fail */
			NBIOT_COAP_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, PARAMETER_CONFIG);
			
#ifdef COAP_DEBUG_LOG_PRINT
			GPRS_DEBUG(DEBUG_INFO,"NB %s %d Fail", CellReselection, CellReselectionVal);
#endif
			return;
		}
	}
	
	if (pClient->Parameter.nconfig.enableBip != EnableBipVal) {
		if (NBIOT_Neul_NBxx_SetConfigUE(pClient, EnableBip, EnableBipVal) == NBIOT_OK) {
			/* Dictate execute is Success */
			NBIOT_COAP_DictateEvent_SuccessExecute(pClient, ICCID_CHECK, PARAMETER_CONFIG);
			
#ifdef COAP_DEBUG_LOG_PRINT
			GPRS_DEBUG(DEBUG_INFO,"NB %s %d OK", EnableBip, EnableBipVal);
#endif
		}
		else {
			/* Dictate execute is Fail */	
			NBIOT_COAP_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, PARAMETER_CONFIG);
			
#ifdef COAP_DEBUG_LOG_PRINT
			GPRS_DEBUG(DEBUG_INFO,"NB %s %d Fail", EnableBip, EnableBipVal);
#endif
			return;
		}
	}
}




#if 0
/**********************************************************************************************************
 @Function			void NET_COAP_NBIOT_Event_SendData(NBIOT_ClientsTypeDef* pClient)
 @Description			NET_COAP_NBIOT_Event_SendData			: 发送数据
 @Input				pClient							: NBIOT客户端实例
 @Return				void
**********************************************************************************************************/
void NET_NBIOT_COAP_Event_SendData(NBIOT_ClientsTypeDef* pClient)
{
	NBIOT_StatusTypeDef NBStatus = NBStatus;
	
#if NBCOAP_SENDDATA_NQMGSCHECK_TYPE
	int SendSentNum = 0;
#endif
	
	NBIOT_COAP_DictateEvent_SetTime(pClient, 30);
	
	/* Data packets need to be sent*/
	if (NET_Coap_Message_SendDataDequeue(pClient->Sendbuf, (unsigned short *)&pClient->Sendlen) == TRUE) {
		/* Get IdleTime */
		COAP_NBIOT_GetIdleTime(pClient, TRUE);
		
		/* Connect Check */
		if ((NBStatus = NBIOT_Neul_NBxx_CheckReadAttachOrDetach(pClient)) == NBIOT_OK) {
			/* Dictate execute is Success */
			pClient->DictateRunCtl.dictateEvent = SEND_DATA;
#ifdef COAP_DEBUG_LOG_PRINT
			GPRS_DEBUG(DEBUG_INFO,"Coap CGATT %d OK", pClient->Parameter.netstate);
#endif
		}
		else {
			/* Dictate execute is Fail */
			NBIOT_COAP_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, SEND_DATA);
			
#ifdef COAP_DEBUG_LOG_PRINT
		#if NBIOT_PRINT_ERROR_CODE_TYPE
			GPRS_DEBUG(DEBUG_INFO,"Coap CGATT %d Fail ECde %d", pClient->Parameter.netstate, NBStatus);
		#else
			GPRS_DEBUG(DEBUG_INFO,"Coap CGATT %d Fail", pClient->Parameter.netstate);
		#endif
#endif
			return;
		}
		
		if (pClient->Parameter.netstate != Attach) {
			NBIOT_COAP_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, SEND_DATA);
			return;
		}
		
#if NBCOAP_SENDDATA_NQMGSCHECK_TYPE
		if (((NBStatus = NBIOT_Neul_NBxx_QuerySendMessageCOAPPayload(pClient)) == NBIOT_OK) && 
		    ((NBStatus = NBIOT_Neul_NBxx_QueryReadMessageCOAPPayload(pClient)) == NBIOT_OK)) {
			/* Dictate execute is Success */
			pClient->DictateRunCtl.dictateEvent = SEND_DATA;
#ifdef COAP_DEBUG_LOG_PRINT
			GPRS_DEBUG(DEBUG_INFO,"Coap NQMGS NQMGR OK");
#endif
		}
		else {
			/* Dictate execute is Fail */
			NBIOT_COAP_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, SEND_DATA);
			
#ifdef COAP_DEBUG_LOG_PRINT
		#if NBIOT_PRINT_ERROR_CODE_TYPE
			GPRS_DEBUG(DEBUG_INFO,"Coap NQMGS NQMGR Fail ECde %d", NBStatus);
		#else
			GPRS_DEBUG(DEBUG_INFO,"Coap NQMGS NQMGR Fail");
		#endif
#endif
			return;
		}
		
		SendSentNum = pClient->Parameter.coapSendMessage.sent;
#endif
		
		/* 发送负载数据 */
		if ((NBStatus = NBIOT_Neul_NBxx_SendCOAPPayload(pClient)) == NBIOT_OK) {
			/* Dictate execute is Success */
			pClient->DictateRunCtl.dictateEvent = SEND_DATA;
#ifdef COAP_DEBUG_LOG_PRINT
			GPRS_DEBUG(DEBUG_INFO,"Coap Send Payload OK");
#endif
		}
		else {
			/* Dictate execute is Fail */
			NBIOT_COAP_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, SEND_DATA);
			
#ifdef COAP_DEBUG_LOG_PRINT
		#if NBIOT_PRINT_ERROR_CODE_TYPE
			GPRS_DEBUG(DEBUG_INFO,"Coap Send Payload Fail ECde %d", NBStatus);
		#else
			GPRS_DEBUG(DEBUG_INFO,"Coap Send Payload Fail");
		#endif
#endif
			return;
		}
		
#if NBCOAP_SENDDATA_NQMGSCHECK_TYPE
		if (NBIOT_Neul_NBxx_QuerySendMessageCOAPPayload(pClient) == NBIOT_OK) {
			/* Dictate execute is Success */
			pClient->DictateRunCtl.dictateEvent = SEND_DATA;
		}
		else {
			/* Dictate execute is Fail */
			NBIOT_COAP_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, SEND_DATA);
			return;
		}
		
		if ((SendSentNum + 1) != pClient->Parameter.coapSendMessage.sent) {
			/* Send Data Fail */
			pClient->DictateRunCtl.dictateEnable = FALSE;
			pClient->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
			pClient->DictateRunCtl.dictateSendDataFailureCnt++;
			if (pClient->DictateRunCtl.dictateSendDataFailureCnt > 3) {
				pClient->DictateRunCtl.dictateSendDataFailureCnt = 0;
				pClient->DictateRunCtl.dictateEvent = STOP_MODE;
			}
		}
		else {
			/* Send Data Success */
			NBIOT_COAP_DictateEvent_SuccessExecute(pClient, RECV_DATA, SEND_DATA);
		}
#else
		/* Send Data Success */
		NBIOT_COAP_DictateEvent_SuccessExecute(pClient, RECV_DATA, SEND_DATA);
#endif
	}
	/* No packets need to be sent */
	else {
		NBIOT_COAP_DictateEvent_SuccessExecute(pClient, EXECUT_DOWNLINK_DATA, SEND_DATA);
	}
}




/**********************************************************************************************************
 @Function			void NET_COAP_NBIOT_Event_RecvData(NBIOT_ClientsTypeDef* pClient)
 @Description			NET_COAP_NBIOT_Event_RecvData			: 接收数据
 @Input				pClient							: NBIOT客户端实例
 @Return				void
**********************************************************************************************************/
void NET_NBIOT_COAP_Event_RecvData(NBIOT_ClientsTypeDef* pClient)
{
	u8 COAPFeedBackData[] = {0xAA, 0xBB};									//COAP反馈包数据
	bool COAPFeedBackFlag = FALSE;										//COAP反馈包接收标志位
	
	NBIOT_COAP_DictateEvent_SetTime(pClient, 60);
	
	if (NBIOT_Neul_NBxx_QueryReadMessageCOAPPayload(pClient) == NBIOT_OK) {
		/* Dictate execute is Success */
		pClient->DictateRunCtl.dictateEvent = RECV_DATA;
	}
	else {
		/* Dictate execute is Fail */
		NBIOT_COAP_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, RECV_DATA);
		return;
	}
	
	if (pClient->Parameter.coapReadMessage.buffered != 0) {
		/* Has Data Need Receive */
		for (int index = 0; index < pClient->Parameter.coapReadMessage.buffered; index++) {
			
			/* 读取负载数据 */
			if (NBIOT_Neul_NBxx_ReadCOAPPayload(pClient) == NBIOT_OK) {
				/* Dictate execute is Success */
				pClient->DictateRunCtl.dictateEvent = pClient->DictateRunCtl.dictateEvent;
			}
			else {	
				/* Dictate execute is Fail */
				if (Stm8_Calculagraph_IsExpiredSec(&pClient->DictateRunCtl.dictateRunTime) == TRUE) {
					/* Dictate TimeOut */
					pClient->DictateRunCtl.dictateEnable = FALSE;
					pClient->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
					pClient->DictateRunCtl.dictateRecvDataFailureCnt++;
					if (pClient->DictateRunCtl.dictateRecvDataFailureCnt > 3) {
						pClient->DictateRunCtl.dictateRecvDataFailureCnt = 0;
						pClient->DictateRunCtl.dictateEvent = STOP_MODE;
					}
				}
				else {
					/* Dictate isn't TimeOut */
					pClient->DictateRunCtl.dictateEvent = pClient->DictateRunCtl.dictateEvent;
				}
				return;
			}
			
			if (memcmp((const char*)pClient->Recvbuf, COAPFeedBackData, sizeof(COAPFeedBackData)) == 0) {
				/* Is Feedback */
				pClient->NetStateIdentification = TRUE;
				if (COAPFeedBackFlag == FALSE) {
					COAPFeedBackFlag = TRUE;
					NET_Coap_Message_SendDataOffSet();
				}
				NBIOT_COAP_DictateEvent_SuccessExecute(pClient, SEND_DATA, RECV_DATA);
				
			#if NBCOAP_LISTEN_PARAMETER_TYPE == NBCOAP_LISTEN_PARAMETER_ENABLE
				NET_COAP_NBIOT_Listen_Enable_EnterParameter(pClient);
			#endif
				
				/* NB 继续活跃注入时间 */
				TCFG_Utility_Set_Nbiot_IdleLifetime(NBIOT_CONTINUE_LIFETIME);
				
				/* Get ConnectTime */
				COAP_NBIOT_GetConnectTime(pClient, TRUE);
				
#ifdef COAP_DEBUG_LOG_PRINT
				GPRS_DEBUG(DEBUG_INFO,"Coap Recv Feedback OK");
#endif
			}
			else {
				/* Not Feedback */
				NET_Coap_Message_RecvDataEnqueue(pClient->Recvbuf, pClient->Recvlen);
#ifdef COAP_DEBUG_LOG_PRINT
				GPRS_DEBUG(DEBUG_INFO,"Coap Recv Data OK");
#endif
			}
		}
	}
	else {
		/* Not Data Need Receive */
		if (Stm8_Calculagraph_IsExpiredSec(&pClient->DictateRunCtl.dictateRunTime) == TRUE) {
			/* Dictate TimeOut */
			pClient->DictateRunCtl.dictateEnable = FALSE;
			pClient->DictateRunCtl.dictateEvent = HARDWARE_REBOOT;
			pClient->DictateRunCtl.dictateRecvDataFailureCnt++;
			if (pClient->DictateRunCtl.dictateRecvDataFailureCnt > 3) {
				pClient->DictateRunCtl.dictateRecvDataFailureCnt = 0;
				pClient->DictateRunCtl.dictateEvent = STOP_MODE;
			}
		}
		else {
			/* Dictate isn't TimeOut */
			pClient->DictateRunCtl.dictateEvent = RECV_DATA;
#ifdef COAP_DEBUG_LOG_PRINT
			GPRS_DEBUG(DEBUG_INFO,"Coap Wait Recv Feedback");
#endif
		}
	}
}

#else	
	
void NET_NBIOT_COAP_Event_SendData(NBIOT_ClientsTypeDef* pClient)
{

}


void NET_NBIOT_COAP_Event_RecvData(NBIOT_ClientsTypeDef* pClient)
{
		
}

#endif	


