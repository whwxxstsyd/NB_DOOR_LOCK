#include "nbiot_config.h"	
#include "net_app_nbiot.h"	
#include "sys.h"	


void NBIOT_Client_Init(NBIOT_ClientsTypeDef* pClient, NET_NBIOT_ClientsTypeDef* NetNbiotStack)
{		
	
	/* 事件运行控制器 */	
	pClient->DictateRunCtl.dictateCoapRANormalEnable				= FALSE;
	pClient->DictateRunCtl.dictateEnable							= FALSE;
	pClient->DictateRunCtl.dictateTimeoutSec						= 0;
	pClient->DictateRunCtl.dictateRebootFailureCnt					= 0;
	pClient->DictateRunCtl.dictateModuleCheckFailureCnt				= 0;
	pClient->DictateRunCtl.dictateParameterConfigFailureCnt			= 0;
	pClient->DictateRunCtl.dictateSimICCIDCheckFailureCnt			= 0;
	pClient->DictateRunCtl.dictateMiscEquipConfigFailureCnt			= 0;
	pClient->DictateRunCtl.dictateAttachCheckFailureCnt				= 0;
	pClient->DictateRunCtl.dictateAttachExecuteFailureCnt			= 0;
	pClient->DictateRunCtl.dictateAttachInquireFailureCnt			= 0;
	pClient->DictateRunCtl.dictatePatameterCheckOutFailureCnt		= 0;
	pClient->DictateRunCtl.dictateFullFunctionalityFailureCnt		= 0;
	pClient->DictateRunCtl.dictateMinimumFunctionalityFailureCnt	= 0;
	pClient->DictateRunCtl.dictateCDPServerCheckFailureCnt			= 0;
	pClient->DictateRunCtl.dictateCDPServerConfigFailureCnt			= 0;
	pClient->DictateRunCtl.dictateNbandModeCheckFailureCnt			= 0;
	pClient->DictateRunCtl.dictateNbandModeConfigFailureCnt			= 0;
	pClient->DictateRunCtl.dictateSendDataFailureCnt				= 0;
	pClient->DictateRunCtl.dictateRecvDataFailureCnt				= 0;
	pClient->DictateRunCtl.dictateSendDataRANormalFailureCnt		= 0;
	pClient->DictateRunCtl.dictateRecvDataRANormalFailureCnt		= 0;
	pClient->DictateRunCtl.dictateListenRunCtlFailureCnt			= 0;
	pClient->DictateRunCtl.dictateEvent						= HARDWARE_REBOOT;


	pClient->Registered									= FALSE;
	pClient->NetStateIdentification						= FALSE;		
	pClient->NetNbiotStack								= NetNbiotStack;

	Stm8_EventRunningTime_Init(&pClient->ConnectTimeMS);	
	Stm8_EventRunningTime_Init(&pClient->IdleTimeMS);

	
}	

