#include "stm32f10x.h"                  // Device header
#include "headfile.h"
char Serial_RxPacket[100];				//定义接收数据包数组，数据包格式"@MSG\r\n"
uint8_t Serial_RxFlag;					//定义接收数据包标志位


//以下为定时器中断服务函数
void TIM2_IRQHandler(void)
{
	if(TIM2->SR&1)
	{
		//此处编写中断代码
		TIM2->SR &= ~1; 
	}
}

void TIM3_IRQHandler(void)
{
	if(TIM3->SR&1)
	{
		//此处编写中断代码
//		speed_now = Encoder_count;
//		Encoder_count = 0;
		pid_control();
		
		TIM3->SR &= ~1; 
	}
}

void TIM4_IRQHandler(void)
{
	if(TIM4->SR&1)
	{
		//此处编写中断代码
		TIM4->SR &= ~1; 
	}
}


//以下为串口中断服务函数
void USART1_IRQHandler(void)
{
	static uint8_t RxState = 0;		//定义表示当前状态机状态的静态变量
	static uint8_t pRxPacket = 0;
	if (USART1->SR&0x20)
	{
		//此处编写中断代码
		//uint8_t RxData = USART_ReceiveData(USART1);			//读取数据寄存器，存放在接收的数据变量
		uint8_t RxData = uart_getbyte(UART_1);
		/*使用状态机的思路，依次处理数据包的不同部分*/
		
		/*当前状态为0，接收数据包包头*/
		if (RxState == 0)
		{
			if (RxData == '@' && Serial_RxFlag == 0)		//如果数据确实是包头，并且上一个数据包已处理完毕
			{
				RxState = 1;			//置下一个状态
				pRxPacket = 0;			//数据包的位置归零
			}
		}
		/*当前状态为1，接收数据包数据，同时判断是否接收到了第一个包尾*/
		else if (RxState == 1)
		{
			if (RxData == '\r')			//如果收到第一个包尾
			{
				RxState = 2;			//置下一个状态
			}
			else						//接收到了正常的数据
			{
				Serial_RxPacket[pRxPacket] = RxData;		//将数据存入数据包数组的指定位置
				pRxPacket ++;			//数据包的位置自增
			}
		}
		/*当前状态为2，接收数据包第二个包尾*/
		else if (RxState == 2)
		{
			if (RxData == '\n')			//如果收到第二个包尾
			{
				RxState = 0;			//状态归0
				Serial_RxPacket[pRxPacket] = '\0';			//将收到的字符数据包添加一个字符串结束标志
				Serial_RxFlag = 1;		//接收数据包标志位置1，成功接收一个数据包
			}
		}

		USART1->SR &= ~0x20;   //清除标志位
	}
}


void USART2_IRQHandler(void)
{
	if (USART2->SR&0x20)
	{
		//此处编写中断代码

		USART2->SR &= ~0x20;   //清除标志位
	}
}

void USART3_IRQHandler(void)
{
	if (USART3->SR&0x20)
	{
		//此处编写中断代码

		USART3->SR &= ~0x20;   //清除标志位
	}
}


//以下为外部中断服务函数
void EXTI0_IRQHandler(void) // PA0/PB0/PC0
{
	if(EXTI->PR&(1<<0))
	{
		//此处编写中断代码
		
		EXTI->PR = 1<<0; //清除标志位
	}
}

void EXTI1_IRQHandler(void) // PA1/PB1/PC1
{
	if(EXTI->PR&(1<<1))
	{
		//此处编写中断代码
		
		EXTI->PR = 1<<1; //清除标志位
	}
}
void EXTI2_IRQHandler(void) // PA2/PB2/PC2
{
	if(EXTI->PR&(1<<2))
	{
		//此处编写中断代码
		if(gpio_get(GPIO_A, Pin_3))
			Encoder_count1 --;
		else
			Encoder_count1 ++;
		
		EXTI->PR = 1<<2; //清除标志位
	}
}
void EXTI3_IRQHandler(void) // PA3/PB3/PC3
{
	if(EXTI->PR&(1<<3))
	{
		//此处编写中断代码
		
		EXTI->PR = 1<<3; //清除标志位
	}
}
void EXTI4_IRQHandler(void) // PA4/PB4/PC4
{
	if(EXTI->PR&(1<<4))
	{
		//此处编写中断代码
		if(gpio_get(GPIO_A, Pin_5))
			Encoder_count2 ++;
		else
			Encoder_count2 --;
		EXTI->PR = 1<<4; //清除标志位
	}
}

void EXTI9_5_IRQHandler(void)
{
	if(EXTI->PR&(1<<5))   //EXTI5  PA5/PB5/PC5
	{
		//此处编写中断代码

		EXTI->PR = 1<<5; //清除标志位
	}
	
	if(EXTI->PR&(1<<6))   //EXTI6  PA6/PB6/PC6
	{
		//此处编写中断代码
		
		EXTI->PR = 1<<6; //清除标志位
	}
	
	if(EXTI->PR&(1<<7))   //EXTI7  PA7/PB7/PC7
	{
		//此处编写中断代码
		
		// 获取原始数据
		/*
		MPU6050_GetData();
		HMC5883L_GetData();
		
		// 陀螺仪角度
		roll_gyro += (float)gx / 16.4 * 0.005;
		pitch_gyro += (float)gy / 16.4 * 0.005;
		yaw_gyro += (float)gz / 16.4 * 0.005;
		
		// 加速度计角度
		roll_acc = atan((float)ay/az) * 57.296;
		pitch_acc = - atan((float)ax/az) * 57.296;	
		yaw_acc = atan((float)ay/ax) * 57.296;
		
		// 磁力计角度
		yaw_hmc = atan2((float)hmc_x, (float)hmc_y) * 57.296;
		
		// 卡尔曼滤波融合角度
		roll_Kalman = Kalman_Filter(&KF_Roll, roll_acc, (float)gx / 16.4);
		pitch_Kalman = Kalman_Filter(&KF_Pitch, pitch_acc, (float)gy / 16.4);
		yaw_Kalman = Kalman_Filter(&KF_Yaw, yaw_hmc, (float)gz / 16.4);
		*/
		EXTI->PR = 1<<7; //清除标志位
	}
	
	if(EXTI->PR&(1<<8))   //EXTI8
	{
		//此处编写中断代码
		
		EXTI->PR = 1<<8; //清除标志位
	}
	
	if(EXTI->PR&(1<<9))   //EXTI9
	{
		//此处编写中断代码
		
		EXTI->PR = 1<<9; //清除标志位
	}
}
