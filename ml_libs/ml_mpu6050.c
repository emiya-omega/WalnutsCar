#include "ml_mpu6050.h"

int16_t ax, ay, az, gx, gy, gz;
float roll_gyro, pitch_gyro, yaw_gyro;
float roll_acc, pitch_acc, yaw_acc;
float roll_Kalman, pitch_Kalman, yaw_Kalman;

void MPU6050_Write(uint8_t addr, uint8_t dat)
{
	I2C_Start();
	I2C_SendByte(MPU6050_ADDR);
	I2C_WaitAck();
	I2C_SendByte(addr);
	I2C_WaitAck();
	I2C_SendByte(dat);
	I2C_WaitAck();
	I2C_Stop();
}

uint8_t MPU6050_Read(uint8_t addr)
{
	I2C_Start();
	I2C_SendByte(MPU6050_ADDR);
	I2C_WaitAck();
	I2C_SendByte(addr);
	I2C_WaitAck();
	I2C_Stop();
	
	I2C_Start();
	I2C_SendByte(MPU6050_ADDR | 0x01);
	I2C_WaitAck();
	uint8_t dat = I2C_ReceiveByte();
	I2C_NotSendAck();
	I2C_Stop();
	
	return dat;
}

void MPU6050_Init()
{
	MPU6050_Write(PWR_MGMT_1, 0x02);   // 时钟源：PLL with Y axis gyroscope reference
	MPU6050_Write(SMPLRT_DIV, 0x27);	 // 设置采样频率为200HZ
	MPU6050_Write(CONFIG, 0x00);       // 失能DLPF
	MPU6050_Write(GYRO_CONFIG, 0x18);	 // 配置陀螺仪   量程为2000
	MPU6050_Write(ACCEL_CONFIG, 0x00); // 配置加速度计 量程为2g
	MPU6050_Write(INT_ENABLE, 0x01);   // 使能INT引脚采样中断
}


void MPU6050_GetData()
{
	uint8_t data_h, data_l;
	data_h = MPU6050_Read(ACCEL_XOUT_H);
	data_l = MPU6050_Read(ACCEL_XOUT_L);
	ax = data_l | (data_h << 8);

	data_h = MPU6050_Read(ACCEL_YOUT_H);
	data_l = MPU6050_Read(ACCEL_YOUT_L);
	ay = data_l | (data_h << 8);

	data_h = MPU6050_Read(ACCEL_ZOUT_H);
	data_l = MPU6050_Read(ACCEL_ZOUT_L);
	az = data_l | (data_h << 8);
	
	data_h = MPU6050_Read(GYRO_XOUT_H);
	data_l = MPU6050_Read(GYRO_XOUT_L);
	gx = data_l | (data_h << 8);

	data_h = MPU6050_Read(GYRO_YOUT_H);
	data_l = MPU6050_Read(GYRO_YOUT_L);
	gy = data_l | (data_h << 8);
	
	data_h = MPU6050_Read(GYRO_ZOUT_H);
	data_l = MPU6050_Read(GYRO_ZOUT_L);
	gz = data_l | (data_h << 8);
	
}



