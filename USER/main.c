////////////////////////////////////////////////////////////////////////////////
/// @file    main.c
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
#define _MAIN_C_
#define MAX_OPT_POWER   20         // 2dbm  单位：0.1dbm
#define MINWARNOPTPWR   -150 
#define AGC_START_PWR   -80        //AGC启控光功率

#define FSK_LOW_MAX_VALUE (1000)
#define FSK_HIGH_MIN_VALUE (2000)

#define FSK_IS_VALUE_DELAY_TIME   (10)

#define SET_FSK_S1_ON   GPIO_SetBits(GPIOA,GPIO_Pin_2)
#define SET_FSK_S2_ON   GPIO_SetBits(GPIOA,GPIO_Pin_10)
#define SET_FSK_S3_ON   GPIO_SetBits(GPIOA,GPIO_Pin_15)
#define SET_FSK_S4_ON   GPIO_SetBits(GPIOA,GPIO_Pin_11)

#define SET_FSK_S1_OFF  GPIO_ResetBits(GPIOA,GPIO_Pin_2)
#define SET_FSK_S2_OFF  GPIO_ResetBits(GPIOA,GPIO_Pin_10)
#define SET_FSK_S3_OFF  GPIO_ResetBits(GPIOA,GPIO_Pin_15)
#define SET_FSK_S4_OFF  GPIO_ResetBits(GPIOA,GPIO_Pin_11)

// Files includes

#include "key.h"
#include "led.h"
#include "math.h"
#include "delay.h"
#include "bsp_iwdg.h"
#include "bsp_adc.h"
#include "bsp_tim.h"

volatile unsigned int systemtick;          //系统时钟计数值
volatile unsigned int TestDlyCnt = 0;      //测试延时寄存器
volatile unsigned int CtrlDly = 0;
volatile unsigned char save_flag = 0;      //保存标志位
volatile unsigned char AutoAGCValue = 0;
volatile unsigned char AutoAGCValueTmp =0;
volatile unsigned char initialflag = 0;
volatile unsigned char RxzeroFlag1 = 0;     //接收光功率为0标志
volatile unsigned char RxzeroFlag2 = 0;
volatile unsigned char RxoverFlag1 = 0;     //接收光功率过高标志
volatile unsigned char RxoverFlag2 = 0;
volatile unsigned char TxzeroFlag1 = 0;     //发射光功率为0标志
volatile unsigned char TxzeroFlag2 = 0;
volatile unsigned char TxoverFlag1 = 0;     //发射光功率过高标志
volatile unsigned char TxoverFlag2 = 0;
CommAllPare_stru AllPare;                   //所有参数结构体
volatile unsigned char FSK_IS_VALUE_DELAY = 0;

const unsigned short AGC_PWM_Value[53] ={
3000,
1178,1016,974,951,
927,905,882,864,
844,822,804,783,
764,746,728,710,
692,677,660,645,
628,616,598,587,
573,562,549,538,
529,520,512,501,
492,484,478,471,
463,458,452,445,
440,434,429,424,
420,416,413,408,
405,401,397,394

};

void ProOptAGC(void)
{
	int itmp;
  if(CtrlDly > 150)
	{
		if(initialflag != 0x01)
		{
			initialflag = 0x01;
		}
		if(AllPare.Rxpwr1 < MINWARNOPTPWR)
		{
			AutoAGCValue = MAX_OPT_POWER<<1;
		}
		else if((AllPare.Rxpwr1 >= MINWARNOPTPWR) && (AllPare.Rxpwr1 <= AGC_START_PWR))
		{
			AutoAGCValue = 0;
		}
		else
		{
			itmp = (AllPare.Rxpwr1 - AGC_START_PWR)*4;
			itmp = itmp /10;
			AutoAGCValue = itmp;
			if(AutoAGCValue == AutoAGCValueTmp +1)
			{
				if((AllPare.Rxpwr1 - AGC_START_PWR)*4 < (AutoAGCValue*10)+4)   //0.1db反向步进差
			  {
				  AutoAGCValue = AutoAGCValueTmp;
			  }
				else
				{
					AutoAGCValueTmp = AutoAGCValue;
				}
			}
			else
			{
				AutoAGCValueTmp = AutoAGCValue;
			}
			if(AutoAGCValue > (MAX_OPT_POWER << 1))
			{
				AutoAGCValue = (MAX_OPT_POWER << 1);
				AutoAGCValueTmp = AutoAGCValue;
			}
		}
	}
	CtrlDly = 0;
}

typedef enum
{
	FAIL = 0,
	PASS = 1,
}Flash_ProgramStatus;


//void EEPROM_Save(void)
//{
//	Flash_ProgramStatus Flashstatus;
//	unsigned int save_flash[16];
//	if(save_flag == 1)
//	{
//		save_flash[0] = 0x11223344;
//		memcpy((unsigned char*)(&save_flash[1]),(unsigned char*)(&AllPare.Rxpwr1),56);
//		Flashstatus = Flash_Write((unsigned int *)&save_flash,16);
//		if(Flashstatus == FAIL)
//		{
//			Flashstatus = Flash_Write(unsign)
//		}
//	}
//}

unsigned char Read_FSK_Value(unsigned char old_value)
{
	
	unsigned char state_temp = old_value;
  float advalue = (float)(ADConversion(ADC_Channel_2)*5.0/4095.0*1000);
	
	if((advalue >= FSK_LOW_MAX_VALUE)&&(advalue <= FSK_HIGH_MIN_VALUE))
	{
		state_temp |= 0x04;		
	}
	else
	{
		state_temp &= 0xfb;
		if(advalue < FSK_LOW_MAX_VALUE)
		{
			state_temp &= 0xfe;
		}
		else if(advalue > FSK_HIGH_MIN_VALUE)
		{
			state_temp |= 0x01;
		}
		advalue = (float)(ADConversion(ADC_Channel_3)*5.0/4095.0*1000);
		
		if(advalue > FSK_HIGH_MIN_VALUE)
		{
			state_temp |= 0x02;
		}
		else if(advalue < FSK_LOW_MAX_VALUE)
		{
			state_temp &= 0xfd;
		}
	}
	return state_temp;
}


void process_auto_ret(void)
{
	unsigned char FSK_is_value;
	FSK_is_value = Read_FSK_Value(AllPare.FSK_Value);
	
	if(FSK_is_value != AllPare.FSK_Value)
	{
		FSK_IS_VALUE_DELAY ++;
		if(FSK_IS_VALUE_DELAY > FSK_IS_VALUE_DELAY_TIME)
		{
			AllPare.FSK_Value = FSK_is_value;
			FSK_IS_VALUE_DELAY = 0;
		}
		
	}
	else
		{
			FSK_IS_VALUE_DELAY = 0;
		}
		
		switch(AllPare.FSK_Value &(0x07))
		{
			case 00 :
				         SET_FSK_S1_ON;
			           SET_FSK_S2_OFF;
			           SET_FSK_S3_OFF;
			           SET_FSK_S4_ON;
				     break;
			case 01 :
				         SET_FSK_S1_OFF;
			           SET_FSK_S1_ON;
			           SET_FSK_S1_ON;
			           SET_FSK_S1_OFF;
			       break;
			case 02 :
				         SET_FSK_S1_ON;
			           SET_FSK_S2_OFF;
			           SET_FSK_S3_ON;
			           SET_FSK_S4_OFF;
				     break;
			case 04 :
			case 05 : 
			case 06 :
			case 07 :
				         SET_FSK_S1_ON;
			           SET_FSK_S2_ON;
			           SET_FSK_S3_ON;
			           SET_FSK_S4_ON;
				     break;
			
			default : 
				        break;
		}
}




void Delay(uint32_t time)
{
	uint32_t i;
	for(i=0;i<time;i++);
}


s32 main(void)
{
	float ftmp;
  LED_Init();
  DELAY_Init();
  KEY_Init();	  
	IWDG_Init();
  ADC_Initialize();
	PWM_Output_Config();
	
	
	TestLED();
	
    while(1) 
		{	
		  IWDG_ReloadCounter();
			//////////////////////////////////////////////////////////////////////////////////////////
			ftmp =(float)(ADConversion(ADC_Channel_7)*5.0/4095.0);                                   //接收光功率RP1
			if(ftmp < 0.0316)
			{
				RxzeroFlag1 = 1;
				AllPare.Rxpwr1 = -999;
			}
			else
			{
				AllPare.Rxpwr1 = (signed short)100*log(ftmp);
				RxzeroFlag1 = 0;
			}
			if(AllPare.Rxpwr1 > MAX_OPT_POWER)
			{
				RxoverFlag1 = 1;
			}
			else
			{
				RxoverFlag1 = 0;
			}
			
			//////////////////////////////////////////////////////////////////////////////////////////
			ftmp =(float)(ADConversion(ADC_Channel_4)*5.0/4095.0);                                    //发射光功率TP1
			if(ftmp < 0.0316)
			{
				TxzeroFlag1 = 1;
				AllPare.Txpwr1 = -999;
			}
			else
			{
				AllPare.Txpwr1 = (signed short)100*log(ftmp);
				TxzeroFlag1 = 0;
			}
			
			//////////////////////////////////////////////////////////////////////////////////////////
			ftmp =(float)(ADConversion(ADC_Channel_5)*5.0/4095.0);                                     //发射电流TC1
			ftmp = ftmp * 1000;
			AllPare.TxCurt1 = (signed short)ftmp;
			
			
			
			//////////////////////////////////////////////////////////////////////////////////////////
			ftmp = (float)(ADConversion(ADC_Channel_6)*5.0/4095.0);			                               //接收光功率RP2
			if(ftmp < 0.0316)
			{
				RxzeroFlag2 = 1;
				AllPare.Rxpwr2 = -999;
			}
			else
			{
				AllPare.Rxpwr2 = (signed short)100*log(ftmp);
				RxzeroFlag2 = 0;
			}
			if(AllPare.Rxpwr2 > MAX_OPT_POWER)
			{
				RxoverFlag2 = 1;
			}
			else
			{
				RxoverFlag2 = 0;
			}
			
			
			//////////////////////////////////////////////////////////////////////////////////////////
			ftmp =(float)(ADConversion(ADC_Channel_7)*5.0/4096.0);                                    //发射光功率TP2  
			if(ftmp < 0.0316)
			{
				TxzeroFlag2 = 1;
				AllPare.Txpwr2 = -999;
			}
			else
			{
				AllPare.Txpwr2 = (signed short)100*log(ftmp);		
				TxzeroFlag2 = 0;
			}                                                                                          
			
						
			//////////////////////////////////////////////////////////////////////////////////////////
			ftmp =(float)(ADConversion(ADC_Channel_8)*5.0/4096.0);                                    //发射电流TC2
			ftmp = ftmp * 1000;
			AllPare.TxCurt2 = (signed short)ftmp;                                                                                           
			
			
			
			ProOptAGC();
			
			if(initialflag == 0x01)
			{
				WritePWM(TIM1,1,AGC_PWM_Value[AutoAGCValue<<1]);  
			}
			else
			{
				WritePWM(TIM1,1,AGC_PWM_Value[40]);   //原始波形
			}
			
			
			
			
			
			Delay(0xff);
		  Pro_Key();
	    ProDisp();
			CpyData();
    }
			
}



/// @}

/// @}

/// @}
