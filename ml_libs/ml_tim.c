#include "headfile.h"

TIM_TypeDef *tim_index[3] = { TIM2 , TIM3 , TIM4 };

//-------------------------------------------------------------------------------------------------------------------
// @brief		内部定时器初始化(同时初始化定时器中断)
// @param	  timn		选择定时器(可选用的定时器参考ml_tim.h中的枚举定义)
// @param	  time_ms  进入定时器中断的间隔(ms)
// @param	  priority 设置中断优先级(0~15 越小优先级越高)
// @return		void  
// Sample usage:	tim_interrupt_ms_init(TIM_2,1000,0);
//-------------------------------------------------------------------------------------------------------------------
void tim_interrupt_ms_init(TIMn_enum timn,int time_ms,uint8_t priority)
{	
		RCC->APB1ENR |= 1<<timn;  //定时器时钟使能

	  tim_index[timn]->ARR = 10*time_ms-1; //自动重装载值
	  tim_index[timn]->PSC = 7200-1;       //预分频器值
	  tim_index[timn]->CR1 |= 0x01;        //使能计数器
    tim_index[timn]->DIER |= 0x01;       //使能定时器中断
	  
		NVIC_init(priority,timn+28);         //中断管理初始化

}


