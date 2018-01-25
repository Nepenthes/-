#include "SPI_faultSet.h"

void GPIO_SPI_Init(void){     //595ͨ�����ų�ʼ��

	GPIO_InitTypeDef gpio_SPI;
	
	gpio_SPI.Mode = GPIO_OUT_PP;
	gpio_SPI.Pin  = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
	
	GPIO_Inilize(GPIO_P1,&gpio_SPI);
	
	SPI_STP = 0;
	SPI_MR  = 1;
}

void  DatsIn595(u8 Dats){	//595һԪ��ֵ

	SPI_CLK 	= 0;
	SPI_DS	= Dats & 0x01;
	SPI_CLK	= 1;
	Dats	  	= Dats>>1;
}

void Frame595Clr(u8 length){ 	//595��Ԫ����

	u16 loop;
	
	SPI_STP = SPI_CLK	= SPI_DS = 0;
	
	for(loop = 0; loop < length * 8; loop ++){
	
		SPI_CLK	= 1;
		SPI_CLK	= 0;
	}
	
	SPI_STP = 1;
	SPI_STP = 0;
}

void Fast595allClr(void){		//����595��������

	SPI_MR = 0;
	delay_ms(10);
	SPI_MR = 1;
	delay_ms(10);
}

void Frame595Send(u8 Frame[],u8 length){	//595��Ԫ��ֵ

	u8 loop;
	
	SPI_STP = 0;
	for(loop = (length - 1); loop >= 0; loop++)DatsIn595(Frame[loop]);	
	SPI_STP = 1;
	SPI_STP = 0;
}

u8 byteReverse(u8 Byte){	//���ֽڵ���

	Byte = (Byte & 0x55) << 1 | (Byte & 0xaa) >> 1;
	Byte = (Byte & 0x33) << 2 | (Byte & 0xcc) >> 2;
	Byte = (Byte & 0x0f) << 4 | (Byte & 0xf0) >> 4;
	return Byte;
}

void BytesReverse(u8 datsin[],u8 datsout[],u16 num){	//���ֽڵ���

	u16 loop;
	
	for(loop = 0;loop < num;loop ++)
		datsout[loop] = byteReverse(datsin[loop]);
}

