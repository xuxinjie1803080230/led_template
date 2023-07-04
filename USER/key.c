////////////////////////////////////////////////////////////////////////////////
/// @file    key.c
/// @author  AE TEAM
/// @brief   key intput.
////////////////////////////////////////////////////////////////////////////////
/// @attention
///
/// THE EXISTING FIRMWARE IS ONLY FOR REFERENCE, WHICH IS DESIGNED TO PROVIDE
/// CUSTOMERS WITH CODING INFORMATION ABOUT THEIR PRODUCTS SO THEY CAN SAVE
/// TIME. THEREFORE, MINDMOTION SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT OR
/// CONSEQUENTIAL DAMAGES ABOUT ANY CLAIMS ARISING OUT OF THE CONTENT OF SUCH
/// HARDWARE AND/OR THE USE OF THE CODING INFORMATION CONTAINED HEREIN IN
/// CONNECTION WITH PRODUCTS MADE BY CUSTOMERS.
///
/// <H2><CENTER>&COPY; COPYRIGHT MINDMOTION </CENTER></H2>
////////////////////////////////////////////////////////////////////////////////

// Define to prevent recursive inclusion
#define _KEY_C_

// Files includes
#include "key.h"
#include "delay.h"
#include "led.h"


volatile unsigned int MainMenu;
volatile unsigned int SubMenu;
unsigned char KeyCnt1 = 0;
unsigned char KeyCnt2 = 0;

////////////////////////////////////////////////////////////////////////////////
/// @brief  Keyboard Scanning
/// @note   None.
/// @param  None.
/// @retval Keyboard value.
////////////////////////////////////////////////////////////////////////////////
u8 KEY_Scan(void)
{
	if(GPIO_ReadInputDataBit(KEY1_GPIO_Port,KEY1_Pin)==Bit_RESET)
	{
		return KEY_UP_PRES;
	}
	if(GPIO_ReadInputDataBit(KEY2_GPIO_Port,KEY2_Pin)==Bit_RESET)
	{
		return KEY_DOWN_PRES;
	}
	if(GPIO_ReadInputDataBit(KEY3_GPIO_Port,KEY3_Pin)==Bit_RESET)
	{
		return KEY_MODE_PRES;
	}
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  initialize KEY GPIO pin
/// @note   if use jtag/swd interface GPIO PIN as KEY, need to be careful, can not debug or program.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void KEY_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOD, ENABLE);

    GPIO_InitStructure.GPIO_Pin  = KEY1_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(KEY1_GPIO_Port, &GPIO_InitStructure);
	
    GPIO_InitStructure.GPIO_Pin  = KEY2_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(KEY2_GPIO_Port, &GPIO_InitStructure);
	
	  GPIO_InitStructure.GPIO_Pin  = KEY3_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(KEY3_GPIO_Port, &GPIO_InitStructure);

}

void ProKey(uint8_t keynum)
{
	if(keynum == KEY_UP_PRES)
	{
		SubMenu++;
		if(SubMenu >= 6)
		{
			SubMenu = 0;
		}
	}
	else if(keynum == KEY_DOWN_PRES)
	{

		if(SubMenu <= 0)
		{
			SubMenu = 6;
		}
		SubMenu--;
	}
	else if(keynum == KEY_MODE_PRES)
	{
		SubMenu = 0;
		MainMenu++;
		if(MainMenu >= 2)
		{
			MainMenu = 0;
		}
	}
}

void Pro_Key(void)
{
	unsigned char keynum;
	
	if(KeyCnt1 < KEYDELAY)
	{
		KeyCnt1++;
	}
	if((keynum = KEY_Scan()) == 0)    //没有按键按下
	{
		KeyCnt1 = 0;
	}
	if(KeyCnt1 ==1)
	{
		if(keynum != 0)
		{
			ProKey(keynum);
		}
		else 
		{
			KeyCnt1 = 0;
		}
	}
	
}




/// @}

