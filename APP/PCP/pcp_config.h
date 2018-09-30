#ifndef __PCP_CONFIG_H
#define   __PCP_CONFIG_H

#include "sys.h"	
#include "nbiotconfig.h"

#define PCP_DEBUG_LOG_RF_PRINT															//定义开启RF输出DEBUG信息

#define PCP_COMMAND_TIMEOUT_SEC			30
#define PCP_COMMAND_FAILURE_CNT			3

/* PCP 协议栈开辟缓存大小 */
#define PCP_BUFFER_SIZE					512
#define PCP_DATASTACK_SIZE				512

#define PCP_START_X						0xFFFE
#define PCP_PROTOCOL_TYPE				0x01

typedef struct PCP_ParameterTypeDef		PCP_ParameterTypeDef;
typedef struct PCP_CoAPNetTransportTypeDef	PCP_CoAPNetTransportTypeDef;
typedef struct PCP_ClientsTypeDef			PCP_ClientsTypeDef;

/* PCP Status */
typedef enum
{
	PCP_OK       						= 0x00,
	PCP_ERROR    						= 0x01,
	PCP_CMD_TIMEOUT					= 0x02,
	
	/* -General Errors- */
	PCP_Operation_not_allowed			= 3,
	PCP_Operation_not_supported		= 4,
	PCP_Need_to_enter_PIN				= 5,
	PCP_Memory_failure					= 23,
	PCP_No_Network_Service				= 30,
	PCP_Incorrect_parameters			= 50,
	PCP_Command_implemented_but		= 51,
	PCP_Command_aborted_by_user		= 52,
	PCP_Uplink_Busy_Flow_Control		= 159,
	PCP_ME_failure						= 300,
	PCP_SMS_service_of_ME_reserved	= 301,
	PCP_operation_not_allowed			= 302,
	PCP_operation_not_supported		= 303,
	PCP_invalid_PDU_mode_parameter	= 304,
	PCP_invalid_text_mode_paramceter	= 305,
	PCP_SIM_not_inserted				= 310,
	PCP_SIM_PIN_required				= 311,
	PCP_PH_SIM_PIN_required			= 312,
	PCP_SIM_failure					= 313,
	PCP_SIM_busy						= 314,
	PCP_SIM_wrong						= 315,
	PCP_SIM_PUK_required				= 316,
	PCP_SIM_PIN2_required				= 317,
	PCP_SIM_PUK2_required				= 318,
	PCP_memory_failure					= 320,
	PCP_invalid_memory_index			= 321,
	PCP_memory_full					= 322,
	PCP_SMSC_address_unknown			= 330,
	PCP_no_network_service				= 331,
	PCP_network_timeout				= 332,
	PCP_acknowledgement_expected		= 340,
	PCP_unknown_error					= 500,
	
	/* -HISI specific error codes- */
	PCP_Required_parameter_not_configured	= 512,
	PCP_TUP_not_registered				= 513,
	PCP_AT_Internal_Error				= 514,
	PCP_CID_is_active					= 515,
	PCP_Incorrect_State_for_Command	= 516,
	PCP_Cid_is_invalid					= 517,
	PCP_Deactive_last_active_cid		= 520,
	PCP_Cid_is_not_defined				= 521,
	
	/* -PCP Private error codes- */
	PCP_Frame_Format_Error				= 600,
	PCP_Frame_CheckCode_Error			= 601,
	PCP_Frame_None						= 602,
	PCP_MessageType_Error				= 603,
	PCP_UpgradePack_Error				= 604,
	PCP_UpgradeDown_Error				= 605,
	PCP_Upgrade_Error					= 606
}PCP_StatusTypeDef;

/* PCP Message Code */
typedef enum
{
	PCP_QueryDeviceVersion				= 0x13,											//查询设备版本
	PCP_NewVersionNotice				= 0x14,											//新版本通知
	PCP_RequestUpgradePackage			= 0x15,											//请求升级包
	PCP_ReportDownloadStatus			= 0x16,											//上报升级包下载状态
	PCP_PerformUpgrade					= 0x17,											//执行升级
	PCP_ReportUpgrades					= 0x18											//上报升级结果
}PCP_MessageCodeTypeDef;

/* PCP Result Code */
typedef enum
{
	PCP_ExecuteSuccess					= 0x00,											//处理成功
	PCP_EquipmentUse					= 0x01,											//设备使用中
	PCP_SignalqualityDifference		= 0x02,											//信号质量差
	PCP_LatestVersion					= 0x03,											//已经是最新版本
	PCP_LackofElectricity				= 0x04,											//电量不足
	PCP_LackofSpace					= 0x05,											//剩余空间不足
	PCP_DownloadTimeout				= 0x06,											//下载超时
	PCP_UpgradePacketCheckFail		= 0x07,											//升级包校验失败
	PCP_UpgradePacketTypeNonsupport	= 0x08,											//升级包类型不支持
	PCP_InsufficientMemory				= 0x09,											//内存不足
	PCP_InstallationFailure			= 0x0A,											//安装升级包失败
	PCP_InternalAnomaly				= 0x7F,											//内部异常
	PCP_UpgradeTaskFail				= 0x80,											//升级任务不存在
	PCP_SliceNotFound					= 0x81											//指定分片不存在
}PCP_ResultCodeTypeDef;

/* PCP Dictate Event */
typedef enum
{
	PCP_EVENT_STOP						= 0x00,											//Stop
	PCP_EVENT_INITIALIZED				= 0x01,											//Initialized
	PCP_EVENT_READY					= 0x02,											//Ready
	PCP_EVENT_FRAME_RECV				= 0x03,											//RECV
	PCP_EVENT_FRAME_SEND				= 0x04,											//SEND
	PCP_EVENT_EXECUTE					= 0x05,											//Execute
	PCP_EVENT_ACTIVEUPLOAD				= 0x06											//ActiveUpload
}PCP_DictateEventTypeDef;

/* PCP Upgrade Status */
typedef enum
{
	PCP_UPGRADE_STANDBY				= 0x00,											//升级准备
	PCP_UPGRADE_QUERYVERSION			= 0x01,											//升级查询设备版本
	PCP_UPGRADE_DOWNLOAD				= 0x02,											//升级下载数据包
	PCP_UPGRADE_ASSEMBLE				= 0x03,											//升级组装升级包
	PCP_UPGRADE_INSTALL				= 0x04,											//升级版本
	PCP_UPGRADE_FAILED					= 0x05											//升级错误
}PCP_UpgradeStatusTypeDef;







/* PCP Message Data Structure */
typedef __packed struct
{
	unsigned short						StartX;											//起始标识	0xFFFE
	unsigned char						ProtocolType;										//版本号		0x01
	unsigned char						MessageType;										//消息码		0xXX
	unsigned short						CRCCheckCode;										//检验码		CRC16
	unsigned short						PacketDataLength;									//数据区长度
	unsigned char						pPacketData[1];									//数据区
}PCP_MessageDataTypeDef;


	



/* PCP Parameter */
struct PCP_ParameterTypeDef
{
	unsigned char						ProtocolType;										//版本号
	unsigned char						MessageType;										//消息码
	unsigned char						PlatformSoftVersion[16];								//目的版本号
	unsigned short						UpgradePackSliceSize;								//升级包分片大小
	unsigned short						UpgradePackSliceNum;								//升级包分片总数
	unsigned short						UpgradePackCheckCode;								//升级包校验码
	unsigned short						UpgradePackSliceIndex;								//升级包分片序号
};

/* PCP CoAP Transport */
struct PCP_CoAPNetTransportTypeDef
{
	NBIOT_ClientsTypeDef*				NBIotStack;
	PCP_StatusTypeDef					(*Write)(PCP_CoAPNetTransportTypeDef*, const char*, u16);
	PCP_StatusTypeDef					(*Read)(PCP_CoAPNetTransportTypeDef*, char*, u16*);
};

/* PCP Clients */
struct PCP_ClientsTypeDef
{
	unsigned char*						Sendbuf;
	unsigned char*						Recvbuf;
	size_t							Sendbuf_size;
	size_t							Recvbuf_size;
	short							Sendlen;
	short							Recvlen;
	unsigned char*						DataProcessStack;
	size_t							DataProcessStack_size;
	unsigned short						Command_Timeout_Sec;
	unsigned short						Command_Failure_Cnt;
	
	struct PCPDictateRuningCtlTypeDef
	{
		bool							dictateEnable;
		unsigned int					dictateTimeoutSec;
		unsigned char					dictateInitializedFailureCnt;
		unsigned char					dictateReadyFailureCnt;
		unsigned char					dictateRecvFailureCnt;
		unsigned char					dictateSendFailureCnt;
		unsigned char					dictateExecuteFailureCnt;
		unsigned char					dictateActiveUploadFailureCnt;
		unsigned char					dictateUpgradeQueryVersionCnt;
		unsigned char					dictateUpgradeDownloadCnt;
		unsigned char					dictateUpgradeAssembleCnt;
		unsigned char					dictateUpgradeInstallCnt;
		Stm32_CalculagraphTypeDef		dictateRunTime;
		PCP_DictateEventTypeDef			dictateEvent;
	}DictateRunCtl;
	
	struct PCPUpgradeExecutionTypeDef
	{
		PCP_UpgradeStatusTypeDef		upgradeStatus;
		unsigned char					DeviceSoftVersion[16];
		unsigned char					PlatformSoftVersion[16];
		unsigned short					PackSliceIndex;
		unsigned short					PackSliceSize;
		unsigned short					PackLastSliceSize;
		unsigned short					PackSliceNum;
		unsigned short					PackCheckCode;
	}UpgradeExecution;
	
	PCP_ParameterTypeDef				Parameter;
	PCP_CoAPNetTransportTypeDef*		CoAPStack;	
	NET_NBIOT_ClientsTypeDef*			NetNbiotStack;
};

/* Application Programming Interface */
void PCP_Client_Init(PCP_ClientsTypeDef* pClient, PCP_CoAPNetTransportTypeDef* NetSock, NET_NBIOT_ClientsTypeDef* NetNbiotStack);				//PCP客户端初始化
PCP_ResultCodeTypeDef PCP_UpgradeDataNewVersionNotice_Callback(PCP_ClientsTypeDef* pClient);											//PCP新版本通知处理
PCP_ResultCodeTypeDef PCP_UpgradeDataDownload_Callback(PCP_ClientsTypeDef* pClient, u16 SliceIndex, u8* UpgradeData, u16 UpgradeDataLength);	//PCP升级包下载回调
PCP_ResultCodeTypeDef PCP_UpgradeDataAssemble_Callback(PCP_ClientsTypeDef* pClient);													//PCP升级包组装回调
PCP_ResultCodeTypeDef PCP_UpgradeDataReportUpgrades_Callback(PCP_ClientsTypeDef* pClient);											//PCP上报升级结果回调

#endif /* __PCP_CONFIG_H */

