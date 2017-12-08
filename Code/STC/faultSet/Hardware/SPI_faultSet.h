#ifndef _SPI_FAULTSET__
#define _SPI_FAULTSET__

#include "config.h"
#include "GPIO.h"
#include "delay.h"

#define 	SPI_DS	P13
#define  SPI_CLK	P15
#define  SPI_STP	P14
#define  SPI_MR	P16

//����Ӳ����������
void GPIO_SPI_Init(void);
void Fast595allClr(void);
void Frame595Clr(u8 length);
void DatsIn595(u8 Dats);
void Frame595Send(u8 Frame[],u8 length);
//���ܺ���
u8 byteReverse(u8 Byte);
void BytesReverse(u8 datsin[],u8 datsout[],u16 num);
//Ӧ�ú���

#endif