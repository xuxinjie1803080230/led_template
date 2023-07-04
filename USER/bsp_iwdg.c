#include "bsp_iwdg.h"

void IWDG_Init(void)
{
	 //检查系统是否从WWDG复位恢复
    if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET) 
	 {
		// 检查复位标志位
    RCC_ClearFlag();
   }
	
	  IWDG->CR = 0u;
	
	 //启动内部低速时钟并等待时钟准备就绪
    RCC_LSICmd(ENABLE);
    while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);

    //设置时钟预分频率
    PVU_CheckStatus();
    IWDG_WriteAccessCmd(0x5555);
    IWDG_SetPrescaler(IWDG_Prescaler_32);

    //设置重载寄存器的值
    RVU_CheckStatus();
    IWDG_WriteAccessCmd(0x5555);
    IWDG_SetReload(0xfff);

    //加载并启用计数器
    IWDG_ReloadCounter();
    IWDG_Enable();
}

