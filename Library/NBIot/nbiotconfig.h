#ifndef __NBIOT_CONFIG_H
#define   __NBIOT_CONFIG_H

#include "sys.h"
#include "net_nbiot_app.h"

#define NBIOT_RESET_GPIOx				GPIOB
#define NBIOT_RESET_PIN					GPIO_PIN_2
#define NBIOT_RESET_RCC_GPIO_CLK_ENABLE()	__HAL_RCC_GPIOB_CLK_ENABLE()
#define NBIOT_RESET_RCC_GPIO_CLK_DISABLE()	__HAL_RCC_GPIOB_CLK_DISABLE()

#define NBIOT_SERIAL_TIMEOUT_MSEC			100												//���������������ݰ���ʱʱ��MS
#define NBIOT_COMMAND_TIMEOUT_MSEC			8000												//ָ��͵ȴ���Ӧ����ʱʱ��MS
#define NBIOT_COMMAND_FAILURE_CNT			3												//ָ��ִ�д����Դ���

/* NBIOT Э��ջ���ٻ����С */
#define NBIOT_BUFFER_SIZE				512
#define NBIOT_ATBUFFER_SIZE				1100
#define NBIOT_DATASTACK_SIZE				1100

/* NBIOT ���� */
#define NBIOT_MANUFACTURER_LIERDA			"Lierda"
#define NBIOT_MANUFACTURER_QUECTEL			"Quectel"

/* NCONFIG���ò��� */
#define AutoConnect						"AUTOCONNECT"										//�Զ�ע��ģʽ
#define CrScrambling					"CR_0354_0338_SCRAMBLING"							//���빦��
#define CrSiAvoid						"CR_0859_SI_AVOID"									//���빦��
#define CombineAttach					"COMBINE_ATTACH"									//���ϸ���
#define CellReselection					"CELL_RESELECTION"									//С����ѡ
#define EnableBip						"ENABLE_BIP"										//����

#define AutoConnectVal					NConfigTrue										//True
#define CrScramblingVal					NConfigTrue										//True
#define CrSiAvoidVal					NConfigTrue										//True
#define CombineAttachVal					NConfigFalse										//False
#define CellReselectionVal				NConfigTrue										//True
#define EnableBipVal					NConfigFalse										//False

/* NBIOT �¼����������� */
#define NBCOAP_LISTEN_MODE_ENTER_IDLE		ENTER_IDLE_MODE
#define NBCOAP_LISTEN_MODE_ENTER_PARAMETER	ENTER_PARAMETER_CHECKOUT
#define NBCOAP_LISTEN_DEFAULT_BOOTMODE		NBCOAP_LISTEN_MODE_ENTER_PARAMETER						//Coap����NBĬ����ʼģʽ

#define NBCOAP_LISTEN_ENTER_IDLE_SEC		35												//Coap����NB����IDLE�ȴ�ʱ��
#define NBCOAP_LISTEN_ENTER_PARAMETER_SEC	15												//Coap����NB����������ȴ�ʱ��

#define NBCOAP_LISTEN_IDLE_DISABLE			0
#define NBCOAP_LISTEN_IDLE_ENABLE			1
#define NBCOAP_LISTEN_IDLE_TYPE			NBCOAP_LISTEN_IDLE_DISABLE							//Coap����NB��������ģʽ

#define NBCOAP_LISTEN_PARAMETER_DISABLE		0
#define NBCOAP_LISTEN_PARAMETER_ENABLE		1
#define NBCOAP_LISTEN_PARAMETER_TYPE		NBCOAP_LISTEN_PARAMETER_ENABLE						//Coap����NB����������ģʽ

/* NBIOT ���ڲ����ʼ����м�ֵ */
#define BAUDRATE_CAL_MIDDLE_NUM			16

/* ��������Э��ֵƫ�Ƶ�ַ */
#define TCLOD_MSGTYPE_OFFSET				4
#define TCLOD_MSGVERSION_OFFSET			5
#define TCLOD_MSGENCRYPT_OFFSET			6
#define TCLOD_MSGID_OFFSET				7
#define TCLOD_DATA_OFFSET				8

/* ��������Э��ֵ */
#define TCLOD_MSGTYPE					'1'
#define TCLOD_MSGVERSION					'1'
#define TCLOD_MSGENCRYPT					'0'

#define TCLOD_MAGIC_NUM					9

typedef struct NBIOT_CDPServerTypeDef		NBIOT_CDPServerTypeDef;
typedef struct NBIOT_ParameterTypeDef		NBIOT_ParameterTypeDef;
typedef struct NBIOT_ATCmdTypeDef			NBIOT_ATCmdTypeDef;
typedef struct NBIOT_ClientsTypeDef		NBIOT_ClientsTypeDef;

/* NBIOT NET MSGID */
typedef enum
{
	TCLOD_LOGIN_REQ					= '0',
	TCLOD_LOGIN_RSP					= '1',
	TCLOD_LOGOUT_REQ					= '2',
	TCLOD_LOGOUT_RSP					= '3',
	
	TCLOD_KEEPALIVE_REQ					= '4',
	TCLOD_KEEPALIVE_RSP					= '5',
	TCLOD_SYSINFO_REQ					= '6',
	TCLOD_SYSINFO_RSP					= '7',
	TCLOD_CONFIG_SET					= 'S',
	TCLOD_CONFIG_SET_RSP				= 's',
	TCLOD_CONFIG_GET					= 'G',
	TCLOD_CONFIG_GET_RSP				= 'g',
	
	TCLOD_MODIFYUSER_REQ				= 'c',
	TCLOD_MODIFYUSER_RSP				= 'd',
	TCLOD_MODIFYPASSWORD_REQ				= 'e',
	TCLOD_MODIFYPASSWORD_RSP				= 'f',
	
	TCLOD_CLOUD_VERSION_REQ				= 'g',
	TCLOD_CLOUD_VERSION_RSP				= 'i',
	TCLOD_CLOUD_UPGRADE_START_REQ			= 'j',
	TCLOD_CLOUD_UPGRADE_START_RSP			= 'k',
	TCLOD_CLOUD_UPGRADE_STOP_REQ			= 'l',
	TCLOD_CLOUD_UPGRADE_STOP_RSP			= 'm',
	TCLOD_CLOUD_UPGRADE_PROGRESS			= 'n',
	TCLOD_CLOUD_UPGRADE_DATA				= 'U',
	TCLOD_CLOUD_UPGRADE_DATA_RSP			= 'u',
}NBIOT_MsgIDTypeDef;

/* NBIOT NET RET CODE */
typedef enum
{
	NETIP_OK							= 100,											//����
	NETIP_UNKNOWNERROR					= 101,											//δ֪����
	NETIP_NOTSUPPORT					= 102,											//�汾��֧��
	NETIP_NOTVALID						= 103,											//�Ƿ�����
	NETIP_LOGINED						= 104,											//���û��Ѿ���¼
	NETIP_UNLOGINED					= 105,											//���û�δ��¼
	NETIP_USERORPWDERROR				= 106,											//�û����������
	NETIP_NOPOWER						= 107,											//��Ȩ��
	NETIP_TIMEOUT						= 108,											//��ʱ
	NETIP_NOTFOUND						= 109,											//����ʧ��,û���ҵ���Ӧ�ļ�
	NETIP_FOUND						= 110,											//���ҳɹ�,����ȫ���ļ�
	NETIP_FOUNDPART					= 111,											//���ҳɹ�,���ز����ļ�
	NETIP_USEREXIST					= 112,											//���û��Ѿ�����
	NETIP_USENOTEXIST					= 113,											//���û�������
	NETIP_GROUPEXIST					= 114,											//���û����Ѿ�����
	NETIP_GROUPNOTEXIST					= 115,											//���û��鲻����
	NETIP_NOTLINKEDIN					= 116,											//device has not linked in the gateway
	NETIP_OFFLINE						= 117,											//device has linked in the gateway,but is offline
	NETIP_NOTMACHED					= 118,											//device has linked in the gateway,but isn't offline
	NETIP_CACHEDSUCCESS					= 119,											//Cached Success
	NETIP_CACHEDFAIL					= 120,											//Cached Fall
	NETIP_NEEDUPDATE					= 121,											//Need Up Date
	NETIP_ERRORPARAM					= 122,											//�������
	NETIP_NEEDREBOOT					= 150,											//�ɹ�,�豸��Ҫ����
	NET_OPERATION_UPGRADE_ALAREADY_START	= 511,											//�ط�����
	NET_OPERATION_UPGRADE_NOT_START		= 512,											//δ��ʼ����
	NET_OPERATION_UPGRADE_FAILED			= 514,											//����ʧ��
	NET_OPERATION_UPGRADE_FAILED_BUSY		= 516,											//�豸æ�������æ
	NET_OPERATION_UPGRADE_NO_POWER		= 517											//���������������ӿ���,�޷�ֹͣ
}NBIOT_RetCodeTypeDef;

/* NBIOT Status */
typedef enum
{
	/* -Normal Errors- */
	NBIOT_OK       					= 0x00,
	NBIOT_ERROR    					= 0x01,
	NBIOT_CMD_TIMEOUT					= 0x02,
	
	/* -General Errors- */
	Operation_not_allowed				= 3,
	Operation_not_supported				= 4,
	Need_to_enter_PIN					= 5,
	Memory_failure						= 23,
	No_Network_Service					= 30,
	Incorrect_parameters				= 50,
	Command_implemented_but				= 51,
	Command_aborted_by_user				= 52,
	Uplink_Busy_Flow_Control				= 159,
	ME_failure						= 300,
	SMS_service_of_ME_reserved			= 301,
	operation_not_allowed				= 302,
	operation_not_supported				= 303,
	invalid_PDU_mode_parameter			= 304,
	invalid_text_mode_parameter			= 305,
	SIM_not_inserted					= 310,
	SIM_PIN_required					= 311,
	PH_SIM_PIN_required					= 312,
	SIM_failure						= 313,
	SIM_busy							= 314,
	SIM_wrong							= 315,
	SIM_PUK_required					= 316,
	SIM_PIN2_required					= 317,
	SIM_PUK2_required					= 318,
	memory_failure						= 320,
	invalid_memory_index				= 321,
	memory_full						= 322,
	SMSC_address_unknown				= 330,
	no_network_service					= 331,
	network_timeout					= 332,
	acknowledgement_expected				= 340,
	unknown_error						= 500,
	
	/* -HISI specific error codes- */
	Required_parameter_not_configured		= 512,
	TUP_not_registered					= 513,
	AT_Internal_Error					= 514,
	CID_is_active						= 515,
	Incorrect_State_for_Command			= 516,
	Cid_is_invalid						= 517,
	CID_is_not_active					= 518,
	Deactive_last_active_cid				= 520,
	Cid_is_not_defined					= 521,
	Uart_parity_error					= 522,
	Uart_frame_error					= 523,
	Mt_not_power_on					= 524,
	sent_data_sequence_repeat_error		= 525,
	at_command_abort_error				= 526,
	command_interrupted					= 527,
	Configuration_conflicts				= 528,
	FOTA_is_updating					= 529,
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
	FullFunc							= 0x01
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
	NConfigFalse						= 0x00,
	NConfigTrue						= 0x01
}NBIOT_NConfigTypeDef;

/* NBIOT ReportError is Status */
typedef enum
{
	CMEDisable						= 0x00,
	CMEEnable							= 0x01
}NBIOT_ReportErrorTypeDef;

/* NBIOT Message Registration Status */
typedef enum
{
	UNINITIALISED						= 0x00,
	MISSING_CONFIG						= 0x01,
	INIT_FAILED						= 0x02,
	INIITIALISED						= 0x03,
	REGISTERING						= 0x04,
	REREGISTERING						= 0x05,
	REGISTERED						= 0x06,
	REREGISTERED						= 0x07,
	MO_DATA_ENABLED					= 0x08,
	NO_UE_IP							= 0x09,
	MEMORY_ERROR						= 0x0A,
	COAP_ERROR						= 0x0B,
	MSG_SEND_FAILED					= 0x0C,
	REJECTED_BY_SERVER					= 0x0D,
	TIMEOUT_AND_RETRYING				= 0x0E,
	TIMEOUT_AND_FAILED					= 0x0F
}NBIOT_MessageRegistrationStatusTypeDef;

/* NBIOT PSM Status */
typedef enum
{
	NormalMode						= 0x00,
	PowerSavingMode					= 0x01
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
	WaitResponse						= 0x01,
	SendFail							= 0x02,
	SendTimeout						= 0x03,
	SendSussess						= 0x04,
	RecvRST							= 0x05
}NBIOT_CONDataStatusTypeDef;

/* NBIOT Dictate Event */
typedef enum
{
	STOP_MODE							= 0x00,											//ֹͣģʽ
	HARDWARE_REBOOT					= 0x01,											//Ӳ������
	REPORT_ERROE						= 0x02,											//�������
	MODULE_CHECK						= 0x03,											//ģ����
	PARAMETER_CONFIG					= 0x04,											//��������
	ICCID_CHECK						= 0x05,											//���SIM��
	MISC_EQUIP_CONFIG					= 0x06,											//��������
	ATTACH_CHECK						= 0x07,											//ע�����
	ATTACH_EXECUTE						= 0x08,											//ע������
	ATTACH_INQUIRE						= 0x09,											//ע����ѯ
	PARAMETER_CHECKOUT					= 0x0A,											//�������
	MINIMUM_FUNCTIONALITY				= 0x0B,											//��С����
	FULL_FUNCTIONALITY					= 0x0C,											//��������
	CDP_SERVER_CHECK					= 0x0D,											//CDP��������ѯ
	CDP_SERVER_CONFIG					= 0x0E,											//CDP����������
	NBAND_MODE_CHECK					= 0x0F,											//NBANDģʽ��ѯ
	NBAND_MODE_CONFIG					= 0x10,											//NBANDģʽ����
	SEND_DATA							= 0x11,											//��������
	RECV_DATA							= 0x12,											//��������
	SEND_DATA_RA_NORMAL					= 0x13,											//��������RA_NORMAL
	RECV_DATA_RA_NORMAL					= 0x14,											//��������RA_NORMAL
	EXECUT_DOWNLINK_DATA				= 0x15,											//ִ����������
	MQTTSN_PROCESS_STACK				= 0x16,											//MQTTSNִ��ջ
	DNS_PROCESS_STACK					= 0x17,											//DNSִ��ջ
	ONENET_PROCESS_STACK				= 0x18,											//ONENETִ��ջ
	LISTEN_RUN_CTL						= 0x19											//�������п�����
}NBIOT_DictateEventTypeDef;

/* NBIOT Listen Event */
typedef enum
{
	ENTER_IDLE_MODE					= 0x00,											//����IDLEģʽ����
	ENTER_PARAMETER_CHECKOUT				= 0x01											//����NBIOT������Ϣ����
}NBIOT_ListenEventTypeDef;

/* NBIOT CDP Server */
typedef __packed struct NBIOT_ServerAddrTypeDef
{
	__packed union
	{
		unsigned char					ip8[4];
		unsigned int					ip32;
	}ip;
	unsigned short						port;
}NBIOT_ServerAddr;

/* NBIOT CDP Server Address */
struct NBIOT_CDPServerTypeDef
{
	unsigned char						CDPServerHost[16];
	unsigned short						CDPServerPort;
};

/* NBIOT Parameter */
struct NBIOT_ParameterTypeDef
{
	char								manufacturer[10];									//������
	char								manufacturermode[20];								//�����ͺ�
	char								modelversion[30];									//ģ������汾
	char								imei[20];											//IMEI
	char								imeisv[20];										//IMEISV
	char								iccid[25];										//��Ӫ�̿���
	char								imsi[20];											//IMSI
	char								cgpaddr[20];										//��������ַ
	char								cgdcont[30];										//����������
	int								rssi;											//�ź�����
	
	struct NetworkRegistrationStatusTypeDef
	{
		unsigned short int				tac;												//�����������
		unsigned int					cellID;											//ȫ��Ψһ��վ��ʶID
	}networkRegStatus;
	
	struct StatisticsRADIOTypeDef
	{
		int							Signalpower;										//�źŹ���
		int							Totalpower;										//�ܹ���
		int							TXpower;											//���͹���
		unsigned int					TXtime;											//���ϴ���������ܷ���ʱ��
		unsigned int					RXtime;											//���ϴ���������ܽ���ʱ��
		unsigned int					CellID;											//ȫ��Ψһ��վ��ʶID
		int							ECL;												//���ǵȼ�
		int							SNR;												//�����
		int							EARFCN;											//��վƵ��
		int							PCI;												//PCI
		int							RSRQ;											//�ο��źŽ�������
	}statisticsRADIO;
	
	struct StatisticsCELLTypeDef
	{
		int							earfcn;											//��վƵ��
		int							physicalcellID;									//С������ID
		int							primarycell;										//1��ʶ��ǰ����С��
		int							rsrp;											//�ο��źŽ��չ���
		int							rsrq;											//�ο��źŽ�������
		int							rssi;											//�ź�����
		int							snr;												//�����
	}statisticsCELL;
	
	struct DataTimeTypeDef
	{
		int							year;
		int							month;
		int							day;
		int							hour;
		int							min;
		int							sec;
		unsigned int					localstamp;
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
	
	NBIOT_CONDataStatusTypeDef			condatastate;
	NBIOT_PSMStatusTypeDef				psmstate;
	NBIOT_ConnectStatusTypeDef			connectedstate;
	NBIOT_NetstateTypeDef				netstate;
	NBIOT_FunctionalityTypeDef			functionality;
	NBIOT_OpenOrCloseFuncTypeDef			nnmistate;
	NBIOT_OpenOrCloseFuncTypeDef			nsmistate;
	NBIOT_BandTypeDef					band;
	NBIOT_ReportErrorTypeDef				cmeestate;
	NBIOT_MessageRegistrationStatusTypeDef	messageregistration;
	NBIOT_CDPServerTypeDef				cdpserver;
};

/* NBIOT BaudrateCal */
typedef __packed struct
{
	unsigned int						StartMs;
	unsigned int						StartClock;
	unsigned int						EndMs;
	unsigned int						EndClock;
	unsigned int						OverFlow;
	unsigned int						MiddleMs;
	unsigned int						MiddleClock;
	unsigned char						MiddleNum;
	unsigned char						TotalNum;
}NBIOT_BaudrateCalTypeDef;

/* NBIOT Baudrate */
typedef struct
{
	bool								EnBaudRateState;
	unsigned int						Baud;
	NBIOT_BaudrateCalTypeDef				NBIOTBaudRateNow;
	NBIOT_BaudrateCalTypeDef				NBIOTBaudRateCal;
}NBIOT_BaudrateTypeDef;

/* NBIOT ATCmd */
struct NBIOT_ATCmdTypeDef
{
	unsigned char*						ATSendbuf;
	unsigned char*						ATRecvbuf;
	size_t							ATSendbuf_size;
	size_t							ATRecvbuf_size;
	short							ATSendlen;
	short							ATRecvlen;
	char*							ATack;
	char*							ATNack;
	Stm32_CalculagraphTypeDef			CmdWaitTime;
	Stm32_CalculagraphTypeDef			SerialWaitTime;
	NBIOT_StatusTypeDef					(*Write)(NBIOT_ATCmdTypeDef*);
	NBIOT_StatusTypeDef 				(*Read)(NBIOT_ATCmdTypeDef*);
};

/* NBIOT Clients */
struct NBIOT_ClientsTypeDef
{
	unsigned char*						Sendbuf;
	unsigned char*						Recvbuf;
	size_t								Sendbuf_size;
	size_t								Recvbuf_size;
	short								Sendlen;
	short								Recvlen;
	unsigned char*						DataProcessStack;
	size_t								DataProcessStack_size;
	unsigned int						Command_Timeout_Msec;
	unsigned int						Command_Failure_Cnt;
	
	unsigned int						CoapConnectTimeSec;
	unsigned int						CoapIdleTimeSec;
	unsigned short						CoapConnectDayTimeSec;
	unsigned short						CoapIdleDayTimeSec;
	Stm32_EventRunningTimeTypeDef		ConnectTimeMS;
	Stm32_EventRunningTimeTypeDef		IdleTimeMS;
	
	/* �¼����п����� */
	struct NBIOTDictateRuningCtlTypeDef
	{
		bool							dictateCoapRANormalEnable;
		bool							dictateEnable;
		unsigned int					dictateTimeoutSec;
		unsigned char					dictateRebootFailureCnt;
		unsigned char					dictateReportErrorFailureCnt;
		unsigned char					dictateModuleCheckFailureCnt;
		unsigned char					dictateParameterConfigFailureCnt;
		unsigned char					dictateSimICCIDCheckFailureCnt;
		unsigned char					dictateMiscEquipConfigFailureCnt;
		unsigned char					dictateAttachCheckFailureCnt;
		unsigned char					dictateAttachExecuteFailureCnt;
		unsigned char					dictateAttachInquireFailureCnt;
		unsigned char					dictatePatameterCheckOutFailureCnt;
		unsigned char					dictateFullFunctionalityFailureCnt;
		unsigned char					dictateMinimumFunctionalityFailureCnt;
		unsigned char					dictateCDPServerCheckFailureCnt;
		unsigned char					dictateCDPServerConfigFailureCnt;
		unsigned char					dictateNbandModeCheckFailureCnt;
		unsigned char					dictateNbandModeConfigFailureCnt;
		unsigned char					dictateSendDataFailureCnt;
		unsigned char					dictateRecvDataFailureCnt;
		unsigned char					dictateSendDataRANormalFailureCnt;
		unsigned char					dictateRecvDataRANormalFailureCnt;
		unsigned char					dictateListenRunCtlFailureCnt;
		Stm32_CalculagraphTypeDef		dictateRunTime;
		NBIOT_DictateEventTypeDef		dictateEvent;
	}DictateRunCtl;
	
	/* �¼����м����� */
	struct NBIOTListenRuningCtlTypeDef
	{
#if NBCOAP_LISTEN_IDLE_TYPE == NBCOAP_LISTEN_IDLE_ENABLE
		struct ListenEnterIdleTypeDef
		{
			bool						listenEnable;
			bool						listenStatus;
			unsigned int				listenTimereachSec;
			Stm32_CalculagraphTypeDef	listenRunTime;
			
			struct EventCtlIdleTypedef
			{
				bool						eventEnable;
				unsigned int				eventTimeoutSec;
				unsigned char				eventFailureCnt;
				Stm32_CalculagraphTypeDef	eventRunTime;
			}EventCtl;
		}ListenEnterIdle;
#endif
#if NBCOAP_LISTEN_PARAMETER_TYPE == NBCOAP_LISTEN_PARAMETER_ENABLE
		struct ListenEnterParameterTypeDef
		{
			bool						listenEnable;
			bool						listenStatus;
			unsigned int				listenTimereachSec;
			Stm32_CalculagraphTypeDef	listenRunTime;
			
			struct EventCtlParameterTypedef
			{
				bool						eventEnable;
				unsigned int				eventTimeoutSec;
				unsigned char				eventFailureCnt;
				Stm32_CalculagraphTypeDef	eventRunTime;
			}EventCtl;
		}ListenEnterParameter;
#endif
		
		NBIOT_ListenEventTypeDef			listenEvent;
	}ListenRunCtl;
	
	bool								Registered;
	bool								NetStateIdentification;
	NBIOT_ParameterTypeDef				Parameter;
	NBIOT_ATCmdTypeDef*					ATCmdStack;
	NET_NBIOT_ClientsTypeDef*			NetNbiotStack;
};

extern NBIOT_BaudrateTypeDef NBIOTBaudRate;;																		//NBIOT�����ʼ���

void NBIOT_Client_Init(NBIOT_ClientsTypeDef* pClient, NBIOT_ATCmdTypeDef* ATCmdStack, NET_NBIOT_ClientsTypeDef* NetNbiotStack);	//NBIOT�ͻ��˳�ʼ��

void NBIOT_COAP_RA_NORMAL_SET_STATE(NBIOT_ClientsTypeDef* pClient, bool enable);										//����COAP����RA����Normal��״̬
bool NBIOT_COAP_RA_NORMAL_GET_STATE(NBIOT_ClientsTypeDef* pClient);													//��ȡCOAP����RA����Normal��״̬

#endif /* __NBIOT_CONFIG_H */

