#ifndef __SPI_H
#define __SPI_H
#include "sys.h"

void SPI2_Init(void);			 //��ʼ��SPI��
void SPI2_SetSpeed(u8 SpeedSet); //����SPI�ٶ�   

u8 SPI_WriteByte(SPI_TypeDef* SPIx,u8 TxData);
void SPI1_Init(void);
 		 
#endif

