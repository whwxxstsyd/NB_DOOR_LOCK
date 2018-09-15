/**
  *********************************************************************************************************
  * @file    platform_map.c
  * @author  Movebroad -- KK
  * @version V1.0
  * @date    2018-03-24
  * @brief   
  *********************************************************************************************************
  * @attention
  *			
  *
  *
  *********************************************************************************************************
  */

#include "platform_map.h"
#include "stm32l1xx_config.h"
#include "hal_eeprom.h"
#include "hal_vbat.h"
#include "hal_temperature.h"
#include "hal_spiflash.h"
#include "radio_hal_rf.h"
#include "radio_rf_app.h"
#include "inspectmessageoperate.h"
#include "string.h"

TCFG_SystemDataTypeDef			TCFG_SystemData;

/**********************************************************************************************************
 @Function			void TCFG_EEPROM_SystemInfo_Init(void)
 @Description			TCFG_EEPROM_SystemInfo_Init					: 系统信息初始化
 @Input				void
 @Return				void
**********************************************************************************************************/
void TCFG_EEPROM_SystemInfo_Init(void)
{
	TCFG_EEPROM_ReadConfigData();
	TCFG_EEPROM_SetBootMode(TCFG_ENV_BOOTMODE_NORMAL);
}

/**********************************************************************************************************
 @Function			void TCFG_EEPROM_WriteConfigData(void)
 @Description			TCFG_EEPROM_WriteConfigData					: 写入系统配置信息
 @Input				void
 @Return				void
**********************************************************************************************************/
void TCFG_EEPROM_WriteConfigData(void)
{
	char vender[4];
	unsigned int Brand = 0;
	int serverip[4];
	
	/* 生产商与设备号 */
	TCFG_EEPROM_SetSNfromBrandKey(TCFG_EEPROM_Get_MAC_SN());
	TCFG_EEPROM_GetVender(vender);
	Brand |= (vender[0] & 0x000000FF) << 3*8;
	Brand |= (vender[1] & 0x000000FF) << 2*8;
	Brand |= (vender[2] & 0x000000FF) << 1*8;
	Brand |= (vender[3] & 0x000000FF) << 0*8;
	TCFG_EEPROM_SetFactoryBrand(Brand);
	
	/* 传感器灵敏度值 */
	TCFG_SystemData.Sensitivity = SENSE_MIDDLE;
	TCFG_EEPROM_SetSavedSensitivity(TCFG_SystemData.Sensitivity);
	
	/* 地磁背景值 */
	TCFG_SystemData.MagBackgroundX = 0x7FFF;
	TCFG_SystemData.MagBackgroundY = 0x7FFF;
	TCFG_SystemData.MagBackgroundZ = 0x7FFF;
	TCFG_EEPROM_SetMagBackgroud(TCFG_SystemData.MagBackgroundX, TCFG_SystemData.MagBackgroundY, TCFG_SystemData.MagBackgroundZ);
	
	/* 地磁背景温度补偿值 */
	TCFG_SystemData.MagBackgroudTemp = 0;
	TCFG_EEPROM_SetMagBackgroudTemp(TCFG_SystemData.MagBackgroudTemp);
	
	/* 地磁温飘计算值 */
	TCFG_SystemData.MagCoefX = 0;
	TCFG_SystemData.MagCoefY = 0;
	TCFG_SystemData.MagCoefZ = 0;
	TCFG_EEPROM_SetMagTempCoef(TCFG_SystemData.MagCoefX, TCFG_SystemData.MagCoefY, TCFG_SystemData.MagCoefZ);
	
	/* 设备工作模式 */
	TCFG_SystemData.WorkMode = NORMAL_WORK;
	TCFG_EEPROM_SetWorkMode(TCFG_SystemData.WorkMode);
	
	/* 无线通道选择 */
	TCFG_SystemData.RFChannel = RADIO_RF_CHANNEL1;
	TCFG_EEPROM_SetRfChannel(TCFG_SystemData.RFChannel);
	
	/* 无线心跳间隔 */
	TCFG_SystemData.Heartinterval = HEART_INTERVAL;
	TCFG_EEPROM_SetHeartinterval(TCFG_SystemData.Heartinterval);
	
	/* 无线调试输出等级 */
	TCFG_SystemData.RFDprintLv = RF_DPRINT_LV_TYPE;
	TCFG_EEPROM_SetRFDprintLv(TCFG_SystemData.RFDprintLv);
	
	/* 地磁模式 */
	TCFG_SystemData.MagMode = 0;
	TCFG_EEPROM_SetMagMode(TCFG_SystemData.MagMode);
	
	/* 雷达次数 */
	TCFG_SystemData.RadarCount = 0;
	TCFG_EEPROM_SetRadarCount(TCFG_SystemData.RadarCount);
	
	/* 雷达调试模式 */
	TCFG_SystemData.RadarDbgMode = 8;
	TCFG_EEPROM_SetRadarDbgMode(TCFG_SystemData.RadarDbgMode);
	
	/* 雷达检测范围 */
	TCFG_SystemData.RadarRange = 0;
	TCFG_EEPROM_SetRadarRange(TCFG_SystemData.RadarRange);
	
	/* 车辆进入延时上报时间 */
	TCFG_SystemData.CarInDelay = 8;
	TCFG_EEPROM_SetCarInDelay(TCFG_SystemData.CarInDelay);
	
	/* 车位检测车辆次数 */
	TCFG_SystemData.SpotStatusCount = 0;
	TCFG_EEPROM_SetStatusCount(TCFG_SystemData.SpotStatusCount);
	
	/* 设备重启次数 */
	TCFG_SystemData.DeviceBootCount = 0;
	TCFG_EEPROM_SetDevBootCnt(TCFG_SystemData.DeviceBootCount);
	
	/* NBIot心跳间隔 */
	TCFG_SystemData.NBIotHeart = NBIOT_HEART_DATA_HOURS;
	TCFG_EEPROM_SetNbiotHeart(TCFG_SystemData.NBIotHeart);
	
	/* NBIot重启次数 */
	TCFG_SystemData.NBIotBootCount = 0;
	TCFG_EEPROM_SetNbiotBootCnt(TCFG_SystemData.NBIotBootCount);
	
	/* NBIotPSM模式 */
	TCFG_SystemData.NBIotPSMEnable = 0;
	TCFG_EEPROM_SetEnableNBiotPSM(TCFG_SystemData.NBIotPSMEnable);
	
	/* NBIot Coap 接收数据次数 */
	TCFG_SystemData.CoapRecvCount = 0;
	TCFG_EEPROM_SetCoapRecvCnt(TCFG_SystemData.CoapRecvCount);
	
	/* NBIot Coap 发送数据次数 */
	TCFG_SystemData.CoapSentCount = 0;
	TCFG_EEPROM_SetCoapSentCnt(TCFG_SystemData.CoapSentCount);
	
	/* NBIot MqttSN 接收数据次数 */
	TCFG_SystemData.MqttSNRecvCount = 0;
	TCFG_EEPROM_SetMqttSNRecvCnt(TCFG_SystemData.MqttSNRecvCount);
	
	/* NBIot MqttSN 发送数据次数 */
	TCFG_SystemData.MqttSNSentCount = 0;
	TCFG_EEPROM_SetMqttSNSentCnt(TCFG_SystemData.MqttSNSentCount);
	
	/* NBIot OneNET 接收数据次数 */
	TCFG_SystemData.OneNETRecvCount = 0;
	TCFG_EEPROM_SetOneNETRecvCnt(TCFG_SystemData.OneNETRecvCount);
	
	/* NBIot OneNET 发送数据次数 */
	TCFG_SystemData.OneNETSentCount = 0;
	TCFG_EEPROM_SetOneNETSentCnt(TCFG_SystemData.OneNETSentCount);
	
	/* RF命令接收条数 */
	TCFG_SystemData.RFCommandCount = 0;
	TCFG_EEPROM_SetRFCmdCnt(TCFG_SystemData.RFCommandCount);
	
	/* NB命令接收条数 */
	TCFG_SystemData.NBCommandCount = 0;
	TCFG_EEPROM_SetNBCmdCnt(TCFG_SystemData.NBCommandCount);
	
	/* Coap间隔时间发送普通数据包用于接收下行数据 */
	TCFG_SystemData.CoapRATimeHour = NBCOAP_RATIME_NORMAL_TYPE;
	TCFG_EEPROM_SetCoapRATimeHour(TCFG_SystemData.CoapRATimeHour);
	
	/* Coap保持连接时间 */
	TCFG_SystemData.CoapConnectTime = 0;
	TCFG_EEPROM_SetCoapConnectTime(TCFG_SystemData.CoapConnectTime);
	
	/* Coap休眠时间 */
	TCFG_SystemData.CoapIdleTime = 0;
	TCFG_EEPROM_SetCoapIdleTime(TCFG_SystemData.CoapIdleTime);
	
	/* Coap保持连接时间(一天) */
	TCFG_SystemData.CoapConnectDayTime = 0;
	TCFG_EEPROM_SetCoapConnectDayTime(TCFG_SystemData.CoapConnectDayTime);
	
	/* Coap休眠时间(一天) */
	TCFG_SystemData.CoapIdleDayTime = 0;
	TCFG_EEPROM_SetCoapIdleDayTime(TCFG_SystemData.CoapIdleDayTime);
	
	/* Coap使用配额时间(一天) */
	TCFG_SystemData.CoapQuotaTime = NBCOAP_COAP_QUOTA_TIME_TYPE;
	TCFG_EEPROM_SetCoapQuotaTime(TCFG_SystemData.CoapQuotaTime);
	
	/* 升级信号值限制下限 */
	TCFG_SystemData.UpgradeLimitRssi = NBCOAP_PCP_UPGRADE_LIMIT_RSSI;
	TCFG_EEPROM_SetUpgradeLimitRssi(TCFG_SystemData.UpgradeLimitRssi);
	
	/* 升级信号质量限制下限 */
	TCFG_SystemData.UpgradeLimitSnr = NBCOAP_PCP_UPGRADE_LIMIT_SNR;
	TCFG_EEPROM_SetUpgradeLimitSnr(TCFG_SystemData.UpgradeLimitSnr);
	
	/* 蜂鸣器控制 */
	TCFG_SystemData.BeepCtrlOff = 0;
	TCFG_EEPROM_SetBeepOff(TCFG_SystemData.BeepCtrlOff);
	
	/* NB核心网地址 */
	sscanf(COAPCDPADDR, "%d.%d.%d.%d", &serverip[3], &serverip[2], &serverip[1], &serverip[0]);
	TCFG_SystemData.NBCoapCDPServer.ip.ip8[3] = serverip[3];
	TCFG_SystemData.NBCoapCDPServer.ip.ip8[2] = serverip[2];
	TCFG_SystemData.NBCoapCDPServer.ip.ip8[1] = serverip[1];
	TCFG_SystemData.NBCoapCDPServer.ip.ip8[0] = serverip[0];
	TCFG_SystemData.NBCoapCDPServer.port = COAPCDPPORT;
	TCFG_EEPROM_SetServerIP(TCFG_SystemData.NBCoapCDPServer.ip.ip32);
	TCFG_EEPROM_SetServerPort(TCFG_SystemData.NBCoapCDPServer.port);
}

/**********************************************************************************************************
 @Function			void TCFG_EEPROM_ReadConfigData(void)
 @Description			TCFG_EEPROM_ReadConfigData					: 读取系统配置信息
 @Input				void
 @Return				void
**********************************************************************************************************/
void TCFG_EEPROM_ReadConfigData(void)
{
	int serverip[4];
	
	/* 获取SubSN */
	TCFG_SystemData.SubSn = TCFG_EEPROM_Get_MAC_SN();
	TCFG_EEPROM_Get_MAC_SN_String();
	
	/* 获取传感器灵敏度值 */
	TCFG_SystemData.Sensitivity = TCFG_EEPROM_GetSavedSensitivity();
	if ((TCFG_SystemData.Sensitivity > SENSE_LOWEST) || (TCFG_SystemData.Sensitivity < SENSE_HIGHEST)) {
		TCFG_SystemData.Sensitivity = SENSE_MIDDLE;
		TCFG_EEPROM_SetSavedSensitivity(TCFG_SystemData.Sensitivity);
	}
	
	/* 获取BOOT版本号 */
	TCFG_SystemData.BootVersion = TCFG_EEPROM_GetBootVersion();
	
	/* 地磁背景值 */
	TCFG_SystemData.MagBackgroundX = TCFG_EEPROM_GetMagBackgroud(TCFG_X_AXIS);
	TCFG_SystemData.MagBackgroundY = TCFG_EEPROM_GetMagBackgroud(TCFG_Y_AXIS);
	TCFG_SystemData.MagBackgroundZ = TCFG_EEPROM_GetMagBackgroud(TCFG_Z_AXIS);
	
	/* 写入背景值 */
	memset((void*)&Qmc5883lData, 1, sizeof(Qmc5883lData));
	Qmc5883lData.X_Back = TCFG_SystemData.MagBackgroundX;
	Qmc5883lData.Y_Back = TCFG_SystemData.MagBackgroundY;
	Qmc5883lData.Z_Back = TCFG_SystemData.MagBackgroundZ;
	
	/* 地磁背景温度补偿值 */
	TCFG_SystemData.MagBackgroudTemp = TCFG_EEPROM_GetMagBackgroudTemp();
	Qmc5883lData.temp_back = TCFG_SystemData.MagBackgroudTemp;
	
	/* 地磁温飘计算值 */
	TCFG_EEPROM_GetMagTempCoef(&TCFG_SystemData.MagCoefX, &TCFG_SystemData.MagCoefY, &TCFG_SystemData.MagCoefZ);
	Qmc5883lData.X_Coef = TCFG_SystemData.MagCoefX;
	Qmc5883lData.Y_Coef = TCFG_SystemData.MagCoefY;
	Qmc5883lData.Z_Coef = TCFG_SystemData.MagCoefZ;
	
	/* 获取设备工作模式 */
	TCFG_SystemData.WorkMode = TCFG_EEPROM_GetWorkMode();
	if ((TCFG_SystemData.WorkMode != DEBUG_WORK) && (TCFG_SystemData.WorkMode != NORMAL_WORK)) {
		TCFG_SystemData.WorkMode = NORMAL_WORK;
		TCFG_EEPROM_SetWorkMode(TCFG_SystemData.WorkMode);
	}
	
	/* 无线通道选择 */
	TCFG_SystemData.RFChannel = TCFG_EEPROM_GetRfChannel();
	RF_CHANNEL1 = TCFG_SystemData.RFChannel;
	
	/* 无线心跳间隔 */
	TCFG_SystemData.Heartinterval = TCFG_EEPROM_GetHeartinterval();
	if ((TCFG_SystemData.Heartinterval > 120) || (TCFG_SystemData.Heartinterval < 1)) {
		TCFG_SystemData.Heartinterval = HEART_INTERVAL;
		TCFG_EEPROM_SetHeartinterval(TCFG_SystemData.Heartinterval);
	}
	
	/* 无线调试输出等级 */
	TCFG_SystemData.RFDprintLv = TCFG_EEPROM_GetRFDprintLv();
	if (TCFG_SystemData.RFDprintLv == 0) {
		TCFG_SystemData.RFDprintLv = RF_DPRINT_LV_TYPE;
		TCFG_EEPROM_SetRFDprintLv(TCFG_SystemData.RFDprintLv);
	}
	
	/* 地磁模式 */
	TCFG_SystemData.MagMode = TCFG_EEPROM_GetMagMode();
	
	/* 雷达次数 */
	TCFG_SystemData.RadarCount = TCFG_EEPROM_GetRadarCount();
	
	/* 雷达调试模式 */
	TCFG_SystemData.RadarDbgMode = TCFG_EEPROM_GetRadarDbgMode();
	
	/* 雷达检测范围 */
	TCFG_SystemData.RadarRange = TCFG_EEPROM_GetRadarRange();
	
	/* 车辆进入延时上报时间 */
	TCFG_SystemData.CarInDelay = TCFG_EEPROM_GetCarInDelay();
	
	/* 车位检测车辆次数 */
	TCFG_SystemData.SpotStatusCount = TCFG_EEPROM_GetStatusCount();
	InspectSpotStatus.SpotStatus[InspectSpotStatus.Rear].spot_count = TCFG_SystemData.SpotStatusCount;
	
	/* 设备重启次数 */
	TCFG_SystemData.DeviceBootCount = TCFG_EEPROM_GetDevBootCnt() + 1;
	TCFG_EEPROM_SetDevBootCnt(TCFG_SystemData.DeviceBootCount);
	
	/* NBIot心跳间隔 */
	TCFG_SystemData.NBIotHeart = TCFG_EEPROM_GetNbiotHeart();
	if ((TCFG_SystemData.NBIotHeart == 0) || (TCFG_SystemData.NBIotHeart > 12)) {
		TCFG_SystemData.NBIotHeart = NBIOT_HEART_DATA_HOURS;
		TCFG_EEPROM_SetNbiotHeart(TCFG_SystemData.NBIotHeart);
	}
	
	/* NBIot重启次数 */
	TCFG_SystemData.NBIotBootCount = TCFG_EEPROM_GetNbiotBootCnt();
	
	/* NBIotPSM模式 */
	TCFG_SystemData.NBIotPSMEnable = TCFG_EEPROM_GetEnableNBiotPSM();
	
	/* NBIot Coap 接收数据次数 */
	TCFG_SystemData.CoapRecvCount = TCFG_EEPROM_GetCoapRecvCnt();
	
	/* NBIot Coap 发送数据次数 */
	TCFG_SystemData.CoapSentCount = TCFG_EEPROM_GetCoapSentCnt();
	
	/* NBIot MqttSN 接收数据次数 */
	TCFG_SystemData.MqttSNRecvCount = TCFG_EEPROM_GetMqttSNRecvCnt();
	
	/* NBIot MqttSN 发送数据次数 */
	TCFG_SystemData.MqttSNSentCount = TCFG_EEPROM_GetMqttSNSentCnt();
	
	/* NBIot OneNET 接收数据次数 */
	TCFG_SystemData.OneNETRecvCount = TCFG_EEPROM_GetOneNETRecvCnt();
	
	/* NBIot OneNET 发送数据次数 */
	TCFG_SystemData.OneNETSentCount = TCFG_EEPROM_GetOneNETSentCnt();
	
	/* Coap间隔时间发送普通数据包用于接收下行数据 */
	TCFG_SystemData.CoapRATimeHour = TCFG_EEPROM_GetCoapRATimeHour();
	if (TCFG_SystemData.CoapRATimeHour == 0) {
		TCFG_SystemData.CoapRATimeHour = 2;
		TCFG_EEPROM_SetCoapRATimeHour(TCFG_SystemData.CoapRATimeHour);
	}
	
	/* Coap保持连接时间 */
	TCFG_SystemData.CoapConnectTime = TCFG_EEPROM_GetCoapConnectTime();
	NbiotClientHandler.CoapConnectTimeSec = TCFG_SystemData.CoapConnectTime;
	
	/* Coap休眠时间 */
	TCFG_SystemData.CoapIdleTime = TCFG_EEPROM_GetCoapIdleTime();
	NbiotClientHandler.CoapIdleTimeSec = TCFG_SystemData.CoapIdleTime;
	
	/* Coap保持连接时间(一天) */
	TCFG_SystemData.CoapConnectDayTime = TCFG_EEPROM_GetCoapConnectDayTime();
	NbiotClientHandler.CoapConnectDayTimeSec = TCFG_SystemData.CoapConnectDayTime;
	
	/* Coap休眠时间(一天) */
	TCFG_SystemData.CoapIdleDayTime = TCFG_EEPROM_GetCoapIdleDayTime();
	NbiotClientHandler.CoapIdleDayTimeSec = TCFG_SystemData.CoapIdleDayTime;
	
	/* Coap使用配额时间(一天) */
	TCFG_SystemData.CoapQuotaTime = TCFG_EEPROM_GetCoapQuotaTime();
	if (TCFG_SystemData.CoapQuotaTime < 500) {
		TCFG_SystemData.CoapQuotaTime = NBCOAP_COAP_QUOTA_TIME_TYPE;
		TCFG_EEPROM_SetCoapQuotaTime(TCFG_SystemData.CoapQuotaTime);
	}
	
	/* NB核心网地址 */
	TCFG_SystemData.NBCoapCDPServer.ip.ip32 = TCFG_EEPROM_GetServerIP();
	TCFG_SystemData.NBCoapCDPServer.port = TCFG_EEPROM_GetServerPort();
	if ((TCFG_SystemData.NBCoapCDPServer.ip.ip32 == 0) && (TCFG_SystemData.NBCoapCDPServer.port == 0)) {
		sscanf(COAPCDPADDR, "%d.%d.%d.%d", &serverip[3], &serverip[2], &serverip[1], &serverip[0]);
		TCFG_SystemData.NBCoapCDPServer.ip.ip8[3] = serverip[3];
		TCFG_SystemData.NBCoapCDPServer.ip.ip8[2] = serverip[2];
		TCFG_SystemData.NBCoapCDPServer.ip.ip8[1] = serverip[1];
		TCFG_SystemData.NBCoapCDPServer.ip.ip8[0] = serverip[0];
		TCFG_SystemData.NBCoapCDPServer.port = COAPCDPPORT;
		TCFG_EEPROM_SetServerIP(TCFG_SystemData.NBCoapCDPServer.ip.ip32);
		TCFG_EEPROM_SetServerPort(TCFG_SystemData.NBCoapCDPServer.port);
	}
}

/**********************************************************************************************************
 @Function			void TCFG_EEPROM_SetBootMode(char bootmode)
 @Description			TCFG_EEPROM_SetBootMode						: 保存Boot启动模式
 @Input				bootmode
 @Return				void
**********************************************************************************************************/
void TCFG_EEPROM_SetBootMode(char bootmode)
{
	FLASH_EEPROM_WriteBuffer(TCFG_ENVFLAG_OFFSET, (unsigned char*)"TCLD", TCFG_ENVFLAG_LENGTH);
	FLASH_EEPROM_WriteByte(TCFG_ENV_BOOTMODE_OFFSET, bootmode);
}

/**********************************************************************************************************
 @Function			char TCFG_EEPROM_GetBootMode(void)
 @Description			TCFG_EEPROM_GetBootMode						: 读取Boot启动模式
 @Input				void
 @Return				bootmode
**********************************************************************************************************/
char TCFG_EEPROM_GetBootMode(void)
{
	return FLASH_EEPROM_ReadByte(TCFG_ENV_BOOTMODE_OFFSET);
}

/**********************************************************************************************************
 @Function			void TCFG_EEPROM_SaveConfigInformation(unsigned int subSN)
 @Description			TCFG_EEPROM_SaveConfigInformation				: 保存配置信息和SN
 @Input				subSN
 @Return				void
**********************************************************************************************************/
void TCFG_EEPROM_SaveConfigInformation(unsigned int subSN)
{
	FLASH_EEPROM_WriteBuffer(TCFG_HEADFLAG_OFFSET, (unsigned char*)"TCLD", TCFG_HEADFLAG_LENGTH);
	FLASH_EEPROM_WriteWord(TCFG_SN_OFFSET, subSN);
}

/**********************************************************************************************************
 @Function			bool TCFG_EEPROM_CheckHeadFlag(char flagtype)
 @Description			TCFG_EEPROM_CheckHeadFlag					: 检测保存信息标志位
 @Input				flagtype
 @Return				bool
**********************************************************************************************************/
bool TCFG_EEPROM_CheckHeadFlag(char flagtype)
{
	if (flagtype == TCFG_FLAGTYPE_GENERAL) {
		if ('T' != FLASH_EEPROM_ReadByte(TCFG_HEADFLAG_OFFSET)) {
			return false;
		}
		if ('C' != FLASH_EEPROM_ReadByte(TCFG_HEADFLAG_OFFSET + 1)) {
			return false;
		}
		if ('L' != FLASH_EEPROM_ReadByte(TCFG_HEADFLAG_OFFSET + 2)) {
			return false;
		}
		if ('D' != FLASH_EEPROM_ReadByte(TCFG_HEADFLAG_OFFSET + 3)) {
			return false;
		}
	}
	else if (flagtype == TCFG_FLAGTYPE_MAGAlGORITHM) {
		if ('T' != FLASH_EEPROM_ReadByte(TCFG_MAGFLAG_OFFSET)) {
			return false;
		}
	}
	else if (flagtype == TCFG_FLAGTYPE_SECU) {
		if ('T' != FLASH_EEPROM_ReadByte(TCFG_SECUFLAG_OFFSET)) {
			return false;
		}
	}
	
	return true;
}

/**********************************************************************************************************
 @Function			void TCFG_EEPROM_SetSpotID(char* spotID)
 @Description			TCFG_EEPROM_SetSpotID						: 保存SpotID
 @Input				&spotID
 @Return				void
**********************************************************************************************************/
void TCFG_EEPROM_SetSpotID(char* spotID)
{
	FLASH_EEPROM_WriteBuffer(TCFG_SPOTID_OFFSET, (unsigned char*)spotID, TCFG_SPOTID_LENGTH);
}

/**********************************************************************************************************
 @Function			void TCFG_EEPROM_GetSpotID(char* spotID)
 @Description			TCFG_EEPROM_GetSpotID						: 读取SpotID
 @Input				&spotID
 @Return				void
**********************************************************************************************************/
void TCFG_EEPROM_GetSpotID(char* spotID)
{
	FLASH_EEPROM_ReadBuffer(TCFG_SPOTID_OFFSET, (unsigned char*)spotID, TCFG_SPOTID_LENGTH);
	if (strlen(spotID) >= TCFG_SPOTID_LENGTH) {
		strcpy(spotID, "unset");
	}
}

/**********************************************************************************************************
 @Function			void TCFG_EEPROM_SetHeartinterval(unsigned short val)
 @Description			TCFG_EEPROM_SetHeartinterval					: 保存Heartinterval
 @Input				val
 @Return				void
**********************************************************************************************************/
void TCFG_EEPROM_SetHeartinterval(unsigned short val)
{
	FLASH_EEPROM_WriteHalfWord(TCFG_HEARTINTERVAL_OFFSET, val);
}

/**********************************************************************************************************
 @Function			unsigned short TCFG_EEPROM_GetHeartinterval(void)
 @Description			TCFG_EEPROM_GetHeartinterval					: 读取Heartinterval
 @Input				void
 @Return				val
**********************************************************************************************************/
unsigned short TCFG_EEPROM_GetHeartinterval(void)
{
	return FLASH_EEPROM_ReadHalfWord(TCFG_HEARTINTERVAL_OFFSET);
}

/**********************************************************************************************************
 @Function			void TCFG_EEPROM_SetSavedSensitivity(unsigned char val)
 @Description			TCFG_EEPROM_SetSavedSensitivity				: 保存Sensitivity
 @Input				val
 @Return				void
**********************************************************************************************************/
void TCFG_EEPROM_SetSavedSensitivity(unsigned char val)
{
	FLASH_EEPROM_WriteByte(TCFG_MAG_SENSITIVITY_OFFSET, val);
}

/**********************************************************************************************************
 @Function			unsigned char TCFG_EEPROM_GetSavedSensitivity(void)
 @Description			TCFG_EEPROM_GetSavedSensitivity				: 读取Sensitivity
 @Input				void
 @Return				val
**********************************************************************************************************/
unsigned char TCFG_EEPROM_GetSavedSensitivity(void)
{
	return FLASH_EEPROM_ReadByte(TCFG_MAG_SENSITIVITY_OFFSET);
}

/**********************************************************************************************************
 @Function			void TCFG_EEPROM_SetBootVersion(unsigned char val)
 @Description			TCFG_EEPROM_SetBootVersion					: 保存BootVersion
 @Input				val
 @Return				void
**********************************************************************************************************/
void TCFG_EEPROM_SetBootVersion(unsigned char val)
{
	FLASH_EEPROM_WriteByte(TCFG_BOOT_VERSION_OFFSET, val);
}

/**********************************************************************************************************
 @Function			unsigned char TCFG_EEPROM_GetBootVersion(void)
 @Description			TCFG_EEPROM_GetBootVersion					: 读取BootVersion
 @Input				void
 @Return				val
**********************************************************************************************************/
unsigned char TCFG_EEPROM_GetBootVersion(void)
{
	return FLASH_EEPROM_ReadByte(TCFG_BOOT_VERSION_OFFSET);
}

/**********************************************************************************************************
 @Function			void TCFG_EEPROM_SetMagBackgroud(int16_t x_axis, int16_t y_axis, int16_t z_axis)
 @Description			TCFG_EEPROM_SetMagBackgroud					: 保存地磁背景值
 @Input				x_axis
					y_axis
					z_axis
 @Return				void
**********************************************************************************************************/
void TCFG_EEPROM_SetMagBackgroud(int16_t x_axis, int16_t y_axis, int16_t z_axis)
{
	FLASH_EEPROM_WriteHalfWord(TCFG_MAG_BACK_X_OFFSET, x_axis);
	FLASH_EEPROM_WriteHalfWord(TCFG_MAG_BACK_Y_OFFSET, y_axis);
	FLASH_EEPROM_WriteHalfWord(TCFG_MAG_BACK_Z_OFFSET, z_axis);
	
	FLASH_EEPROM_WriteByte(TCFG_MAGFLAG_OFFSET, 'T');
}

/**********************************************************************************************************
 @Function			unsigned short TCFG_EEPROM_GetMagBackgroud(char axis)
 @Description			TCFG_EEPROM_GetMagBackgroud					: 读取地磁背景值
 @Input				axis
 @Return				val
**********************************************************************************************************/
unsigned short TCFG_EEPROM_GetMagBackgroud(char axis)
{
	if (true == TCFG_EEPROM_CheckHeadFlag(TCFG_FLAGTYPE_MAGAlGORITHM)) {
		if (axis == TCFG_X_AXIS) {
			return FLASH_EEPROM_ReadHalfWord(TCFG_MAG_BACK_X_OFFSET);
		}
		else if (axis == TCFG_Y_AXIS) {
			return FLASH_EEPROM_ReadHalfWord(TCFG_MAG_BACK_Y_OFFSET);
		}
		else if (axis == TCFG_Z_AXIS) {
			return FLASH_EEPROM_ReadHalfWord(TCFG_MAG_BACK_Z_OFFSET);
		}
	}
	
	return 0x7FFF;
}

/**********************************************************************************************************
 @Function			void TCFG_EEPROM_SetMagTempCoef(signed char coef_x, signed char coef_y, signed char coef_z)
 @Description			TCFG_EEPROM_SetMagTempCoef					: 保存地磁温飘计算值
 @Input				coef_x
					coef_y
					coef_z
 @Return				void
**********************************************************************************************************/
void TCFG_EEPROM_SetMagTempCoef(signed char coef_x, signed char coef_y, signed char coef_z)
{
	int sval32;
	
	sval32 = Qmc5883lData.X_Back*(10000+Qmc5883lData.X_Coef*Qmc5883lData.temp_back)/(10000+coef_x*Qmc5883lData.temp_back);
	if (sval32 > 32767)
		Qmc5883lData.X_Back = 32767;
	else if (sval32 < -32768)
		Qmc5883lData.X_Back = -32768;
	else
		Qmc5883lData.X_Back = sval32;
	
	sval32 = Qmc5883lData.Y_Back*(10000+Qmc5883lData.Y_Coef*Qmc5883lData.temp_back)/(10000+coef_y*Qmc5883lData.temp_back);
	if (sval32 > 32767)
		Qmc5883lData.Y_Back = 32767;
	else if (sval32 < -32768)
		Qmc5883lData.Y_Back = -32768;
	else
		Qmc5883lData.Y_Back = sval32;
	
	sval32 = Qmc5883lData.Z_Back*(10000+Qmc5883lData.Z_Coef*Qmc5883lData.temp_back)/(10000+coef_z*Qmc5883lData.temp_back);
	if (sval32 > 32767)
		Qmc5883lData.Z_Back = 32767;
	else if (sval32 < -32768)
		Qmc5883lData.Z_Back = -32768;
	else
		Qmc5883lData.Z_Back = sval32;
	
	Qmc5883lData.X_Coef = coef_x;
	Qmc5883lData.Y_Coef = coef_y;
	Qmc5883lData.Z_Coef = coef_z;
	
	FLASH_EEPROM_WriteByte(TCFG_MAG_COEF_X_OFFSET, coef_x);
	FLASH_EEPROM_WriteByte(TCFG_MAG_COEF_Y_OFFSET, coef_y);
	FLASH_EEPROM_WriteByte(TCFG_MAG_COEF_Z_OFFSET, coef_z);
}

/**********************************************************************************************************
 @Function			void TCFG_EEPROM_GetMagTempCoef(signed char* coef_x, signed char* coef_y, signed char* coef_z)
 @Description			TCFG_EEPROM_GetMagTempCoef					: 读取地磁温飘计算值
 @Input				*coef_x
					*coef_y
					*coef_z
 @Return				val
**********************************************************************************************************/
void TCFG_EEPROM_GetMagTempCoef(signed char* coef_x, signed char* coef_y, signed char* coef_z)
{
	*coef_x = FLASH_EEPROM_ReadByte(TCFG_MAG_COEF_X_OFFSET);
	*coef_y = FLASH_EEPROM_ReadByte(TCFG_MAG_COEF_Y_OFFSET);
	*coef_z = FLASH_EEPROM_ReadByte(TCFG_MAG_COEF_Z_OFFSET);
	
	return ;
}

/**********************************************************************************************************
 @Function			void TCFG_EEPROM_SetMagBackgroudTemp(short temp)
 @Description			TCFG_EEPROM_SetMagBackgroudTemp				: 保存MagBackgroudTemp
 @Input				temp
 @Return				void
**********************************************************************************************************/
void TCFG_EEPROM_SetMagBackgroudTemp(short temp)
{
	FLASH_EEPROM_WriteHalfWord(TCFG_MAG_BACK_TEMP_OFFSET, temp);
}

/**********************************************************************************************************
 @Function			short TCFG_EEPROM_GetMagBackgroudTemp(void)
 @Description			TCFG_EEPROM_GetMagBackgroudTemp				: 读取MagBackgroudTemp
 @Input				void
 @Return				temp
**********************************************************************************************************/
short TCFG_EEPROM_GetMagBackgroudTemp(void)
{
	return FLASH_EEPROM_ReadHalfWord(TCFG_MAG_BACK_TEMP_OFFSET);
}

/**********************************************************************************************************
 @Function			void TCFG_EEPROM_SetSubSN(unsigned int subsn)
 @Description			TCFG_EEPROM_SetSubSN						: 保存SubSN
 @Input				subsn
 @Return				void
**********************************************************************************************************/
void TCFG_EEPROM_SetSubSN(unsigned int subsn)
{
	FLASH_EEPROM_WriteWord(TCFG_SN_OFFSET, subsn);
}

/**********************************************************************************************************
 @Function			unsigned int TCFG_EEPROM_GetSubSN(void)
 @Description			TCFG_EEPROM_GetSubSN						: 读取SubSN
 @Input				void
 @Return				subsn
**********************************************************************************************************/
unsigned int TCFG_EEPROM_GetSubSN(void)
{
	return FLASH_EEPROM_ReadWord(TCFG_SN_OFFSET);
}

/**********************************************************************************************************
 @Function			void TCFG_EEPROM_SetBrandCode(unsigned int brand)
 @Description			TCFG_EEPROM_SetBrandCode						: 保存BrandCode
 @Input				brand
 @Return				void
**********************************************************************************************************/
void TCFG_EEPROM_SetBrandCode(unsigned int brand)
{
	FLASH_EEPROM_WriteWord(TCFG_SECU_BRAND_OFFSET, brand);
}

/**********************************************************************************************************
 @Function			unsigned int TCFG_EEPROM_GetBrandCode(void)
 @Description			TCFG_EEPROM_GetBrandCode						: 读取BrandCode
 @Input				void
 @Return				brand
**********************************************************************************************************/
unsigned int TCFG_EEPROM_GetBrandCode(void)
{
	return FLASH_EEPROM_ReadWord(TCFG_SECU_BRAND_OFFSET);
}

/**********************************************************************************************************
 @Function			void TCFG_EEPROM_SetBootCount(unsigned char count)
 @Description			TCFG_EEPROM_SetBootCount						: 保存BootCount
 @Input				count
 @Return				void
**********************************************************************************************************/
void TCFG_EEPROM_SetBootCount(unsigned char count)
{
	FLASH_EEPROM_WriteByte(TCFG_RECORD_BOOTCNT_OFFSET, count);
}

/**********************************************************************************************************
 @Function			unsigned char TCFG_EEPROM_GetBootCount(void)
 @Description			TCFG_EEPROM_GetBootCount						: 读取BootCount
 @Input				void
 @Return				count
**********************************************************************************************************/
unsigned char TCFG_EEPROM_GetBootCount(void)
{
	return FLASH_EEPROM_ReadByte(TCFG_RECORD_BOOTCNT_OFFSET);
}

/**********************************************************************************************************
 @Function			void TCFG_EEPROM_SetServerIP(unsigned int val)
 @Description			TCFG_EEPROM_SetServerIP						: 保存ServerIP
 @Input				val
 @Return				void
**********************************************************************************************************/
void TCFG_EEPROM_SetServerIP(unsigned int val)
{
	FLASH_EEPROM_WriteWord(TCFG_RECORD_SERVER_OFFSET, val);
}

/**********************************************************************************************************
 @Function			unsigned int TCFG_EEPROM_GetServerIP(void)
 @Description			TCFG_EEPROM_GetServerIP						: 读取ServerIP
 @Input				void
 @Return				val
**********************************************************************************************************/
unsigned int TCFG_EEPROM_GetServerIP(void)
{
	return FLASH_EEPROM_ReadWord(TCFG_RECORD_SERVER_OFFSET);
}

/**********************************************************************************************************
 @Function			void TCFG_EEPROM_SetServerPort(unsigned short val)
 @Description			TCFG_EEPROM_SetServerPort					: 保存ServerPort
 @Input				val
 @Return				void
**********************************************************************************************************/
void TCFG_EEPROM_SetServerPort(unsigned short val)
{
	FLASH_EEPROM_WriteHalfWord(TCFG_RECORD_SERVER_OFFSET + 4, val);
}

/**********************************************************************************************************
 @Function			unsigned short TCFG_EEPROM_GetServerPort(void)
 @Description			TCFG_EEPROM_GetServerPort					: 读取ServerPort
 @Input				void
 @Return				val
**********************************************************************************************************/
unsigned short TCFG_EEPROM_GetServerPort(void)
{
	return FLASH_EEPROM_ReadHalfWord(TCFG_RECORD_SERVER_OFFSET + 4);
}

/**********************************************************************************************************
 @Function			char* TCFG_EEPROM_Get_ServerIP_String(void)
 @Description			TCFG_EEPROM_Get_ServerIP_String				: 读取ServerIP字符串
 @Input				void
 @Return				ServerIP_string
**********************************************************************************************************/
char* TCFG_EEPROM_Get_ServerIP_String(void)
{
	TCFG_SystemData.NBCoapCDPServer.ip.ip32 = TCFG_EEPROM_GetServerIP();
	
	memset((void*)&TCFG_SystemData.NBCoapCDPServerIP, 0, sizeof(TCFG_SystemData.NBCoapCDPServerIP));
	sprintf((char *)TCFG_SystemData.NBCoapCDPServerIP, "%d.%d.%d.%d", 
	TCFG_SystemData.NBCoapCDPServer.ip.ip8[3], TCFG_SystemData.NBCoapCDPServer.ip.ip8[2], 
	TCFG_SystemData.NBCoapCDPServer.ip.ip8[1], TCFG_SystemData.NBCoapCDPServer.ip.ip8[0]);
	
	return (char *)TCFG_SystemData.NBCoapCDPServerIP;
}

/**********************************************************************************************************
 @Function			char* TCFG_EEPROM_Get_ServerPort_String(void)
 @Description			TCFG_EEPROM_Get_ServerPort_String				: 读取ServerPort字符串
 @Input				void
 @Return				ServerPort_string
**********************************************************************************************************/
char* TCFG_EEPROM_Get_ServerPort_String(void)
{
	TCFG_SystemData.NBCoapCDPServer.port = TCFG_EEPROM_GetServerPort();
	
	memset((void*)&TCFG_SystemData.NBCoapCDPServerPort, 0, sizeof(TCFG_SystemData.NBCoapCDPServerPort));
	sprintf((char *)TCFG_SystemData.NBCoapCDPServerPort, "%d", TCFG_SystemData.NBCoapCDPServer.port);
	
	return (char *)TCFG_SystemData.NBCoapCDPServerPort;
}

/**********************************************************************************************************
 @Function			void TCFG_EEPROM_SetWorkMode(unsigned char val)
 @Description			TCFG_EEPROM_SetWorkMode						: 保存WorkMode
 @Input				val
 @Return				void
**********************************************************************************************************/
void TCFG_EEPROM_SetWorkMode(unsigned char val)
{
	FLASH_EEPROM_WriteByte(TCFG_WORKMODE_OFFSET, val);
}

/**********************************************************************************************************
 @Function			unsigned char TCFG_EEPROM_GetWorkMode(void)
 @Description			TCFG_EEPROM_GetWorkMode						: 读取WorkMode
 @Input				void
 @Return				val
**********************************************************************************************************/
unsigned char TCFG_EEPROM_GetWorkMode(void)
{
	return FLASH_EEPROM_ReadByte(TCFG_WORKMODE_OFFSET);
}

/**********************************************************************************************************
 @Function			char* TCFG_EEPROM_Get_WorkMode_String(void)
 @Description			TCFG_EEPROM_Get_WorkMode_String				: 读取WorkMode字符串
 @Input				void
 @Return				workmode_string
**********************************************************************************************************/
char* TCFG_EEPROM_Get_WorkMode_String(void)
{
	if (DeviceIdleMode == true) {
		memset((void*)&TCFG_SystemData.WorkModeStr, 0, sizeof(TCFG_SystemData.WorkModeStr));
		strncpy((char *)TCFG_SystemData.WorkModeStr, "idle", strlen("idle"));
	}
	else if (DeviceActivedMode != true) {
		memset((void*)&TCFG_SystemData.WorkModeStr, 0, sizeof(TCFG_SystemData.WorkModeStr));
		strncpy((char *)TCFG_SystemData.WorkModeStr, "notact", strlen("notact"));
	}
	else if (TCFG_SystemData.WorkMode == NORMAL_WORK) {
		memset((void*)&TCFG_SystemData.WorkModeStr, 0, sizeof(TCFG_SystemData.WorkModeStr));
		strncpy((char *)TCFG_SystemData.WorkModeStr, "norm", strlen("norm"));
	}
	else if (TCFG_SystemData.WorkMode == DEBUG_WORK) {
		memset((void*)&TCFG_SystemData.WorkModeStr, 0, sizeof(TCFG_SystemData.WorkModeStr));
		strncpy((char *)TCFG_SystemData.WorkModeStr, "debug", strlen("debug"));
	}
	else {
		memset((void*)&TCFG_SystemData.WorkModeStr, 0, sizeof(TCFG_SystemData.WorkModeStr));
		strncpy((char *)TCFG_SystemData.WorkModeStr, "unknown", strlen("unknown"));
	}
	
	return (char*)TCFG_SystemData.WorkModeStr;
}

/**********************************************************************************************************
 @Function			void TCFG_EEPROM_SetRadarCount(unsigned int val)
 @Description			TCFG_EEPROM_SetRadarCount					: 保存RadarCount
 @Input				val
 @Return				void
**********************************************************************************************************/
void TCFG_EEPROM_SetRadarCount(unsigned int val)
{
	FLASH_EEPROM_WriteWord(TCFG_RADARCOUNT_OFFSET, val);
}

/**********************************************************************************************************
 @Function			unsigned int TCFG_EEPROM_GetRadarCount(void)
 @Description			TCFG_EEPROM_GetRadarCount					: 读取RadarCount
 @Input				void
 @Return				val
**********************************************************************************************************/
unsigned int TCFG_EEPROM_GetRadarCount(void)
{
	return FLASH_EEPROM_ReadWord(TCFG_RADARCOUNT_OFFSET);
}

/**********************************************************************************************************
 @Function			void TCFG_AddRadarCount(void)
 @Description			TCFG_AddRadarCount							: RadarCount++
 @Input				void
 @Return				void
**********************************************************************************************************/
void TCFG_AddRadarCount(void)
{
	TCFG_SystemData.RadarCount++;
}

/**********************************************************************************************************
 @Function			unsigned int TCFG_GetRadarCount(void)
 @Description			TCFG_GetRadarCount							: 获取RadarCount
 @Input				void
 @Return				val
**********************************************************************************************************/
unsigned int TCFG_GetRadarCount(void)
{
	return TCFG_SystemData.RadarCount;
}

/**********************************************************************************************************
 @Function			void TCFG_EEPROM_SetRadarDbgMode(unsigned char val)
 @Description			TCFG_EEPROM_SetRadarDbgMode					: 保存RadarDbgMode
 @Input				val
 @Return				void
**********************************************************************************************************/
void TCFG_EEPROM_SetRadarDbgMode(unsigned char val)
{
	if (val > 8) val = 0;
	else val = 8 - val;
	FLASH_EEPROM_WriteByte(TCFG_RADARDBG_OFFSET, val);
}

/**********************************************************************************************************
 @Function			unsigned char TCFG_EEPROM_GetRadarDbgMode(void)
 @Description			TCFG_EEPROM_GetRadarDbgMode					: 读取RadarDbgMode
 @Input				void
 @Return				val
**********************************************************************************************************/
unsigned char TCFG_EEPROM_GetRadarDbgMode(void)
{
	unsigned char val8;
	
	val8 = FLASH_EEPROM_ReadByte(TCFG_RADARDBG_OFFSET);
	if (val8 > 8) return 0;
	else val8 = 8 - val8;
	
	return val8;
}

/**********************************************************************************************************
 @Function			void TCFG_EEPROM_SetStatusCount(unsigned int val)
 @Description			TCFG_EEPROM_SetStatusCount					: 保存StatusCount
 @Input				val
 @Return				void
**********************************************************************************************************/
void TCFG_EEPROM_SetStatusCount(unsigned int val)
{
	FLASH_EEPROM_WriteWord(TCFG_STATUSCNT_OFFSET, val);
}

/**********************************************************************************************************
 @Function			unsigned int TCFG_EEPROM_GetStatusCount(void)
 @Description			TCFG_EEPROM_GetStatusCount					: 读取StatusCount
 @Input				void
 @Return				val
**********************************************************************************************************/
unsigned int TCFG_EEPROM_GetStatusCount(void)
{
	return FLASH_EEPROM_ReadWord(TCFG_STATUSCNT_OFFSET);
}

/**********************************************************************************************************
 @Function			void TCFG_EEPROM_SetRfChannel(unsigned char val)
 @Description			TCFG_EEPROM_SetRfChannel						: 保存RfChannel
 @Input				val
 @Return				void
**********************************************************************************************************/
void TCFG_EEPROM_SetRfChannel(unsigned char val)
{
	FLASH_EEPROM_WriteByte(TCFG_RFCHANNEL_OFFSET, val);
}

/**********************************************************************************************************
 @Function			unsigned char TCFG_EEPROM_GetRfChannel(void)
 @Description			TCFG_EEPROM_GetRfChannel						: 读取RfChannel
 @Input				void
 @Return				val
**********************************************************************************************************/
unsigned char TCFG_EEPROM_GetRfChannel(void)
{
	unsigned char val;
	
	val = FLASH_EEPROM_ReadByte(TCFG_RFCHANNEL_OFFSET);
	switch (val)
	{
	case 36:
		val = 36;
		break;
	case 4:
		val = 4;
		break;
	case 6:
		val = 6;
		break;
	case 16:
		val = 16;
		break;
	case 26:
		val = 26;
		break;
	default:
		val = 36;
		break;
	}
	
	return val;
}

/**********************************************************************************************************
 @Function			void TCFG_EEPROM_SetRFDprintLv(uint8_t val)
 @Description			TCFG_EEPROM_SetRFDprintLv					: 保存RFDprintLv
 @Input				val
 @Return				void
**********************************************************************************************************/
void TCFG_EEPROM_SetRFDprintLv(uint8_t val)
{
	FLASH_EEPROM_WriteByte(TCFG_RF_DPRINT_LV_OFFSET, val);
}

/**********************************************************************************************************
 @Function			unsigned char TCFG_EEPROM_GetRFDprintLv(void)
 @Description			TCFG_EEPROM_GetRFDprintLv					: 读取RFDprintLv
 @Input				void
 @Return				val
**********************************************************************************************************/
unsigned char TCFG_EEPROM_GetRFDprintLv(void)
{
	return FLASH_EEPROM_ReadByte(TCFG_RF_DPRINT_LV_OFFSET);
}

/**********************************************************************************************************
 @Function			void TCFG_EEPROM_SetEnableNBiotPSM(unsigned char val)
 @Description			TCFG_EEPROM_SetEnableNBiotPSM					: 保存EnableNBiotPSM
 @Input				val
 @Return				void
**********************************************************************************************************/
void TCFG_EEPROM_SetEnableNBiotPSM(unsigned char val)
{
	FLASH_EEPROM_WriteByte(TCFG_ENABLE_NBIOTPSM_OFFSET, val);
}

/**********************************************************************************************************
 @Function			unsigned char TCFG_EEPROM_GetEnableNBiotPSM(void)
 @Description			TCFG_EEPROM_GetEnableNBiotPSM					: 读取EnableNBiotPSM
 @Input				void
 @Return				val
**********************************************************************************************************/
unsigned char TCFG_EEPROM_GetEnableNBiotPSM(void)
{
	return FLASH_EEPROM_ReadByte(TCFG_ENABLE_NBIOTPSM_OFFSET);
}

/**********************************************************************************************************
 @Function			void TCFG_EEPROM_SetActiveDeice(unsigned char val)
 @Description			TCFG_EEPROM_SetActiveDeice					: 保存ActiveDeice
 @Input				val
 @Return				void
**********************************************************************************************************/
void TCFG_EEPROM_SetActiveDevice(unsigned char val)
{
	if (val) val = 0;
	else val = 1;
	FLASH_EEPROM_WriteByte(TCFG_ACTIVE_DEVICE_OFFSET, val);
}

/**********************************************************************************************************
 @Function			unsigned char TCFG_EEPROM_GetActiveDeice(void)
 @Description			TCFG_EEPROM_GetActiveDeice					: 读取ActiveDeice
 @Input				void
 @Return				val
**********************************************************************************************************/
unsigned char TCFG_EEPROM_GetActiveDevice(void)
{
	if (1 == FLASH_EEPROM_ReadByte(TCFG_ACTIVE_DEVICE_OFFSET)) {
		return 0;
	}
	else {
		return 1;
	}
}

/**********************************************************************************************************
 @Function			void TCFG_EEPROM_SetNbiotBootCnt(unsigned int val)
 @Description			TCFG_EEPROM_SetNbiotBootCnt					: 保存NbiotBootCnt
 @Input				val
 @Return				void
**********************************************************************************************************/
void TCFG_EEPROM_SetNbiotBootCnt(unsigned int val)
{
	FLASH_EEPROM_WriteWord(TCFG_NBIOT_BOOTCNT_OFFSET, val);
}

/**********************************************************************************************************
 @Function			unsigned int TCFG_EEPROM_GetNbiotBootCnt(void)
 @Description			TCFG_EEPROM_GetNbiotBootCnt					: 读取NbiotBootCnt
 @Input				void
 @Return				val
**********************************************************************************************************/
unsigned int TCFG_EEPROM_GetNbiotBootCnt(void)
{
	return FLASH_EEPROM_ReadWord(TCFG_NBIOT_BOOTCNT_OFFSET);
}

/**********************************************************************************************************
 @Function			void TCFG_EEPROM_SetCoapSentCnt(unsigned int val)
 @Description			TCFG_EEPROM_SetCoapSentCnt					: 保存CoapSentCnt
 @Input				val
 @Return				void
**********************************************************************************************************/
void TCFG_EEPROM_SetCoapSentCnt(unsigned int val)
{
	FLASH_EEPROM_WriteWord(TCFG_COAP_SENTCNT_OFFSET, val);
}

/**********************************************************************************************************
 @Function			unsigned int TCFG_EEPROM_GetCoapSentCnt(void)
 @Description			TCFG_EEPROM_GetCoapSentCnt					: 读取CoapSentCnt
 @Input				void
 @Return				val
**********************************************************************************************************/
unsigned int TCFG_EEPROM_GetCoapSentCnt(void)
{
	return FLASH_EEPROM_ReadWord(TCFG_COAP_SENTCNT_OFFSET);
}

/**********************************************************************************************************
 @Function			void TCFG_EEPROM_SetCoapRecvCnt(unsigned int val)
 @Description			TCFG_EEPROM_SetCoapRecvCnt					: 保存CoapRecvCnt
 @Input				val
 @Return				void
**********************************************************************************************************/
void TCFG_EEPROM_SetCoapRecvCnt(unsigned int val)
{
	FLASH_EEPROM_WriteWord(TCFG_COAP_RECVCNT_OFFSET, val);
}

/**********************************************************************************************************
 @Function			unsigned int TCFG_EEPROM_GetCoapRecvCnt(void)
 @Description			TCFG_EEPROM_GetCoapRecvCnt					: 读取CoapRecvCnt
 @Input				void
 @Return				val
**********************************************************************************************************/
unsigned int TCFG_EEPROM_GetCoapRecvCnt(void)
{
	return FLASH_EEPROM_ReadWord(TCFG_COAP_RECVCNT_OFFSET);
}

/**********************************************************************************************************
 @Function			void TCFG_EEPROM_SetMqttSNSentCnt(unsigned int val)
 @Description			TCFG_EEPROM_SetMqttSNSentCnt					: 保存MqttSNSentCnt
 @Input				val
 @Return				void
**********************************************************************************************************/
void TCFG_EEPROM_SetMqttSNSentCnt(unsigned int val)
{
	FLASH_EEPROM_WriteWord(TCFG_MQTTSN_SENTCNT_OFFSET, val);
}

/**********************************************************************************************************
 @Function			unsigned int TCFG_EEPROM_GetMqttSNSentCnt(void)
 @Description			TCFG_EEPROM_GetMqttSNSentCnt					: 读取MqttSNSentCnt
 @Input				void
 @Return				val
**********************************************************************************************************/
unsigned int TCFG_EEPROM_GetMqttSNSentCnt(void)
{
	return FLASH_EEPROM_ReadWord(TCFG_MQTTSN_SENTCNT_OFFSET);
}

/**********************************************************************************************************
 @Function			void TCFG_EEPROM_SetMqttSNRecvCnt(unsigned int val)
 @Description			TCFG_EEPROM_SetMqttSNRecvCnt					: 保存MqttSNRecvCnt
 @Input				val
 @Return				void
**********************************************************************************************************/
void TCFG_EEPROM_SetMqttSNRecvCnt(unsigned int val)
{
	FLASH_EEPROM_WriteWord(TCFG_MQTTSN_RECVCNT_OFFSET, val);
}

/**********************************************************************************************************
 @Function			unsigned int TCFG_EEPROM_GetMqttSNRecvCnt(void)
 @Description			TCFG_EEPROM_GetMqttSNRecvCnt					: 读取MqttSNRecvCnt
 @Input				void
 @Return				val
**********************************************************************************************************/
unsigned int TCFG_EEPROM_GetMqttSNRecvCnt(void)
{
	return FLASH_EEPROM_ReadWord(TCFG_MQTTSN_RECVCNT_OFFSET);
}

/**********************************************************************************************************
 @Function			void TCFG_EEPROM_SetOneNETSentCnt(unsigned int val)
 @Description			TCFG_EEPROM_SetOneNETSentCnt					: 保存OneNETSentCnt
 @Input				val
 @Return				void
**********************************************************************************************************/
void TCFG_EEPROM_SetOneNETSentCnt(unsigned int val)
{
	FLASH_EEPROM_WriteWord(TCFG_ONENET_SENTCNT_OFFSET, val);
}

/**********************************************************************************************************
 @Function			unsigned int TCFG_EEPROM_GetOneNETSentCnt(void)
 @Description			TCFG_EEPROM_GetOneNETSentCnt					: 读取OneNETSentCnt
 @Input				void
 @Return				val
**********************************************************************************************************/
unsigned int TCFG_EEPROM_GetOneNETSentCnt(void)
{
	return FLASH_EEPROM_ReadWord(TCFG_ONENET_SENTCNT_OFFSET);
}

/**********************************************************************************************************
 @Function			void TCFG_EEPROM_SetOneNETRecvCnt(unsigned int val)
 @Description			TCFG_EEPROM_SetOneNETRecvCnt					: 保存OneNETRecvCnt
 @Input				val
 @Return				void
**********************************************************************************************************/
void TCFG_EEPROM_SetOneNETRecvCnt(unsigned int val)
{
	FLASH_EEPROM_WriteWord(TCFG_ONENET_RECVCNT_OFFSET, val);
}

/**********************************************************************************************************
 @Function			unsigned int TCFG_EEPROM_GetOneNETRecvCnt(void)
 @Description			TCFG_EEPROM_GetOneNETRecvCnt					: 读取OneNETRecvCnt
 @Input				void
 @Return				val
**********************************************************************************************************/
unsigned int TCFG_EEPROM_GetOneNETRecvCnt(void)
{
	return FLASH_EEPROM_ReadWord(TCFG_ONENET_RECVCNT_OFFSET);
}

/**********************************************************************************************************
 @Function			void TCFG_EEPROM_SetDevBootCnt(unsigned short val)
 @Description			TCFG_EEPROM_SetDevBootCnt					: 保存DevBootCnt
 @Input				val
 @Return				void
**********************************************************************************************************/
void TCFG_EEPROM_SetDevBootCnt(unsigned short val)
{
	FLASH_EEPROM_WriteHalfWord(TCFG_DEV_BOOTCNT_OFFSET, val);
}

/**********************************************************************************************************
 @Function			unsigned short TCFG_EEPROM_GetDevBootCnt(void)
 @Description			TCFG_EEPROM_GetDevBootCnt					: 读取DevBootCnt
 @Input				void
 @Return				val
**********************************************************************************************************/
unsigned short TCFG_EEPROM_GetDevBootCnt(void)
{
	return FLASH_EEPROM_ReadHalfWord(TCFG_DEV_BOOTCNT_OFFSET);
}

/**********************************************************************************************************
 @Function			void TCFG_EEPROM_SetEventTime(unsigned int val)
 @Description			TCFG_EEPROM_SetEventTime						: 保存EventTime
 @Input				val
 @Return				void
**********************************************************************************************************/
void TCFG_EEPROM_SetEventTime(unsigned int val)
{
	FLASH_EEPROM_WriteWord(TCFG_EVENT_TIME_OFFSET, val);
}

/**********************************************************************************************************
 @Function			unsigned int TCFG_EEPROM_GetEventTime(void)
 @Description			TCFG_EEPROM_GetEventTime						: 读取EventTime
 @Input				void
 @Return				val
**********************************************************************************************************/
unsigned int TCFG_EEPROM_GetEventTime(void)
{
	return FLASH_EEPROM_ReadWord(TCFG_EVENT_TIME_OFFSET);
}

/**********************************************************************************************************
 @Function			void TCFG_EEPROM_SetBackgroundTemp(int16_t val)
 @Description			TCFG_EEPROM_SetBackgroundTemp					: 保存BackgroundTemp
 @Input				val
 @Return				void
**********************************************************************************************************/
void TCFG_EEPROM_SetBackgroundTemp(int16_t val)
{
	FLASH_EEPROM_WriteHalfWord(TCFG_TEMP_BACKGROUND_OFFSET, val);
}

/**********************************************************************************************************
 @Function			int16_t TCFG_EEPROM_GetBackgroundTemp(void)
 @Description			TCFG_EEPROM_GetBackgroundTemp					: 读取BackgroundTemp
 @Input				void
 @Return				val
**********************************************************************************************************/
int16_t TCFG_EEPROM_GetBackgroundTemp(void)
{
	return FLASH_EEPROM_ReadHalfWord(TCFG_TEMP_BACKGROUND_OFFSET);
}

/**********************************************************************************************************
 @Function			bool TCFG_EEPROM_CheckInfoBurned(void)
 @Description			TCFG_EEPROM_CheckInfoBurned					: 检测生产商信息
 @Input				void
 @Return				bool
**********************************************************************************************************/
bool TCFG_EEPROM_CheckInfoBurned(void)
{
	if ( (0x00 == FLASH_EEPROM_ReadByte(TCFG_FACTORY_BRAND_SN_OFFSET + 0)) && \
		(0x00 == FLASH_EEPROM_ReadByte(TCFG_FACTORY_BRAND_SN_OFFSET + 1)) && \
		(0x00 == FLASH_EEPROM_ReadByte(TCFG_FACTORY_BRAND_SN_OFFSET + 2)) && \
		(0x00 == FLASH_EEPROM_ReadByte(TCFG_FACTORY_BRAND_SN_OFFSET + 3)) ) {
		return false;
	}
	else {
		return true;
	}
}

/**********************************************************************************************************
 @Function			bool TCFG_EEPROM_CheckNewSNorBrand(void)
 @Description			TCFG_EEPROM_CheckNewSNorBrand					: 检测新的设备号或厂牌信息
 @Input				void
 @Return				bool
**********************************************************************************************************/
bool TCFG_EEPROM_CheckNewSNorBrand(void)
{
	bool ret = false;
	char vender[4];
	unsigned int Brand = 0;
	
	/* 之前不为此固件,备份域为空 */
	if ((TCFG_EEPROM_GetSNfromBrandKey() == 0) && (TCFG_EEPROM_GetFactoryBrand() == 0)) {
		/* 写入生产商与设备号至备份域 */
		TCFG_EEPROM_SetSNfromBrandKey(TCFG_EEPROM_Get_MAC_SN());
		TCFG_EEPROM_GetVender(vender);
		Brand |= (vender[0] & 0x000000FF) << 3*8;
		Brand |= (vender[1] & 0x000000FF) << 2*8;
		Brand |= (vender[2] & 0x000000FF) << 1*8;
		Brand |= (vender[3] & 0x000000FF) << 0*8;
		TCFG_EEPROM_SetFactoryBrand(Brand);
		ret = false;
		goto exit;
	}
	
	/* 设备号与备份域设备号不同,需恢复EEPROM出厂设置 */
	if (TCFG_EEPROM_Get_MAC_SN() != TCFG_EEPROM_GetSNfromBrandKey()) {			//设备号不同
		ret = true;
		goto exit;
	}
	
	/* 厂牌号与备份域厂牌号不同,需恢复EEPROM出厂设置 */
	TCFG_EEPROM_GetVender(vender);
	Brand |= (vender[0] & 0x000000FF) << 3*8;
	Brand |= (vender[1] & 0x000000FF) << 2*8;
	Brand |= (vender[2] & 0x000000FF) << 1*8;
	Brand |= (vender[3] & 0x000000FF) << 0*8;
	if (Brand != TCFG_EEPROM_GetFactoryBrand()) {							//厂牌号不同
		ret = true;
		goto exit;
	}
	
exit:
	return ret;
}

/**********************************************************************************************************
 @Function			void TCFG_EEPROM_SetSNfromBrandKey(unsigned int val)
 @Description			TCFG_EEPROM_SetSNfromBrandKey					: 保存SNfromBrandKey
 @Input				val
 @Return				void
**********************************************************************************************************/
void TCFG_EEPROM_SetSNfromBrandKey(unsigned int val)
{
	FLASH_EEPROM_WriteWord(TCFG_FACTORY_BRAND_SN_OFFSET + 4, val);
}

/**********************************************************************************************************
 @Function			unsigned int TCFG_EEPROM_GetSNfromBrandKey(void)
 @Description			TCFG_EEPROM_GetSNfromBrandKey					: 读取SNfromBrandKey
 @Input				void
 @Return				val
**********************************************************************************************************/
unsigned int TCFG_EEPROM_GetSNfromBrandKey(void)
{
	return FLASH_EEPROM_ReadWord(TCFG_FACTORY_BRAND_SN_OFFSET + 4);
}

/**********************************************************************************************************
 @Function			void TCFG_EEPROM_SetFactoryBrand(unsigned int val)
 @Description			TCFG_EEPROM_SetFactoryBrand					: 保存FactoryBrand
 @Input				val
 @Return				void
**********************************************************************************************************/
void TCFG_EEPROM_SetFactoryBrand(unsigned int val)
{
	FLASH_EEPROM_WriteWord(TCFG_FACTORY_BRAND_SN_OFFSET, val);
}

/**********************************************************************************************************
 @Function			unsigned int TCFG_EEPROM_GetFactoryBrand(void)
 @Description			TCFG_EEPROM_GetFactoryBrand					: 读取FactoryBrand
 @Input				void
 @Return				val
**********************************************************************************************************/
unsigned int TCFG_EEPROM_GetFactoryBrand(void)
{
	return FLASH_EEPROM_ReadWord(TCFG_FACTORY_BRAND_SN_OFFSET);
}

/**********************************************************************************************************
 @Function			void TCFG_EEPROM_Set_MAC_SN(unsigned int subsn)
 @Description			TCFG_EEPROM_Set_MAC_SN						: 保存MAC SN
 @Input				sn
 @Return				void
**********************************************************************************************************/
void TCFG_EEPROM_Set_MAC_SN(unsigned int sn)
{
	unsigned int subsn = 0;
	
	subsn |= (sn & 0xFF000000) >> 3*8;
	subsn |= (sn & 0x00FF0000) >> 1*8;
	subsn |= (sn & 0x0000FF00) << 1*8;
	subsn |= (sn & 0x000000FF) << 3*8;
	
	FLASH_EEPROM_WriteWord(TCFG_FACTORY_MAC_SN_OFFSET, subsn);
}

/**********************************************************************************************************
 @Function			unsigned int TCFG_EEPROM_Get_MAC_SN(void)
 @Description			TCFG_EEPROM_Get_MAC_SN						: 读取MAC SN
 @Input				void
 @Return				sn
**********************************************************************************************************/
unsigned int TCFG_EEPROM_Get_MAC_SN(void)
{
	unsigned int subsn = 0;
	unsigned int sn = 0;
	
	subsn = FLASH_EEPROM_ReadWord(TCFG_FACTORY_MAC_SN_OFFSET);
	sn |= (subsn & 0xFF000000) >> 3*8;
	sn |= (subsn & 0x00FF0000) >> 1*8;
	sn |= (subsn & 0x0000FF00) << 1*8;
	sn |= (subsn & 0x000000FF) << 3*8;
	
	sn = (sn == 0) ? 1 : sn;
	
	return sn;
}

/**********************************************************************************************************
 @Function			char* TCFG_EEPROM_Get_MAC_SN_String(void)
 @Description			TCFG_EEPROM_Get_MAC_SN_String					: 读取MAC SN字符串
 @Input				void
 @Return				sn_string
**********************************************************************************************************/
char* TCFG_EEPROM_Get_MAC_SN_String(void)
{
	unsigned int subsn = 0;
	unsigned int sn = 0;
	
	subsn = FLASH_EEPROM_ReadWord(TCFG_FACTORY_MAC_SN_OFFSET);
	sn |= (subsn & 0xFF000000) >> 3*8;
	sn |= (subsn & 0x00FF0000) >> 1*8;
	sn |= (subsn & 0x0000FF00) << 1*8;
	sn |= (subsn & 0x000000FF) << 3*8;
	
	sn = (sn == 0) ? 1 : sn;
	
	memset((void *)TCFG_SystemData.SubMacSN, 0x0, sizeof(TCFG_SystemData.SubMacSN));
	sprintf((char*)TCFG_SystemData.SubMacSN, "%08x", sn);
	
	return (char*)TCFG_SystemData.SubMacSN;
}

/**********************************************************************************************************
 @Function			void TCFG_EEPROM_SetVender(char* vender)
 @Description			TCFG_EEPROM_SetVender						: 保存vender
 @Input				&vender
 @Return				void
**********************************************************************************************************/
void TCFG_EEPROM_SetVender(char* vender)
{
	FLASH_EEPROM_WriteBuffer(TCFG_FACTORY_VENDER_OFFSET, (unsigned char*)vender, TCFG_FACTORY_VENDER_LENGTH);
}

/**********************************************************************************************************
 @Function			void TCFG_EEPROM_GetVender(char* vender)
 @Description			TCFG_EEPROM_GetVender						: 读取vender
 @Input				&vender
 @Return				void
**********************************************************************************************************/
void TCFG_EEPROM_GetVender(char* vender)
{
	FLASH_EEPROM_ReadBuffer(TCFG_FACTORY_VENDER_OFFSET, (unsigned char*)vender, TCFG_FACTORY_VENDER_LENGTH);
}

/**********************************************************************************************************
 @Function			char* TCFG_EEPROM_Get_Vender_String(void)
 @Description			TCFG_EEPROM_Get_Vender_String					: 读取vender字符串
 @Input				void
 @Return				vender_string
**********************************************************************************************************/
char* TCFG_EEPROM_Get_Vender_String(void)
{
	TCFG_EEPROM_GetVender((char *)&TCFG_SystemData.SubVender);
	
	return (char*)TCFG_SystemData.SubVender;
}

/**********************************************************************************************************
 @Function			void TCFG_EEPROM_SetMagMode(uint8_t val)
 @Description			TCFG_EEPROM_SetMagMode						: 保存MagMode
 @Input				val
 @Return				void
**********************************************************************************************************/
void TCFG_EEPROM_SetMagMode(uint8_t val)
{
	FLASH_EEPROM_WriteByte(TCFG_MAG_MODE_OFFSET, val);
}

/**********************************************************************************************************
 @Function			uint8_t TCFG_EEPROM_GetMagMode(void)
 @Description			TCFG_EEPROM_GetMagMode						: 读取MagMode
 @Input				void
 @Return				val
**********************************************************************************************************/
unsigned char TCFG_EEPROM_GetMagMode(void)
{
	return FLASH_EEPROM_ReadByte(TCFG_MAG_MODE_OFFSET);
}

/**********************************************************************************************************
 @Function			void TCFG_EEPROM_SetNbiotHeart(uint8_t val)
 @Description			TCFG_EEPROM_SetNbiotHeart					: 保存NbiotHeart
 @Input				val
 @Return				void
**********************************************************************************************************/
void TCFG_EEPROM_SetNbiotHeart(uint8_t val)
{
	if (val == 0) {
		val = 4;
	}
	else if (val > 12) {
		val = 12;
	}
	
	FLASH_EEPROM_WriteByte(TCFG_NB_HEART_OFFSET, val);
}

/**********************************************************************************************************
 @Function			uint8_t TCFG_EEPROM_GetNbiotHeart(void)
 @Description			TCFG_EEPROM_GetNbiotHeart					: 读取NbiotHeart
 @Input				void
 @Return				val
**********************************************************************************************************/
unsigned char TCFG_EEPROM_GetNbiotHeart(void)
{
	unsigned char val = 0;
	
	val = FLASH_EEPROM_ReadByte(TCFG_NB_HEART_OFFSET);
	if (val == 0) {
		val = 4;
	}
	else if (val > 12) {
		val = 12;
	}
	
	return val;
}

/**********************************************************************************************************
 @Function			void TCFG_EEPROM_SetRadarRange(uint8_t val)
 @Description			TCFG_EEPROM_SetRadarRange					: 保存RadarRange
 @Input				val
 @Return				void
**********************************************************************************************************/
void TCFG_EEPROM_SetRadarRange(uint8_t val)
{
	FLASH_EEPROM_WriteByte(TCFG_RADAR_RANGE_OFFSET, val);
}

/**********************************************************************************************************
 @Function			unsigned char TCFG_EEPROM_GetRadarRange(void)
 @Description			TCFG_EEPROM_GetRadarRange					: 读取RadarRange
 @Input				void
 @Return				val
**********************************************************************************************************/
unsigned char TCFG_EEPROM_GetRadarRange(void)
{
	return FLASH_EEPROM_ReadByte(TCFG_RADAR_RANGE_OFFSET);
}

/**********************************************************************************************************
 @Function			void TCFG_EEPROM_SetCarInDelay(uint8_t val)
 @Description			TCFG_EEPROM_SetCarInDelay					: 保存CarInDelay
 @Input				val
 @Return				void
**********************************************************************************************************/
void TCFG_EEPROM_SetCarInDelay(uint8_t val)
{
	FLASH_EEPROM_WriteByte(TCFG_CARIN_DELAY_OFFSET, val);
}

/**********************************************************************************************************
 @Function			unsigned char TCFG_EEPROM_GetCarInDelay(void)
 @Description			TCFG_EEPROM_GetCarInDelay					: 读取CarInDelay
 @Input				void
 @Return				val
**********************************************************************************************************/
unsigned char TCFG_EEPROM_GetCarInDelay(void)
{
	unsigned char val;
	
	val = FLASH_EEPROM_ReadByte(TCFG_CARIN_DELAY_OFFSET);
	if (val < 8) {
		val = 8;
	}
	
	return val;
}

/**********************************************************************************************************
 @Function			void TCFG_EEPROM_SetRFCmdCnt(uint8_t val)
 @Description			TCFG_EEPROM_SetRFCmdCnt						: 保存RFCmdCnt
 @Input				val
 @Return				void
**********************************************************************************************************/
void TCFG_EEPROM_SetRFCmdCnt(uint8_t val)
{
	FLASH_EEPROM_WriteByte(TCFG_RF_CMDCNT_OFFSET, val);
}

/**********************************************************************************************************
 @Function			unsigned char TCFG_EEPROM_GetRFCmdCnt(void)
 @Description			TCFG_EEPROM_GetRFCmdCnt						: 读取RFCmdCnt
 @Input				void
 @Return				val
**********************************************************************************************************/
unsigned char TCFG_EEPROM_GetRFCmdCnt(void)
{
	return FLASH_EEPROM_ReadByte(TCFG_RF_CMDCNT_OFFSET);
}

/**********************************************************************************************************
 @Function			void TCFG_EEPROM_SetNBCmdCnt(uint8_t val)
 @Description			TCFG_EEPROM_SetNBCmdCnt						: 保存NBCmdCnt
 @Input				val
 @Return				void
**********************************************************************************************************/
void TCFG_EEPROM_SetNBCmdCnt(uint8_t val)
{
	FLASH_EEPROM_WriteByte(TCFG_NB_CMDCNT_OFFSET, val);
}

/**********************************************************************************************************
 @Function			unsigned char TCFG_EEPROM_GetNBCmdCnt(void)
 @Description			TCFG_EEPROM_GetNBCmdCnt						: 读取NBCmdCnt
 @Input				void
 @Return				val
**********************************************************************************************************/
unsigned char TCFG_EEPROM_GetNBCmdCnt(void)
{
	return FLASH_EEPROM_ReadByte(TCFG_NB_CMDCNT_OFFSET);
}

/**********************************************************************************************************
 @Function			void TCFG_EEPROM_SetCoapRATimeHour(unsigned char val)
 @Description			TCFG_EEPROM_SetCoapRATimeHour					: 保存CoapRATimeHour
 @Input				val
 @Return				void
**********************************************************************************************************/
void TCFG_EEPROM_SetCoapRATimeHour(unsigned char val)
{
	if (val > 12) val = 12;
	FLASH_EEPROM_WriteByte(TCFG_COAP_RA_TIME_OFFSET, val);
}

/**********************************************************************************************************
 @Function			unsigned char TCFG_EEPROM_GetCoapRATimeHour(void)
 @Description			TCFG_EEPROM_GetCoapRATimeHour					: 读取CoapRATimeHour
 @Input				void
 @Return				val
**********************************************************************************************************/
unsigned char TCFG_EEPROM_GetCoapRATimeHour(void)
{
	unsigned char val8;
	
	val8 = FLASH_EEPROM_ReadByte(TCFG_COAP_RA_TIME_OFFSET);
	if (val8 > 12) val8 = 12;
	
	return val8;
}

/**********************************************************************************************************
 @Function			void TCFG_EEPROM_SetCoapConnectTime(unsigned int val)
 @Description			TCFG_EEPROM_SetCoapConnectTime				: 保存CoapConnectTime
 @Input				val
 @Return				void
**********************************************************************************************************/
void TCFG_EEPROM_SetCoapConnectTime(unsigned int val)
{
	FLASH_EEPROM_WriteWord(TCFG_COAP_CON_TIME_OFFSET, val);
}

/**********************************************************************************************************
 @Function			unsigned int TCFG_EEPROM_GetCoapConnectTime(void)
 @Description			TCFG_EEPROM_GetCoapConnectTime				: 读取CoapConnectTime
 @Input				void
 @Return				val
**********************************************************************************************************/
unsigned int TCFG_EEPROM_GetCoapConnectTime(void)
{
	return FLASH_EEPROM_ReadWord(TCFG_COAP_CON_TIME_OFFSET);
}

/**********************************************************************************************************
 @Function			void TCFG_EEPROM_SetCoapIdleTime(unsigned int val)
 @Description			TCFG_EEPROM_SetCoapIdleTime					: 保存CoapIdleTime
 @Input				val
 @Return				void
**********************************************************************************************************/
void TCFG_EEPROM_SetCoapIdleTime(unsigned int val)
{
	FLASH_EEPROM_WriteWord(TCFG_COAP_IDLE_TIME_OFFSET, val);
}

/**********************************************************************************************************
 @Function			unsigned int TCFG_EEPROM_GetCoapIdleTime(void)
 @Description			TCFG_EEPROM_GetCoapIdleTime					: 读取CoapIdleTime
 @Input				void
 @Return				val
**********************************************************************************************************/
unsigned int TCFG_EEPROM_GetCoapIdleTime(void)
{
	return FLASH_EEPROM_ReadWord(TCFG_COAP_IDLE_TIME_OFFSET);
}

/**********************************************************************************************************
 @Function			void TCFG_EEPROM_SetCoapConnectDayTime(unsigned short val)
 @Description			TCFG_EEPROM_SetCoapConnectDayTime				: 保存CoapConnectDayTime
 @Input				val
 @Return				void
**********************************************************************************************************/
void TCFG_EEPROM_SetCoapConnectDayTime(unsigned short val)
{
	FLASH_EEPROM_WriteHalfWord(TCFG_COAP_CON_DAY_OFFSET, val);
}

/**********************************************************************************************************
 @Function			unsigned short TCFG_EEPROM_GetCoapConnectDayTime(void)
 @Description			TCFG_EEPROM_GetCoapConnectDayTime				: 读取CoapConnectDayTime
 @Input				void
 @Return				val
**********************************************************************************************************/
unsigned short TCFG_EEPROM_GetCoapConnectDayTime(void)
{
	return FLASH_EEPROM_ReadHalfWord(TCFG_COAP_CON_DAY_OFFSET);
}

/**********************************************************************************************************
 @Function			void TCFG_EEPROM_SetCoapIdleDayTime(unsigned short val)
 @Description			TCFG_EEPROM_SetCoapIdleDayTime				: 保存CoapIdleDayTime
 @Input				val
 @Return				void
**********************************************************************************************************/
void TCFG_EEPROM_SetCoapIdleDayTime(unsigned short val)
{
	FLASH_EEPROM_WriteHalfWord(TCFG_COAP_IDLE_DAY_OFFSET, val);
}

/**********************************************************************************************************
 @Function			unsigned short TCFG_EEPROM_GetCoapIdleDayTime(void)
 @Description			TCFG_EEPROM_GetCoapIdleDayTime				: 读取CoapIdleDayTime
 @Input				void
 @Return				val
**********************************************************************************************************/
unsigned short TCFG_EEPROM_GetCoapIdleDayTime(void)
{
	return FLASH_EEPROM_ReadHalfWord(TCFG_COAP_IDLE_DAY_OFFSET);
}

/**********************************************************************************************************
 @Function			void TCFG_EEPROM_SetCoapQuotaTime(unsigned short val)
 @Description			TCFG_EEPROM_SetCoapQuotaTime					: 保存CoapQuotaTime
 @Input				val
 @Return				void
**********************************************************************************************************/
void TCFG_EEPROM_SetCoapQuotaTime(unsigned short val)
{
	FLASH_EEPROM_WriteHalfWord(TCFG_COAP_QUOTA_TIME_OFFSET, val);
}

/**********************************************************************************************************
 @Function			unsigned short TCFG_EEPROM_GetCoapQuotaTime(void)
 @Description			TCFG_EEPROM_GetCoapQuotaTime					: 读取CoapQuotaTime
 @Input				void
 @Return				val
**********************************************************************************************************/
unsigned short TCFG_EEPROM_GetCoapQuotaTime(void)
{
	return FLASH_EEPROM_ReadHalfWord(TCFG_COAP_QUOTA_TIME_OFFSET);
}

/**********************************************************************************************************
 @Function			void TCFG_EEPROM_SetUpgradeLimitRssi(short val)
 @Description			TCFG_EEPROM_SetUpgradeLimitRssi				: 保存UpgradeLimitRssi
 @Input				val
 @Return				void
**********************************************************************************************************/
void TCFG_EEPROM_SetUpgradeLimitRssi(short val)
{
	FLASH_EEPROM_WriteHalfWord(TCFG_UPDATA_LIMITRSSI_OFFSET, val);
}

/**********************************************************************************************************
 @Function			short TCFG_EEPROM_GetUpgradeLimitRssi(void)
 @Description			TCFG_EEPROM_GetUpgradeLimitRssi				: 读取UpgradeLimitRssi
 @Input				void
 @Return				val
**********************************************************************************************************/
short TCFG_EEPROM_GetUpgradeLimitRssi(void)
{
	return FLASH_EEPROM_ReadHalfWord(TCFG_UPDATA_LIMITRSSI_OFFSET);
}

/**********************************************************************************************************
 @Function			void TCFG_EEPROM_SetUpgradeLimitSnr(short val)
 @Description			TCFG_EEPROM_SetUpgradeLimitSnr				: 保存UpgradeLimitSnr
 @Input				val
 @Return				void
**********************************************************************************************************/
void TCFG_EEPROM_SetUpgradeLimitSnr(short val)
{
	FLASH_EEPROM_WriteHalfWord(TCFG_UPDATA_LIMITSNR_OFFSET, val);
}

/**********************************************************************************************************
 @Function			short TCFG_EEPROM_GetUpgradeLimitSnr(void)
 @Description			TCFG_EEPROM_GetUpgradeLimitSnr				: 读取UpgradeLimitSnr
 @Input				void
 @Return				val
**********************************************************************************************************/
short TCFG_EEPROM_GetUpgradeLimitSnr(void)
{
	return FLASH_EEPROM_ReadHalfWord(TCFG_UPDATA_LIMITSNR_OFFSET);
}

/**********************************************************************************************************
 @Function			void TCFG_EEPROM_SetBeepOff(uint8_t val)
 @Description			TCFG_EEPROM_SetBeepOff						: 保存BeepOff
 @Input				val
 @Return				void
**********************************************************************************************************/
void TCFG_EEPROM_SetBeepOff(uint8_t val)
{
	FLASH_EEPROM_WriteByte(TCFG_BEEP_OFF_OFFSET, val);
}

/**********************************************************************************************************
 @Function			unsigned char TCFG_EEPROM_GetBeepOff(void)
 @Description			TCFG_EEPROM_GetBeepOff						: 读取BeepOff
 @Input				void
 @Return				val
**********************************************************************************************************/
unsigned char TCFG_EEPROM_GetBeepOff(void)
{
	return FLASH_EEPROM_ReadByte(TCFG_BEEP_OFF_OFFSET);
}

/**********************************************************************************************************
 @Function			void TCFG_Utility_Add_Device_BootCount(void)
 @Description			TCFG_Utility_Add_Device_BootCount				: Device重启次数累加
 @Input				void
 @Return				void
**********************************************************************************************************/
void TCFG_Utility_Add_Device_BootCount(void)
{
	TCFG_SystemData.DeviceBootCount = TCFG_EEPROM_GetDevBootCnt() + 1;
	TCFG_EEPROM_SetDevBootCnt(TCFG_SystemData.DeviceBootCount);
}

/**********************************************************************************************************
 @Function			unsigned short TCFG_Utility_Get_Device_BootCount(void)
 @Description			TCFG_Utility_Get_Device_BootCount				: Device重启次数获取
 @Input				void
 @Return				Device_BootCount
**********************************************************************************************************/
unsigned short TCFG_Utility_Get_Device_BootCount(void)
{
	return TCFG_SystemData.DeviceBootCount;
}

/**********************************************************************************************************
 @Function			void TCFG_Utility_Add_Nbiot_BootCount(void)
 @Description			TCFG_Utility_Add_Nbiot_BootCount				: NBIot重启次数累加
 @Input				void
 @Return				void
**********************************************************************************************************/
void TCFG_Utility_Add_Nbiot_BootCount(void)
{
	TCFG_SystemData.NBIotBootCount++;
	if (TCFG_SystemData.NBIotBootCount > (10 + TCFG_EEPROM_GetNbiotBootCnt())) {
		TCFG_EEPROM_SetNbiotBootCnt(TCFG_SystemData.NBIotBootCount);
	}
}

/**********************************************************************************************************
 @Function			unsigned int TCFG_Utility_Get_Nbiot_BootCount(void)
 @Description			TCFG_Utility_Get_Nbiot_BootCount				: NBIot重启次数获取
 @Input				void
 @Return				Nbiot_BootCount
**********************************************************************************************************/
unsigned int TCFG_Utility_Get_Nbiot_BootCount(void)
{
	return TCFG_SystemData.NBIotBootCount;
}

/**********************************************************************************************************
 @Function			void TCFG_Utility_Add_Coap_SentCount(void)
 @Description			TCFG_Utility_Add_Coap_SentCount				: NBIot Coap发送次数累加
 @Input				void
 @Return				void
**********************************************************************************************************/
void TCFG_Utility_Add_Coap_SentCount(void)
{
	TCFG_SystemData.CoapSentCount++;
	if (TCFG_SystemData.CoapSentCount > (10 + TCFG_EEPROM_GetCoapSentCnt())) {
		TCFG_EEPROM_SetCoapSentCnt(TCFG_SystemData.CoapSentCount);
	}
}

/**********************************************************************************************************
 @Function			unsigned int TCFG_Utility_Get_Coap_SentCount(void)
 @Description			TCFG_Utility_Get_Coap_SentCount				: NBIot Coap发送次数获取
 @Input				void
 @Return				Coap_SentCount
**********************************************************************************************************/
unsigned int TCFG_Utility_Get_Coap_SentCount(void)
{
	return TCFG_SystemData.CoapSentCount;
}

/**********************************************************************************************************
 @Function			void TCFG_Utility_Add_Coap_RecvCount(void)
 @Description			TCFG_Utility_Add_Coap_RecvCount				: NBIot Coap接收次数累加
 @Input				void
 @Return				void
**********************************************************************************************************/
void TCFG_Utility_Add_Coap_RecvCount(void)
{
	TCFG_SystemData.CoapRecvCount++;
	if (TCFG_SystemData.CoapRecvCount > (10 + TCFG_EEPROM_GetCoapRecvCnt())) {
		TCFG_EEPROM_SetCoapRecvCnt(TCFG_SystemData.CoapRecvCount);
	}
}

/**********************************************************************************************************
 @Function			unsigned int TCFG_Utility_Get_Coap_RecvCount(void)
 @Description			TCFG_Utility_Get_Coap_RecvCount				: NBIot Coap接收次数获取
 @Input				void
 @Return				Coap_RecvCount
**********************************************************************************************************/
unsigned int TCFG_Utility_Get_Coap_RecvCount(void)
{
	return TCFG_SystemData.CoapRecvCount;
}

/**********************************************************************************************************
 @Function			void TCFG_Utility_Add_MqttSN_SentCount(void)
 @Description			TCFG_Utility_Add_MqttSN_SentCount				: NBIot MqttSN发送次数累加
 @Input				void
 @Return				void
**********************************************************************************************************/
void TCFG_Utility_Add_MqttSN_SentCount(void)
{
	TCFG_SystemData.MqttSNSentCount++;
	if (TCFG_SystemData.MqttSNSentCount > (10 + TCFG_EEPROM_GetMqttSNSentCnt())) {
		TCFG_EEPROM_SetMqttSNSentCnt(TCFG_SystemData.MqttSNSentCount);
	}
}

/**********************************************************************************************************
 @Function			unsigned int TCFG_Utility_Get_MqttSN_SentCount(void)
 @Description			TCFG_Utility_Get_MqttSN_SentCount				: NBIot MqttSN发送次数获取
 @Input				void
 @Return				MqttSN_SentCount
**********************************************************************************************************/
unsigned int TCFG_Utility_Get_MqttSN_SentCount(void)
{
	return TCFG_SystemData.MqttSNSentCount;
}

/**********************************************************************************************************
 @Function			void TCFG_Utility_Add_MqttSN_RecvCount(void)
 @Description			TCFG_Utility_Add_MqttSN_RecvCount				: NBIot MqttSN接收次数累加
 @Input				void
 @Return				void
**********************************************************************************************************/
void TCFG_Utility_Add_MqttSN_RecvCount(void)
{
	TCFG_SystemData.MqttSNRecvCount++;
	if (TCFG_SystemData.MqttSNRecvCount > (10 + TCFG_EEPROM_GetMqttSNRecvCnt())) {
		TCFG_EEPROM_SetMqttSNRecvCnt(TCFG_SystemData.MqttSNRecvCount);
	}
}

/**********************************************************************************************************
 @Function			unsigned int TCFG_Utility_Get_MqttSN_RecvCount(void)
 @Description			TCFG_Utility_Get_MqttSN_RecvCount				: NBIot MqttSN接收次数获取
 @Input				void
 @Return				MqttSN_RecvCount
**********************************************************************************************************/
unsigned int TCFG_Utility_Get_MqttSN_RecvCount(void)
{
	return TCFG_SystemData.MqttSNRecvCount;
}

/**********************************************************************************************************
 @Function			void TCFG_Utility_Add_OneNET_SentCount(void)
 @Description			TCFG_Utility_Add_OneNET_SentCount				: NBIot OneNET发送次数累加
 @Input				void
 @Return				void
**********************************************************************************************************/
void TCFG_Utility_Add_OneNET_SentCount(void)
{
	TCFG_SystemData.OneNETSentCount++;
	if (TCFG_SystemData.OneNETSentCount > (10 + TCFG_EEPROM_GetOneNETSentCnt())) {
		TCFG_EEPROM_SetOneNETSentCnt(TCFG_SystemData.OneNETSentCount);
	}
}

/**********************************************************************************************************
 @Function			unsigned int TCFG_Utility_Get_OneNET_SentCount(void)
 @Description			TCFG_Utility_Get_OneNET_SentCount				: NBIot OneNET发送次数获取
 @Input				void
 @Return				OneNET_SentCount
**********************************************************************************************************/
unsigned int TCFG_Utility_Get_OneNET_SentCount(void)
{
	return TCFG_SystemData.OneNETSentCount;
}

/**********************************************************************************************************
 @Function			void TCFG_Utility_Add_OneNET_RecvCount(void)
 @Description			TCFG_Utility_Add_OneNET_RecvCount				: NBIot OneNET接收次数累加
 @Input				void
 @Return				void
**********************************************************************************************************/
void TCFG_Utility_Add_OneNET_RecvCount(void)
{
	TCFG_SystemData.OneNETRecvCount++;
	if (TCFG_SystemData.OneNETRecvCount > (10 + TCFG_EEPROM_GetOneNETRecvCnt())) {
		TCFG_EEPROM_SetOneNETRecvCnt(TCFG_SystemData.OneNETRecvCount);
	}
}

/**********************************************************************************************************
 @Function			unsigned int TCFG_Utility_Get_OneNET_RecvCount(void)
 @Description			TCFG_Utility_Get_OneNET_RecvCount				: NBIot OneNET接收次数获取
 @Input				void
 @Return				OneNET_RecvCount
**********************************************************************************************************/
unsigned int TCFG_Utility_Get_OneNET_RecvCount(void)
{
	return TCFG_SystemData.OneNETRecvCount;
}

/**********************************************************************************************************
 @Function			void TCFG_Utility_Set_Nbiot_IdleLifetime(unsigned short val)
 @Description			TCFG_Utility_Set_Nbiot_IdleLifetime			: NBIot 休眠模式保活时间设置
 @Input				val
 @Return				void
**********************************************************************************************************/
void TCFG_Utility_Set_Nbiot_IdleLifetime(unsigned short val)
{
	TCFG_SystemData.NBIdleLifetime = (val > 30) ? 30 : val;
}

/**********************************************************************************************************
 @Function			unsigned short TCFG_Utility_Get_Nbiot_IdleLifetime(void)
 @Description			TCFG_Utility_Get_Nbiot_IdleLifetime			: NBIot 休眠模式保活时间获取
 @Input				void
 @Return				val
**********************************************************************************************************/
unsigned short TCFG_Utility_Get_Nbiot_IdleLifetime(void)
{
	return TCFG_SystemData.NBIdleLifetime;
}

/**********************************************************************************************************
 @Function			void TCFG_Utility_Sub_Nbiot_IdleLifetime(void)
 @Description			TCFG_Utility_Sub_Nbiot_IdleLifetime			: NBIot 休眠模式保活时间递减
 @Input				void
 @Return				void
**********************************************************************************************************/
void TCFG_Utility_Sub_Nbiot_IdleLifetime(void)
{
	if (TCFG_SystemData.NBIdleLifetime > 0) {
		TCFG_SystemData.NBIdleLifetime--;
	}
}

/**********************************************************************************************************
 @Function			char* TCFG_Utility_Get_Nbiot_Iccid_String(void)
 @Description			TCFG_Utility_Get_Nbiot_Iccid_String			: 读取Nbiot Iccid字符串
 @Input				void
 @Return				Nbiot_Iccid
**********************************************************************************************************/
char* TCFG_Utility_Get_Nbiot_Iccid_String(void)
{
#if NETPROTOCAL == NETCOAP
	return (char*)NbiotClientHandler.Parameter.iccid;
#elif NETPROTOCAL == NETMQTTSN
	return (char*)MqttSNClientHandler.SocketStack->NBIotStack->Parameter.iccid;
#elif NETPROTOCAL == NETONENET
	return (char*)OneNETClientHandler.LWM2MStack->NBIotStack->Parameter.iccid;
#endif
}

/**********************************************************************************************************
 @Function			char* TCFG_Utility_Get_Nbiot_Imei_String(void)
 @Description			TCFG_Utility_Get_Nbiot_Imei_String				: 读取Nbiot Imei字符串
 @Input				void
 @Return				Nbiot_Iccid
**********************************************************************************************************/
char* TCFG_Utility_Get_Nbiot_Imei_String(void)
{
#if NETPROTOCAL == NETCOAP
	return (char*)NbiotClientHandler.Parameter.imei;
#elif NETPROTOCAL == NETMQTTSN
	return (char*)MqttSNClientHandler.SocketStack->NBIotStack->Parameter.imei;
#elif NETPROTOCAL == NETONENET
	return (char*)OneNETClientHandler.LWM2MStack->NBIotStack->Parameter.imei;
#endif
}

/**********************************************************************************************************
 @Function			int TCFG_Utility_Get_Nbiot_Rssi_IntVal(void)
 @Description			TCFG_Utility_Get_Nbiot_Rssi_IntVal				: 读取Nbiot Rssi值
 @Input				void
 @Return				Nbiot_Rssi
**********************************************************************************************************/
int TCFG_Utility_Get_Nbiot_Rssi_IntVal(void)
{
#if NETPROTOCAL == NETCOAP
	return NbiotClientHandler.Parameter.rssi;
#elif NETPROTOCAL == NETMQTTSN
	return MqttSNClientHandler.SocketStack->NBIotStack->Parameter.rssi;
#elif NETPROTOCAL == NETONENET
	return OneNETClientHandler.LWM2MStack->NBIotStack->Parameter.rssi;
#endif
}

/**********************************************************************************************************
 @Function			unsigned char TCFG_Utility_Get_Nbiot_Rssi_UnCharVal(void)
 @Description			TCFG_Utility_Get_Nbiot_Rssi_UnCharVal			: 读取Nbiot Rssi值
 @Input				void
 @Return				Nbiot_Rssi
**********************************************************************************************************/
unsigned char TCFG_Utility_Get_Nbiot_Rssi_UnCharVal(void)
{
#if NETPROTOCAL == NETCOAP
	return NbiotClientHandler.Parameter.rssi;
#elif NETPROTOCAL == NETMQTTSN
	return MqttSNClientHandler.SocketStack->NBIotStack->Parameter.rssi;
#elif NETPROTOCAL == NETONENET
	return OneNETClientHandler.LWM2MStack->NBIotStack->Parameter.rssi;
#endif
}

/**********************************************************************************************************
 @Function			unsigned char TCFG_Utility_Get_Nbiot_WorkMode(void)
 @Description			TCFG_Utility_Get_Nbiot_WorkMode				: 读取Nbiot WorkMode值
 @Input				void
 @Return				Nbiot_Rssi
**********************************************************************************************************/
unsigned char TCFG_Utility_Get_Nbiot_WorkMode(void)
{
#if NETPROTOCAL == NETCOAP
	return NbiotClientHandler.Parameter.connectedstate;
#elif NETPROTOCAL == NETMQTTSN
	return MqttSNClientHandler.SocketStack->NBIotStack->Parameter.connectedstate;
#elif NETPROTOCAL == NETONENET
	return OneNETClientHandler.LWM2MStack->NBIotStack->Parameter.connectedstate;
#endif
}

/**********************************************************************************************************
 @Function			unsigned char TCFG_Utility_Get_Nbiot_Registered(void)
 @Description			TCFG_Utility_Get_Nbiot_Registered				: 读取Nbiot Registered值
 @Input				void
 @Return				Nbiot_Registered
**********************************************************************************************************/
unsigned char TCFG_Utility_Get_Nbiot_Registered(void)
{
#if NETPROTOCAL == NETCOAP
	return NbiotClientHandler.Registered;
#elif NETPROTOCAL == NETMQTTSN
	return MqttSNClientHandler.SocketStack->NBIotStack->Registered;
#elif NETPROTOCAL == NETONENET
	return OneNETClientHandler.LWM2MStack->NBIotStack->Registered;
#endif
}

/**********************************************************************************************************
 @Function			unsigned char TCFG_Utility_Get_Nbiot_NetStateIdentification(void)
 @Description			TCFG_Utility_Get_Nbiot_NetStateIdentification		: 读取Nbiot NetStateIdentification值
 @Input				void
 @Return				Nbiot_NetStateIdentification
**********************************************************************************************************/
unsigned char TCFG_Utility_Get_Nbiot_NetStateIdentification(void)
{
#if NETPROTOCAL == NETCOAP
	return NbiotClientHandler.NetStateIdentification;
#elif NETPROTOCAL == NETMQTTSN
	return MqttSNClientHandler.SocketStack->NBIotStack->NetStateIdentification;
#elif NETPROTOCAL == NETONENET
	return OneNETClientHandler.LWM2MStack->NBIotStack->NetStateIdentification;
#endif
}

/**********************************************************************************************************
 @Function			int TCFG_Utility_Get_Nbiot_RadioSignalpower(void)
 @Description			TCFG_Utility_Get_Nbiot_RadioSignalpower			: 读取Nbiot RadioSignalpower值
 @Input				void
 @Return				RadioSignalpower
**********************************************************************************************************/
int TCFG_Utility_Get_Nbiot_RadioSignalpower(void)
{
#if NETPROTOCAL == NETCOAP
	return NbiotClientHandler.Parameter.statisticsRADIO.Signalpower;
#elif NETPROTOCAL == NETMQTTSN
	return MqttSNClientHandler.SocketStack->NBIotStack->Parameter.statisticsRADIO.Signalpower;
#elif NETPROTOCAL == NETONENET
	return OneNETClientHandler.LWM2MStack->NBIotStack->Parameter.statisticsRADIO.Signalpower;
#endif
}

/**********************************************************************************************************
 @Function			int TCFG_Utility_Get_Nbiot_RadioTotalpower(void)
 @Description			TCFG_Utility_Get_Nbiot_RadioTotalpower			: 读取Nbiot RadioTotalpower值
 @Input				void
 @Return				RadioTotalpower
**********************************************************************************************************/
int TCFG_Utility_Get_Nbiot_RadioTotalpower(void)
{
#if NETPROTOCAL == NETCOAP
	return NbiotClientHandler.Parameter.statisticsRADIO.Totalpower;
#elif NETPROTOCAL == NETMQTTSN
	return MqttSNClientHandler.SocketStack->NBIotStack->Parameter.statisticsRADIO.Totalpower;
#elif NETPROTOCAL == NETONENET
	return OneNETClientHandler.LWM2MStack->NBIotStack->Parameter.statisticsRADIO.Totalpower;
#endif
}

/**********************************************************************************************************
 @Function			int TCFG_Utility_Get_Nbiot_RadioTXpower(void)
 @Description			TCFG_Utility_Get_Nbiot_RadioTXpower			: 读取Nbiot RadioTXpower值
 @Input				void
 @Return				RadioTXpower
**********************************************************************************************************/
int TCFG_Utility_Get_Nbiot_RadioTXpower(void)
{
#if NETPROTOCAL == NETCOAP
	return NbiotClientHandler.Parameter.statisticsRADIO.TXpower;
#elif NETPROTOCAL == NETMQTTSN
	return MqttSNClientHandler.SocketStack->NBIotStack->Parameter.statisticsRADIO.TXpower;
#elif NETPROTOCAL == NETONENET
	return OneNETClientHandler.LWM2MStack->NBIotStack->Parameter.statisticsRADIO.TXpower;
#endif
}

/**********************************************************************************************************
 @Function			unsigned int TCFG_Utility_Get_Nbiot_RadioTXtime(void)
 @Description			TCFG_Utility_Get_Nbiot_RadioTXtime				: 读取Nbiot RadioTXtime值
 @Input				void
 @Return				RadioTXtime
**********************************************************************************************************/
unsigned int TCFG_Utility_Get_Nbiot_RadioTXtime(void)
{
#if NETPROTOCAL == NETCOAP
	return NbiotClientHandler.Parameter.statisticsRADIO.TXtime;
#elif NETPROTOCAL == NETMQTTSN
	return MqttSNClientHandler.SocketStack->NBIotStack->Parameter.statisticsRADIO.TXtime;
#elif NETPROTOCAL == NETONENET
	return OneNETClientHandler.LWM2MStack->NBIotStack->Parameter.statisticsRADIO.TXtime;
#endif
}

/**********************************************************************************************************
 @Function			unsigned int TCFG_Utility_Get_Nbiot_RadioRXtime(void)
 @Description			TCFG_Utility_Get_Nbiot_RadioTXtime				: 读取Nbiot RadioRXtime值
 @Input				void
 @Return				RadioRXtime
**********************************************************************************************************/
unsigned int TCFG_Utility_Get_Nbiot_RadioRXtime(void)
{
#if NETPROTOCAL == NETCOAP
	return NbiotClientHandler.Parameter.statisticsRADIO.RXtime;
#elif NETPROTOCAL == NETMQTTSN
	return MqttSNClientHandler.SocketStack->NBIotStack->Parameter.statisticsRADIO.RXtime;
#elif NETPROTOCAL == NETONENET
	return OneNETClientHandler.LWM2MStack->NBIotStack->Parameter.statisticsRADIO.RXtime;
#endif
}

/**********************************************************************************************************
 @Function			unsigned int TCFG_Utility_Get_Nbiot_RadioCellID(void)
 @Description			TCFG_Utility_Get_Nbiot_RadioCellID				: 读取Nbiot RadioCellID值
 @Input				void
 @Return				RadioCellID
**********************************************************************************************************/
unsigned int TCFG_Utility_Get_Nbiot_RadioCellID(void)
{
#if NETPROTOCAL == NETCOAP
	return NbiotClientHandler.Parameter.statisticsRADIO.CellID;
#elif NETPROTOCAL == NETMQTTSN
	return MqttSNClientHandler.SocketStack->NBIotStack->Parameter.statisticsRADIO.CellID;
#elif NETPROTOCAL == NETONENET
	return OneNETClientHandler.LWM2MStack->NBIotStack->Parameter.statisticsRADIO.CellID;
#endif
}

/**********************************************************************************************************
 @Function			int TCFG_Utility_Get_Nbiot_RadioECL(void)
 @Description			TCFG_Utility_Get_Nbiot_RadioECL				: 读取Nbiot RadioECL值
 @Input				void
 @Return				RadioECL
**********************************************************************************************************/
int TCFG_Utility_Get_Nbiot_RadioECL(void)
{
#if NETPROTOCAL == NETCOAP
	return NbiotClientHandler.Parameter.statisticsRADIO.ECL;
#elif NETPROTOCAL == NETMQTTSN
	return MqttSNClientHandler.SocketStack->NBIotStack->Parameter.statisticsRADIO.ECL;
#elif NETPROTOCAL == NETONENET
	return OneNETClientHandler.LWM2MStack->NBIotStack->Parameter.statisticsRADIO.ECL;
#endif
}

/**********************************************************************************************************
 @Function			int TCFG_Utility_Get_Nbiot_RadioSNR(void)
 @Description			TCFG_Utility_Get_Nbiot_RadioSNR				: 读取Nbiot RadioSNR值
 @Input				void
 @Return				RadioSNR
**********************************************************************************************************/
int TCFG_Utility_Get_Nbiot_RadioSNR(void)
{
#if NETPROTOCAL == NETCOAP
	return NbiotClientHandler.Parameter.statisticsRADIO.SNR;
#elif NETPROTOCAL == NETMQTTSN
	return MqttSNClientHandler.SocketStack->NBIotStack->Parameter.statisticsRADIO.SNR;
#elif NETPROTOCAL == NETONENET
	return OneNETClientHandler.LWM2MStack->NBIotStack->Parameter.statisticsRADIO.SNR;
#endif
}

/**********************************************************************************************************
 @Function			int TCFG_Utility_Get_Nbiot_RadioEARFCN(void)
 @Description			TCFG_Utility_Get_Nbiot_RadioEARFCN				: 读取Nbiot RadioEARFCN值
 @Input				void
 @Return				RadioEARFCN
**********************************************************************************************************/
int TCFG_Utility_Get_Nbiot_RadioEARFCN(void)
{
#if NETPROTOCAL == NETCOAP
	return NbiotClientHandler.Parameter.statisticsRADIO.EARFCN;
#elif NETPROTOCAL == NETMQTTSN
	return MqttSNClientHandler.SocketStack->NBIotStack->Parameter.statisticsRADIO.EARFCN;
#elif NETPROTOCAL == NETONENET
	return OneNETClientHandler.LWM2MStack->NBIotStack->Parameter.statisticsRADIO.EARFCN;
#endif
}

/**********************************************************************************************************
 @Function			int TCFG_Utility_Get_Nbiot_RadioPCI(void)
 @Description			TCFG_Utility_Get_Nbiot_RadioPCI				: 读取Nbiot RadioPCI值
 @Input				void
 @Return				RadioPCI
**********************************************************************************************************/
int TCFG_Utility_Get_Nbiot_RadioPCI(void)
{
#if NETPROTOCAL == NETCOAP
	return NbiotClientHandler.Parameter.statisticsRADIO.PCI;
#elif NETPROTOCAL == NETMQTTSN
	return MqttSNClientHandler.SocketStack->NBIotStack->Parameter.statisticsRADIO.PCI;
#elif NETPROTOCAL == NETONENET
	return OneNETClientHandler.LWM2MStack->NBIotStack->Parameter.statisticsRADIO.PCI;
#endif
}

/**********************************************************************************************************
 @Function			int TCFG_Utility_Get_Nbiot_RadioRSRQ(void)
 @Description			TCFG_Utility_Get_Nbiot_RadioRSRQ				: 读取Nbiot RadioRSRQ值
 @Input				void
 @Return				RadioRSRQ
**********************************************************************************************************/
int TCFG_Utility_Get_Nbiot_RadioRSRQ(void)
{
#if NETPROTOCAL == NETCOAP
	return NbiotClientHandler.Parameter.statisticsRADIO.RSRQ;
#elif NETPROTOCAL == NETMQTTSN
	return MqttSNClientHandler.SocketStack->NBIotStack->Parameter.statisticsRADIO.RSRQ;
#elif NETPROTOCAL == NETONENET
	return OneNETClientHandler.LWM2MStack->NBIotStack->Parameter.statisticsRADIO.RSRQ;
#endif
}

/**********************************************************************************************************
 @Function			int TCFG_Utility_Get_Nbiot_CellEarfcn(void)
 @Description			TCFG_Utility_Get_Nbiot_CellEarfcn				: 读取Nbiot CellEarfcn值
 @Input				void
 @Return				CellEarfcn
**********************************************************************************************************/
int TCFG_Utility_Get_Nbiot_CellEarfcn(void)
{
#if NETPROTOCAL == NETCOAP
	return NbiotClientHandler.Parameter.statisticsCELL.earfcn;
#elif NETPROTOCAL == NETMQTTSN
	return MqttSNClientHandler.SocketStack->NBIotStack->Parameter.statisticsCELL.earfcn;
#elif NETPROTOCAL == NETONENET
	return OneNETClientHandler.LWM2MStack->NBIotStack->Parameter.statisticsCELL.earfcn;
#endif
}

/**********************************************************************************************************
 @Function			int TCFG_Utility_Get_Nbiot_CellPhysicalCellID(void)
 @Description			TCFG_Utility_Get_Nbiot_CellPhysicalCellID		: 读取Nbiot PhysicalCellCellID值
 @Input				void
 @Return				CellCellID
**********************************************************************************************************/
int TCFG_Utility_Get_Nbiot_CellPhysicalCellID(void)
{
#if NETPROTOCAL == NETCOAP
	return NbiotClientHandler.Parameter.statisticsCELL.physicalcellID;
#elif NETPROTOCAL == NETMQTTSN
	return MqttSNClientHandler.SocketStack->NBIotStack->Parameter.statisticsCELL.physicalcellID;
#elif NETPROTOCAL == NETONENET
	return OneNETClientHandler.LWM2MStack->NBIotStack->Parameter.statisticsCELL.physicalcellID;
#endif
}

/**********************************************************************************************************
 @Function			int TCFG_Utility_Get_Nbiot_CellPrimaryCell(void)
 @Description			TCFG_Utility_Get_Nbiot_CellPrimaryCell			: 读取Nbiot PrimaryCellCell值
 @Input				void
 @Return				CellCell
**********************************************************************************************************/
int TCFG_Utility_Get_Nbiot_CellPrimaryCell(void)
{
#if NETPROTOCAL == NETCOAP
	return NbiotClientHandler.Parameter.statisticsCELL.primarycell;
#elif NETPROTOCAL == NETMQTTSN
	return MqttSNClientHandler.SocketStack->NBIotStack->Parameter.statisticsCELL.primarycell;
#elif NETPROTOCAL == NETONENET
	return OneNETClientHandler.LWM2MStack->NBIotStack->Parameter.statisticsCELL.primarycell;
#endif
}

/**********************************************************************************************************
 @Function			int TCFG_Utility_Get_Nbiot_CellRsrp(void)
 @Description			TCFG_Utility_Get_Nbiot_CellRsrp				: 读取Nbiot CellRsrp值
 @Input				void
 @Return				CellRsrp
**********************************************************************************************************/
int TCFG_Utility_Get_Nbiot_CellRsrp(void)
{
#if NETPROTOCAL == NETCOAP
	return NbiotClientHandler.Parameter.statisticsCELL.rsrp;
#elif NETPROTOCAL == NETMQTTSN
	return MqttSNClientHandler.SocketStack->NBIotStack->Parameter.statisticsCELL.rsrp;
#elif NETPROTOCAL == NETONENET
	return OneNETClientHandler.LWM2MStack->NBIotStack->Parameter.statisticsCELL.rsrp;
#endif
}

/**********************************************************************************************************
 @Function			int TCFG_Utility_Get_Nbiot_CellRsrq(void)
 @Description			TCFG_Utility_Get_Nbiot_CellRsrq				: 读取Nbiot CellRsrq值
 @Input				void
 @Return				CellRsrq
**********************************************************************************************************/
int TCFG_Utility_Get_Nbiot_CellRsrq(void)
{
#if NETPROTOCAL == NETCOAP
	return NbiotClientHandler.Parameter.statisticsCELL.rsrq;
#elif NETPROTOCAL == NETMQTTSN
	return MqttSNClientHandler.SocketStack->NBIotStack->Parameter.statisticsCELL.rsrq;
#elif NETPROTOCAL == NETONENET
	return OneNETClientHandler.LWM2MStack->NBIotStack->Parameter.statisticsCELL.rsrq;
#endif
}

/**********************************************************************************************************
 @Function			int TCFG_Utility_Get_Nbiot_CellCellrssi(void)
 @Description			TCFG_Utility_Get_Nbiot_CellCellrssi			: 读取Nbiot CellCellrssi值
 @Input				void
 @Return				CellCellrssi
**********************************************************************************************************/
int TCFG_Utility_Get_Nbiot_CellCellrssi(void)
{
#if NETPROTOCAL == NETCOAP
	return NbiotClientHandler.Parameter.statisticsCELL.rssi;
#elif NETPROTOCAL == NETMQTTSN
	return MqttSNClientHandler.SocketStack->NBIotStack->Parameter.statisticsCELL.rssi;
#elif NETPROTOCAL == NETONENET
	return OneNETClientHandler.LWM2MStack->NBIotStack->Parameter.statisticsCELL.rssi;
#endif
}

/**********************************************************************************************************
 @Function			int TCFG_Utility_Get_Nbiot_CellSnr(void)
 @Description			TCFG_Utility_Get_Nbiot_CellSnr				: 读取Nbiot CellSnr值
 @Input				void
 @Return				Nbiot_CellSnr
**********************************************************************************************************/
int TCFG_Utility_Get_Nbiot_CellSnr(void)
{
#if NETPROTOCAL == NETCOAP
	return NbiotClientHandler.Parameter.statisticsCELL.snr;
#elif NETPROTOCAL == NETMQTTSN
	return MqttSNClientHandler.SocketStack->NBIotStack->Parameter.statisticsCELL.snr;
#elif NETPROTOCAL == NETONENET
	return OneNETClientHandler.LWM2MStack->NBIotStack->Parameter.statisticsCELL.snr;
#endif
}

/**********************************************************************************************************
 @Function			unsigned int TCFG_Utility_Get_Nbiot_NetworkRegistStatusTac(void)
 @Description			TCFG_Utility_Get_Nbiot_NetworkRegistStatusTac	: 读取Nbiot NetworkRegistStatusTac值
 @Input				void
 @Return				Nbiot_NetworkRegistStatusTac
**********************************************************************************************************/
unsigned int TCFG_Utility_Get_Nbiot_NetworkRegistStatusTac(void)
{
#if NETPROTOCAL == NETCOAP
	return NbiotClientHandler.Parameter.networkRegStatus.tac;
#elif NETPROTOCAL == NETMQTTSN
	return MqttSNClientHandler.SocketStack->NBIotStack->Parameter.networkRegStatus.tac;
#elif NETPROTOCAL == NETONENET
	return OneNETClientHandler.LWM2MStack->NBIotStack->Parameter.networkRegStatus.tac;
#endif
}

/**********************************************************************************************************
 @Function			unsigned int TCFG_Utility_Get_Nbiot_NetworkRegistStatusCellID(void)
 @Description			TCFG_Utility_Get_Nbiot_NetworkRegistStatusCellID	: 读取Nbiot NetworkRegistStatusCellID值
 @Input				void
 @Return				Nbiot_NetworkRegistStatusCellID
**********************************************************************************************************/
unsigned int TCFG_Utility_Get_Nbiot_NetworkRegistStatusCellID(void)
{
#if NETPROTOCAL == NETCOAP
	return NbiotClientHandler.Parameter.networkRegStatus.cellID;
#elif NETPROTOCAL == NETMQTTSN
	return MqttSNClientHandler.SocketStack->NBIotStack->Parameter.networkRegStatus.cellID;
#elif NETPROTOCAL == NETONENET
	return OneNETClientHandler.LWM2MStack->NBIotStack->Parameter.networkRegStatus.cellID;
#endif
}

/**********************************************************************************************************
 @Function			unsigned int TCFG_Utility_GetCoapConnectTime(void)
 @Description			TCFG_Utility_GetCoapConnectTime				: 读取Nbiot CoapConnectTime值
 @Input				void
 @Return				Nbiot_CoapConnectTime
**********************************************************************************************************/
unsigned int TCFG_Utility_GetCoapConnectTime(void)
{
	return NbiotClientHandler.CoapConnectTimeSec;
}

/**********************************************************************************************************
 @Function			unsigned int TCFG_Utility_GetCoapIdleTime(void)
 @Description			TCFG_Utility_GetCoapIdleTime					: 读取Nbiot CoapIdleTime值
 @Input				void
 @Return				Nbiot_CoapIdleTime
**********************************************************************************************************/
unsigned int TCFG_Utility_GetCoapIdleTime(void)
{
	return NbiotClientHandler.CoapIdleTimeSec;
}

/**********************************************************************************************************
 @Function			unsigned short TCFG_Utility_GetCoapConnectDayTime(void)
 @Description			TCFG_Utility_GetCoapConnectDayTime				: 读取Nbiot CoapConnectDayTime值
 @Input				void
 @Return				Nbiot_CoapConnectDayTime
**********************************************************************************************************/
unsigned short TCFG_Utility_GetCoapConnectDayTime(void)
{
	return NbiotClientHandler.CoapConnectDayTimeSec;
}

/**********************************************************************************************************
 @Function			unsigned short TCFG_Utility_GetCoapIdleDayTime(void)
 @Description			TCFG_Utility_GetCoapIdleDayTime				: 读取Nbiot CoapIdleDayTime值
 @Input				void
 @Return				Nbiot_CoapIdleDayTime
**********************************************************************************************************/
unsigned short TCFG_Utility_GetCoapIdleDayTime(void)
{
	return NbiotClientHandler.CoapIdleDayTimeSec;
}

/**********************************************************************************************************
 @Function			char* TCFG_Utility_Get_Nbiot_Manufacturer(void)
 @Description			TCFG_Utility_Get_Nbiot_Manufacturer			: 读取Nbiot Manufacturer值
 @Input				void
 @Return				Nbiot_Manufacturer
**********************************************************************************************************/
char* TCFG_Utility_Get_Nbiot_Manufacturer(void)
{
#if NETPROTOCAL == NETCOAP
	return NbiotClientHandler.Parameter.manufacturer;
#elif NETPROTOCAL == NETMQTTSN
	return MqttSNClientHandler.SocketStack->NBIotStack->Parameter.manufacturer;
#elif NETPROTOCAL == NETONENET
	return OneNETClientHandler.LWM2MStack->NBIotStack->Parameter.manufacturer;
#endif
}

/**********************************************************************************************************
 @Function			char* TCFG_Utility_Get_Nbiot_Manufacturermode(void)
 @Description			TCFG_Utility_Get_Nbiot_Manufacturermode			: 读取Nbiot Manufacturermode值
 @Input				void
 @Return				Nbiot_Manufacturermode
**********************************************************************************************************/
char* TCFG_Utility_Get_Nbiot_Manufacturermode(void)
{
#if NETPROTOCAL == NETCOAP
	return NbiotClientHandler.Parameter.manufacturermode;
#elif NETPROTOCAL == NETMQTTSN
	return MqttSNClientHandler.SocketStack->NBIotStack->Parameter.manufacturermode;
#elif NETPROTOCAL == NETONENET
	return OneNETClientHandler.LWM2MStack->NBIotStack->Parameter.manufacturermode;
#endif
}

/**********************************************************************************************************
 @Function			char* TCFG_Utility_Get_Nbiot_ModelVersion(void)
 @Description			TCFG_Utility_Get_Nbiot_ModelVersion			: 读取Nbiot ModelVersion值
 @Input				void
 @Return				Nbiot_ModelVersion
**********************************************************************************************************/
char* TCFG_Utility_Get_Nbiot_ModelVersion(void)
{
#if NETPROTOCAL == NETCOAP
	return NbiotClientHandler.Parameter.modelversion;
#elif NETPROTOCAL == NETMQTTSN
	return MqttSNClientHandler.SocketStack->NBIotStack->Parameter.modelversion;
#elif NETPROTOCAL == NETONENET
	return OneNETClientHandler.LWM2MStack->NBIotStack->Parameter.modelversion;
#endif
}

/**********************************************************************************************************
 @Function			unsigned int TCFG_Utility_Get_Nbiot_SentCount(void)
 @Description			TCFG_Utility_Get_Nbiot_SentCount				: 读取Nbiot SentCount值
 @Input				void
 @Return				Nbiot_SentCount
**********************************************************************************************************/
unsigned int TCFG_Utility_Get_Nbiot_SentCount(void)
{
#if NETPROTOCAL == NETCOAP
	return TCFG_Utility_Get_Coap_SentCount();
#elif NETPROTOCAL == NETMQTTSN
	return TCFG_Utility_Get_MqttSN_SentCount();
#elif NETPROTOCAL == NETONENET
	return TCFG_Utility_Get_OneNET_SentCount();
#endif
}

/**********************************************************************************************************
 @Function			unsigned int TCFG_Utility_Get_Nbiot_RecvCount(void)
 @Description			TCFG_Utility_Get_Nbiot_RecvCount				: 读取Nbiot RecvCount值
 @Input				void
 @Return				Nbiot_RecvCount
**********************************************************************************************************/
unsigned int TCFG_Utility_Get_Nbiot_RecvCount(void)
{
#if NETPROTOCAL == NETCOAP
	return TCFG_Utility_Get_Coap_RecvCount();
#elif NETPROTOCAL == NETMQTTSN
	return TCFG_Utility_Get_MqttSN_RecvCount();
#elif NETPROTOCAL == NETONENET
	return TCFG_Utility_Get_OneNET_RecvCount();
#endif
}

/**********************************************************************************************************
 @Function			unsigned short TCFG_Utility_Get_Device_Batt_ShortVal(void)
 @Description			TCFG_Utility_Get_Device_Batt_ShortVal			: 读取Device Batt值
 @Input				void
 @Return				Device_Batt
**********************************************************************************************************/
unsigned short TCFG_Utility_Get_Device_Batt_ShortVal(void)
{
	return VBAT_ADC_Read(1000);
}

/**********************************************************************************************************
 @Function			short TCFG_Utility_Get_Device_Temperature(void)
 @Description			TCFG_Utility_Get_Device_Temperature			: 读取Device Temperature值
 @Input				void
 @Return				Device_Temperature
**********************************************************************************************************/
short TCFG_Utility_Get_Device_Temperature(void)
{
	return TEMPERATURE_ADC_Read(1000);
}

/**********************************************************************************************************
 @Function			char* TCFG_Utility_Get_Build_Time_String(void)
 @Description			TCFG_Utility_Get_Build_Time_String				: 读取Build Time字符串
 @Input				void
 @Return				buildtime_string
**********************************************************************************************************/
char* TCFG_Utility_Get_Build_Time_String(void)
{
	char bulidData[12];
	
	memset(bulidData, 0x0, sizeof(bulidData));
	memset((void*)&TCFG_SystemData.BuildTime, 0, sizeof(TCFG_SystemData.BuildTime));
	
	memcpy(bulidData, __DATE__, 12);
	if (!memcmp(&bulidData[0], "Jan", 3)) {
		TCFG_SystemData.BuildTime[5] = '0';
		TCFG_SystemData.BuildTime[6] = '1';
	}
	else	if (!memcmp(&bulidData[0], "Feb", 3)) {
		TCFG_SystemData.BuildTime[5] = '0';
		TCFG_SystemData.BuildTime[6] = '2';
	}
	else if (!memcmp(&bulidData[0], "Mar", 3)) {
		TCFG_SystemData.BuildTime[5] = '0';
		TCFG_SystemData.BuildTime[6] = '3';
	}
	else if(!memcmp(&bulidData[0], "Apr", 3)) {
		TCFG_SystemData.BuildTime[5] = '0';
		TCFG_SystemData.BuildTime[6] = '4';
	}
	else if (!memcmp(&bulidData[0], "May", 3)) {
		TCFG_SystemData.BuildTime[5] = '0';
		TCFG_SystemData.BuildTime[6] = '5';
	}
	else if (!memcmp(&bulidData[0], "Jun", 3)) {
		TCFG_SystemData.BuildTime[5] = '0';
		TCFG_SystemData.BuildTime[6] = '6';
	}
	else if (!memcmp(&bulidData[0], "Jul", 3)) {
		TCFG_SystemData.BuildTime[5] = '0';
		TCFG_SystemData.BuildTime[6] = '7';
	}
	else if (!memcmp(&bulidData[0], "Aug", 3)) {
		TCFG_SystemData.BuildTime[5] = '0';
		TCFG_SystemData.BuildTime[6] = '8';
	}
	else if (!memcmp(&bulidData[0], "Sep", 3)) {
		TCFG_SystemData.BuildTime[5] = '0';
		TCFG_SystemData.BuildTime[6] = '9';
	}
	else if (!memcmp(&bulidData[0], "Oct", 3)) {
		TCFG_SystemData.BuildTime[5] = '1';
		TCFG_SystemData.BuildTime[6] = '0';
	}
	else if (!memcmp(&bulidData[0], "Nov", 3)) {
		TCFG_SystemData.BuildTime[5] = '1';
		TCFG_SystemData.BuildTime[6] = '1';
	}
	else if (!memcmp(&bulidData[0], "Dec", 3)) {
		TCFG_SystemData.BuildTime[5] = '1';
		TCFG_SystemData.BuildTime[6] = '2';
	}
	
	memcpy(TCFG_SystemData.BuildTime, &bulidData[7], 4);
	memcpy(&TCFG_SystemData.BuildTime[8], &bulidData[4], 2);
	TCFG_SystemData.BuildTime[4] = '-';
	TCFG_SystemData.BuildTime[7] = '-';
  	TCFG_SystemData.BuildTime[11] = '\0';
	
	return (char*)TCFG_SystemData.BuildTime;
}

/**********************************************************************************************************
 @Function			unsigned int TCFG_Utility_Get_Run_Time(void)
 @Description			TCFG_Utility_Get_Run_Time					: 读取系统运行时间
 @Input				void
 @Return				runtime
**********************************************************************************************************/
unsigned int TCFG_Utility_Get_Run_Time(void)
{
	return Stm32_GetSecondTick();
}

/**********************************************************************************************************
 @Function			unsigned short TCFG_Utility_Get_Mvb_ModelType(void)
 @Description			TCFG_Utility_Get_Mvb_ModelType				: 读取设备类型
 @Input				void
 @Return				ModelType
**********************************************************************************************************/
unsigned short TCFG_Utility_Get_Mvb_ModelType(void)
{
	return MVB_MODEL_TYPE;
}

/**********************************************************************************************************
 @Function			unsigned char TCFG_Utility_Get_RadarLibNum(void)
 @Description			TCFG_Utility_Get_RadarLibNum					: 读取雷达库版本号
 @Input				void
 @Return				RadarLibNum
**********************************************************************************************************/
unsigned char TCFG_Utility_Get_RadarLibNum(void)
{
	return tradar_get_lib_num();
}

/**********************************************************************************************************
 @Function			unsigned char TCFG_Utility_Get_AlgoLibNum(void)
 @Description			TCFG_Utility_Get_AlgoLibNum					: 读取检测算法库版本号
 @Input				void
 @Return				RadarLibNum
**********************************************************************************************************/
unsigned char TCFG_Utility_Get_AlgoLibNum(void)
{
	return talgo_get_lib_num();
}

/**********************************************************************************************************
 @Function			unsigned char TCFG_Utility_Get_SoftResetFlag(void)
 @Description			TCFG_Utility_Get_SoftResetFlag				: 读取设备重启标识符
 @Input				void
 @Return				SoftResetFlag
**********************************************************************************************************/
unsigned char TCFG_Utility_Get_SoftResetFlag(void)
{
	return SoftResetFlag;
}

/**********************************************************************************************************
 @Function			unsigned short TCFG_Utility_Get_ReInitModuleCount(void)
 @Description			TCFG_Utility_Get_ReInitModuleCount				: 读取模块异常初始化次数
 @Input				void
 @Return				ReInitModuleCount
**********************************************************************************************************/
unsigned short TCFG_Utility_Get_ReInitModuleCount(void)
{
	return ReInitModuleGetCnt();
}

/**********************************************************************************************************
 @Function			unsigned short TCFG_Utility_Get_DistanceRange(void)
 @Description			TCFG_Utility_Get_DistanceRange				: 读取雷达检测范围
 @Input				void
 @Return				DistanceRange
**********************************************************************************************************/
unsigned short TCFG_Utility_Get_DistanceRange(void)
{
	return tradar_get_distance_range() - 4;
}

/**********************************************************************************************************
 @Function			unsigned char TCFG_Utility_Get_Major_Softnumber(void)
 @Description			TCFG_Utility_Get_Major_Softnumber				: 读取Major_Softnumber
 @Input				void
 @Return				val
**********************************************************************************************************/
unsigned char TCFG_Utility_Get_Major_Softnumber(void)
{
	return SOFTWAREMAJOR + NETPROTOCAL;
}

/**********************************************************************************************************
 @Function			unsigned char TCFG_Utility_Get_Sub_Softnumber(void)
 @Description			TCFG_Utility_Get_Sub_Softnumber				: 读取Sub_Softnumber
 @Input				void
 @Return				val
**********************************************************************************************************/
unsigned char TCFG_Utility_Get_Sub_Softnumber(void)
{
	return SOFTWARESUB;
}

/**********************************************************************************************************
 @Function			unsigned char TCFG_Utility_Get_Major_Hardnumber(void)
 @Description			TCFG_Utility_Get_Sub_Hardnumber				: 读取Major_Hardnumber
 @Input				void
 @Return				val
**********************************************************************************************************/
unsigned char TCFG_Utility_Get_Major_Hardnumber(void)
{
#if RADAR_MODEL_TYPE == RADAR_MODEL_V1
	return HARDWAREMAJOR_V1;
#elif RADAR_MODEL_TYPE == RADAR_MODEL_V2
	return HARDWAREMAJOR_V2;
#else
	#error RADAR_MODEL_TYPE Define Error
#endif
}

/**********************************************************************************************************
 @Function			char* TCFG_Utility_Get_Softwear_Version_String(void)
 @Description			TCFG_Utility_Get_Softwear_Version_String			: 读取Softwear Version字符串
 @Input				void
 @Return				Softwear_Version_String
**********************************************************************************************************/
char* TCFG_Utility_Get_Softwear_Version_String(void)
{
	memset((void*)&TCFG_SystemData.SoftVersion, 0, sizeof(TCFG_SystemData.SoftVersion));
	
	sprintf((char*)TCFG_SystemData.SoftVersion, "%d:%d.%d", \
		TCFG_EEPROM_GetBootVersion(), TCFG_Utility_Get_Major_Softnumber(), TCFG_Utility_Get_Sub_Softnumber());
	
	return (char*)TCFG_SystemData.SoftVersion;
}

/**********************************************************************************************************
 @Function			char* TCFG_Utility_Get_Hardwear_Version_String(void)
 @Description			TCFG_Utility_Get_Hardwear_Version_String			: 读取Hardwear Version字符串
 @Input				void
 @Return				Hadrwear_Version_String
**********************************************************************************************************/
char* TCFG_Utility_Get_Hardwear_Version_String(void)
{
	memset((void*)&TCFG_SystemData.HardVersion, 0, sizeof(TCFG_SystemData.HardVersion));
	
#if RADAR_MODEL_TYPE == RADAR_MODEL_V1
	if (Radio_Rf_get_Status() == TRF_OK) {
		if (GD25Q_SPIFLASH_Get_Status() == GD25Q80CSIG_OK) {
			sprintf((char*)TCFG_SystemData.HardVersion, "%s-NB-RF-GD25", CPUVERSION);
		}
		else {
			sprintf((char*)TCFG_SystemData.HardVersion, "%s-NB-RF", CPUVERSION);
		}
	}
	else {
		if (GD25Q_SPIFLASH_Get_Status() == GD25Q80CSIG_OK) {
			sprintf((char*)TCFG_SystemData.HardVersion, "%s-NB-GD25", CPUVERSION);
		}
		else {
			sprintf((char*)TCFG_SystemData.HardVersion, "%s-NB", CPUVERSION);
		}
	}
#elif RADAR_MODEL_TYPE == RADAR_MODEL_V2
	if (Radio_Rf_get_Status() == TRF_OK) {
		if (GD25Q_SPIFLASH_Get_Status() == GD25Q80CSIG_OK) {
			sprintf((char*)TCFG_SystemData.HardVersion, "%s-NB-RF-GD25-RV2", CPUVERSION);
		}
		else {
			sprintf((char*)TCFG_SystemData.HardVersion, "%s-NB-RF-RV2", CPUVERSION);
		}
	}
	else {
		if (GD25Q_SPIFLASH_Get_Status() == GD25Q80CSIG_OK) {
			sprintf((char*)TCFG_SystemData.HardVersion, "%s-NB-GD25-RV2", CPUVERSION);
		}
		else {
			sprintf((char*)TCFG_SystemData.HardVersion, "%s-NB-RV2", CPUVERSION);
		}
	}
#else
	#error RADAR_MODEL_TYPE Define Error
#endif
	
	return (char*)TCFG_SystemData.HardVersion;
}

/********************************************** END OF FLEE **********************************************/
