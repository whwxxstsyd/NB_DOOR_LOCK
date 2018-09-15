#ifndef __PCP_UPGRADE_H
#define   __PCP_UPGRADE_H

#include "sys.h"
#include "pcpconfig.h"

typedef __packed struct
{
	u8			Status;
	u32			BaseAddr;
	u16			BlockNum;
	u16			BlockLen;
	u16			DataLen;
	u32			SoftVer;
	u32			CheckCode;
}PCP_APPInfoTypeDef;

#define	UPGRADE_BACKUP_APP_WRITE_DISABLE	0												//控制备份
#define	UPGRADE_BACKUP_APP_WRITE_ENABLE	1												//强制备份
#define	UPGRADE_BACKUP_APP_WRITE_TYPE		UPGRADE_BACKUP_APP_WRITE_DISABLE						//备份区备份模式

#if 0
#define	UPGRADE_LOW_LIMIT_RSSI			-100												//信号值限制下限
#define	UPGRADE_LOW_LIMIT_SNR			-100												//信号质量限制下限
#else
#define	UPGRADE_LOW_LIMIT_RSSI			TCFG_EEPROM_GetUpgradeLimitRssi()						//信号值限制下限
#define	UPGRADE_LOW_LIMIT_SNR			TCFG_EEPROM_GetUpgradeLimitSnr()						//信号质量限制下限
#endif

#define	UPGRADE_PACK_SLICE_BLOCK_SIZE		512												//升级包最大分片大小

PCP_ResultCodeTypeDef PCP_Upgrade_BackupCurrentAPP(PCP_ClientsTypeDef* pClient);											//升级备份当前APP
PCP_ResultCodeTypeDef PCP_Upgrade_NewVersionNotice(PCP_ClientsTypeDef* pClient);											//升级新版本通知
PCP_ResultCodeTypeDef PCP_Upgrade_DataDownload(PCP_ClientsTypeDef* pClient, u16 SliceIndex, u8* UpgradeData, u16 UpgradeDataLength);	//升级包下载
PCP_ResultCodeTypeDef PCP_Upgrade_DataAssemble(PCP_ClientsTypeDef* pClient);												//升级包组装校验
PCP_ResultCodeTypeDef PCP_Upgrade_AfterUpdata(PCP_ClientsTypeDef* pClient);												//升级包开始升级

u8 CalculateSumCheckCode(u32 regCode, uint8_t* buf, uint32_t length);														//加和校验
u8 CalculateStringToHex(u8 HighChar, u8 LowChar);																		//字符转换

#endif
