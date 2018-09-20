#ifndef __PLATFORM_CONFIG_H
#define   __PLATFORM_CONFIG_H

#include "include.h"	


#define	CPUVERSION					"STM32L151xBA"						//�������ͺ�

#define	MVB_STATICMAGNETIC_MODEL		51
#define	MVB_FLOWMAGNETIC_MODEL			52
#define	MVB_SPOT_LOCKER				53
#define	MVB_MODEL_TYPE					MVB_STATICMAGNETIC_MODEL				//�豸����

#define	SYSTEMCLOCKMSI					4194000
#define	SYSTEMCLOCKHSI					32000000
#define	SYSTEMCLOCK					SYSTEMCLOCKMSI						//ϵͳʱ��

#define	NETCOAP						0								//COAP
#define	NETMQTTSN					1								//MQTTSN
#define	NETONENET					2								//ONENET
#define	NETPROTOCAL					NETCOAP							//����ģʽ

#define	NBIOT_PRINT_ERRORCODE_DISABLE		0
#define	NBIOT_PRINT_ERRORCODE_ENABLE		1
#define	NBIOT_PRINT_ERROR_CODE_TYPE		NBIOT_PRINT_ERRORCODE_ENABLE			//NBIOT���������ģʽ

#define	LOWPOWERDISABLE				0
#define	LOWPOWERENABLE					1
#define	LOWPOWERMODE					LOWPOWERENABLE						//�͹���ģʽ

#define	DEBUGLOWPOWERDISABLE			0
#define	DEBUGLOWPOWERENABLE			1
#define	DEBUGLOWPOWERMODE				DEBUGLOWPOWERDISABLE				//���Ե͹���ģʽ

#define	NBCOAP_SENDMODE_NORMAL			SEND_DATA
#define	NBCOAP_SENDMODE_RAIDLE			SEND_DATA_RA_NORMAL
#define	NBCOAP_SENDMODE_TYPE			NBCOAP_SENDMODE_RAIDLE				//COAP����ģʽ

#define	NBCOAP_RASENDMODE_NORMAL			0
#define	NBCOAP_RASENDMODE_IDLE			1
#define	NBCOAP_RASENDMODE_TYPE			NBCOAP_RASENDMODE_IDLE				//COAP����RAģʽ

#define	NBCOAP_SENDDATA_NQMGSCHECK_DISABLE	0
#define	NBCOAP_SENDDATA_NQMGSCHECK_ENABLE	1
#define	NBCOAP_SENDDATA_NQMGSCHECK_TYPE	NBCOAP_SENDDATA_NQMGSCHECK_DISABLE		//COAP��������NQMGS���

#define	NBCOAP_QUOTA_TIME_3YEAR			1972
#define	NBCOAP_QUOTA_TIME_5YEAR			1183
#define	NBCOAP_COAP_QUOTA_TIME_TYPE		NBCOAP_QUOTA_TIME_3YEAR				//Coapһ��ʹ�����ʱ��

#define	NBCOAP_RATIME_NORMAL_2HOUR		2
#define	NBCOAP_RATIME_NORMAL_4HOUR		4
#define	NBCOAP_RATIME_NORMAL_TYPE		NBCOAP_RATIME_NORMAL_2HOUR			//Coap���ʱ�䷢����ͨ���ݰ�

#define	NBCOAP_PCP_UPGRADE_LIMIT_RSSI		10								//PCP���������ź�ֵ
#define	NBCOAP_PCP_UPGRADE_LIMIT_SNR		5								//PCP�������������

#define	NBIOT_CONTINUE_LIFETIME			2								//NBIOT��������������Ծ20��
#define	NBIOT_MAX_LIFETIME				12								//NBIOT������������Ծ120��

#define	RADIO_CMD_ECHO_DISABLE			0
#define	RADIO_CMD_ECHO_ENABLE			1
#define	RADIO_CMD_ECHO_TYPE				RADIO_CMD_ECHO_DISABLE				//�����������

#define	DEVICE_BOOT_START_MAGINIT_DISABLE	0
#define	DEVICE_BOOT_START_MAGINIT_ENABLE	1
#define	DEVICE_BOOT_START_MAGINIT_TYPE	DEVICE_BOOT_START_MAGINIT_DISABLE		//�ϵ��ʼ������

#define	NBIOT_MODEL_LIERDA				0
#define	NBIOT_MODEL_QUECTEL				1
#define	NBIOT_MODEL_TYPE				NBIOT_MODEL_LIERDA					//NBģ�鳧��

#define	NBIOT_AUTO_MODEL_DISABLE			0
#define	NBIOT_AUTO_MODEL_ENABLE			1
#define	NBIOT_AUTO_MODEL_TYPE			NBIOT_AUTO_MODEL_ENABLE				//NBģ�鳧���Զ�ѡ��

#define	RADAR_MODEL_V1					1
#define	RADAR_MODEL_V2					2
#define	RADAR_MODEL_TYPE				RADAR_MODEL_V2						//�״�����

#define	BEEP_ACTIVE					0
#define	BEEP_PASSIVE					1
#define	BEEP_MODEL_TYPE				BEEP_PASSIVE						//����������

#define	BEEP_ON_US					230								//��Դ��������ʱ��
#define	BEEP_OFF_US					230								//��Դ��������ʱ��

#define	SOFTWAREMAJOR					20								//���̼��汾
#define	SOFTWARESUB					125								//�ӹ̼��汾
#define	HARDWAREMAJOR_V1				2								//��Ӳ���汾
#define	HARDWAREMAJOR_V2				12								//��Ӳ���汾

#define	RADIO_PRINT_WORKINFO			1								//�������Workinfo
#define	RADIO_PRINT_NETINFO				1								//�������NetInfo
#define	RADIO_PRINT_DEVINFO				1								//�������DeviceInfo
#define	RADIO_PRINT_UPGRADEINFO			1								//�������UpgradeInfo

#define	NBCOAP_SENDCODE_SHORT_STATUS		0								//NB�ϱ���Ϣʹ��CoapShortStatus
#define	NBCOAP_SENDCODE_LONG_STATUS		1								//NB�ϱ���Ϣʹ��CoapLongStatus
#define	NBCOAP_SENDCODE_WORK_INFO		1									//NB�ϱ���Ϣʹ��CoapWorkInfo
#define	NBCOAP_SENDCODE_BASIC_INFO		1								//NB�ϱ���Ϣʹ��CoapBasicInfo
#define	NBCOAP_SENDCODE_DYNAMIC_INFO		1								//NB�ϱ���Ϣʹ��CoapDynamicInfo
#define	NBCOAP_SENDCODE_RADAR_INFO		0								//NB�ϱ���Ϣʹ��CoapRadarInfo

#define	NBMQTTSN_SENDCODE_STATUS_BASIC	0								//NB�ϱ���Ϣʹ��MqttSNStatusBasic
#define	NBMQTTSN_SENDCODE_STATUS_EXTEND	1								//NB�ϱ���Ϣʹ��MqttSNStatusExtend
#define	NBMQTTSN_SENDCODE_WORK_INFO		1								//NB�ϱ���Ϣʹ��MqttSNWorkInfo
#define	NBMQTTSN_SENDCODE_BASIC_INFO		1								//NB�ϱ���Ϣʹ��MqttSNBasicInfo
#define	NBMQTTSN_SENDCODE_DYNAMIC_INFO	1								//NB�ϱ���Ϣʹ��MqttSNDynamicInfo
#define	NBMQTTSN_SENDCODE_RADAR_INFO		0								//NB�ϱ���Ϣʹ��MqttSNRadarInfo

#define	RF_DPRINT_LV_0					0								//������Ϣ����ӡ
#define	RF_DPRINT_LV_1					1								//������Ϣ��ӡ
#define	RF_DPRINT_LV_2					2								//NB��Ϣ��ӡ
#define	RF_DPRINT_LV_3					3								//������Ϣ��ӡ
#define	RF_DPRINT_LV_TYPE				RF_DPRINT_LV_2						//���ߵ�����Ϣ����ȼ�

#define	RADIO_SI4438													//���߿���
//#define	GD25Q_80CSIG													//SPIFLASH����

#define	RADIO_RF_CHANNEL1				36								//����Ĭ��ͨ��

#define	USART1_DISABLE					0
#define	USART1_ENABLE					1
#define	USART1_TYPE					USART1_ENABLE						//����1״̬

#define	USART2_DISABLE					0
#define	USART2_ENABLE					1
#define	USART2_TYPE					USART2_DISABLE						//����2״̬

#define	IDLE_WORK						4								//����
#define	NOTACTIVE_WORK					3								//������

#define	DEBUG_WORK					1								//���Թ���ģʽ
#define	NORMAL_WORK					0								//��������ģʽ

#define	RADIO_GATWAY_NEARBY_HEART_SEC		5								//���߽��������������������ʱ��
#define	RADIO_GATCMD_NEARBY_HEART_SEC		3								//���߽�������������������ʱ��

#define	TIME_TO_MODULE_INIT				3600*24*3							//���ʱ���ʼ��ģ��

#define	NBIOT_HEART_DATA_HOURS			4								//NB�������ݰ�ʱ��

#define	DNS_SERVER_HOST_IP				"114.114.114.114"					//DNS������Ĭ������
#define	DNS_SERVER_LOCAL_PORT			5000
#define	DNS_SERVER_TELE_PORT			53

#define	MQTTSN_SERVER_HOST_NAME			"mqtt.movebroad.cn"					//MQTTSN������Ĭ������
#define	MQTTSN_SERVER_HOST_IP			"47.98.140.145"
#define	MQTTSN_SERVER_LOCAL_PORT			4000
#define	MQTTSN_SERVER_TELE_PORT			1884


#endif


