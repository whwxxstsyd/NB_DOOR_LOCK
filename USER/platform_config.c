/**
  *********************************************************************************************************
  * @file    platform_config.c
  * @author  Movebroad -- KK
  * @version V1.0
  * @date    2017-10-26
  * @brief   
  *********************************************************************************************************
  * @attention
  *			
  *
  *
  *********************************************************************************************************
  */

#include "platform_config.h"
#include "platform_map.h"
#include "hal_spiflash.h"
#include "radio_rf_app.h"
#include "radio_hal_rf.h"
#include "pcpupgrade.h"

bool BootUp				= true;										//BootUp

/* Work Mode */
bool DeviceIdleMode		= false;										//IDLE MODE
bool DeviceActivedMode		= true;										//ACTIVED MODE

/* Coap Packet */
COAP_PacketShortTypeDef		CoapShortStructure;								//Coap Short Packet
COAP_PacketLongTypeDef		CoapLongStructure;								//Coap Long Packet
COAP_PacketInfoTypeDef		CoapInfoStructure;								//Coap Info Packet

/* MqttSN Packet */
#if MQTTSN_MSG_VERSION_STREAM_TYPE == MQTTSN_MSG_VERSION_JSON_STREAM
MQTTSN_StatusBasicTypeDef	MqttSNStatusBasicStructure;						//MqttSN Basic Status Packet
MQTTSN_StatusExtendTypeDef	MqttSNStatusExtendStructure;						//MqttSN Extend Status Packet
MQTTSN_InfoWorkTypeDef		MqttSNInfoWorkStructure;							//MqttSN Work Info Packet
MQTTSN_InfoBasicTypeDef		MqttSNInfoBasicStructure;						//MqttSN Basic Info Packet
MQTTSN_InfoDynamicTypeDef	MqttSNInfoDynamicStructure;						//MqttSN Dynamic Info Packet
MQTTSN_InfoRadarTypeDef		MqttSNInfoRadarStructure;						//MqttSN Radar Info Packet
MQTTSN_InfoResponseTypeDef	MqttSNInfoResponseStructure;						//MqttSN Response Info Packet
#endif
#if MQTTSN_MSG_VERSION_STREAM_TYPE == MQTTSN_MSG_VERSION_BYTE_STREAM
MQTTSN_PacketShortTypeDef	MqttSNShortStructure;							//MqttSN Short Packet
MQTTSN_PacketLongTypeDef		MqttSNLongStructure;							//MqttSN Long Packet
MQTTSN_PacketInfoTypeDef		MqttSNInfoStructure;							//MqttSN Info Packet
#endif

/* NET Handler */		
NET_NBIOT_ClientsTypeDef	NetNbiotClientHandler;							//NET NBIOT Clinet Handler
NBIOT_ATCmdTypeDef			NbiotATCmdHandler;								//NBIOT AT Cmd Handler
NBIOT_ClientsTypeDef		NbiotClientHandler;							//Coap Clinet Handler
PCP_CoAPNetTransportTypeDef	PCPCoAPNetHandler;								//PCP Net Handler
PCP_ClientsTypeDef			PCPClientHandler;								//PCP Clinet Handler
DNS_SocketNetTypeDef		DNSSocketNetHandler;							//DNS Net Handler
DNS_ClientsTypeDef			DNSClientHandler;								//DNS Clinet Handler
MQTTSN_SocketNetTypeDef		MqttSNSocketNetHandler;						//MqttSN Net Handler
MQTTSN_ClientsTypeDef		MqttSNClientHandler;							//MqttSN Clinet Handler
ONENET_LWM2MTransportTypeDef	OneNETLWM2MNetHandler;						//ONENET Net Handler
ONENET_ClientsTypeDef		OneNETClientHandler;							//ONENET Clinet Handler

/**********************************************************************************************************
 @Function			void RadioPrintWorkinfo(void)
 @Description			RadioPrintWorkinfo
 @Input				void
 @Return				void
**********************************************************************************************************/
void RadioPrintWorkinfo(void)
{
#if NBCOAP_SENDCODE_WORK_INFO
	NETCoapNeedSendCode.WorkInfo = 1;
#endif
#if NBMQTTSN_SENDCODE_WORK_INFO
	NETMqttSNNeedSendCode.InfoWork = 1;
#endif
	
	TCFG_EEPROM_GetMagTempCoef(&TCFG_SystemData.MagCoefX, &TCFG_SystemData.MagCoefY, &TCFG_SystemData.MagCoefZ);
	
	GPRS_DEBUG(DEBUG_INFO, "WorkInfo:");
	
#if NETPROTOCAL == NETCOAP
	GPRS_DEBUG(DEBUG_INFO, "Soft:%d:%d.%d", TCFG_EEPROM_GetBootVersion(), TCFG_Utility_Get_Major_Softnumber(), TCFG_Utility_Get_Sub_Softnumber());
	GPRS_DEBUG(DEBUG_INFO, "Sense:%d", TCFG_EEPROM_GetSavedSensitivity());
	GPRS_DEBUG(DEBUG_INFO, "Mode:%s", TCFG_EEPROM_Get_WorkMode_String());
	GPRS_DEBUG(DEBUG_INFO, "Channel:%d", TCFG_EEPROM_GetRfChannel());
	GPRS_DEBUG(DEBUG_INFO, "Range:%d", TCFG_Utility_Get_DistanceRange());
	GPRS_DEBUG(DEBUG_INFO, "Earfcn:%d", TCFG_Utility_Get_Nbiot_RadioEARFCN());
	GPRS_DEBUG(DEBUG_INFO, "CellId:%d", TCFG_Utility_Get_Nbiot_RadioCellID());
	GPRS_DEBUG(DEBUG_INFO, "Cmdcnt:%d.%d", TCFG_EEPROM_GetRFCmdCnt(), TCFG_EEPROM_GetNBCmdCnt());
	GPRS_DEBUG(DEBUG_INFO, "Nbworkmode:%d", TCFG_Utility_Get_Nbiot_WorkMode());
	GPRS_DEBUG(DEBUG_INFO, "Nbruntime:%d.%d", TCFG_Utility_GetCoapConnectTime(), TCFG_Utility_GetCoapIdleTime());
	GPRS_DEBUG(DEBUG_INFO, "NbruntimeDay:%d.%d", TCFG_Utility_GetCoapConnectDayTime(), TCFG_Utility_GetCoapIdleDayTime());
	GPRS_DEBUG(DEBUG_INFO, "Coef:%d.%d.%d", TCFG_SystemData.MagCoefX, TCFG_SystemData.MagCoefY, TCFG_SystemData.MagCoefZ);
#elif NETPROTOCAL == NETMQTTSN
	GPRS_DEBUG(DEBUG_INFO, "Soft:%d:%d.%d", TCFG_EEPROM_GetBootVersion(), TCFG_Utility_Get_Major_Softnumber(), TCFG_Utility_Get_Sub_Softnumber());
	GPRS_DEBUG(DEBUG_INFO, "Sense:%d", TCFG_EEPROM_GetSavedSensitivity());
	GPRS_DEBUG(DEBUG_INFO, "Mode:%s", TCFG_EEPROM_Get_WorkMode_String());
	GPRS_DEBUG(DEBUG_INFO, "Channel:%d", TCFG_EEPROM_GetRfChannel());
	GPRS_DEBUG(DEBUG_INFO, "Range:%d", TCFG_Utility_Get_DistanceRange());
	GPRS_DEBUG(DEBUG_INFO, "Earfcn:%d", TCFG_Utility_Get_Nbiot_RadioEARFCN());
	GPRS_DEBUG(DEBUG_INFO, "CellId:%d", TCFG_Utility_Get_Nbiot_RadioCellID());
	GPRS_DEBUG(DEBUG_INFO, "Cmdcnt:%d.%d", TCFG_EEPROM_GetRFCmdCnt(), TCFG_EEPROM_GetNBCmdCnt());
	GPRS_DEBUG(DEBUG_INFO, "Coef:%d.%d.%d", TCFG_SystemData.MagCoefX, TCFG_SystemData.MagCoefY, TCFG_SystemData.MagCoefZ);
#endif
}

/**********************************************************************************************************
 @Function			void RadioPrintNetinfo(void)
 @Description			RadioPrintNetinfo
 @Input				void
 @Return				void
**********************************************************************************************************/
void RadioPrintNetinfo(void)
{
#if NBCOAP_SENDCODE_BASIC_INFO
	NETCoapNeedSendCode.BasicInfo = 1;
#endif
#if NBCOAP_SENDCODE_DYNAMIC_INFO
	NETCoapNeedSendCode.DynamicInfo = 1;
#endif
#if NBMQTTSN_SENDCODE_BASIC_INFO
	NETMqttSNNeedSendCode.InfoBasic = 1;
#endif
#if NBMQTTSN_SENDCODE_DYNAMIC_INFO
	NETMqttSNNeedSendCode.InfoDynamic = 1;
#endif
	
	GPRS_DEBUG(DEBUG_INFO, "NetInfo:");
	
#if NETPROTOCAL == NETCOAP
	GPRS_DEBUG(DEBUG_INFO, "Manufacturer:%s", NbiotClientHandler.Parameter.manufacturer);
	GPRS_DEBUG(DEBUG_INFO, "ManufacturerMode:%s", NbiotClientHandler.Parameter.manufacturermode);
	GPRS_DEBUG(DEBUG_INFO, "MduVer:%s", NbiotClientHandler.Parameter.modelversion);
	GPRS_DEBUG(DEBUG_INFO, "IMEI:%s", NbiotClientHandler.Parameter.imei);
	GPRS_DEBUG(DEBUG_INFO, "ICCID:%s", NbiotClientHandler.Parameter.iccid);
	GPRS_DEBUG(DEBUG_INFO, "IMSI:%s", NbiotClientHandler.Parameter.imsi);
	GPRS_DEBUG(DEBUG_INFO, "CGP:%s", NbiotClientHandler.Parameter.cgpaddr);
	GPRS_DEBUG(DEBUG_INFO, "CGD:%s", NbiotClientHandler.Parameter.cgdcont);
	GPRS_DEBUG(DEBUG_INFO, "RSSI:%d", NbiotClientHandler.Parameter.rssi);
	GPRS_DEBUG(DEBUG_INFO, "SNR:%d", NbiotClientHandler.Parameter.statisticsRADIO.SNR);
	GPRS_DEBUG(DEBUG_INFO, "CDPHost:%s", NbiotClientHandler.Parameter.cdpserver.CDPServerHost);
	GPRS_DEBUG(DEBUG_INFO, "CDPPort:%d", NbiotClientHandler.Parameter.cdpserver.CDPServerPort);
#elif NETPROTOCAL == NETMQTTSN
	GPRS_DEBUG(DEBUG_INFO, "Manufacturer:%s", MqttSNClientHandler.SocketStack->NBIotStack->Parameter.manufacturer);
	GPRS_DEBUG(DEBUG_INFO, "ManufacturerMode:%s", MqttSNClientHandler.SocketStack->NBIotStack->Parameter.manufacturermode);
	GPRS_DEBUG(DEBUG_INFO, "MduVer:%s", MqttSNClientHandler.SocketStack->NBIotStack->Parameter.modelversion);
	GPRS_DEBUG(DEBUG_INFO, "IMEI:%s", MqttSNClientHandler.SocketStack->NBIotStack->Parameter.imei);
	GPRS_DEBUG(DEBUG_INFO, "ICCID:%s", MqttSNClientHandler.SocketStack->NBIotStack->Parameter.iccid);
	GPRS_DEBUG(DEBUG_INFO, "IMSI:%s", MqttSNClientHandler.SocketStack->NBIotStack->Parameter.imsi);
	GPRS_DEBUG(DEBUG_INFO, "CGP:%s", MqttSNClientHandler.SocketStack->NBIotStack->Parameter.cgpaddr);
	GPRS_DEBUG(DEBUG_INFO, "CGD:%s", MqttSNClientHandler.SocketStack->NBIotStack->Parameter.cgdcont);
	GPRS_DEBUG(DEBUG_INFO, "RSSI:%d", MqttSNClientHandler.SocketStack->NBIotStack->Parameter.rssi);
	GPRS_DEBUG(DEBUG_INFO, "SNR:%d", MqttSNClientHandler.SocketStack->NBIotStack->Parameter.statisticsRADIO.SNR);
	GPRS_DEBUG(DEBUG_INFO, "MqttSN :%s", MQTTSN_SERVER_HOST_NAME);
	GPRS_DEBUG(DEBUG_INFO, "MqttSN :%s:%d", MqttSNClientHandler.SocketStack->ServerHost, MqttSNClientHandler.SocketStack->ServerPort);
#endif
}

/**********************************************************************************************************
 @Function			void RadioPrintDeviceinfo(void)
 @Description			RadioPrintDeviceinfo
 @Input				void
 @Return				void
**********************************************************************************************************/
void RadioPrintDeviceinfo(void)
{
	GPRS_DEBUG(DEBUG_INFO, "DeviceInfo:");
	
	GPRS_DEBUG(DEBUG_INFO, "Runtime:%d", TCFG_Utility_Get_Run_Time());
	GPRS_DEBUG(DEBUG_INFO, "Batt:%d", TCFG_Utility_Get_Device_Batt_ShortVal());
	GPRS_DEBUG(DEBUG_INFO, "RadarLib:%d", TCFG_Utility_Get_RadarLibNum());
	GPRS_DEBUG(DEBUG_INFO, "RadarCnt:%d", TCFG_GetRadarCount());
	GPRS_DEBUG(DEBUG_INFO, "Temperature:%d", TCFG_Utility_Get_Device_Temperature());
	GPRS_DEBUG(DEBUG_INFO, "RadarDbgMode:%d", TCFG_EEPROM_GetRadarDbgMode());
	GPRS_DEBUG(DEBUG_INFO, "AlgoLib:%d", TCFG_Utility_Get_AlgoLibNum());
	GPRS_DEBUG(DEBUG_INFO, "QmcReboot:%d", TCFG_Utility_Get_ReInitModuleCount());
	GPRS_DEBUG(DEBUG_INFO, "Nbboot:%d", TCFG_Utility_Get_Nbiot_BootCount());
	GPRS_DEBUG(DEBUG_INFO, "Nbsent:%d", TCFG_Utility_Get_Nbiot_SentCount());
	GPRS_DEBUG(DEBUG_INFO, "Nbrecv:%d", TCFG_Utility_Get_Nbiot_RecvCount());
}

/**********************************************************************************************************
 @Function			void RadioPrintUpgradeinfo(void)
 @Description			RadioPrintUpgradeinfo
 @Input				void
 @Return				void
**********************************************************************************************************/
void RadioPrintUpgradeinfo(void)
{
	GPRS_DEBUG(DEBUG_INFO, "UpgradeInfo:");
	
#ifdef GD25Q_80CSIG
	PCP_APPInfoTypeDef APP1Info;
	PCP_APPInfoTypeDef APP2Info;
	
	if (GD25Q80CSIG_OK != GD25Q_SPIFLASH_Get_Status()) {
		GPRS_DEBUG(DEBUG_INFO, "SPI FLASH Fail");
		return;
	}
	
	Radio_Rf_Interrupt_Deinit();
	GD25Q_SPIFLASH_WakeUp();
	GD25Q_SPIFLASH_Init();
	
	APP1Info.Status		= GD25Q_SPIFLASH_GetByte(APP1_INFO_UPGRADE_STATUS_OFFSET);
	APP1Info.BaseAddr	= GD25Q_SPIFLASH_GetWord(APP1_INFO_UPGRADE_BASEADDR_OFFSET);
	APP1Info.BlockNum	= GD25Q_SPIFLASH_GetHalfWord(APP1_INFO_UPGRADE_BLOCKNUM_OFFSET);
	APP1Info.BlockLen	= GD25Q_SPIFLASH_GetHalfWord(APP1_INFO_UPGRADE_BLOCKLEN_OFFSET);
	APP1Info.DataLen	= GD25Q_SPIFLASH_GetHalfWord(APP1_INFO_UPGRADE_DATALEN_OFFSET);
	APP1Info.SoftVer	= GD25Q_SPIFLASH_GetWord(APP1_INFO_UPGRADE_SOFTVER_OFFSET);
	APP1Info.CheckCode	= GD25Q_SPIFLASH_GetWord(APP1_DATA_CHECK_CODE_OFFSET);
	
	APP2Info.Status	= GD25Q_SPIFLASH_GetByte(APP2_INFO_UPGRADE_STATUS_OFFSET);
	APP2Info.BaseAddr	= GD25Q_SPIFLASH_GetWord(APP2_INFO_UPGRADE_BASEADDR_OFFSET);
	APP2Info.BlockNum	= GD25Q_SPIFLASH_GetHalfWord(APP2_INFO_UPGRADE_BLOCKNUM_OFFSET);
	APP2Info.BlockLen	= GD25Q_SPIFLASH_GetHalfWord(APP2_INFO_UPGRADE_BLOCKLEN_OFFSET);
	APP2Info.DataLen	= GD25Q_SPIFLASH_GetHalfWord(APP2_INFO_UPGRADE_DATALEN_OFFSET);
	APP2Info.SoftVer	= GD25Q_SPIFLASH_GetWord(APP2_INFO_UPGRADE_SOFTVER_OFFSET);
	APP2Info.CheckCode	= GD25Q_SPIFLASH_GetWord(APP2_DATA_CHECK_CODE_OFFSET);
	
	GD25Q_SPIFLASH_PowerDown();
	
	GPRS_DEBUG(DEBUG_INFO, "APP1 Status: %02X", APP1Info.Status);
	GPRS_DEBUG(DEBUG_INFO, "APP1 BaseAddr: %08X", APP1Info.BaseAddr);
	GPRS_DEBUG(DEBUG_INFO, "APP1 BlockNum: %d", APP1Info.BlockNum);
	GPRS_DEBUG(DEBUG_INFO, "APP1 BlockLen: %d", APP1Info.BlockLen);
	GPRS_DEBUG(DEBUG_INFO, "APP1 DataLen: %d", APP1Info.DataLen);
	GPRS_DEBUG(DEBUG_INFO, "APP1 SoftVer: V%d.%d", APP1Info.SoftVer>>16, APP1Info.SoftVer&0xFFFF);
	GPRS_DEBUG(DEBUG_INFO, "APP1 CheckCode: %X", APP1Info.CheckCode);
	GPRS_DEBUG(DEBUG_INFO, "APP2 Status: %02X", APP2Info.Status);
	GPRS_DEBUG(DEBUG_INFO, "APP2 BaseAddr: %08X", APP2Info.BaseAddr);
	GPRS_DEBUG(DEBUG_INFO, "APP2 BlockNum: %d", APP2Info.BlockNum);
	GPRS_DEBUG(DEBUG_INFO, "APP2 BlockLen: %d", APP2Info.BlockLen);
	GPRS_DEBUG(DEBUG_INFO, "APP2 DataLen: %d", APP2Info.DataLen);
	GPRS_DEBUG(DEBUG_INFO, "APP2 SoftVer: V%d.%d", APP2Info.SoftVer>>16, APP2Info.SoftVer&0xFFFF);
	GPRS_DEBUG(DEBUG_INFO, "APP2 CheckCode: %X", APP2Info.CheckCode);
#else
	GPRS_DEBUG(DEBUG_INFO, "SPI FLASH Invalid");
#endif
}

/********************************************** END OF FLEE **********************************************/
