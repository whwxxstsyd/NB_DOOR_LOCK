/**
  *********************************************************************************************************
  * @file    pcpupgrade.c
  * @author  Movebroad -- KK
  * @version V1.0
  * @date    2018-08-07
  * @brief   
  *********************************************************************************************************
  * @attention
  *
  *
  *
  *********************************************************************************************************
  */	

#include "pcpupgrade.h"


static void STMFLASH_ReadBuffer(uint32_t addr, uint8_t *buf, uint32_t length)
{
	while (length--) {
		*buf++ = *(__IO uint8_t *)addr++;
	}	
}

u8 CalculateSumCheckCode(u32 regCode, uint8_t* buf, uint32_t length)
{
	u32 sumCheckCode = regCode;
	
	while (length--) {
		sumCheckCode += *buf++;
	}
	
	sumCheckCode %= 0xFF;
	
	return sumCheckCode;
}

u8 CalculateStringToHex(u8 HighChar, u8 LowChar)
{
	u8 val = 0;
	
	if ((HighChar >= '0') && (HighChar <= '9')) {
		val |= ((HighChar - '0') << 4);
	}
	else if ((HighChar >= 'A') && (HighChar <= 'F')) {
		val |= ((HighChar - 'A' + 10) << 4);
	}
	else if ((HighChar >= 'a') && (HighChar <= 'f')) {
		val |= ((HighChar - 'a' + 10) << 4);
	}
	
	if ((LowChar >= '0') && (LowChar <= '9')) {
		val |= (LowChar - '0');
	}
	else if ((LowChar >= 'A') && (LowChar <= 'F')) {
		val |= (LowChar - 'A' + 10);
	}
	else if ((LowChar >= 'a') && (LowChar <= 'f')) {
		val |= (LowChar - 'a' + 10);
	}
	
	return val;
}

/**********************************************************************************************************
 @Function			PCP_ResultCodeTypeDef PCP_Upgrade_BackupCurrentAPP(PCP_ClientsTypeDef* pClient)
 @Description			PCP_Upgrade_BackupCurrentAPP			: PCP�������ݵ�ǰAPP
 @Input				pClient							: PCP�ͻ���ʵ��
 @Return				void
**********************************************************************************************************/
PCP_ResultCodeTypeDef PCP_Upgrade_BackupCurrentAPP(PCP_ClientsTypeDef* pClient)
{
#ifdef GD25Q_80CSIG
	PCP_ResultCodeTypeDef PCPResultCodeStatus = PCP_ExecuteSuccess;
	u8 STMFlashCheckCode = 0;
	u8 SPIFlashCheckCode = 0;
	int BackupTimes = 3;
	
	if (GD25Q80CSIG_OK != GD25Q_SPIFLASH_Get_Status()) {
		PCPResultCodeStatus = PCP_InternalAnomaly;
		goto exit;
	}
	
	GPRS_DEBUG(DEBUG_INFO, "Backup APP ...");
	Radio_Rf_Interrupt_Deinit();
	GD25Q_SPIFLASH_WakeUp();
	GD25Q_SPIFLASH_Init();
	
#if UPGRADE_BACKUP_APP_WRITE_TYPE == UPGRADE_BACKUP_APP_WRITE_DISABLE
	if (GD25Q_SPIFLASH_GetByte(APP2_INFO_UPGRADE_STATUS_OFFSET) == 0x55) {
		if (GD25Q_SPIFLASH_GetWord(APP2_INFO_UPGRADE_SOFTVER_OFFSET) == ((SOFTWAREMAJOR<<16)|(SOFTWARESUB<<0))) {
			/* ���������е�ǰ�汾APP */
			GD25Q_SPIFLASH_PowerDown();
			GPRS_DEBUG(DEBUG_INFO, "APP has been backed up");
			PCPResultCodeStatus = PCP_LatestVersion;
			goto exit;
		}
	}
#endif
	
	/* ������û�е�ǰAPP��APP�汾��ͬ */
	do {
		IWDG_Feed();
		/* ����APP2�� */
		GD25Q_SPIFLASH_EraseBlock(APP2_BASE_ADDR + 0 * GD25Q80_BLOCK_BYTE_SIZE);
		GD25Q_SPIFLASH_EraseBlock(APP2_BASE_ADDR + 1 * GD25Q80_BLOCK_BYTE_SIZE);
		GD25Q_SPIFLASH_EraseBlock(APP2_BASE_ADDR + 2 * GD25Q80_BLOCK_BYTE_SIZE);
		GD25Q_SPIFLASH_EraseBlock(APP2_BASE_ADDR + 3 * GD25Q80_BLOCK_BYTE_SIZE);
		/* д��APP2DATA */
		for (int packIndex = 0; packIndex < 228; packIndex++) {
			STMFLASH_ReadBuffer(APP_LOWEST_ADDRESS + packIndex * 512, pClient->DataProcessStack, 512);
			GD25Q_SPIFLASH_WriteBuffer(pClient->DataProcessStack, APP2_DATA_ADDR + packIndex * 512, 512);
			STMFlashCheckCode = CalculateSumCheckCode(STMFlashCheckCode, pClient->DataProcessStack, 512);
		}
		/* У��APP2DATA */
		for (int packIndex = 0; packIndex < 228; packIndex++) {
			GD25Q_SPIFLASH_ReadBuffer(pClient->DataProcessStack, APP2_DATA_ADDR + packIndex * 512, 512);
			SPIFlashCheckCode = CalculateSumCheckCode(SPIFlashCheckCode, pClient->DataProcessStack, 512);
		}
		BackupTimes--;
	} while ((STMFlashCheckCode != SPIFlashCheckCode) && (BackupTimes > 0));
	
	if (STMFlashCheckCode != SPIFlashCheckCode) {
		GD25Q_SPIFLASH_PowerDown();
		GPRS_DEBUG(DEBUG_INFO, "APP backed up fail");
		goto exit;
	}
		
	/* д��APP2INFO */
	GD25Q_SPIFLASH_SetByte(APP2_INFO_UPGRADE_STATUS_OFFSET, 0x55);										//��ʶ������
	GD25Q_SPIFLASH_SetWord(APP2_INFO_UPGRADE_BASEADDR_OFFSET, APP2_DATA_ADDR);								//����������ַ
	GD25Q_SPIFLASH_SetHalfWord(APP2_INFO_UPGRADE_BLOCKNUM_OFFSET, 228);									//����������
	GD25Q_SPIFLASH_SetHalfWord(APP2_INFO_UPGRADE_BLOCKLEN_OFFSET, 512);									//�������鳤��
	GD25Q_SPIFLASH_SetHalfWord(APP2_INFO_UPGRADE_DATALEN_OFFSET, 512);									//����������Ч���ݳ���
	
	GD25Q_SPIFLASH_SetWord(APP2_INFO_UPGRADE_SOFTVER_OFFSET, ((SOFTWAREMAJOR<<16)|(SOFTWARESUB<<0)));			//�������汾��
	GD25Q_SPIFLASH_SetWord(APP2_INFO_DOWNLOAD_TIME_OFFSET, RTC_GetUnixTimeToStamp());						//����������ʱ��
	GD25Q_SPIFLASH_SetWord(APP2_DATA_CHECK_CODE_OFFSET, SPIFlashCheckCode);
	
	GD25Q_SPIFLASH_PowerDown();
	GPRS_DEBUG(DEBUG_INFO, "APP backup over code %X", STMFlashCheckCode);
	
exit:
	return PCPResultCodeStatus;

#else
	
	GPRS_DEBUG(DEBUG_INFO, ">>>>>>>>>PCP_Upgrade_BackupCurrentAPP Running<<<<<<<<<");

	return PCP_ExecuteSuccess;

#endif
}

/**********************************************************************************************************
 @Function			PCP_ResultCodeTypeDef PCP_Upgrade_NewVersionNotice(PCP_ClientsTypeDef* pClient)
 @Description			PCP_Upgrade_NewVersionNotice			: PCP�����°汾֪ͨ����
 @Input				pClient							: PCP�ͻ���ʵ��
 @Return				void
**********************************************************************************************************/
PCP_ResultCodeTypeDef PCP_Upgrade_NewVersionNotice(PCP_ClientsTypeDef* pClient)
{
#ifdef GD25Q_80CSIG
	PCP_ResultCodeTypeDef PCPResultCodeStatus = PCP_ExecuteSuccess;
	u32 MajorVer = 0, SubVer = 0;
	
	if (GD25Q80CSIG_OK != GD25Q_SPIFLASH_Get_Status()) {
		PCPResultCodeStatus = PCP_InternalAnomaly;
		goto exit;
	}
	
	GPRS_DEBUG(DEBUG_INFO, "NewVersion APP ...");
	
	/* �ź�����������ȵ��ж����� */
	if ((TCFG_Utility_Get_Nbiot_Rssi_IntVal() < UPGRADE_LOW_LIMIT_RSSI) || (TCFG_Utility_Get_Nbiot_RadioSNR() < UPGRADE_LOW_LIMIT_SNR)) {
		GPRS_DEBUG(DEBUG_INFO, "Signal Quality Difference");
		PCPResultCodeStatus = PCP_SignalqualityDifference;
		goto exit;
	}
	
	/* ����ǰ���ݵ�ǰAPP */
	PCP_Upgrade_BackupCurrentAPP(pClient);
	
	Radio_Rf_Interrupt_Deinit();
	GD25Q_SPIFLASH_WakeUp();
	GD25Q_SPIFLASH_Init();
	
	if (sscanf((const char*)pClient->Parameter.PlatformSoftVersion, "V%d.%d", &MajorVer, &SubVer) <= 0) {
		PCPResultCodeStatus = PCP_InternalAnomaly;
		goto exit;
	}
	if (GD25Q_SPIFLASH_GetWord(APP1_INFO_UPGRADE_SOFTVER_OFFSET) == ((MajorVer<<16)|(SubVer<<0))) {
		if (GD25Q_SPIFLASH_GetByte(APP1_INFO_UPGRADE_STATUS_OFFSET) == 0x55) {
			/* ���������е�ǰ�����汾APP */
			GD25Q_SPIFLASH_PowerDown();
			GPRS_DEBUG(DEBUG_INFO, "APP has been latestVersion");
			PCPResultCodeStatus = PCP_LatestVersion;
			goto exit;
		}
		else {
			/* ���������и�APP�������� */
			/* ��Ƭ��ŵ���δ�������ݰ�,ʵ�ֶϵ����� */
			pClient->UpgradeExecution.PackSliceIndex += GD25Q_SPIFLASH_GetNumofByte(APP1_PACKSLICE_STATUS_OFFSET, APP_PACKSLICE_NUM, 0x00);
			if (pClient->Parameter.UpgradePackSliceNum <= pClient->UpgradeExecution.PackSliceIndex) {
				pClient->UpgradeExecution.PackSliceIndex = pClient->Parameter.UpgradePackSliceNum - 1;
			}
			GD25Q_SPIFLASH_PowerDown();
			GPRS_DEBUG(DEBUG_INFO, "APP Upgradeing");
			PCPResultCodeStatus = PCP_ExecuteSuccess;
			goto exit;
		}
	}
	
	/* ������û�е�ǰ����APP��APP�汾��ͬ */
	/* ����APP1�� */
	GD25Q_SPIFLASH_EraseBlock(APP1_BASE_ADDR + 0 * GD25Q80_BLOCK_BYTE_SIZE);
	GD25Q_SPIFLASH_EraseBlock(APP1_BASE_ADDR + 1 * GD25Q80_BLOCK_BYTE_SIZE);
	GD25Q_SPIFLASH_EraseBlock(APP1_BASE_ADDR + 2 * GD25Q80_BLOCK_BYTE_SIZE);
	GD25Q_SPIFLASH_EraseBlock(APP1_BASE_ADDR + 3 * GD25Q80_BLOCK_BYTE_SIZE);
	/* д��APP1INFO */
	GD25Q_SPIFLASH_SetWord(APP1_INFO_UPGRADE_BASEADDR_OFFSET, APP1_DATA_ADDR);								//����������ַ
	GD25Q_SPIFLASH_SetHalfWord(APP1_INFO_UPGRADE_BLOCKNUM_OFFSET, pClient->Parameter.UpgradePackSliceNum);		//����������
	GD25Q_SPIFLASH_SetHalfWord(APP1_INFO_UPGRADE_BLOCKLEN_OFFSET, UPGRADE_PACK_SLICE_BLOCK_SIZE);				//�������鳤��
	GD25Q_SPIFLASH_SetHalfWord(APP1_INFO_UPGRADE_DATALEN_OFFSET, pClient->Parameter.UpgradePackSliceSize);		//����������Ч���ݳ���
	
	GD25Q_SPIFLASH_SetWord(APP1_INFO_UPGRADE_SOFTVER_OFFSET, ((MajorVer<<16)|(SubVer<<0)));					//�������汾��
	GD25Q_SPIFLASH_SetWord(APP1_INFO_DOWNLOAD_TIME_OFFSET, RTC_GetUnixTimeToStamp());						//����������ʱ��
	GD25Q_SPIFLASH_SetWord(APP1_DATA_CHECK_CODE_OFFSET, CalculateStringToHex(pClient->Parameter.UpgradePackCheckCode>>8, pClient->Parameter.UpgradePackCheckCode&0xFF));
	
	GD25Q_SPIFLASH_PowerDown();
	GPRS_DEBUG(DEBUG_INFO, "APP will start to upgrade");
	
exit:
	return PCPResultCodeStatus;

#else

	GPRS_DEBUG(DEBUG_INFO, ">>>>>>>>>PCP_Upgrade_NewVersionNotice Running<<<<<<<<<");

	return PCP_ExecuteSuccess;

#endif
}

/**********************************************************************************************************
 @Function			PCP_ResultCodeTypeDef PCP_Upgrade_DataDownload(PCP_ClientsTypeDef* pClient, u16 SliceIndex, u8* UpgradeData, u16 UpgradeDataLength)
 @Description			PCP_Upgrade_DataDownload				: PCP���������ش���
 @Input				pClient							: PCP�ͻ���ʵ��
					SliceIndex						: PCP��Ƭ���
					UpgradeData						: ����������
					UpgradeDataLength					: ����������
 @Return				void
**********************************************************************************************************/
PCP_ResultCodeTypeDef PCP_Upgrade_DataDownload(PCP_ClientsTypeDef* pClient, u16 SliceIndex, u8* UpgradeData, u16 UpgradeDataLength)
{
#ifdef GD25Q_80CSIG
	PCP_ResultCodeTypeDef PCPResultCodeStatus = PCP_ExecuteSuccess;
	
	if (GD25Q80CSIG_OK != GD25Q_SPIFLASH_Get_Status()) {
		PCPResultCodeStatus = PCP_InternalAnomaly;
		goto exit;
	}
	
	Radio_Rf_Interrupt_Deinit();
	GD25Q_SPIFLASH_WakeUp();
	GD25Q_SPIFLASH_Init();
	
	if ((SliceIndex + 1) == pClient->UpgradeExecution.PackSliceNum) {
		/* �÷�ƬΪ���һ����Ƭ��ȡ�䳤�� */
		pClient->UpgradeExecution.PackLastSliceSize = UpgradeDataLength;
	}
	
	if (GD25Q_SPIFLASH_GetByte(APP1_PACKSLICE_STATUS_OFFSET + SliceIndex) != 0xFF) {
		/* �÷�Ƭ������д�� */
		pClient->UpgradeExecution.PackSliceIndex = SliceIndex + 1;
		GD25Q_SPIFLASH_PowerDown();
		GPRS_DEBUG(DEBUG_INFO, "PackSlice has been write");
		goto exit;
	}
	
	if (UpgradeDataLength > pClient->UpgradeExecution.PackSliceSize) {
		/* �÷�Ƭ���ݳ����쳣 */
		GD25Q_SPIFLASH_PowerDown();
		GPRS_DEBUG(DEBUG_INFO, "PackSlice Size Fail");
		goto exit;
	}
	
	/* ��Ƭ����д���Ƭ��ַ */
	/* д��APP1DATA */
	GD25Q_SPIFLASH_WriteBuffer(UpgradeData, APP1_DATA_ADDR + SliceIndex * UPGRADE_PACK_SLICE_BLOCK_SIZE, UpgradeDataLength);
	GD25Q_SPIFLASH_SetByte(APP1_PACKSLICE_STATUS_OFFSET + SliceIndex, 0x00);
	
	pClient->UpgradeExecution.PackSliceIndex = SliceIndex + 1;
	GD25Q_SPIFLASH_PowerDown();
	GPRS_DEBUG(DEBUG_INFO, "PackSlice write to flash");
	
exit:
	return PCPResultCodeStatus;


#else


	GPRS_DEBUG(DEBUG_INFO, ">>>>>>>>>PCP_Upgrade_DataDownload Running<<<<<<<<<");

	return PCP_ExecuteSuccess;
	
#endif

	

}

/**********************************************************************************************************
 @Function			PCP_ResultCodeTypeDef PCP_Upgrade_DataAssemble(PCP_ClientsTypeDef* pClient)
 @Description			PCP_Upgrade_DataAssemble				: PCP��������װУ�鴦��
 @Input				pClient							: PCP�ͻ���ʵ��
 @Return				void
**********************************************************************************************************/
PCP_ResultCodeTypeDef PCP_Upgrade_DataAssemble(PCP_ClientsTypeDef* pClient)
{
#ifdef GD25Q_80CSIG
	PCP_ResultCodeTypeDef PCPResultCodeStatus = PCP_ExecuteSuccess;
	u8 SPIFlashCheckCode = 0;
	
	if (GD25Q80CSIG_OK != GD25Q_SPIFLASH_Get_Status()) {
		PCPResultCodeStatus = PCP_InternalAnomaly;
		goto exit;
	}
	
	Radio_Rf_Interrupt_Deinit();
	GD25Q_SPIFLASH_WakeUp();
	GD25Q_SPIFLASH_Init();
	
	/* У�����ݰ��Ƿ�������� */
	if (pClient->UpgradeExecution.PackSliceNum != GD25Q_SPIFLASH_GetNumofByte(APP1_PACKSLICE_STATUS_OFFSET, pClient->UpgradeExecution.PackSliceNum, 0x00)) {
		/* ���ݰ�û��������� */
		GD25Q_SPIFLASH_EraseSector(APP1_INFORMATION_ADDR);
		GD25Q_SPIFLASH_PowerDown();
		GPRS_DEBUG(DEBUG_INFO, "Upgrade Check Fail");
		PCPResultCodeStatus = PCP_UpgradePacketCheckFail;
		goto exit;
	}
	
	/* У������������ */
	for (int packIndex = 0; packIndex < (pClient->UpgradeExecution.PackSliceNum - 1); packIndex++) {
		GD25Q_SPIFLASH_ReadBuffer(pClient->DataProcessStack, APP1_DATA_ADDR + packIndex * UPGRADE_PACK_SLICE_BLOCK_SIZE, pClient->UpgradeExecution.PackSliceSize);
		SPIFlashCheckCode = CalculateSumCheckCode(SPIFlashCheckCode, pClient->DataProcessStack, pClient->UpgradeExecution.PackSliceSize);
	}
	GD25Q_SPIFLASH_ReadBuffer(pClient->DataProcessStack, APP1_DATA_ADDR + ((pClient->UpgradeExecution.PackSliceNum - 1) * UPGRADE_PACK_SLICE_BLOCK_SIZE), pClient->UpgradeExecution.PackLastSliceSize);
	SPIFlashCheckCode = CalculateSumCheckCode(SPIFlashCheckCode, pClient->DataProcessStack, pClient->UpgradeExecution.PackLastSliceSize);
	if (GD25Q_SPIFLASH_GetWord(APP1_DATA_CHECK_CODE_OFFSET) != SPIFlashCheckCode) {
		/* У������� */
		GD25Q_SPIFLASH_EraseSector(APP1_INFORMATION_ADDR);
		GD25Q_SPIFLASH_PowerDown();
		GPRS_DEBUG(DEBUG_INFO, "Upgrade Check Fail");
		PCPResultCodeStatus = PCP_UpgradePacketCheckFail;
		goto exit;
	}
	
	GD25Q_SPIFLASH_PowerDown();
	GPRS_DEBUG(DEBUG_INFO, "Upgrade Check Ok");
	
exit:
	return PCPResultCodeStatus;


#else

	GPRS_DEBUG(DEBUG_INFO, ">>>>>>>>>PCP_Upgrade_DataAssemble Running<<<<<<<<<");

	return PCP_ExecuteSuccess;

#endif
}

/**********************************************************************************************************
 @Function			PCP_ResultCodeTypeDef PCP_Upgrade_AfterUpdata(PCP_ClientsTypeDef* pClient)
 @Description			PCP_Upgrade_AfterUpdata				: PCP��������ʼ��������
 @Input				pClient							: PCP�ͻ���ʵ��
 @Return				void
**********************************************************************************************************/
PCP_ResultCodeTypeDef PCP_Upgrade_AfterUpdata(PCP_ClientsTypeDef* pClient)
{
#ifdef GD25Q_80CSIG
	PCP_ResultCodeTypeDef PCPResultCodeStatus = PCP_ExecuteSuccess;
	
	if (GD25Q80CSIG_OK != GD25Q_SPIFLASH_Get_Status()) {
		PCPResultCodeStatus = PCP_InternalAnomaly;
		goto exit;
	}
	
	GPRS_DEBUG(DEBUG_INFO, "Start Boot Upgrade APP ...");
	Radio_Rf_Interrupt_Deinit();
	GD25Q_SPIFLASH_WakeUp();
	GD25Q_SPIFLASH_Init();
	
	if (GD25Q_SPIFLASH_GetByte(APP1_INFO_UPGRADE_STATUS_OFFSET) == 0xFF) {
		GD25Q_SPIFLASH_SetByte(APP1_INFO_UPGRADE_STATUS_OFFSET, 0x55);									//��ʶ���������ҿ�����
	}
	
	TCFG_EEPROM_SetBootMode(TCFG_ENV_BOOTMODE_SPIFLASH_UPGRADE);										//��������ģʽ
	BEEP_CtrlRepeat_Extend(5, 50, 50);
	Stm32_System_Software_Reboot();
	
exit:
	return PCPResultCodeStatus;

#else


	GPRS_DEBUG(DEBUG_INFO, ">>>>>>>>>PCP_Upgrade_AfterUpdata Running<<<<<<<<<");

	return PCP_ExecuteSuccess;

	
#endif
}

/********************************************** END OF FLEE **********************************************/

