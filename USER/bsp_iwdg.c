#include "bsp_iwdg.h"

void IWDG_Init(void)
{
	 //���ϵͳ�Ƿ��WWDG��λ�ָ�
    if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET) 
	 {
		// ��鸴λ��־λ
    RCC_ClearFlag();
   }
	
	  IWDG->CR = 0u;
	
	 //�����ڲ�����ʱ�Ӳ��ȴ�ʱ��׼������
    RCC_LSICmd(ENABLE);
    while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);

    //����ʱ��Ԥ��Ƶ��
    PVU_CheckStatus();
    IWDG_WriteAccessCmd(0x5555);
    IWDG_SetPrescaler(IWDG_Prescaler_32);

    //�������ؼĴ�����ֵ
    RVU_CheckStatus();
    IWDG_WriteAccessCmd(0x5555);
    IWDG_SetReload(0xfff);

    //���ز����ü�����
    IWDG_ReloadCounter();
    IWDG_Enable();
}

