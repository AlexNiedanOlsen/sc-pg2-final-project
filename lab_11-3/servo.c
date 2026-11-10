/*
 * servo.c
 *
 *  Created on: Nov 3, 2026
 *      Author: kublk
 */
#include "servo.h"
#include "Timer.h"
#include "lcd.h"
#include "button.h"

extern volatile int button_event;

volatile int largeNum;
volatile int smallNum;
volatile int calibrated = 0;

void servo_init(){

    //syst timers
    SYSCTL_RCGCGPIO_R |= 0x02;
    SYSCTL_RCGCTIMER_R |= 0x02;
    timer_waitMillis(20);

    //Timer init
    TIMER1_CTL_R &= ~0x00000100;
    TIMER1_CFG_R = 0x00000004;
    TIMER1_TBMR_R = TIMER1_TBMR_R & ~0x00000005 | 0x0000000A;
    TIMER1_CTL_R &= ~0x00004000;
    TIMER1_TBPR_R = 0x0002;
    TIMER1_TBILR_R = 0x7100;
    TIMER1_TBPMR_R = 0x0002;
    TIMER1_TBMATCHR_R = 0x3E80;

    //re-enable timer
    TIMER1_CTL_R |= 0x00000100;

    //GPIO init
    GPIO_PORTB_AFSEL_R |= 0b00100000;
    GPIO_PORTB_PCTL_R  = 0x00700000 | (GPIO_PORTB_PCTL_R & ~0x00F00000);
    GPIO_PORTB_DEN_R   |= 0b00100000;
    GPIO_PORTB_DIR_R   |= 0b00100000;
}

void servo_set(float angle){
    int ticks;
    //angle to ticks
    if(!calibrated){
        ticks = (int) (160000 - ((angle / 180.0) + 1) * 16000);
    }
    else{
        ticks = (angle / 180.0) * (largeNum - smallNum) + smallNum;
    }
    //load
    TIMER1_TBPMR_R = ticks >> 16 & 0x000000FF;
    TIMER1_TBMATCHR_R = ticks & 0x0000FFFF;
}

void calibrateServo(){
    servo_set(0);
    int angle = 0;
    while(1){
        servo_set(angle);
        lcd_printf("%d", angle);
        if(button_event == 1){
            angle += 3;
            button_event = 0;
        }
        else if(button_event == 2){
            angle -= 3;
            button_event = 0;
        }
        else if(button_event == 4){
            largeNum = (0x00FF0000 & (TIMER1_TBPMR_R << 16)) | (0x0000FFFF & TIMER1_TBMATCHR_R);
            lcd_printf("%d", largeNum);
            button_event = 0;
            timer_waitMillis(500);
            break;
        }
    }
    while(1){
        servo_set(angle);
        lcd_printf("%d", angle);
        if(button_event == 1){
            angle += 3;
            button_event = 0;
        }
        else if(button_event == 2){
            angle -= 3;
            button_event = 0;
        }
        else if(button_event == 3){
            angle += 180;
        }
        else if(button_event == 4){
            smallNum = (0x00FF0000 & (TIMER1_TBPMR_R << 16)) | (0x0000FFFF & TIMER1_TBMATCHR_R);
            lcd_printf("%d", smallNum);
            break;
        }
    }
    calibrated = 1;
}
