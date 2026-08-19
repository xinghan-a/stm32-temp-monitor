#ifndef __AD_H
#define __AD_H

void AD_Init(void);
uint16_t AD_GetValue(uint8_t ADC_Channel);
float NTC_GetTemp(uint16_t AD_value);
uint16_t ADC_Median_Filter(uint8_t ADC_Channel); 

#endif
