/*
 * Copyright(c) 2014,�Ͼ���ʢ����
 * All rights reserved
 * �ļ����ƣ�STM32_FSMC.C
 * ժ  Ҫ  ��������
 * ��ǰ�汾��V0.0����д�ߣ�����
 * ��ʷ�汾��
 * �޸ļ�¼��
 *
 */
#include "STM32_FSMC.H"

/*
 * ��������FSMC_Configuration
 * ����  ��BANK4 NORģʽB����
 * ����  ����
 * ���  ����
 * ����  ����
 */
void FSMC_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;                  //����GPIO�Ľṹ
  FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;   //��Ҫ����NOR FLASHģʽ����
  FSMC_NORSRAMTimingInitTypeDef  p;                     //��Ҫ����NOR FLASHģʽ�µĶ�дʱ��

/******************** ʱ������ ********************/
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC,ENABLE);     //����FSMCʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE 
                       | RCC_APB2Periph_GPIOF | RCC_APB2Periph_GPIOG
					   | RCC_APB2Periph_AFIO, ENABLE);  //������Ӧ�˿ڵ�ʱ�� 
/******************** �ܽ����� ********************/
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;      //�����������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     //����ٶ�
  /* �˿�D */
  GPIO_InitStructure.GPIO_Pin  = 0xF8FB;                //FSMC_D1D0A18A17,A16D15D14D13,NE1`WAIT`WR`RW,CLK*D3D2					
  GPIO_Init(GPIOD, &GPIO_InitStructure);                //��ʼ���˿�D
  /* �˿�E */
  GPIO_InitStructure.GPIO_Pin  = 0xFF88;                //FSMC_D12-D4***,A19***
  GPIO_Init(GPIOE, &GPIO_InitStructure);                //��ʼ���˿�E
  /* �˿�F */
  GPIO_InitStructure.GPIO_Pin  = 0xF03F;                //FSMC_A9-A6,****,**A5-A0
  GPIO_Init(GPIOF, &GPIO_InitStructure);                //��ʼ���˿�F
  /* �˿�G */
  GPIO_InitStructure.GPIO_Pin  = 0x167F;                //FSMC_***NE4,*NE3`NE2*,*INT2`A15-A10				
  GPIO_Init(GPIOG, &GPIO_InitStructure);                //��ʼ���˿�G

/******************** FSMC���� ********************/
  p.FSMC_AddressSetupTime = 0XF;                        //��ַ����ʱ��
  p.FSMC_AddressHoldTime = 0XF;                         //��ַ����ʱ��
  p.FSMC_DataSetupTime = 0X50;                          //���ݽ���ʱ��
  p.FSMC_BusTurnAroundDuration = 0XF;                   //���߻ָ�ʱ��
  p.FSMC_CLKDivision = 0X2;                             //ʱ�ӷ�Ƶ
  p.FSMC_DataLatency = 0Xf;                             //���ݱ���ʱ��
  p.FSMC_AccessMode = FSMC_AccessMode_B;                //�ڵ�ַ\�����߲����õ�����ª�ABCDģʽ�����𲻴�
                                                        //����Ա����ֻ��ʹ����չģʽ����Ч

  FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;      //���������ַ�߲�����
  FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_NOR;                  //�洢������NOR FLASH
  
  FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;    //ʹ���첽дģʽ����ֹͻ��ģʽ
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;  //����Ա������ֻ��ͻ��ģʽ����Ч���ȴ��źż���Ϊ�� 
  FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;                  //��ֹ�Ƕ���ͻ��ģʽ
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;//����Ա���ý���ͻ��ģʽ����Ч��NWAIT�ź���ʲôʱ�ڲ���
  FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;              //����Ա������ֻ��ͻ��ģʽ����Ч������NWAIT�ź� 
  FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;              //��ֹͻ��д���� 
  FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;       //дʹ��
  FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;          //��չģʽ����ʹ�ö����Ķ���дģʽ
  FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;                        //���ö�дʱ�� 
  FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p;                            //����дʱ�� 

  
  /********************* BANK1����>LCD ************************/
  FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;                        //NOR FLASH��BANK4 
  FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_8b;         //���ݿ���Ϊ8λ
  FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure); 
  FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);
  
/********************* BANK2����>SRAM ************************/
  FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM2;                        //NOR FLASH��BANK4
  FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_8b;         //���ݿ���Ϊ8λ
  FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure); 
  FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM2, ENABLE);   
  
  
/********************* BANK3����>TFT ************************/
  FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM3;                        //NOR FLASH��BANK4
  FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;         //���ݿ���Ϊ16λ
  FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure); 
  FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM3, ENABLE);   
  
   
/********************* BANK4����>LCD ************************/
  FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM4;                        //NOR FLASH��BANK4
  FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_8b;         //���ݿ���Ϊ8λ
  FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure); 
  FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM4, ENABLE); 
}

/******************* (C) COPYRIGHT 2014 ��ʢ���� *****END OF FILE*******************/