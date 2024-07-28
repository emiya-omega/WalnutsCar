#include "headfile.h"

pid_t motorA;
pid_t motorB;
pid_t angle;
pid_t pos;

//void datavision_send()  // 上位机波形发送函数
//{
	// 数据包头
//	uart_sendbyte(UART_1, 0x03);
//	uart_sendbyte(UART_1, 0xfc);

	// 发送数据
//	uart_sendbyte(UART_1, (uint8_t)motorA.target);  
//	uart_sendbyte(UART_1, (uint8_t)motorA.now);
//	uart_sendbyte(UART_1, (uint8_t)motorB.target);  
//	uart_sendbyte(UART_1, (uint8_t)motorB.now);
	// 数据包尾
//	uart_sendbyte(UART_1, 0xfc);
//	uart_sendbyte(UART_1, 0x03);
//}

// 自定义 sign 函数
float sign(float x) {
    if (x > 0) 
	return 1.0;

    else if (x < 0)
	 return -1.0;
	else
	return 0;
}
void pid_init(pid_t *pid, uint32_t mode, float p, float i, float d)
{
	pid->pid_mode = mode;
	pid->p = p;
	pid->i = i;
	pid->d = d;
}

/*void motor_target_set(int spe1, int spe2)
{
	if(spe1 >= 0)
	{
		motorA_dir = 1;
		motorA.target = spe1;
	}
	else
	{
		motorA_dir = 0;
		motorA.target = -spe1;
	}
	
	if(spe2 >= 0)
	{
		motorB_dir = 1;
		motorB.target = spe2;
	}
	else
	{
		motorB_dir = 0;
		motorB.target = -spe2;
	}
}*/



void pid_control()
{
	// 角度环
	// 1.设定目标角度
	//angle.target = -20;
	// 2.获取当前角度
	//angle.now = yaw_Kalman;
	// 3.PID控制器计算输出
	//pid_cal(&angle);
	
	//位置环
	//设定目标位置
	float value = atof(Serial_RxPacket);
	float direction = value - 320;
	if (fabs(direction) > 250)
    {   motorA_duty(0);
        motorB_duty(0);
	}
	pos.target=320;
	
	pos.now = value;
	pid_cal_pos(&pos);


	// 速度环
	// 1.根据灰度传感器信息 设定目标速度
	track(&pos);
	// 1.角度环PID输出 设定为速度环的目标值
	//motor_target_set(-angle.out, angle.out);
	// 2.获取当前速度
	/*if(motorA_dir){motorA.now = Encoder_count1;}else{motorA.now = -Encoder_count1;}
	if(motorB_dir){motorB.now = Encoder_count2;}else{motorB.now = -Encoder_count2;}
	Encoder_count1 = 0;
	Encoder_count2 = 0;
	// 3.输入PID控制器进行计算
	pid_cal(&motorA);
	pid_cal(&motorB);
	// 电机输出限幅
	pidout_limit(&motorA);
	pidout_limit(&motorB);
	// 4.PID的输出值 输入给电机
	motorA_duty(motorA.out);
	motorB_duty(motorB.out);
	*/
//	datavision_send();
	
}
void pid_cal_pos(pid_t *pid)
{
	// 更新pid误差
	pid->error[0]=pid->error[1];
	pid->error[1]=pid->error[2];
	pid->error[2] = pid->target - pid->now;
	//更新pid输出
	pid->adjust[0]=pid->adjust[1];
	pid->adjust[1]=pid->adjust[2];
	//pid->adjust[2]=pid->adjust[1]+pid->p*(pid->error[2]-pid->error[1])+pid->i*pid->error[2]+pid->d*(pid->error[2]-2*pid->error[1]+pid->error[0]);
	//这样写如果有问题用上面这段
	pid->adjust[2] = pid->adjust[1] \
        + pid->p*(pid->error[2] - pid->error[1]) \
        + pid->i*pid->error[2] \
        + pid->d*(pid->error[2] - 2*pid->error[1] + pid->error[0]); 
 
	//限制在70绝对值之内
	if (fabs(pid->adjust[2]) > 70) {
        pid->adjust[2] = sign(pid->adjust[2]) * 70;
    }
	
}
/*void pid_cal(pid_t *pid)
{
	// 计算当前偏差
	pid->error[0] = pid->target - pid->now;

	// 计算输出
	if(pid->pid_mode == DELTA_PID)  // 增量式
	{
		pid->pout = pid->p * (pid->error[0] - pid->error[1]);
		pid->iout = pid->i * pid->error[0];
		pid->dout = pid->d * (pid->error[0] - 2 * pid->error[1] + pid->error[2]);
		pid->out += pid->pout + pid->iout + pid->dout;
	}
	else if(pid->pid_mode == POSITION_PID)  // 位置式
	{
		pid->pout = pid->p * pid->error[0];
		pid->iout += pid->i * pid->error[0];
		pid->dout = pid->d * (pid->error[0] - pid->error[1]);
		pid->out = pid->pout + pid->iout + pid->dout;
	}

	// 记录前两次偏差
	pid->error[2] = pid->error[1];
	pid->error[1] = pid->error[0];

	// 输出限幅
//	if(pid->out>=MAX_DUTY)	
//		pid->out=MAX_DUTY;
//	if(pid->out<=0)	
//		pid->out=0;
	
}*/

void pidout_limit(pid_t *pid)
{
	// 输出限幅
	if(pid->out>=MAX_DUTY)	
		pid->out=MAX_DUTY;
	if(pid->out<=0)	
		pid->out=0;
}


