#include "bsp_adc.h"

u16 ADC1_SingleChannel_Get(void)
{
    u16 uiADData;
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);              //�����ʼת��
    ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
		while(ADC_GetFlagStatus(ADC1, ADC_IT_EOC)== 1)       //�ж�ת���Ƿ����                      
		{
			ADC_ClearFlag(ADC1, ADC_IT_EOC);                   //����жϱ�־λ
			uiADData = ADC_GetConversionValue(ADC1);
		}
    return uiADData;
}

u16 Get_Adc_Average(u8 times)        //�����ѹƽ��ֵ                 
{
    u32 temp_val = 0;
    u8 t;
    u8 delay;
    for(t = 0; t < times; t++) {
        temp_val += ADC1_SingleChannel_Get();
        for(delay = 0; delay < 100; delay++);
    }
    return temp_val / times;
}


void ADC_GPIO_Config()
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;       //ģ������
	GPIO_InitStruct.GPIO_Pin = ADC_RXPWR1_PIN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(ADC_RXPWR1_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = ADC_RXPWR2_PIN;
	GPIO_Init(ADC_RXPWR2_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = ADC_TXPWR1_PIN;
	GPIO_Init(ADC_TXPWR1_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = ADC_TXPWR2_PIN;
	GPIO_Init(ADC_TXPWR2_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = ADC_TXCURT_PIN;
	GPIO_Init(ADC_TXCURT_PORT, &GPIO_InitStruct);
}
void ADC_Mode_Config()
{
	ADC_InitTypeDef   ADC_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	//ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;          //����ʡ�ԣ�
	ADC_InitStruct.ADC_DataAlign = ADC_CR_RIGHT;             //�Ҷ���
	ADC_InitStruct.ADC_ExternalTrigConv = ADC1_ExternalTrigConv_T1_CC1;     //�ⲿ����Դѡ��
	ADC_InitStruct.ADC_Mode = ADC_Mode_Imm;                  //ADCģʽΪ����ת��
	ADC_InitStruct.ADC_PRESCARE = ADC_PCLK2_PRESCARE_16;     //Ԥ��Ƶϵ��
	ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;      //�ֱ���12λ
	ADC_Init(ADC1, &ADC_InitStruct);                         //ADC��ʼ��
	
	ADC_ExternalTrigConvCmd(ADC1, DISABLE);                  //�ⲿ�����ر�
	ADCxSampleTimeConfig(ADC1,ADC_Samctl_28_5);              //����ʱ������Ϊ28.5��ʱ������
	ADC_ANY_Cmd(ADC1, DISABLE);                              //ADC����ͨ��ģʽ�ر�
	ADC_DMACmd(ADC1, DISABLE);                               //DMAģʽ�ر�
	ADC_ITConfig(ADC1,ADC_IT_EOC, DISABLE);                  //ADCת���жϱ�־λ�ر�
	ADC_ITConfig(ADC1,ADC_IT_AWD, DISABLE);                  //ADC���ڱȽ��жϱ�־λ�ر�
	ADC_Cmd(ADC1, ENABLE);                                   //ʹ��ADC
  											
}
u16 ADConversion(ADCCHANNEL_TypeDef ADC_Channel)
{
	u16 AdconversionValue;
	ADC1->ADCHS = 0u;                  //�ȹر�����ͨ��
	ADCxChannelEnable(ADC1, ADC_Channel);        //�������ͨ��
  AdconversionValue = Get_Adc_Average(5);            //����ƽ����ѹֵ
	return AdconversionValue;
}

void ADC_Initialize(void)
{
	unsigned int cnt;
	ADC_Cmd(ADC1,DISABLE);
	for(cnt=0;cnt<100;cnt++);
	ADC_GPIO_Config();
	ADC_Mode_Config();
	for(cnt=0;cnt<100;cnt++);
	ADCxChannelEnable(ADC1,ADC_Channel_7);
	cnt = ADC1_SingleChannel_Get();
}

