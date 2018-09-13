#include "basic_fun.h"


#define HEX(x)   x < 10? (x + '0'):(x + 'A'-10)


///////////////////BCD HEX ת��////////////////////////////////////////////////
// ��������  : BCDתHex
u8 BCDToHex(u8 TC_char)
{	
  	u8 Hbcd,Lbcd;	
	Hbcd = TC_char>>4;
	Lbcd = TC_char&0x0f;
	TC_char = Hbcd*10+Lbcd;
	return TC_char;
}

// ��������  : BCDתHex
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



// ��������  : BCDתHex
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
// ��������  : HexתBCD
// hexֵ<100(0x64)
u8 HexToBCD(u8 TC_char)
{	
	u8 Hbcd,Lbcd;
	Hbcd = TC_char/10;
	Hbcd %= 10;
	Lbcd = TC_char%10;
	TC_char = Hbcd*0x10+Lbcd;
	return TC_char;
}
// ��������  : HexתBCD
u16 HexToIntBCD(u8 HexVar)
{	
  	u16  BcdVar = 0;	
	
	BcdVar|=(HexVar/100)<<8;
	HexVar=HexVar%100;
	
	BcdVar|=(HexVar/10)<<4;
	BcdVar|=HexVar%10;
 	
 	return BcdVar;
}
// ��������  : 16λHexתBCD
// hexֵ<10000(0x2710)
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
// ��������  : �ֽڵĴ�С�˵�ת��
// �������1 : Count  ��������ݸ���
// �������2 : P      ���������ָ��
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
// ��������  : ������char�ͺϳ�һ��int
u16 TwoCharToInt(u8 c1,u8 c2)
{	
  u16 tempcmd;
	tempcmd=c1;
	tempcmd<<=8;
	tempcmd|=c2;
	return tempcmd;
}
////////////////////hex ascii ��ת��///////////////////////////////////////////
// ��������  : "9"��ASCII����"A"��ASCII��ֵ���==7  ע����ĸ��ͳһΪ��д��ĸ��ASCII��ֵ 
u8 Hi_HexToAsc(u8 c1)
{	
	c1=c1>>4;	
	return(c1>=0xa? (c1+0x37):(c1+0x30));
}	
// ��������  : "9"��ASCII����"A"��ASCII��ֵ���==7  ע����ĸ��ͳһΪ��д��ĸ��ASCII��ֵ 
u8 Low_HexToAsc(u8 c1)
{	
	c1=c1&0xf;
	return(c1>=0xa? (c1+0x37):(c1+0x30));
}

//��һ���ֽڵ�����ת��Ϊ����ASCII ��
void CharToAsc(u8 c1,u8 *buf)	
{
	u8 t1,t2;
	
	t1 = Hi_HexToAsc(c1);
	t2 = Low_HexToAsc(c1);	
	
	*buf 	 = t1;	
	*(buf+1) = t2;
}

//�������ֽڵ�����ת��Ϊ�ĸ�ASCII ��
void u16ToAsc(u16 c1,u8 *buf)
{
	u8 t1,t2;	
	
	t1 = c1&0xFF;
	t2 = (c1>>8)&0xFF;					
	
	CharToAsc(t2,buf);					
	
	CharToAsc(t1,buf+2);	
}

//���ĸ��ֽڵ�����ת��Ϊ�˸�ASCII ��
void u32ToAsc(u32 c1,u8 *buf)	
{	
	u16 t1,t2;
	
	t1 = c1&0xFFFF;
	t2 = (c1>>16)&0xFFFF;				

	u16ToAsc(t2,buf);		
	u16ToAsc(t1,buf+4);	
}
// ��������  : ��ascii ��'0'~'f' ת���� 0~f һ��byte
u8 AscToHex(u8 c1)
{	
	if (c1>=0x30 && c1<=0x39) 		c1=c1-0x30;
	else if (c1>=0x41 && c1<=0x46)	c1=c1-0x37;
	else if (c1>=0x61 && c1<=0x66)	c1=c1-0x57;
	return(c1);
}
// ��������  : ������ASC��ϳ�һ��HEX�ַ�
u8 TwoAscTOHex(u8 TPA,u8 TPB)   
{    
	TPA = AscToHex(TPA);
	TPB = AscToHex(TPB);
	return((TPA<<4)|TPB);		
}	



// ��������  : ��ASC�뻺��ת����HEX������
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
//����д��ĸת��ΪСд
u8 CapitalTo(u8 cl)
{
	if (cl>=0x41 && cl<=0x5a)
	{
		cl +=0x20;
	}
	return cl;
}
//����д��ĸת��ΪСд
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



static s32 debug_level = DEBUG_ERROR; // ���Դ�ӡ��Ϣ����ȼ�
	
/**		  	
 * @brief ��ȡ���Դ�ӡ��Ϣ����ȼ�
 * @param ��
 * @return ���Դ�ӡ��Ϣ����ȼ�
 */
s32 get_debug_level(void)
{
	return debug_level;
}

/**		  
 * @brief		���õ��Դ�ӡ��Ϣ����ȼ� 
 * @param[in] level ���Դ�ӡ��Ϣ����ȼ�
 * @return ��
 */
void set_debug_level(s32 level)
{
	debug_level = level;
}

	

