#include "bsp_tim.h"

void TIM_GPIO_Config()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);
	
	GPIO_PinAFConfig(TIM_CH1_GPIO_PORT,GPIO_PinSource8,GPIO_AF_2);    //复用功能配置
	GPIO_PinAFConfig(TIM_CH2_GPIO_PORT,GPIO_PinSource9,GPIO_AF_2);
	GPIO_PinAFConfig(TIM_CH3_GPIO_PORT,GPIO_PinSource10,GPIO_AF_2);
	GPIO_PinAFConfig(TIM_CH4_GPIO_PORT,GPIO_PinSource11,GPIO_AF_2);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;          //复用推挽模式
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
	
	/*时基结构体配置*/
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV4;        //时钟分频
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;    //向上计数模式
	TIM_TimeBaseInitStruct.TIM_Period = 1024;                       //周期
	TIM_TimeBaseInitStruct.TIM_Prescaler = (1-1);                   //预分频系数
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;               //重复计数器的值
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStruct);                //时基结构体初始化
	
	/*PWM输出结构体配置*/
	TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Set;         //输出通道空闲电平极性配置
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;                  //PWM1模式
	TIM_OCInitStruct.TIM_OCNIdleState = TIM_OCNIdleState_Reset;     //互补输出通道空闲电平极性
	TIM_OCInitStruct.TIM_OCNPolarity = TIM_OCNPolarity_Low;         //互补输出通道电平极性配置
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;          //输出通道电平极性配置
	TIM_OCInitStruct.TIM_OutputNState = TIM_OutputNState_Disable;   //互补输出通道关闭
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;      //输出通道使能
	TIM_OCInitStruct.TIM_Pulse = 0;                                 //脉宽
	
	TIM_OC1Init(TIM1, &TIM_OCInitStruct);                           //初始化TIM通道1结构体配置（开启通道）
	TIM_OC2Init(TIM1, &TIM_OCInitStruct);                           //初始化TIM通道2结构体配置（开启通道）
	TIM_OC3Init(TIM1, &TIM_OCInitStruct);                           //初始化TIM通道3结构体配置（开启通道）
	TIM_OC4Init(TIM1, &TIM_OCInitStruct);                           //初始化TIM通道4结构体配置（开启通道）
	
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);               //使能TIM通道2上的预加载寄存器（不能直接写入寄存器的值，等待事件更新）
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM1, ENABLE);                             //启用外围预加载寄存器
	TIM_CtrlPWMOutputs(TIM1, ENABLE);                               //启用TIM外设主要输出
	TIM_Cmd(TIM1, ENABLE);                                          //使能计数器
}

void WritePWM(TIM_TypeDef* TIMx,unsigned char channel,unsigned short pulseValue)   //输出PWM波形
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


