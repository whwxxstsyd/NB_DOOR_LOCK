/**
  *********************************************************************************************************
  * @file    net_mqttsn_message_operate.c
  * @author  Movebroad -- KK
  * @version V1.0
  * @date    2018-03-02
  * @brief   
  *********************************************************************************************************
  * @attention
  *
  *
  *
  *********************************************************************************************************
  */

#include "net_mqttsn_message_operate.h"
#include "platform_config.h"
#include "platform_map.h"
#include "stm32l1xx_config.h"
#include "hal_rtc.h"
#include "radar_api.h"
#include "tmesh_algorithm.h"
#include "string.h"

#if MQTTSN_MSG_VERSION_STREAM_TYPE == MQTTSN_MSG_VERSION_JSON_STREAM

MQTTSN_SwapStatusBasicTypeDef		NETMqttSNMessageParkStatusBasic;
MQTTSN_SwapStatusExtendTypeDef	NETMqttSNMessageParkStatusExtend;
MQTTSN_SwapInfoWorkTypeDef		NETMqttSNMessageParkInfoWork;
MQTTSN_SwapInfoBasicTypeDef		NETMqttSNMessageParkInfoBasic;
MQTTSN_SwapInfoDynamicTypeDef		NETMqttSNMessageParkInfoDynamic;
MQTTSN_SwapInfoRadarTypeDef		NETMqttSNMessageParkInfoRadar;
MQTTSN_SwapInfoResponseTypeDef	NETMqttSNMessageParkInfoResponse;

#endif

#if MQTTSN_MSG_VERSION_STREAM_TYPE == MQTTSN_MSG_VERSION_BYTE_STREAM

MQTTSN_SwapSendDataTypeDef		NETMqttSNMessageSendPark;

#endif

#if MQTTSN_MSG_VERSION_STREAM_TYPE == MQTTSN_MSG_VERSION_JSON_STREAM

/**********************************************************************************************************
 @Function			int NET_Message_Operate_Creat_Json_MoteStatus_Basic(char* outBuffer)
 @Description			NET_Message_Operate_Creat_Json_MoteStatus_Basic
 @Input				outBuffer
 @Return				Length
 @attention			!!<<< MaxLength 512Byte >>>!!
**********************************************************************************************************/
int NET_Message_Operate_Creat_Json_MoteStatus_Basic(char* outBuffer)
{
	MQTTSN_StatusBasicTypeDef dataBuf;
	struct tm datatime;
	
	if (NET_MqttSN_Message_StatusBasicDequeue(&dataBuf) != true) {
		return 0;
	}
	
	datatime = RTC_ConvUnixToCalendar(dataBuf.DateTime + 8 * 3600);
	
	sprintf(outBuffer, 
		"{"
			"\"SN\":\"%08x\","
			"\"Name\":\"TMoteStatus\","
			"\"TMoteStatus\":"
			"{"
				"\"Status\":%d,"
				"\"Count\":%d,"
				"\"Time\":\"20%02d-%02d-%02d %02d:%02d:%02d\""
			"}"
		"}",
		
		dataBuf.DeviceSN,
		dataBuf.Status,
		dataBuf.Count,
		datatime.tm_year, datatime.tm_mon, datatime.tm_mday,
		datatime.tm_hour, datatime.tm_min, datatime.tm_sec
	);
	
	return strlen(outBuffer);
}

/**********************************************************************************************************
 @Function			int NET_Message_Operate_Creat_Json_MoteStatus_Extend(char* outBuffer)
 @Description			NET_Message_Operate_Creat_Json_MoteStatus_Extend
 @Input				outBuffer
 @Return				Length
 @attention			!!<<< MaxLength 512Byte >>>!!
**********************************************************************************************************/
int NET_Message_Operate_Creat_Json_MoteStatus_Extend(char* outBuffer)
{
	MQTTSN_StatusExtendTypeDef dataBuf;
	struct tm datatime;
	
	if (NET_MqttSN_Message_StatusExtendDequeue(&dataBuf) != true) {
		return 0;
	}
	
	datatime = RTC_ConvUnixToCalendar(dataBuf.DateTime + 8 * 3600);
	
	sprintf(outBuffer, 
		"{"
			"\"SN\":\"%08x\","
			"\"Name\":\"TMoteStatus\","
			"\"TMoteStatus\":"
			"{"
				"\"Status\":%d,"
				"\"Count\":%d,"
				"\"Time\":\"20%02d-%02d-%02d %02d:%02d:%02d\","
				"\"x\":%d,"
				"\"y\":%d,"
				"\"z\":%d,"
#if MQTTSN_STATUS_MSG_VERSION_TYPE == MQTTSN_STATUS_MSG_VERSION_V2
				"\"xBack\":%d,"
				"\"yBack\":%d,"
				"\"zBack\":%d,"
#endif
				"\"MagDiff\":%d,"
				"\"Distance\":%d,"
				"\"Strength\":%d,"
				"\"CoverCount\":%d,"
#if MQTTSN_STATUS_MSG_VERSION_TYPE == MQTTSN_STATUS_MSG_VERSION_V1
				"\"RadarDiff\":%d"
#elif MQTTSN_STATUS_MSG_VERSION_TYPE == MQTTSN_STATUS_MSG_VERSION_V2
				"\"RadarDiff\":%d,"
				"\"RadarVal\":\"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\","
				"\"RadarBack\":\"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\","
				"\"Rssi\":%d,"
				"\"Snr\":%d,"
				"\"TempCore\":%d,"
				"\"TempQmc\":%d,"
				"\"DebugVal\":%d"
#endif
			"}"
		"}",
		
		dataBuf.DeviceSN,
		dataBuf.Status,
		dataBuf.Count,
		datatime.tm_year, datatime.tm_mon, datatime.tm_mday,
		datatime.tm_hour, datatime.tm_min, datatime.tm_sec,
		dataBuf.MagX,
		dataBuf.MagY,
		dataBuf.MagZ,
#if MQTTSN_STATUS_MSG_VERSION_TYPE == MQTTSN_STATUS_MSG_VERSION_V2
		dataBuf.MagneticBackX,
		dataBuf.MagneticBackY,
		dataBuf.MagneticBackZ,
#endif
		dataBuf.MagDiff,
		dataBuf.Distance,
		dataBuf.Strength,
		dataBuf.CoverCount,
#if MQTTSN_STATUS_MSG_VERSION_TYPE == MQTTSN_STATUS_MSG_VERSION_V1
		dataBuf.RadarDiff
#elif MQTTSN_STATUS_MSG_VERSION_TYPE == MQTTSN_STATUS_MSG_VERSION_V2
		dataBuf.RadarDiff,
		dataBuf.Radarval[0], dataBuf.Radarval[1], dataBuf.Radarval[2], dataBuf.Radarval[3],
		dataBuf.Radarval[4], dataBuf.Radarval[5], dataBuf.Radarval[6], dataBuf.Radarval[7],
		dataBuf.Radarval[8], dataBuf.Radarval[9], dataBuf.Radarval[10],dataBuf.Radarval[11],
		dataBuf.Radarval[12],dataBuf.Radarval[13],dataBuf.Radarval[14],dataBuf.Radarval[15],
		dataBuf.Radarback[0], dataBuf.Radarback[1], dataBuf.Radarback[2], dataBuf.Radarback[3],
		dataBuf.Radarback[4], dataBuf.Radarback[5], dataBuf.Radarback[6], dataBuf.Radarback[7],
		dataBuf.Radarback[8], dataBuf.Radarback[9], dataBuf.Radarback[10],dataBuf.Radarback[11],
		dataBuf.Radarback[12],dataBuf.Radarback[13],dataBuf.Radarback[14],dataBuf.Radarback[15],
		dataBuf.NBRssi,
		dataBuf.NBSnr,
		dataBuf.MCUTemp,
		dataBuf.QMCTemp,
		dataBuf.Debugval
#endif
	);
	
	return strlen(outBuffer);
}

/**********************************************************************************************************
 @Function			int NET_Message_Operate_Creat_Json_MoteInfo_Work(char* outBuffer)
 @Description			NET_Message_Operate_Creat_Json_MoteInfo_Work
 @Input				outBuffer
 @Return				Length
 @attention			!!<<< MaxLength 512Byte >>>!!
**********************************************************************************************************/
int NET_Message_Operate_Creat_Json_MoteInfo_Work(char* outBuffer)
{
	MQTTSN_InfoWorkTypeDef dataBuf;
	
	if (NET_MqttSN_Message_InfoWorkDequeue(&dataBuf) != true) {
		return 0;
	}
	
	TCFG_EEPROM_GetMagTempCoef(&TCFG_SystemData.MagCoefX, &TCFG_SystemData.MagCoefY, &TCFG_SystemData.MagCoefZ);
	
	sprintf(outBuffer, 
		"{"
			"\"SN\":\"%08x\","
			"\"WorkInfo\":"
			"{"
				"\"Sense\":%d,"
				"\"Mode\":\"%s\","
				"\"Channel\":%d,"
				"\"Range\":%d,"
				"\"Earfcn\":%d,"
				"\"Cellid\":%d,"
				"\"Cmdcnt\":\"%d.%d\","
				"\"Nbruntime\":\"%d.%d\","
				"\"NbruntimeDay\":\"%d.%d\","
				"\"Coef\":\"%d.%d.%d\","
				"\"Beepoff\":\"%d\""
			"}"
		"}",
		
		dataBuf.DeviceSN,
		TCFG_EEPROM_GetSavedSensitivity(),
		TCFG_EEPROM_Get_WorkMode_String(),
		TCFG_EEPROM_GetRfChannel(),
		TCFG_Utility_Get_DistanceRange(),
		TCFG_Utility_Get_Nbiot_RadioEARFCN(), 
		TCFG_Utility_Get_Nbiot_RadioCellID(),
		TCFG_EEPROM_GetRFCmdCnt(), TCFG_EEPROM_GetNBCmdCnt(),
		TCFG_Utility_GetCoapConnectTime(), TCFG_Utility_GetCoapIdleTime(),
		TCFG_Utility_GetCoapConnectDayTime(), TCFG_Utility_GetCoapIdleDayTime(),
		TCFG_SystemData.MagCoefX, TCFG_SystemData.MagCoefY, TCFG_SystemData.MagCoefZ,
		TCFG_EEPROM_GetBeepOff()
	);
	
	return strlen(outBuffer);
}

/**********************************************************************************************************
 @Function			int NET_Message_Operate_Creat_Json_MoteInfo_Basic(char* outBuffer)
 @Description			NET_Message_Operate_Creat_Json_MoteInfo_Basic
 @Input				outBuffer
 @Return				Length
 @attention			!!<<< MaxLength 512Byte >>>!!
**********************************************************************************************************/
int NET_Message_Operate_Creat_Json_MoteInfo_Basic(char* outBuffer)
{
	MQTTSN_InfoBasicTypeDef dataBuf;
	
	if (NET_MqttSN_Message_InfoBasicDequeue(&dataBuf) != true) {
		return 0;
	}
	
	sprintf(outBuffer, 
		"{"
			"\"SN\":\"%08x\","
			"\"TMoteInfo\":"
			"{"
				"\"Type\":\"%d.2\","
				"\"Vender\":\"%s\","
				"\"Hard\":\"%s\","
				"\"Soft\":\"%d:%d.%d\","
				"\"Sim\":\"%s\","
				"\"Imei\":\"%s\","
				"\"Nbvender\":\"%s\","
				"\"Nbmode\":\"%s\","
				"\"Boot\":\"%d.%d\","
				"\"Ver\":\"%s\","
				"\"Rmold\":\"%d\""
			"}"
		"}",
		
		dataBuf.DeviceSN,
		TCFG_Utility_Get_Mvb_ModelType(),
		TCFG_EEPROM_Get_Vender_String(),
		TCFG_Utility_Get_Hardwear_Version_String(),
		TCFG_EEPROM_GetBootVersion(), TCFG_Utility_Get_Major_Softnumber(), TCFG_Utility_Get_Sub_Softnumber(),
		TCFG_Utility_Get_Nbiot_Iccid_String(),
		TCFG_Utility_Get_Nbiot_Imei_String(),
		TCFG_Utility_Get_Nbiot_Manufacturer(),
		TCFG_Utility_Get_Nbiot_Manufacturermode(),
		TCFG_Utility_Get_SoftResetFlag(), TCFG_Utility_Get_Device_BootCount(),
		TCFG_Utility_Get_Nbiot_ModelVersion(),
		Radar_GetModel()
	);
	
	return strlen(outBuffer);
}

/**********************************************************************************************************
 @Function			int NET_Message_Operate_Creat_Json_MoteInfo_Dynamic(char* outBuffer)
 @Description			NET_Message_Operate_Creat_Json_MoteInfo_Dynamic
 @Input				outBuffer
 @Return				Length
 @attention			!!<<< MaxLength 512Byte >>>!!
**********************************************************************************************************/
int NET_Message_Operate_Creat_Json_MoteInfo_Dynamic(char* outBuffer)
{
	MQTTSN_InfoDynamicTypeDef dataBuf;
	
	if (NET_MqttSN_Message_InfoDynamicDequeue(&dataBuf) != true) {
		return 0;
	}
	
	sprintf(outBuffer, 
		"{"
			"\"SN\":\"%08x\","
			"\"TMoteInfo\":"
			"{"
				"\"Runtime\":%d,"
				"\"Rssi\":%d,"
				"\"Snr\":%d,"
				"\"Batt\":%d,"
				"\"Rlib\":\"%d\","
				"\"Rcnt\":%d,"
				"\"Temp\":%d,"
				"\"Rdgb\":%d,"
				"\"Psm\":%d,"
				"\"Algo\":%d,"
				"\"Qmcrbt\":%d,"
				"\"Nbboot\":%d,"
				"\"Nbsent\":%d,"
				"\"Nbrecv\":%d,"
				"\"Indelay\":%d,"
				"\"Nbheart\":%d"
			"}"
		"}",
		
		dataBuf.DeviceSN,
		TCFG_Utility_Get_Run_Time(),
		TCFG_Utility_Get_Nbiot_Rssi_IntVal(),
		TCFG_Utility_Get_Nbiot_RadioSNR(),
		TCFG_Utility_Get_Device_Batt_ShortVal(),
		TCFG_Utility_Get_RadarLibNum(),
		TCFG_GetRadarCount(),
		TCFG_Utility_Get_Device_Temperature(),
		TCFG_EEPROM_GetRadarDbgMode(),
		TCFG_EEPROM_GetEnableNBiotPSM(),
		TCFG_Utility_Get_AlgoLibNum(),
		TCFG_Utility_Get_ReInitModuleCount(),
		TCFG_Utility_Get_Nbiot_BootCount(),
		TCFG_Utility_Get_Nbiot_SentCount(),
		TCFG_Utility_Get_Nbiot_RecvCount(),
		TCFG_EEPROM_GetCarInDelay(),
		TCFG_EEPROM_GetNbiotHeart()
	);
	
	return strlen(outBuffer);
}

/**********************************************************************************************************
 @Function			int NET_Message_Operate_Creat_Json_MoteInfo_Radar(char* outBuffer)
 @Description			NET_Message_Operate_Creat_Json_MoteInfo_Radar
 @Input				outBuffer
 @Return				Length
 @attention			!!<<< MaxLength 512Byte >>>!!
**********************************************************************************************************/
int NET_Message_Operate_Creat_Json_MoteInfo_Radar(char* outBuffer)
{
	MQTTSN_InfoRadarTypeDef dataBuf;
	
	if (NET_MqttSN_Message_InfoRadarDequeue(&dataBuf) != true) {
		return 0;
	}
	
	sprintf(outBuffer, 
		"{"
			"\"SN\":\"%08x\","
			"\"RadarDbg\":"
			"{"
				"\"dif\":%d,"
				"\"x\":%d,"
				"\"y\":%d,"
				"\"z\":%d,"
				"\"snr\":%d,"
				"\"rssi\":%d,"
				"\"temp\":%d,"
				"\"qtemp\":%d,"
				"\"other\":\"%02d%02d%02d%02d%02d,%02d%02d%02d%02d%02d,%02d%02d%02d%02d%02d%02d;%02d%02d%02d%02d%02d,%02d%02d%02d.dif=%d.frl=%d&%d(%d,%d,%d)%u\""
			"}"
		"}",
		
		dataBuf.DeviceSN,
		sRadarData.Diff,
		Qmc5883lData.X_Now,
		Qmc5883lData.Y_Now,
		Qmc5883lData.Z_Now,
		TCFG_Utility_Get_Nbiot_RadioSNR(),
		TCFG_Utility_Get_Nbiot_Rssi_IntVal(),
		TCFG_Utility_Get_Device_Temperature(),
		Qmc5883lData.temp_now,
		radar_targetinfo.pMagNow[2],  radar_targetinfo.pMagNow[3],  radar_targetinfo.pMagNow[4],  radar_targetinfo.pMagNow[5],
		radar_targetinfo.pMagNow[6],  radar_targetinfo.pMagNow[7],  radar_targetinfo.pMagNow[8],  radar_targetinfo.pMagNow[9],
		radar_targetinfo.pMagNow[10], radar_targetinfo.pMagNow[11], radar_targetinfo.pMagNow[12], radar_targetinfo.pMagNow[13],
		radar_targetinfo.pMagNow[14], radar_targetinfo.pMagNow[15], radar_targetinfo.pMagNow[16], radar_targetinfo.pMagNow[17],
		radar_targetinfo.pMagBG[2], radar_targetinfo.pMagBG[3], radar_targetinfo.pMagBG[4], radar_targetinfo.pMagBG[5],
		radar_targetinfo.pMagBG[6], radar_targetinfo.pMagBG[7], radar_targetinfo.pMagBG[8], radar_targetinfo.pMagBG[9],
		sRadarData.Diff_v2,
		talgo_get_fredomain_least(),
		talgo_get_fredomain_least_inhalfhour(),
		Qmc5883lData.X_Back,
		Qmc5883lData.Y_Back,
		Qmc5883lData.Z_Back,
		errcode
	);
	
	return strlen(outBuffer);
}

/**********************************************************************************************************
 @Function			int NET_Message_Operate_Creat_Json_MoteInfo_Response(char* outBuffer)
 @Description			NET_Message_Operate_Creat_Json_MoteInfo_Response
 @Input				outBuffer
					errcode
 @Return				Length
 @attention			!!<<< MaxLength 512Byte >>>!!
**********************************************************************************************************/
int NET_Message_Operate_Creat_Json_MoteInfo_Response(char* outBuffer)
{
	MQTTSN_InfoResponseTypeDef dataBuf;
	
	if (NET_MqttSN_Message_InfoResponseDequeue(&dataBuf) != true) {
		return 0;
	}
	
	sprintf(outBuffer, 
		"{"
			"\"SN\":\"%08x\","
			"\"ResponseInfo\":"
			"{"
				"\"ret\":%d"
			"}"
		"}",
		
		dataBuf.DeviceSN,
		dataBuf.Errcode
	);
	
	return strlen(outBuffer);
}

#endif

#if MQTTSN_MSG_VERSION_STREAM_TYPE == MQTTSN_MSG_VERSION_BYTE_STREAM

/**********************************************************************************************************
 @Function			int NET_MQTTSN_Message_Operate_Creat_Json_Work_Info(char* outBuffer)
 @Description			NET_MQTTSN_Message_Operate_Creat_Json_Work_Info
 @Input				outBuffer
 @Return				Length
 @attention			!!<<< MaxLength 240Byte >>>!!
**********************************************************************************************************/
int NET_MQTTSN_Message_Operate_Creat_Json_Work_Info(char* outBuffer)
{
	TCFG_EEPROM_GetMagTempCoef(&TCFG_SystemData.MagCoefX, &TCFG_SystemData.MagCoefY, &TCFG_SystemData.MagCoefZ);
	
	sprintf(outBuffer, 
		"{"
			"\"SN\":\"%08x\","
			"\"WorkInfo\":"
			"{"
				"\"Sense\":%d,"
				"\"Mode\":\"%s\","
				"\"Channel\":%d,"
				"\"Range\":%d,"
				"\"Earfcn\":%d,"
				"\"Cellid\":%d,"
				"\"Cmdcnt\":\"%d.%d\","
				"\"Nbruntime\":\"%d.%d\","
				"\"NbruntimeDay\":\"%d.%d\","
				"\"Coef\":\"%d.%d.%d\","
				"\"Beepoff\":\"%d\""
			"}"
		"}",
		
		TCFG_EEPROM_Get_MAC_SN(),
		TCFG_EEPROM_GetSavedSensitivity(),
		TCFG_EEPROM_Get_WorkMode_String(),
		TCFG_EEPROM_GetRfChannel(),
		TCFG_Utility_Get_DistanceRange(),
		TCFG_Utility_Get_Nbiot_RadioEARFCN(),
		TCFG_Utility_Get_Nbiot_RadioCellID(),
		TCFG_EEPROM_GetRFCmdCnt(), TCFG_EEPROM_GetNBCmdCnt(),
		TCFG_Utility_GetCoapConnectTime(), TCFG_Utility_GetCoapIdleTime(),
		TCFG_Utility_GetCoapConnectDayTime(), TCFG_Utility_GetCoapIdleDayTime(),
		TCFG_SystemData.MagCoefX, TCFG_SystemData.MagCoefY, TCFG_SystemData.MagCoefZ,
		TCFG_EEPROM_GetBeepOff()
	);
	
	return strlen(outBuffer);
}

/**********************************************************************************************************
 @Function			int NET_MQTTSN_Message_Operate_Creat_Json_Basic_Info(char* outBuffer)
 @Description			NET_MQTTSN_Message_Operate_Creat_Json_Basic_Info
 @Input				outBuffer
 @Return				Length
 @attention			!!<<< MaxLength 240Byte >>>!!
**********************************************************************************************************/
int NET_MQTTSN_Message_Operate_Creat_Json_Basic_Info(char* outBuffer)
{
	sprintf(outBuffer, 
		"{"
			"\"SN\":\"%08x\","
			"\"TMoteInfo\":"
			"{"
				"\"Type\":\"%d.1\","
				"\"Vender\":\"%s\","
				"\"Hard\":\"%s\","
				"\"Soft\":\"%d:%d.%d\","
				"\"Sim\":\"%s\","
				"\"Imei\":\"%s\","
				"\"Nbvender\":\"%s\","
				"\"Nbmode\":\"%s\","
				"\"Boot\":\"%d.%d\","
				"\"Ver\":\"%s\","
				"\"Rmold\":\"%d\""
			"}"
		"}",
		
		TCFG_EEPROM_Get_MAC_SN(),
		TCFG_Utility_Get_Mvb_ModelType(),
		TCFG_EEPROM_Get_Vender_String(),
		TCFG_Utility_Get_Hardwear_Version_String(),
		TCFG_EEPROM_GetBootVersion(), TCFG_Utility_Get_Major_Softnumber(), TCFG_Utility_Get_Sub_Softnumber(),
		TCFG_Utility_Get_Nbiot_Iccid_String(),
		TCFG_Utility_Get_Nbiot_Imei_String(),
		TCFG_Utility_Get_Nbiot_Manufacturer(),
		TCFG_Utility_Get_Nbiot_Manufacturermode(),
		TCFG_Utility_Get_SoftResetFlag(), TCFG_Utility_Get_Device_BootCount(),
		TCFG_Utility_Get_Nbiot_ModelVersion(),
		Radar_GetModel()
	);
	
	return strlen(outBuffer);
}

/**********************************************************************************************************
 @Function			int NET_MQTTSN_Message_Operate_Creat_Json_Dynamic_Info(char* outBuffer)
 @Description			NET_MQTTSN_Message_Operate_Creat_Json_Dynamic_Info
 @Input				outBuffer
 @Return				Length
 @attention			!!<<< MaxLength 240Byte >>>!!
**********************************************************************************************************/
int NET_MQTTSN_Message_Operate_Creat_Json_Dynamic_Info(char* outBuffer)
{
	sprintf(outBuffer, 
		"{"
			"\"SN\":\"%08x\","
			"\"TMoteInfo\":"
			"{"
				"\"Runtime\":%d,"
				"\"Rssi\":%d,"
				"\"Snr\":%d,"
				"\"Batt\":%d,"
				"\"Rlib\":\"%d\","
				"\"Rcnt\":%d,"
				"\"Temp\":%d,"
				"\"Rdgb\":%d,"
				"\"Psm\":%d,"
				"\"Algo\":%d,"
				"\"Qmcrbt\":%d,"
				"\"Nbboot\":%d,"
				"\"Nbsent\":%d,"
				"\"Nbrecv\":%d,"
				"\"Indelay\":%d,"
				"\"Nbheart\":%d"
			"}"
		"}",
		
		TCFG_EEPROM_Get_MAC_SN(),
		TCFG_Utility_Get_Run_Time(),
		TCFG_Utility_Get_Nbiot_Rssi_IntVal(),
		TCFG_Utility_Get_Nbiot_RadioSNR(),
		TCFG_Utility_Get_Device_Batt_ShortVal(),
		TCFG_Utility_Get_RadarLibNum(),
		TCFG_GetRadarCount(),
		TCFG_Utility_Get_Device_Temperature(),
		TCFG_EEPROM_GetRadarDbgMode(),
		TCFG_EEPROM_GetEnableNBiotPSM(),
		TCFG_Utility_Get_AlgoLibNum(),
		TCFG_Utility_Get_ReInitModuleCount(),
		TCFG_Utility_Get_Nbiot_BootCount(),
		TCFG_Utility_Get_Nbiot_SentCount(),
		TCFG_Utility_Get_Nbiot_RecvCount(),
		TCFG_EEPROM_GetCarInDelay(),
		TCFG_EEPROM_GetNbiotHeart()
	);
	
	return strlen(outBuffer);
}

/**********************************************************************************************************
 @Function			int NET_MQTTSN_Message_Operate_Creat_Json_Radar_Info(char* outBuffer)
 @Description			NET_MQTTSN_Message_Operate_Creat_Json_Radar_Info
 @Input				outBuffer
 @Return				Length
 @attention			!!<<< MaxLength 240Byte >>>!!
**********************************************************************************************************/
int NET_MQTTSN_Message_Operate_Creat_Json_Radar_Info(char* outBuffer)
{
	sprintf(outBuffer, 
		"{"
			"\"SN\":\"%08x\","
			"\"RadarDbg\":"
			"{"
				"\"dif\":%d,"
				"\"x\":%d,"
				"\"y\":%d,"
				"\"z\":%d,"
				"\"snr\":%d,"
				"\"rssi\":%d,"
				"\"temp\":%d,"
				"\"qtemp\":%d,"
				"\"other\":\"%02d%02d%02d%02d%02d,%02d%02d%02d%02d%02d,%02d%02d%02d%02d%02d%02d;%02d%02d%02d%02d%02d,%02d%02d%02d.dif=%d.frl=%d&%d(%d,%d,%d)%u\""
			"}"
		"}",
		
		TCFG_EEPROM_Get_MAC_SN(),
		sRadarData.Diff,
		Qmc5883lData.X_Now,
		Qmc5883lData.Y_Now,
		Qmc5883lData.Z_Now,
		TCFG_Utility_Get_Nbiot_RadioSNR(),
		TCFG_Utility_Get_Nbiot_Rssi_IntVal(),
		TCFG_Utility_Get_Device_Temperature(),
		Qmc5883lData.temp_now,
		radar_targetinfo.pMagNow[2],  radar_targetinfo.pMagNow[3],  radar_targetinfo.pMagNow[4],  radar_targetinfo.pMagNow[5],
		radar_targetinfo.pMagNow[6],  radar_targetinfo.pMagNow[7],  radar_targetinfo.pMagNow[8],  radar_targetinfo.pMagNow[9],
		radar_targetinfo.pMagNow[10], radar_targetinfo.pMagNow[11], radar_targetinfo.pMagNow[12], radar_targetinfo.pMagNow[13],
		radar_targetinfo.pMagNow[14], radar_targetinfo.pMagNow[15], radar_targetinfo.pMagNow[16], radar_targetinfo.pMagNow[17],
		radar_targetinfo.pMagBG[2], radar_targetinfo.pMagBG[3], radar_targetinfo.pMagBG[4], radar_targetinfo.pMagBG[5],
		radar_targetinfo.pMagBG[6], radar_targetinfo.pMagBG[7], radar_targetinfo.pMagBG[8], radar_targetinfo.pMagBG[9],
		sRadarData.Diff_v2,
		talgo_get_fredomain_least(),
		talgo_get_fredomain_least_inhalfhour(),
		Qmc5883lData.X_Back,
		Qmc5883lData.Y_Back,
		Qmc5883lData.Z_Back,
		errcode
	);
	
	return strlen(outBuffer);
}

/**********************************************************************************************************
 @Function			int NET_MQTTSN_Message_Operate_Creat_Json_Response_Info(char* outBuffer, u16 errcode)
 @Description			NET_MQTTSN_Message_Operate_Creat_Json_Response_Info
 @Input				outBuffer
					errcode
 @Return				Length
 @attention			!!<<< MaxLength 240Byte >>>!!
**********************************************************************************************************/
int NET_MQTTSN_Message_Operate_Creat_Json_Response_Info(char* outBuffer, u16 errcode)
{
	sprintf(outBuffer, 
		"{"
			"\"SN\":\"%08x\","
			"\"ResponseInfo\":"
			"{"
				"\"ret\":%d"
			"}"
		"}",
		
		TCFG_EEPROM_Get_MAC_SN(),
		errcode
	);
	
	return strlen(outBuffer);
}

#endif

#if MQTTSN_MSG_VERSION_STREAM_TYPE == MQTTSN_MSG_VERSION_JSON_STREAM

/********************************************Is Full******************************************************/
/**********************************************************************************************************
 @Function			bool NET_MqttSN_Message_StatusBasicisFull(void)
 @Description			NET_MqttSN_Message_StatusBasicisFull
 @Input				void
 @Return				true							: 已满
					false						: 未满
**********************************************************************************************************/
bool NET_MqttSN_Message_StatusBasicisFull(void)
{
	bool MessageState;
	
	if ((NETMqttSNMessageParkStatusBasic.Rear + 1) % MQTTSN_STATUS_BASIC_PARK_NUM == NETMqttSNMessageParkStatusBasic.Front) {
		MessageState = true;
	}
	else {
		MessageState = false;
	}
	
	return MessageState;
}

/**********************************************************************************************************
 @Function			bool NET_MqttSN_Message_StatusExtendisFull(void)
 @Description			NET_MqttSN_Message_StatusExtendisFull
 @Input				void
 @Return				true							: 已满
					false						: 未满
**********************************************************************************************************/
bool NET_MqttSN_Message_StatusExtendisFull(void)
{
	bool MessageState;
	
	if ((NETMqttSNMessageParkStatusExtend.Rear + 1) % MQTTSN_STATUS_EXTEND_PARK_NUM == NETMqttSNMessageParkStatusExtend.Front) {
		MessageState = true;
	}
	else {
		MessageState = false;
	}
	
	return MessageState;
}

/**********************************************************************************************************
 @Function			bool NET_MqttSN_Message_InfoWorkisFull(void)
 @Description			NET_MqttSN_Message_InfoWorkisFull
 @Input				void
 @Return				true							: 已满
					false						: 未满
**********************************************************************************************************/
bool NET_MqttSN_Message_InfoWorkisFull(void)
{
	bool MessageState;
	
	if ((NETMqttSNMessageParkInfoWork.Rear + 1) % MQTTSN_INFO_WORK_PARK_NUM == NETMqttSNMessageParkInfoWork.Front) {
		MessageState = true;
	}
	else {
		MessageState = false;
	}
	
	return MessageState;
}

/**********************************************************************************************************
 @Function			bool NET_MqttSN_Message_InfoBasicisFull(void)
 @Description			NET_MqttSN_Message_InfoBasicisFull
 @Input				void
 @Return				true							: 已满
					false						: 未满
**********************************************************************************************************/
bool NET_MqttSN_Message_InfoBasicisFull(void)
{
	bool MessageState;
	
	if ((NETMqttSNMessageParkInfoBasic.Rear + 1) % MQTTSN_INFO_BASIC_PARK_NUM == NETMqttSNMessageParkInfoBasic.Front) {
		MessageState = true;
	}
	else {
		MessageState = false;
	}
	
	return MessageState;
}

/**********************************************************************************************************
 @Function			bool NET_MqttSN_Message_InfoDynamicisFull(void)
 @Description			NET_MqttSN_Message_InfoDynamicisFull
 @Input				void
 @Return				true							: 已满
					false						: 未满
**********************************************************************************************************/
bool NET_MqttSN_Message_InfoDynamicisFull(void)
{
	bool MessageState;
	
	if ((NETMqttSNMessageParkInfoDynamic.Rear + 1) % MQTTSN_INFO_DYNAMIC_PARK_NUM == NETMqttSNMessageParkInfoDynamic.Front) {
		MessageState = true;
	}
	else {
		MessageState = false;
	}
	
	return MessageState;
}

/**********************************************************************************************************
 @Function			bool NET_MqttSN_Message_InfoRadarisFull(void)
 @Description			NET_MqttSN_Message_InfoRadarisFull
 @Input				void
 @Return				true							: 已满
					false						: 未满
**********************************************************************************************************/
bool NET_MqttSN_Message_InfoRadarisFull(void)
{
	bool MessageState;
	
	if ((NETMqttSNMessageParkInfoRadar.Rear + 1) % MQTTSN_INFO_RADAR_PARK_NUM == NETMqttSNMessageParkInfoRadar.Front) {
		MessageState = true;
	}
	else {
		MessageState = false;
	}
	
	return MessageState;
}

/**********************************************************************************************************
 @Function			bool NET_MqttSN_Message_InfoResponseisFull(void)
 @Description			NET_MqttSN_Message_InfoResponseisFull
 @Input				void
 @Return				true							: 已满
					false						: 未满
**********************************************************************************************************/
bool NET_MqttSN_Message_InfoResponseisFull(void)
{
	bool MessageState;
	
	if ((NETMqttSNMessageParkInfoResponse.Rear + 1) % MQTTSN_INFO_RESPONSE_PARK_NUM == NETMqttSNMessageParkInfoResponse.Front) {
		MessageState = true;
	}
	else {
		MessageState = false;
	}
	
	return MessageState;
}

/********************************************Is Empty*****************************************************/
/**********************************************************************************************************
 @Function			bool NET_MqttSN_Message_StatusBasicisEmpty(void)
 @Description			NET_MqttSN_Message_StatusBasicisEmpty
 @Input				void
 @Return				true							: 已空
					false						: 未空
**********************************************************************************************************/
bool NET_MqttSN_Message_StatusBasicisEmpty(void)
{
	bool MessageState;
	
	if (NETMqttSNMessageParkStatusBasic.Front == NETMqttSNMessageParkStatusBasic.Rear) {
		MessageState = true;
	}
	else {
		MessageState = false;
	}
	
	return MessageState;
}

/**********************************************************************************************************
 @Function			bool NET_MqttSN_Message_StatusExtendisEmpty(void)
 @Description			NET_MqttSN_Message_StatusExtendisEmpty
 @Input				void
 @Return				true							: 已空
					false						: 未空
**********************************************************************************************************/
bool NET_MqttSN_Message_StatusExtendisEmpty(void)
{
	bool MessageState;
	
	if (NETMqttSNMessageParkStatusExtend.Front == NETMqttSNMessageParkStatusExtend.Rear) {
		MessageState = true;
	}
	else {
		MessageState = false;
	}
	
	return MessageState;
}

/**********************************************************************************************************
 @Function			bool NET_MqttSN_Message_InfoWorkisEmpty(void)
 @Description			NET_MqttSN_Message_InfoWorkisEmpty
 @Input				void
 @Return				true							: 已空
					false						: 未空
**********************************************************************************************************/
bool NET_MqttSN_Message_InfoWorkisEmpty(void)
{
	bool MessageState;
	
	if (NETMqttSNMessageParkInfoWork.Front == NETMqttSNMessageParkInfoWork.Rear) {
		MessageState = true;
	}
	else {
		MessageState = false;
	}
	
	return MessageState;
}

/**********************************************************************************************************
 @Function			bool NET_MqttSN_Message_InfoBasicisEmpty(void)
 @Description			NET_MqttSN_Message_InfoBasicisEmpty
 @Input				void
 @Return				true							: 已空
					false						: 未空
**********************************************************************************************************/
bool NET_MqttSN_Message_InfoBasicisEmpty(void)
{
	bool MessageState;
	
	if (NETMqttSNMessageParkInfoBasic.Front == NETMqttSNMessageParkInfoBasic.Rear) {
		MessageState = true;
	}
	else {
		MessageState = false;
	}
	
	return MessageState;
}

/**********************************************************************************************************
 @Function			bool NET_MqttSN_Message_InfoDynamicisEmpty(void)
 @Description			NET_MqttSN_Message_InfoDynamicisEmpty
 @Input				void
 @Return				true							: 已空
					false						: 未空
**********************************************************************************************************/
bool NET_MqttSN_Message_InfoDynamicisEmpty(void)
{
	bool MessageState;
	
	if (NETMqttSNMessageParkInfoDynamic.Front == NETMqttSNMessageParkInfoDynamic.Rear) {
		MessageState = true;
	}
	else {
		MessageState = false;
	}
	
	return MessageState;
}

/**********************************************************************************************************
 @Function			bool NET_MqttSN_Message_InfoRadarisEmpty(void)
 @Description			NET_MqttSN_Message_InfoRadarisEmpty
 @Input				void
 @Return				true							: 已空
					false						: 未空
**********************************************************************************************************/
bool NET_MqttSN_Message_InfoRadarisEmpty(void)
{
	bool MessageState;
	
	if (NETMqttSNMessageParkInfoRadar.Front == NETMqttSNMessageParkInfoRadar.Rear) {
		MessageState = true;
	}
	else {
		MessageState = false;
	}
	
	return MessageState;
}

/**********************************************************************************************************
 @Function			bool NET_MqttSN_Message_InfoResponseisEmpty(void)
 @Description			NET_MqttSN_Message_InfoResponseisEmpty
 @Input				void
 @Return				true							: 已空
					false						: 未空
**********************************************************************************************************/
bool NET_MqttSN_Message_InfoResponseisEmpty(void)
{
	bool MessageState;
	
	if (NETMqttSNMessageParkInfoResponse.Front == NETMqttSNMessageParkInfoResponse.Rear) {
		MessageState = true;
	}
	else {
		MessageState = false;
	}
	
	return MessageState;
}

/******************************************** Enqueue ****************************************************/
/**********************************************************************************************************
 @Function			void NET_MqttSN_Message_StatusBasicEnqueue(StatusBasicTypeDef dataBuf)
 @Description			NET_MqttSN_Message_StatusBasicEnqueue
 @Input				dataBuf	 		 				: 需写入数据
 @Return				void
**********************************************************************************************************/
void NET_MqttSN_Message_StatusBasicEnqueue(MQTTSN_StatusBasicTypeDef dataBuf)
{
	NETMqttSNMessageParkStatusBasic.Rear = (NETMqttSNMessageParkStatusBasic.Rear + 1) % MQTTSN_STATUS_BASIC_PARK_NUM;			//队尾偏移1
	NETMqttSNMessageParkStatusBasic.StatusBasic[NETMqttSNMessageParkStatusBasic.Rear].DeviceSN = dataBuf.DeviceSN;
	NETMqttSNMessageParkStatusBasic.StatusBasic[NETMqttSNMessageParkStatusBasic.Rear].Status = dataBuf.Status;
	NETMqttSNMessageParkStatusBasic.StatusBasic[NETMqttSNMessageParkStatusBasic.Rear].Count = dataBuf.Count;
	NETMqttSNMessageParkStatusBasic.StatusBasic[NETMqttSNMessageParkStatusBasic.Rear].DateTime = dataBuf.DateTime;
	
	if (NET_MqttSN_Message_StatusBasicisFull() == true) {															//队列已满
		NETMqttSNMessageParkStatusBasic.Front = (NETMqttSNMessageParkStatusBasic.Front + 1) % MQTTSN_STATUS_BASIC_PARK_NUM;	//队头偏移1
	}
}

/**********************************************************************************************************
 @Function			void NET_MqttSN_Message_StatusExtendEnqueue(StatusExtendTypeDef dataBuf)
 @Description			NET_MqttSN_Message_StatusExtendEnqueue
 @Input				dataBuf	 		 				: 需写入数据
 @Return				void
**********************************************************************************************************/
void NET_MqttSN_Message_StatusExtendEnqueue(MQTTSN_StatusExtendTypeDef dataBuf)
{
	NETMqttSNMessageParkStatusExtend.Rear = (NETMqttSNMessageParkStatusExtend.Rear + 1) % MQTTSN_STATUS_EXTEND_PARK_NUM;		//队尾偏移1
	NETMqttSNMessageParkStatusExtend.StatusExtend[NETMqttSNMessageParkStatusExtend.Rear].DeviceSN = dataBuf.DeviceSN;
	NETMqttSNMessageParkStatusExtend.StatusExtend[NETMqttSNMessageParkStatusExtend.Rear].Status = dataBuf.Status;
	NETMqttSNMessageParkStatusExtend.StatusExtend[NETMqttSNMessageParkStatusExtend.Rear].Count = dataBuf.Count;
	NETMqttSNMessageParkStatusExtend.StatusExtend[NETMqttSNMessageParkStatusExtend.Rear].DateTime = dataBuf.DateTime;
	NETMqttSNMessageParkStatusExtend.StatusExtend[NETMqttSNMessageParkStatusExtend.Rear].MagX = dataBuf.MagX;
	NETMqttSNMessageParkStatusExtend.StatusExtend[NETMqttSNMessageParkStatusExtend.Rear].MagY = dataBuf.MagY;
	NETMqttSNMessageParkStatusExtend.StatusExtend[NETMqttSNMessageParkStatusExtend.Rear].MagZ = dataBuf.MagZ;
	NETMqttSNMessageParkStatusExtend.StatusExtend[NETMqttSNMessageParkStatusExtend.Rear].MagDiff = dataBuf.MagDiff;
	NETMqttSNMessageParkStatusExtend.StatusExtend[NETMqttSNMessageParkStatusExtend.Rear].Distance = dataBuf.Distance;
	NETMqttSNMessageParkStatusExtend.StatusExtend[NETMqttSNMessageParkStatusExtend.Rear].Strength = dataBuf.Strength;
	NETMqttSNMessageParkStatusExtend.StatusExtend[NETMqttSNMessageParkStatusExtend.Rear].CoverCount = dataBuf.CoverCount;
	NETMqttSNMessageParkStatusExtend.StatusExtend[NETMqttSNMessageParkStatusExtend.Rear].RadarDiff = dataBuf.RadarDiff;
#if MQTTSN_STATUS_MSG_VERSION_TYPE == MQTTSN_STATUS_MSG_VERSION_V2
	NETMqttSNMessageParkStatusExtend.StatusExtend[NETMqttSNMessageParkStatusExtend.Rear].NBRssi = dataBuf.NBRssi;
	NETMqttSNMessageParkStatusExtend.StatusExtend[NETMqttSNMessageParkStatusExtend.Rear].NBSnr = dataBuf.NBSnr;
	NETMqttSNMessageParkStatusExtend.StatusExtend[NETMqttSNMessageParkStatusExtend.Rear].MCUTemp = dataBuf.MCUTemp;
	NETMqttSNMessageParkStatusExtend.StatusExtend[NETMqttSNMessageParkStatusExtend.Rear].QMCTemp = dataBuf.QMCTemp;
	NETMqttSNMessageParkStatusExtend.StatusExtend[NETMqttSNMessageParkStatusExtend.Rear].MagneticBackX = dataBuf.MagneticBackX;
	NETMqttSNMessageParkStatusExtend.StatusExtend[NETMqttSNMessageParkStatusExtend.Rear].MagneticBackY = dataBuf.MagneticBackY;
	NETMqttSNMessageParkStatusExtend.StatusExtend[NETMqttSNMessageParkStatusExtend.Rear].MagneticBackZ = dataBuf.MagneticBackZ;
	NETMqttSNMessageParkStatusExtend.StatusExtend[NETMqttSNMessageParkStatusExtend.Rear].Debugval = dataBuf.Debugval;
	for (int i = 0; i < 16; i++) {
		NETMqttSNMessageParkStatusExtend.StatusExtend[NETMqttSNMessageParkStatusExtend.Rear].Radarval[i] = dataBuf.Radarval[i];
		NETMqttSNMessageParkStatusExtend.StatusExtend[NETMqttSNMessageParkStatusExtend.Rear].Radarback[i] = dataBuf.Radarback[i];
	}
#endif
	
	if (NET_MqttSN_Message_StatusExtendisFull() == true) {															//队列已满
		NETMqttSNMessageParkStatusExtend.Front = (NETMqttSNMessageParkStatusExtend.Front + 1) % MQTTSN_STATUS_EXTEND_PARK_NUM;	//队头偏移1
	}
}

/**********************************************************************************************************
 @Function			void NET_MqttSN_Message_InfoWorkEnqueue(MQTTSN_InfoWorkTypeDef dataBuf)
 @Description			NET_MqttSN_Message_InfoWorkEnqueue
 @Input				dataBuf	 		 				: 需写入数据
 @Return				void
**********************************************************************************************************/
void NET_MqttSN_Message_InfoWorkEnqueue(MQTTSN_InfoWorkTypeDef dataBuf)
{
	NETMqttSNMessageParkInfoWork.Rear = (NETMqttSNMessageParkInfoWork.Rear + 1) % MQTTSN_INFO_WORK_PARK_NUM;					//队尾偏移1
	NETMqttSNMessageParkInfoWork.InfoWork[NETMqttSNMessageParkInfoWork.Rear].DeviceSN = dataBuf.DeviceSN;
	
	if (NET_MqttSN_Message_InfoWorkisFull() == true) {															//队列已满
		NETMqttSNMessageParkInfoWork.Front = (NETMqttSNMessageParkInfoWork.Front + 1) % MQTTSN_INFO_WORK_PARK_NUM;			//队头偏移1
	}
}

/**********************************************************************************************************
 @Function			void NET_MqttSN_Message_InfoBasicEnqueue(MQTTSN_InfoBasicTypeDef dataBuf)
 @Description			NET_MqttSN_Message_InfoBasicEnqueue
 @Input				dataBuf	 		 				: 需写入数据
 @Return				void
**********************************************************************************************************/
void NET_MqttSN_Message_InfoBasicEnqueue(MQTTSN_InfoBasicTypeDef dataBuf)
{
	NETMqttSNMessageParkInfoBasic.Rear = (NETMqttSNMessageParkInfoBasic.Rear + 1) % MQTTSN_INFO_BASIC_PARK_NUM;				//队尾偏移1
	NETMqttSNMessageParkInfoBasic.InfoBasic[NETMqttSNMessageParkInfoBasic.Rear].DeviceSN = dataBuf.DeviceSN;
	
	if (NET_MqttSN_Message_InfoBasicisFull() == true) {															//队列已满
		NETMqttSNMessageParkInfoBasic.Front = (NETMqttSNMessageParkInfoBasic.Front + 1) % MQTTSN_INFO_BASIC_PARK_NUM;			//队头偏移1
	}
}

/**********************************************************************************************************
 @Function			void NET_MqttSN_Message_InfoDynamicEnqueue(MQTTSN_InfoDynamicTypeDef dataBuf)
 @Description			NET_MqttSN_Message_InfoDynamicEnqueue
 @Input				dataBuf	 		 				: 需写入数据
 @Return				void
**********************************************************************************************************/
void NET_MqttSN_Message_InfoDynamicEnqueue(MQTTSN_InfoDynamicTypeDef dataBuf)
{
	NETMqttSNMessageParkInfoDynamic.Rear = (NETMqttSNMessageParkInfoDynamic.Rear + 1) % MQTTSN_INFO_DYNAMIC_PARK_NUM;			//队尾偏移1
	NETMqttSNMessageParkInfoDynamic.InfoDynamic[NETMqttSNMessageParkInfoDynamic.Rear].DeviceSN = dataBuf.DeviceSN;
	
	if (NET_MqttSN_Message_InfoDynamicisFull() == true) {															//队列已满
		NETMqttSNMessageParkInfoDynamic.Front = (NETMqttSNMessageParkInfoDynamic.Front + 1) % MQTTSN_INFO_DYNAMIC_PARK_NUM;	//队头偏移1
	}
}

/**********************************************************************************************************
 @Function			void NET_MqttSN_Message_InfoRadarEnqueue(MQTTSN_InfoRadarTypeDef dataBuf)
 @Description			NET_MqttSN_Message_InfoRadarEnqueue
 @Input				dataBuf	 		 				: 需写入数据
 @Return				void
**********************************************************************************************************/
void NET_MqttSN_Message_InfoRadarEnqueue(MQTTSN_InfoRadarTypeDef dataBuf)
{
	NETMqttSNMessageParkInfoRadar.Rear = (NETMqttSNMessageParkInfoRadar.Rear + 1) % MQTTSN_INFO_RADAR_PARK_NUM;				//队尾偏移1
	NETMqttSNMessageParkInfoRadar.InfoRadar[NETMqttSNMessageParkInfoRadar.Rear].DeviceSN = dataBuf.DeviceSN;
	
	if (NET_MqttSN_Message_InfoRadarisFull() == true) {															//队列已满
		NETMqttSNMessageParkInfoRadar.Front = (NETMqttSNMessageParkInfoRadar.Front + 1) % MQTTSN_INFO_RADAR_PARK_NUM;			//队头偏移1
	}
}

/**********************************************************************************************************
 @Function			void NET_MqttSN_Message_InfoResponseEnqueue(MQTTSN_InfoResponseTypeDef dataBuf)
 @Description			NET_MqttSN_Message_InfoResponseEnqueue
 @Input				dataBuf	 		 				: 需写入数据
 @Return				void
**********************************************************************************************************/
void NET_MqttSN_Message_InfoResponseEnqueue(MQTTSN_InfoResponseTypeDef dataBuf)
{
	NETMqttSNMessageParkInfoResponse.Rear = (NETMqttSNMessageParkInfoResponse.Rear + 1) % MQTTSN_INFO_RESPONSE_PARK_NUM;		//队尾偏移1
	NETMqttSNMessageParkInfoResponse.InfoResponse[NETMqttSNMessageParkInfoResponse.Rear].DeviceSN = dataBuf.DeviceSN;
	NETMqttSNMessageParkInfoResponse.InfoResponse[NETMqttSNMessageParkInfoResponse.Rear].Errcode = dataBuf.Errcode;
	
	if (NET_MqttSN_Message_InfoResponseisFull() == true) {															//队列已满
		NETMqttSNMessageParkInfoResponse.Front = (NETMqttSNMessageParkInfoResponse.Front + 1) % MQTTSN_INFO_RESPONSE_PARK_NUM;	//队头偏移1
	}
}

/******************************************** Dequeue ****************************************************/
/**********************************************************************************************************
 @Function			bool NET_MqttSN_Message_StatusBasicDequeue(StatusBasicTypeDef* dataBuf)
 @Description			NET_MqttSN_Message_StatusBasicDequeue
 @Input				dataBuf	 		 				: 需读出数据地址
 @Return				true								: 未空
					false							: 已空
**********************************************************************************************************/
bool NET_MqttSN_Message_StatusBasicDequeue(MQTTSN_StatusBasicTypeDef* dataBuf)
{
	bool MessageState;
	unsigned char front;
	
	if (NET_MqttSN_Message_StatusBasicisEmpty() == true) {															//队列已空
		MessageState = false;
	}
	else {																								//队列未空
		front = (NETMqttSNMessageParkStatusBasic.Front + 1) % MQTTSN_STATUS_BASIC_PARK_NUM;								//队头偏移1
		dataBuf->DeviceSN = NETMqttSNMessageParkStatusBasic.StatusBasic[front].DeviceSN;
		dataBuf->Status = NETMqttSNMessageParkStatusBasic.StatusBasic[front].Status;
		dataBuf->Count = NETMqttSNMessageParkStatusBasic.StatusBasic[front].Count;
		dataBuf->DateTime = NETMqttSNMessageParkStatusBasic.StatusBasic[front].DateTime;
		MessageState = true;
	}
	
	return MessageState;
}

/**********************************************************************************************************
 @Function			bool NET_MqttSN_Message_StatusExtendDequeue(StatusExtendTypeDef* dataBuf)
 @Description			NET_MqttSN_Message_StatusExtendDequeue
 @Input				dataBuf	 		 				: 需读出数据地址
 @Return				true								: 未空
					false							: 已空
**********************************************************************************************************/
bool NET_MqttSN_Message_StatusExtendDequeue(MQTTSN_StatusExtendTypeDef* dataBuf)
{
	bool MessageState;
	unsigned char front;
	
	if (NET_MqttSN_Message_StatusExtendisEmpty() == true) {														//队列已空
		MessageState = false;
	}
	else {																								//队列未空
		front = (NETMqttSNMessageParkStatusExtend.Front + 1) % MQTTSN_STATUS_EXTEND_PARK_NUM;							//队头偏移1
		dataBuf->DeviceSN = NETMqttSNMessageParkStatusExtend.StatusExtend[front].DeviceSN;
		dataBuf->Status = NETMqttSNMessageParkStatusExtend.StatusExtend[front].Status;
		dataBuf->Count = NETMqttSNMessageParkStatusExtend.StatusExtend[front].Count;
		dataBuf->DateTime = NETMqttSNMessageParkStatusExtend.StatusExtend[front].DateTime;
		dataBuf->MagX = NETMqttSNMessageParkStatusExtend.StatusExtend[front].MagX;
		dataBuf->MagY = NETMqttSNMessageParkStatusExtend.StatusExtend[front].MagY;
		dataBuf->MagZ = NETMqttSNMessageParkStatusExtend.StatusExtend[front].MagZ;
		dataBuf->MagDiff = NETMqttSNMessageParkStatusExtend.StatusExtend[front].MagDiff;
		dataBuf->Distance = NETMqttSNMessageParkStatusExtend.StatusExtend[front].Distance;
		dataBuf->Strength = NETMqttSNMessageParkStatusExtend.StatusExtend[front].Strength;
		dataBuf->CoverCount = NETMqttSNMessageParkStatusExtend.StatusExtend[front].CoverCount;
		dataBuf->RadarDiff = NETMqttSNMessageParkStatusExtend.StatusExtend[front].RadarDiff;
#if MQTTSN_STATUS_MSG_VERSION_TYPE == MQTTSN_STATUS_MSG_VERSION_V2
		dataBuf->NBRssi = NETMqttSNMessageParkStatusExtend.StatusExtend[front].NBRssi;
		dataBuf->NBSnr = NETMqttSNMessageParkStatusExtend.StatusExtend[front].NBSnr;
		dataBuf->MCUTemp = NETMqttSNMessageParkStatusExtend.StatusExtend[front].MCUTemp;
		dataBuf->QMCTemp = NETMqttSNMessageParkStatusExtend.StatusExtend[front].QMCTemp;
		dataBuf->MagneticBackX = NETMqttSNMessageParkStatusExtend.StatusExtend[front].MagneticBackX;
		dataBuf->MagneticBackY = NETMqttSNMessageParkStatusExtend.StatusExtend[front].MagneticBackY;
		dataBuf->MagneticBackZ = NETMqttSNMessageParkStatusExtend.StatusExtend[front].MagneticBackZ;
		dataBuf->Debugval = NETMqttSNMessageParkStatusExtend.StatusExtend[front].Debugval;
		for (int i = 0; i < 16; i++) {
			dataBuf->Radarval[i] = NETMqttSNMessageParkStatusExtend.StatusExtend[front].Radarval[i];
			dataBuf->Radarback[i] = NETMqttSNMessageParkStatusExtend.StatusExtend[front].Radarback[i];
		}
#endif
		MessageState = true;
	}
	
	return MessageState;
}

/**********************************************************************************************************
 @Function			bool NET_MqttSN_Message_InfoWorkDequeue(MQTTSN_InfoWorkTypeDef* dataBuf)
 @Description			NET_MqttSN_Message_InfoWorkDequeue
 @Input				dataBuf	 		 				: 需读出数据地址
 @Return				true								: 未空
					false							: 已空
**********************************************************************************************************/
bool NET_MqttSN_Message_InfoWorkDequeue(MQTTSN_InfoWorkTypeDef* dataBuf)
{
	bool MessageState;
	unsigned char front;
	
	if (NET_MqttSN_Message_InfoWorkisEmpty() == true) {															//队列已空
		MessageState = false;
	}
	else {																								//队列未空
		front = (NETMqttSNMessageParkInfoWork.Front + 1) % MQTTSN_INFO_WORK_PARK_NUM;									//队头偏移1
		dataBuf->DeviceSN = NETMqttSNMessageParkInfoWork.InfoWork[front].DeviceSN;
		MessageState = true;
	}
	
	return MessageState;
}

/**********************************************************************************************************
 @Function			bool NET_MqttSN_Message_InfoBasicDequeue(MQTTSN_InfoBasicTypeDef* dataBuf)
 @Description			NET_MqttSN_Message_InfoBasicDequeue
 @Input				dataBuf	 		 				: 需读出数据地址
 @Return				true								: 未空
					false							: 已空
**********************************************************************************************************/
bool NET_MqttSN_Message_InfoBasicDequeue(MQTTSN_InfoBasicTypeDef* dataBuf)
{
	bool MessageState;
	unsigned char front;
	
	if (NET_MqttSN_Message_InfoBasicisEmpty() == true) {															//队列已空
		MessageState = false;
	}
	else {																								//队列未空
		front = (NETMqttSNMessageParkInfoBasic.Front + 1) % MQTTSN_INFO_BASIC_PARK_NUM;									//队头偏移1
		dataBuf->DeviceSN = NETMqttSNMessageParkInfoBasic.InfoBasic[front].DeviceSN;
		MessageState = true;
	}
	
	return MessageState;
}

/**********************************************************************************************************
 @Function			bool NET_MqttSN_Message_InfoDynamicDequeue(MQTTSN_InfoDynamicTypeDef* dataBuf)
 @Description			NET_MqttSN_Message_InfoDynamicDequeue
 @Input				dataBuf	 		 				: 需读出数据地址
 @Return				true								: 未空
					false							: 已空
**********************************************************************************************************/
bool NET_MqttSN_Message_InfoDynamicDequeue(MQTTSN_InfoDynamicTypeDef* dataBuf)
{
	bool MessageState;
	unsigned char front;
	
	if (NET_MqttSN_Message_InfoDynamicisEmpty() == true) {															//队列已空
		MessageState = false;
	}
	else {																								//队列未空
		front = (NETMqttSNMessageParkInfoDynamic.Front + 1) % MQTTSN_INFO_DYNAMIC_PARK_NUM;								//队头偏移1
		dataBuf->DeviceSN = NETMqttSNMessageParkInfoDynamic.InfoDynamic[front].DeviceSN;
		MessageState = true;
	}
	
	return MessageState;
}

/**********************************************************************************************************
 @Function			bool NET_MqttSN_Message_InfoRadarDequeue(MQTTSN_InfoRadarTypeDef* dataBuf)
 @Description			NET_MqttSN_Message_InfoRadarDequeue
 @Input				dataBuf	 		 				: 需读出数据地址
 @Return				true								: 未空
					false							: 已空
**********************************************************************************************************/
bool NET_MqttSN_Message_InfoRadarDequeue(MQTTSN_InfoRadarTypeDef* dataBuf)
{
	bool MessageState;
	unsigned char front;
	
	if (NET_MqttSN_Message_InfoRadarisEmpty() == true) {															//队列已空
		MessageState = false;
	}
	else {																								//队列未空
		front = (NETMqttSNMessageParkInfoRadar.Front + 1) % MQTTSN_INFO_RADAR_PARK_NUM;									//队头偏移1
		dataBuf->DeviceSN = NETMqttSNMessageParkInfoRadar.InfoRadar[front].DeviceSN;
		MessageState = true;
	}
	
	return MessageState;
}

/**********************************************************************************************************
 @Function			bool NET_MqttSN_Message_InfoResponseDequeue(MQTTSN_InfoResponseTypeDef* dataBuf)
 @Description			NET_MqttSN_Message_InfoResponseDequeue
 @Input				dataBuf	 		 				: 需读出数据地址
 @Return				true								: 未空
					false							: 已空
**********************************************************************************************************/
bool NET_MqttSN_Message_InfoResponseDequeue(MQTTSN_InfoResponseTypeDef* dataBuf)
{
	bool MessageState;
	unsigned char front;
	
	if (NET_MqttSN_Message_InfoResponseisEmpty() == true) {														//队列已空
		MessageState = false;
	}
	else {																								//队列未空
		front = (NETMqttSNMessageParkInfoResponse.Front + 1) % MQTTSN_INFO_RESPONSE_PARK_NUM;							//队头偏移1
		dataBuf->DeviceSN = NETMqttSNMessageParkInfoResponse.InfoResponse[front].DeviceSN;
		dataBuf->Errcode = NETMqttSNMessageParkInfoResponse.InfoResponse[front].Errcode;
		MessageState = true;
	}
	
	return MessageState;
}

/********************************************* OffSet ****************************************************/
/**********************************************************************************************************
 @Function			bool NET_MqttSN_Message_StatusBasicOffSet(void)
 @Description			NET_MqttSN_Message_StatusBasicOffSet
 @Input				void
 @Return				true							: 未空
					false						: 已空
**********************************************************************************************************/
bool NET_MqttSN_Message_StatusBasicOffSet(void)
{
	bool MessageState;
	
	if (NET_MqttSN_Message_StatusBasicisEmpty() == true) {															//队列已空
		MessageState = false;
	}
	else {																								//队列未空
		NETMqttSNMessageParkStatusBasic.Front = (NETMqttSNMessageParkStatusBasic.Front + 1) % MQTTSN_STATUS_BASIC_PARK_NUM;
		MessageState = true;
	}
	
	return MessageState;
}

/**********************************************************************************************************
 @Function			bool NET_MqttSN_Message_StatusExtendOffSet(void)
 @Description			NET_MqttSN_Message_StatusExtendOffSet
 @Input				void
 @Return				true							: 未空
					false						: 已空
**********************************************************************************************************/
bool NET_MqttSN_Message_StatusExtendOffSet(void)
{
	bool MessageState;
	
	if (NET_MqttSN_Message_StatusExtendisEmpty() == true) {														//队列已空
		MessageState = false;
	}
	else {																								//队列未空
		NETMqttSNMessageParkStatusExtend.Front = (NETMqttSNMessageParkStatusExtend.Front + 1) % MQTTSN_STATUS_EXTEND_PARK_NUM;
		MessageState = true;
	}
	
	return MessageState;
}

/**********************************************************************************************************
 @Function			bool NET_MqttSN_Message_InfoWorkOffSet(void)
 @Description			NET_MqttSN_Message_InfoWorkOffSet
 @Input				void
 @Return				true							: 未空
					false						: 已空
**********************************************************************************************************/
bool NET_MqttSN_Message_InfoWorkOffSet(void)
{
	bool MessageState;
	
	if (NET_MqttSN_Message_InfoWorkisEmpty() == true) {															//队列已空
		MessageState = false;
	}
	else {																								//队列未空
		NETMqttSNMessageParkInfoWork.Front = (NETMqttSNMessageParkInfoWork.Front + 1) % MQTTSN_INFO_WORK_PARK_NUM;
		MessageState = true;
	}
	
	return MessageState;
}

/**********************************************************************************************************
 @Function			bool NET_MqttSN_Message_InfoBasicOffSet(void)
 @Description			NET_MqttSN_Message_InfoBasicOffSet
 @Input				void
 @Return				true							: 未空
					false						: 已空
**********************************************************************************************************/
bool NET_MqttSN_Message_InfoBasicOffSet(void)
{
	bool MessageState;
	
	if (NET_MqttSN_Message_InfoBasicisEmpty() == true) {															//队列已空
		MessageState = false;
	}
	else {																								//队列未空
		NETMqttSNMessageParkInfoBasic.Front = (NETMqttSNMessageParkInfoBasic.Front + 1) % MQTTSN_INFO_BASIC_PARK_NUM;
		MessageState = true;
	}
	
	return MessageState;
}

/**********************************************************************************************************
 @Function			bool NET_MqttSN_Message_InfoDynamicOffSet(void)
 @Description			NET_MqttSN_Message_InfoDynamicOffSet
 @Input				void
 @Return				true							: 未空
					false						: 已空
**********************************************************************************************************/
bool NET_MqttSN_Message_InfoDynamicOffSet(void)
{
	bool MessageState;
	
	if (NET_MqttSN_Message_InfoDynamicisEmpty() == true) {															//队列已空
		MessageState = false;
	}
	else {																								//队列未空
		NETMqttSNMessageParkInfoDynamic.Front = (NETMqttSNMessageParkInfoDynamic.Front + 1) % MQTTSN_INFO_DYNAMIC_PARK_NUM;
		MessageState = true;
	}
	
	return MessageState;
}

/**********************************************************************************************************
 @Function			bool NET_MqttSN_Message_InfoRadarOffSet(void)
 @Description			NET_MqttSN_Message_InfoRadarOffSet
 @Input				void
 @Return				true							: 未空
					false						: 已空
**********************************************************************************************************/
bool NET_MqttSN_Message_InfoRadarOffSet(void)
{
	bool MessageState;
	
	if (NET_MqttSN_Message_InfoRadarisEmpty() == true) {															//队列已空
		MessageState = false;
	}
	else {																								//队列未空
		NETMqttSNMessageParkInfoRadar.Front = (NETMqttSNMessageParkInfoRadar.Front + 1) % MQTTSN_INFO_RADAR_PARK_NUM;
		MessageState = true;
	}
	
	return MessageState;
}

/**********************************************************************************************************
 @Function			bool NET_MqttSN_Message_InfoResponseOffSet(void)
 @Description			NET_MqttSN_Message_InfoResponseOffSet
 @Input				void
 @Return				true							: 未空
					false						: 已空
**********************************************************************************************************/
bool NET_MqttSN_Message_InfoResponseOffSet(void)
{
	bool MessageState;
	
	if (NET_MqttSN_Message_InfoResponseisEmpty() == true) {														//队列已空
		MessageState = false;
	}
	else {																								//队列未空
		NETMqttSNMessageParkInfoResponse.Front = (NETMqttSNMessageParkInfoResponse.Front + 1) % MQTTSN_INFO_RESPONSE_PARK_NUM;
		MessageState = true;
	}
	
	return MessageState;
}

/********************************************* Rear *****************************************************/
/**********************************************************************************************************
 @Function			unsigned char NET_MqttSN_Message_StatusBasicRear(void)
 @Description			NET_MqttSN_Message_StatusBasicRear
 @Input				void
 @Return				队尾值
**********************************************************************************************************/
unsigned char NET_MqttSN_Message_StatusBasicRear(void)
{
	return NETMqttSNMessageParkStatusBasic.Rear;
}

/**********************************************************************************************************
 @Function			unsigned char NET_MqttSN_Message_StatusExtendRear(void)
 @Description			NET_MqttSN_Message_StatusExtendRear
 @Input				void
 @Return				队尾值
**********************************************************************************************************/
unsigned char NET_MqttSN_Message_StatusExtendRear(void)
{
	return NETMqttSNMessageParkStatusExtend.Rear;
}

/**********************************************************************************************************
 @Function			unsigned char NET_MqttSN_Message_InfoWorkRear(void)
 @Description			NET_MqttSN_Message_InfoWorkRear
 @Input				void
 @Return				队尾值
**********************************************************************************************************/
unsigned char NET_MqttSN_Message_InfoWorkRear(void)
{
	return NETMqttSNMessageParkInfoWork.Rear;
}

/**********************************************************************************************************
 @Function			unsigned char NET_MqttSN_Message_InfoBasicRear(void)
 @Description			NET_MqttSN_Message_InfoBasicRear
 @Input				void
 @Return				队尾值
**********************************************************************************************************/
unsigned char NET_MqttSN_Message_InfoBasicRear(void)
{
	return NETMqttSNMessageParkInfoBasic.Rear;
}

/**********************************************************************************************************
 @Function			unsigned char NET_MqttSN_Message_InfoDynamicRear(void)
 @Description			NET_MqttSN_Message_InfoDynamicRear
 @Input				void
 @Return				队尾值
**********************************************************************************************************/
unsigned char NET_MqttSN_Message_InfoDynamicRear(void)
{
	return NETMqttSNMessageParkInfoDynamic.Rear;
}

/**********************************************************************************************************
 @Function			unsigned char NET_MqttSN_Message_InfoRadarRear(void)
 @Description			NET_MqttSN_Message_InfoRadarRear
 @Input				void
 @Return				队尾值
**********************************************************************************************************/
unsigned char NET_MqttSN_Message_InfoRadarRear(void)
{
	return NETMqttSNMessageParkInfoRadar.Rear;
}

/**********************************************************************************************************
 @Function			unsigned char NET_MqttSN_Message_InfoResponseRear(void)
 @Description			NET_MqttSN_Message_InfoResponseRear
 @Input				void
 @Return				队尾值
**********************************************************************************************************/
unsigned char NET_MqttSN_Message_InfoResponseRear(void)
{
	return NETMqttSNMessageParkInfoResponse.Rear;
}

#endif

#if MQTTSN_MSG_VERSION_STREAM_TYPE == MQTTSN_MSG_VERSION_BYTE_STREAM

/**********************************************************************************************************
 @Function			bool NET_MqttSN_Message_SendDataisFull(void)
 @Description			NET_MqttSN_Message_SendDataisFull	: 检查发送队列是否已满
 @Input				void
 @Return				true							: 已满
					false						: 未满
**********************************************************************************************************/
bool NET_MqttSN_Message_SendDataisFull(void)
{
	bool MessageState;
	
	if ((NETMqttSNMessageSendPark.Rear + 1) % MQTTSN_SEND_PARK_NUM == NETMqttSNMessageSendPark.Front) {
		MessageState = true;
	}
	else {
		MessageState = false;
	}
	
	return MessageState;
}

/**********************************************************************************************************
 @Function			bool NET_MqttSN_Message_SendDataisEmpty(void)
 @Description			NET_MqttSN_Message_SendDataisEmpty	: 检查发送队列是否已空
 @Input				void
 @Return				true							: 已空
					false						: 未空
**********************************************************************************************************/
bool NET_MqttSN_Message_SendDataisEmpty(void)
{
	bool MessageState;
	
	if (NETMqttSNMessageSendPark.Front == NETMqttSNMessageSendPark.Rear) {
		MessageState = true;
	}
	else {
		MessageState = false;
	}
	
	return MessageState;
}

/**********************************************************************************************************
 @Function			void NET_MqttSN_Message_SendDataEnqueue(unsigned char* dataBuf, unsigned short dataLength)
 @Description			NET_MqttSN_Message_SendDataEnqueue	: 发送数据写入队列
 @Input				dataBuf	 		 			: 需写入数据
					dataLength					: 需写入数据长度
 @Return				void
**********************************************************************************************************/
void NET_MqttSN_Message_SendDataEnqueue(unsigned char* dataBuf, unsigned short dataLength)
{
	if ((dataBuf == NULL) || (dataLength > COAP_SEND_BUFFER_SIZE)) {
		return;
	}
	
	if (NET_MqttSN_Message_SendDataisFull() == true) {												//队列已满
		NETMqttSNMessageSendPark.Rear = (NETMqttSNMessageSendPark.Rear + 1) % MQTTSN_SEND_PARK_NUM;			//队尾偏移1
		memset((u8 *)&NETMqttSNMessageSendPark.Park[NETMqttSNMessageSendPark.Rear], 0x0, sizeof(NETMqttSNMessageSendPark.Park[NETMqttSNMessageSendPark.Rear]));
		memcpy(NETMqttSNMessageSendPark.Park[NETMqttSNMessageSendPark.Rear].Buffer, dataBuf, dataLength);
		NETMqttSNMessageSendPark.Park[NETMqttSNMessageSendPark.Rear].Length = dataLength;
		NETMqttSNMessageSendPark.Front = (NETMqttSNMessageSendPark.Front + 1) % MQTTSN_SEND_PARK_NUM;			//队头偏移1
	}
	else {																					//队列未满
		NETMqttSNMessageSendPark.Rear = (NETMqttSNMessageSendPark.Rear + 1) % MQTTSN_SEND_PARK_NUM;			//队尾偏移1
		memset((u8 *)&NETMqttSNMessageSendPark.Park[NETMqttSNMessageSendPark.Rear], 0x0, sizeof(NETMqttSNMessageSendPark.Park[NETMqttSNMessageSendPark.Rear]));
		memcpy(NETMqttSNMessageSendPark.Park[NETMqttSNMessageSendPark.Rear].Buffer, dataBuf, dataLength);
		NETMqttSNMessageSendPark.Park[NETMqttSNMessageSendPark.Rear].Length = dataLength;
	}
}

/**********************************************************************************************************
 @Function			bool NET_MqttSN_Message_SendDataDequeue(unsigned char* dataBuf, unsigned short* dataLength)
 @Description			NET_MqttSN_Message_SendDataDequeue	: 发送数据读出队列
 @Input				dataBuf	 		 			: 需读出数据地址
					dataLength					: 需读出数据长度地址
 @Return				true							: 未空
					false						: 已空
**********************************************************************************************************/
bool NET_MqttSN_Message_SendDataDequeue(unsigned char* dataBuf, unsigned short* dataLength)
{
	bool MessageState;
	unsigned char front;
	
	if (NET_MqttSN_Message_SendDataisEmpty() == true) {												//队列已空
		MessageState = false;
	}
	else {																					//队列未空
		front = (NETMqttSNMessageSendPark.Front + 1) % MQTTSN_SEND_PARK_NUM;								//队头偏移1
		memcpy(dataBuf, NETMqttSNMessageSendPark.Park[front].Buffer, NETMqttSNMessageSendPark.Park[front].Length);
		*dataLength = NETMqttSNMessageSendPark.Park[front].Length;
		MessageState = true;
	}
	
	return MessageState;
}

/**********************************************************************************************************
 @Function			bool NET_MqttSN_Message_SendDataOffSet(void)
 @Description			NET_MqttSN_Message_SendDataOffSet	: 发送数据队列(队列头偏移1)
 @Input				void
 @Return				true							: 未空
					false						: 已空
**********************************************************************************************************/
bool NET_MqttSN_Message_SendDataOffSet(void)
{
	bool MessageState;
	
	if (NET_MqttSN_Message_SendDataisEmpty() == true) {												//队列已空
		MessageState = false;
	}
	else {																					//队列未空
		NETMqttSNMessageSendPark.Front = (NETMqttSNMessageSendPark.Front + 1) % MQTTSN_SEND_PARK_NUM;
		MessageState = true;
	}
	
	return MessageState;
}

/**********************************************************************************************************
 @Function			unsigned char NET_MqttSN_Message_SendDataRear(void)
 @Description			NET_MqttSN_Message_SendDataRear	: 发送数据队尾值
 @Input				void
 @Return				发送数据队尾值
**********************************************************************************************************/
unsigned char NET_MqttSN_Message_SendDataRear(void)
{
	return NETMqttSNMessageSendPark.Rear;
}

#endif

/********************************************** END OF FLEE **********************************************/
