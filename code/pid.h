#ifndef __PID_h_
#define __PID_h_
#include "headfile.h"

enum
{
  POSITION_PID = 0,  // λ��ʽ
  DELTA_PID,         // ����ʽ
};

typedef struct
{
	float target;	
	float now;
	float error[3];		
	float p,i,d;
	float pout, dout, iout;
	float out;  
	float adjust[3]; 
	
	uint32_t pid_mode;

}pid_t;
float sign(float x);
void pid_cal(pid_t *pid);	23
void pid_cal_pos(pid_t *pid);
void pid_control(void);
void pid_init(pid_t *pid, uint32_t mode, float p, float i, float d);
void motor_target_set(int spe1, int spe2);
void pidout_limit(pid_t *pid);

extern pid_t motorA;
extern pid_t motorB;
extern pid_t angle;
extern pid_t pos;
#endif
