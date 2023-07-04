////////////////////////////////////////////////////////////////////////////////
/// @file    led.c
/// @author  AE TEAM
/// @brief   THIS FILE PROVIDES ALL THE SYSTEM FUNCTIONS.
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
#define _LED_C_

// Files includes

#include "led.h"

extern volatile unsigned int TestDlyCnt ;
extern volatile unsigned int MainMenu;
extern volatile unsigned int SubMenu;
extern volatile unsigned int systemtick; 
volatile unsigned int CpyDlyCnt;
volatile unsigned int CPYTIMER = 240;  //����������ʱ�Ĵ���ֵ
extern CommAllPare_stru AllPare;
extern volatile unsigned char TxzeroFlag1;     //����⹦��Ϊ0��־
extern volatile unsigned char TxzeroFlag2;
extern volatile unsigned char TxoverFlag1;     //����⹦�ʹ��߱�־
extern volatile unsigned char TxoverFlag2;
uint8_t LED_Buff[5];
uint8_t LED_Temp[5];
uint8_t LEDON = 1;            //0:�ر�������ʾ  1����������ʾ
extern volatile unsigned char RxzeroFlag1;     //���չ⹦��Ϊ0��־
extern volatile unsigned char RxzeroFlag2;
extern volatile unsigned char RxoverFlag1;     //���չ⹦�ʹ��߱�־
extern volatile unsigned char RxoverFlag2; 

const static unsigned char smg[] = 
{
     //���Ҫ��ʾС���㣬��ֵ��ȥ0x80
    0xc0,//*0
    0xf9,//*1
    0xa4,//*2
    0xb0,//*3
    0x99,//*4
    0x92,//*5
    0x82,//*6
    0xf8,//*7
    0x80,//*8
    0x90,//*9
    0xbf,//*- ����  0xa
    0xff,//*  �ո�  0xb
    0x86,//*E ��ʾ��ĸE 0xc
    0x88,//*A ��ʾ��ĸA 0xd
    0xc7,//*L ��ʾ��ĸL 0xe
    0xa3,//*o ��ʾ��ĸo 0xf
    0x8c,//*P ��ʾ��ĸP 0x10
    0x89,//*H ��ʾ��ĸH 0x11
    0xc2,//  ��ʾG 0X12
    0xa1, // ��ʾd 0x13
    0xc6, // ��ʾC 0X14
    0xc1,   //��ʾ U 0x15
    0xab,   // ��ʾn 0x16
    0xcf ,   // ��ʾI 0X17
    0x83,    // b  0x18
    0x8e,      //��ʾF 0X19
    0xc8,   //��ʾN 0X1A
    0x87,  //��ʾt 0X1B
    0xcc    //��ʾR 0X1C
};

/*int ������ ת��Ϊ long ������*/
unsigned long BCD(unsigned int value)
{
	unsigned long i = 0;
	unsigned char j;
	j=0;
	while(value >= 10000)
	{
		value = value - 10000;
		j++;
	}
	i |= j ;
	i = i << 4;
	
  j=0;
	while(value >= 1000)
	{
		value = value - 1000;
		j++;
	}
	i |= j ;
	i = i << 4;

  j=0;
	while(value >= 100)
	{
		value = value - 100;
		j++;
	}
	i |= j ;
	i = i <<4 ;

  j=0;
	while(value >= 10)
	{
		value = value - 10;
		j++;
	}
	i |= j;
	i = i <<4;
	
	i |= value;
	return (i);
}


/* ��ʼ��LED GPIO */
void LED_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	  GPIO_InitStructure.GPIO_Pin  =  LED1_EN_PIN;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
   
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	  GPIO_InitStructure.GPIO_Pin  = LED2_EN_PIN | LED3_EN_PIN | DD_EN_PIN | LED0_EN_PIN;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	                                   
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Pin = LEDPORTPIN;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	
		WriteLedPort(0xFF);
		LED0_OFF();
		LED1_OFF();
		LED2_OFF();
		LED3_OFF();
		DD_OFF();
}






void WriteLedPort(uint8_t value)
{
	uint8_t cnt;
	for(cnt=0;cnt<8;cnt++)
	{
		if(value & 0x80)
		{
			GPIO_WriteBit(GPIOB, (0x0080 >> cnt), Bit_SET);
		}
		else
		{
			GPIO_WriteBit(GPIOB, (0x0080 >> cnt), Bit_RESET);
		}
		value <<= 1;
	}
}








void LEDDisp(void)
{
  static uint8_t LEDTICK;
	uint8_t tmp;
  LEDTICK++;
  if(LEDTICK>=5)
	{
		LEDTICK = 0;
	}	
	
		LED0_OFF();
		LED1_OFF();
		LED2_OFF();
		LED3_OFF();
		DD_OFF();

	switch(LEDTICK)
	{
		case(0) : 
		{
			tmp = smg[LED_Buff[0] & 0x1f];
		  if((LED_Buff[0] & 0x80) != 0)
		  {
			  tmp &= 0x7f;
		  }
			WriteLedPort(tmp & 0xff);             //����ܶ���GPIO����
			GPIO_ResetBits(GPIOB, LED0_EN_PIN);   //�����LED0 GPIO����
		  break;
		}	
		case(1) :
		{
			tmp = smg[LED_Buff[1] & 0x1f];
		  if((LED_Buff[1] & 0x80) != 0)
		  {
			  tmp &= 0x7f;
		  }
			WriteLedPort(tmp & 0xff);
			GPIO_ResetBits(GPIOC, LED1_EN_PIN);
		  break;
		}	
		case(2) :
		{
			tmp = smg[LED_Buff[2] & 0x1f];
		  if((LED_Buff[2] & 0x80) != 0)
		  {
			  tmp &= 0x7f;
		  }
      WriteLedPort(tmp & 0xff);
			GPIO_ResetBits(GPIOB, LED2_EN_PIN);
		  break;
		}	
		case(3) :
		{
			tmp = smg[LED_Buff[3] & 0x1f];
		  if((LED_Buff[3] & 0x80) != 0)
		  {
			  tmp &= 0x7f;
		  }
			WriteLedPort(tmp & 0xff);
			GPIO_ResetBits(GPIOB, LED3_EN_PIN);
		  break;
		}	
		case(4) :
		{
			if(LED_Buff[4] != 1)
			{
				WriteLedPort(0xff & 0xff);
			}
			else
			{
				WriteLedPort(0xfc & 0xff);
			}
			 GPIO_ResetBits(GPIOB, DD_EN_PIN);
		  break;
		}	
		default : break ;
	}
}








void CpyData(void)
{
  
        CpyDlyCnt++;
    
    if(CpyDlyCnt > CPYTIMER){
        CpyDlyCnt = 0;
        LED_Buff[0] = LED_Temp[0];
        LED_Buff[1] = LED_Temp[1];
        LED_Buff[2] = LED_Temp[2];
        LED_Buff[3] = LED_Temp[3];
        LED_Buff[4] = LED_Temp[4];
    }
   
}








void ProDisp(void)
{
	unsigned int itmp;
	if(LEDON==0)
	{
		LED_Temp[0] = 0xb;
		LED_Temp[1] = 0xb;
		LED_Temp[2] = 0xb;
		LED_Temp[3] = 0xb;
		LED_Temp[4] = 0;
	}
	else
	{
		if(MainMenu==0)
		{
				switch(SubMenu)
			{
				case 0:                  //��ʾ���չ⹦��
				{
						LED_Temp[0] = 1;  //'1'
						LED_Temp[4] = 0;
		
						if(RxzeroFlag1 == 1){
								LED_Temp[1] = 0xb;
								LED_Temp[2] = 0xe;
								LED_Temp[3] = 0xf;
						}else{
								if((RxoverFlag1 == 1) && ((systemtick & 0x10) != 0 )){
										LED_Temp[1] = 0x11;       //��ʾ'H'
										LED_Temp[2] = 0xb;
										LED_Temp[3] = 0xb;
								}else{                      
										if(AllPare.Rxpwr1 >= 0){
												LED_Temp[1] = 0xb;
												itmp = AllPare.Rxpwr1;
										}else{
												LED_Temp[1] = 0xa;
												itmp = -AllPare.Rxpwr1;
										}
										if(RxoverFlag1 == 1){
												LED_Temp[1] = 0x11;    //��ʾ'H'
										}
										
										itmp = BCD(itmp);
										if(AllPare.Rxpwr1 <-99){    //����99��ʾ��λ����
												LED_Temp[2] = ((itmp >> 8) & 0xf);
												LED_Temp[3] = ((itmp >> 4) & 0xf) | 0x80;
										}else{                  //��ʾһλ����һλС��
												LED_Temp[2] = ((itmp >> 4) & 0xf) | 0x80;
												LED_Temp[3] = itmp & 0xf;
										}
								}
						}
						break;
				}
				case 1:                   //��ʾ����⹦��
				{
					LED_Temp[0] = 0x1;
					LED_Temp[1] = 0x0;
					if(TxzeroFlag1 == 1)
					{
						LED_Temp[1] = 0xb;
						LED_Temp[2] = 0xe;
						LED_Temp[3] = 0xf;
					}
					else
					{
						if(AllPare.Txpwr1 >= 0 )
						{
							LED_Temp[1] = 0xb;
							itmp = AllPare.Txpwr1;
						}
						else if(AllPare.Txpwr1 < 0)
						{
							LED_Temp[1] = 0xa;
							itmp = -AllPare.Txpwr1;
						}
						itmp = BCD(itmp);
						if(AllPare.Txpwr1 < -99)
						{
							LED_Temp[2] = ((itmp >> 8) & 0xf);
							LED_Temp[3] = ((itmp >> 4) & 0xf) | 0x80;
						} 
						else
						{
							LED_Temp[2] = ((itmp >> 4) & 0xf) | 0x80;
							LED_Temp[3] = (itmp & 0xf);
						}
					}
					break;
				}
				case 2:                   //��ʾ����
				{
					LED_Temp[0] = 0x1;
					LED_Temp[4] = 0x0;
					itmp = AllPare.TxCurt1;
					itmp = BCD(itmp);
					LED_Temp[1] = ((itmp >> 8) & 0xf);
					LED_Temp[2] = ((itmp >> 4) & 0xf)| 0x80;
					LED_Temp[3] = (itmp & 0xf);
					break;
				}
				case 3:
				{
					LED_Temp[0] = 0x1;
					LED_Temp[1] = 0xb;
					LED_Temp[2] = 0xb;
					LED_Temp[3] = 0x3;
					LED_Temp[4] = 0x0;
					break;
				}
				case 4:
				{
					LED_Temp[0] = 0x1;
					LED_Temp[1] = 0xb;
					LED_Temp[2] = 0xb;
					LED_Temp[3] = 0x4;
					LED_Temp[4] = 0x0;
					break;
				}
				case 5:
				{
					LED_Temp[0] = 0x1;
					LED_Temp[1] = 0xb;
					LED_Temp[2] = 0xb;
					LED_Temp[3] = 0x5;
					LED_Temp[4] = 0x0;
					break;
				}
				default: break;
			} 			  
		}
		if(MainMenu==1)
		{
			switch(SubMenu)
			{
				case 0:
				{
					LED_Temp[0] = 0x2;
					LED_Temp[4] = 0x0;
					if(RxzeroFlag2 == 1)
					{
						LED_Temp[1] = 0xb;
						LED_Temp[2] = 0xe;   // 'L'
						LED_Temp[3] = 0xf;   // 'o'
					}
					else
					{
						if((RxoverFlag2 == 1) && (systemtick & 0x10) != 0)
						{
							LED_Temp[1] = 0x11;  //'H'
							LED_Temp[2] = 0xb;
							LED_Temp[3] = 0xb;
						}
						else
						{
							if(AllPare.Rxpwr2 < 0)
							{
								LED_Temp[1] = 0xa;
								itmp = -AllPare.Rxpwr2;
							}
							else
							{
								LED_Temp[1] = 0xb;
								itmp = AllPare.Rxpwr2;
							}
							if(RxoverFlag2 == 1)
							{
								LED_Temp[1] = 0x11;  //'H'
							}
							itmp = BCD(itmp);
							if(AllPare.Rxpwr2 < -99)
							{
								LED_Temp[2] = (itmp >> 8) & 0xf ;
								LED_Temp[3] = ((itmp >> 4) & 0xf) | 0x80;
							}
							else
							{
								LED_Temp[2] = ((itmp >> 4) & 0xf) | 0x80;
								LED_Temp[3] = (itmp & 0xf);
							}
					  }
					}
					break;
				}
				case 1:
				{
					LED_Temp[0] = 0x2;
					LED_Temp[4] = 0x0;
					if(TxzeroFlag2 == 1)
					{
						LED_Temp[1] = 0xb;
						LED_Temp[2] = 0xe;
						LED_Temp[3] = 0xf;
					}
					else
					{
						if((TxoverFlag2 == 1)&& ((systemtick & 0x10) == 0))
						{
							LED_Temp[1] = 0xb;
							LED_Temp[2] = 0xb;
							LED_Temp[3] = 0x11;
						}
						if(AllPare.Txpwr2 < 0)
						{
							LED_Temp[1] = 0xa;
							itmp = AllPare.Txpwr2;
						}
						else
						{
							LED_Temp[1] = 0xb;
							itmp = AllPare.Txpwr2;
						}
						itmp = BCD(itmp);
						if(AllPare.Txpwr2 < -99)
						{
							LED_Temp[2] = (itmp >> 8) & 0xf;
							LED_Temp[3] = ((itmp >> 4) & 0xf) | 0x80;
						}
					}
					break;
				}
				case 2:
				{
					LED_Temp[0] = 0x2;
					LED_Temp[4] = 0x0;
					itmp = AllPare.TxCurt2;
					itmp = BCD(itmp);
					LED_Temp[1] = (itmp >> 8) & 0xf;
					LED_Temp[2] = ((itmp >> 4) & 0xf) | 0x80;
					LED_Temp[3] = (itmp & 0xf);
					break;
				}
				case 3:
				{
					LED_Temp[0] = 0x2;
					LED_Temp[1] = 0xb;
				  LED_Temp[2] = 0xb;
					LED_Temp[3] = 0x3;
					LED_Temp[4] = 0x0;
					break;
				}
				case 4:
				{
					LED_Temp[0] = 0x2;
					LED_Temp[1] = 0xb;
				  LED_Temp[2] = 0xb;
					LED_Temp[3] = 0x4;
					LED_Temp[4] = 0x0;
					break;
				}
				case 5:
				{
					LED_Temp[0] = 0x2;
					LED_Temp[1] = 0xb;
				  LED_Temp[2] = 0xb;
					LED_Temp[3] = 0x5;
					LED_Temp[4] = 0x0;
					break;
				}
				default: break;
			}
		}
	
	}
	
}






 
void TestLED(void)
{
    unsigned char i = 0;
	
  
    for(i = 10; i > 0; i--){
       TestDlyCnt = 0;
        LED_Buff[0] = i-1;
        LED_Buff[1] = i-1;
        LED_Buff[2] = i-1;
        LED_Buff[3] = i-1;
        LED_Buff[4] = i-1;
        while((TestDlyCnt & 0x80) == 0){
             IWDG_ReloadCounter();
        }
    
    }
}







/// @}

/// @}

/// @}
