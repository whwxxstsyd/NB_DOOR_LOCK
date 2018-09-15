/**
  *********************************************************************************************************
  * @file    pcpfunc.c
  * @author  Movebroad -- KK
  * @version V1.0
  * @date    2018-07-19
  * @brief   
  *********************************************************************************************************
  * @attention
  *
  *
  *
  *********************************************************************************************************
  */

#include "pcpfunc.h"
#include "platform_config.h"
#include "platform_map.h"
#include "stm32l1xx_config.h"
#include "delay.h"
#include "usart.h"
#include "radio_rf_app.h"

/**********************************************************************************************************
 @Function			PCP_StatusTypeDef PCP_Func_FrameCheck(unsigned char* DataFrame, short DataLength)
 @Description			PCP_Func_FrameCheck					: PCP数据帧检查
 @Input				DataFrame							: PCP数据
					DataLength						: PCP数据长度
 @Return				void
**********************************************************************************************************/
PCP_StatusTypeDef PCP_Func_FrameCheck(unsigned char* DataFrame, short DataLength)
{
	PCP_StatusTypeDef PCPStatus = PCP_OK;
	PCP_MessageDataTypeDef* PCPMessage = (PCP_MessageDataTypeDef*)DataFrame;
	unsigned short PCPReadCRCCheckCode;
	
	if (DataLength < 8) {
		PCPStatus = PCP_Frame_Format_Error;
		goto exit;
	}
	
	PCPReadCRCCheckCode = PCPMessage->CRCCheckCode;
	PCPMessage->CRCCheckCode = 0;
	
	if (PCPSock_ntohs(PCPReadCRCCheckCode) != PCPCrcCheck_getCrcCheckCode(DataFrame, DataLength)) {
		PCPMessage->CRCCheckCode = PCPReadCRCCheckCode;
		PCPStatus = PCP_Frame_CheckCode_Error;
		goto exit;
	}
	else {
		PCPMessage->CRCCheckCode = PCPReadCRCCheckCode;
		PCPStatus = PCP_OK;
	}
	
exit:
	return PCPStatus;
}


/**********************************************************************************************************
 @Function			PCP_StatusTypeDef PCP_Func_SelectMessageExecuteCmd(PCP_ClientsTypeDef* pClient)
 @Description			PCP_Func_SelectMessageExecuteCmd		: PCP判断不同消息码处理不同命令
 @Input				pClient							: PCP客户端实例
 @Return				void
**********************************************************************************************************/
PCP_StatusTypeDef PCP_Func_SelectMessageExecuteCmd(PCP_ClientsTypeDef* pClient)
{
	PCP_StatusTypeDef PCPStatus = PCP_MessageType_Error;
	PCP_MessageDataTypeDef* PCPMessageRecv = (PCP_MessageDataTypeDef*)pClient->Recvbuf;
	
	pClient->Parameter.ProtocolType = PCPMessageRecv->ProtocolType;
	pClient->Parameter.MessageType = PCPMessageRecv->MessageType;
	
	/* 查询设备版本 */
	if (pClient->Parameter.MessageType == PCP_QueryDeviceVersion) {
		PCPStatus = PCP_Func_AckQueryDeviceVersion(pClient);
#ifdef PCP_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("PCP RvQueryDevVer %s", pClient->UpgradeExecution.DeviceSoftVersion);
#endif
		goto exit;
	}
	
	/* 新版本通知 */
	if (pClient->Parameter.MessageType == PCP_NewVersionNotice) {
		PCPStatus = PCP_Func_AckNewVersionNotice(pClient);
#ifdef PCP_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("PCP RvNewVerNotice %s", pClient->Parameter.PlatformSoftVersion);
#endif
		goto exit;
	}
	
	/* 请求升级包应答 */
	if (pClient->Parameter.MessageType == PCP_RequestUpgradePackage) {
		PCPStatus = PCP_Func_AckRequestUpgradePackage(pClient);
#ifdef PCP_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("PCP RvRequestUpgradeAck %d", pClient->Parameter.UpgradePackSliceIndex);
#endif
		goto exit;
	}
	
	/* 上报升级包下载状态应答 */
	if (pClient->Parameter.MessageType == PCP_ReportDownloadStatus) {
		PCPStatus = PCP_Func_AckReportDownloadStatus(pClient);
#ifdef PCP_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("PCP RvReportDownloadStatusAck %d", PCPStatus);
#endif
		goto exit;
	}
	
	/* 执行升级 */
	if (pClient->Parameter.MessageType == PCP_PerformUpgrade) {
		PCPStatus = PCP_Func_AckPerformUpgrade(pClient);
#ifdef PCP_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("PCP RvPerformUpgrade %d", PCPStatus);
#endif
		goto exit;
	}
	
	/* 上报升级结果应答 */
	if (pClient->Parameter.MessageType == PCP_ReportUpgrades) {
		PCPStatus = PCP_Func_AckReportUpgrades(pClient);
#ifdef PCP_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("PCP RvReportUpgradesAck %d", PCPStatus);
#endif
		goto exit;
	}
	
exit:
	return PCPStatus;
}

/**********************************************************************************************************
 @Function			PCP_StatusTypeDef PCP_Func_AckQueryDeviceVersion(PCP_ClientsTypeDef* pClient)
 @Description			PCP_Func_AckQueryDeviceVersion		: PCP查询设备版本应答
 @Input				pClient							: PCP客户端实例
 @Return				void
**********************************************************************************************************/
PCP_StatusTypeDef PCP_Func_AckQueryDeviceVersion(PCP_ClientsTypeDef* pClient)
{
	PCP_StatusTypeDef PCPStatus = PCP_OK;
	PCP_MessageDataTypeDef* PCPMessageProcess = (PCP_MessageDataTypeDef*)pClient->DataProcessStack;
	PCP_AckQueryDeviceVersionTypeDef* PCPAckQueryDeviceVersion = (PCP_AckQueryDeviceVersionTypeDef*)PCPMessageProcess->pPacketData;
	
	memset((void*)pClient->DataProcessStack, 0x0, pClient->DataProcessStack_size);
	
	/* 写入查询设备版本应答 */
	PCPMessageProcess->StartX = PCPSock_htons(PCP_START_X);
	PCPMessageProcess->ProtocolType = PCP_PROTOCOL_TYPE;
	PCPMessageProcess->MessageType = PCP_QueryDeviceVersion;
	PCPMessageProcess->CRCCheckCode = 0x0000;
	PCPMessageProcess->PacketDataLength = PCPSock_htons(sizeof(PCP_AckQueryDeviceVersionTypeDef));
	
	PCPAckQueryDeviceVersion->ResultCode = PCP_ExecuteSuccess;
	memcpy(PCPAckQueryDeviceVersion->DeviceSoftVersion, pClient->UpgradeExecution.DeviceSoftVersion, sizeof(PCPAckQueryDeviceVersion->DeviceSoftVersion));
	
	PCPMessageProcess->CRCCheckCode = PCPSock_htons(PCPCrcCheck_getCrcCheckCode(pClient->DataProcessStack, sizeof(PCP_MessageDataTypeDef) + sizeof(PCP_AckQueryDeviceVersionTypeDef) - 1));
	
	NET_PCP_Message_SendDataEnqueue(pClient->DataProcessStack, sizeof(PCP_MessageDataTypeDef) + sizeof(PCP_AckQueryDeviceVersionTypeDef) - 1);
	NET_PCP_Message_RecvDataOffSet();
	
	pClient->UpgradeExecution.upgradeStatus = PCP_UPGRADE_QUERYVERSION;
	
	return PCPStatus;
}

/**********************************************************************************************************
 @Function			PCP_StatusTypeDef PCP_Func_AckNewVersionNotice(PCP_ClientsTypeDef* pClient)
 @Description			PCP_Func_AckNewVersionNotice			: PCP新版本通知应答
 @Input				pClient							: PCP客户端实例
 @Return				void
**********************************************************************************************************/
PCP_StatusTypeDef PCP_Func_AckNewVersionNotice(PCP_ClientsTypeDef* pClient)
{
	PCP_StatusTypeDef PCPStatus = PCP_OK;
	PCP_ResultCodeTypeDef PCPResultCodeStatus = PCP_ExecuteSuccess;
	PCP_MessageDataTypeDef* PCPMessageRecv = (PCP_MessageDataTypeDef*)pClient->Recvbuf;
	PCP_RckNewVersionNoticeTypeDef* PCPRckNewVersionNotice = (PCP_RckNewVersionNoticeTypeDef*)PCPMessageRecv->pPacketData;
	PCP_MessageDataTypeDef* PCPMessageProcess = (PCP_MessageDataTypeDef*)pClient->DataProcessStack;
	
	pClient->DictateRunCtl.dictateUpgradeQueryVersionCnt = 0;
	
	/* 获取新版本通知数据 */
	memcpy(pClient->Parameter.PlatformSoftVersion, PCPRckNewVersionNotice->PlatformSoftVersion, sizeof(pClient->Parameter.PlatformSoftVersion));
	pClient->Parameter.UpgradePackSliceSize = PCPSock_ntohs(PCPRckNewVersionNotice->UpgradePackSliceSize);
	pClient->Parameter.UpgradePackSliceNum = PCPSock_ntohs(PCPRckNewVersionNotice->UpgradePackSliceNum);
	pClient->Parameter.UpgradePackCheckCode = PCPSock_ntohs(PCPRckNewVersionNotice->UpgradePackCheckCode);
	
	/* 参数写入升级运行管理器 */
	memcpy(pClient->UpgradeExecution.PlatformSoftVersion, pClient->Parameter.PlatformSoftVersion, sizeof(pClient->UpgradeExecution.PlatformSoftVersion));
	pClient->UpgradeExecution.PackSliceIndex = 0;
	pClient->UpgradeExecution.PackSliceSize = pClient->Parameter.UpgradePackSliceSize;
	pClient->UpgradeExecution.PackSliceNum = pClient->Parameter.UpgradePackSliceNum;
	pClient->UpgradeExecution.PackCheckCode = pClient->Parameter.UpgradePackCheckCode;
	
	/* 新版本通知处理回调函数 */
	PCPResultCodeStatus = PCP_UpgradeDataNewVersionNotice_Callback(pClient);
	if (PCPResultCodeStatus != PCP_ExecuteSuccess) {
		pClient->UpgradeExecution.upgradeStatus = PCP_UPGRADE_STANDBY;
	}
	else {
		pClient->UpgradeExecution.upgradeStatus = PCP_UPGRADE_DOWNLOAD;
	}
	
	memset((void*)pClient->DataProcessStack, 0x0, pClient->DataProcessStack_size);
	
	/* 写入新版本通知应答 */
	PCPMessageProcess->StartX = PCPSock_htons(PCP_START_X);
	PCPMessageProcess->ProtocolType = PCP_PROTOCOL_TYPE;
	PCPMessageProcess->MessageType = PCP_NewVersionNotice;
	PCPMessageProcess->CRCCheckCode = 0x0000;
	PCPMessageProcess->PacketDataLength = PCPSock_htons(1);
	PCPMessageProcess->pPacketData[0] = PCPResultCodeStatus;
	
	PCPMessageProcess->CRCCheckCode = PCPSock_htons(PCPCrcCheck_getCrcCheckCode(pClient->DataProcessStack, sizeof(PCP_MessageDataTypeDef)));
	
	NET_PCP_Message_SendDataEnqueue(pClient->DataProcessStack, sizeof(PCP_MessageDataTypeDef));
	NET_PCP_Message_RecvDataOffSet();
	
	return PCPStatus;
}

/**********************************************************************************************************
 @Function			PCP_StatusTypeDef PCP_Func_AckRequestUpgradePackage(PCP_ClientsTypeDef* pClient)
 @Description			PCP_Func_AckRequestUpgradePackage		: PCP请求升级包应答
 @Input				pClient							: PCP客户端实例
 @Return				void
**********************************************************************************************************/
PCP_StatusTypeDef PCP_Func_AckRequestUpgradePackage(PCP_ClientsTypeDef* pClient)
{
	PCP_StatusTypeDef PCPStatus = PCP_OK;
	PCP_MessageDataTypeDef* PCPMessageRecv = (PCP_MessageDataTypeDef*)pClient->Recvbuf;
	PCP_AckRequestUpgradePackageTypeDef* PCPAckRequestUpgradePackage = (PCP_AckRequestUpgradePackageTypeDef*)PCPMessageRecv->pPacketData;
	
	pClient->DictateRunCtl.dictateUpgradeDownloadCnt = 0;
	
	/* 判断请求升级包结果码 */
	if (PCPAckRequestUpgradePackage->ResultCode != PCP_ExecuteSuccess) {
		NET_PCP_Message_RecvDataOffSet();
		PCPStatus = PCP_UpgradePack_Error;
		pClient->UpgradeExecution.upgradeStatus = PCP_UPGRADE_FAILED;
		goto exit;
	}
	
	/* 获取请求升级包响应数据 */
	pClient->Parameter.UpgradePackSliceIndex = PCPSock_ntohs(PCPAckRequestUpgradePackage->UpgradePackSliceIndex);
	
	/* 升级包下载处理回调 */
	PCP_UpgradeDataDownload_Callback(pClient, PCPSock_ntohs(PCPAckRequestUpgradePackage->UpgradePackSliceIndex), \
									  PCPAckRequestUpgradePackage->pUpgradeData, \
									  PCPSock_ntohs(PCPMessageRecv->PacketDataLength) - (sizeof(PCP_AckRequestUpgradePackageTypeDef) - 1));
	
	NET_PCP_Message_RecvDataOffSet();
	
	/* 判断升级包是否下载完成 */
	if (pClient->UpgradeExecution.PackSliceIndex >= pClient->UpgradeExecution.PackSliceNum) {
		pClient->UpgradeExecution.upgradeStatus = PCP_UPGRADE_ASSEMBLE;
	}
	
exit:
	return PCPStatus;
}

/**********************************************************************************************************
 @Function			PCP_StatusTypeDef PCP_Func_AckReportDownloadStatus(PCP_ClientsTypeDef* pClient)
 @Description			PCP_Func_AckReportDownloadStatus		: PCP上报升级包下载状态应答
 @Input				pClient							: PCP客户端实例
 @Return				void
**********************************************************************************************************/
PCP_StatusTypeDef PCP_Func_AckReportDownloadStatus(PCP_ClientsTypeDef* pClient)
{
	PCP_StatusTypeDef PCPStatus = PCP_OK;
	PCP_MessageDataTypeDef* PCPMessageRecv = (PCP_MessageDataTypeDef*)pClient->Recvbuf;
	
	if (PCPMessageRecv->pPacketData[0] != PCP_ExecuteSuccess) {
		NET_PCP_Message_RecvDataOffSet();
		pClient->UpgradeExecution.upgradeStatus = PCP_UPGRADE_FAILED;
		PCPStatus = PCP_UpgradeDown_Error;
		goto exit;
	}
	
	NET_PCP_Message_RecvDataOffSet();
	
	if (pClient->UpgradeExecution.upgradeStatus != PCP_UPGRADE_INSTALL) {
		pClient->UpgradeExecution.upgradeStatus = PCP_UPGRADE_ASSEMBLE;
	}
	
exit:
	return PCPStatus;
}

/**********************************************************************************************************
 @Function			PCP_StatusTypeDef PCP_Func_AckPerformUpgrade(PCP_ClientsTypeDef* pClient)
 @Description			PCP_Func_AckPerformUpgrade			: PCP执行升级
 @Input				pClient							: PCP客户端实例
 @Return				void
**********************************************************************************************************/
PCP_StatusTypeDef PCP_Func_AckPerformUpgrade(PCP_ClientsTypeDef* pClient)
{
	PCP_StatusTypeDef PCPStatus = PCP_OK;
	PCP_MessageDataTypeDef* PCPMessageProcess = (PCP_MessageDataTypeDef*)pClient->DataProcessStack;
	
	memset((void*)pClient->DataProcessStack, 0x0, pClient->DataProcessStack_size);
	
	/* 写入执行升级应答 */
	PCPMessageProcess->StartX = PCPSock_htons(PCP_START_X);
	PCPMessageProcess->ProtocolType = PCP_PROTOCOL_TYPE;
	PCPMessageProcess->MessageType = PCP_PerformUpgrade;
	PCPMessageProcess->CRCCheckCode = 0x0000;
	PCPMessageProcess->PacketDataLength = PCPSock_htons(1);
	PCPMessageProcess->pPacketData[0] = PCP_ExecuteSuccess;
	
	PCPMessageProcess->CRCCheckCode = PCPSock_htons(PCPCrcCheck_getCrcCheckCode(pClient->DataProcessStack, sizeof(PCP_MessageDataTypeDef)));
	
	NET_PCP_Message_SendDataEnqueue(pClient->DataProcessStack, sizeof(PCP_MessageDataTypeDef));
	NET_PCP_Message_RecvDataOffSet();
	
	pClient->UpgradeExecution.upgradeStatus = PCP_UPGRADE_INSTALL;
	pClient->DictateRunCtl.dictateUpgradeAssembleCnt = 0;
	
	return PCPStatus;
}

/**********************************************************************************************************
 @Function			PCP_StatusTypeDef PCP_Func_AckReportUpgrades(PCP_ClientsTypeDef* pClient)
 @Description			PCP_Func_AckReportUpgrades			: PCP上报升级结果应答
 @Input				pClient							: PCP客户端实例
 @Return				void
**********************************************************************************************************/
PCP_StatusTypeDef PCP_Func_AckReportUpgrades(PCP_ClientsTypeDef* pClient)
{
	PCP_StatusTypeDef PCPStatus = PCP_OK;
	PCP_MessageDataTypeDef* PCPMessageRecv = (PCP_MessageDataTypeDef*)pClient->Recvbuf;
	
	if (PCPMessageRecv->pPacketData[0] != PCP_ExecuteSuccess) {
		NET_PCP_Message_RecvDataOffSet();
		pClient->UpgradeExecution.upgradeStatus = PCP_UPGRADE_FAILED;
		PCPStatus = PCP_Upgrade_Error;
		goto exit;
	}
	
	pClient->DictateRunCtl.dictateUpgradeInstallCnt = 0;
	
	NET_PCP_Message_RecvDataOffSet();
	
	pClient->UpgradeExecution.upgradeStatus = PCP_UPGRADE_STANDBY;
	
	PCP_UpgradeDataReportUpgrades_Callback(pClient);
	
exit:
	return PCPStatus;
}


/**********************************************************************************************************
 @Function			PCP_StatusTypeDef PCP_Func_SelectUpgradeStatusExecuteCmd(PCP_ClientsTypeDef* pClient)
 @Description			PCP_Func_SelectUpgradeStatusExecuteCmd	: PCP判断不同主动上传数据码处理不同命令
 @Input				pClient							: PCP客户端实例
 @Return				void
**********************************************************************************************************/
PCP_StatusTypeDef PCP_Func_SelectUpgradeStatusExecuteCmd(PCP_ClientsTypeDef* pClient)
{
	PCP_StatusTypeDef PCPStatus = PCP_OK;
	
	/* 查询设备版本应答包 */
	if (pClient->UpgradeExecution.upgradeStatus == PCP_UPGRADE_QUERYVERSION) {
		PCPStatus = PCP_Func_QueryDeviceVersion(pClient);
#ifdef PCP_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("PCP SdQueryDeviceVersion %s", pClient->UpgradeExecution.DeviceSoftVersion);
#endif
		goto exit;
	}
	
	/* 请求升级包 */
	if (pClient->UpgradeExecution.upgradeStatus == PCP_UPGRADE_DOWNLOAD) {
		PCPStatus = PCP_Func_RequestUpgradePackage(pClient);
#ifdef PCP_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("PCP SdRequestUpgradePackage %d", pClient->UpgradeExecution.PackSliceIndex);
#endif
		goto exit;
	}
	
	/* 上报升级包下载状态 */
	if (pClient->UpgradeExecution.upgradeStatus == PCP_UPGRADE_ASSEMBLE) {
		PCPStatus = PCP_Func_ReportDownloadStatus(pClient);
#ifdef PCP_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("PCP SdReportDownloadStatus");
#endif
		goto exit;
	}
	
	/* 上报升级结果 */
	if (pClient->UpgradeExecution.upgradeStatus == PCP_UPGRADE_INSTALL) {
		PCPStatus = PCP_Func_ReportUpgrades(pClient);
#ifdef PCP_DEBUG_LOG_RF_PRINT
		Radio_Trf_Debug_Printf_Level2("PCP SdReportUpgrades");
#endif
		goto exit;
	}
	
exit:
	return PCPStatus;
}

/**********************************************************************************************************
 @Function			PCP_StatusTypeDef PCP_Func_QueryDeviceVersion(PCP_ClientsTypeDef* pClient)
 @Description			PCP_Func_QueryDeviceVersion			: PCP查询设备版本应答包
 @Input				pClient							: PCP客户端实例
 @Return				void
**********************************************************************************************************/
PCP_StatusTypeDef PCP_Func_QueryDeviceVersion(PCP_ClientsTypeDef* pClient)
{
	PCP_StatusTypeDef PCPStatus = PCP_OK;
	PCP_MessageDataTypeDef* PCPMessageProcess = (PCP_MessageDataTypeDef*)pClient->DataProcessStack;
	PCP_AckQueryDeviceVersionTypeDef* PCPAckQueryDeviceVersion = (PCP_AckQueryDeviceVersionTypeDef*)PCPMessageProcess->pPacketData;
	
	/* 查询设备版本应答包尝试次数 */
	pClient->DictateRunCtl.dictateUpgradeQueryVersionCnt++;
	if (pClient->DictateRunCtl.dictateUpgradeQueryVersionCnt > 2) {
		/* 确实没有需要升级包 */
		pClient->DictateRunCtl.dictateUpgradeQueryVersionCnt = 0;
		pClient->UpgradeExecution.upgradeStatus = PCP_UPGRADE_STANDBY;
		goto exit;
	}
	
	memset((void*)pClient->DataProcessStack, 0x0, pClient->DataProcessStack_size);
	
	/* 写入查询设备版本应答 */
	PCPMessageProcess->StartX = PCPSock_htons(PCP_START_X);
	PCPMessageProcess->ProtocolType = PCP_PROTOCOL_TYPE;
	PCPMessageProcess->MessageType = PCP_QueryDeviceVersion;
	PCPMessageProcess->CRCCheckCode = 0x0000;
	PCPMessageProcess->PacketDataLength = PCPSock_htons(sizeof(PCP_AckQueryDeviceVersionTypeDef));
	
	PCPAckQueryDeviceVersion->ResultCode = PCP_ExecuteSuccess;
	memcpy(PCPAckQueryDeviceVersion->DeviceSoftVersion, pClient->UpgradeExecution.DeviceSoftVersion, sizeof(PCPAckQueryDeviceVersion->DeviceSoftVersion));
	
	PCPMessageProcess->CRCCheckCode = PCPSock_htons(PCPCrcCheck_getCrcCheckCode(pClient->DataProcessStack, sizeof(PCP_MessageDataTypeDef) + sizeof(PCP_AckQueryDeviceVersionTypeDef) - 1));
	
	NET_PCP_Message_SendDataEnqueue(pClient->DataProcessStack, sizeof(PCP_MessageDataTypeDef) + sizeof(PCP_AckQueryDeviceVersionTypeDef) - 1);
	
exit:
	return PCPStatus;
}

/**********************************************************************************************************
 @Function			PCP_StatusTypeDef PCP_Func_RequestUpgradePackage(PCP_ClientsTypeDef* pClient)
 @Description			PCP_Func_RequestUpgradePackage		: PCP请求升级包
 @Input				pClient							: PCP客户端实例
 @Return				void
**********************************************************************************************************/
PCP_StatusTypeDef PCP_Func_RequestUpgradePackage(PCP_ClientsTypeDef* pClient)
{
	PCP_StatusTypeDef PCPStatus = PCP_OK;
	PCP_MessageDataTypeDef* PCPMessageProcess = (PCP_MessageDataTypeDef*)pClient->DataProcessStack;
	PCP_RequestUpgradePackageTypeDef* PCPRequestUpgradePackage = (PCP_RequestUpgradePackageTypeDef*)PCPMessageProcess->pPacketData;
	
	/* 请求升级包尝试次数 */
	pClient->DictateRunCtl.dictateUpgradeDownloadCnt++;
	if (pClient->DictateRunCtl.dictateUpgradeDownloadCnt > 5) {
		pClient->DictateRunCtl.dictateUpgradeDownloadCnt = 0;
		pClient->UpgradeExecution.upgradeStatus = PCP_UPGRADE_FAILED;
		goto exit;
	}
	
	memset((void*)pClient->DataProcessStack, 0x0, pClient->DataProcessStack_size);
	
	/* 写入请求升级包 */
	PCPMessageProcess->StartX = PCPSock_htons(PCP_START_X);
	PCPMessageProcess->ProtocolType = PCP_PROTOCOL_TYPE;
	PCPMessageProcess->MessageType = PCP_RequestUpgradePackage;
	PCPMessageProcess->CRCCheckCode = 0x0000;
	PCPMessageProcess->PacketDataLength = PCPSock_htons(sizeof(PCP_RequestUpgradePackageTypeDef));
	
	memcpy(PCPRequestUpgradePackage->PlatformSoftVersion, pClient->UpgradeExecution.PlatformSoftVersion, sizeof(PCPRequestUpgradePackage->PlatformSoftVersion));
	PCPRequestUpgradePackage->UpgradePackSliceIndex = PCPSock_htons(pClient->UpgradeExecution.PackSliceIndex);
	
	PCPMessageProcess->CRCCheckCode = PCPSock_htons(PCPCrcCheck_getCrcCheckCode(pClient->DataProcessStack, sizeof(PCP_MessageDataTypeDef) + sizeof(PCP_RequestUpgradePackageTypeDef) - 1));
	
	NET_PCP_Message_SendDataEnqueue(pClient->DataProcessStack, sizeof(PCP_MessageDataTypeDef) + sizeof(PCP_RequestUpgradePackageTypeDef) - 1);
	
exit:
	return PCPStatus;
}

/**********************************************************************************************************
 @Function			PCP_StatusTypeDef PCP_Func_ReportDownloadStatus(PCP_ClientsTypeDef* pClient)
 @Description			PCP_Func_ReportDownloadStatus			: PCP上报升级包下载状态
 @Input				pClient							: PCP客户端实例
 @Return				void
**********************************************************************************************************/
PCP_StatusTypeDef PCP_Func_ReportDownloadStatus(PCP_ClientsTypeDef* pClient)
{
	PCP_StatusTypeDef PCPStatus = PCP_OK;
	PCP_ResultCodeTypeDef PCPResultCodeStatus = PCP_ExecuteSuccess;
	PCP_MessageDataTypeDef* PCPMessageProcess = (PCP_MessageDataTypeDef*)pClient->DataProcessStack;
	
	/* 上报升级包下载状态尝试次数 */
	pClient->DictateRunCtl.dictateUpgradeAssembleCnt++;
	if (pClient->DictateRunCtl.dictateUpgradeAssembleCnt > 5) {
		pClient->DictateRunCtl.dictateUpgradeAssembleCnt = 0;
		pClient->UpgradeExecution.upgradeStatus = PCP_UPGRADE_FAILED;
		goto exit;
	}
	
	/* 组装升级包处理回调 */
	PCPResultCodeStatus = PCP_UpgradeDataAssemble_Callback(pClient);
	if (PCPResultCodeStatus != PCP_ExecuteSuccess) {
		pClient->UpgradeExecution.upgradeStatus = PCP_UPGRADE_STANDBY;
	}
	
	memset((void*)pClient->DataProcessStack, 0x0, pClient->DataProcessStack_size);
	
	/* 写入上报升级包下载状态 */
	PCPMessageProcess->StartX = PCPSock_htons(PCP_START_X);
	PCPMessageProcess->ProtocolType = PCP_PROTOCOL_TYPE;
	PCPMessageProcess->MessageType = PCP_ReportDownloadStatus;
	PCPMessageProcess->CRCCheckCode = 0x0000;
	PCPMessageProcess->PacketDataLength = PCPSock_htons(1);
	PCPMessageProcess->pPacketData[0] = PCPResultCodeStatus;
	
	PCPMessageProcess->CRCCheckCode = PCPSock_htons(PCPCrcCheck_getCrcCheckCode(pClient->DataProcessStack, sizeof(PCP_MessageDataTypeDef)));
	
	NET_PCP_Message_SendDataEnqueue(pClient->DataProcessStack, sizeof(PCP_MessageDataTypeDef));
	
exit:
	return PCPStatus;
}

/**********************************************************************************************************
 @Function			PCP_StatusTypeDef PCP_Func_ReportUpgrades(PCP_ClientsTypeDef* pClient)
 @Description			PCP_Func_ReportUpgrades				: PCP上报升级结果
 @Input				pClient							: PCP客户端实例
 @Return				void
**********************************************************************************************************/
PCP_StatusTypeDef PCP_Func_ReportUpgrades(PCP_ClientsTypeDef* pClient)
{
	PCP_StatusTypeDef PCPStatus = PCP_OK;
	PCP_MessageDataTypeDef* PCPMessageProcess = (PCP_MessageDataTypeDef*)pClient->DataProcessStack;
	PCP_ReportUpgradesTypeDef* PCPReportUpgrades = (PCP_ReportUpgradesTypeDef*)PCPMessageProcess->pPacketData;
	
	/* 上报升级包下载状态尝试次数 */
	pClient->DictateRunCtl.dictateUpgradeInstallCnt++;
	if (pClient->DictateRunCtl.dictateUpgradeInstallCnt > 5) {
		pClient->DictateRunCtl.dictateUpgradeInstallCnt = 0;
		pClient->UpgradeExecution.upgradeStatus = PCP_UPGRADE_FAILED;
		goto exit;
	}
	
	memset((void*)pClient->DataProcessStack, 0x0, pClient->DataProcessStack_size);
	
	/* 写入上报升级结果 */
	PCPMessageProcess->StartX = PCPSock_htons(PCP_START_X);
	PCPMessageProcess->ProtocolType = PCP_PROTOCOL_TYPE;
	PCPMessageProcess->MessageType = PCP_ReportUpgrades;
	PCPMessageProcess->CRCCheckCode = 0x0000;
	PCPMessageProcess->PacketDataLength = PCPSock_htons(sizeof(PCP_ReportUpgradesTypeDef));
	
	PCPReportUpgrades->ResultCode = PCP_ExecuteSuccess;
	memcpy(PCPReportUpgrades->DeviceSoftVersion, pClient->UpgradeExecution.PlatformSoftVersion, sizeof(PCPReportUpgrades->DeviceSoftVersion));
	
	PCPMessageProcess->CRCCheckCode = PCPSock_htons(PCPCrcCheck_getCrcCheckCode(pClient->DataProcessStack, sizeof(PCP_MessageDataTypeDef) + sizeof(PCP_ReportUpgradesTypeDef) - 1));
	
	NET_PCP_Message_SendDataEnqueue(pClient->DataProcessStack, sizeof(PCP_MessageDataTypeDef) + sizeof(PCP_ReportUpgradesTypeDef) - 1);
	
exit:
	return PCPStatus;
}

/********************************************** END OF FLEE **********************************************/
