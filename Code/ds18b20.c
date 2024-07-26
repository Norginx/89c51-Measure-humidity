#include <INTRINS.H>
#include "ds18b20.h"
#include <reg52.h>
sbit DS18B20_PORT=P3^7;
sbit DS18B21_PORT=P3^6;
 
// 延时n毫秒
void delay_ms(unsigned int n)
{
    unsigned int i=0,j=0;
    for(i=0;i<n;i++)
        for(j=0;j<123;j++);
}


// 延时10*n微秒
void delay_10us(unsigned int n)
{
    while(n--);
}


/**
* @brief  DS18B20初始化
*/
void ds18b20_reset(void){
	// 拉低DQ
	DS18B20_PORT=0;	
	// 持续750us
	delay_10us(75);
	// 释放DQ
	DS18B20_PORT=1;
	// 等待20us
	delay_10us(2);
}
/**
* @brief DS18B20响应信号，用来判断DS18B20是否存在
* @return 0:成功 1:失败
*/
unsigned char ds18b20_ack(void){
    unsigned char timeout = 0;
    // 读取响应信号，60us左右的低电平信号
    while(DS18B20_PORT && timeout <20){
        timeout ++;
        delay_10us(1);
    }
    if(timeout >= 20){
        // 超时了
        return 1;
    }else{
        timeout = 0;
    }
    // 释放引脚，引脚会被拉高，240us左右的高电平信号
		while((!DS18B20_PORT)&&timeout<20)	//等待DQ为高电平
		{
			timeout++;
			delay_10us(1);
		}
    if(timeout >= 20){
        return 1;
    }
    return 0;
}
/**
* @brief DS18B20初始化
* @return 0:成功 1:失败
*/
unsigned char ds18b20_init(void){
    ds18b20_reset();
    return ds18b20_ack();
}
/**
* @brief DS18B20写一个字节
* @return 写字节
*/
void ds18b20_write_byte(unsigned char dat){
	unsigned char i=0;
	unsigned char temp=0;

	for(i=0;i<8;i++)//循环8次，每次写一位，且先写低位再写高位
	{
		temp=dat&0x01;//选择低位准备写入
		dat>>=1;//将次高位移到低位
		if(temp)
		{
			DS18B20_PORT=0;
			_nop_();_nop_();
			DS18B20_PORT=1;	
			delay_10us(6);
		}
		else
		{
			DS18B20_PORT=0;
			delay_10us(6);
			DS18B20_PORT=1;
			_nop_();_nop_();	
		}	
	}	
}
/**
* @brief DS18B20读取一个bit
* @return 读取到的bit
*/
unsigned char ds18b20_read_bit(void){
	unsigned char dat=0;
	
	DS18B20_PORT=0;
	_nop_();_nop_();
	DS18B20_PORT=1;	
	_nop_();_nop_(); //该段时间不能过长，必须在15us内读取数据
	if(DS18B20_PORT)dat=1;	//如果总线上为1则数据dat为1，否则为0
	else dat=0;
	delay_10us(5);
	return dat;
}
/**
* @brief DS18B20读取一个字节
* @return 读取到的字节
*/
unsigned char ds18b20_read_byte(void){
	unsigned char i=0;
	unsigned char dat=0;
	unsigned char temp=0;

	for(i=0;i<8;i++)//循环8次，每次读取一位，且先读低位再读高位
	{
		temp=ds18b20_read_bit();
		dat=(temp<<7)|(dat>>1);
	}
	return dat;	
}
/**
* @brief DS18B20开始转换
*/
void ds18b20_start(void){
    ds18b20_reset();
    ds18b20_ack();
    ds18b20_write_byte(0xcc);
    ds18b20_write_byte(0x44);
}
/**
* @brief DS18B20读取温度
* @return 温度值
*/
float ds18b20_get_temperature(void){
	float temp;
	unsigned char tempH=0;
	unsigned char tempL=0;
	unsigned short value=0;

	// 开始转换
	ds18b20_start();
	// 复位
	ds18b20_reset();
	// 等待DS18B20的响应
	ds18b20_ack();
	// 发送SKIP ROM命令
	ds18b20_write_byte(0xcc);
	// 发送读存储器命令
    ds18b20_write_byte(0xbe);

	// 读取温度值低字节
	tempL = ds18b20_read_byte();
	// 读取温度值高字节
	tempH = ds18b20_read_byte();
	// 计算温度值
	value = (tempH<<8) + tempL;

	// 温度值转换，负温度是补码
	if((value&0xf800)==0xf800)//判断符号位，负温度
	{
		// 取反再加1
		value=(~value)+1; 
		temp=value*(-0.0625);
	}
	else 
	{
		// 正温度
		temp=value*0.0625;	
	}
	return temp;
}































/**
* @brief  DS18B20初始化
*/
void ds18b21_reset(void){
	// 拉低DQ
	DS18B21_PORT=0;	
	// 持续750us
	delay_10us(75);
	// 释放DQ
	DS18B21_PORT=1;
	// 等待20us
	delay_10us(2);
}
/**
* @brief DS18B20响应信号，用来判断DS18B20是否存在
* @return 0:成功 1:失败
*/
unsigned char ds18b21_ack(void){
    unsigned char timeout = 0;
    // 读取响应信号，60us左右的低电平信号
    while(DS18B21_PORT && timeout <20){
        timeout ++;
        delay_10us(1);
    }
    if(timeout >= 20){
        // 超时了
        return 1;
    }else{
        timeout = 0;
    }
    // 释放引脚，引脚会被拉高，240us左右的高电平信号
		while((!DS18B21_PORT)&&timeout<20)	//等待DQ为高电平
		{
			timeout++;
			delay_10us(1);
		}
    if(timeout >= 20){
        return 1;
    }
    return 0;
}
/**
* @brief DS18B20初始化
* @return 0:成功 1:失败
*/
unsigned char ds18b21_init(void){
    ds18b21_reset();
    return ds18b21_ack();
}
/**
* @brief DS18B20写一个字节
* @return 写字节
*/
void ds18b21_write_byte(unsigned char dat){
	unsigned char i=0;
	unsigned char temp=0;

	for(i=0;i<8;i++)//循环8次，每次写一位，且先写低位再写高位
	{
		temp=dat&0x01;//选择低位准备写入
		dat>>=1;//将次高位移到低位
		if(temp)
		{
			DS18B21_PORT=0;
			_nop_();_nop_();
			DS18B21_PORT=1;	
			delay_10us(6);
		}
		else
		{
			DS18B21_PORT=0;
			delay_10us(6);
			DS18B21_PORT=1;
			_nop_();_nop_();	
		}	
	}	
}
/**
* @brief DS18B20读取一个bit
* @return 读取到的bit
*/
unsigned char ds18b21_read_bit(void){
	unsigned char dat=0;
	
	DS18B21_PORT=0;
	_nop_();_nop_();
	DS18B21_PORT=1;	
	_nop_();_nop_(); //该段时间不能过长，必须在15us内读取数据
	if(DS18B21_PORT)dat=1;	//如果总线上为1则数据dat为1，否则为0
	else dat=0;
	delay_10us(5);
	return dat;
}
/**
* @brief DS18B20读取一个字节
* @return 读取到的字节
*/
unsigned char ds18b21_read_byte(void){
	unsigned char i=0;
	unsigned char dat=0;
	unsigned char temp=0;

	for(i=0;i<8;i++)//循环8次，每次读取一位，且先读低位再读高位
	{
		temp=ds18b21_read_bit();
		dat=(temp<<7)|(dat>>1);
	}
	return dat;	
}
/**
* @brief DS18B20开始转换
*/
void ds18b21_start(void){
    ds18b21_reset();
    ds18b21_ack();
    ds18b21_write_byte(0xcc);
    ds18b21_write_byte(0x44);
}
/**
* @brief DS18B20读取温度
* @return 温度值
*/
float ds18b21_get_temperature(void){
	float temp;
	unsigned char tempH=0;
	unsigned char tempL=0;
	unsigned short value=0;

	// 开始转换
	ds18b21_start();
	// 复位
	ds18b21_reset();
	// 等待DS18B20的响应
	ds18b21_ack();
	// 发送SKIP ROM命令
	ds18b21_write_byte(0xcc);
	// 发送读存储器命令
    ds18b21_write_byte(0xbe);

	// 读取温度值低字节
	tempL = ds18b21_read_byte();
	// 读取温度值高字节
	tempH = ds18b21_read_byte();
	// 计算温度值
	value = (tempH<<8) + tempL;

	// 温度值转换，负温度是补码
	if((value&0xf800)==0xf800)//判断符号位，负温度
	{
		// 取反再加1
		value=(~value)+1; 
		temp=value*(-0.0625);
	}
	else 
	{
		// 正温度
		temp=value*0.0625;	
	}
	return temp;
}








