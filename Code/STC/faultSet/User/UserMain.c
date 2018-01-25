#include <Includes.H>

u8 BD_addr,BD_type;	//��ʰ������ַ������

void mainFunActive(void){
#if(DEBUG == ENABLE)
const u8 LOGSIZE = 60;
xdata	u8 Disp_log[60] = {0};
xdata	u8 temp_log[10] = {0};
xdata	u8 loop;
#endif

#if(FUN_BOOTUP == MODE_A) 		//������Ϊ����������
	static u8 relayControlDats[RELAYDATS_SIZE];
#if(RELAYDATS_CORRECT == 1)
	u8 temp[RELAYDATS_SIZE];
#endif
	BD_addr = Localaddr_get();		//����ַ
	BD_type = LocalBDType_get();	//������

	memset(Disp_log,0,LOGSIZE * sizeof(u8));
	sprintf(Disp_log,"��ʰ�����Ѷ�ȡ");
	Debug_log(Disp_log);

	Beep_time(200,100);		//������ʾ
	Beep_time(200,100);
	
	while(1){
		
		frameInsAccept();		//��������
	
		if(!memcmp(relayControlDats,COREDATS,RELAYDATS_SIZE)){	//�Ƿ�Ϊ������
		
			memcpy(relayControlDats,COREDATS,RELAYDATS_SIZE);	//��������

			switch(BD_type){
			
				case BOARD_typeA:		
#if(RELAYDATS_CORRECT == 1)		//�Ƿ���Ҫ���е������
							BytesReverse(relayControlDats,temp,RELAYDATS_SIZE);	
							Frame595Send(temp,RELAYDATS_SIZE);		//��ʰ�ִ�е��������ĸ�������
#endif			
							Frame595Send(relayControlDats,RELAYDATS_SIZE);	//��ʰ�ִ�и�������
#if(DEBUG == ENABLE)
							memset(Disp_log,0,LOGSIZE * sizeof(u8));
							sprintf(Disp_log,"��ʰ������Ѹ��£�");
							for(loop = 0;loop < RELAYDATS_SIZE;loop ++){
								
								sprintf(temp_log,"%02X",relayControlDats[loop]);
								strcat(Disp_log,temp_log);
							}
							Debug_log(Disp_log);
#endif
							Beep_time(80,40);		//������ʾ
							Beep_time(80,40);
							
							Frame595Clr(RELAYDATS_SIZE);	//��ʰ��������
							break;
							
				case BOARD_typeB:	break;

				case BOARD_typeC:	break;
							
							default: break;
			}
		}
	}
#elif(FUN_BOOTUP == MODE_B)	//������Ϊ��д����ʰ�����
	u8 relayBoradType = BOARD_TYPE;
	
	EEPROM_SectorErase(0);
	delay_ms(20);
	EEPROM_write_n(0,&relayBoradType,1);
	delay_ms(20);
	
	Beep_time(200,100);		//���͸�����ʾ
	Beep_time(200,100);
#endif
}

int main(void){
	
	Bsp_Init();
	
	mainFunActive();
	
	return 0;
}