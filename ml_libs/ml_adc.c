#include "headfile.h"

ADC_TypeDef *adc_index[3] = {ADC1,ADC2};

//-------------------------------------------------------------------------------------------------------------------
// @brief		ADC引脚初始化
// @param	  adc_channel		选择ADC通道
// @return		void  
// Sample usage:	adc_pin_init(adc_channel);  内部调用 无需手动调用
//-------------------------------------------------------------------------------------------------------------------
void adc_pin_init(ADCINx_enum adc_channel)
{
	switch(adc_channel)
	{
		case ADC_Channel_0:{gpio_init(GPIO_A,Pin_0,AIN);break;};
		case ADC_Channel_1:{gpio_init(GPIO_A,Pin_1,AIN);break;};
		case ADC_Channel_2:{gpio_init(GPIO_A,Pin_2,AIN);break;};
		case ADC_Channel_3:{gpio_init(GPIO_A,Pin_3,AIN);break;};
		case ADC_Channel_4:{gpio_init(GPIO_A,Pin_4,AIN);break;};
		case ADC_Channel_5:{gpio_init(GPIO_A,Pin_5,AIN);break;};
		case ADC_Channel_6:{gpio_init(GPIO_A,Pin_6,AIN);break;};
		case ADC_Channel_7:{gpio_init(GPIO_A,Pin_7,AIN);break;};
		case ADC_Channel_8:{gpio_init(GPIO_B,Pin_0,AIN);break;};
		case ADC_Channel_9:{gpio_init(GPIO_B,Pin_1,AIN);break;};
		case ADC_Channel_10:{gpio_init(GPIO_C,Pin_0,AIN);break;};
		case ADC_Channel_11:{gpio_init(GPIO_C,Pin_1,AIN);break;};
		case ADC_Channel_12:{gpio_init(GPIO_C,Pin_2,AIN);break;};
		case ADC_Channel_13:{gpio_init(GPIO_C,Pin_3,AIN);break;};
		case ADC_Channel_14:{gpio_init(GPIO_C,Pin_4,AIN);break;};
		case ADC_Channel_15:{gpio_init(GPIO_C,Pin_5,AIN);break;};
	}
	
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		ADC初始化
// @param	  adc		  选择ADC
// @param	  adc_channel		选择ADC通道
// @return		void  
// Sample usage:	adc_init(ADC_1,ADC_Channel_1);
//-------------------------------------------------------------------------------------------------------------------
void adc_init(ADCx_enum adc,ADCINx_enum adc_channel)
{
	adc_pin_init(adc_channel);
	
	RCC->APB2ENR |= 1<<(9+adc); //使能adc1时钟
	RCC->APB2RSTR|= 1<<(9+adc); //时钟复位
	RCC->APB2RSTR&= ~(1<<(9+adc));//结束复位
	
	RCC->CFGR &= ~(3<<14); //分频因子清零
	RCC->CFGR |= 1<<15;  //6分频 adc频率为72/6=12MHZ
	
	adc_index[adc]->CR1 &= ~(0x0f<<16);  //adc工作模式清零 默认独立模式
	adc_index[adc]->CR1 &= ~(1<<8);  //非扫描模式
	adc_index[adc]->CR2 &= ~(1<<1);  //单次转换模式
	
	adc_index[adc]->CR2 &= ~(7<<17); //启动规则通道组转换位清零
	adc_index[adc]->CR2 |= 7<<17; //软件触发(SWSTART)
	adc_index[adc]->CR2 |= 1<<20; //开启外部软件触发
	
	adc_index[adc]->CR2 &= ~(1<<11); //数据右对齐
	
	adc_index[adc]->SQR1 &= 0xff0fffff; //只开通一个转换通道
	
	if(adc_channel>=10)     //设置采样时间
	{
		adc_index[adc]->SMPR1 &= ~(7<<(3*(adc_channel-10)));
		adc_index[adc]->SMPR1 |= 7<<(3*(adc_channel-10));
	}
	else
	{
		adc_index[adc]->SMPR2 &= ~(7<<(3*adc_channel));
		adc_index[adc]->SMPR2 |= 7<<(3*adc_channel);
	}
	
	adc_index[adc]->CR2 |= 1<<0;  //开启ADC转换器
	
	adc_index[adc]->CR2 |= 1<<3; //使能复位校准
	while(adc_index[adc]->CR2&(1<<3));//等待校准结束
	adc_index[adc]->CR2 |= 1<<2;  //开启AD校准
	while(adc_index[adc]->CR2&(1<<2)); //等待校准结束
	
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		读取ADC转换值
// @param	  adc		  选择ADC
// @param	  adc_channel		选择ADC通道
// @return		uint16_t
// Sample usage:	uint16_t value = adc_get(ADC_1,ADC_Channel_1);
//-------------------------------------------------------------------------------------------------------------------
uint16_t adc_get(ADCx_enum adc,ADCINx_enum adc_channel)
{
	adc_index[adc]->SQR3 = adc_channel;  //选择通道
	adc_index[adc]->CR2 |= 1<<22; //开始转换规则通道
	while(!(adc_index[adc]->SR&(1<<1)));  //等待转换结束
	
	return adc_index[adc]->DR;    //返回AD转换值
}
