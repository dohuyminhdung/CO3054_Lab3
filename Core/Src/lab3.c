#include "lab3.h"

//////////////////////////// RED-AMBER-GREEN PROCESS //////////////////////////////////////////////
COLOR_STATE state = INIT;
int RED_TIMER = 5;
int AMBER_TIMER = 2;
int GREEN_TIMER = 3;
int buffer1[BUFFER_SIZE];
int buffer2[BUFFER_SIZE];
void setBuffer1_Default(){
	buffer1[RED_S] = RED_TIMER;
	buffer1[AMBER_S] = AMBER_TIMER;
	buffer1[GREEN_S] = GREEN_TIMER;
}
void setBuffer1(COLOR color, int color_timer){
	buffer1[color] = color_timer;
}
void setBuffer2(){
	buffer2[RED_S] = buffer1[AMBER_S] + buffer1[GREEN_S];
	buffer2[AMBER_S] = buffer1[AMBER_S];
	buffer2[GREEN_S] = buffer1[RED_S] - buffer1[AMBER_S];
	//A+B+C = (B + C) + B + (A - B)
}
void setTraffic1(COLOR color){
	if(color == RED_S)
		lcd_DrawCircle(30, 30, RED, 20, 1);
	else if(color == AMBER_S)
		lcd_DrawCircle(30, 30, YELLOW, 20, 1);
	else if(color == GREEN_S)
		lcd_DrawCircle(30, 30, GREEN, 20, 1);
	else
		lcd_DrawCircle(30, 30, WHITE, 20, 1);
}
void setTraffic2(COLOR color){
	if(color == RED_S)
		lcd_DrawCircle(30, 100, RED, 20, 1);
	else if(color == AMBER_S)
		lcd_DrawCircle(30, 100, YELLOW, 20, 1);
	else if(color == GREEN_S)
		lcd_DrawCircle(30, 100, GREEN, 20, 1);
	else
		lcd_DrawCircle(30, 100, WHITE, 20, 1);
}

void displayCD(int CD1, int CD2){
	lcd_ShowIntNum(70, 30, CD1, 2, BLACK, WHITE, 32);
	lcd_ShowIntNum(70, 100, CD2, 2, BLACK, WHITE, 32);
}

#define CD 0 //CD = Count Down 1 second
#define SEG 1 //SEG = 7 SEGMENT LED Freq
#define BTN_TIMER 3 //BTN Timer Freq
#define BTN_LAB3_0 0
#define BTN_LAB3_1 1
#define BTN_LAB3_2 2

void btn_timer(void){
	if(isFlag(BTN_TIMER)){
		setTimer(BTN_TIMER, BTN_CYCLE);
		button_scan();
	}
}

int CD1 = 0;
int CD2 = 0;
int automatic = 0;
void trafficSetUp(){
	lcd_init();
	button_init();
	setBuffer1_Default();
	setTimer(CD, 1000);
	setTimer(BTN_TIMER, BTN_CYCLE);
	setTimer(SEG, LED_CYCLE);
	setBuffer2();
//	state = INIT;
}
void fsm_autoRun(){
	switch(state){
	case INIT:
		if(1){
			automatic = 1;
			state = RED_GREEN;
			CD1 = buffer1[RED_S];
			CD2 = buffer2[GREEN_S];
		}
		break;
	case RED_GREEN:
		setTraffic1(RED_S);
		setTraffic2(GREEN_S);
		if(isFlag(CD)){
			--CD1, --CD2;
			setTimer(CD, 1000);
		}
		if(CD2 <= 0){
			state = RED_AMBER;
			CD2 = buffer2[AMBER_S];
		}
		break;
	case RED_AMBER:
		setTraffic1(RED_S);
		setTraffic2(AMBER_S);
		if(isFlag(CD)){
			--CD1, --CD2;
			setTimer(CD, 1000);
		}
		if(CD2 <= 0){
			state = GREEN_RED;
			CD1 = buffer1[GREEN_S];
			CD2 = buffer2[RED_S];
		}
		break;
	case GREEN_RED:
		setTraffic1(GREEN_S);
		setTraffic2(RED_S);
		if(isFlag(CD)){
			--CD1, --CD2;
			setTimer(CD, 1000);
		}
		if(CD1 <= 0){
			state = AMBER_RED;
			CD1 = buffer1[AMBER_S];
		}
		break;
	case AMBER_RED:
		setTraffic1(AMBER_S);
		setTraffic2(RED_S);
		if(isFlag(CD)){
			--CD1, --CD2;
			setTimer(CD, 1000);
		}
		if(CD1 <= 0){
			state = RED_GREEN;
			CD1 = buffer1[RED_S];
			CD2 = buffer2[GREEN_S];
		}
		break;
	default: break;
	}
	if(automatic == 1){
		if(isPress(BTN_LAB3_0)){
			automatic = 0;
			state = RED_MAN;
			setTraffic1(RED_S);
			setTraffic2(RED_S);
			setTimer(CD, BLINK_TIME);
			setTimer(SEG, 500);
		}
	}
	if(automatic == 1 && isFlag(SEG)){
		displayCD(CD1, CD2);
		setTimer(SEG, LED_CYCLE);
	}
}

int manual_color = 0;
void fsm_manualRun(){
	switch(state){
	case RED_MAN:
		if(isFlag(CD)){
			if(manual_color == 0){
				setTraffic1(WHITE_S);
				setTraffic2(WHITE_S);
			}
			else {
				setTraffic1(RED_S);
				setTraffic2(RED_S);
			}
			setTimer(CD, BLINK_TIME);
			manual_color = 1 - manual_color;
		}
		if(isFlag(SEG)){
			setTimer(SEG, LED_CYCLE);
			displayCD(RED_TIMER, 2);
		}
		if(isPress(BTN_LAB3_0)){
			state = AMBER_MAN;
			setTraffic1(AMBER_S);
			setTraffic2(AMBER_S);
			setTimer(CD, BLINK_TIME);
		}
		if(isPress(BTN_LAB3_1)){
			if(++RED_TIMER >= 100) RED_TIMER = 5;
		}
		if(isPress(BTN_LAB3_2)){
			buffer1[RED_S] = RED_TIMER;
		}
		break;
	case AMBER_MAN:
		if(isFlag(CD)){
			if(manual_color == 0){
				setTraffic1(WHITE_S);
				setTraffic2(WHITE_S);
			}
			else {
				setTraffic1(AMBER_S);
				setTraffic2(AMBER_S);
			}
			setTimer(CD, BLINK_TIME);
			manual_color = 1 - manual_color;
		}
		if(isFlag(SEG)){
			setTimer(SEG, LED_CYCLE);
			displayCD(AMBER_TIMER, 3);
		}
		if(isPress(BTN_LAB3_0)){
			state = GREEN_MAN;
			setTraffic1(GREEN_S);
			setTraffic2(GREEN_S);
			setTimer(CD, BLINK_TIME);
		}
		if(isPress(BTN_LAB3_1)){
			if(++AMBER_TIMER >= 6) AMBER_TIMER = 1;
		}
		if(isPress(BTN_LAB3_2)){
			buffer1[AMBER_S] = AMBER_TIMER;
		}
		break;
	case GREEN_MAN:
		if(isFlag(CD)){
			if(manual_color == 0){
				setTraffic1(WHITE_S);
				setTraffic2(WHITE_S);
			}
			else {
				setTraffic1(GREEN_S);
				setTraffic2(GREEN_S);
			}
			setTimer(CD, BLINK_TIME);
			manual_color = 1 - manual_color;
		}
		if(isFlag(SEG)){
			setTimer(SEG, LED_CYCLE);
			displayCD(GREEN_TIMER, 4);
		}
		if(isPress(BTN_LAB3_0)){
			setBuffer2();
			state = INIT;
		}
		if(isPress(BTN_LAB3_1)){
			if(++GREEN_TIMER >= 94) GREEN_TIMER = 3;
		}
		if(isPress(BTN_LAB3_2)){
			buffer1[GREEN_S] = GREEN_TIMER;
		}
		break;
	default: break;
	}
}
void lab3_run(){
	fsm_autoRun();
	fsm_manualRun();
	btn_timer();
}
