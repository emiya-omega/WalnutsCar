#ifndef _filter_h
#define _filter_h
#include "math.h"

typedef struct
{
	float Q_angle; // ��������-�Ƕȷ���
	float Q_bias;  // ��������-��Ư����
	float R;       // ��������(���ٶȼ�/������)
	float P[2][2]; // ���Э�������
	float dt;      // ����ʱ��
	float K1, K2;  // ����������

	
	float Angle;     // �Ƕȹ���ֵ
	float Gyro_bias; // �����Ǿ������ֵ

}KF_t;

float Mahony_Filter(float gyro, float acc);
float Kalman_Filter(KF_t *kf, float obsValue, float ut);

extern KF_t KF_Yaw, KF_Roll, KF_Pitch;

#endif
