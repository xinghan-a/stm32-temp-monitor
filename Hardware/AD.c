#include "stm32f10x.h"                  // Device header
#include <math.h>
#include "Delay.h"

#define R1             10000.0f    //上拉固定电阻 10K
#define R0             10000.0f    //NTC 25℃阻值 10K
#define B_VALUE        3950.0f     //B值
#define T0             298.15f     //25℃ 开尔文温度
#define SAMPLE_NUM     5           //中值滤波采集次数

void AD_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	ADC_InitTypeDef ADC_InitStrcture;
	ADC_InitStrcture.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStrcture.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStrcture.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStrcture.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStrcture.ADC_ScanConvMode = DISABLE;
	ADC_InitStrcture.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1, &ADC_InitStrcture);
	
	ADC_Cmd(ADC1, ENABLE);
	
	ADC_ResetCalibration(ADC1);
	while (ADC_GetResetCalibrationStatus(ADC1) == SET);
	ADC_StartCalibration(ADC1);
	while (ADC_GetCalibrationStatus(ADC1) == SET);
}
uint16_t AD_GetValue(uint8_t ADC_Channel)
{
	ADC_RegularChannelConfig(ADC1, ADC_Channel, 1, ADC_SampleTime_55Cycles5);
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);					
	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);	
	return ADC_GetConversionValue(ADC1);
}

float NTC_GetTemp(uint16_t AD_value)
{
    float Rntc,temp;
    if(AD_value == 4095) AD_value=4094; //防除零
    
    Rntc = R1 * AD_value / (4095.0f - AD_value);
    
    temp = 1.0f / ( 1.0f/T0 + log(Rntc/R0)/B_VALUE );     //B值公式
    temp = temp - 273.15f;
    
    return temp;
}

uint16_t Array[SAMPLE_NUM];

uint16_t ADC_Median_Filter(uint8_t ADC_Channel)       //中值滤波
{
    // 1. 采集多次
    for(int i=0; i<SAMPLE_NUM; i++)
	{
        Array[i] = AD_GetValue(ADC_Channel);
        Delay_ms(10);  
    }
    // 2. 冒泡排序
for(int i=0; i<SAMPLE_NUM-1; i++)        // 外层循环：控制需要多少轮
{
    for(int j=0; j<SAMPLE_NUM-1-i; j++)  // 内层循环：每轮比较的次数
    {
        if(Array[j] > Array[j+1])        // 如果前一个比后一个大
        {
          // 交换两个元素
          uint16_t tmp = Array[j];
          Array[j] = Array[j+1];
          Array[j+1] = tmp;
        }
    }
}
    // 3. 返回中间值
    return Array[SAMPLE_NUM/2];
}
	