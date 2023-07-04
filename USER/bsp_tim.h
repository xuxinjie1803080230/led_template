#ifndef __BSP_TIM_H
#define __BSP_TIM_H

#include "hal_conf.h"

#define TIM_CH1_GPIO_PORT  GPIOA
#define TIM_CH1_GPIO_PIN   GPIO_Pin_8

#define TIM_CH2_GPIO_PORT  GPIOA
#define TIM_CH2_GPIO_PIN   GPIO_Pin_9

#define TIM_CH3_GPIO_PORT  GPIOA
#define TIM_CH3_GPIO_PIN   GPIO_Pin_10

#define TIM_CH4_GPIO_PORT  GPIOA
#define TIM_CH4_GPIO_PIN   GPIO_Pin_11

void PWM_Output_Config(void);
void WritePWM(TIM_TypeDef* TIMx,unsigned char channel,unsigned short pulseValue);
#endif /*__BSP_TIM_H*/
