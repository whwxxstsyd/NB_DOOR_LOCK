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
bool DeviceIdleMode			= false;										//IDLE MODE
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
NET_NBIOT_ClientsTypeDef		NetNbiotClientHandler;							//NET NBIOT Clinet Handler
NBIOT_ATCmdTypeDef			NbiotATCmdHandler;								//NBIOT AT Cmd Handler
NBIOT_ClientsTypeDef		NbiotClientHandler;								//Coap Clinet Handler
PCP_CoAPNetTransportTypeDef	PCPCoAPNetHandler;								//PCP Net Handler
PCP_ClientsTypeDef			PCPClientHandler;								//PCP Clinet Handler
DNS_SocketNetTypeDef		DNSSocketNetHandler;							//DNS Net Handler
DNS_ClientsTypeDef			DNSClientHandler;								//DNS Clinet Handler
MQTTSN_SocketNetTypeDef		MqttSNSocketNetHandler;							//MqttSN Net Handler
MQTTSN_ClientsTypeDef		MqttSNClientHandler;							//MqttSN Clinet Handler
ONENET_LWM2MTransportTypeDef	OneNETLWM2MNetHandler;							//ONENET Net Handler
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
	
	Radio_Trf_Printf("WorkInfo:");
	
#if NETPROTOCAL == NETCOAP
	Radio_Trf_Printf("Soft:%d:%d.%d", TCFG_EEPROM_GetBootVersion(), TCFG_Utility_Get_Major_Softnumber(), TCFG_Utility_Get_Sub_Softnumber());
	Radio_Trf_Printf("Sense:%d", TCFG_EEPROM_GetSavedSensitivity());
	Radio_Trf_Printf("Mode:%s", TCFG_EEPROM_Get_WorkMode_String());
	Radio_Trf_Printf("Channel:%d", TCFG_EEPROM_GetRfChannel());
	Radio_Trf_Printf("Range:%d", TCFG_Utility_Get_DistanceRange());
	Radio_Trf_Printf("Earfcn:%d", TCFG_Utility_Get_Nbiot_RadioEARFCN());
	Radio_Trf_Printf("CellId:%d", TCFG_Utility_Get_Nbiot_RadioCellID());
	Radio_Trf_Printf("Cmdcnt:%d.%d", TCFG_EEPROM_GetRFCmdCnt(), TCFG_EEPROM_GetNBCmdCnt());
	Radio_Trf_Printf("Nbworkmode:%d", TCFG_Utility_Get_Nbiot_WorkMode());
	Radio_Trf_Printf("Nbruntime:%d.%d", TCFG_Utility_GetCoapConnectTime(), TCFG_Utility_GetCoapIdleTime());
	Radio_Trf_Printf("NbruntimeDay:%d.%d", TCFG_Utility_GetCoapConnectDayTime(), TCFG_Utility_GetCoapIdleDayTime());
	Radio_Trf_Printf("Coef:%d.%d.%d", TCFG_SystemData.MagCoefX, TCFG_SystemData.MagCoefY, TCFG_SystemData.MagCoefZ);
#elif NETPROTOCAL == NETMQTTSN
	Radio_Trf_Printf("Soft:%d:%d.%d", TCFG_EEPROM_GetBootVersion(), TCFG_Utility_Get_Major_Softnumber(), TCFG_Utility_Get_Sub_Softnumber());
	Radio_Trf_Printf("Sense:%d", TCFG_EEPROM_GetSavedSensitivity());
	Radio_Trf_Printf("Mode:%s", TCFG_EEPROM_Get_WorkMode_String());
	Radio_Trf_Printf("Channel:%d", TCFG_EEPROM_GetRfChannel());
	Radio_Trf_Printf("Range:%d", TCFG_Utility_Get_DistanceRange());
	Radio_Trf_Printf("Earfcn:%d", TCFG_Utility_Get_Nbiot_RadioEARFCN());
	Radio_Trf_Printf("CellId:%d", TCFG_Utility_Get_Nbiot_RadioCellID());
	Radio_Trf_Printf("Cmdcnt:%d.%d", TCFG_EEPROM_GetRFCmdCnt(), TCFG_EEPROM_GetNBCmdCnt());
	Radio_Trf_Printf("Coef:%d.%d.%d", TCFG_SystemData.MagCoefX, TCFG_SystemData.MagCoefY, TCFG_SystemData.MagCoefZ);
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
	
	Radio_Trf_Printf("NetInfo:");
	
#if NETPROTOCAL == NETCOAP
	Radio_Trf_Printf("Manufacturer:%s", NbiotClientHandler.Parameter.manufacturer);
	Radio_Trf_Printf("ManufacturerMode:%s", NbiotClientHandler.Parameter.manufacturermode);
	Radio_Trf_Printf("MduVer:%s", NbiotClientHandler.Parameter.modelversion);
	Radio_Trf_Printf("IMEI:%s", NbiotClientHandler.Parameter.imei);
	Radio_Trf_Printf("ICCID:%s", NbiotClientHandler.Parameter.iccid);
	Radio_Trf_Printf("IMSI:%s", NbiotClientHandler.Parameter.imsi);
	Radio_Trf_Printf("CGP:%s", NbiotClientHandler.Parameter.cgpaddr);
	Radio_Trf_Printf("CGD:%s", NbiotClientHandler.Parameter.cgdcont);
	Radio_Trf_Printf("RSSI:%d", NbiotClientHandler.Parameter.rssi);
	Radio_Trf_Printf("SNR:%d", NbiotClientHandler.Parameter.statisticsRADIO.SNR);
	Radio_Trf_Printf("CDPHost:%s", NbiotClientHandler.Parameter.cdpserver.CDPServerHost);
	Radio_Trf_Printf("CDPPort:%d", NbiotClientHandler.Parameter.cdpserver.CDPServerPort);
#elif NETPROTOCAL == NETMQTTSN
	Radio_Trf_Printf("Manufacturer:%s", MqttSNClientHandler.SocketStack->NBIotStack->Parameter.manufacturer);
	Radio_Trf_Printf("ManufacturerMode:%s", MqttSNClientHandler.SocketStack->NBIotStack->Parameter.manufacturermode);
	Radio_Trf_Printf("MduVer:%s", MqttSNClientHandler.SocketStack->NBIotStack->Parameter.modelversion);
	Radio_Trf_Printf("IMEI:%s", MqttSNClientHandler.SocketStack->NBIotStack->Parameter.imei);
	Radio_Trf_Printf("ICCID:%s", MqttSNClientHandler.SocketStack->NBIotStack->Parameter.iccid);
	Radio_Trf_Printf("IMSI:%s", MqttSNClientHandler.SocketStack->NBIotStack->Parameter.imsi);
	Radio_Trf_Printf("CGP:%s", MqttSNClientHandler.SocketStack->NBIotStack->Parameter.cgpaddr);
	Radio_Trf_Printf("CGD:%s", MqttSNClientHandler.SocketStack->NBIotStack->Parameter.cgdcont);
	Radio_Trf_Printf("RSSI:%d", MqttSNClientHandler.SocketStack->NBIotStack->Parameter.rssi);
	Radio_Trf_Printf("SNR:%d", MqttSNClientHandler.SocketStack->NBIotStack->Parameter.statisticsRADIO.SNR);
	Radio_Trf_Printf("MqttSN :%s", MQTTSN_SERVER_HOST_NAME);
	Radio_Trf_Printf("MqttSN :%s:%d", MqttSNClientHandler.SocketStack->ServerHost, MqttSNClientHandler.SocketStack->ServerPort);
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
	Radio_Trf_Printf("DeviceInfo:");
	
	Radio_Trf_Printf("Runtime:%d", TCFG_Utility_Get_Run_Time());
	Radio_Trf_Printf("Batt:%d", TCFG_Utility_Get_Device_Batt_ShortVal());
	Radio_Trf_Printf("RadarLib:%d", TCFG_Utility_Get_RadarLibNum());
	Radio_Trf_Printf("RadarCnt:%d", TCFG_GetRadarCount());
	Radio_Trf_Printf("Temperature:%d", TCFG_Utility_Get_Device_Temperature());
	Radio_Trf_Printf("RadarDbgMode:%d", TCFG_EEPROM_GetRadarDbgMode());
	Radio_Trf_Printf("AlgoLib:%d", TCFG_Utility_Get_AlgoLibNum());
	Radio_Trf_Printf("QmcReboot:%d", TCFG_Utility_Get_ReInitModuleCount());
	Radio_Trf_Printf("Nbboot:%d", TCFG_Utility_Get_Nbiot_BootCount());
	Radio_Trf_Printf("Nbsent:%d", TCFG_Utility_Get_Nbiot_SentCount());
	Radio_Trf_Printf("Nbrecv:%d", TCFG_Utility_Get_Nbiot_RecvCount());
}

/**********************************************************************************************************
 @Function			void RadioPrintUpgradeinfo(void)
 @Description			RadioPrintUpgradeinfo
 @Input				void
 @Return				void
**********************************************************************************************************/
void RadioPrintUpgradeinfo(void)
{
	Radio_Trf_Printf("UpgradeInfo:");
	
#ifdef GD25Q_80CSIG
	PCP_APPInfoTypeDef APP1Info;
	PCP_APPInfoTypeDef APP2Info;
	
	if (GD25Q80CSIG_OK != GD25Q_SPIFLASH_Get_Status()) {
		Radio_Trf_Printf("SPI FLASH Fail");
		return;
	}
	
	Radio_Rf_Interrupt_Deinit();
	GD25Q_SPIFLASH_WakeUp();
	GD25Q_SPIFLASH_Init();
	
	APP1Info.Status	= GD25Q_SPIFLASH_GetByte(APP1_INFO_UPGRADE_STATUS_OFFSET);
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
	
	Radio_Trf_Printf("APP1 Status: %02X", APP1Info.Status);
	Radio_Trf_Printf("APP1 BaseAddr: %08X", APP1Info.BaseAddr);
	Radio_Trf_Printf("APP1 BlockNum: %d", APP1Info.BlockNum);
	Radio_Trf_Printf("APP1 BlockLen: %d", APP1Info.BlockLen);
	Radio_Trf_Printf("APP1 DataLen: %d", APP1Info.DataLen);
	Radio_Trf_Printf("APP1 SoftVer: V%d.%d", APP1Info.SoftVer>>16, APP1Info.SoftVer&0xFFFF);
	Radio_Trf_Printf("APP1 CheckCode: %X", APP1Info.CheckCode);
	Radio_Trf_Printf("APP2 Status: %02X", APP2Info.Status);
	Radio_Trf_Printf("APP2 BaseAddr: %08X", APP2Info.BaseAddr);
	Radio_Trf_Printf("APP2 BlockNum: %d", APP2Info.BlockNum);
	Radio_Trf_Printf("APP2 BlockLen: %d", APP2Info.BlockLen);
	Radio_Trf_Printf("APP2 DataLen: %d", APP2Info.DataLen);
	Radio_Trf_Printf("APP2 SoftVer: V%d.%d", APP2Info.SoftVer>>16, APP2Info.SoftVer&0xFFFF);
	Radio_Trf_Printf("APP2 CheckCode: %X", APP2Info.CheckCode);
#else
	Radio_Trf_Printf("SPI FLASH Invalid");
#endif
}

/********************************************** END OF FLEE **********************************************/
