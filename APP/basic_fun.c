#include "basic_fun.h"


#define HEX(x)   x < 10? (x + '0'):(x + 'A'-10)


///////////////////BCD HEX 转换////////////////////////////////////////////////
// 功能描述  : BCD转Hex
u8 BCDToHex(u8 TC_char)
{	
  	u8 Hbcd,Lbcd;	
	Hbcd = TC_char>>4;
	Lbcd = TC_char&0x0f;
	TC_char = Hbcd*10+Lbcd;
	return TC_char;
}

// 功能描述  : BCD转Hex
u16 U16BCDToU16Hex(u16 TC_char)
{
	u16 k,hex=0;
	u8 bcd,i;	
	
	for(i=0,k=1;i<4;i++)
	{				
		bcd = TC_char>>4*i;
		bcd = bcd&0x0f;
		hex += bcd*k;
		k = k*10;
	}
	
	return hex;
}



// 功能描述  : BCD转Hex
u32 U32BCDToU32Hex(u32 TC_char)
{		
	u32 k,hex=0;
	u8 bcd,i;	
	for(i=0,k=1;i<8;i++)
	{	
		bcd = TC_char>>4*i;
		bcd = bcd&0x0f;
		hex += bcd*k;
		k = k*10;
	}
	
	return hex;
}
// 功能描述  : Hex转BCD
// hex值<100(0x64)
u8 HexToBCD(u8 TC_char)
{	
	u8 Hbcd,Lbcd;
	Hbcd = TC_char/10;
	Hbcd %= 10;
	Lbcd = TC_char%10;
	TC_char = Hbcd*0x10+Lbcd;
	return TC_char;
}
// 功能描述  : Hex转BCD
u16 HexToIntBCD(u8 HexVar)
{	
  	u16  BcdVar = 0;	
	
	BcdVar|=(HexVar/100)<<8;
	HexVar=HexVar%100;
	
	BcdVar|=(HexVar/10)<<4;
	BcdVar|=HexVar%10;
 	
 	return BcdVar;
}
// 功能描述  : 16位Hex转BCD
// hex值<10000(0x2710)
u16 IntHexToIntBCD(u16 HexVar)
{
	u16  BcdVar=0;
	
	BcdVar|=(HexVar/1000)<<12;
	HexVar=HexVar%1000;

	BcdVar|=(HexVar/100)<<8;
	HexVar=HexVar%100;

	BcdVar|=(HexVar/10)<<4;

	BcdVar|=HexVar%10;

 	return BcdVar;
}
///////////////////////////////////////////////////////////////////////////////
// 功能描述  : 字节的大小端的转换
// 输入参数1 : Count  处理的数据个数
// 输入参数2 : P      处理的数据指针
void ByteEndian(u16 Count ,u8 *P)
{
	u16 Ta,i = 0;
	Count--;
	for(i=0; i<Count;i++,Count--)
	{
		Ta=P[i];
		P[i]=P[Count];
		P[Count]=Ta;      
	}
}
// 功能描述  : 将两个char型合成一个int
u16 TwoCharToInt(u8 c1,u8 c2)
{	
  u16 tempcmd;
	tempcmd=c1;
	tempcmd<<=8;
	tempcmd|=c2;
	return tempcmd;
}
////////////////////hex ascii 码转换///////////////////////////////////////////
// 功能描述  : "9"的ASCII码与"A"的ASCII码值相差==7  注：字母都统一为大写字母的ASCII码值 
u8 Hi_HexToAsc(u8 c1)
{	
	c1=c1>>4;	
	return(c1>=0xa? (c1+0x37):(c1+0x30));
}	
// 功能描述  : "9"的ASCII码与"A"的ASCII码值相差==7  注：字母都统一为大写字母的ASCII码值 
u8 Low_HexToAsc(u8 c1)
{	
	c1=c1&0xf;
	return(c1>=0xa? (c1+0x37):(c1+0x30));
}

//将一个字节的数据转化为两个ASCII 码
void CharToAsc(u8 c1,u8 *buf)	
{
	u8 t1,t2;
	
	t1 = Hi_HexToAsc(c1);
	t2 = Low_HexToAsc(c1);	
	
	*buf 	 = t1;	
	*(buf+1) = t2;
}

//将两个字节的数据转化为四个ASCII 码
void u16ToAsc(u16 c1,u8 *buf)
{
	u8 t1,t2;	
	
	t1 = c1&0xFF;
	t2 = (c1>>8)&0xFF;					
	
	CharToAsc(t2,buf);					
	
	CharToAsc(t1,buf+2);	
}

//将四个字节的数据转化为八个ASCII 码
void u32ToAsc(u32 c1,u8 *buf)	
{	
	u16 t1,t2;
	
	t1 = c1&0xFFFF;
	t2 = (c1>>16)&0xFFFF;				

	u16ToAsc(t2,buf);		
	u16ToAsc(t1,buf+4);	
}
// 功能描述  : 把ascii 的'0'~'f' 转换成 0~f 一个byte
u8 AscToHex(u8 c1)
{	
	if (c1>=0x30 && c1<=0x39) 		c1=c1-0x30;
	else if (c1>=0x41 && c1<=0x46)	c1=c1-0x37;
	else if (c1>=0x61 && c1<=0x66)	c1=c1-0x57;
	return(c1);
}
// 功能描述  : 把两个ASC码合成一个HEX字符
u8 TwoAscTOHex(u8 TPA,u8 TPB)   
{    
	TPA = AscToHex(TPA);
	TPB = AscToHex(TPB);
	return((TPA<<4)|TPB);		
}	



// 功能描述  : 把ASC码缓冲转换成HEX缓冲区
void BufAscToHex(u8 *PTemp,u16 Len)  
{	
  	u8 Ta,*PsouA;		
	u16 i;
	PsouA = PTemp;
	Len >>=1;
	
	for(i=0; i<Len; i++)
	{
		Ta = *PTemp++;
		*PsouA++ = TwoAscTOHex(Ta,*PTemp++);
	}
}


int  BufStrToASC(char *PTemp,u16 size)
{    
	char* pChar = NULL;  
	u8 temp_H,temp_L;
	int count;	
	
	if(size*2 < 1024)
	{		
		memcpy((void*)(PTemp+size),(void*)PTemp,size);
		pChar = PTemp + size;			
		for(count=0; (pChar != '\0')&&(count < size);count++) 
		{ 	
			temp_H = HEX(((*pChar) >> 4)); 
			temp_L = HEX(((*pChar) & 0x0F));
			PTemp[count * 2] = temp_H; 
			PTemp[count * 2 + 1] = temp_L;			  
			pChar ++; 
		}
		return 1;
	}
		
   	return 0;
}


//////////////////////////////////////////////////////////////////////////////
//将大写字母转换为小写
u8 CapitalTo(u8 cl)
{
	if (cl>=0x41 && cl<=0x5a)
	{
		cl +=0x20;
	}
	return cl;
}
//将大写字母转换为小写
void BufCapitalTo(u8 *p,u16 len)
{
	u16 i;
	for(i=0;i<len;i++)
	{
		*p = CapitalTo(*p);
		p++;
	}
}









/**@file 
 * @note tiansixinxi. All Right Reserved.
 * @brief  
 * 
 * @author  madongfang
 * @date     2016-5-26
 * @version  V1.0.0
 * 
 * @note ///Description here 
 * @note History:        
 * @note     <author>   <time>    <version >   <desc>
 * @note  
 * @warning  
 */



static s32 debug_level = DEBUG_ERROR; // 调试打印信息输出等级
	
/**		  	
 * @brief 获取调试打印信息输出等级
 * @param 无
 * @return 调试打印信息输出等级
 */
s32 get_debug_level(void)
{
	return debug_level;
}

/**		  
 * @brief		设置调试打印信息输出等级 
 * @param[in] level 调试打印信息输出等级
 * @return 无
 */
void set_debug_level(s32 level)
{
	debug_level = level;
}

	

