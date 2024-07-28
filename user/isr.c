#include "stm32f10x.h"                  // Device header
#include "headfile.h"
char Serial_RxPacket[100];				//����������ݰ����飬���ݰ���ʽ"@MSG\r\n"
uint8_t Serial_RxFlag;					//����������ݰ���־λ


//����Ϊ��ʱ���жϷ�����
void TIM2_IRQHandler(void)
{
	if(TIM2->SR&1)
	{
		//�˴���д�жϴ���
		TIM2->SR &= ~1; 
	}
}

void TIM3_IRQHandler(void)
{
	if(TIM3->SR&1)
	{
		//�˴���д�жϴ���
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
		//�˴���д�жϴ���
		TIM4->SR &= ~1; 
	}
}


//����Ϊ�����жϷ�����
void USART1_IRQHandler(void)
{
	static uint8_t RxState = 0;		//�����ʾ��ǰ״̬��״̬�ľ�̬����
	static uint8_t pRxPacket = 0;
	if (USART1->SR&0x20)
	{
		//�˴���д�жϴ���
		//uint8_t RxData = USART_ReceiveData(USART1);			//��ȡ���ݼĴ���������ڽ��յ����ݱ���
		uint8_t RxData = uart_getbyte(UART_1);
		/*ʹ��״̬����˼·�����δ������ݰ��Ĳ�ͬ����*/
		
		/*��ǰ״̬Ϊ0���������ݰ���ͷ*/
		if (RxState == 0)
		{
			if (RxData == '@' && Serial_RxFlag == 0)		//�������ȷʵ�ǰ�ͷ��������һ�����ݰ��Ѵ������
			{
				RxState = 1;			//����һ��״̬
				pRxPacket = 0;			//���ݰ���λ�ù���
			}
		}
		/*��ǰ״̬Ϊ1���������ݰ����ݣ�ͬʱ�ж��Ƿ���յ��˵�һ����β*/
		else if (RxState == 1)
		{
			if (RxData == '\r')			//����յ���һ����β
			{
				RxState = 2;			//����һ��״̬
			}
			else						//���յ�������������
			{
				Serial_RxPacket[pRxPacket] = RxData;		//�����ݴ������ݰ������ָ��λ��
				pRxPacket ++;			//���ݰ���λ������
			}
		}
		/*��ǰ״̬Ϊ2���������ݰ��ڶ�����β*/
		else if (RxState == 2)
		{
			if (RxData == '\n')			//����յ��ڶ�����β
			{
				RxState = 0;			//״̬��0
				Serial_RxPacket[pRxPacket] = '\0';			//���յ����ַ����ݰ����һ���ַ���������־
				Serial_RxFlag = 1;		//�������ݰ���־λ��1���ɹ�����һ�����ݰ�
			}
		}

		USART1->SR &= ~0x20;   //�����־λ
	}
}


void USART2_IRQHandler(void)
{
	if (USART2->SR&0x20)
	{
		//�˴���д�жϴ���

		USART2->SR &= ~0x20;   //�����־λ
	}
}

void USART3_IRQHandler(void)
{
	if (USART3->SR&0x20)
	{
		//�˴���д�жϴ���

		USART3->SR &= ~0x20;   //�����־λ
	}
}


//����Ϊ�ⲿ�жϷ�����
void EXTI0_IRQHandler(void) // PA0/PB0/PC0
{
	if(EXTI->PR&(1<<0))
	{
		//�˴���д�жϴ���
		
		EXTI->PR = 1<<0; //�����־λ
	}
}

void EXTI1_IRQHandler(void) // PA1/PB1/PC1
{
	if(EXTI->PR&(1<<1))
	{
		//�˴���д�жϴ���
		
		EXTI->PR = 1<<1; //�����־λ
	}
}
void EXTI2_IRQHandler(void) // PA2/PB2/PC2
{
	if(EXTI->PR&(1<<2))
	{
		//�˴���д�жϴ���
		if(gpio_get(GPIO_A, Pin_3))
			Encoder_count1 --;
		else
			Encoder_count1 ++;
		
		EXTI->PR = 1<<2; //�����־λ
	}
}
void EXTI3_IRQHandler(void) // PA3/PB3/PC3
{
	if(EXTI->PR&(1<<3))
	{
		//�˴���д�жϴ���
		
		EXTI->PR = 1<<3; //�����־λ
	}
}
void EXTI4_IRQHandler(void) // PA4/PB4/PC4
{
	if(EXTI->PR&(1<<4))
	{
		//�˴���д�жϴ���
		if(gpio_get(GPIO_A, Pin_5))
			Encoder_count2 ++;
		else
			Encoder_count2 --;
		EXTI->PR = 1<<4; //�����־λ
	}
}

void EXTI9_5_IRQHandler(void)
{
	if(EXTI->PR&(1<<5))   //EXTI5  PA5/PB5/PC5
	{
		//�˴���д�жϴ���

		EXTI->PR = 1<<5; //�����־λ
	}
	
	if(EXTI->PR&(1<<6))   //EXTI6  PA6/PB6/PC6
	{
		//�˴���д�жϴ���
		
		EXTI->PR = 1<<6; //�����־λ
	}
	
	if(EXTI->PR&(1<<7))   //EXTI7  PA7/PB7/PC7
	{
		//�˴���д�жϴ���
		
		// ��ȡԭʼ����
		/*
		MPU6050_GetData();
		HMC5883L_GetData();
		
		// �����ǽǶ�
		roll_gyro += (float)gx / 16.4 * 0.005;
		pitch_gyro += (float)gy / 16.4 * 0.005;
		yaw_gyro += (float)gz / 16.4 * 0.005;
		
		// ���ٶȼƽǶ�
		roll_acc = atan((float)ay/az) * 57.296;
		pitch_acc = - atan((float)ax/az) * 57.296;	
		yaw_acc = atan((float)ay/ax) * 57.296;
		
		// �����ƽǶ�
		yaw_hmc = atan2((float)hmc_x, (float)hmc_y) * 57.296;
		
		// �������˲��ںϽǶ�
		roll_Kalman = Kalman_Filter(&KF_Roll, roll_acc, (float)gx / 16.4);
		pitch_Kalman = Kalman_Filter(&KF_Pitch, pitch_acc, (float)gy / 16.4);
		yaw_Kalman = Kalman_Filter(&KF_Yaw, yaw_hmc, (float)gz / 16.4);
		*/
		EXTI->PR = 1<<7; //�����־λ
	}
	
	if(EXTI->PR&(1<<8))   //EXTI8
	{
		//�˴���д�жϴ���
		
		EXTI->PR = 1<<8; //�����־λ
	}
	
	if(EXTI->PR&(1<<9))   //EXTI9
	{
		//�˴���д�жϴ���
		
		EXTI->PR = 1<<9; //�����־λ
	}
}
