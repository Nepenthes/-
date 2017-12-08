#include "485Trans_USART.h"

extern u8 BD_addr,BD_type;	 //��ʰ��ַ������

const u8 flgCfmHead = 0x7e;						//֡ͷ���
const u8 frameCfmTail[2] = {0x0d,0x0a};		//֡β������
u8 frameCfmHead[3] = {0};							//֡ͷ����ͷ��ǡ���ַ���������

extern u8 xdata UART485_BUF[COM_RX2_Lenth];	//485�������ݻ���

u8 COREDATS[COREDATS_SIZE] = {0};		//֡�����ݻ���

void USART485Trans_Init(void){	//485ͨѶ�ӿڳ�ʼ��

	GPIO_InitTypeDef gpio_485;
	COMx_InitDefine  UART485Port;
	
	gpio_485.Mode = GPIO_OUT_PP;
	gpio_485.Pin  = GPIO_Pin_2;
	GPIO_Inilize(GPIO_P1,&gpio_485);
	
	UART485Port.UART_Mode = UART_8bit_BRTx;
	UART485Port.UART_BRT_Use = BRT_Timer2;
	UART485Port.UART_BaudRate = 38400UL;
	UART485Port.Morecommunicate = DISABLE;
	UART485Port.UART_RxEnable = ENABLE;
	UART485Port.BaudRateDouble = DISABLE;
	UART485Port.UART_Interrupt = ENABLE;
	UART485Port.UART_Polity = PolityHigh;
	UART485Port.UART_P_SW = UART2_SW_P10_P11; 
	UART485Port.UART_RXD_TXD_Short = DISABLE;
	USART_Configuration(USART2,&UART485Port);
	
	RD485; //��ʼ����������ʹ��
}

void frameInsAccept(void){	//��������

	u8 loop,corelen;
	
	frameCfmHead[0] = flgCfmHead;	 //֡ͷ��װ
	frameCfmHead[1] = BD_addr;
	frameCfmHead[2] = BD_type;
	
	for(loop = 0; loop < COM_RX2_Lenth; loop ++){
		
		if(UART485_BUF[loop] = flgCfmHead){	//֡ͷ���У��/*��һ��*/
	
			if(!memcmp(frameCfmHead,&UART485_BUF[loop],3)){ //֡ͷУ��/*�ڶ���*/
			
				corelen 	= UART485_BUF[loop + 3];	//֡ͷ��Ǻ����λ��ʾ֡�˳���
				if(!memcmp(frameCfmTail,&UART485_BUF[loop + corelen + 4],2)){	//֡βУ�飬֡ͷ+��ַһλ+����һλ+֡�˳�һλ+��һλ��Խ����ǰλ��/*������*/
				
					memset(COREDATS,0,COREDATS_SIZE * sizeof(u8));		//֡�˻�������
					memcpy(COREDATS,&UART485_BUF[loop + 4],corelen);	//֡ͷ�����λΪ֡��ͷ�ֽڣ��Դ�Ϊ���˸���֡�����ݵ�����
					memset(&UART485_BUF[loop],0,loop + corelen + 6);				//��ʷ���ݷ�������  6��֡ͷ3 + ֡�˳�1 + ֡β2
				}				
			}
		}
	}
}



