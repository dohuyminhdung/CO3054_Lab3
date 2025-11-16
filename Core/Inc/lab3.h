#ifndef INC_LAB3_H_
#define INC_LAB3_H_

#include "main.h"
#include "button.h"
#include "software_timer.h"
#include "lcd.h"
#include <stdint.h>

#define BLINK_TIME 500
#define LED_CYCLE 250
#define BTN_CYCLE 50

void btn_timer(void);

typedef enum{
	INIT,
	RED_GREEN,
	RED_AMBER,
	GREEN_RED,
	AMBER_RED,
	RED_MAN,
	AMBER_MAN,
	GREEN_MAN
} COLOR_STATE;

typedef enum{
	RED_S,
	AMBER_S,
	GREEN_S,
	WHITE_S
} COLOR;
extern COLOR_STATE state;
extern int RED_TIMER;
extern int AMBER_TIMER;
extern int GREEN_TIMER;
#define BUFFER_SIZE 4
extern int buffer1[BUFFER_SIZE];
extern int buffer2[BUFFER_SIZE];
void setBuffer1(COLOR color, int color_timer);
void setBuffer1_Default();
void setBuffer2();
void setTraffic1(COLOR color);
void setTraffic2(COLOR color);

///////////////////////////// 7 SEGMENT LED PROCESS /////////////////////
extern int CD1;
extern int CD2;
void displayCD(int CD1, int CD2);
//FSM
extern int automatic;
extern int manual_color;
void trafficSetUp();
void fsm_autoRun();
void fsm_manualRun();
void lab3_run();

#endif /* INC_LAB3_H_ */
