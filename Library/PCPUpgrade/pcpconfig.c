/**
  *********************************************************************************************************
  * @file    pcpconfig.c
  * @author  Movebroad -- KK
  * @version V1.0
  * @date    2018-07-16
  * @brief   PCP	: �豸���й���������������ƽ̨֮��Ӧ�ò�����Э��
  *********************************************************************************************************
  * @attention
  *
  *
  *
  *********************************************************************************************************
  */
	
#include "pcpconfig.h"	
#include "pcpupgrade.h"


unsigned char PCP_SendBuf[PCP_BUFFER_SIZE];
unsigned char PCP_RecvBuf[PCP_BUFFER_SIZE];
unsigned char PCP_DataStack[PCP_DATASTACK_SIZE];

/**********************************************************************************************************
 @Function			void PCP_Client_Init(PCP_ClientsTypeDef* pClient, PCP_CoAPNetTransportTypeDef* NetSock, NET_NBIOT_ClientsTypeDef* NetNbiotStack)
 @Description			PCP_Client_Init						: ��ʼ��PCP�ͻ���
 @Input				pClient								: PCP�ͻ���ʵ��
					NetSock								: PCP CoAP Э��ջ
					NetNbiotStack						: NET NBIOT Э��ջ
 @Return				void
**********************************************************************************************************/
void PCP_Client_Init(PCP_ClientsTypeDef* pClient, PCP_CoAPNetTransportTypeDef* NetSock, NET_NBIOT_ClientsTypeDef* NetNbiotStack)
{	
	pClient->Sendbuf										= PCP_SendBuf;
	pClient->Recvbuf										= PCP_RecvBuf;
	pClient->Sendbuf_size									= sizeof(PCP_SendBuf);
	pClient->Recvbuf_size									= sizeof(PCP_RecvBuf);
	pClient->Sendlen										= 0;
	pClient->Recvlen										= 0;
	pClient->DataProcessStack								= PCP_DataStack;
	pClient->DataProcessStack_size							= sizeof(PCP_DataStack);
	
	pClient->Command_Timeout_Sec							= PCP_COMMAND_TIMEOUT_SEC;
	pClient->Command_Failure_Cnt							= PCP_COMMAND_FAILURE_CNT;
	
	pClient->DictateRunCtl.dictateEnable					= false;
	pClient->DictateRunCtl.dictateTimeoutSec				= 0;
	pClient->DictateRunCtl.dictateInitializedFailureCnt		= 0;
	pClient->DictateRunCtl.dictateReadyFailureCnt			= 0;
	pClient->DictateRunCtl.dictateRecvFailureCnt			= 0;
	pClient->DictateRunCtl.dictateSendFailureCnt			= 0;
	pClient->DictateRunCtl.dictateExecuteFailureCnt			= 0;
	pClient->DictateRunCtl.dictateActiveUploadFailureCnt	= 0;
	pClient->DictateRunCtl.dictateUpgradeQueryVersionCnt	= 0;
	pClient->DictateRunCtl.dictateUpgradeDownloadCnt		= 0;
	pClient->DictateRunCtl.dictateUpgradeAssembleCnt		= 0;	
	pClient->DictateRunCtl.dictateUpgradeInstallCnt			= 0;
	pClient->DictateRunCtl.dictateEvent						= PCP_EVENT_INITIALIZED;
	
	pClient->UpgradeExecution.upgradeStatus					= PCP_UPGRADE_STANDBY;
	sprintf((char*)pClient->UpgradeExecution.DeviceSoftVersion, "V%d.%d", TCFG_Utility_Get_Major_Softnumber(), TCFG_Utility_Get_Sub_Softnumber());
	sprintf((char*)pClient->UpgradeExecution.PlatformSoftVersion, "V%d.%d", TCFG_Utility_Get_Major_Softnumber(), TCFG_Utility_Get_Sub_Softnumber());
	pClient->UpgradeExecution.PackSliceIndex				= 0;
	pClient->UpgradeExecution.PackSliceSize					= 0;
	pClient->UpgradeExecution.PackLastSliceSize				= 0;
	pClient->UpgradeExecution.PackSliceNum					= 0;
	pClient->UpgradeExecution.PackCheckCode					= 0;
		
	pClient->CoAPStack										= NetSock;
	pClient->NetNbiotStack									= NetNbiotStack;
}

/**********************************************************************************************************
 @Function			PCP_ResultCodeTypeDef PCP_UpgradeDataNewVersionNotice_Callback(PCP_ClientsTypeDef* pClient)
 @Description			PCP_UpgradeDataNewVersionNotice_Callback: PCP�°汾֪ͨ����ص�����
 @Input				pClient							: PCP�ͻ���ʵ��
 @Return				void
**********************************************************************************************************/
PCP_ResultCodeTypeDef PCP_UpgradeDataNewVersionNotice_Callback(PCP_ClientsTypeDef* pClient)
{
	PCP_ResultCodeTypeDef PCPResultCodeStatus = PCP_ExecuteSuccess;
	
#ifdef PCP_DEBUG_LOG_RF_PRINT
	GPRS_DEBUG(DEBUG_INFO, "PlatSoftVer: %s", pClient->Parameter.PlatformSoftVersion);
	GPRS_DEBUG(DEBUG_INFO, "PackSliceSize: %d", pClient->Parameter.UpgradePackSliceSize);
	GPRS_DEBUG(DEBUG_INFO, "PackSliceNum: %d", pClient->Parameter.UpgradePackSliceNum);
	GPRS_DEBUG(DEBUG_INFO, "PackCheckCode: %X", CalculateStringToHex(pClient->Parameter.UpgradePackCheckCode>>8, pClient->Parameter.UpgradePackCheckCode&0xFF));
#endif
	
	PCPResultCodeStatus = PCP_Upgrade_NewVersionNotice(pClient);
	
	return PCPResultCodeStatus;
}

/**********************************************************************************************************
 @Function			PCP_ResultCodeTypeDef PCP_UpgradeDataDownload_Callback(PCP_ClientsTypeDef* pClient, u16 SliceIndex, u8* UpgradeData, u16 UpgradeDataLength)
 @Description			PCP_UpgradeDataDownload_Callback		: PCP���������ش���ص�����
 @Input				pClient							: PCP�ͻ���ʵ��
					SliceIndex						: PCP��Ƭ���
					UpgradeData						: ����������
					UpgradeDataLength					: ����������
 @Return				void
**********************************************************************************************************/
PCP_ResultCodeTypeDef PCP_UpgradeDataDownload_Callback(PCP_ClientsTypeDef* pClient, u16 SliceIndex, u8* UpgradeData, u16 UpgradeDataLength)
{
	PCP_ResultCodeTypeDef PCPResultCodeStatus = PCP_ExecuteSuccess;
	
#ifdef PCP_DEBUG_LOG_RF_PRINT
	GPRS_DEBUG(DEBUG_INFO, "Down%d.%d: OK", SliceIndex, UpgradeDataLength);
#endif
	
	PCPResultCodeStatus = PCP_Upgrade_DataDownload(pClient, SliceIndex, UpgradeData, UpgradeDataLength);
	
	return PCPResultCodeStatus;
}

/**********************************************************************************************************
 @Function			PCP_ResultCodeTypeDef PCP_UpgradeDataAssemble_Callback(PCP_ClientsTypeDef* pClient)
 @Description			PCP_UpgradeDataAssemble_Callback		: PCP��������װ����ص�����
 @Input				pClient							: PCP�ͻ���ʵ��
 @Return				void
**********************************************************************************************************/
PCP_ResultCodeTypeDef PCP_UpgradeDataAssemble_Callback(PCP_ClientsTypeDef* pClient)
{
	PCP_ResultCodeTypeDef PCPResultCodeStatus = PCP_ExecuteSuccess;
	
#ifdef PCP_DEBUG_LOG_RF_PRINT
	GPRS_DEBUG(DEBUG_INFO, "Download Over!!");
#endif
	
	PCPResultCodeStatus = PCP_Upgrade_DataAssemble(pClient);
	
	return PCPResultCodeStatus;
}

/**********************************************************************************************************
 @Function			PCP_ResultCodeTypeDef PCP_UpgradeDataReportUpgrades_Callback(PCP_ClientsTypeDef* pClient)
 @Description			PCP_UpgradeDataReportUpgrades_Callback	: PCP�ϱ������������ص�����
 @Input				pClient							: PCP�ͻ���ʵ��
 @Return				void
**********************************************************************************************************/
PCP_ResultCodeTypeDef PCP_UpgradeDataReportUpgrades_Callback(PCP_ClientsTypeDef* pClient)
{
	PCP_ResultCodeTypeDef PCPResultCodeStatus = PCP_ExecuteSuccess;
	
#ifdef PCP_DEBUG_LOG_RF_PRINT
	GPRS_DEBUG(DEBUG_INFO, "Upgrade Over!!");
#endif
	
	PCPResultCodeStatus = PCP_Upgrade_AfterUpdata(pClient);
	
	return PCPResultCodeStatus;
}

/********************************************** END OF FLEE **********************************************/

