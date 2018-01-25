#include "dataManager.h"

osPoolId  memid_SourceBD_pool;								 
osPoolDef(SourceBD_pool, 10, SourceBD_MEAS);                  // �ڴ�ض���
osMessageQId  msgid_MsgBox_SourceBD;
osMessageQDef(MsgBox_SourceBD, 8, &SourceBD_MEAS);            // ��Ϣ���ж���

void osMsg_memPoolInit(void){

	static bool memInit_flg = false;

	if(!memInit_flg){

		memid_SourceBD_pool		= osPoolCreate(osPool(SourceBD_pool));	//�����ڴ��
		msgid_MsgBox_SourceBD 	= osMessageCreate(osMessageQ(MsgBox_SourceBD), NULL);   //������Ϣ����

		memInit_flg = true;
	}
}
