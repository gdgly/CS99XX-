#include "stm32f10x.h"
#include "SPI_CPLD.H"
#include "BSP_config.h"
#include "string.h"
#include 	<math.h>


#define     CPLD_DELAY      1
#define     CPLD_DELAY_F()      {int i = 0; for(i = 0; i < 10; i++);}

#define			SPI_CS_EN()							GPIO_ResetBits(GPIOG,GPIO_Pin_11)
#define			SPI_CS_DIS()						GPIO_SetBits(GPIOG,GPIO_Pin_11)
#define			SPI_MOSI_HIGH()						GPIO_SetBits(GPIOG,GPIO_Pin_14)
#define			SPI_MOSI_LOW()						GPIO_ResetBits(GPIOG,GPIO_Pin_14)
#define			SPI_CLK_HIGH()						GPIO_SetBits(GPIOB,GPIO_Pin_8)
#define			SPI_CLK_LOW()						GPIO_ResetBits(GPIOB,GPIO_Pin_8)
#define			SPI_MISO_DI()						GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_12)


#define			SPI_HV_CS_EN()						GPIO_ResetBits(GPIOE,GPIO_Pin_0)
#define			SPI_HV_CS_DIS()		    			GPIO_SetBits(GPIOE,GPIO_Pin_0)
#define			SPI_HV_MOSI_HIGH()					GPIO_SetBits(GPIOE,GPIO_Pin_1)
#define			SPI_HV_MOSI_LOW()					GPIO_ResetBits(GPIOE,GPIO_Pin_1)
#define			SPI_HV_CLK_HIGH()					GPIO_SetBits(GPIOE,GPIO_Pin_6)
#define			SPI_HV_CLK_LOW()					GPIO_ResetBits(GPIOE,GPIO_Pin_6)
#define			SPI_HV_MISO_DI()					() /* ����Ҫ�������� */

void HV_SPI_CPLD_GPIO(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);  //����GPIOEʱ��
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 			 //�������
    GPIO_Init(GPIOE, &GPIO_InitStructure);
}

void SPI_CPLD_GPIO(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOG, ENABLE);  //����GPIOEʱ��
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 			 //�������
    GPIO_Init(GPIOG, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 			 //�������
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 				//��������
    GPIO_Init(GPIOG, &GPIO_InitStructure);
    
}

void init_spi_cpld(void)
{
    SPI_CPLD_GPIO();
    HV_SPI_CPLD_GPIO();
}
//*************************************************************

/* ��������cpld_write
 * ����  ����CPLDд���������һ�ֽ�
 * ����  ������
 * ���  ������ֵ
 */
//**************************************************************
void cpld_write(u32 SendData)
{
	u8  ShiftCounter;
	u32 DataBuffer;
	
	SPI_CS_DIS();
    CPLD_DELAY_F();;
	
	SPI_CS_EN(); //CS=0;ƬѡCPLD
    CPLD_DELAY_F();;
	
	for(ShiftCounter=0;ShiftCounter<26;ShiftCounter++)
	{
		DataBuffer = SendData;
		DataBuffer = DataBuffer & 0x00000001; //ȡ�������λ
		SendData = SendData >>1; //��������һλ
		SPI_CLK_HIGH(); //ʱ���ø�
		
		if(DataBuffer == 0x00000000)
		{
            SPI_MOSI_LOW();
		}
		else
		{
            SPI_MOSI_HIGH();
		}
        CPLD_DELAY_F();;
		
        SPI_CLK_LOW(); //����д��CPLD
        CPLD_DELAY_F();;
	}
    
    CPLD_DELAY_F();
    
	SPI_CS_DIS(); //CS=1;ȡ��Ƭѡ
}

uint8_t cpld_hv_buf[2];

void cmd_cpld_hv_port(uint8_t ID, uint8_t bits, uint8_t status)
{
	status != 0? (cpld_hv_buf[ID] |= 1<<bits) : (cpld_hv_buf[ID] &= ~(1<<bits));
}
void reset_posrts_cpld_ctrl(void)
{
    memset(cpld_hv_buf, 0, sizeof cpld_hv_buf);
}
/* ��cpld������ѹ������λ����������ֵ������ֵ */
void cpld_count_angle(double *sin_v, double *cos_v)
{
    uint16_t cpld_data1 = 0;
    uint16_t cpld_data2 = 0;
	double cos_val = 0.0;
	double sin_val = 0.0;
    double PI = 3.1415926;
    
    cpld_data1 = ReadDataFromCPLD(CPLD_READ_PHASE);	//������λ��
    cpld_data2 = ReadDataFromCPLD(CPLD_VOL_CUNT);//����ѹ����ֵ
    
    if(cpld_data2 > 0)
    {
        sin_val = sin((double)cpld_data1 / cpld_data2 * PI);
        cos_val = cos((double)cpld_data1 / cpld_data2 * PI);
    }
    
    *sin_v = sin_val;
    *cos_v = cos_val;
}

void hv_cpld_write(u32 SendData)
{
    #define CPLD_CMD_DELAY    5
	u8  ShiftCounter;
	u32 DataBuffer;
	
    if(SendData == CMD_CPLD_HV_H)
    {
        SendData |= cpld_hv_buf[HV_PORT_H];
    }
    else
    {
        SendData |= cpld_hv_buf[HV_PORT_L];
    }
    
	SPI_HV_CS_DIS();
    CPLD_DELAY_F();
	
	SPI_HV_CS_EN(); //CS=0;ƬѡCPLD
    CPLD_DELAY_F();
	
	for(ShiftCounter=0;ShiftCounter<26;ShiftCounter++)
	{
		DataBuffer = SendData;
		DataBuffer = DataBuffer & 0x1; //ȡ�������λ
		SendData = SendData >>1; //��������һλ
		SPI_HV_CLK_HIGH(); //ʱ���ø�
        CPLD_DELAY_F();
		
		if(DataBuffer == 0x0)
		{
            SPI_HV_MOSI_LOW();
		}
		else
		{
            SPI_HV_MOSI_HIGH();
		}
        
        CPLD_DELAY_F();
		
		SPI_HV_CLK_LOW(); //����д��CPLD
        CPLD_DELAY_F();
	}
    
	SPI_HV_CS_DIS(); //CS=1;ȡ��Ƭѡ
    CPLD_DELAY_F();
}

//��CPLD�ж�����
u16 ReadDataFromCPLD(u32 SendData)
{
	u8  ShiftCounter;
	u16 DataBuffer=0;
	u32 SendDataBuffer;
	
	SendDataBuffer   = SendData;
	cpld_write(SendDataBuffer); //��CPLD���Ͷ�����
	
    CPLD_DELAY_F();
	SPI_CS_EN(); //Ƭѡ
	DataBuffer = 0;
	
	for(ShiftCounter=0;ShiftCounter<16;ShiftCounter++)
	{
		DataBuffer = DataBuffer << 1; //����һλ
        
		SPI_CLK_HIGH(); //ʱ���ø�
        CPLD_DELAY_F();
        
		SPI_CLK_LOW();
        CPLD_DELAY_F();;
		
		if(SPI_MISO_DI() == 1)
		{
			DataBuffer = DataBuffer | 0x0001; //��λ��1
		}
		else
		{
			DataBuffer = DataBuffer & 0xfffe; //��λ��0
		}
	}
	
	SPI_CS_DIS();
    CPLD_DELAY_F();
	
	return(DataBuffer);
}


/******************* (C) COPYRIGHT 2014 ��ʢ���� *****END OF FILE****/