#ifndef __ONENET_CONFIG_H
#define   __ONENET_CONFIG_H

#include "sys.h"
#include "nbiotconfig.h"
#include "net_nbiot_app.h"

#define ONENET_COMMAND_TIMEOUT_SEC			30
#define ONENET_COMMAND_FAILURE_CNT			3

/* ONENET 协议栈开辟缓存大小 */
#define ONENET_BUFFER_SIZE				512
#define ONENET_DATASTACK_SIZE				512


typedef struct ONENET_ParameterTypeDef		ONENET_ParameterTypeDef;
typedef struct ONENET_LWM2MTransportTypeDef	ONENET_LWM2MTransportTypeDef;
typedef struct ONENET_ClientsTypeDef		ONENET_ClientsTypeDef;



/* ONENET Status */
typedef enum
{
	ONENET_OK       					= 0x00,
	ONENET_ERROR    					= 0x01,
	ONENET_CMD_TIMEOUT					= 0x02,
	
	/* -General Errors- */
	ONENET_Operation_not_allowed			= 3,
	ONENET_Operation_not_supported		= 4,
	ONENET_Need_to_enter_PIN				= 5,
	ONENET_Memory_failure				= 23,
	ONENET_No_Network_Service			= 30,
	ONENET_Incorrect_parameters			= 50,
	ONENET_Command_implemented_but		= 51,
	ONENET_Command_aborted_by_user		= 52,
	ONENET_Uplink_Busy_Flow_Control		= 159,
	ONENET_ME_failure					= 300,
	ONENET_SMS_service_of_ME_reserved		= 301,
	ONENET_operation_not_allowed			= 302,
	ONENET_operation_not_supported		= 303,
	ONENET_invalid_PDU_mode_parameter		= 304,
	ONENET_invalid_text_mode_parameter		= 305,
	ONENET_SIM_not_inserted				= 310,
	ONENET_SIM_PIN_required				= 311,
	ONENET_PH_SIM_PIN_required			= 312,
	ONENET_SIM_failure					= 313,
	ONENET_SIM_busy					= 314,
	ONENET_SIM_wrong					= 315,
	ONENET_SIM_PUK_required				= 316,
	ONENET_SIM_PIN2_required				= 317,
	ONENET_SIM_PUK2_required				= 318,
	ONENET_memory_failure				= 320,
	ONENET_invalid_memory_index			= 321,
	ONENET_memory_full					= 322,
	ONENET_SMSC_address_unknown			= 330,
	ONENET_no_network_service			= 331,
	ONENET_network_timeout				= 332,
	ONENET_acknowledgement_expected		= 340,
	ONENET_unknown_error				= 500,
	
	/* -HISI specific error codes- */
	ONENET_Required_parameter_not_configured= 512,
	ONENET_TUP_not_registered			= 513,
	ONENET_AT_Internal_Error				= 514,
	ONENET_CID_is_active				= 515,
	ONENET_Incorrect_State_for_Command		= 516,
	ONENET_Cid_is_invalid				= 517,
	ONENET_CID_is_not_active				= 518,
	ONENET_Deactive_last_active_cid		= 520,
	ONENET_Cid_is_not_defined			= 521,
	ONENET_Uart_parity_error				= 522,
	ONENET_Uart_frame_error				= 523,
	ONENET_Mt_not_power_on				= 524,
	ONENET_sent_data_sequence_repeat_error	= 525,
	ONENET_at_command_abort_error			= 526,
	ONENET_command_interrupted			= 527,
	ONENET_Configuration_conflicts		= 528,
	ONENET_FOTA_is_updating				= 529,
	ONENET_Not_the_at_allocated_socket		= 530
}ONENET_StatusTypeDef;

/* ONENET Is ProcessState */
typedef enum
{
	ONENET_PROCESSSTATE_INIT				= 0x00,
	
	
	
	
}ONENET_ProcessStateTypeDef;

/* ONENET Parameter */
struct ONENET_ParameterTypeDef
{
	char								suiteVersion[20];									//通信套件版本
	
	
	
};




/* ONENET Transport */
struct ONENET_LWM2MTransportTypeDef
{
	NBIOT_ClientsTypeDef*				NBIotStack;
	
	
	ONENET_StatusTypeDef				(*Write)(ONENET_LWM2MTransportTypeDef*, const char*, u16);
	ONENET_StatusTypeDef				(*Read)(ONENET_LWM2MTransportTypeDef*, char*, u16*);
};

/* ONENET Clients */
struct ONENET_ClientsTypeDef
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
	
	struct ONENETDictateRuningCtlTypeDef
	{
		bool							dictateEnable;
		unsigned int					dictateTimeoutSec;
		unsigned char					dictateInitFailureCnt;
		
		
		Stm32_CalculagraphTypeDef		dictateRunTime;
	}DictateRunCtl;
	
	
	
	
	
	ONENET_ParameterTypeDef				Parameter;
	ONENET_ProcessStateTypeDef			ProcessState;
	ONENET_LWM2MTransportTypeDef*			LWM2MStack;
	NET_NBIOT_ClientsTypeDef*			NetNbiotStack;
};











/* Application Programming Interface */
void OneNET_Client_Init(ONENET_ClientsTypeDef* pClient, ONENET_LWM2MTransportTypeDef* NetSock, NET_NBIOT_ClientsTypeDef* NetNbiotStack);	//ONENET客户端初始化

#endif /* __ONENET_CONFIG_H */
