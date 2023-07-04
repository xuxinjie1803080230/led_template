////////////////////////////////////////////////////////////////////////////////
/// @file    led.h
/// @author  AE TEAM
/// @brief   THIS FILE PROVIDES ALL THE SYSTEM FIRMWARE FUNCTIONS.
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
#ifndef __LED_H
#define __LED_H

// Files includes
#include <string.h>

#include "mm32_device.h"
#include "hal_conf.h"




////////////////////////////////////////////////////////////////////////////////
/// @defgroup MM32_Example_Layer
/// @brief MM32 Example Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup MM32_RESOURCE
/// @brief MM32 Examples resource modules
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup MM32_Exported_Constants
/// @{

#define LED0_EN_PORT         GPIOB
#define LED0_EN_PIN          GPIO_Pin_15

#define LED2_EN_PORT         GPIOB
#define LED2_EN_PIN          GPIO_Pin_12

#define LED3_EN_PORT         GPIOB
#define LED3_EN_PIN          GPIO_Pin_13

#define DD_EN_PORT           GPIOB
#define DD_EN_PIN            GPIO_Pin_14

#define LED1_EN_PORT         GPIOC
#define LED1_EN_PIN          GPIO_Pin_13

#define LEDPORTPIN   ((unsigned short)(0x00FF))


#pragma pack(1)
typedef struct
{
	signed short Rxpwr1;             //���չ⹦��1  ��λ: 0.1dbm
	signed short Rxpwr2;             //���չ⹦��2  ��λ��0.1dbm
	signed short Txpwr1;             //����⹦��1  ��λ��0.1dbm
	signed short Txpwr2;             //����⹦��2  ��λ��0.1dbm
	signed short TxCurt1;            //�������1    ��λ��0.1ma
  signed short TxCurt2;            //�������2    ��λ��0.1ma	
	unsigned short AD_8V;            //��ѹ         ��λ��0.01v
	signed short Op_Pwr1;            //1·�����ƽ  ��λ��0.1dbuv
	signed short Op_Pwr2;            //2·�����ƽ  ��λ0.1dbuv
	signed short temperature;        //�¶�
	unsigned char ATT_A;             //A·ATT
  unsigned char EQ_A;              //A·EQ
  unsigned char ATT_B;             //B·ATT
  unsigned char EQ_B;              //B·EQ
	unsigned char Channel_Num;       //Ƶ����   	
	
	unsigned char FSK_Value;
}CommAllPare_stru;;
#pragma pack()



#define LED0_ON()    GPIO_ResetBits(LED0_EN_PORT,LED0_EN_PIN)
#define LED0_OFF()    GPIO_SetBits(LED0_EN_PORT,LED0_EN_PIN)
#define LED0_TOGGLE()    (GPIO_ReadOutputDataBit(LED0_EN_PORT,LED0_EN_PIN))?(GPIO_ResetBits(LED0_EN_PORT,LED0_EN_PIN)):(GPIO_SetBits(LED0_EN_PORT,LED0_EN_PIN))

#define LED1_ON()    GPIO_ResetBits(LED1_EN_PORT,LED1_EN_PIN)
#define LED1_OFF()    GPIO_SetBits(LED1_EN_PORT,LED1_EN_PIN)

#define LED2_ON()    GPIO_ResetBits(LED2_EN_PORT,LED2_EN_PIN)
#define LED2_OFF()    GPIO_SetBits(LED2_EN_PORT,LED2_EN_PIN)

#define LED3_ON()    GPIO_ResetBits(LED3_EN_PORT,LED3_EN_PIN)
#define LED3_OFF()    GPIO_SetBits(LED3_EN_PORT,LED3_EN_PIN)

#define DD_ON()      GPIO_ResetBits(DD_EN_PORT,DD_EN_PIN)
#define DD_OFF()     GPIO_SetBits(DD_EN_PORT,DD_EN_PIN)




void LED_Init(void);
void WriteLedPort(uint8_t value);
void TestLED(void);
void LEDDisp(void);
void ProDisp(void);
void CpyData(void);
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
