#include "sys.h"
#include "usart3.h"	  
#include "includes.h"

u8 uart3_rx_buf[UART_PACKAGE_MAX+1][UART_RX_MAX_LEN+1];
u8 uart3_package_ptr;
u8 uart3_rx_ptr;


// RFID
void uart3_init(u32 bound)
{
	u8 i;
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	

	USART_DeInit(USART3);  
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(GPIOB, &GPIO_InitStructure); 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);  


	//USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

	USART_Init(USART3, &USART_InitStructure); //��ʼ������
	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���

	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�����ж�
	USART_Cmd(USART3, ENABLE);                    //ʹ�ܴ��� 
	
	uart3_package_ptr = 0;
	uart3_rx_ptr = 0;
	for(i=0; i<UART_PACKAGE_MAX; i++)
	{
		uart3_rx_buf[i][UART_RX_MAX_LEN] = 0x00;
	}

}

//RFID�ϴ���ʽ 0x55 0xAA 0x21 0x04 ID0 ID1 ID2 ID3 crc1 crc2
void USART3_IRQHandler(void)                
{
	u8 rxtemp;
	u8 crc1,crc2;
	u16 crc;
//	static u16 last_crc=0;
	
	OSIntEnter();
	
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) 
	{
		rxtemp =USART_ReceiveData(USART3);	
		
		if(uart3_rx_ptr==0)
		{
			if(rxtemp == 0x55)      // ��������55  AA    
			{
				uart3_rx_buf[uart3_package_ptr][uart3_rx_ptr++] = rxtemp;  
			}
		}
		else if(uart3_rx_ptr==1)
		{
			if((uart3_rx_buf[uart3_package_ptr][0] == 0x55)&&(rxtemp == 0xAA))
			{
				uart3_rx_buf[uart3_package_ptr][uart3_rx_ptr++] = rxtemp;   
			}
			else
			{
				uart3_rx_ptr = 0;
			} 
		}
		else
		{
			uart3_rx_buf[uart3_package_ptr][uart3_rx_ptr++] = rxtemp; 
			
			if(uart3_rx_ptr == 10)
			{
				crc = Crc16Code(&uart3_rx_buf[uart3_package_ptr][2],6);  // crc       
	                    crc1 =  crc;
	                    crc2 =  crc>>8;
	                    if((crc1 == uart3_rx_buf[uart3_package_ptr][uart3_rx_ptr-2]) &&(crc2 == uart3_rx_buf[uart3_package_ptr][uart3_rx_ptr-1]))
	                    {
	                    	//if(last_crc !=crc)	// ���յ���ͬ�Ŀ���ֻ����һ��
	                    	{
						//last_crc = crc;
						uart3_rx_buf[uart3_package_ptr][UART_RX_MAX_LEN] = uart3_rx_ptr;
						uart3_package_ptr++;
					}	                     	
				} 
				uart3_rx_ptr = 0;
			}           							
		}

		if(uart3_rx_ptr > UART_RX_MAX_LEN)uart3_rx_ptr = 0;
		if(uart3_package_ptr > UART_PACKAGE_MAX)uart3_package_ptr=0;	
	} 
	OSIntExit();
}


void uart3_send_buf(u8 * string, u16 len)
{
	u16 i;
	
	for(i=0;i<len;i++)
	{
		while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);
		USART_SendData(USART3, *string++);	
	}
}





