/********************************* �����к�̫�������޹�˾ *******************************
* ʵ �� �� ��wifi����ʵ��ʵ��
* ʵ��˵�� ���ڵ��Զ�����һ����������ʵ��wifiģ���Զ����ӣ���ͨ��ATָ���뵥Ƭ������
* ʵ��ƽ̨ ����̫ARM��Ƭ��������
* ���ӷ�ʽ ����ο�interface.h�ļ�
* ע    �� ��ESP8266.c���������͵Ĳ���������
* ��    �� ����̫���Ӳ�Ʒ�з���    QQ ��1909197536
* ��    �� ��http://shop120013844.taobao.com/
****************************************************************************************/

#include "stm32f10x.h"
#include "interface.h"
#include "uart.h"
#include "ESP8266.h"
#include "serialportAPI.h"
#include <stdlib.h>
#include <stdio.h>

#define HOST_NAME   "192.168.31.136"
#define HOST_PORT   8080

#define RECVBUF_SIZE 64

uint32_t tick_3s=0;
uint32_t tick_5ms=0;
uint32_t tick_500ms=0;
unsigned char buffer[RECVBUF_SIZE] = {0};

//ȫ�ֱ�������

int main(void)
{
	int len=0;
	UserLEDInit();
	USART1Conf(9600);
	DBG("system begin");
	TIM2_Init();

	while(1)
	{
		DBG("chang baud");
		delay(500);
		UartBegin(115200,&USART3Conf,&PutChar);
		SetBaud(19200);
		UartBegin(19200,&USART3Conf,&PutChar);//���Ĳ����ʵ�19200
		if(0 != SetBaud(19200))//���²������¼���Ƿ�ɹ�
		{
			break;//�ɹ����˳�
		}	
	}

	while(0 == WifiInitSmart(HOST_NAME,HOST_PORT));
	
	while(1)
	{	
		if(SerialAvailable() > 5)
		{
			len = recv(buffer, RECVBUF_SIZE, 100);
			if (len > 0)
			{
				tick_3s = 0;
				send(buffer, len);
			} 
		}
		if(tick_5ms >= 5)
		{
			tick_5ms = 0;
			tick_3s++;
			tick_500ms++;
			if(tick_500ms >= 100)
			{
				tick_500ms = 0;
				LEDToggle(LED_PIN);
			}
			if(tick_3s >= 600)
			{
				tick_3s = 0;
				//��ʱ��������Ƿ����������������,wifi��λ��������
				if (getSystemStatus() != STATUS_GETLINK)
				{
					DBG("TCP unlink");
					while(!WifiInitSmart(HOST_NAME,HOST_PORT));
				}else
				{
					DBG("TCP link tick");
				}			
			}			
		}

	}
}

