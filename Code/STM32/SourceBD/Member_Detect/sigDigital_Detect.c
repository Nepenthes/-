#include "dataManager.h"

extern osMessageQId MsgBoxID_SBD_DigDet;
extern ARM_DRIVER_USART Driver_USART1;		//�豸�����⴮��һ�豸����

osThreadId tid_sigDig_Det_Thread;
osThreadDef(sigDig_Det_Thread,osPriorityNormal,1,512);

void sigDig_Det_Init(void){
	
	;
}

void sigDig_Det_Thread(const void *argument){

	;
}

void sigDig_DetThread_Active(void){

	sigDig_Det_Init();
	tid_sigDig_Det_Thread = osThreadCreate(osThread(sigDig_Det_Thread),NULL);
}
