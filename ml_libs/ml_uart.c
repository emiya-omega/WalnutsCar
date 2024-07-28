#include "headfile.h"

#define PCLK1	36000000  //除了USART1外，其余串口都挂在APB1总线上，使用PCLK1分频 时钟频率为36MHZ
#define PCLK2 72000000  //USART1挂在APB2总线上，使用PCLK2分频 时钟频率为72MHZ 

USART_TypeDef *uart_index[3] = { USART1 , USART2 , USART3 };

//-------------------------------------------------------------------------------------------------------------------
//以下代码添加后 可以使用printf函数 需要初始化串口UART_1
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef’ d in stdio.h. */ 
FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定向fputc函数
//printf的输出，指向fputc，由fputc输出到串口
//这里使用串口1(USART1)输出printf信息
int fputc(int ch, FILE *f)
{
	while((USART1->SR&0x40)==0);  //等待发送完成
	USART1->DR = (uint8_t) ch;  
	return ch;
}
#endif 
//-------------------------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------------------------
// @brief		串口引脚初始化
// @param		uartn		选择串口
// @return		void  
// Sample usage:		uart_pin_init(uartn);   内部调用 无需手动调用
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
// @brief		串口波特率计算
// @param		uartn		选择串口
// @param		baud		波特率
// @return		void  
// Sample usage:		uart_baud_config(uartn,baud);   内部调用 无需手动调用
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
// @brief		串口配置初始化
// @param		uartn		选择串口
// @param		baud		串口波特率
// @return		void  
// Sample usage:		uart_init(UART_1,9600,0x00);
//-------------------------------------------------------------------------------------------------------------------
void uart_init(UARTn_enum uartn,int baud,uint8_t priority)
{
	if(uartn == UART_1)
		RCC->APB2ENR |= 1<<14;
	else
		RCC->APB1ENR |= 1<<(uartn+16);  //串口时钟使能
	uart_pin_init(uartn);               //串口引脚初始化

	uart_baud_config(uartn,baud);       //串口波特率配置
	uart_index[uartn]->CR1 |= 0x202C;   //使能串口 使能发送接收 使能接收中断
	NVIC_init(priority,uartn+37);       //中断管理器初始化
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		串口发送字节
// @param		uartn		选择串口
// @param		Byte    发送的字节
// @return		void  
// Sample usage:		uart_sendbyte(UART_1,0x01);
//-------------------------------------------------------------------------------------------------------------------
void uart_sendbyte(UARTn_enum uartn,uint8_t Byte)  
{
	while((uart_index[uartn]->SR&0x40)==0);  //等待发送完成
	uart_index[uartn]->DR = Byte;        //数据存入数据寄存器
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		串口发送字符串
// @param		uartn		选择串口
// @param		str    发送的字符串
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
// @brief		串口接收字节(在串口中断里调用)
// @param		uartn		选择串口
// @param		data    接收的字节存储在data变量(需要自己定义一个data变量)
// @return		void  
// Sample usage:	uint8_t dat = uart_getbyte(UART_1);
//-------------------------------------------------------------------------------------------------------------------
uint8_t uart_getbyte(UARTn_enum uartn)
{
	uint8_t data = uart_index[uartn]->DR;	   //从数据寄存器中读取数据
	return data;
}







