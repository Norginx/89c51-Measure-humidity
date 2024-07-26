#include <INTRINS.H>
#include "ds18b20.h"
#include <reg52.h>
sbit DS18B20_PORT=P3^7;
sbit DS18B21_PORT=P3^6;
 
// ��ʱn����
void delay_ms(unsigned int n)
{
    unsigned int i=0,j=0;
    for(i=0;i<n;i++)
        for(j=0;j<123;j++);
}


// ��ʱ10*n΢��
void delay_10us(unsigned int n)
{
    while(n--);
}


/**
* @brief  DS18B20��ʼ��
*/
void ds18b20_reset(void){
	// ����DQ
	DS18B20_PORT=0;	
	// ����750us
	delay_10us(75);
	// �ͷ�DQ
	DS18B20_PORT=1;
	// �ȴ�20us
	delay_10us(2);
}
/**
* @brief DS18B20��Ӧ�źţ������ж�DS18B20�Ƿ����
* @return 0:�ɹ� 1:ʧ��
*/
unsigned char ds18b20_ack(void){
    unsigned char timeout = 0;
    // ��ȡ��Ӧ�źţ�60us���ҵĵ͵�ƽ�ź�
    while(DS18B20_PORT && timeout <20){
        timeout ++;
        delay_10us(1);
    }
    if(timeout >= 20){
        // ��ʱ��
        return 1;
    }else{
        timeout = 0;
    }
    // �ͷ����ţ����Żᱻ���ߣ�240us���ҵĸߵ�ƽ�ź�
		while((!DS18B20_PORT)&&timeout<20)	//�ȴ�DQΪ�ߵ�ƽ
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
* @brief DS18B20��ʼ��
* @return 0:�ɹ� 1:ʧ��
*/
unsigned char ds18b20_init(void){
    ds18b20_reset();
    return ds18b20_ack();
}
/**
* @brief DS18B20дһ���ֽ�
* @return д�ֽ�
*/
void ds18b20_write_byte(unsigned char dat){
	unsigned char i=0;
	unsigned char temp=0;

	for(i=0;i<8;i++)//ѭ��8�Σ�ÿ��дһλ������д��λ��д��λ
	{
		temp=dat&0x01;//ѡ���λ׼��д��
		dat>>=1;//���θ�λ�Ƶ���λ
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
* @brief DS18B20��ȡһ��bit
* @return ��ȡ����bit
*/
unsigned char ds18b20_read_bit(void){
	unsigned char dat=0;
	
	DS18B20_PORT=0;
	_nop_();_nop_();
	DS18B20_PORT=1;	
	_nop_();_nop_(); //�ö�ʱ�䲻�ܹ�����������15us�ڶ�ȡ����
	if(DS18B20_PORT)dat=1;	//���������Ϊ1������datΪ1������Ϊ0
	else dat=0;
	delay_10us(5);
	return dat;
}
/**
* @brief DS18B20��ȡһ���ֽ�
* @return ��ȡ�����ֽ�
*/
unsigned char ds18b20_read_byte(void){
	unsigned char i=0;
	unsigned char dat=0;
	unsigned char temp=0;

	for(i=0;i<8;i++)//ѭ��8�Σ�ÿ�ζ�ȡһλ�����ȶ���λ�ٶ���λ
	{
		temp=ds18b20_read_bit();
		dat=(temp<<7)|(dat>>1);
	}
	return dat;	
}
/**
* @brief DS18B20��ʼת��
*/
void ds18b20_start(void){
    ds18b20_reset();
    ds18b20_ack();
    ds18b20_write_byte(0xcc);
    ds18b20_write_byte(0x44);
}
/**
* @brief DS18B20��ȡ�¶�
* @return �¶�ֵ
*/
float ds18b20_get_temperature(void){
	float temp;
	unsigned char tempH=0;
	unsigned char tempL=0;
	unsigned short value=0;

	// ��ʼת��
	ds18b20_start();
	// ��λ
	ds18b20_reset();
	// �ȴ�DS18B20����Ӧ
	ds18b20_ack();
	// ����SKIP ROM����
	ds18b20_write_byte(0xcc);
	// ���Ͷ��洢������
    ds18b20_write_byte(0xbe);

	// ��ȡ�¶�ֵ���ֽ�
	tempL = ds18b20_read_byte();
	// ��ȡ�¶�ֵ���ֽ�
	tempH = ds18b20_read_byte();
	// �����¶�ֵ
	value = (tempH<<8) + tempL;

	// �¶�ֵת�������¶��ǲ���
	if((value&0xf800)==0xf800)//�жϷ���λ�����¶�
	{
		// ȡ���ټ�1
		value=(~value)+1; 
		temp=value*(-0.0625);
	}
	else 
	{
		// ���¶�
		temp=value*0.0625;	
	}
	return temp;
}































/**
* @brief  DS18B20��ʼ��
*/
void ds18b21_reset(void){
	// ����DQ
	DS18B21_PORT=0;	
	// ����750us
	delay_10us(75);
	// �ͷ�DQ
	DS18B21_PORT=1;
	// �ȴ�20us
	delay_10us(2);
}
/**
* @brief DS18B20��Ӧ�źţ������ж�DS18B20�Ƿ����
* @return 0:�ɹ� 1:ʧ��
*/
unsigned char ds18b21_ack(void){
    unsigned char timeout = 0;
    // ��ȡ��Ӧ�źţ�60us���ҵĵ͵�ƽ�ź�
    while(DS18B21_PORT && timeout <20){
        timeout ++;
        delay_10us(1);
    }
    if(timeout >= 20){
        // ��ʱ��
        return 1;
    }else{
        timeout = 0;
    }
    // �ͷ����ţ����Żᱻ���ߣ�240us���ҵĸߵ�ƽ�ź�
		while((!DS18B21_PORT)&&timeout<20)	//�ȴ�DQΪ�ߵ�ƽ
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
* @brief DS18B20��ʼ��
* @return 0:�ɹ� 1:ʧ��
*/
unsigned char ds18b21_init(void){
    ds18b21_reset();
    return ds18b21_ack();
}
/**
* @brief DS18B20дһ���ֽ�
* @return д�ֽ�
*/
void ds18b21_write_byte(unsigned char dat){
	unsigned char i=0;
	unsigned char temp=0;

	for(i=0;i<8;i++)//ѭ��8�Σ�ÿ��дһλ������д��λ��д��λ
	{
		temp=dat&0x01;//ѡ���λ׼��д��
		dat>>=1;//���θ�λ�Ƶ���λ
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
* @brief DS18B20��ȡһ��bit
* @return ��ȡ����bit
*/
unsigned char ds18b21_read_bit(void){
	unsigned char dat=0;
	
	DS18B21_PORT=0;
	_nop_();_nop_();
	DS18B21_PORT=1;	
	_nop_();_nop_(); //�ö�ʱ�䲻�ܹ�����������15us�ڶ�ȡ����
	if(DS18B21_PORT)dat=1;	//���������Ϊ1������datΪ1������Ϊ0
	else dat=0;
	delay_10us(5);
	return dat;
}
/**
* @brief DS18B20��ȡһ���ֽ�
* @return ��ȡ�����ֽ�
*/
unsigned char ds18b21_read_byte(void){
	unsigned char i=0;
	unsigned char dat=0;
	unsigned char temp=0;

	for(i=0;i<8;i++)//ѭ��8�Σ�ÿ�ζ�ȡһλ�����ȶ���λ�ٶ���λ
	{
		temp=ds18b21_read_bit();
		dat=(temp<<7)|(dat>>1);
	}
	return dat;	
}
/**
* @brief DS18B20��ʼת��
*/
void ds18b21_start(void){
    ds18b21_reset();
    ds18b21_ack();
    ds18b21_write_byte(0xcc);
    ds18b21_write_byte(0x44);
}
/**
* @brief DS18B20��ȡ�¶�
* @return �¶�ֵ
*/
float ds18b21_get_temperature(void){
	float temp;
	unsigned char tempH=0;
	unsigned char tempL=0;
	unsigned short value=0;

	// ��ʼת��
	ds18b21_start();
	// ��λ
	ds18b21_reset();
	// �ȴ�DS18B20����Ӧ
	ds18b21_ack();
	// ����SKIP ROM����
	ds18b21_write_byte(0xcc);
	// ���Ͷ��洢������
    ds18b21_write_byte(0xbe);

	// ��ȡ�¶�ֵ���ֽ�
	tempL = ds18b21_read_byte();
	// ��ȡ�¶�ֵ���ֽ�
	tempH = ds18b21_read_byte();
	// �����¶�ֵ
	value = (tempH<<8) + tempL;

	// �¶�ֵת�������¶��ǲ���
	if((value&0xf800)==0xf800)//�жϷ���λ�����¶�
	{
		// ȡ���ټ�1
		value=(~value)+1; 
		temp=value*(-0.0625);
	}
	else 
	{
		// ���¶�
		temp=value*0.0625;	
	}
	return temp;
}








