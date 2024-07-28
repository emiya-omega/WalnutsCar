#include "headfile.h"

#define PCLK1	36000000  //����USART1�⣬���മ�ڶ�����APB1�����ϣ�ʹ��PCLK1��Ƶ ʱ��Ƶ��Ϊ36MHZ
#define PCLK2 72000000  //USART1����APB2�����ϣ�ʹ��PCLK2��Ƶ ʱ��Ƶ��Ϊ72MHZ 

USART_TypeDef *uart_index[3] = { USART1 , USART2 , USART3 };

//-------------------------------------------------------------------------------------------------------------------
//���´�����Ӻ� ����ʹ��printf���� ��Ҫ��ʼ������UART_1
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef�� d in stdio.h. */ 
FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc����
//printf�������ָ��fputc����fputc���������
//����ʹ�ô���1(USART1)���printf��Ϣ
int fputc(int ch, FILE *f)
{
	while((USART1->SR&0x40)==0);  //�ȴ��������
	USART1->DR = (uint8_t) ch;  
	return ch;
}
#endif 
//-------------------------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------------------------
// @brief		�������ų�ʼ��
// @param		uartn		ѡ�񴮿�
// @return		void  
// Sample usage:		uart_pin_init(uartn);   �ڲ����� �����ֶ�����
//-------------------------------------------------------------------------------------------------------------------
void uart_pin_init(UARTn_enum uartn)
{
	switch(uartn)
	{
		case UART_1:
			gpio_init(GPIO_A,Pin_9,AF_PP);
			gpio_init(GPIO_A,Pin_10,ID);
		  break;
		case UART_2:
			gpio_init(GPIO_A,Pin_2,AF_PP);
			gpio_init(GPIO_A,Pin_3,ID);
		  break;
		case UART_3:
			gpio_init(GPIO_B,Pin_10,AF_PP);
			gpio_init(GPIO_B,Pin_11,ID);
		  break;
	}

}

//-------------------------------------------------------------------------------------------------------------------
// @brief		���ڲ����ʼ���
// @param		uartn		ѡ�񴮿�
// @param		baud		������
// @return		void  
// Sample usage:		uart_baud_config(uartn,baud);   �ڲ����� �����ֶ�����
//-------------------------------------------------------------------------------------------------------------------
void uart_baud_config(UARTn_enum uartn,int baud)
{
	uint32_t temp;  
  float temp1;	
	int fre;
	if(uartn==UART_1)
		fre=PCLK2;
	else 
		fre=PCLK1;
	temp = fre/(16*baud);
	temp1 = (float)fre/(16.0*baud) - temp;
	temp1 = temp1*16;

	uart_index[uartn]->BRR = (temp<<4)+(uint32_t)temp1;

}

//-------------------------------------------------------------------------------------------------------------------
// @brief		�������ó�ʼ��
// @param		uartn		ѡ�񴮿�
// @param		baud		���ڲ�����
// @return		void  
// Sample usage:		uart_init(UART_1,9600,0x00);
//-------------------------------------------------------------------------------------------------------------------
void uart_init(UARTn_enum uartn,int baud,uint8_t priority)
{
	if(uartn == UART_1)
		RCC->APB2ENR |= 1<<14;
	else
		RCC->APB1ENR |= 1<<(uartn+16);  //����ʱ��ʹ��
	uart_pin_init(uartn);               //�������ų�ʼ��

	uart_baud_config(uartn,baud);       //���ڲ���������
	uart_index[uartn]->CR1 |= 0x202C;   //ʹ�ܴ��� ʹ�ܷ��ͽ��� ʹ�ܽ����ж�
	NVIC_init(priority,uartn+37);       //�жϹ�������ʼ��
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		���ڷ����ֽ�
// @param		uartn		ѡ�񴮿�
// @param		Byte    ���͵��ֽ�
// @return		void  
// Sample usage:		uart_sendbyte(UART_1,0x01);
//-------------------------------------------------------------------------------------------------------------------
void uart_sendbyte(UARTn_enum uartn,uint8_t Byte)  
{
	while((uart_index[uartn]->SR&0x40)==0);  //�ȴ��������
	uart_index[uartn]->DR = Byte;        //���ݴ������ݼĴ���
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		���ڷ����ַ���
// @param		uartn		ѡ�񴮿�
// @param		str    ���͵��ַ���
// @return		void  
// Sample usage:		uart_sendstr(UART_1,"error");
//-------------------------------------------------------------------------------------------------------------------
void uart_sendstr(UARTn_enum uartn, char* str)
{
	while(*str)
	{
		uart_sendbyte(uartn, *str++);
	}
}


//-------------------------------------------------------------------------------------------------------------------
// @brief		���ڽ����ֽ�(�ڴ����ж������)
// @param		uartn		ѡ�񴮿�
// @param		data    ���յ��ֽڴ洢��data����(��Ҫ�Լ�����һ��data����)
// @return		void  
// Sample usage:	uint8_t dat = uart_getbyte(UART_1);
//-------------------------------------------------------------------------------------------------------------------
uint8_t uart_getbyte(UARTn_enum uartn)
{
	uint8_t data = uart_index[uartn]->DR;	   //�����ݼĴ����ж�ȡ����
	return data;
}







