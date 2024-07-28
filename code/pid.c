#include "headfile.h"

pid_t motorA;
pid_t motorB;
pid_t angle;
pid_t pos;

//void datavision_send()  // ��λ�����η��ͺ���
//{
	// ���ݰ�ͷ
//	uart_sendbyte(UART_1, 0x03);
//	uart_sendbyte(UART_1, 0xfc);

	// ��������
//	uart_sendbyte(UART_1, (uint8_t)motorA.target);  
//	uart_sendbyte(UART_1, (uint8_t)motorA.now);
//	uart_sendbyte(UART_1, (uint8_t)motorB.target);  
//	uart_sendbyte(UART_1, (uint8_t)motorB.now);
	// ���ݰ�β
//	uart_sendbyte(UART_1, 0xfc);
//	uart_sendbyte(UART_1, 0x03);
//}

// �Զ��� sign ����
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
	// �ǶȻ�
	// 1.�趨Ŀ��Ƕ�
	//angle.target = -20;
	// 2.��ȡ��ǰ�Ƕ�
	//angle.now = yaw_Kalman;
	// 3.PID�������������
	//pid_cal(&angle);
	
	//λ�û�
	//�趨Ŀ��λ��
	float value = atof(Serial_RxPacket);
	float direction = value - 320;
	if (fabs(direction) > 250)
    {   motorA_duty(0);
        motorB_duty(0);
	}
	pos.target=320;
	
	pos.now = value;
	pid_cal_pos(&pos);


	// �ٶȻ�
	// 1.���ݻҶȴ�������Ϣ �趨Ŀ���ٶ�
	track(&pos);
	// 1.�ǶȻ�PID��� �趨Ϊ�ٶȻ���Ŀ��ֵ
	//motor_target_set(-angle.out, angle.out);
	// 2.��ȡ��ǰ�ٶ�
	/*if(motorA_dir){motorA.now = Encoder_count1;}else{motorA.now = -Encoder_count1;}
	if(motorB_dir){motorB.now = Encoder_count2;}else{motorB.now = -Encoder_count2;}
	Encoder_count1 = 0;
	Encoder_count2 = 0;
	// 3.����PID���������м���
	pid_cal(&motorA);
	pid_cal(&motorB);
	// �������޷�
	pidout_limit(&motorA);
	pidout_limit(&motorB);
	// 4.PID�����ֵ ��������
	motorA_duty(motorA.out);
	motorB_duty(motorB.out);
	*/
//	datavision_send();
	
}
void pid_cal_pos(pid_t *pid)
{
	// ����pid���
	pid->error[0]=pid->error[1];
	pid->error[1]=pid->error[2];
	pid->error[2] = pid->target - pid->now;
	//����pid���
	pid->adjust[0]=pid->adjust[1];
	pid->adjust[1]=pid->adjust[2];
	//pid->adjust[2]=pid->adjust[1]+pid->p*(pid->error[2]-pid->error[1])+pid->i*pid->error[2]+pid->d*(pid->error[2]-2*pid->error[1]+pid->error[0]);
	//����д������������������
	pid->adjust[2] = pid->adjust[1] \
        + pid->p*(pid->error[2] - pid->error[1]) \
        + pid->i*pid->error[2] \
        + pid->d*(pid->error[2] - 2*pid->error[1] + pid->error[0]); 
 
	//������70����ֵ֮��
	if (fabs(pid->adjust[2]) > 70) {
        pid->adjust[2] = sign(pid->adjust[2]) * 70;
    }
	
}
/*void pid_cal(pid_t *pid)
{
	// ���㵱ǰƫ��
	pid->error[0] = pid->target - pid->now;

	// �������
	if(pid->pid_mode == DELTA_PID)  // ����ʽ
	{
		pid->pout = pid->p * (pid->error[0] - pid->error[1]);
		pid->iout = pid->i * pid->error[0];
		pid->dout = pid->d * (pid->error[0] - 2 * pid->error[1] + pid->error[2]);
		pid->out += pid->pout + pid->iout + pid->dout;
	}
	else if(pid->pid_mode == POSITION_PID)  // λ��ʽ
	{
		pid->pout = pid->p * pid->error[0];
		pid->iout += pid->i * pid->error[0];
		pid->dout = pid->d * (pid->error[0] - pid->error[1]);
		pid->out = pid->pout + pid->iout + pid->dout;
	}

	// ��¼ǰ����ƫ��
	pid->error[2] = pid->error[1];
	pid->error[1] = pid->error[0];

	// ����޷�
//	if(pid->out>=MAX_DUTY)	
//		pid->out=MAX_DUTY;
//	if(pid->out<=0)	
//		pid->out=0;
	
}*/

void pidout_limit(pid_t *pid)
{
	// ����޷�
	if(pid->out>=MAX_DUTY)	
		pid->out=MAX_DUTY;
	if(pid->out<=0)	
		pid->out=0;
}


