#include "headfile.h"
#include "motor.h"
#include "pid.h"

/*void gray_init()
{
	gpio_init(GPIO_B, Pin_12, IU);   // D1
	gpio_init(GPIO_B, Pin_13, IU);   // D2
	gpio_init(GPIO_B, Pin_14, IU);   // D3
	gpio_init(GPIO_B, Pin_15, IU);   // D4
	gpio_init(GPIO_A, Pin_8, IU);    // D5
	gpio_init(GPIO_C, Pin_13, IU);   // D6
	gpio_init(GPIO_C, Pin_14, IU);   // D7
	gpio_init(GPIO_C, Pin_15, IU);   // D8
}
*/

void track(pid_t *pid)                        //指明&pos
{	
	if (pid->adjust[2] > 0)//右转
	{
		motorB_duty(30);
		motorA_duty(30+pid->adjust[2]);
		
		    
		
	}
	else if(pid->adjust[2]<0)//turn left
	{
		motorA_duty(30);
		motorB_duty(30-pid->adjust[2]);
	}
	else if(pid->adjust[2]==0)//go straight
	{
		motorA_duty(30);
		motorB_duty(30);
	}
	/*else if(pid->adjust[2]==-0.5)//go straight
	{
		motor_target_set(30,30);
	}*/
	/*else if(pid->adjust[2]==0)//g s
	{
		motorA_duty(int duty)
		motorB_duty(int duty)
		//motor_target_set(5,5);
	}*/


		
}

