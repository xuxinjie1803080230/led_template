#ifndef __BSP_ADC_H
#define __BSP_ADC_H

#include <string.h>
#include "mm32_device.h"
#include "hal_conf.h"


#define ADC_RXPWR1_PIN   GPIO_Pin_3
#define ADC_RXPWR1_PORT  GPIOA

#define ADC_RXPWR2_PIN   GPIO_Pin_4
#define ADC_RXPWR2_PORT  GPIOA

#define ADC_TXPWR1_PIN   GPIO_Pin_5
#define ADC_TXPWR1_PORT  GPIOA

#define ADC_TXPWR2_PIN   GPIO_Pin_6
#define ADC_TXPWR2_PORT  GPIOA

#define ADC_TXCURT_PIN   GPIO_Pin_7
#define ADC_TXCURT_PORT  GPIOA


void ADC_Initialize(void);
u16 ADConversion(ADCCHANNEL_TypeDef ADC_Channel);
#endif 
