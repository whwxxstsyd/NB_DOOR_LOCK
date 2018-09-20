#include "net_app_coap.h"
#include "include.h"
#include "net_platform_config.h"	



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
			//NET_NBIOT_COAP_Event_StopMode(pClient);
			break;	
		
		case HARDWARE_REBOOT:
			//NET_NBIOT_COAP_Event_HardwareReboot(pClient);
			break;
		
		case REPORT_ERROE:
			NET_NBIOT_COAP_Event_ReportError(pClient);
			break;
		
		case MODULE_CHECK:
			//NET_NBIOT_COAP_Event_ModuleCheck(pClient);
			break;
		
		case PARAMETER_CONFIG:
			//NET_NBIOT_COAP_Event_ParameterConfig(pClient);
			break;
		
		case ICCID_CHECK:
			//NET_NBIOT_COAP_Event_SimICCIDCheck(pClient);
			break;
		
		case MISC_EQUIP_CONFIG:
			//NET_NBIOT_COAP_Event_MiscEquipConfig(pClient);
			break;
		
		case ATTACH_CHECK:
			//NET_NBIOT_COAP_Event_AttachCheck(pClient);
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
			//NET_NBIOT_COAP_Event_CDPServerConfig(pClient);
			break;	
		
		case NBAND_MODE_CHECK:
			NET_NBIOT_COAP_Event_NbandModeCheck(pClient);
			break;
		
		case NBAND_MODE_CONFIG:
			NET_NBIOT_COAP_Event_NbandModeConfig(pClient);
			break;
		
		case SEND_DATA:
			//NET_NBIOT_COAP_Event_SendData(pClient);
			break;
		
		case RECV_DATA:
			//NET_NBIOT_COAP_Event_RecvData(pClient);
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
 @Function			static void NBIOT_COAP_GetDictateFailureCnt(NBIOT_ClientsTypeDef* pClient, 
 															NBIOT_DictateEventTypeDef dictateNoTimeOut)
 															
 @Description			NBIOT_COAP_DictateEvent_FailExecute	: 事件运行控制器出错执行(内部使用)
 @Input				pClient								: NBIOT客户端实例	
					dictateNoTimeOut						: 事假处理错误未超时
 @Return			void
**********************************************************************************************************/

static unsigned char* NBIOT_COAP_GetDictateFailureCnt(NBIOT_ClientsTypeDef* pClient, NBIOT_DictateEventTypeDef dictateNoTimeOut)
{
#if 0 
	
	unsigned char* dictateFailureCnt;
	
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

#endif
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
#if 0
	unsigned char* dictateFailureCnt;
	
	dictateFailureCnt = NBIOT_COAP_GetDictateFailureCnt(pClient, dictateNoTimeOut);
		
	if (Stm32_Calculagraph_IsExpiredSec(&pClient->DictateRunCtl.dictateRunTime) == true) {
		/* Dictate TimeOut */
		pClient->DictateRunCtl.dictateEnable = false;
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
#else

	pClient->DictateRunCtl.dictateEvent = dictateNoTimeOut;

#endif
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
	unsigned char* dictateFailureCnt;
	
	//dictateFailureCnt = NBIOT_COAP_GetDictateFailureCnt(pClient, dictateNoTimeOut);
	
	//pClient->DictateRunCtl.dictateEnable = false;
	pClient->DictateRunCtl.dictateEvent = dictateSuccess;
	//*dictateFailureCnt = 0;			
}










/**********************************************************************************************************
 @Function			void NET_NBIOT_COAP_Event_ReportError(NBIOT_ClientsTypeDef* pClient)
 @Description			NET_NBIOT_COAP_Event_ReportError		: 错误码输出
 @Input				pClient							: NBIOT客户端实例
 @Return				void
**********************************************************************************************************/
void NET_NBIOT_COAP_Event_ReportError(NBIOT_ClientsTypeDef* pClient)
{	
	if (NBIOT_Neul_NBxx_SetReportTerminationError(pClient, CMEEnable) == NBIOT_OK) 
	{	
		/* Dictate execute is Success */
		NBIOT_COAP_DictateEvent_SuccessExecute(pClient, MODULE_CHECK, REPORT_ERROE);
		
#ifdef COAP_DEBUG_LOG_PRINT
		GPRS_DEBUG(DEBUG_INFO, "NB ReportErrorCode Set %d Ok", CMEEnable);
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
	
	if ((NBStatus = NBIOT_Neul_NBxx_CheckReadSupportedBands(pClient)) == NBIOT_OK) 
	{
		/* Dictate execute is Success */
		NBIOT_COAP_DictateEvent_SuccessExecute(pClient, NBAND_MODE_CHECK, NBAND_MODE_CHECK);
		
#ifdef COAP_DEBUG_LOG_PRINT
		GPRS_DEBUG(DEBUG_INFO, "Coap BAND Read %d Ok", pClient->Parameter.band);
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
	
	if ((NBStatus = NBIOT_Neul_NBxx_CheckReadSupportedBands(pClient)) == NBIOT_OK) 
	{
		/* Dictate execute is Success */
		NBIOT_COAP_DictateEvent_SuccessExecute(pClient, FULL_FUNCTIONALITY, NBAND_MODE_CONFIG);
		
#ifdef COAP_DEBUG_LOG_PRINT
		GPRS_DEBUG(DEBUG_INFO, "Coap BAND Read %d Ok", pClient->Parameter.band);
#endif
	}
	else 
	{	
		/* Dictate execute is Fail */
		//NBIOT_COAP_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, NBAND_MODE_CONFIG);
		
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
			GPRS_DEBUG(DEBUG_INFO, "Coap BAND Set %d Ok", NBIOT_COAP_BAND_TYPE);
#endif
		}
		else 
		{
			/* Dictate execute is Fail */
			//NBIOT_COAP_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, NBAND_MODE_CONFIG);
				
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
	
	if ((NBStatus = NBIOT_Neul_NBxx_SetAttachOrDetach(pClient, Attach)) == NBIOT_OK)
	{
		/* Dictate execute is Success */
		NBIOT_COAP_DictateEvent_SuccessExecute(pClient, ATTACH_INQUIRE, ATTACH_EXECUTE);
		
#ifdef COAP_DEBUG_LOG_RF_PRINT
		GPRS_DEBUG(DEBUG_INFO, "Coap Set CGATT %d Ok", Attach);
#endif
	}
	else
	{
		/* Dictate execute is Fail */
		//NBIOT_COAP_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, ATTACH_EXECUTE);
		
#ifdef COAP_DEBUG_LOG_RF_PRINT
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
	
	if ((NBStatus = NBIOT_Neul_NBxx_CheckReadAttachOrDetach(pClient)) == NBIOT_OK) 
	{
		/* Dictate execute is Success */
		pClient->DictateRunCtl.dictateEvent = ATTACH_INQUIRE;
#ifdef COAP_DEBUG_LOG_RF_PRINT
		GPRS_DEBUG(DEBUG_INFO, "Coap CGATT %d Ok", pClient->Parameter.netstate);
#endif
	}
	else 
	{
		/* Dictate execute is Fail */
		NBIOT_COAP_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, ATTACH_INQUIRE);
		
#ifdef COAP_DEBUG_LOG_RF_PRINT
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
		//NBIOT_COAP_GetConnectTime(pClient, true);
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
		
#ifdef COAP_DEBUG_LOG_RF_PRINT
		GPRS_DEBUG(DEBUG_INFO, "Coap Parameter Check Ok");
#endif
	}
	else 
	{
		/* Dictate execute is Fail */
		NBIOT_COAP_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, PARAMETER_CHECKOUT);
		
#ifdef COAP_DEBUG_LOG_RF_PRINT
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
 @Function			void NET_NBIOT_COAP_Event_CDPServerCheck(NBIOT_ClientsTypeDef* pClient)
 @Description			NET_NBIOT_COAP_Event_CDPServerCheck	: CDP服务器查询
 @Input				pClient							: NBIOT客户端实例
 @Return				void
**********************************************************************************************************/

void NET_NBIOT_COAP_Event_CDPServerCheck(NBIOT_ClientsTypeDef* pClient)
{	
	NBIOT_StatusTypeDef NBStatus = NBStatus;
			
	if ((NBStatus = NBIOT_Neul_NBxx_CheckReadCDPServer(pClient)) == NBIOT_OK) 
	{
		/* Dictate execute is Success */
		NBIOT_COAP_DictateEvent_SuccessExecute(pClient, CDP_SERVER_CHECK, CDP_SERVER_CHECK);
		
#ifdef COAP_DEBUG_LOG_RF_PRINT
		GPRS_DEBUG(DEBUG_INFO,"Coap CDP Read %s:%d Ok", pClient->Parameter.cdpserver.CDPServerHost, pClient->Parameter.cdpserver.CDPServerPort);
#endif
	}
	else 
	{
		/* Dictate execute is Fail */
		NBIOT_COAP_DictateEvent_FailExecute(pClient, HARDWARE_REBOOT, STOP_MODE, CDP_SERVER_CHECK);
		
#ifdef COAP_DEBUG_LOG_RF_PRINT
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



