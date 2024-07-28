#include "headfile.h"

//-------------------------------------------------------------------------------------------------------------------
// @brief		PWM引脚初始化
// @param	  timn_chn		选择定时器通道
// @return		void  
// Sample usage:	pwm_pin_init(timn_chn);  内部调用 无需手动调用
//-------------------------------------------------------------------------------------------------------------------
void pwm_pin_init(TIMn_CHn_enum timn_chn)
{
		switch(timn_chn)
		{
			case TIM2_CH1:
        gpio_init(GPIO_A,Pin_0,AF_PP);
			  break;
			case TIM2_CH2:
        gpio_init(GPIO_A,Pin_1,AF_PP);
			  break;
			case TIM2_CH3:
        gpio_init(GPIO_A,Pin_2,AF_PP);
			  break;
			case TIM2_CH4:
        gpio_init(GPIO_A,Pin_3,AF_PP);
			  break;
			case TIM3_CH1:
        gpio_init(GPIO_A,Pin_6,AF_PP);
			  break;
			case TIM3_CH2:
        gpio_init(GPIO_A,Pin_7,AF_PP);
			  break;
			case TIM3_CH3:
        gpio_init(GPIO_B,Pin_0,AF_PP);
			  break;
			case TIM3_CH4:
        gpio_init(GPIO_B,Pin_1,AF_PP);
			  break;
			case TIM4_CH1:
        gpio_init(GPIO_B,Pin_6,AF_PP);
			  break;
			case TIM4_CH2:
        gpio_init(GPIO_B,Pin_7,AF_PP);
			  break;
			case TIM4_CH3:
        gpio_init(GPIO_B,Pin_8,AF_PP);
			  break;
			case TIM4_CH4:
        gpio_init(GPIO_B,Pin_9,AF_PP);
			  break;

		}
}

 

//-------------------------------------------------------------------------------------------------------------------
// @brief		PWM初始化
// @param	  timn		选择定时器(可选用的定时器参考ml_tim.h中的枚举定义)
// @param	  timn_chn		选择定时器通道(可选用的通道参考ml_pwm.h中的枚举定义)
// @param	  fre		定义频率（频率要大于15）
// @return		void  
// Sample usage:	pwm_init(TIM_2,TIM2_CH1,50);     
//-------------------------------------------------------------------------------------------------------------------
void pwm_init(TIMn_enum timn,TIMn_CHn_enum timn_chn,int fre)
{ 
	uint8_t ch = timn_chn%4+1;
	RCC->APB1ENR |= 1<<timn;
	tim_index[timn]->ARR = 1000000/fre-1; //自动重装载值
	tim_index[timn]->PSC = 72-1;       //预分频器值
  pwm_pin_init(timn_chn);
	
	switch(ch)
	{
	  case 1:
		{
			tim_index[timn]->CCMR1 |= 6<<4;	   //设置PWM1模式
			tim_index[timn]->CCMR1 |= 1<<3;       //预装载使能
			tim_index[timn]->CCR1 = 0;            //初始化CCR值为duty
			break;
		}
		case 2:
		{
			tim_index[timn]->CCMR1 |= 6<<12;	   //设置PWM1模式	
      tim_index[timn]->CCMR1 |= 1<<11;			 //预装载使能
			tim_index[timn]->CCR2 = 0;            //初始化CCR值为duty
			break;
		}
		case 3:
		{
			tim_index[timn]->CCMR2 |= 6<<4;	   //设置PWM1模式
      tim_index[timn]->CCMR2 |= 1<<3;			 //预装载使能
			tim_index[timn]->CCR3 = 0;            //初始化CCR值为duty
			break;		
		}
		case 4:
		{
			tim_index[timn]->CCMR2 |= 6<<12;	   //设置PWM1模式		
      tim_index[timn]->CCMR2 |= 1<<11;			 //预装载使能
			tim_index[timn]->CCR4 = 0;            //初始化CCR值为duty
			break;				
		}
	
	}
	tim_index[timn]->CCER |= 0x01<<(4*(ch-1)); //PWM输出使能
	tim_index[timn]->CR1 |= 0x81;        //使能计数器和ARPE
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		PWM占空比更新
// @param	  timn		选择定时器(可选用的定时器参考ml_tim.h中的枚举定义)
// @param	  timn_chn		选择定时器通道(可选用的通道参考ml_pwm.h中的枚举定义)
// @param	  duty    更新功率
// @return		void  
// Sample usage:	pwm_update(TIM_2,TIM2_CH1,0);     
//-------------------------------------------------------------------------------------------------------------------
void pwm_update(TIMn_enum timn,TIMn_CHn_enum timn_chn,uint16_t duty)
{
	uint8_t ch = timn_chn%4+1;
	if(duty>=MAX_DUTY)
		duty=MAX_DUTY;
	uint16_t temp = tim_index[timn]->ARR;   //读取ARR
	switch(ch)
	{
		case 1:
			tim_index[timn]->CCR1 = (uint16_t)((float)duty/MAX_DUTY*(temp+1));               //更新CCR值为duty
			break;
		case 2:	
			tim_index[timn]->CCR2 = (uint16_t)((float)duty/MAX_DUTY*(temp+1));               //更新CCR值为duty
			break;
		case 3:	
			tim_index[timn]->CCR3 = (uint16_t)((float)duty/MAX_DUTY*(temp+1));               //更新CCR值为duty
			break;		
		case 4:
			tim_index[timn]->CCR4 = (uint16_t)((float)duty/MAX_DUTY*(temp+1));               //更新CCR值为duty
			break;					
	}
}


