#ifndef __TEMP_LED_H
#define __TEMP_LED_H

void Led_TempCtrl_Init(void);
void Led_TempCtrl(float Temp);
uint8_t TempLed_GetState(void);

#endif
