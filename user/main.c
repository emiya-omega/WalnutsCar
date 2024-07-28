#include "headfile.h"

int main(void)
{
	delay_ms(10000);
	OLED_Init();
	
	motor_init();
	encoder_init();

	uart_init(UART_1,9600,0);
	
	pid_init(&motorA, DELTA_PID, 10, 10, 5);
	pid_init(&motorB, DELTA_PID, 10, 10, 5);
	pid_init(&angle, POSITION_PID, 2, 0, 0.5);
	pid_init(&pos, DELTA_PID, 1.5, 0.4, 0.1);

	
	//gray_init();
	
	I2C_Init();
	//MPU6050_Init();
	//HMC5883L_Init();
	exti_init(EXTI_PB7,RISING,0);
	
	tim_interrupt_ms_init(TIM_3,10,0);
	while (1)
	{
		/*if (Serial_RxFlag == 1)		//如果接收到数据包
		{
			float value = atof(Serial_RxPacket);

			OLED_ShowString(4, 1, "                ");
			OLED_ShowString(4, 1, Serial_RxPacket);				//OLED清除指定位置，并显示接收到的数据包		
			OLED_ShowFloat(4, 1 , value,3,2);
			Serial_RxFlag = 0;			//处理完成后，需要将接收数据包标志位清零，否则将无法接收后续数据包
		}
//		printf("ax:%d, ay:%d az:%d gx:%d gy:%d gz:%d\r\n", ax, ay, az, gx, gy, gz);
//		printf("yaw:%.2f  pitch:%.2f roll:%.2f\r\n", yaw_Kalman, pitch_Kalman, roll_Kalman);
//		OLED_ShowFloat(1, 1, yaw_Kalman, 3, 2);
//		delay_ms(20);
*/	} 
}
