#include "bsp_tim.h"

void TIM_GPIO_Config()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);
	
	GPIO_PinAFConfig(TIM_CH1_GPIO_PORT,GPIO_PinSource8,GPIO_AF_2);    //���ù�������
	GPIO_PinAFConfig(TIM_CH2_GPIO_PORT,GPIO_PinSource9,GPIO_AF_2);
	GPIO_PinAFConfig(TIM_CH3_GPIO_PORT,GPIO_PinSource10,GPIO_AF_2);
	GPIO_PinAFConfig(TIM_CH4_GPIO_PORT,GPIO_PinSource11,GPIO_AF_2);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;          //��������ģʽ
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
 	GPIO_InitStruct.GPIO_Pin = TIM_CH1_GPIO_PIN;
	GPIO_Init(TIM_CH1_GPIO_PORT,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = TIM_CH2_GPIO_PIN;
	GPIO_Init(TIM_CH2_GPIO_PORT,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = TIM_CH3_GPIO_PIN;
	GPIO_Init(TIM_CH3_GPIO_PORT,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = TIM_CH4_GPIO_PIN;
	GPIO_Init(TIM_CH4_GPIO_PORT,&GPIO_InitStruct);
}

void TIM_MODE_Config()
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	
	/*ʱ���ṹ������*/
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV4;        //ʱ�ӷ�Ƶ
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;    //���ϼ���ģʽ
	TIM_TimeBaseInitStruct.TIM_Period = 1024;                       //����
	TIM_TimeBaseInitStruct.TIM_Prescaler = (1-1);                   //Ԥ��Ƶϵ��
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;               //�ظ���������ֵ
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStruct);                //ʱ���ṹ���ʼ��
	
	/*PWM����ṹ������*/
	TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Set;         //���ͨ�����е�ƽ��������
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;                  //PWM1ģʽ
	TIM_OCInitStruct.TIM_OCNIdleState = TIM_OCNIdleState_Reset;     //�������ͨ�����е�ƽ����
	TIM_OCInitStruct.TIM_OCNPolarity = TIM_OCNPolarity_Low;         //�������ͨ����ƽ��������
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;          //���ͨ����ƽ��������
	TIM_OCInitStruct.TIM_OutputNState = TIM_OutputNState_Disable;   //�������ͨ���ر�
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;      //���ͨ��ʹ��
	TIM_OCInitStruct.TIM_Pulse = 0;                                 //����
	
	TIM_OC1Init(TIM1, &TIM_OCInitStruct);                           //��ʼ��TIMͨ��1�ṹ�����ã�����ͨ����
	TIM_OC2Init(TIM1, &TIM_OCInitStruct);                           //��ʼ��TIMͨ��2�ṹ�����ã�����ͨ����
	TIM_OC3Init(TIM1, &TIM_OCInitStruct);                           //��ʼ��TIMͨ��3�ṹ�����ã�����ͨ����
	TIM_OC4Init(TIM1, &TIM_OCInitStruct);                           //��ʼ��TIMͨ��4�ṹ�����ã�����ͨ����
	
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);               //ʹ��TIMͨ��2�ϵ�Ԥ���ؼĴ���������ֱ��д��Ĵ�����ֵ���ȴ��¼����£�
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM1, ENABLE);                             //������ΧԤ���ؼĴ���
	TIM_CtrlPWMOutputs(TIM1, ENABLE);                               //����TIM������Ҫ���
	TIM_Cmd(TIM1, ENABLE);                                          //ʹ�ܼ�����
}

void WritePWM(TIM_TypeDef* TIMx,unsigned char channel,unsigned short pulseValue)   //���PWM����
{
	if(channel > 4)
	{
		return ;
	}
	else
	{
		switch(channel)
		{
			case (1) :
			{
				if((TIMx->CCR1)&&(0x0000ffff) == pulseValue)
				{
					return ;
				}
				TIMx->CCR1 = pulseValue;
			  break;
			}
			case (2) :
			{
				if((TIMx->CCR2)&&(0x0000ffff) == pulseValue)
				{
					return ;
				}
				TIMx->CCR2 = pulseValue;
				break;
			}
			case (3) :
			{
				if((TIMx->CCR3)&&(0x0000ffff) == pulseValue)
				{
					return ;
				}
				TIMx->CCR3 = pulseValue;
				break;
			}
			case (4) :
			{
				if((TIMx->CCR4)&&(0x0000ffff) == pulseValue)
				{
					return ;
				}
				TIMx->CCR4 = pulseValue;
				break;
			}
			
			default : break;
		}
	}
}


void PWM_Output_Config(void)
{
	TIM_GPIO_Config();
	TIM_MODE_Config();
}


