/**
  *********************************************************************************************************
  * @file    net_coap_message_operate.c
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

#include "net_coap_message_operate.h"
#include "platform_config.h"
#include "platform_map.h"
#include "stm32l1xx_config.h"
#include "radar_api.h"
#include "tmesh_algorithm.h"
#include "string.h"

COAP_SwapSendDataTypeDef		NETCoapMessageSendPark;
COAP_SwapRecvDataTypeDef		NETCoapMessageRecvPark;

/**********************************************************************************************************
 @Function			int NET_COAP_Message_Operate_Creat_Json_Work_Info(char* outBuffer)
 @Description			NET_COAP_Message_Operate_Creat_Json_Work_Info
 @Input				outBuffer
 @Return				Length
 @attention			!!<<< MaxLength 240Byte >>>!!
**********************************************************************************************************/
int NET_COAP_Message_Operate_Creat_Json_Work_Info(char* outBuffer)
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
 @Function			int NET_COAP_Message_Operate_Creat_Json_Basic_Info(char* outBuffer)
 @Description			NET_COAP_Message_Operate_Creat_Json_Basic_Info
 @Input				outBuffer
 @Return				Length
 @attention			!!<<< MaxLength 240Byte >>>!!
**********************************************************************************************************/
int NET_COAP_Message_Operate_Creat_Json_Basic_Info(char* outBuffer)
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
 @Function			int NET_COAP_Message_Operate_Creat_Json_Dynamic_Info(char* outBuffer)
 @Description			NET_COAP_Message_Operate_Creat_Json_Dynamic_Info
 @Input				outBuffer
 @Return				Length
 @attention			!!<<< MaxLength 240Byte >>>!!
**********************************************************************************************************/
int NET_COAP_Message_Operate_Creat_Json_Dynamic_Info(char* outBuffer)
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
 @Function			int NET_COAP_Message_Operate_Creat_Json_Radar_Info(char* outBuffer)
 @Description			NET_COAP_Message_Operate_Creat_Json_Radar_Info
 @Input				outBuffer
 @Return				Length
 @attention			!!<<< MaxLength 240Byte >>>!!
**********************************************************************************************************/
int NET_COAP_Message_Operate_Creat_Json_Radar_Info(char* outBuffer)
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
 @Function			int NET_COAP_Message_Operate_Creat_Json_Response_Info(char* outBuffer, u16 errcode)
 @Description			NET_COAP_Message_Operate_Creat_Json_Response_Info
 @Input				outBuffer
					errcode
 @Return				Length
 @attention			!!<<< MaxLength 240Byte >>>!!
**********************************************************************************************************/
int NET_COAP_Message_Operate_Creat_Json_Response_Info(char* outBuffer, u16 errcode)
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

/**********************************************************************************************************
 @Function			static bool NET_Coap_Message_SendDataisFull(void)
 @Description			NET_Coap_Message_SendDataisFull	: 检查发送队列是否已满
 @Input				void
 @Return				true							: 已满
					false						: 未满
**********************************************************************************************************/
static bool NET_Coap_Message_SendDataisFull(void)
{
	bool MessageState;
	
	if ((NETCoapMessageSendPark.Rear + 1) % COAP_SEND_PARK_NUM == NETCoapMessageSendPark.Front) {
		MessageState = true;
	}
	else {
		MessageState = false;
	}
	
	return MessageState;
}

/**********************************************************************************************************
 @Function			static bool NET_Coap_Message_RecvDataisFull(void)
 @Description			NET_Coap_Message_SendDataisFull	: 检查接收队列是否已满
 @Input				void
 @Return				true							: 已满
					false						: 未满
**********************************************************************************************************/
static bool NET_Coap_Message_RecvDataisFull(void)
{
	bool MessageState;
	
	if ((NETCoapMessageRecvPark.Rear + 1) % COAP_RECV_PARK_NUM == NETCoapMessageRecvPark.Front) {
		MessageState = true;
	}
	else {
		MessageState = false;
	}
	
	return MessageState;
}

/**********************************************************************************************************
 @Function			static bool NET_Coap_Message_SendDataisEmpty(void)
 @Description			NET_Coap_Message_SendDataisEmpty	: 检查发送队列是否已空
 @Input				void
 @Return				true							: 已空
					false						: 未空
**********************************************************************************************************/
static bool NET_Coap_Message_SendDataisEmpty(void)
{
	bool MessageState;
	
	if (NETCoapMessageSendPark.Front == NETCoapMessageSendPark.Rear) {
		MessageState = true;
	}
	else {
		MessageState = false;
	}
	
	return MessageState;
}

/**********************************************************************************************************
 @Function			static bool NET_Coap_Message_RecvDataisEmpty(void)
 @Description			NET_Coap_Message_SendDataisEmpty	: 检查接收队列是否已空
 @Input				void
 @Return				true							: 已空
					false						: 未空
**********************************************************************************************************/
static bool NET_Coap_Message_RecvDataisEmpty(void)
{
	bool MessageState;
	
	if (NETCoapMessageRecvPark.Front == NETCoapMessageRecvPark.Rear) {
		MessageState = true;
	}
	else {
		MessageState = false;
	}
	
	return MessageState;
}

/**********************************************************************************************************
 @Function			void NET_Coap_Message_SendDataEnqueue(unsigned char* dataBuf, unsigned short dataLength)
 @Description			NET_Coap_Message_SendDataEnqueue	: 发送数据写入队列
 @Input				dataBuf	 		 			: 需写入数据
					dataLength					: 需写入数据长度
 @Return				void
**********************************************************************************************************/
void NET_Coap_Message_SendDataEnqueue(unsigned char* dataBuf, unsigned short dataLength)
{
	if ((dataBuf == NULL) || (dataLength > COAP_SEND_BUFFER_SIZE)) {
		return;
	}
	
	if (NET_Coap_Message_SendDataisFull() == true) {													//队列已满
		NETCoapMessageSendPark.Rear = (NETCoapMessageSendPark.Rear + 1) % COAP_SEND_PARK_NUM;				//队尾偏移1
		memset((u8 *)&NETCoapMessageSendPark.Park[NETCoapMessageSendPark.Rear], 0x0, sizeof(NETCoapMessageSendPark.Park[NETCoapMessageSendPark.Rear]));
		memcpy(NETCoapMessageSendPark.Park[NETCoapMessageSendPark.Rear].Buffer, dataBuf, dataLength);
		NETCoapMessageSendPark.Park[NETCoapMessageSendPark.Rear].Length = dataLength;
		NETCoapMessageSendPark.Front = (NETCoapMessageSendPark.Front + 1) % COAP_SEND_PARK_NUM;				//队头偏移1
	}
	else {																					//队列未满
		NETCoapMessageSendPark.Rear = (NETCoapMessageSendPark.Rear + 1) % COAP_SEND_PARK_NUM;				//队尾偏移1
		memset((u8 *)&NETCoapMessageSendPark.Park[NETCoapMessageSendPark.Rear], 0x0, sizeof(NETCoapMessageSendPark.Park[NETCoapMessageSendPark.Rear]));
		memcpy(NETCoapMessageSendPark.Park[NETCoapMessageSendPark.Rear].Buffer, dataBuf, dataLength);
		NETCoapMessageSendPark.Park[NETCoapMessageSendPark.Rear].Length = dataLength;
	}
}

/**********************************************************************************************************
 @Function			void NET_Coap_Message_RecvDataEnqueue(unsigned char* dataBuf, unsigned short dataLength)
 @Description			NET_Coap_Message_RecvDataEnqueue	: 接收数据写入队列
 @Input				dataBuf	 		 			: 需写入数据
					dataLength					: 需写入数据长度
 @Return				void
**********************************************************************************************************/
void NET_Coap_Message_RecvDataEnqueue(unsigned char* dataBuf, unsigned short dataLength)
{
	if ((dataBuf == NULL) || (dataLength > COAP_RECV_BUFFER_SIZE)) {
		return;
	}
	
	if (NET_Coap_Message_RecvDataisFull() == true) {													//队列已满
		NETCoapMessageRecvPark.Rear = (NETCoapMessageRecvPark.Rear + 1) % COAP_RECV_PARK_NUM;				//队尾偏移1
		memset((u8 *)&NETCoapMessageRecvPark.Park[NETCoapMessageRecvPark.Rear], 0x0, sizeof(NETCoapMessageRecvPark.Park[NETCoapMessageRecvPark.Rear]));
		memcpy(NETCoapMessageRecvPark.Park[NETCoapMessageRecvPark.Rear].Buffer, dataBuf, dataLength);
		NETCoapMessageRecvPark.Park[NETCoapMessageRecvPark.Rear].Length = dataLength;
		NETCoapMessageRecvPark.Front = (NETCoapMessageRecvPark.Front + 1) % COAP_RECV_PARK_NUM;				//队头偏移1
	}
	else {																					//队列未满
		NETCoapMessageRecvPark.Rear = (NETCoapMessageRecvPark.Rear + 1) % COAP_RECV_PARK_NUM;				//队尾偏移1
		memset((u8 *)&NETCoapMessageRecvPark.Park[NETCoapMessageRecvPark.Rear], 0x0, sizeof(NETCoapMessageRecvPark.Park[NETCoapMessageRecvPark.Rear]));
		memcpy(NETCoapMessageRecvPark.Park[NETCoapMessageRecvPark.Rear].Buffer, dataBuf, dataLength);
		NETCoapMessageRecvPark.Park[NETCoapMessageRecvPark.Rear].Length = dataLength;
	}
}

/**********************************************************************************************************
 @Function			bool NET_Coap_Message_SendDataDequeue(unsigned char* dataBuf, unsigned short* dataLength)
 @Description			NET_Coap_Message_SendDataDequeue	: 发送数据读出队列
 @Input				dataBuf	 		 			: 需读出数据地址
					dataLength					: 需读出数据长度地址
 @Return				true							: 未空
					false						: 已空
**********************************************************************************************************/
bool NET_Coap_Message_SendDataDequeue(unsigned char* dataBuf, unsigned short* dataLength)
{
	bool MessageState;
	unsigned char front;
	
	if (NET_Coap_Message_SendDataisEmpty() == true) {													//队列已空
		MessageState = false;
	}
	else {																					//队列未空
		front = (NETCoapMessageSendPark.Front + 1) % COAP_SEND_PARK_NUM;									//队头偏移1
		memcpy(dataBuf, NETCoapMessageSendPark.Park[front].Buffer, NETCoapMessageSendPark.Park[front].Length);
		*dataLength = NETCoapMessageSendPark.Park[front].Length;
		MessageState = true;
	}
	
	return MessageState;
}

/**********************************************************************************************************
 @Function			bool NET_Coap_Message_RecvDataDequeue(unsigned char* dataBuf, unsigned short* dataLength)
 @Description			NET_Coap_Message_RecvDataDequeue	: 接收数据读出队列
 @Input				dataBuf	 		 			: 需读出数据地址
					dataLength					: 需读出数据长度地址
 @Return				true							: 未空
					false						: 已空
**********************************************************************************************************/
bool NET_Coap_Message_RecvDataDequeue(unsigned char* dataBuf, unsigned short* dataLength)
{
	bool MessageState;
	unsigned char front;
	
	if (NET_Coap_Message_RecvDataisEmpty() == true) {													//队列已空
		MessageState = false;
	}
	else {																					//队列未空
		front = (NETCoapMessageRecvPark.Front + 1) % COAP_RECV_PARK_NUM;									//队头偏移1
		memcpy(dataBuf, NETCoapMessageRecvPark.Park[front].Buffer, NETCoapMessageRecvPark.Park[front].Length);
		*dataLength = NETCoapMessageRecvPark.Park[front].Length;
		MessageState = true;
	}
	
	return MessageState;
}

/**********************************************************************************************************
 @Function			bool NET_Coap_Message_SendDataOffSet(void)
 @Description			NET_Coap_Message_SendDataOffSet	: 发送数据队列(队列头偏移1)
 @Input				void
 @Return				true							: 未空
					false						: 已空
**********************************************************************************************************/
bool NET_Coap_Message_SendDataOffSet(void)
{
	bool MessageState;
	
	if (NET_Coap_Message_SendDataisEmpty() == true) {													//队列已空
		MessageState = false;
	}
	else {																					//队列未空
		NETCoapMessageSendPark.Front = (NETCoapMessageSendPark.Front + 1) % COAP_SEND_PARK_NUM;
		MessageState = true;
	}
	
	return MessageState;
}

/**********************************************************************************************************
 @Function			bool NET_Coap_Message_RecvDataOffSet(void)
 @Description			NET_Coap_Message_RecvDataOffSet	: 接收数据队列(队列头偏移1)
 @Input				void
 @Return				true							: 未空
					false						: 已空
**********************************************************************************************************/
bool NET_Coap_Message_RecvDataOffSet(void)
{
	bool MessageState;
	
	if (NET_Coap_Message_RecvDataisEmpty() == true) {													//队列已空
		MessageState = false;
	}
	else {																					//队列未空
		NETCoapMessageRecvPark.Front = (NETCoapMessageRecvPark.Front + 1) % COAP_RECV_PARK_NUM;
		MessageState = true;
	}
	
	return MessageState;
}

/**********************************************************************************************************
 @Function			unsigned char NET_Coap_Message_SendDataRear(void)
 @Description			NET_Coap_Message_SendDataRear		: 发送数据队尾值
 @Input				void
 @Return				发送数据队尾值
**********************************************************************************************************/
unsigned char NET_Coap_Message_SendDataRear(void)
{
	return NETCoapMessageSendPark.Rear;
}

/**********************************************************************************************************
 @Function			unsigned char NET_Coap_Message_RecvDataRear(void)
 @Description			NET_Coap_Message_SendDataRear		: 接收数据队尾值
 @Input				void
 @Return				接收数据队尾值
**********************************************************************************************************/
unsigned char NET_Coap_Message_RecvDataRear(void)
{
	return NETCoapMessageRecvPark.Rear;
}

/********************************************** END OF FLEE **********************************************/
