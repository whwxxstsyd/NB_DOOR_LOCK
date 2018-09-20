#ifndef __NBIOT_CONFIG_H
#define __NBIOT_CONFIG_H

#include "include.h"		
	

/* NBIOT NET RET CODE */
typedef enum
{
	NETIP_OK							= 100,											//正常
	NETIP_UNKNOWNERROR					= 101,											//未知错误
	NETIP_NOTSUPPORT					= 102,											//版本不支持
	NETIP_NOTVALID						= 103,											//非法请求
	NETIP_LOGINED						= 104,											//该用户已经登录
	NETIP_UNLOGINED					= 105,											//该用户未登录
	NETIP_USERORPWDERROR				= 106,											//用户名密码错误
	NETIP_NOPOWER						= 107,											//无权限
	NETIP_TIMEOUT						= 108,											//超时
	NETIP_NOTFOUND						= 109,											//查找失败,没有找到对应文件
	NETIP_FOUND							= 110,											//查找成功,返回全部文件
	NETIP_FOUNDPART					= 111,											//查找成功,返回部分文件
	NETIP_USEREXIST					= 112,											//该用户已经存在
	NETIP_USENOTEXIST					= 113,											//该用户不存在
	NETIP_GROUPEXIST					= 114,											//该用户组已经存在
	NETIP_GROUPNOTEXIST				= 115,											//该用户组不存在
	NETIP_NOTLINKEDIN					= 116,											//device has not linked in the gateway
	NETIP_OFFLINE						= 117,											//device has linked in the gateway,but is offline
	NETIP_NOTMACHED					= 118,											//device has linked in the gateway,but isn't offline
	NETIP_CACHEDSUCCESS				= 119,											//Cached Success
	NETIP_CACHEDFAIL					= 120,											//Cached Fall
	NETIP_NEEDUPDATE					= 121,											//Need Up Date
	NETIP_ERRORPARAM					= 122,											//错误参数
	NETIP_NEEDREBOOT					= 150,											//成功,设备需要重启
	NET_OPERATION_UPGRADE_ALAREADY_START	= 511,											//重发开启
	NET_OPERATION_UPGRADE_NOT_START		= 512,											//未开始升级
	NET_OPERATION_UPGRADE_FAILED			= 514,											//操作失败
	NET_OPERATION_UPGRADE_FAILED_BUSY		= 516,											//设备忙或服务器忙
	NET_OPERATION_UPGRADE_NO_POWER		= 517											//该升级由其他连接开启,无法停止
}NBIOT_RetCodeTypeDef;

/* NBIOT Status */
typedef enum
{
	/* -Normal Errors- */
	NBIOT_OK       						= 0x00,
	NBIOT_ERROR    						= 0x01,
	NBIOT_CMD_TIMEOUT					= 0x02,	
	
	/* -General Errors- */
	Operation_not_allowed				= 3,
	Operation_not_supported			= 4,
	Need_to_enter_PIN					= 5,
	Memory_failure						= 23,
	No_Network_Service					= 30,
	Incorrect_parameters				= 50,
	Command_implemented_but			= 51,
	Command_aborted_by_user			= 52,
	Uplink_Busy_Flow_Control			= 159,
	ME_failure							= 300,
	SMS_service_of_ME_reserved		= 301,
	operation_not_allowed				= 302,
	operation_not_supported			= 303,
	invalid_PDU_mode_parameter		= 304,
	invalid_text_mode_parameter		= 305,
	SIM_not_inserted					= 310,
	SIM_PIN_required					= 311,
	PH_SIM_PIN_required				= 312,
	SIM_failure							= 313,
	SIM_busy							= 314,
	SIM_wrong							= 315,
	SIM_PUK_required					= 316,
	SIM_PIN2_required					= 317,
	SIM_PUK2_required					= 318,
	memory_failure						= 320,
	invalid_memory_index				= 321,
	memory_full							= 322,
	SMSC_address_unknown				= 330,
	no_network_service					= 331,
	network_timeout					= 332,
	acknowledgement_expected			= 340,
	unknown_error						= 500,
	
	/* -HISI specific error codes- */
	Required_parameter_not_configured		= 512,
	TUP_not_registered						= 513,
	AT_Internal_Error						= 514,
	CID_is_active							= 515,
	Incorrect_State_for_Command			= 516,
	Cid_is_invalid							= 517,
	CID_is_not_active						= 518,
	Deactive_last_active_cid				= 520,
	Cid_is_not_defined						= 521,
	Uart_parity_error						= 522,
	Uart_frame_error						= 523,
	Mt_not_power_on						= 524,
	sent_data_sequence_repeat_error		= 525,
	at_command_abort_error					= 526,
	command_interrupted					= 527,
	Configuration_conflicts				= 528,
	FOTA_is_updating						= 529,
	Not_the_at_allocated_socket			= 530
}NBIOT_StatusTypeDef;


/* NBIOT BAND */
typedef enum
{
	NBand_2100MHz						= 1,
	NBand_1800MHz						= 3,
	NBand_850MHz						= 5,
	NBand_900MHz						= 8,
	NBand_800MHz						= 20,
	NBand_700MHz						= 28
}NBIOT_BandTypeDef;

/* NBIOT Detach or Attach Netstate */
typedef enum
{
	Detach							= 0x00,
	Attach							= 0x01
}NBIOT_NetstateTypeDef;

/* NBIOT MinFunc of FullFunc Functionality */
typedef enum
{
	MinFunc							= 0x00,
	FullFunc						= 0x01
}NBIOT_FunctionalityTypeDef;

/* NBIOT Function is Open? */
typedef enum
{
	CloseFunc							= 0x00,
	OpenFunc							= 0x01
}NBIOT_OpenOrCloseFuncTypeDef;

/* NBIOT NConfig is True? */
typedef enum
{
	NConfigFalse					= 0x00,
	NConfigTrue						= 0x01
}NBIOT_NConfigTypeDef;

/* NBIOT ReportError is Status */
typedef enum
{
	CMEDisable						= 0x00,
	CMEEnable						= 0x01
}NBIOT_ReportErrorTypeDef;

/* NBIOT Message Registration Status */
typedef enum
{
	UNINITIALISED					= 0x00,
	MISSING_CONFIG					= 0x01,
	INIT_FAILED						= 0x02,
	INIITIALISED					= 0x03,
	REGISTERING						= 0x04,
	REREGISTERING					= 0x05,
	REGISTERED						= 0x06,
	REREGISTERED					= 0x07,
	MO_DATA_ENABLED				= 0x08,
	NO_UE_IP						= 0x09,
	MEMORY_ERROR					= 0x0A,
	COAP_ERROR						= 0x0B,
	MSG_SEND_FAILED				= 0x0C,
	REJECTED_BY_SERVER				= 0x0D,
	TIMEOUT_AND_RETRYING			= 0x0E,
	TIMEOUT_AND_FAILED				= 0x0F
}NBIOT_MessageRegistrationStatusTypeDef;

/* NBIOT PSM Status */
typedef enum
{
	NormalMode						= 0x00,
	PowerSavingMode				= 0x01
}NBIOT_PSMStatusTypeDef;

/* NBIOT Connect Status */
typedef enum
{
	IdleMode							= 0x00,
	ConnectedMode						= 0x01
}NBIOT_ConnectStatusTypeDef;

/* NBIOT CON Status */
typedef enum
{
	Unsent							= 0x00,
	WaitResponse					= 0x01,
	SendFail						= 0x02,
	SendTimeout						= 0x03,
	SendSussess						= 0x04,
	RecvRST							= 0x05
}NBIOT_CONDataStatusTypeDef;

/* NBIOT Dictate Event */
typedef enum
{	
	STOP_MODE							= 0x00,											//停止模式
	HARDWARE_REBOOT					= 0x01,											//硬件重启
	REPORT_ERROE						= 0x02,											//错误输出
	MODULE_CHECK						= 0x03,											//模块检查
	PARAMETER_CONFIG					= 0x04,											//参数配置
	ICCID_CHECK							= 0x05,											//检测SIM卡
	MISC_EQUIP_CONFIG					= 0x06,											//其他配置
	ATTACH_CHECK						= 0x07,											//注网检查
	ATTACH_EXECUTE						= 0x08,											//注网进行
	ATTACH_INQUIRE						= 0x09,											//注网查询
	PARAMETER_CHECKOUT					= 0x0A,											//参数检出
	MINIMUM_FUNCTIONALITY				= 0x0B,											//最小功能
	FULL_FUNCTIONALITY					= 0x0C,											//完整功能
	CDP_SERVER_CHECK					= 0x0D,											//CDP服务器查询
	CDP_SERVER_CONFIG					= 0x0E,											//CDP服务器配置
	NBAND_MODE_CHECK					= 0x0F,											//NBAND模式查询
	NBAND_MODE_CONFIG					= 0x10,											//NBAND模式配置
	SEND_DATA							= 0x11,											//发送数据
	RECV_DATA							= 0x12,											//接收数据
	SEND_DATA_RA_NORMAL				= 0x13,											//发送数据RA_NORMAL
	RECV_DATA_RA_NORMAL				= 0x14,											//接收数据RA_NORMAL
	EXECUT_DOWNLINK_DATA				= 0x15,											//执行下行数据
	MQTTSN_PROCESS_STACK				= 0x16,											//MQTTSN执行栈
	DNS_PROCESS_STACK					= 0x17,											//DNS执行栈
	ONENET_PROCESS_STACK				= 0x18,											//ONENET执行栈
	LISTEN_RUN_CTL						= 0x19											//监听运行控制器
}NBIOT_DictateEventTypeDef;

	
/* NBIOT Listen Event */
typedef enum
{
	ENTER_IDLE_MODE					= 0x00,											//进入IDLE模式监听
	ENTER_PARAMETER_CHECKOUT			= 0x01											//进入NBIOT运行信息监听
}NBIOT_ListenEventTypeDef;	


/* NBIOT CDP Server Address */
typedef struct NBIOT_CDPServerTypeDef
{
	u8						CDPServerHost[16];
	u8						CDPServerPort;	
}NBIOT_CDPServerTypeDef;	





/* NBIOT Parameter */
typedef struct NBIOT_ParameterTypeDef
{
	char								manufacturer[10];								//厂商名
	char								manufacturermode[20];							//厂商型号
	char								modelversion[30];								//模块软件版本
	char								imei[20];										//IMEI
	char								imeisv[20];										//IMEISV
	char								iccid[25];										//运营商卡号
	char								imsi[20];										//IMSI
	char								cgpaddr[20];									//核心网地址
	char								cgdcont[30];									//核心网名称
	int									rssi;											//信号质量
	
	struct NetworkRegistrationStatusTypeDef
	{
		unsigned short int				tac;											//跟踪区域代码
		unsigned int					cellID;											//全球唯一基站标识ID
	}networkRegStatus;
	
	struct StatisticsRADIOTypeDef
	{
		int							Signalpower;										//信号功率
		int							Totalpower;											//总功率
		int							TXpower;											//发送功率
		unsigned int				TXtime;												//自上次重启后的总发送时间
		unsigned int				RXtime;												//自上次重启后的总接收时间
		unsigned int				CellID;												//全球唯一基站标识ID
		int							ECL;												//覆盖等级
		int							SNR;												//信噪比
		int							EARFCN;												//基站频点
		int							PCI;												//PCI
		int							RSRQ;												//参考信号接收质量
	}statisticsRADIO;
	
	struct StatisticsCELLTypeDef
	{
		int							earfcn;											//基站频点
		int							physicalcellID;								//小区物理ID
		int							primarycell;									//1标识当前服务小区
		int							rsrp;											//参考信号接收功率
		int							rsrq;											//参考信号接收质量
		int							rssi;											//信号质量
		int							snr;											//信噪比
	}statisticsCELL;
	
	struct DataTimeTypeDef
	{
		int							year;
		int							month;
		int							day;
		int							hour;
		int							min;
		int							sec;
		unsigned int				localstamp;
	}dataTime;
	
	struct COAPSendMessage
	{
		int							pending;
		int							sent;
		int							error;
	}coapSendMessage;
	
	struct COAPReadMessage
	{
		int							buffered;
		int							received;
		int							dropped;
	}coapReadMessage;
	
	struct NConfigTypeDef
	{
		NBIOT_NConfigTypeDef			autoConnect;
		NBIOT_NConfigTypeDef			crScrambling;
		NBIOT_NConfigTypeDef			crSiAvoid;
		NBIOT_NConfigTypeDef			combineAttach;
		NBIOT_NConfigTypeDef			cellReselection;
		NBIOT_NConfigTypeDef			enableBip;
	}nconfig;
	
	NBIOT_CONDataStatusTypeDef				condatastate;
	NBIOT_PSMStatusTypeDef					psmstate;
	NBIOT_ConnectStatusTypeDef				connectedstate;
	NBIOT_NetstateTypeDef					netstate;
	NBIOT_FunctionalityTypeDef				functionality;
	NBIOT_OpenOrCloseFuncTypeDef			nnmistate;
	NBIOT_OpenOrCloseFuncTypeDef			nsmistate;
	NBIOT_BandTypeDef						band;
	NBIOT_ReportErrorTypeDef				cmeestate;
	NBIOT_MessageRegistrationStatusTypeDef	messageregistration;
	NBIOT_CDPServerTypeDef					cdpserver;
	
}NBIOT_ParameterTypeDef;






/* NBIOT Clients */
typedef struct NBIOT_ClientsTypeDef
{
	u8*						Sendbuf;
	u8*						Recvbuf;
	size_t					Sendbuf_size;
	size_t					Recvbuf_size;
	short					Sendlen;
	short					Recvlen;
	u8*						DataProcessStack;
	size_t					DataProcessStack_size;
	u16						Command_Timeout_Msec;
	u16						Command_Failure_Cnt;
	
	u16						CoapConnectTimeSec;
	u16						CoapIdleTimeSec;
	u8						CoapConnectDayTimeSec;
	u8						CoapIdleDayTimeSec;	

	struct NBIOTDictateRuningCtlTypeDef
	{
		bool							dictateCoapRANormalEnable;
		bool							dictateEnable;	
		NBIOT_DictateEventTypeDef		dictateEvent;
	}DictateRunCtl;	

	bool								Registered;
	bool								NetStateIdentification;
	
	NBIOT_ParameterTypeDef				Parameter;

}NBIOT_ClientsTypeDef;










#endif


