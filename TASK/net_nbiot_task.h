#ifndef __NET_NBIOT_TASK_H
#define   __NET_NBIOT_TASK_H	


#define NETCoapNeedSendCode_initializer		{ 0, 0, 1, 0, 1, 1, 0, 0, NETIP_OK}
#define NETMqttSNNeedSendCode_initializer		{ 0, 0, 1, 0, 1, 1, 0, 0, NETIP_OK}
	

typedef struct
{
	u8			ShortStatus;	
	u8			LongStatus;
	u8			WorkInfo;
	u8			WorkInfoWait;
	u8			BasicInfo;
	u8			DynamicInfo;
	u8			RadarInfo;
	u8			ResponseInfo;
	u16			ResponseInfoErrcode;
	
}NETCoapNeedSendCodeTypeDef;

extern NETCoapNeedSendCodeTypeDef NETCoapNeedSendCode;



typedef struct
{
	u8			StatusBasic;
	u8			StatusExtend;
	u8			InfoWork;
	u8			InfoWorkWait;
	u8			InfoBasic;
	u8			InfoDynamic;
	u8			InfoRadar;
	u8			InfoResponse;			
	u16			InfoResponseErrcode;
	
}NETMqttSNNeedSendCodeTypeDef;

extern NETMqttSNNeedSendCodeTypeDef NETMqttSNNeedSendCode;



/* NET Is PollExecution */
typedef enum
{
	NET_POLL_EXECUTION_COAP			= 0x00,
	NET_POLL_EXECUTION_DNS				= 0x01,
	NET_POLL_EXECUTION_MQTTSN			= 0x02,
	NET_POLL_EXECUTION_PCP				= 0x03,
	NET_POLL_EXECUTION_ONENET			= 0x04
}NET_NBIOT_PollExecutionTypeDef;
	
	
/* NET Clients */
typedef struct NET_NBIOT_ClientsTypeDef
{
	NET_NBIOT_PollExecutionTypeDef	PollExecution;	
}NET_NBIOT_ClientsTypeDef;	



void Net_NBIoT_Task(void);



#endif

