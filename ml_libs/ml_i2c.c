#include "ml_i2c.h"

#define SDA_Output(x)  gpio_set(I2C_GPIO, I2C_SDA_GPIO_Pin, x)
#define SCL_Output(x)  gpio_set(I2C_GPIO, I2C_SCL_GPIO_Pin, x)
#define SDA_Input() gpio_get(I2C_GPIO, I2C_SDA_GPIO_Pin)

void I2C_Init()
{
	gpio_init(I2C_GPIO, I2C_SCL_GPIO_Pin, OUT_OD);
	gpio_init(I2C_GPIO, I2C_SDA_GPIO_Pin, OUT_OD);
	
	SDA_Output(1);
	SCL_Output(1);
}

// 起始信号
void I2C_Start()
{
	SDA_Output(1);
	SCL_Output(1);
	SDA_Output(0);
	SCL_Output(0);
}

// 终止信号 
void I2C_Stop()
{
	SDA_Output(0);
	SCL_Output(1);
	SDA_Output(1);
}

// 主机发送一个字节
void I2C_SendByte(uint8_t byte)
{
	for(int i = 0; i < 8; i++)
	{
		if(byte & (0x80>>i))
			SDA_Output(1);
		else
			SDA_Output(0);
		SCL_Output(1);
		SCL_Output(0);
	}
}

// 主机接收一个字节
uint8_t I2C_ReceiveByte()
{
	uint8_t byte = 0;
	SDA_Output(1);
	for(int i = 0; i < 8; i++)
	{
		SCL_Output(1);
		if(SDA_Input())
			byte |= (0x80>>i);
		SCL_Output(0);
	}
	
	return byte;
}

// 主机应答 
void I2C_SendAck()
{
	SDA_Output(0);
	SCL_Output(1);
	SCL_Output(0);
}

// 主机不应答 
void I2C_NotSendAck()
{
	SDA_Output(1);
	SCL_Output(1);
	SCL_Output(0);
}

// 等待从机应答 
uint8_t I2C_WaitAck()
{
	uint8_t byte = 0;
	SDA_Output(1);
	SCL_Output(1);
	byte = SDA_Input();
	SCL_Output(0);
	
	return byte;
}

