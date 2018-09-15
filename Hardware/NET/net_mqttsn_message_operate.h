#ifndef __NET_MQTTSN_MESSAGE_OPERATE_H
#define   __NET_MQTTSN_MESSAGE_OPERATE_H

#include "sys.h"

#define MQTTSN_MSG_VERSION_JSON_STREAM		0														//Json Stream
#define MQTTSN_MSG_VERSION_BYTE_STREAM		1														//Byte Stream
#define MQTTSN_MSG_VERSION_STREAM_TYPE		MQTTSN_MSG_VERSION_BYTE_STREAM								//MQTTSN数据包版本

#define MQTTSN_STATUS_MSG_VERSION_V1		0
#define MQTTSN_STATUS_MSG_VERSION_V2		1
#define MQTTSN_STATUS_MSG_VERSION_TYPE		MQTTSN_STATUS_MSG_VERSION_V2									//MQTTSN状态包版本

#define MQTTSN_MSGTYPE_TYPE_SHORT_STATUS	0x37
#define MQTTSN_MSGTYPE_TYPE_LONG_STATUS		0x3A
#define MQTTSN_MSGTYPE_TYPE_INFO			0x35

#define NET_MESSAGE_GET_MAGICNUM(Byte)		{Byte[0]='T';Byte[1]='C';Byte[2]='L';Byte[3]='D';}
#define MESSAGE_VERSION					'1'

#if MQTTSN_MSG_VERSION_STREAM_TYPE == MQTTSN_MSG_VERSION_JSON_STREAM

typedef enum
{
	MSG_JSON							= '1',
	MSG_FIRMWARE						= '3'
}NET_Message_MsgType_TypeDef;

typedef enum
{
	ENCRYPT_NONE						= '0',
	ENCRYPT_AES128						= '1',
	ENCRYPT_GZIP						= '2'
}NET_Message_EncryptMode_TypeDef;

typedef enum
{
	LOGIN_REQ							= '0',
	LOGIN_RSP							= '1',
	LOGOUT_REQ						= '2',
	LOGOUT_RSP						= '3',
	
	KEEPALIVE_REQ						= '4',
	KEEPALIVE_RSP						= '5',
	SYSINFO_REQ						= '6',
	SYSINFO_RSP						= '7',
	CONFIG_SET						= 'S',
	CONFIG_SET_RSP						= 's',
	CONFIG_GET						= 'G',
	CONFIG_GET_RSP						= 'g',
	
	MODIFYUSER_REQ						= 'c',
	MODIFYUSER_RSP						= 'd',
	MODIFYPASSWORD_REQ					= 'e',
	MODIFYPASSWORD_RSP					= 'f',
	
	CLOUD_VERSION_REQ					= 'g',
	CLOUD_VERSION_RSP					= 'i',
	CLOUD_UPGRADE_START_REQ				= 'j',
	CLOUD_UPGRADE_START_RSP				= 'k',
	CLOUD_UPGRADE_STOP_REQ				= 'l',
	CLOUD_UPGRADE_STOP_RSP				= 'm',
	CLOUD_UPGRADE_PROGRESS				= 'n',
	CLOUD_UPGRADE_DATA					= 'U',
	CLOUD_UPGRADE_DATA_RSP				= 'u',
	
	MSGID_POST						= 'S',
	MSGID_PUT							= 'P'
}NET_Message_CtrlMsg_TypeDef;

typedef __packed struct
{
	unsigned char						MagicNum[4];
	unsigned char						MsgType;
	unsigned char						Version;
	unsigned char						EncryptMode;
	unsigned char						MsgId;
}NET_Message_TcldHeadTypeDef;

typedef __packed struct
{
	NET_Message_TcldHeadTypeDef			MsgHead;
	char								pData[1];
}NET_Message_TcldMsgTypeDef;

#endif

#if MQTTSN_MSG_VERSION_STREAM_TYPE == MQTTSN_MSG_VERSION_BYTE_STREAM

typedef __packed struct
{
	u8								DataLen;
	u8								ProtocolType:3;
	u8								Reserved1:2;
	u8								ProtocolVersion:3;
	u8								Reserved2:4;
	u8								PacketType:4;
	u8								PacketNumber;
	u32								DeviceSN;
}MQTTSN_PacketHeadTypeDef;																		//数据包头8Byte

typedef __packed struct
{
	u32								DestSN;
	u8								Version;
	u8								Type;
}MQTTSN_PacketMsgTypeDef;																		//消息包头6Byte

typedef __packed struct
{
	MQTTSN_PacketHeadTypeDef				HeadPacket;
	MQTTSN_PacketMsgTypeDef				MsgPacket;
	u32								DateTime;
	u16								SpotCount;
	u8								SpotStatus;
}MQTTSN_PacketShortTypeDef;																		//短包21Byte

typedef __packed struct
{
	MQTTSN_PacketHeadTypeDef				HeadPacket;
	MQTTSN_PacketMsgTypeDef				MsgPacket;
	u32								DateTime;
	u16								SpotCount;
	u8								SpotStatus;
	u16								MagneticX;
	u16								MagneticY;
	u16								MagneticZ;
	u16								MagneticDiff;
	u8								RadarDistance;
	u8								RadarStrength;
	u8								RadarCoverCount;
	u8								RadarDiff;
#if MQTTSN_STATUS_MSG_VERSION_TYPE == MQTTSN_STATUS_MSG_VERSION_V2
	u8								NBRssi;
	s8								NBSnr;
	s8								MCUTemp;
	s8								QMCTemp;
	s16								MagneticBackX;
	s16								MagneticBackY;
	s16								MagneticBackZ;
	s16								Debugval;
	u8								Radarval[16];
	u8								Radarback[16];
#endif
}MQTTSN_PacketLongTypeDef;																		//长包77Byte

typedef __packed struct
{
	MQTTSN_PacketHeadTypeDef				HeadPacket;
	MQTTSN_PacketMsgTypeDef				MsgPacket;
	u8								InfoData[300];
}MQTTSN_PacketInfoTypeDef;																		//Info包314Byte

#endif

#if MQTTSN_MSG_VERSION_STREAM_TYPE == MQTTSN_MSG_VERSION_JSON_STREAM

#define MQTTSN_STATUS_BASIC_PARK_NUM		3
#define MQTTSN_STATUS_EXTEND_PARK_NUM		10
#define MQTTSN_INFO_WORK_PARK_NUM			3
#define MQTTSN_INFO_BASIC_PARK_NUM			3
#define MQTTSN_INFO_DYNAMIC_PARK_NUM		3
#define MQTTSN_INFO_RADAR_PARK_NUM			3
#define MQTTSN_INFO_RESPONSE_PARK_NUM		3

/* MqttSN Status Basic */
typedef __packed struct
{
	unsigned int						DeviceSN;
	unsigned char						Status;
	unsigned int						Count;
	unsigned int						DateTime;
}MQTTSN_StatusBasicTypeDef;

typedef struct
{
	unsigned char						Front;
	unsigned char						Rear;
	MQTTSN_StatusBasicTypeDef			StatusBasic[MQTTSN_STATUS_BASIC_PARK_NUM];
}MQTTSN_SwapStatusBasicTypeDef;

/* MqttSN Status Extend */
typedef __packed struct
{
	unsigned int						DeviceSN;
	unsigned char						Status;
	unsigned int						Count;
	unsigned int						DateTime;
	int								MagX;
	int								MagY;
	int								MagZ;
	int								MagDiff;
	int								Distance;
	int								Strength;
	int								CoverCount;
	int								RadarDiff;
#if MQTTSN_STATUS_MSG_VERSION_TYPE == MQTTSN_STATUS_MSG_VERSION_V2
	unsigned char						NBRssi;
	signed char						NBSnr;
	signed char						MCUTemp;
	signed char						QMCTemp;
	signed short						MagneticBackX;
	signed short						MagneticBackY;
	signed short						MagneticBackZ;
	signed short						Debugval;
	unsigned char						Radarval[16];
	unsigned char						Radarback[16];
#endif
}MQTTSN_StatusExtendTypeDef;

typedef struct
{
	unsigned char						Front;
	unsigned char						Rear;
	MQTTSN_StatusExtendTypeDef			StatusExtend[MQTTSN_STATUS_EXTEND_PARK_NUM];
}MQTTSN_SwapStatusExtendTypeDef;

/* MqttSN Info Work */
typedef __packed struct
{
	unsigned int						DeviceSN;
}MQTTSN_InfoWorkTypeDef;

typedef struct
{
	unsigned char						Front;
	unsigned char						Rear;
	MQTTSN_InfoWorkTypeDef				InfoWork[MQTTSN_INFO_WORK_PARK_NUM];
}MQTTSN_SwapInfoWorkTypeDef;

/* MqttSN Info Basic */
typedef __packed struct
{
	unsigned int						DeviceSN;
}MQTTSN_InfoBasicTypeDef;

typedef struct
{
	unsigned char						Front;
	unsigned char						Rear;
	MQTTSN_InfoBasicTypeDef				InfoBasic[MQTTSN_INFO_BASIC_PARK_NUM];
}MQTTSN_SwapInfoBasicTypeDef;

/* MqttSN Info Dynamic */
typedef __packed struct
{
	unsigned int						DeviceSN;
}MQTTSN_InfoDynamicTypeDef;

typedef struct
{
	unsigned char						Front;
	unsigned char						Rear;
	MQTTSN_InfoDynamicTypeDef			InfoDynamic[MQTTSN_INFO_DYNAMIC_PARK_NUM];
}MQTTSN_SwapInfoDynamicTypeDef;

/* MqttSN Info Radar */
typedef __packed struct
{
	unsigned int						DeviceSN;
}MQTTSN_InfoRadarTypeDef;

typedef struct
{
	unsigned char						Front;
	unsigned char						Rear;
	MQTTSN_InfoRadarTypeDef				InfoRadar[MQTTSN_INFO_RADAR_PARK_NUM];
}MQTTSN_SwapInfoRadarTypeDef;

/* MqttSN Info Response */
typedef __packed struct
{
	unsigned int						DeviceSN;
	unsigned short						Errcode;
}MQTTSN_InfoResponseTypeDef;

typedef struct
{
	unsigned char						Front;
	unsigned char						Rear;
	MQTTSN_InfoResponseTypeDef			InfoResponse[MQTTSN_INFO_RESPONSE_PARK_NUM];
}MQTTSN_SwapInfoResponseTypeDef;

#endif

#if MQTTSN_MSG_VERSION_STREAM_TYPE == MQTTSN_MSG_VERSION_BYTE_STREAM

#define MQTTSN_SEND_BUFFER_SIZE			320
#define MQTTSN_SEND_PARK_NUM				10

/* 发送数据队列 */
typedef struct
{
	unsigned char						Front;
	unsigned char						Rear;
	
	struct MqttSNSendParkTypeDef
	{
		unsigned char					Buffer[MQTTSN_SEND_BUFFER_SIZE];
		unsigned short					Length;
	}Park[MQTTSN_SEND_PARK_NUM];
}MQTTSN_SwapSendDataTypeDef;

#endif


#if MQTTSN_MSG_VERSION_STREAM_TYPE == MQTTSN_MSG_VERSION_JSON_STREAM

int NET_Message_Operate_Creat_Json_MoteStatus_Basic(char* outBuffer);
int NET_Message_Operate_Creat_Json_MoteStatus_Extend(char* outBuffer);
int NET_Message_Operate_Creat_Json_MoteInfo_Work(char* outBuffer);
int NET_Message_Operate_Creat_Json_MoteInfo_Basic(char* outBuffer);
int NET_Message_Operate_Creat_Json_MoteInfo_Dynamic(char* outBuffer);
int NET_Message_Operate_Creat_Json_MoteInfo_Radar(char* outBuffer);
int NET_Message_Operate_Creat_Json_MoteInfo_Response(char* outBuffer);

bool NET_MqttSN_Message_StatusBasicisFull(void);
bool NET_MqttSN_Message_StatusExtendisFull(void);
bool NET_MqttSN_Message_InfoWorkisFull(void);
bool NET_MqttSN_Message_InfoBasicisFull(void);
bool NET_MqttSN_Message_InfoDynamicisFull(void);
bool NET_MqttSN_Message_InfoRadarisFull(void);
bool NET_MqttSN_Message_InfoResponseisFull(void);

bool NET_MqttSN_Message_StatusBasicisEmpty(void);
bool NET_MqttSN_Message_StatusExtendisEmpty(void);
bool NET_MqttSN_Message_InfoWorkisEmpty(void);
bool NET_MqttSN_Message_InfoBasicisEmpty(void);
bool NET_MqttSN_Message_InfoDynamicisEmpty(void);
bool NET_MqttSN_Message_InfoRadarisEmpty(void);
bool NET_MqttSN_Message_InfoResponseisEmpty(void);

void NET_MqttSN_Message_StatusBasicEnqueue(MQTTSN_StatusBasicTypeDef dataBuf);
void NET_MqttSN_Message_StatusExtendEnqueue(MQTTSN_StatusExtendTypeDef dataBuf);
void NET_MqttSN_Message_InfoWorkEnqueue(MQTTSN_InfoWorkTypeDef dataBuf);
void NET_MqttSN_Message_InfoBasicEnqueue(MQTTSN_InfoBasicTypeDef dataBuf);
void NET_MqttSN_Message_InfoDynamicEnqueue(MQTTSN_InfoDynamicTypeDef dataBuf);
void NET_MqttSN_Message_InfoRadarEnqueue(MQTTSN_InfoRadarTypeDef dataBuf);
void NET_MqttSN_Message_InfoResponseEnqueue(MQTTSN_InfoResponseTypeDef dataBuf);

bool NET_MqttSN_Message_StatusBasicDequeue(MQTTSN_StatusBasicTypeDef* dataBuf);
bool NET_MqttSN_Message_StatusExtendDequeue(MQTTSN_StatusExtendTypeDef* dataBuf);
bool NET_MqttSN_Message_InfoWorkDequeue(MQTTSN_InfoWorkTypeDef* dataBuf);
bool NET_MqttSN_Message_InfoBasicDequeue(MQTTSN_InfoBasicTypeDef* dataBuf);
bool NET_MqttSN_Message_InfoDynamicDequeue(MQTTSN_InfoDynamicTypeDef* dataBuf);
bool NET_MqttSN_Message_InfoRadarDequeue(MQTTSN_InfoRadarTypeDef* dataBuf);
bool NET_MqttSN_Message_InfoResponseDequeue(MQTTSN_InfoResponseTypeDef* dataBuf);

bool NET_MqttSN_Message_StatusBasicOffSet(void);
bool NET_MqttSN_Message_StatusExtendOffSet(void);
bool NET_MqttSN_Message_InfoWorkOffSet(void);
bool NET_MqttSN_Message_InfoBasicOffSet(void);
bool NET_MqttSN_Message_InfoDynamicOffSet(void);
bool NET_MqttSN_Message_InfoRadarOffSet(void);
bool NET_MqttSN_Message_InfoResponseOffSet(void);

unsigned char NET_MqttSN_Message_StatusBasicRear(void);
unsigned char NET_MqttSN_Message_StatusExtendRear(void);
unsigned char NET_MqttSN_Message_InfoWorkRear(void);
unsigned char NET_MqttSN_Message_InfoBasicRear(void);
unsigned char NET_MqttSN_Message_InfoDynamicRear(void);
unsigned char NET_MqttSN_Message_InfoRadarRear(void);
unsigned char NET_MqttSN_Message_InfoResponseRear(void);

#endif

#if MQTTSN_MSG_VERSION_STREAM_TYPE == MQTTSN_MSG_VERSION_BYTE_STREAM

int NET_MQTTSN_Message_Operate_Creat_Json_Work_Info(char* outBuffer);
int NET_MQTTSN_Message_Operate_Creat_Json_Basic_Info(char* outBuffer);
int NET_MQTTSN_Message_Operate_Creat_Json_Dynamic_Info(char* outBuffer);
int NET_MQTTSN_Message_Operate_Creat_Json_Radar_Info(char* outBuffer);
int NET_MQTTSN_Message_Operate_Creat_Json_Response_Info(char* outBuffer, u16 errcode);

bool NET_MqttSN_Message_SendDataisFull(void);														//检查发送队列是否已满
bool NET_MqttSN_Message_SendDataisEmpty(void);														//检查发送队列是否已空
void NET_MqttSN_Message_SendDataEnqueue(unsigned char* dataBuf, unsigned short dataLength);					//发送数据写入队列
bool NET_MqttSN_Message_SendDataDequeue(unsigned char* dataBuf, unsigned short* dataLength);					//发送数据读出队列
bool NET_MqttSN_Message_SendDataOffSet(void);														//发送数据队列(队列头偏移1)
unsigned char NET_MqttSN_Message_SendDataRear(void);													//发送数据队尾值

#endif

#endif /* __NET_MQTTSN_MESSAGE_OPERATE_H */
