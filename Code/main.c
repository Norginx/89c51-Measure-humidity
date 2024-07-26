#include <reg52.h>
#include "lcd.h"
#include "uart.h"
#include "stdio.h"
#include "ds18b20.h"
#include "key.h"
#include <string.h>
char str[60];
unsigned char connection;
unsigned char TIME_BASE;
unsigned short COUNT;
unsigned char send_flag=0;
unsigned short count_sned;
float temperature[3];
unsigned char page=0;
unsigned char auto_enable=0;
float RH;
unsigned char rec_flag = 0;
void send_string(unsigned char *s)
{
  while(*s) //当字符串s不为空时
  {
    SBUF = *s; //将字符串s中的字符发送到串口
    while(TI == 0); //等待上一个字符发送完成
    TI = 0; //清除发送完成标志位TI
    s++; //指向下一个字符
  }

}
void connect_to_mqtt()
{
    sprintf(str,"process:0%%      ");
    LCD_ShowString(1,1,str);
    sprintf(str,"                ");
    LCD_ShowString(2,1,str);
    ES=0;
    send_string("AT+RST\r\n");
    rec_flag=1;
    TIME_BASE=0;
    while(rec_flag);
    delay_ms(200);
    
    sprintf(str,"process15%%      ");
    LCD_ShowString(1,1,str);
    sprintf(str,"                ");
    LCD_ShowString(2,1,str);
    send_string("AT+CWMODE=1\r\n");
    rec_flag=1;
    TIME_BASE=0;
    while(rec_flag);
    delay_ms(200);
    
    sprintf(str,"process:50%%      ");
    LCD_ShowString(1,1,str);
    sprintf(str,"                ");
    LCD_ShowString(2,1,str);
    send_string("AT+CWJAP=\"norgin\",\"12345678\"\r\n");
    rec_flag=1;
    TIME_BASE=0;
    while(rec_flag);
    delay_ms(500);
    
    sprintf(str,"process:60%%      ");
    LCD_ShowString(1,1,str);
    sprintf(str,"                ");
    LCD_ShowString(2,1,str);
    send_string("AT+MQTTUSERCFG=0,1,\"myUser\",\"admin\",\"norgin2002\",0,0,\"\"\r\n");
    rec_flag=1;
    TIME_BASE=0;
    while(rec_flag);
    delay_ms(200);
    
    sprintf(str,"process:80%%      ");
    LCD_ShowString(1,1,str);
    sprintf(str,"                ");
    LCD_ShowString(2,1,str);
    send_string("AT+MQTTCONN=0,\"42.51.41.107\",1883,0\r\n");
    rec_flag=1;
    TIME_BASE=0;
    while(rec_flag);
    delay_ms(200);
    
    sprintf(str,"process:99%%      ");
    LCD_ShowString(1,1,str);
    sprintf(str,"                ");
    LCD_ShowString(2,1,str);
    send_string("AT+MQTTSUB=0,\"cmd\",0\r\n");
    rec_flag=1;
    TIME_BASE=0;
    while(rec_flag);
    delay_ms(200);
    ES=1;
	ET0 = 1;
	TR0 = 1;
    TR1=1;
}

void send_msg()
{
    memset(str, 0, sizeof(str));
    sprintf(str,"AT+MQTTPUB=0,\"data\",\"A%.1fB%.1fC%.1f\",0,0\r\n",RH,temperature[0],temperature[1]);
    send_string(str);
    //printf("0,0\n");
}


void main()
{
    unsigned char kvalue;
	LCD_Init();//用之前必须要初始化
	UART_Init();
    SystemInit();
    ds18b20_init();
    ds18b21_init();
    key_init();
	while(1)
    {
        kvalue = key_sacn();
        temperature[0] = ds18b20_get_temperature();
        temperature[1] = ds18b21_get_temperature();
        RH=(temperature[0]+temperature[1])*0.72f+40.6;
        switch(kvalue)
        {
            case 1:{page=!page;send_string("AT+MQTTPUB=0,\"data\",\"@\",0,0\r\n");connection=0;}break;
            case 2:connect_to_mqtt();break;
            case 3:send_flag=1; break;
        }
        if(page==0)
        {
            sprintf(str,"tempW:%.1f       ",temperature[0]);
            LCD_ShowString(1,1,str);
            sprintf(str,"tempD:%.1f       ",temperature[1]);
            LCD_ShowString(2,1,str);
        }
        else if(page==1)
        {
            sprintf(str,"RH:%.1f%%     ",RH);
            LCD_ShowString(1,1,str);
            if(connection==1)
            {
                sprintf(str,"Connected         ");
            }
            else
            {
                sprintf(str,"Disconnect        ");
            }
            LCD_ShowString(2,1,str);
        }
        if(send_flag==1)
        {
            send_flag=0;
            send_msg();
        }
            //delay_ms(10);
	}
}


void Timer0_IRQ(void) interrupt 1
{
	TH0 = (65536 - 45872) / 256;
	TL0 = (65536 - 45872) % 256;
	/* 进入中断50ms +1次 */
	TIME_BASE++;
    COUNT++;
	
	if(TIME_BASE == 40)	 //	20ms * 50ms = 刚好等于1000ms 也就是1秒
	{
        rec_flag=0;
        TIME_BASE=0;
        //printf("test\r\n");
	}
    if(COUNT==count_sned&&auto_enable==1)
    {
        COUNT=0;
        send_flag=1;
    }
    ET0 = 1;
	TR0 = 1;
    
}


void scon_isr() interrupt 4//中断服务函数，设置为串口中断
{
    rec_flag=0;
    if(SBUF=='D')
    {
        send_flag=1;
    }
    else if(SBUF=='F')
    {
        COUNT=0;
        count_sned=10;
    }
    else if(SBUF=='x')
    {
        COUNT=0;
        count_sned=20;
    }
    else if(SBUF=='w')
    {
        COUNT=0;
        count_sned=30;
    }
    else if(SBUF=='k')
    {
        auto_enable=1;
    }
    else if(SBUF=='s')
    {
        auto_enable=0;
    }
    else if(SBUF=='#')
    {
        connection=1;
    }
	RI=0;
}


