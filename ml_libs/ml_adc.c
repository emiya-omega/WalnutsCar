#include "headfile.h"

ADC_TypeDef *adc_index[3] = {ADC1,ADC2};

//-------------------------------------------------------------------------------------------------------------------
// @brief		ADC���ų�ʼ��
// @param	  adc_channel		ѡ��ADCͨ��
// @return		void  
// Sample usage:	adc_pin_init(adc_channel);  �ڲ����� �����ֶ�����
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
// @brief		ADC��ʼ��
// @param	  adc		  ѡ��ADC
// @param	  adc_channel		ѡ��ADCͨ��
// @return		void  
// Sample usage:	adc_init(ADC_1,ADC_Channel_1);
//-------------------------------------------------------------------------------------------------------------------
void adc_init(ADCx_enum adc,ADCINx_enum adc_channel)
{
	adc_pin_init(adc_channel);
	
	RCC->APB2ENR |= 1<<(9+adc); //ʹ��adc1ʱ��
	RCC->APB2RSTR|= 1<<(9+adc); //ʱ�Ӹ�λ
	RCC->APB2RSTR&= ~(1<<(9+adc));//������λ
	
	RCC->CFGR &= ~(3<<14); //��Ƶ��������
	RCC->CFGR |= 1<<15;  //6��Ƶ adcƵ��Ϊ72/6=12MHZ
	
	adc_index[adc]->CR1 &= ~(0x0f<<16);  //adc����ģʽ���� Ĭ�϶���ģʽ
	adc_index[adc]->CR1 &= ~(1<<8);  //��ɨ��ģʽ
	adc_index[adc]->CR2 &= ~(1<<1);  //����ת��ģʽ
	
	adc_index[adc]->CR2 &= ~(7<<17); //��������ͨ����ת��λ����
	adc_index[adc]->CR2 |= 7<<17; //�������(SWSTART)
	adc_index[adc]->CR2 |= 1<<20; //�����ⲿ�������
	
	adc_index[adc]->CR2 &= ~(1<<11); //�����Ҷ���
	
	adc_index[adc]->SQR1 &= 0xff0fffff; //ֻ��ͨһ��ת��ͨ��
	
	if(adc_channel>=10)     //���ò���ʱ��
	{
		adc_index[adc]->SMPR1 &= ~(7<<(3*(adc_channel-10)));
		adc_index[adc]->SMPR1 |= 7<<(3*(adc_channel-10));
	}
	else
	{
		adc_index[adc]->SMPR2 &= ~(7<<(3*adc_channel));
		adc_index[adc]->SMPR2 |= 7<<(3*adc_channel);
	}
	
	adc_index[adc]->CR2 |= 1<<0;  //����ADCת����
	
	adc_index[adc]->CR2 |= 1<<3; //ʹ�ܸ�λУ׼
	while(adc_index[adc]->CR2&(1<<3));//�ȴ�У׼����
	adc_index[adc]->CR2 |= 1<<2;  //����ADУ׼
	while(adc_index[adc]->CR2&(1<<2)); //�ȴ�У׼����
	
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		��ȡADCת��ֵ
// @param	  adc		  ѡ��ADC
// @param	  adc_channel		ѡ��ADCͨ��
// @return		uint16_t
// Sample usage:	uint16_t value = adc_get(ADC_1,ADC_Channel_1);
//-------------------------------------------------------------------------------------------------------------------
uint16_t adc_get(ADCx_enum adc,ADCINx_enum adc_channel)
{
	adc_index[adc]->SQR3 = adc_channel;  //ѡ��ͨ��
	adc_index[adc]->CR2 |= 1<<22; //��ʼת������ͨ��
	while(!(adc_index[adc]->SR&(1<<1)));  //�ȴ�ת������
	
	return adc_index[adc]->DR;    //����ADת��ֵ
}
