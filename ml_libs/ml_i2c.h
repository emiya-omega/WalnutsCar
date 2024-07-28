#ifndef _i2c_h
#define _i2c_h
#include "headfile.h"

/* 
   ���ģ��I2CЭ��
	 �޸������������� 
	 �Զ���SCL��SDA����
	 �轫��Ӧ�������óɿ�©���	 
*/
#define I2C_GPIO 	         GPIO_B
#define I2C_SCL_GPIO_Pin   Pin_10	
#define I2C_SDA_GPIO_Pin   Pin_11 	

void I2C_Init(void);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_SendByte(uint8_t byte);
uint8_t I2C_ReceiveByte(void);
void I2C_SendAck(void);
void I2C_NotSendAck(void);
uint8_t I2C_WaitAck(void);

#endif
