#include <reg52.h>
#define BAUD_RATE_TIMER_RELOAD ((65536 - 409) & 0xFFFF) 
void UART_Init()
{
	SCON=0X50;			//����Ϊ������ʽ1
	TMOD=0X20;			//���ü�����������ʽ2
	PCON=0X80;			//�����ʼӱ�
	TH1=0XF3;			//��������ʼֵ���ã�ע�Ⲩ������4800��
	TL1=0XF3;
	ES=1;				//�򿪽����ж�
	EA=1;				//�����ж�
	TR1=1;				//�򿪼�����
}

//void UART_SendByte(unsigned char Byte)
//{      
//	SBUF=Byte;
//	while(TI==0);
//	TI=0;
//}


void SystemInit(void)
{
	TMOD |= 0x01;
	TH0 = (65536 - 45872) / 256;	 // ��ʱ50ms  //����11.0592MHZ
	TL0 = (65536 - 45872) % 256;
	ET0 = 1;
	TR0 = 1;
	EA  = 1;
}
