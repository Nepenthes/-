#include "sys.h"
#include "usart2.h"	  
#include "includes.h"

u8 uart2_rx_buf[UART_PACKAGE_MAX+1][UART_RX_MAX_LEN+1];
u8 uart2_package_ptr;
u8 uart2_rx_ptr;


// wifi/zigbee
void uart2_init(u32 bound)
{
	u8 i;
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	

	USART_DeInit(USART2);  
	
	//USART1_TX   PA.2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��PA2

	//USART1_RX	  PA.3
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);  //��ʼ��PA3


	//USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

	USART_Init(USART2, &USART_InitStructure); //��ʼ������
	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���

	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�����ж�
	USART_Cmd(USART2, ENABLE);                    //ʹ�ܴ��� 
	

	uart2_package_ptr = 0;
	uart2_rx_ptr = 0;
	for(i=0; i<UART_PACKAGE_MAX; i++)
	{
		uart2_rx_buf[i][UART_RX_MAX_LEN] = 0x00;
	}	
}



void USART2_IRQHandler(void)                
{
	u8 rxtemp;
	u8 crc1,crc2;
	u16 crc;
	
	
	OSIntEnter();
	
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) 
	{
		rxtemp =USART_ReceiveData(USART2);	
		
		if(uart2_rx_ptr==0)
		{
			if(rxtemp == 0xAA)      // PC��������aa  55   
			{
				uart2_rx_buf[uart2_package_ptr][uart2_rx_ptr++] = rxtemp;  
			}
		}
		else if(uart2_rx_ptr==1)
		{
			if((uart2_rx_buf[uart2_package_ptr][0] == 0xAA)&&(rxtemp == 0x55))
			{
				uart2_rx_buf[uart2_package_ptr][uart2_rx_ptr++] = rxtemp;   
			}
			else
			{
				uart2_rx_ptr = 0;
			} 
		}
		else
		{
			uart2_rx_buf[uart2_package_ptr][uart2_rx_ptr++] = rxtemp; 
						
			if(uart2_rx_ptr >= uart2_rx_buf[uart2_package_ptr][3]+6)
			{
				crc = Crc16Code(&uart2_rx_buf[uart2_package_ptr][2],uart2_rx_buf[uart2_package_ptr][3]+2);  // crc       
	                    	crc1 =  crc;
	                    	crc2 =  crc>>8;
	                     	if((crc1 == uart2_rx_buf[uart2_package_ptr][uart2_rx_ptr-2]) &&(crc2 == uart2_rx_buf[uart2_package_ptr][uart2_rx_ptr-1]))
	                     	{
	                     		uart2_rx_buf[uart2_package_ptr][UART_RX_MAX_LEN] = uart2_rx_ptr;
					uart2_package_ptr++;
				} 
				uart2_rx_ptr = 0;
			}           							
		}

		if(uart2_rx_ptr > UART_RX_MAX_LEN)uart2_rx_ptr = 0;
		if(uart2_package_ptr > UART_PACKAGE_MAX)uart2_package_ptr=0;	
	} 

	OSIntExit();
}


void uart2_send_buf(u8 * string, u16 len)
{
	u16 i;
	
	for(i=0;i<len;i++)
	{
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
		USART_SendData(USART2, *string++);	
	}
}




