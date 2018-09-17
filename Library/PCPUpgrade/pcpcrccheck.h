#ifndef __PCP_CRC_CHECK_H
#define   __PCP_CRC_CHECK_H

#include "include.h"	


u16 PCPCrcCheck_doCrcCheckCode(u16 regCode, u8* message, u16 length);				//计算PCPCrc校验值
u16 PCPCrcCheck_getCrcCheckCode(u8* crcCheckData, u16 crcCheckLength);			//获取计算PCPCrc校验值

#endif /* __PCP_CRC_CHECK_H */	

