#include "headfile.h"

//-------------------------------------------------------------------------------------------------------------------
// @brief		EXTI���ų�ʼ��(Ĭ������)
// @param	  pin	  ѡ������
// @return		void  
// Sample usage:		exti_pin_init(pin);  //�ڲ����� �������
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
// @brief		EXTI��ʼ��
// @param	  pin	  ѡ���ⲿ�ж�����
// @param	  trigger    ѡ�񴥷���ʽ(������/�½���)
// @param	  priority   �����ж����ȼ�(0~15)
// @return		void  
// Sample usage:		exti_init(EXTI_PA5,RISING,0);
//-------------------------------------------------------------------------------------------------------------------
void exti_init(EXTI_Pnx_enum pin,EXTI_Trigger_enum trigger,uint8_t priority)
{
	uint8_t tempoffs = ((pin/3)%4)*4;
	uint8_t tempaddr = (pin/3)/4;
	
	exti_pin_init(pin);   //���ų�ʼ��
	
	RCC->APB2ENR |= 1<<0;  //ʹ��AFIO
	
	AFIO->EXTICR[tempaddr] &= ~(0x0f<<tempoffs);  //�ⲿ�ж���������
	if(pin%3!=0)        //����PA����
		AFIO->EXTICR[tempaddr] |= (1<<(pin%3-1))<<tempoffs;  //�ⲿ�ж�����
	
	EXTI->IMR |= 1<<(pin/3);  //�����ж�
	
	if(trigger == RISING)
		EXTI->RTSR |= 1<<(pin/3); //�����ش���
	else
		EXTI->FTSR |= 1<<(pin/3); //�½��ش���

	if(pin/3<=4)
		NVIC_init(priority,pin/3+6);   //�����ж�
	else
		NVIC_init(priority,EXTI9_5_IRQn);
	
}
