////////////////////////////////////////////////////////////////////////////////
/// @file    key.h
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
#ifndef __KEY_H
#define __KEY_H

// Files includes
#include <string.h>
#include "mm32_device.h"
#include "hal_conf.h"


////////////////////////////////////////////////////////////////////////////////
/// @defgroup MM32_Example_Layer
/// @brief MM32 Example Layer

#define KEY1_GPIO_Port                  GPIOA
#define KEY1_Pin                        GPIO_Pin_12

#define KEY2_GPIO_Port                  GPIOD
#define KEY2_Pin                        GPIO_Pin_2
#define KEY3_GPIO_Port                  GPIOD
#define KEY3_Pin                        GPIO_Pin_3


#define KEY_UP_PRES                 1                                               //KEY1
#define KEY_DOWN_PRES               2                                               //KEY2
#define KEY_MODE_PRES               3                                               //KEY3

#define KEYDELAY    150   //°´¼üÑÓÊ±
#define KEYDELAY2   300

void KEY_Init(void);
u8 KEY_Scan(void);
void ProKey(uint8_t keynum);
void Pro_Key(void);
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
