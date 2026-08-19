#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Delay.h"
#include "AD.h"
#include "LED.h"
#include "LedTempCtrl.h"

uint16_t AD1;
float Temp;

int main(void)
{
	OLED_Init();
	LED_Init();
	AD_Init();
	Led_TempCtrl_Init();
	
	OLED_ShowString(1, 1, "AD0:");
	OLED_ShowString(2, 1, "Temp:");
	
	while (1)
	{
		AD1 = ADC_Median_Filter(ADC_Channel_1);
		Temp = NTC_GetTemp(AD1);
		
		//		Temp = Temp - 2.0;         //LED干扰电路，手动修正      //疑似接线不稳导致ADC电压异常，现已正常
		
		OLED_ShowNum(1, 5, AD1, 4);	
		
		OLED_ShowNum(2,6,(int)Temp,2);
		OLED_ShowString(2,8,".");
		OLED_ShowNum(2,9,(int)(Temp*10)%10,1);
		OLED_ShowString(2,11,"oC");
		
		Led_TempCtrl(Temp);

		Delay_ms(100);
	}
}
