/**
  *********************************************************************************************************
  * @file    net_nbiot_app.c
  * @author  Movebroad -- KK
  * @version V1.0
  * @date    2018-04-26
  * @brief   
  *********************************************************************************************************
  * @attention
  *
  *
  *
  *********************************************************************************************************
  */

#include "net_nbiot_app.h"
#include "net_coap_app.h"
#include "net_pcp_app.h"
#include "net_dns_app.h"
#include "net_mqttsn_app.h"
#include "net_onenet_app.h"
#include "stm32l1xx_config.h"
#include "platform_config.h"
#include "platform_map.h"
#include "inspectmessageoperate.h"
#include "hal_rtc.h"
#include "radar_api.h"
#include "string.h"

NETCoapNeedSendCodeTypeDef	NETCoapNeedSendCode = NETCoapNeedSendCode_initializer;
NETMqttSNNeedSendCodeTypeDef	NETMqttSNNeedSendCode = NETMqttSNNeedSendCode_initializer;

/**********************************************************************************************************
 @Function			void NET_NBIOT_Initialization(void)
 @Description			NET_NBIOT_Initialization						: NET初始化
 @Input				void
 @Return				void
**********************************************************************************************************/
void NET_NBIOT_Initialization(void)
{
	/* NET NBIOT客户端初始化 */
	NET_NBIOT_Client_Init(&NetNbiotClientHandler);
	
	/* NBIOT数据传输接口初始化 */
	NBIOT_Transport_Init(&NbiotATCmdHandler);
	/* NBIOT客户端初始化 */
	NBIOT_Client_Init(&NbiotClientHandler, &NbiotATCmdHandler, &NetNbiotClientHandler);
	
#ifndef NETPROTOCAL
	#error No Define NETPROTOCAL!
#else
#if (NETPROTOCAL == NETCOAP)
	
	/* PCP数据传输接口初始化 */
	PCP_Transport_Init(&PCPCoAPNetHandler, &NbiotClientHandler);
	/* PCP客户端初始化 */
	PCP_Client_Init(&PCPClientHandler, &PCPCoAPNetHandler, &NetNbiotClientHandler);
	
#elif (NETPROTOCAL == NETMQTTSN)
	
	/* DNS数据传输接口初始化 */
	DNS_Transport_Init(&DNSSocketNetHandler, &NbiotClientHandler, DNS_SERVER_LOCAL_PORT, DNS_SERVER_HOST_IP, DNS_SERVER_TELE_PORT);
	/* DNS客户端初始化 */
	DNS_Client_Init(&DNSClientHandler, &DNSSocketNetHandler, &NetNbiotClientHandler);
	
	/* MqttSN数据传输接口初始化 */
	MQTTSN_Transport_Init(&MqttSNSocketNetHandler, &NbiotClientHandler, MQTTSN_SERVER_LOCAL_PORT, MQTTSN_SERVER_HOST_IP, MQTTSN_SERVER_TELE_PORT);
	/* MQTTSN客户端初始化 */
	MQTTSN_Client_Init(&MqttSNClientHandler, &MqttSNSocketNetHandler, &NetNbiotClientHandler);
	
#elif (NETPROTOCAL == NETONENET)
	
	/* ONENET数据传输接口初始化 */
	ONENET_Transport_Init(&OneNETLWM2MNetHandler, &NbiotClientHandler);
	/* ONENET客户端初始化 */
	OneNET_Client_Init(&OneNETClientHandler, &OneNETLWM2MNetHandler, &NetNbiotClientHandler);
	
#else
	#error NETPROTOCAL Define Error
#endif
#endif
}

/**********************************************************************************************************
 @Function			void NET_NBIOT_Client_Init(NET_NBIOT_ClientsTypeDef* pClient)
 @Description			NET_NBIOT_Client_Init					: 初始化NET NBIOT客户端
 @Input				pClient								: NET NBIOT客户端实例
 @Return				void
**********************************************************************************************************/
void NET_NBIOT_Client_Init(NET_NBIOT_ClientsTypeDef* pClient)
{
#if NETPROTOCAL == NETCOAP
	
	pClient->PollExecution								= NET_POLL_EXECUTION_COAP;
	
#elif NETPROTOCAL == NETMQTTSN
	
	pClient->PollExecution								= NET_POLL_EXECUTION_DNS;
	
#elif NETPROTOCAL == NETONENET
	
	pClient->PollExecution								= NET_POLL_EXECUTION_ONENET;
	
#endif
}

/**********************************************************************************************************
 @Function			void NET_NBIOT_DataProcessing(NET_NBIOT_ClientsTypeDef* pClient)
 @Description			NET_NBIOT_DataProcessing						: NET数据处理
 @Input				void
 @Return				void
**********************************************************************************************************/
void NET_NBIOT_DataProcessing(NET_NBIOT_ClientsTypeDef* pClient)
{
#if NETPROTOCAL == NETCOAP
	
	u32 len = 0;
	SpotStatusTypedef SpotStatusData;
	
	/* 检查是否有数据需要发送 */
	if (Inspect_Message_SpotStatusisEmpty() == false) {
	#if NBCOAP_SENDCODE_LONG_STATUS
		NETCoapNeedSendCode.LongStatus = 1;
	#endif
	}
	
	/* COAP SHORT STATUS DATA ENQUEUE */
	if (NETCoapNeedSendCode.ShortStatus) {
#if NBCOAP_SENDCODE_SHORT_STATUS
		Inspect_Message_SpotStatusDequeue(&SpotStatusData);
		CoapShortStructure.HeadPacket.DeviceSN				= TCFG_EEPROM_Get_MAC_SN();
		CoapShortStructure.HeadPacket.DataLen				= 0x00;
		CoapShortStructure.HeadPacket.ProtocolType			= 0x00;
		CoapShortStructure.HeadPacket.Reserved1				= 0x00;
		CoapShortStructure.HeadPacket.ProtocolVersion		= 0x00;
		CoapShortStructure.HeadPacket.Reserved2				= 0x00;
		CoapShortStructure.HeadPacket.PacketType			= 0x05;
		CoapShortStructure.HeadPacket.PacketNumber			= 0x00;
		CoapShortStructure.MsgPacket.DestSN				= 0x00;
		CoapShortStructure.MsgPacket.Version				= 0x01;
		CoapShortStructure.MsgPacket.Type					= COAP_MSGTYPE_TYPE_SHORT_STATUS;
		CoapShortStructure.DateTime						= SpotStatusData.unixTime;
		CoapShortStructure.SpotStatus						= SpotStatusData.spot_status;
		CoapShortStructure.SpotCount						= SpotStatusData.spot_count;
		NET_Coap_Message_SendDataEnqueue((unsigned char *)&CoapShortStructure, sizeof(CoapShortStructure));
		NETCoapNeedSendCode.ShortStatus = 0;
		Inspect_Message_SpotStatusOffSet();
		TCFG_Utility_Add_Coap_SentCount();
	#if NBCOAP_LISTEN_IDLE_TYPE == NBCOAP_LISTEN_IDLE_ENABLE
		NbiotClientHandler.ListenRunCtl.ListenEnterIdle.listenEnable = true;
	#endif
	#if NBCOAP_LISTEN_PARAMETER_TYPE == NBCOAP_LISTEN_PARAMETER_ENABLE
		NbiotClientHandler.ListenRunCtl.ListenEnterParameter.listenEnable = true;
	#endif
#endif
	}
	/* COAP LONG STATUS DATA ENQUEUE */
	else if (NETCoapNeedSendCode.LongStatus) {
#if NBCOAP_SENDCODE_LONG_STATUS
		Inspect_Message_SpotStatusDequeue(&SpotStatusData);
		CoapLongStructure.HeadPacket.DeviceSN				= TCFG_EEPROM_Get_MAC_SN();
		CoapLongStructure.HeadPacket.DataLen				= 0x00;
		CoapLongStructure.HeadPacket.ProtocolType			= 0x00;
		CoapLongStructure.HeadPacket.Reserved1				= 0x00;
		CoapLongStructure.HeadPacket.ProtocolVersion			= 0x00;
		CoapLongStructure.HeadPacket.Reserved2				= 0x00;
		CoapLongStructure.HeadPacket.PacketType				= 0x05;
		CoapLongStructure.HeadPacket.PacketNumber			= 0x00;
		CoapLongStructure.MsgPacket.DestSN					= 0x00;
#if NBIOT_STATUS_MSG_VERSION_TYPE == NBIOT_STATUS_MSG_VERSION_33BYTE_V1
		CoapLongStructure.MsgPacket.Version				= 0x01;
#elif NBIOT_STATUS_MSG_VERSION_TYPE == NBIOT_STATUS_MSG_VERSION_77BYTE_V2
		CoapLongStructure.MsgPacket.Version				= 0x02;
#endif
		CoapLongStructure.MsgPacket.Type					= COAP_MSGTYPE_TYPE_LONG_STATUS;
		CoapLongStructure.DateTime						= SpotStatusData.unixTime;
		CoapLongStructure.SpotStatus						= SpotStatusData.spot_status;
		CoapLongStructure.SpotCount						= SpotStatusData.spot_count;
		CoapLongStructure.MagneticX						= SpotStatusData.qmc5883lData.X_Now;
		CoapLongStructure.MagneticY						= SpotStatusData.qmc5883lData.Y_Now;
		CoapLongStructure.MagneticZ						= SpotStatusData.qmc5883lData.Z_Now;
		CoapLongStructure.MagneticDiff					= SpotStatusData.qmc5883lDiff.BackVal_Diff;
		CoapLongStructure.RadarDistance					= SpotStatusData.radarData.DisVal;
		CoapLongStructure.RadarStrength					= SpotStatusData.radarData.MagVal;
		CoapLongStructure.RadarCoverCount					= SpotStatusData.radarData.Diff_v2;
		CoapLongStructure.RadarDiff						= SpotStatusData.radarData.Diff;
#if NBIOT_STATUS_MSG_VERSION_TYPE == NBIOT_STATUS_MSG_VERSION_77BYTE_V2
		CoapLongStructure.NBRssi							= TCFG_Utility_Get_Nbiot_Rssi_IntVal();
		CoapLongStructure.NBSnr							= TCFG_Utility_Get_Nbiot_RadioSNR() > 127 ? 127 : TCFG_Utility_Get_Nbiot_RadioSNR();
		CoapLongStructure.MCUTemp						= TCFG_Utility_Get_Device_Temperature();
		CoapLongStructure.QMCTemp						= Qmc5883lData.temp_now;
		CoapLongStructure.MagneticBackX					= Qmc5883lData.X_Back;
		CoapLongStructure.MagneticBackY					= Qmc5883lData.Y_Back;
		CoapLongStructure.MagneticBackZ					= Qmc5883lData.Z_Back;
		CoapLongStructure.Debugval						= SpotStatusData.radarData.timedomain_dif;
		for (int i = 0; i < 16; i++) {
			CoapLongStructure.Radarval[i] = radar_targetinfo.pMagNow[i+2]>255?255:radar_targetinfo.pMagNow[i+2];
			CoapLongStructure.Radarback[i] = radar_targetinfo.pMagBG[i+2]>255?255:radar_targetinfo.pMagBG[i+2];
		}
#endif
		NET_Coap_Message_SendDataEnqueue((unsigned char *)&CoapLongStructure, sizeof(CoapLongStructure));
		NETCoapNeedSendCode.LongStatus = 0;
		Inspect_Message_SpotStatusOffSet();
		TCFG_Utility_Add_Coap_SentCount();
	#if NBCOAP_LISTEN_IDLE_TYPE == NBCOAP_LISTEN_IDLE_ENABLE
		NbiotClientHandler.ListenRunCtl.ListenEnterIdle.listenEnable = true;
	#endif
	#if NBCOAP_LISTEN_PARAMETER_TYPE == NBCOAP_LISTEN_PARAMETER_ENABLE
		NbiotClientHandler.ListenRunCtl.ListenEnterParameter.listenEnable = true;
	#endif
#endif
	}
	/* COAP WORK INFO DATA ENQUEUE */
	else if (NETCoapNeedSendCode.WorkInfo) {
#if NBCOAP_SENDCODE_WORK_INFO
		if (TCFG_Utility_Get_Nbiot_Registered() != true) {
			return;
		}
		memset((void*)&CoapInfoStructure.InfoData, 0, sizeof(CoapInfoStructure.InfoData));
		CoapInfoStructure.HeadPacket.DeviceSN				= TCFG_EEPROM_Get_MAC_SN();
		CoapInfoStructure.HeadPacket.DataLen				= 0x00;
		CoapInfoStructure.HeadPacket.ProtocolType			= 0x00;
		CoapInfoStructure.HeadPacket.Reserved1				= 0x00;
		CoapInfoStructure.HeadPacket.ProtocolVersion			= 0x00;
		CoapInfoStructure.HeadPacket.Reserved2				= 0x00;
		CoapInfoStructure.HeadPacket.PacketType				= 0x05;
		CoapInfoStructure.HeadPacket.PacketNumber			= 0x00;
		CoapInfoStructure.MsgPacket.DestSN					= 0x00;
		CoapInfoStructure.MsgPacket.Version				= 0x01;
		CoapInfoStructure.MsgPacket.Type					= COAP_MSGTYPE_TYPE_INFO;
		len = NET_COAP_Message_Operate_Creat_Json_Work_Info((char *)&CoapInfoStructure.InfoData);
		NET_Coap_Message_SendDataEnqueue((unsigned char *)&CoapInfoStructure, sizeof(CoapInfoStructure) - sizeof(CoapInfoStructure.InfoData) + len);
		NETCoapNeedSendCode.WorkInfo = 0;
		TCFG_Utility_Add_Coap_SentCount();
	#if NBCOAP_LISTEN_IDLE_TYPE == NBCOAP_LISTEN_IDLE_ENABLE
		NbiotClientHandler.ListenRunCtl.ListenEnterIdle.listenEnable = NbiotClientHandler.ListenRunCtl.ListenEnterIdle.listenEnable;
	#endif
	#if NBCOAP_LISTEN_PARAMETER_TYPE == NBCOAP_LISTEN_PARAMETER_ENABLE
		NbiotClientHandler.ListenRunCtl.ListenEnterParameter.listenEnable = true;
	#endif
#endif
	}
	/* COAP BASIC INFO DATA ENQUEUE */
	else if (NETCoapNeedSendCode.BasicInfo) {
#if NBCOAP_SENDCODE_BASIC_INFO
		if (TCFG_Utility_Get_Nbiot_Registered() != true) {
			return;
		}
		memset((void*)&CoapInfoStructure.InfoData, 0, sizeof(CoapInfoStructure.InfoData));
		CoapInfoStructure.HeadPacket.DeviceSN				= TCFG_EEPROM_Get_MAC_SN();
		CoapInfoStructure.HeadPacket.DataLen				= 0x00;
		CoapInfoStructure.HeadPacket.ProtocolType			= 0x00;
		CoapInfoStructure.HeadPacket.Reserved1				= 0x00;
		CoapInfoStructure.HeadPacket.ProtocolVersion			= 0x00;
		CoapInfoStructure.HeadPacket.Reserved2				= 0x00;
		CoapInfoStructure.HeadPacket.PacketType				= 0x05;
		CoapInfoStructure.HeadPacket.PacketNumber			= 0x00;
		CoapInfoStructure.MsgPacket.DestSN					= 0x00;
		CoapInfoStructure.MsgPacket.Version				= 0x01;
		CoapInfoStructure.MsgPacket.Type					= COAP_MSGTYPE_TYPE_INFO;
		len = NET_COAP_Message_Operate_Creat_Json_Basic_Info((char *)&CoapInfoStructure.InfoData);
		NET_Coap_Message_SendDataEnqueue((unsigned char *)&CoapInfoStructure, sizeof(CoapInfoStructure) - sizeof(CoapInfoStructure.InfoData) + len);
		NETCoapNeedSendCode.BasicInfo = 0;
		TCFG_Utility_Add_Coap_SentCount();
	#if NBCOAP_LISTEN_IDLE_TYPE == NBCOAP_LISTEN_IDLE_ENABLE
		NbiotClientHandler.ListenRunCtl.ListenEnterIdle.listenEnable = true;
	#endif
	#if NBCOAP_LISTEN_PARAMETER_TYPE == NBCOAP_LISTEN_PARAMETER_ENABLE
		NbiotClientHandler.ListenRunCtl.ListenEnterParameter.listenEnable = true;
	#endif
#endif
	}
	/* COAP DYNAMIC INFO DATA ENQUEUE */
	else if (NETCoapNeedSendCode.DynamicInfo) {
#if NBCOAP_SENDCODE_DYNAMIC_INFO
		if (TCFG_Utility_Get_Nbiot_Registered() != true) {
			return;
		}
		memset((void*)&CoapInfoStructure.InfoData, 0, sizeof(CoapInfoStructure.InfoData));
		CoapInfoStructure.HeadPacket.DeviceSN				= TCFG_EEPROM_Get_MAC_SN();
		CoapInfoStructure.HeadPacket.DataLen				= 0x00;
		CoapInfoStructure.HeadPacket.ProtocolType			= 0x00;
		CoapInfoStructure.HeadPacket.Reserved1				= 0x00;
		CoapInfoStructure.HeadPacket.ProtocolVersion			= 0x00;
		CoapInfoStructure.HeadPacket.Reserved2				= 0x00;
		CoapInfoStructure.HeadPacket.PacketType				= 0x05;
		CoapInfoStructure.HeadPacket.PacketNumber			= 0x00;
		CoapInfoStructure.MsgPacket.DestSN					= 0x00;
		CoapInfoStructure.MsgPacket.Version				= 0x01;
		CoapInfoStructure.MsgPacket.Type					= COAP_MSGTYPE_TYPE_INFO;
		len = NET_COAP_Message_Operate_Creat_Json_Dynamic_Info((char *)&CoapInfoStructure.InfoData);
		NET_Coap_Message_SendDataEnqueue((unsigned char *)&CoapInfoStructure, sizeof(CoapInfoStructure) - sizeof(CoapInfoStructure.InfoData) + len);
		NETCoapNeedSendCode.DynamicInfo = 0;
		TCFG_Utility_Add_Coap_SentCount();
	#if NBCOAP_LISTEN_IDLE_TYPE == NBCOAP_LISTEN_IDLE_ENABLE
		NbiotClientHandler.ListenRunCtl.ListenEnterIdle.listenEnable = true;
	#endif
	#if NBCOAP_LISTEN_PARAMETER_TYPE == NBCOAP_LISTEN_PARAMETER_ENABLE
		NbiotClientHandler.ListenRunCtl.ListenEnterParameter.listenEnable = true;
	#endif
#endif
	}
	/* COAP RADAR INFO DATA ENQUEUE */
	else if (NETCoapNeedSendCode.RadarInfo) {
#if NBCOAP_SENDCODE_RADAR_INFO
		memset((void*)&CoapInfoStructure.InfoData, 0, sizeof(CoapInfoStructure.InfoData));
		CoapInfoStructure.HeadPacket.DeviceSN				= TCFG_EEPROM_Get_MAC_SN();
		CoapInfoStructure.HeadPacket.DataLen				= 0x00;
		CoapInfoStructure.HeadPacket.ProtocolType			= 0x00;
		CoapInfoStructure.HeadPacket.Reserved1				= 0x00;
		CoapInfoStructure.HeadPacket.ProtocolVersion			= 0x00;
		CoapInfoStructure.HeadPacket.Reserved2				= 0x00;
		CoapInfoStructure.HeadPacket.PacketType				= 0x05;
		CoapInfoStructure.HeadPacket.PacketNumber			= 0x00;
		CoapInfoStructure.MsgPacket.DestSN					= 0x00;
		CoapInfoStructure.MsgPacket.Version				= 0x01;
		CoapInfoStructure.MsgPacket.Type					= COAP_MSGTYPE_TYPE_INFO;
		len = NET_COAP_Message_Operate_Creat_Json_Radar_Info((char *)&CoapInfoStructure.InfoData);
		NET_Coap_Message_SendDataEnqueue((unsigned char *)&CoapInfoStructure, sizeof(CoapInfoStructure) - sizeof(CoapInfoStructure.InfoData) + len);
		NETCoapNeedSendCode.RadarInfo = 0;
		TCFG_Utility_Add_Coap_SentCount();
	#if NBCOAP_LISTEN_IDLE_TYPE == NBCOAP_LISTEN_IDLE_ENABLE
		NbiotClientHandler.ListenRunCtl.ListenEnterIdle.listenEnable = true;
	#endif
	#if NBCOAP_LISTEN_PARAMETER_TYPE == NBCOAP_LISTEN_PARAMETER_ENABLE
		NbiotClientHandler.ListenRunCtl.ListenEnterParameter.listenEnable = true;
	#endif
#endif
	}
	/* COAP RESPONSE INFO DATA ENQUEUE */
	else if (NETCoapNeedSendCode.ResponseInfo) {
		memset((void*)&CoapInfoStructure.InfoData, 0, sizeof(CoapInfoStructure.InfoData));
		CoapInfoStructure.HeadPacket.DeviceSN				= TCFG_EEPROM_Get_MAC_SN();
		CoapInfoStructure.HeadPacket.DataLen				= 0x00;
		CoapInfoStructure.HeadPacket.ProtocolType			= 0x00;
		CoapInfoStructure.HeadPacket.Reserved1				= 0x00;
		CoapInfoStructure.HeadPacket.ProtocolVersion			= 0x00;
		CoapInfoStructure.HeadPacket.Reserved2				= 0x00;
		CoapInfoStructure.HeadPacket.PacketType				= 0x05;
		CoapInfoStructure.HeadPacket.PacketNumber			= 0x00;
		CoapInfoStructure.MsgPacket.DestSN					= 0x00;
		CoapInfoStructure.MsgPacket.Version				= 0x01;
		CoapInfoStructure.MsgPacket.Type					= COAP_MSGTYPE_TYPE_INFO;
		len = NET_COAP_Message_Operate_Creat_Json_Response_Info((char *)&CoapInfoStructure.InfoData, NETCoapNeedSendCode.ResponseInfoErrcode);
		NET_Coap_Message_SendDataEnqueue((unsigned char *)&CoapInfoStructure, sizeof(CoapInfoStructure) - sizeof(CoapInfoStructure.InfoData) + len);
		NETCoapNeedSendCode.ResponseInfo = 0;
		TCFG_Utility_Add_Coap_SentCount();
	#if NBCOAP_LISTEN_IDLE_TYPE == NBCOAP_LISTEN_IDLE_ENABLE
		NbiotClientHandler.ListenRunCtl.ListenEnterIdle.listenEnable = true;
	#endif
	#if NBCOAP_LISTEN_PARAMETER_TYPE == NBCOAP_LISTEN_PARAMETER_ENABLE
		NbiotClientHandler.ListenRunCtl.ListenEnterParameter.listenEnable = true;
	#endif
	}
	
#elif NETPROTOCAL == NETMQTTSN
	
#if MQTTSN_MSG_VERSION_STREAM_TYPE == MQTTSN_MSG_VERSION_BYTE_STREAM
	u32 len = 0;
#endif
	SpotStatusTypedef SpotStatusData;
	
	/* 检查是否有数据需要发送 */
	if (Inspect_Message_SpotStatusisEmpty() == false) {
	#if NBMQTTSN_SENDCODE_STATUS_EXTEND
		NETMqttSNNeedSendCode.StatusExtend = 1;
	#endif
	}
	
#if MQTTSN_MSG_VERSION_STREAM_TYPE == MQTTSN_MSG_VERSION_JSON_STREAM
	
	/* MQTTSN STATUS BASIC DATA ENQUEUE */
	if (NETMqttSNNeedSendCode.StatusBasic) {
#if NBMQTTSN_SENDCODE_STATUS_BASIC
		Inspect_Message_SpotStatusDequeue(&SpotStatusData);
		MqttSNStatusBasicStructure.DeviceSN				= TCFG_EEPROM_Get_MAC_SN();
		MqttSNStatusBasicStructure.Status					= SpotStatusData.spot_status;
		MqttSNStatusBasicStructure.Count					= SpotStatusData.spot_count;
		MqttSNStatusBasicStructure.DateTime				= SpotStatusData.unixTime;
		NET_MqttSN_Message_StatusBasicEnqueue(MqttSNStatusBasicStructure);
		NETMqttSNNeedSendCode.StatusBasic = 0;
		Inspect_Message_SpotStatusOffSet();
		TCFG_Utility_Add_MqttSN_SentCount();
#endif
	}
	/* MQTTSN STATUS EXTEND DATA ENQUEUE */
	else if (NETMqttSNNeedSendCode.StatusExtend) {
#if NBMQTTSN_SENDCODE_STATUS_EXTEND
		Inspect_Message_SpotStatusDequeue(&SpotStatusData);
		MqttSNStatusExtendStructure.DeviceSN				= TCFG_EEPROM_Get_MAC_SN();
		MqttSNStatusExtendStructure.Status					= SpotStatusData.spot_status;
		MqttSNStatusExtendStructure.Count					= SpotStatusData.spot_count;
		MqttSNStatusExtendStructure.DateTime				= SpotStatusData.unixTime;
		MqttSNStatusExtendStructure.MagX					= SpotStatusData.qmc5883lData.X_Now;
		MqttSNStatusExtendStructure.MagY					= SpotStatusData.qmc5883lData.Y_Now;
		MqttSNStatusExtendStructure.MagZ					= SpotStatusData.qmc5883lData.Z_Now;
		MqttSNStatusExtendStructure.MagDiff				= SpotStatusData.qmc5883lDiff.BackVal_Diff;
		MqttSNStatusExtendStructure.Distance				= SpotStatusData.radarData.DisVal;
		MqttSNStatusExtendStructure.Strength				= SpotStatusData.radarData.MagVal;
		MqttSNStatusExtendStructure.CoverCount				= SpotStatusData.radarData.Diff_v2;
		MqttSNStatusExtendStructure.RadarDiff				= SpotStatusData.radarData.Diff;
#if MQTTSN_STATUS_MSG_VERSION_TYPE == MQTTSN_STATUS_MSG_VERSION_V2
		MqttSNStatusExtendStructure.NBRssi					= TCFG_Utility_Get_Nbiot_Rssi_IntVal();
		MqttSNStatusExtendStructure.NBSnr					= TCFG_Utility_Get_Nbiot_RadioSNR() > 127 ? 127 : TCFG_Utility_Get_Nbiot_RadioSNR();
		MqttSNStatusExtendStructure.MCUTemp				= TCFG_Utility_Get_Device_Temperature();
		MqttSNStatusExtendStructure.QMCTemp				= Qmc5883lData.temp_now;
		MqttSNStatusExtendStructure.MagneticBackX			= Qmc5883lData.X_Back;
		MqttSNStatusExtendStructure.MagneticBackY			= Qmc5883lData.Y_Back;
		MqttSNStatusExtendStructure.MagneticBackZ			= Qmc5883lData.Z_Back;
		MqttSNStatusExtendStructure.Debugval				= SpotStatusData.radarData.timedomain_dif;
		for (int i = 0; i < 16; i++) {
			MqttSNStatusExtendStructure.Radarval[i] = radar_targetinfo.pMagNow[i+2]>255?255:radar_targetinfo.pMagNow[i+2];
			MqttSNStatusExtendStructure.Radarback[i] = radar_targetinfo.pMagBG[i+2]>255?255:radar_targetinfo.pMagBG[i+2];
		}
#endif
		NET_MqttSN_Message_StatusExtendEnqueue(MqttSNStatusExtendStructure);
		NETMqttSNNeedSendCode.StatusExtend = 0;
		Inspect_Message_SpotStatusOffSet();
		TCFG_Utility_Add_MqttSN_SentCount();
#endif
	}
	/* MQTTSN INFO WORK DATA ENQUEUE */
	else if (NETMqttSNNeedSendCode.InfoWork) {
#if NBMQTTSN_SENDCODE_WORK_INFO
		if (TCFG_Utility_Get_Nbiot_Registered() != true) {
			return;
		}
		MqttSNInfoWorkStructure.DeviceSN					= TCFG_EEPROM_Get_MAC_SN();
		NET_MqttSN_Message_InfoWorkEnqueue(MqttSNInfoWorkStructure);
		NETMqttSNNeedSendCode.InfoWork = 0;
		TCFG_Utility_Add_MqttSN_SentCount();
#endif
	}
	/* MQTTSN INFO BASIC DATA ENQUEUE */
	else if (NETMqttSNNeedSendCode.InfoBasic) {
#if NBMQTTSN_SENDCODE_BASIC_INFO
		if (TCFG_Utility_Get_Nbiot_Registered() != true) {
			return;
		}
		MqttSNInfoBasicStructure.DeviceSN					= TCFG_EEPROM_Get_MAC_SN();
		NET_MqttSN_Message_InfoBasicEnqueue(MqttSNInfoBasicStructure);
		NETMqttSNNeedSendCode.InfoBasic = 0;
		TCFG_Utility_Add_MqttSN_SentCount();
#endif
	}
	/* MQTTSN INFO DYNAMIC DATA ENQUEUE */
	else if (NETMqttSNNeedSendCode.InfoDynamic) {
#if NBMQTTSN_SENDCODE_DYNAMIC_INFO
		if (TCFG_Utility_Get_Nbiot_Registered() != true) {
			return;
		}
		MqttSNInfoDynamicStructure.DeviceSN				= TCFG_EEPROM_Get_MAC_SN();
		NET_MqttSN_Message_InfoDynamicEnqueue(MqttSNInfoDynamicStructure);
		NETMqttSNNeedSendCode.InfoDynamic = 0;
		TCFG_Utility_Add_MqttSN_SentCount();
#endif
	}
	/* MQTTSN INFO RADAR DATA ENQUEUE */
	else if (NETMqttSNNeedSendCode.InfoRadar) {
#if NBMQTTSN_SENDCODE_RADAR_INFO
		MqttSNInfoRadarStructure.DeviceSN					= TCFG_EEPROM_Get_MAC_SN();
		NET_MqttSN_Message_InfoRadarEnqueue(MqttSNInfoRadarStructure);
		NETMqttSNNeedSendCode.InfoRadar = 0;
		TCFG_Utility_Add_MqttSN_SentCount();
#endif
	}
	/* MQTTSN INFO RESPONSE DATA ENQUEUE */
	else if (NETMqttSNNeedSendCode.InfoResponse) {
		MqttSNInfoResponseStructure.DeviceSN				= TCFG_EEPROM_Get_MAC_SN();
		MqttSNInfoResponseStructure.Errcode				= NETMqttSNNeedSendCode.InfoResponseErrcode;
		NET_MqttSN_Message_InfoResponseEnqueue(MqttSNInfoResponseStructure);
		NETMqttSNNeedSendCode.InfoResponse = 0;
		TCFG_Utility_Add_MqttSN_SentCount();
	}
#endif

#if MQTTSN_MSG_VERSION_STREAM_TYPE == MQTTSN_MSG_VERSION_BYTE_STREAM
	
	/* MQTTSN SHORT STATUS DATA ENQUEUE */
	if (NETMqttSNNeedSendCode.StatusBasic) {
#if NBMQTTSN_SENDCODE_STATUS_BASIC
		Inspect_Message_SpotStatusDequeue(&SpotStatusData);
		MqttSNShortStructure.HeadPacket.DeviceSN			= TCFG_EEPROM_Get_MAC_SN();
		MqttSNShortStructure.HeadPacket.DataLen				= 0x00;
		MqttSNShortStructure.HeadPacket.ProtocolType			= 0x00;
		MqttSNShortStructure.HeadPacket.Reserved1			= 0x00;
		MqttSNShortStructure.HeadPacket.ProtocolVersion		= 0x00;
		MqttSNShortStructure.HeadPacket.Reserved2			= 0x00;
		MqttSNShortStructure.HeadPacket.PacketType			= 0x05;
		MqttSNShortStructure.HeadPacket.PacketNumber			= 0x00;
		MqttSNShortStructure.MsgPacket.DestSN				= 0x00;
		MqttSNShortStructure.MsgPacket.Version				= 0x01;
		MqttSNShortStructure.MsgPacket.Type				= MQTTSN_MSGTYPE_TYPE_SHORT_STATUS;
		MqttSNShortStructure.DateTime						= SpotStatusData.unixTime;
		MqttSNShortStructure.SpotStatus					= SpotStatusData.spot_status;
		MqttSNShortStructure.SpotCount					= SpotStatusData.spot_count;
		NET_MqttSN_Message_SendDataEnqueue((unsigned char *)&MqttSNShortStructure, sizeof(MqttSNShortStructure));
		NETMqttSNNeedSendCode.StatusBasic = 0;
		Inspect_Message_SpotStatusOffSet();
		TCFG_Utility_Add_MqttSN_SentCount();
#endif
	}
	/* MQTTSN LONG STATUS DATA ENQUEUE */
	else if (NETMqttSNNeedSendCode.StatusExtend) {
#if NBMQTTSN_SENDCODE_STATUS_EXTEND
		Inspect_Message_SpotStatusDequeue(&SpotStatusData);
		MqttSNLongStructure.HeadPacket.DeviceSN				= TCFG_EEPROM_Get_MAC_SN();
		MqttSNLongStructure.HeadPacket.DataLen				= 0x00;
		MqttSNLongStructure.HeadPacket.ProtocolType			= 0x00;
		MqttSNLongStructure.HeadPacket.Reserved1			= 0x00;
		MqttSNLongStructure.HeadPacket.ProtocolVersion		= 0x00;
		MqttSNLongStructure.HeadPacket.Reserved2			= 0x00;
		MqttSNLongStructure.HeadPacket.PacketType			= 0x05;
		MqttSNLongStructure.HeadPacket.PacketNumber			= 0x00;
		MqttSNLongStructure.MsgPacket.DestSN				= 0x00;
#if MQTTSN_STATUS_MSG_VERSION_TYPE == MQTTSN_STATUS_MSG_VERSION_V1
		MqttSNLongStructure.MsgPacket.Version				= 0x01;
#elif MQTTSN_STATUS_MSG_VERSION_TYPE == MQTTSN_STATUS_MSG_VERSION_V2
		MqttSNLongStructure.MsgPacket.Version				= 0x02;
#endif
		MqttSNLongStructure.MsgPacket.Type					= MQTTSN_MSGTYPE_TYPE_LONG_STATUS;
		MqttSNLongStructure.DateTime						= SpotStatusData.unixTime;
		MqttSNLongStructure.SpotStatus					= SpotStatusData.spot_status;
		MqttSNLongStructure.SpotCount						= SpotStatusData.spot_count;
		MqttSNLongStructure.MagneticX						= SpotStatusData.qmc5883lData.X_Now;
		MqttSNLongStructure.MagneticY						= SpotStatusData.qmc5883lData.Y_Now;
		MqttSNLongStructure.MagneticZ						= SpotStatusData.qmc5883lData.Z_Now;
		MqttSNLongStructure.MagneticDiff					= SpotStatusData.qmc5883lDiff.BackVal_Diff;
		MqttSNLongStructure.RadarDistance					= SpotStatusData.radarData.DisVal;
		MqttSNLongStructure.RadarStrength					= SpotStatusData.radarData.MagVal;
		MqttSNLongStructure.RadarCoverCount				= SpotStatusData.radarData.Diff_v2;
		MqttSNLongStructure.RadarDiff						= SpotStatusData.radarData.Diff;
#if MQTTSN_STATUS_MSG_VERSION_TYPE == MQTTSN_STATUS_MSG_VERSION_V2
		MqttSNLongStructure.NBRssi						= TCFG_Utility_Get_Nbiot_Rssi_IntVal();
		MqttSNLongStructure.NBSnr						= TCFG_Utility_Get_Nbiot_RadioSNR() > 127 ? 127 : TCFG_Utility_Get_Nbiot_RadioSNR();
		MqttSNLongStructure.MCUTemp						= TCFG_Utility_Get_Device_Temperature();
		MqttSNLongStructure.QMCTemp						= Qmc5883lData.temp_now;
		MqttSNLongStructure.MagneticBackX					= Qmc5883lData.X_Back;
		MqttSNLongStructure.MagneticBackY					= Qmc5883lData.Y_Back;
		MqttSNLongStructure.MagneticBackZ					= Qmc5883lData.Z_Back;
		MqttSNLongStructure.Debugval						= SpotStatusData.radarData.timedomain_dif;
		for (int i = 0; i < 16; i++) {
			MqttSNLongStructure.Radarval[i] = radar_targetinfo.pMagNow[i+2]>255?255:radar_targetinfo.pMagNow[i+2];
			MqttSNLongStructure.Radarback[i] = radar_targetinfo.pMagBG[i+2]>255?255:radar_targetinfo.pMagBG[i+2];
		}
#endif
		NET_MqttSN_Message_SendDataEnqueue((unsigned char *)&MqttSNLongStructure, sizeof(MqttSNLongStructure));
		NETMqttSNNeedSendCode.StatusExtend = 0;
		Inspect_Message_SpotStatusOffSet();
		TCFG_Utility_Add_MqttSN_SentCount();
#endif
	}
	/* MQTTSN INFO WORK DATA ENQUEUE */
	else if (NETMqttSNNeedSendCode.InfoWork) {
#if NBMQTTSN_SENDCODE_WORK_INFO
		if (TCFG_Utility_Get_Nbiot_Registered() != true) {
			return;
		}
		memset((void*)&MqttSNInfoStructure.InfoData, 0, sizeof(MqttSNInfoStructure.InfoData));
		MqttSNInfoStructure.HeadPacket.DeviceSN				= TCFG_EEPROM_Get_MAC_SN();
		MqttSNInfoStructure.HeadPacket.DataLen				= 0x00;
		MqttSNInfoStructure.HeadPacket.ProtocolType			= 0x00;
		MqttSNInfoStructure.HeadPacket.Reserved1			= 0x00;
		MqttSNInfoStructure.HeadPacket.ProtocolVersion		= 0x00;
		MqttSNInfoStructure.HeadPacket.Reserved2			= 0x00;
		MqttSNInfoStructure.HeadPacket.PacketType			= 0x05;
		MqttSNInfoStructure.HeadPacket.PacketNumber			= 0x00;
		MqttSNInfoStructure.MsgPacket.DestSN				= 0x00;
		MqttSNInfoStructure.MsgPacket.Version				= 0x01;
		MqttSNInfoStructure.MsgPacket.Type					= MQTTSN_MSGTYPE_TYPE_INFO;
		len = NET_MQTTSN_Message_Operate_Creat_Json_Work_Info((char *)&MqttSNInfoStructure.InfoData);
		NET_MqttSN_Message_SendDataEnqueue((unsigned char *)&MqttSNInfoStructure, sizeof(MqttSNInfoStructure) - sizeof(MqttSNInfoStructure.InfoData) + len);
		NETMqttSNNeedSendCode.InfoWork = 0;
		TCFG_Utility_Add_MqttSN_SentCount();
#endif
	}
	/* MQTTSN INFO BASIC DATA ENQUEUE */
	else if (NETMqttSNNeedSendCode.InfoBasic) {
#if NBMQTTSN_SENDCODE_BASIC_INFO
		if (TCFG_Utility_Get_Nbiot_Registered() != true) {
			return;
		}
		memset((void*)&MqttSNInfoStructure.InfoData, 0, sizeof(MqttSNInfoStructure.InfoData));
		MqttSNInfoStructure.HeadPacket.DeviceSN				= TCFG_EEPROM_Get_MAC_SN();
		MqttSNInfoStructure.HeadPacket.DataLen				= 0x00;
		MqttSNInfoStructure.HeadPacket.ProtocolType			= 0x00;
		MqttSNInfoStructure.HeadPacket.Reserved1			= 0x00;
		MqttSNInfoStructure.HeadPacket.ProtocolVersion		= 0x00;
		MqttSNInfoStructure.HeadPacket.Reserved2			= 0x00;
		MqttSNInfoStructure.HeadPacket.PacketType			= 0x05;
		MqttSNInfoStructure.HeadPacket.PacketNumber			= 0x00;
		MqttSNInfoStructure.MsgPacket.DestSN				= 0x00;
		MqttSNInfoStructure.MsgPacket.Version				= 0x01;
		MqttSNInfoStructure.MsgPacket.Type					= MQTTSN_MSGTYPE_TYPE_INFO;
		len = NET_MQTTSN_Message_Operate_Creat_Json_Basic_Info((char *)&MqttSNInfoStructure.InfoData);
		NET_MqttSN_Message_SendDataEnqueue((unsigned char *)&MqttSNInfoStructure, sizeof(MqttSNInfoStructure) - sizeof(MqttSNInfoStructure.InfoData) + len);
		NETMqttSNNeedSendCode.InfoBasic = 0;
		TCFG_Utility_Add_MqttSN_SentCount();
#endif
	}
	/* MQTTSN INFO DYNAMIC DATA ENQUEUE */
	else if (NETMqttSNNeedSendCode.InfoDynamic) {
#if NBMQTTSN_SENDCODE_DYNAMIC_INFO
		if (TCFG_Utility_Get_Nbiot_Registered() != true) {
			return;
		}
		memset((void*)&MqttSNInfoStructure.InfoData, 0, sizeof(MqttSNInfoStructure.InfoData));
		MqttSNInfoStructure.HeadPacket.DeviceSN				= TCFG_EEPROM_Get_MAC_SN();
		MqttSNInfoStructure.HeadPacket.DataLen				= 0x00;
		MqttSNInfoStructure.HeadPacket.ProtocolType			= 0x00;
		MqttSNInfoStructure.HeadPacket.Reserved1			= 0x00;
		MqttSNInfoStructure.HeadPacket.ProtocolVersion		= 0x00;
		MqttSNInfoStructure.HeadPacket.Reserved2			= 0x00;
		MqttSNInfoStructure.HeadPacket.PacketType			= 0x05;
		MqttSNInfoStructure.HeadPacket.PacketNumber			= 0x00;
		MqttSNInfoStructure.MsgPacket.DestSN				= 0x00;
		MqttSNInfoStructure.MsgPacket.Version				= 0x01;
		MqttSNInfoStructure.MsgPacket.Type					= MQTTSN_MSGTYPE_TYPE_INFO;
		len = NET_MQTTSN_Message_Operate_Creat_Json_Dynamic_Info((char *)&MqttSNInfoStructure.InfoData);
		NET_MqttSN_Message_SendDataEnqueue((unsigned char *)&MqttSNInfoStructure, sizeof(MqttSNInfoStructure) - sizeof(MqttSNInfoStructure.InfoData) + len);
		NETMqttSNNeedSendCode.InfoDynamic = 0;
		TCFG_Utility_Add_MqttSN_SentCount();
#endif
	}
	/* MQTTSN INFO RADAR DATA ENQUEUE */
	else if (NETMqttSNNeedSendCode.InfoRadar) {
#if NBMQTTSN_SENDCODE_RADAR_INFO
		memset((void*)&MqttSNInfoStructure.InfoData, 0, sizeof(MqttSNInfoStructure.InfoData));
		MqttSNInfoStructure.HeadPacket.DeviceSN				= TCFG_EEPROM_Get_MAC_SN();
		MqttSNInfoStructure.HeadPacket.DataLen				= 0x00;
		MqttSNInfoStructure.HeadPacket.ProtocolType			= 0x00;
		MqttSNInfoStructure.HeadPacket.Reserved1			= 0x00;
		MqttSNInfoStructure.HeadPacket.ProtocolVersion		= 0x00;
		MqttSNInfoStructure.HeadPacket.Reserved2			= 0x00;
		MqttSNInfoStructure.HeadPacket.PacketType			= 0x05;
		MqttSNInfoStructure.HeadPacket.PacketNumber			= 0x00;
		MqttSNInfoStructure.MsgPacket.DestSN				= 0x00;
		MqttSNInfoStructure.MsgPacket.Version				= 0x01;
		MqttSNInfoStructure.MsgPacket.Type					= MQTTSN_MSGTYPE_TYPE_INFO;
		len = NET_MQTTSN_Message_Operate_Creat_Json_Radar_Info((char *)&MqttSNInfoStructure.InfoData);
		NET_MqttSN_Message_SendDataEnqueue((unsigned char *)&MqttSNInfoStructure, sizeof(MqttSNInfoStructure) - sizeof(MqttSNInfoStructure.InfoData) + len);
		NETMqttSNNeedSendCode.InfoRadar = 0;
		TCFG_Utility_Add_MqttSN_SentCount();
#endif
	}
	/* MQTTSN INFO RESPONSE DATA ENQUEUE */
	else if (NETMqttSNNeedSendCode.InfoResponse) {
		memset((void*)&MqttSNInfoStructure.InfoData, 0, sizeof(MqttSNInfoStructure.InfoData));
		MqttSNInfoStructure.HeadPacket.DeviceSN				= TCFG_EEPROM_Get_MAC_SN();
		MqttSNInfoStructure.HeadPacket.DataLen				= 0x00;
		MqttSNInfoStructure.HeadPacket.ProtocolType			= 0x00;
		MqttSNInfoStructure.HeadPacket.Reserved1			= 0x00;
		MqttSNInfoStructure.HeadPacket.ProtocolVersion		= 0x00;
		MqttSNInfoStructure.HeadPacket.Reserved2			= 0x00;
		MqttSNInfoStructure.HeadPacket.PacketType			= 0x05;
		MqttSNInfoStructure.HeadPacket.PacketNumber			= 0x00;
		MqttSNInfoStructure.MsgPacket.DestSN				= 0x00;
		MqttSNInfoStructure.MsgPacket.Version				= 0x01;
		MqttSNInfoStructure.MsgPacket.Type					= MQTTSN_MSGTYPE_TYPE_INFO;
		len = NET_MQTTSN_Message_Operate_Creat_Json_Response_Info((char *)&MqttSNInfoStructure.InfoData, NETMqttSNNeedSendCode.InfoResponseErrcode);
		NET_MqttSN_Message_SendDataEnqueue((unsigned char *)&MqttSNInfoStructure, sizeof(MqttSNInfoStructure) - sizeof(MqttSNInfoStructure.InfoData) + len);
		NETMqttSNNeedSendCode.InfoResponse = 0;
		TCFG_Utility_Add_MqttSN_SentCount();
	}
	
#endif
#endif
}

/**********************************************************************************************************
 @Function			void NET_NBIOT_TaskProcessing(NET_NBIOT_ClientsTypeDef* pClient)
 @Description			NET_NBIOT_TaskProcessing						: NET工作处理
 @Input				void
 @Return				void
**********************************************************************************************************/
void NET_NBIOT_TaskProcessing(NET_NBIOT_ClientsTypeDef* pClient)
{
	/* NBIOT PollExecution */
#if NETPROTOCAL == NETCOAP
	
	switch (pClient->PollExecution)
	{
	case NET_POLL_EXECUTION_COAP:
		NET_COAP_APP_PollExecution(&NbiotClientHandler);
		break;
	
	case NET_POLL_EXECUTION_DNS:
		pClient->PollExecution = NET_POLL_EXECUTION_COAP;
		break;
	
	case NET_POLL_EXECUTION_MQTTSN:
		pClient->PollExecution = NET_POLL_EXECUTION_COAP;
		break;
	
	case NET_POLL_EXECUTION_PCP:
		NET_PCP_APP_PollExecution(&PCPClientHandler);
		break;
	
	case NET_POLL_EXECUTION_ONENET:
		pClient->PollExecution = NET_POLL_EXECUTION_COAP;
		break;
	}
	
#elif NETPROTOCAL == NETMQTTSN
	
	switch (pClient->PollExecution)
	{
	case NET_POLL_EXECUTION_COAP:
		pClient->PollExecution = NET_POLL_EXECUTION_DNS;
		break;
	
	case NET_POLL_EXECUTION_DNS:
		NET_DNS_APP_PollExecution(&DNSClientHandler);
		break;
	
	case NET_POLL_EXECUTION_MQTTSN:
		NET_MQTTSN_APP_PollExecution(&MqttSNClientHandler);
		break;
	
	case NET_POLL_EXECUTION_PCP:
		pClient->PollExecution = NET_POLL_EXECUTION_DNS;
		break;
	
	case NET_POLL_EXECUTION_ONENET:
		pClient->PollExecution = NET_POLL_EXECUTION_DNS;
		break;
	}
	
#elif NETPROTOCAL == NETONENET
	
	switch (pClient->PollExecution)
	{
	case NET_POLL_EXECUTION_COAP:
		pClient->PollExecution = NET_POLL_EXECUTION_ONENET;
		break;
	
	case NET_POLL_EXECUTION_DNS:
		pClient->PollExecution = NET_POLL_EXECUTION_ONENET;
		break;
	
	case NET_POLL_EXECUTION_MQTTSN:
		pClient->PollExecution = NET_POLL_EXECUTION_ONENET;
		break;
	
	case NET_POLL_EXECUTION_PCP:
		pClient->PollExecution = NET_POLL_EXECUTION_ONENET;
		break;
	
	case NET_POLL_EXECUTION_ONENET:
		NET_ONENET_APP_PollExecution(&OneNETClientHandler);
		break;
	}
	
#endif
}

/**********************************************************************************************************
 @Function			void NET_NBIOT_App_Task(void)
 @Description			NET_NBIOT_App_Task							: NET处理
 @Input				void
 @Return				void
**********************************************************************************************************/
void NET_NBIOT_App_Task(void)
{
	NET_NBIOT_DataProcessing(&NetNbiotClientHandler);							//数据处理
	
	NET_NBIOT_TaskProcessing(&NetNbiotClientHandler);							//工作处理
}

/********************************************** END OF FLEE **********************************************/
