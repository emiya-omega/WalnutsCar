#include "headfile.h"

//-------------------------------------------------------
//以下代码用于配置中断 已在函数库中内部调用 用户无需关心
//-------------------------------------------------------

void NVIC_Group_Config(uint8_t group)
{
	uint32_t temp,temp1;
	temp1 = ((~group)&0x07)<<8; //设置AIRCR中8~10位的值 代表5个优先级分组
	temp = SCB->AIRCR;          //读取原值
	temp &= 0x0000F8FF;         //密匙位和分组位清空 其余位保留
	temp |= temp1;              //重新写入分组位
	temp |= 0x05FA0000;         //写入密匙
	SCB->AIRCR = temp;
}

void NVIC_init(uint8_t PreemptionPriority,uint8_t nvic_channel)
{
	uint8_t temp;
	
	NVIC_Group_Config(4);                                  //默认选择分组4:4位抢占优先级+0位子优先级
	temp = NVIC->IP[nvic_channel]&0x01;                    //清空中断优先级设置寄存器中高四位
	NVIC->IP[nvic_channel] = temp|(PreemptionPriority<<4); //写入抢占优先级
	
	NVIC->ISER[nvic_channel/32] = 1<<nvic_channel%32;      //中断使能
	
}
