#ifndef __PCP_FUNC_H
#define   __PCP_FUNC_H

#include "include.h"
#include "pcpconfig.h"

/* Ack Query Device Version */
typedef struct
{
	unsigned char						ResultCode;										//结果码
	unsigned char						DeviceSoftVersion[16];							//当前版本号
}PCP_AckQueryDeviceVersionTypeDef;

/* Rck New Version Notice */
typedef struct
{
	unsigned char						PlatformSoftVersion[16];						//目的版本号
	unsigned short						UpgradePackSliceSize;							//升级包分片大小
	unsigned short						UpgradePackSliceNum;							//升级包分片总数
	unsigned short						UpgradePackCheckCode;							//升级包校验码
}PCP_RckNewVersionNoticeTypeDef;

/* Request Upgrade Package */
typedef struct
{
	unsigned char						PlatformSoftVersion[16];						//目的版本号
	unsigned short						UpgradePackSliceIndex;							//升级包分片序号
}PCP_RequestUpgradePackageTypeDef;

/* Ack Request Upgrade Package */
typedef struct
{
	unsigned char						ResultCode;										//结果码
	unsigned short						UpgradePackSliceIndex;							//升级包分片序号
	unsigned char						pUpgradeData[1];								//分片数据
}PCP_AckRequestUpgradePackageTypeDef;

/* Report Upgrades */
typedef struct
{
	unsigned char						ResultCode;										//结果码	
	unsigned char						DeviceSoftVersion[16];							//当前版本号
}PCP_ReportUpgradesTypeDef;
	
PCP_StatusTypeDef PCP_Func_FrameCheck(unsigned char* DataFrame, short DataLength);				//PCP数据帧检查

PCP_StatusTypeDef PCP_Func_SelectMessageExecuteCmd(PCP_ClientsTypeDef* pClient);				//PCP判断不同消息码处理不同命令
PCP_StatusTypeDef PCP_Func_AckQueryDeviceVersion(PCP_ClientsTypeDef* pClient);					//PCP查询设备版本
PCP_StatusTypeDef PCP_Func_AckNewVersionNotice(PCP_ClientsTypeDef* pClient);					//PCP新版本通知
PCP_StatusTypeDef PCP_Func_AckRequestUpgradePackage(PCP_ClientsTypeDef* pClient);				//PCP请求升级包应答
PCP_StatusTypeDef PCP_Func_AckReportDownloadStatus(PCP_ClientsTypeDef* pClient);				//PCP上报升级包下载状态应答
PCP_StatusTypeDef PCP_Func_AckPerformUpgrade(PCP_ClientsTypeDef* pClient);						//PCP执行升级
PCP_StatusTypeDef PCP_Func_AckReportUpgrades(PCP_ClientsTypeDef* pClient);						//PCP上报升级结果应答

PCP_StatusTypeDef PCP_Func_SelectUpgradeStatusExecuteCmd(PCP_ClientsTypeDef* pClient);			//PCP判断不同主动上传数据码处理不同命令
PCP_StatusTypeDef PCP_Func_QueryDeviceVersion(PCP_ClientsTypeDef* pClient);					//PCP查询设备版本应答包
PCP_StatusTypeDef PCP_Func_RequestUpgradePackage(PCP_ClientsTypeDef* pClient);					//PCP请求升级包
PCP_StatusTypeDef PCP_Func_ReportDownloadStatus(PCP_ClientsTypeDef* pClient);					//PCP上报升级包下载状态
PCP_StatusTypeDef PCP_Func_ReportUpgrades(PCP_ClientsTypeDef* pClient);						//PCP上报升级结果

#endif /* __PCP_FUNC_H */

