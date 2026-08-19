#include "stm32f10x.h"                  // Device header
#include "LED.h"

static uint8_t Led_State = 0;

void Led_TempCtrl_Init(void)
{
	Led_State = 0;
	// 同步硬件状态，上电直接亮对应灯
    LED1_OFF();
    LED2_OFF();
    LED3_ON();
}

void Led_TempCtrl(float Temp)
{
    #define LOW_TO_MID   26.5f
    #define MID_TO_LOW   25.5f
    #define MID_TO_HIGH  30.5f
    #define HIGH_TO_MID  29.5f

    // 第一个switch：状态切换逻辑
    switch(Led_State)
    {
        case 0:                                               //当前处于低温档
            if(Temp > LOW_TO_MID) Led_State = 1;
            break;
        case 1:                                               //当前处于中温档
            if(Temp < MID_TO_LOW) Led_State = 0;
            else if(Temp > MID_TO_HIGH) Led_State = 2;
            break;
        case 2:                                               //当前处于高温档
            if(Temp < HIGH_TO_MID) Led_State = 1;
            break;
    }

    // 第二个switch：硬件输出控制
    LED1_OFF();
    LED2_OFF();
    LED3_OFF();
    switch(Led_State)
    {
        case 0: LED3_ON(); break;
        case 1: LED2_ON(); break;
        case 2: LED1_ON(); break;
    }
}
