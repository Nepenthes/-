#ifndef		__CONFIG_H_
#define		__CONFIG_H_

#define		MODE_A	'A'  //��������ģʽ
#define 	MODE_B	'B'  //������ʰ�̵�����������ģʽ

#define		BOARD_typeA		0x11
#define		BOARD_typeB		0x22
#define		BOARD_typeC		0x33

/*********************************************************//*������������*/
#define		FUN_BOOTUP	 	MODE_A								 //����ģʽ
#define		BOARD_TYPE		BOARD_typeA							 //����ģʽʱ����ʰ��������ã�����ģʽΪBʱʹ�ã�

#define		DEBUG				ENABLE							 //�Ƿ������Դ������

#define		RELAYDATS_SIZE		16								 //�̵��������ֽڳ���

#define		RELAYDATS_CORRECT	ENABLE						 	 //595�����Ƿ���Ҫ�������
/*********************************************************/

//#define MAIN_Fosc		22118400L	//������ʱ��
//#define MAIN_Fosc		12000000L	//������ʱ��
#define MAIN_Fosc		11059200L	//������ʱ��
//#define MAIN_Fosc		 5529600L	//������ʱ��
//#define MAIN_Fosc		24000000L	//������ʱ��


/*********************************************************/

#include	"STC15Fxxxx.H"

/**************************************************************************/

#define Main_Fosc_KHZ	(MAIN_Fosc / 1000)

/***********************************************************/

#endif
