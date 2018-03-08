#include "dataManager.h"

osPoolId  memid_SourceBD_pool;								 
osPoolDef(SourceBD_pool, 32, SourceBD_MEAS);	// �ڴ�ض���

osMessageQId MsgBoxID_SBD_Relay;
osMessageQDef(SBD_MsgBox_Relay, 6, &SourceBD_MEAS);		// ��Ϣ���ж���
osMessageQId MsgBoxID_SBD_AnaDet;
osMessageQDef(SBD_MsgBox_AnaDet, 6, &SourceBD_MEAS);		// ��Ϣ���ж���
osMessageQId MsgBoxID_SBD_DigDet;
osMessageQDef(SBD_MsgBox_DigDet, 6, &SourceBD_MEAS);		// ��Ϣ���ж���
osMessageQId MsgBoxID_SBD_UTZigB;
osMessageQDef(SBD_MsgBox_UTZigB, 6, &SourceBD_MEAS);		// ��Ϣ���ж���
osMessageQId MsgBoxID_SBD_UT485;
osMessageQDef(SBD_MsgBox_UT485, 6, &SourceBD_MEAS);		// ��Ϣ���ж���

void osMsg_memPoolInit(void){

	static bool memInit_flg = false;

	if(!memInit_flg){

		memid_SourceBD_pool	= osPoolCreate(osPool(SourceBD_pool));	//�����ڴ��
		
		MsgBoxID_SBD_Relay 	= osMessageCreate(osMessageQ(SBD_MsgBox_Relay), NULL);    //������Ϣ����
		MsgBoxID_SBD_AnaDet = osMessageCreate(osMessageQ(SBD_MsgBox_AnaDet), NULL);   //������Ϣ����
		MsgBoxID_SBD_DigDet = osMessageCreate(osMessageQ(SBD_MsgBox_DigDet), NULL);   //������Ϣ����
		MsgBoxID_SBD_UTZigB = osMessageCreate(osMessageQ(SBD_MsgBox_UTZigB), NULL);   //������Ϣ����
		MsgBoxID_SBD_UT485 	= osMessageCreate(osMessageQ(SBD_MsgBox_UT485), NULL);    //������Ϣ����

		memInit_flg = true;
	}
}
