#include "bsp_adc.h"

u16 ADC1_SingleChannel_Get(void)
{
    u16 uiADData;
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);              //软件开始转换
    ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
		while(ADC_GetFlagStatus(ADC1, ADC_IT_EOC)== 1)       //判断转换是否完成                      
		{
			ADC_ClearFlag(ADC1, ADC_IT_EOC);                   //清除中断标志位
			uiADData = ADC_GetConversionValue(ADC1);
		}
    return uiADData;
}

u16 Get_Adc_Average(u8 times)        //计算电压平均值                 
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
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;       //模拟输入
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
	//ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;          //（可省略）
	ADC_InitStruct.ADC_DataAlign = ADC_CR_RIGHT;             //右对齐
	ADC_InitStruct.ADC_ExternalTrigConv = ADC1_ExternalTrigConv_T1_CC1;     //外部触发源选择
	ADC_InitStruct.ADC_Mode = ADC_Mode_Imm;                  //ADC模式为单次转换
	ADC_InitStruct.ADC_PRESCARE = ADC_PCLK2_PRESCARE_16;     //预分频系数
	ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;      //分辨率12位
	ADC_Init(ADC1, &ADC_InitStruct);                         //ADC初始化
	
	ADC_ExternalTrigConvCmd(ADC1, DISABLE);                  //外部触发关闭
	ADCxSampleTimeConfig(ADC1,ADC_Samctl_28_5);              //采样时间配置为28.5个时钟周期
	ADC_ANY_Cmd(ADC1, DISABLE);                              //ADC任意通道模式关闭
	ADC_DMACmd(ADC1, DISABLE);                               //DMA模式关闭
	ADC_ITConfig(ADC1,ADC_IT_EOC, DISABLE);                  //ADC转换中断标志位关闭
	ADC_ITConfig(ADC1,ADC_IT_AWD, DISABLE);                  //ADC窗口比较中断标志位关闭
	ADC_Cmd(ADC1, ENABLE);                                   //使能ADC
  											
}
u16 ADConversion(ADCCHANNEL_TypeDef ADC_Channel)
{
	u16 AdconversionValue;
	ADC1->ADCHS = 0u;                  //先关闭所有通道
	ADCxChannelEnable(ADC1, ADC_Channel);        //打开所需的通道
  AdconversionValue = Get_Adc_Average(5);            //计算平均电压值
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

