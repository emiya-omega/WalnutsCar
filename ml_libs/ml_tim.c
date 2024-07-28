#include "headfile.h"

TIM_TypeDef *tim_index[3] = { TIM2 , TIM3 , TIM4 };

//-------------------------------------------------------------------------------------------------------------------
// @brief		�ڲ���ʱ����ʼ��(ͬʱ��ʼ����ʱ���ж�)
// @param	  timn		ѡ��ʱ��(��ѡ�õĶ�ʱ���ο�ml_tim.h�е�ö�ٶ���)
// @param	  time_ms  ���붨ʱ���жϵļ��(ms)
// @param	  priority �����ж����ȼ�(0~15 ԽС���ȼ�Խ��)
// @return		void  
// Sample usage:	tim_interrupt_ms_init(TIM_2,1000,0);
//-------------------------------------------------------------------------------------------------------------------
void tim_interrupt_ms_init(TIMn_enum timn,int time_ms,uint8_t priority)
{	
		RCC->APB1ENR |= 1<<timn;  //��ʱ��ʱ��ʹ��

	  tim_index[timn]->ARR = 10*time_ms-1; //�Զ���װ��ֵ
	  tim_index[timn]->PSC = 7200-1;       //Ԥ��Ƶ��ֵ
	  tim_index[timn]->CR1 |= 0x01;        //ʹ�ܼ�����
    tim_index[timn]->DIER |= 0x01;       //ʹ�ܶ�ʱ���ж�
	  
		NVIC_init(priority,timn+28);         //�жϹ����ʼ��

}


