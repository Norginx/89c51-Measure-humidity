#include <INTRINS.H>
#include <reg52.h>
#include "key.h"

sbit key3 = P2^2;
sbit key1 = P2^1;
sbit key2 = P2^0;

void delay(unsigned short i)
{
    while(i--);
}

void key_init(void)
{
    key1 = 1;
    key2 = 1;
    key3 = 1;
}


unsigned char key_sacn(void)
{
    if(key1==0)
    {
        delay(1000);
        if(key1==0)
        {
            while(key1==0);
            return 1;
        }
    }
    else if(key2==0)
    {
        delay(1000);
        if(key2==0)
        {
            while(key2==0);
            return 2;
        }
    }
    else if(key3==0)
    {
        delay(1000);
        if(key3==0)
        {
            while(key3==0);
            return 3;
        }
    }
    return 0;
}