#include <reg52.h>
#define BAUD_RATE_TIMER_RELOAD ((65536 - 409) & 0xFFFF) 
void UART_Init()
{
	SCON=0X50;			//设置为工作方式1
	TMOD=0X20;			//设置计数器工作方式2
	PCON=0X80;			//波特率加倍
	TH1=0XF3;			//计数器初始值设置，注意波特率是4800的
	TL1=0XF3;
	ES=1;				//打开接收中断
	EA=1;				//打开总中断
	TR1=1;				//打开计数器
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
	TH0 = (65536 - 45872) / 256;	 // 定时50ms  //晶振：11.0592MHZ
	TL0 = (65536 - 45872) % 256;
	ET0 = 1;
	TR0 = 1;
	EA  = 1;
}
