#include "headfile.h"

//-------------------------------------------------------------------------------------------------------------------
// @brief		EXTI引脚初始化(默认上拉)
// @param	  pin	  选择引脚
// @return		void  
// Sample usage:		exti_pin_init(pin);  //内部调用 无需关心
//-------------------------------------------------------------------------------------------------------------------
void exti_pin_init(EXTI_Pnx_enum pin)
{
	switch(pin)
	{
		case EXTI_PA0:{gpio_init(GPIO_A,Pin_0,IU);break;};
		case EXTI_PB0:{gpio_init(GPIO_B,Pin_0,IU);break;};
		case EXTI_PC0:{gpio_init(GPIO_C,Pin_0,IU);break;};
		case EXTI_PA1:{gpio_init(GPIO_A,Pin_1,IU);break;};
		case EXTI_PB1:{gpio_init(GPIO_B,Pin_1,IU);break;};
		case EXTI_PC1:{gpio_init(GPIO_C,Pin_1,IU);break;};
		case EXTI_PA2:{gpio_init(GPIO_A,Pin_2,IU);break;};
		case EXTI_PB2:{gpio_init(GPIO_B,Pin_2,IU);break;};
		case EXTI_PC2:{gpio_init(GPIO_C,Pin_2,IU);break;};
		case EXTI_PA3:{gpio_init(GPIO_A,Pin_3,IU);break;};
		case EXTI_PB3:{gpio_init(GPIO_B,Pin_3,IU);break;};
		case EXTI_PC3:{gpio_init(GPIO_C,Pin_3,IU);break;};
		case EXTI_PA4:{gpio_init(GPIO_A,Pin_4,IU);break;};
		case EXTI_PB4:{gpio_init(GPIO_B,Pin_4,IU);break;};
		case EXTI_PC4:{gpio_init(GPIO_C,Pin_4,IU);break;};
		case EXTI_PA5:{gpio_init(GPIO_A,Pin_5,IU);break;};
		case EXTI_PB5:{gpio_init(GPIO_B,Pin_5,IU);break;};
		case EXTI_PC5:{gpio_init(GPIO_C,Pin_5,IU);break;};
		case EXTI_PA6:{gpio_init(GPIO_A,Pin_6,IU);break;};
		case EXTI_PB6:{gpio_init(GPIO_B,Pin_6,IU);break;};
		case EXTI_PC6:{gpio_init(GPIO_C,Pin_6,IU);break;};
		case EXTI_PA7:{gpio_init(GPIO_A,Pin_7,IU);break;};
		case EXTI_PB7:{gpio_init(GPIO_B,Pin_7,IU);break;};
		case EXTI_PC7:{gpio_init(GPIO_C,Pin_7,IU);break;};
	}
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		EXTI初始化
// @param	  pin	  选择外部中断引脚
// @param	  trigger    选择触发方式(上升沿/下降沿)
// @param	  priority   设置中断优先级(0~15)
// @return		void  
// Sample usage:		exti_init(EXTI_PA5,RISING,0);
//-------------------------------------------------------------------------------------------------------------------
void exti_init(EXTI_Pnx_enum pin,EXTI_Trigger_enum trigger,uint8_t priority)
{
	uint8_t tempoffs = ((pin/3)%4)*4;
	uint8_t tempaddr = (pin/3)/4;
	
	exti_pin_init(pin);   //引脚初始化
	
	RCC->APB2ENR |= 1<<0;  //使能AFIO
	
	AFIO->EXTICR[tempaddr] &= ~(0x0f<<tempoffs);  //外部中断配置清零
	if(pin%3!=0)        //不是PA引脚
		AFIO->EXTICR[tempaddr] |= (1<<(pin%3-1))<<tempoffs;  //外部中断配置
	
	EXTI->IMR |= 1<<(pin/3);  //开启中断
	
	if(trigger == RISING)
		EXTI->RTSR |= 1<<(pin/3); //上升沿触发
	else
		EXTI->FTSR |= 1<<(pin/3); //下降沿触发

	if(pin/3<=4)
		NVIC_init(priority,pin/3+6);   //配置中断
	else
		NVIC_init(priority,EXTI9_5_IRQn);
	
}
