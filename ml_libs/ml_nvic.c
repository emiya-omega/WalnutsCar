#include "headfile.h"

//-------------------------------------------------------
//���´������������ж� ���ں��������ڲ����� �û��������
//-------------------------------------------------------

void NVIC_Group_Config(uint8_t group)
{
	uint32_t temp,temp1;
	temp1 = ((~group)&0x07)<<8; //����AIRCR��8~10λ��ֵ ����5�����ȼ�����
	temp = SCB->AIRCR;          //��ȡԭֵ
	temp &= 0x0000F8FF;         //�ܳ�λ�ͷ���λ��� ����λ����
	temp |= temp1;              //����д�����λ
	temp |= 0x05FA0000;         //д���ܳ�
	SCB->AIRCR = temp;
}

void NVIC_init(uint8_t PreemptionPriority,uint8_t nvic_channel)
{
	uint8_t temp;
	
	NVIC_Group_Config(4);                                  //Ĭ��ѡ�����4:4λ��ռ���ȼ�+0λ�����ȼ�
	temp = NVIC->IP[nvic_channel]&0x01;                    //����ж����ȼ����üĴ����и���λ
	NVIC->IP[nvic_channel] = temp|(PreemptionPriority<<4); //д����ռ���ȼ�
	
	NVIC->ISER[nvic_channel/32] = 1<<nvic_channel%32;      //�ж�ʹ��
	
}
