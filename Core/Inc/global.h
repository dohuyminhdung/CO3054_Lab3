#ifndef INC_GLOBAL_H_
#define INC_GLOBAL_H_

#include "main.h"
#include <stdint.h>

//Traffic light exercise begin
#define TIMER_TRAFFIC  0
#define TIMER_LED_SCAN 1

#define RED_PIN     GPIO_PIN_4
#define GREEN_PIN   GPIO_PIN_5
#define YELLOW_PIN  GPIO_PIN_6

#define RED		3
#define GREEN  	2
#define YELLOW	1
//Traffic light exercise end

#define TI_BUTTON_TIMER 0
#define TI_BUTTON_TIME 20

#define TI_LED7SEG_TIMER 1
#define TI_LED7SEG_TIME 2

#endif /* INC_GLOBAL_H_ */
