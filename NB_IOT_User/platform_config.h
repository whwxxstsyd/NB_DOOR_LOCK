#ifndef __PLATFORM_CONFIG_H
#define   __PLATFORM_CONFIG_H

#include "sys.h"
#include "net_coap_app.h"
#include "net_pcp_app.h"
#include "net_dns_app.h"
#include "net_mqttsn_app.h"
#include "net_onenet_app.h"

//#define	MVB_SUBSN						0x81011000						//设备号
//#define	MVB_BRAND						"mvb"							//厂牌

#define	CPUVERSION					"STM32L151xBA"						//处理器型号

#define	MVB_STATICMAGNETIC_MODEL			51
#define	MVB_FLOWMAGNETIC_MODEL			52
#define	MVB_SPOT_LOCKER				53
#define	MVB_MODEL_TYPE					MVB_STATICMAGNETIC_MODEL				//设备类型

#define	SYSTEMCLOCKMSI					4194000
#define	SYSTEMCLOCKHSI					32000000
#define	SYSTEMCLOCK					SYSTEMCLOCKMSI						//系统时钟
	
#define	NETCOAP							0								//COAP
#define	NETMQTTSN						1								//MQTTSN
#define	NETONENET						2								//ONENET
#define	NETPROTOCAL					NETCOAP							//网络模式

#define	NBIOT_PRINT_ERRORCODE_DISABLE		0
#define	NBIOT_PRINT_ERRORCODE_ENABLE		1
#define	NBIOT_PRINT_ERROR_CODE_TYPE		NBIOT_PRINT_ERRORCODE_ENABLE			//NBIOT输出错误码模式

#define	LOWPOWERDISABLE				0
#define	LOWPOWERENABLE					1
#define	LOWPOWERMODE					LOWPOWERENABLE						//低功耗模式

#define	DEBUGLOWPOWERDISABLE			0
#define	DEBUGLOWPOWERENABLE				1
#define	DEBUGLOWPOWERMODE				DEBUGLOWPOWERDISABLE				//调试低功耗模式

#define	NBCOAP_SENDMODE_NORMAL			SEND_DATA
#define	NBCOAP_SENDMODE_RAIDLE			SEND_DATA_RA_NORMAL
#define	NBCOAP_SENDMODE_TYPE			NBCOAP_SENDMODE_RAIDLE				//COAP发送模式

#define	NBCOAP_RASENDMODE_NORMAL			0
#define	NBCOAP_RASENDMODE_IDLE			1
#define	NBCOAP_RASENDMODE_TYPE			NBCOAP_RASENDMODE_IDLE				//COAP发送RA模式

#define	NBCOAP_SENDDATA_NQMGSCHECK_DISABLE	0
#define	NBCOAP_SENDDATA_NQMGSCHECK_ENABLE	1
#define	NBCOAP_SENDDATA_NQMGSCHECK_TYPE	NBCOAP_SENDDATA_NQMGSCHECK_DISABLE		//COAP发送数据NQMGS检查

#define	NBCOAP_QUOTA_TIME_3YEAR			1972
#define	NBCOAP_QUOTA_TIME_5YEAR			1183
#define	NBCOAP_COAP_QUOTA_TIME_TYPE		NBCOAP_QUOTA_TIME_3YEAR				//Coap一天使用配额时间

#define	NBCOAP_RATIME_NORMAL_2HOUR		2
#define	NBCOAP_RATIME_NORMAL_4HOUR		4
#define	NBCOAP_RATIME_NORMAL_TYPE		NBCOAP_RATIME_NORMAL_2HOUR			//Coap间隔时间发送普通数据包

#define	NBCOAP_PCP_UPGRADE_LIMIT_RSSI		10								//PCP升级限制信号值
#define	NBCOAP_PCP_UPGRADE_LIMIT_SNR		5								//PCP升级限制信噪比

#define	NBIOT_CONTINUE_LIFETIME			2								//NBIOT休眠正反继续活跃20秒
#define	NBIOT_MAX_LIFETIME				12								//NBIOT休眠正反最大活跃120秒

#define	RADIO_CMD_ECHO_DISABLE			0
#define	RADIO_CMD_ECHO_ENABLE			1
#define	RADIO_CMD_ECHO_TYPE				RADIO_CMD_ECHO_DISABLE				//无线命令回显

#define	DEVICE_BOOT_START_MAGINIT_DISABLE	0
#define	DEVICE_BOOT_START_MAGINIT_ENABLE	1
#define	DEVICE_BOOT_START_MAGINIT_TYPE	DEVICE_BOOT_START_MAGINIT_DISABLE		//上电初始化背景

#define	NBIOT_MODEL_LIERDA				0
#define	NBIOT_MODEL_QUECTEL				1
#define	NBIOT_MODEL_TYPE				NBIOT_MODEL_LIERDA					//NB模组厂家

#define	NBIOT_AUTO_MODEL_DISABLE			0
#define	NBIOT_AUTO_MODEL_ENABLE			1
#define	NBIOT_AUTO_MODEL_TYPE			NBIOT_AUTO_MODEL_ENABLE				//NB模组厂家自动选择

#define	RADAR_MODEL_V1					1
#define	RADAR_MODEL_V2					2
#define	RADAR_MODEL_TYPE				RADAR_MODEL_V2						//雷达类型

#define	BEEP_ACTIVE					0
#define	BEEP_PASSIVE					1
#define	BEEP_MODEL_TYPE				BEEP_PASSIVE						//蜂鸣器类型

#define	BEEP_ON_US					230								//无源蜂鸣器开时间
#define	BEEP_OFF_US					230								//无源蜂鸣器关时间

#define	SOFTWAREMAJOR					20								//主固件版本
#define	SOFTWARESUB					125								//从固件版本
#define	HARDWAREMAJOR_V1				2								//主硬件版本
#define	HARDWAREMAJOR_V2				12								//主硬件版本

#define	RADIO_PRINT_WORKINFO			1								//无线输出Workinfo
#define	RADIO_PRINT_NETINFO				1								//无线输出NetInfo
#define	RADIO_PRINT_DEVINFO				1								//无线输出DeviceInfo
#define	RADIO_PRINT_UPGRADEINFO			1								//无线输出UpgradeInfo

#define	NBCOAP_SENDCODE_SHORT_STATUS		0								//NB上报信息使能CoapShortStatus
#define	NBCOAP_SENDCODE_LONG_STATUS		1								//NB上报信息使能CoapLongStatus
#define	NBCOAP_SENDCODE_WORK_INFO		1								//NB上报信息使能CoapWorkInfo
#define	NBCOAP_SENDCODE_BASIC_INFO		1								//NB上报信息使能CoapBasicInfo
#define	NBCOAP_SENDCODE_DYNAMIC_INFO		1								//NB上报信息使能CoapDynamicInfo
#define	NBCOAP_SENDCODE_RADAR_INFO		0								//NB上报信息使能CoapRadarInfo

#define	NBMQTTSN_SENDCODE_STATUS_BASIC	0								//NB上报信息使能MqttSNStatusBasic
#define	NBMQTTSN_SENDCODE_STATUS_EXTEND	1								//NB上报信息使能MqttSNStatusExtend
#define	NBMQTTSN_SENDCODE_WORK_INFO		1								//NB上报信息使能MqttSNWorkInfo
#define	NBMQTTSN_SENDCODE_BASIC_INFO		1								//NB上报信息使能MqttSNBasicInfo
#define	NBMQTTSN_SENDCODE_DYNAMIC_INFO	1								//NB上报信息使能MqttSNDynamicInfo
#define	NBMQTTSN_SENDCODE_RADAR_INFO		0								//NB上报信息使能MqttSNRadarInfo

#define	RF_DPRINT_LV_0					0								//调试信息不打印
#define	RF_DPRINT_LV_1					1								//基本信息打印
#define	RF_DPRINT_LV_2					2								//NB信息打印
#define	RF_DPRINT_LV_3					3								//所有信息打印
#define	RF_DPRINT_LV_TYPE				RF_DPRINT_LV_2						//无线调试信息输出等级

#define	RADIO_SI4438													//无线开启
#define	GD25Q_80CSIG													//SPIFLASH开启

#define	RADIO_RF_CHANNEL1				36								//无线默认通道

#define	USART1_DISABLE					0
#define	USART1_ENABLE					1
#define	USART1_TYPE					USART1_ENABLE						//串口1状态

#define	USART2_DISABLE					0
#define	USART2_ENABLE					1
#define	USART2_TYPE					USART2_DISABLE						//串口2状态

#define	IDLE_WORK						4								//休眠
#define	NOTACTIVE_WORK					3								//不工作

#define	DEBUG_WORK					1								//调试工作模式
#define	NORMAL_WORK					0								//正常工作模式

#define	RADIO_GATWAY_NEARBY_HEART_SEC		5								//无线接收网关数据心跳包间隔时间
#define	RADIO_GATCMD_NEARBY_HEART_SEC		3								//无线接收网关数据命令包间隔时间

#define	TIME_TO_MODULE_INIT				3600*24*3							//间隔时间初始化模块

#define	NBIOT_HEART_DATA_HOURS			4								//NB心跳数据包时间

#define	DNS_SERVER_HOST_IP				"114.114.114.114"					//DNS服务器默认配置
#define	DNS_SERVER_LOCAL_PORT			5000
#define	DNS_SERVER_TELE_PORT			53

#define	MQTTSN_SERVER_HOST_NAME			"mqtt.movebroad.cn"					//MQTTSN服务器默认配置
#define	MQTTSN_SERVER_HOST_IP			"47.98.140.145"
#define	MQTTSN_SERVER_LOCAL_PORT			4000
#define	MQTTSN_SERVER_TELE_PORT			1884

extern bool BootUp;														//BootUp
extern bool DeviceIdleMode;												//IDLE MODE
extern bool DeviceActivedMode;											//ACTIVED MODE

extern COAP_PacketShortTypeDef			CoapShortStructure;					//Coap Short Packet
extern COAP_PacketLongTypeDef				CoapLongStructure;					//Coap Long Packet
extern COAP_PacketInfoTypeDef				CoapInfoStructure;					//Coap Info Packet

#if MQTTSN_MSG_VERSION_STREAM_TYPE == MQTTSN_MSG_VERSION_JSON_STREAM
extern MQTTSN_StatusBasicTypeDef			MqttSNStatusBasicStructure;			//MqttSN Basic Status Packet
extern MQTTSN_StatusExtendTypeDef			MqttSNStatusExtendStructure;			//MqttSN Extend Status Packet
extern MQTTSN_InfoWorkTypeDef				MqttSNInfoWorkStructure;				//MqttSN Work Info Packet
extern MQTTSN_InfoBasicTypeDef			MqttSNInfoBasicStructure;			//MqttSN Basic Info Packet
extern MQTTSN_InfoDynamicTypeDef			MqttSNInfoDynamicStructure;			//MqttSN Dynamic Info Packet
extern MQTTSN_InfoRadarTypeDef			MqttSNInfoRadarStructure;			//MqttSN Radar Info Packet
extern MQTTSN_InfoResponseTypeDef			MqttSNInfoResponseStructure;			//MqttSN Response Info Packet
#endif
#if MQTTSN_MSG_VERSION_STREAM_TYPE == MQTTSN_MSG_VERSION_BYTE_STREAM
extern MQTTSN_PacketShortTypeDef			MqttSNShortStructure;				//MqttSN Short Packet
extern MQTTSN_PacketLongTypeDef			MqttSNLongStructure;				//MqttSN Long Packet
extern MQTTSN_PacketInfoTypeDef			MqttSNInfoStructure;				//MqttSN Info Packet
#endif

extern NET_NBIOT_ClientsTypeDef			NetNbiotClientHandler;				//NET NBIOT Clinet Handler
extern NBIOT_ATCmdTypeDef				NbiotATCmdHandler;					//NBIOT AT Cmd Handler
extern NBIOT_ClientsTypeDef				NbiotClientHandler;					//Coap Clinet Handler
extern PCP_CoAPNetTransportTypeDef			PCPCoAPNetHandler;					//PCP Net Handler
extern PCP_ClientsTypeDef				PCPClientHandler;					//PCP Clinet Handler
extern DNS_SocketNetTypeDef				DNSSocketNetHandler;				//DNS Net Handler
extern DNS_ClientsTypeDef				DNSClientHandler;					//DNS Clinet Handler
extern MQTTSN_SocketNetTypeDef			MqttSNSocketNetHandler;				//MqttSN Net Handler
extern MQTTSN_ClientsTypeDef				MqttSNClientHandler;				//MqttSN Clinet Handler
extern ONENET_LWM2MTransportTypeDef		OneNETLWM2MNetHandler;				//ONENET Net Handler
extern ONENET_ClientsTypeDef				OneNETClientHandler;				//ONENET Clinet Handler

void RadioPrintWorkinfo(void);											//Radio Print WorkInfo
void RadioPrintNetinfo(void);												//Radio Print NetInfo
void RadioPrintDeviceinfo(void);											//Radio Print DeviceInfo
void RadioPrintUpgradeinfo(void);											//Radio Print UpgradeInfo







#endif
