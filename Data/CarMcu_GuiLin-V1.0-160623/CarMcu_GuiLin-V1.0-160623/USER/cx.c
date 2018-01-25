#include "stm32f10x.h"
#include "includes.h"
#include "usart.h"	  
#include "delay.h"

extern u8 uart1Rx_timerout;
extern u16 DisableTrafficTimer;

u8 Traffic_red;
u8 TrafficOverTimer;
u8 TrafficCnt;
u16 speedOvertimer;
u16 discRanging;
u16 disValue;
u16 curSpeed;

void TrafficDetect(void);

/********************************************************************/
// ϵͳ��ʱ��
/********************************************************************/
void TIM3_Configuration(u16 arr,u16 psc)
{
    	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��

	TIM_DeInit(TIM3);

	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; 						//�Զ���װ�ؼĴ������ڵ�ֵ(��ʱʱ��)�ۼ�1��Ƶ�ʺ���������»����ж�
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 					//ʱ��Ԥ��Ƶ�� ���磺ʱ��Ƶ��=72/(ʱ��Ԥ��Ƶ+1) 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 		//������Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���

	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx					 
}

void TIM3_IRQHandler(void)   //TIM3�ж� 5 MS
{
 	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  ); 
		if(uart1Rx_timerout)uart1Rx_timerout--;	
		TrafficDetect();	
	}
}


/********************************************************************/
// ���ټ��
/********************************************************************/
void Tim2_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TIM_TimeBaseStructure.TIM_Period = 65500;
	TIM_TimeBaseStructure.TIM_Prescaler = 71;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	//TIM_Cmd(TIM2, DISABLE);
	TIM_Cmd(TIM2, ENABLE);
}

void SpeedDet_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;	
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource7);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 		 
	GPIO_Init(GPIOA, &GPIO_InitStructure);		
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line7;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure); 
			 						 
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		 			
	NVIC_Init(&NVIC_InitStructure);	

	Tim2_Init();
}


void EXTI9_5_IRQHandler(void)
{
	u16 speedCnt;
	
	OSIntEnter();
	
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7))
    	{
        	TIM_SetCounter(TIM2,0);
		speedOvertimer = 0;
    	}	
    	else
    	{
        	speedCnt = TIM_GetCounter(TIM2);
		if(speedCnt)   curSpeed = 100000/speedCnt*3;		
		speedOvertimer = 0;
    	}   
	
    	EXTI_ClearITPendingBit(EXTI_Line7); 
    	OSIntExit();
} 

/********************************************************************/
// ���̵Ƽ��
/********************************************************************/
void Tim4_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	TIM_TimeBaseStructure.TIM_Period = 65500;
	TIM_TimeBaseStructure.TIM_Prescaler = 71;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

	TIM_Cmd(TIM4, ENABLE);
}

void Traffic_PA1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE);
												
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;       
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource1); 
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line1;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure); 
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	Tim4_Init();
	
}

void TrafficDetect(void)
{
	if(DisableTrafficTimer)DisableTrafficTimer--;
	
	TrafficOverTimer++;
	if(TrafficOverTimer > (500/5))	// 300ms ��ⲻ�����
	{
		TrafficOverTimer = 0;
		TrafficCnt = 0;
		Traffic_red = 0;
	}
}

void EXTI1_IRQHandler(void)
{
	u16 count1;
	
	OSIntEnter();
	
	if(!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1))
	{
	     TIM_SetCounter(TIM4,0);
	}	
	else
	{
		count1 = TIM_GetCounter(TIM4);
		
		if(count1 <= 1200 && count1 >= 800 ) 	// ���
		{
			TrafficCnt++;
			if(TrafficCnt>10)
			{
				TrafficCnt = 0;
				TrafficOverTimer = 0;
				Traffic_red = 1; 
			}
		}
		
		/*
		else if(count1 <= 3500 && count1 > 1500)   // �̵�
		{  
			TrafficCnt++;
			if(TrafficCnt>10)
			{
				TrafficCnt = 0;
				Traffic_red = 0; 
				TrafficOverTimer = 0;
			}
		}
		*/
	}
	
   	EXTI_ClearITPendingBit(EXTI_Line1); 	
    	OSIntExit();
} 

/********************************************************************/
// ���������
/********************************************************************/

void Tim5_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	TIM_TimeBaseStructure.TIM_Period = 65000;
	TIM_TimeBaseStructure.TIM_Prescaler = 71;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);

	TIM_Cmd(TIM5, ENABLE);
}

void Ultra_PC0_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;       
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
 	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOC,GPIO_Pin_1);
										
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;       
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	 
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource0); 
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure); 
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	Tim5_Init();
}

// �������жϺ���
void EXTI0_IRQHandler(void)
{
	OSIntEnter();
	
	if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_0))
	{
	     TIM_SetCounter(TIM5,0);
	}	
	else
	{
		disValue = TIM_GetCounter(TIM5)/6;
	}
	
	EXTI_ClearITPendingBit(EXTI_Line0); 
   	OSIntExit();
}

void Ultra_Ranging(void)
{
	u8 i;
	u32 Ultr_Temp = 0;	

	disValue = 0;
	for(i=0;i<5;i++)
	{
		GPIO_SetBits(GPIOC,GPIO_Pin_1);	
		delay_us(10);
		GPIO_ResetBits(GPIOC,GPIO_Pin_1);
		OSTimeDlyHMSM(0,0,0,60);  
		Ultr_Temp += disValue;
	}
	discRanging = Ultr_Temp/5;
}





