#include "ping.h"
#include "Timer.h"
#include "lcd.h"

void ping_init() {
    //clock enables
    SYSCTL_RCGCGPIO_R |= 0x02;
    SYSCTL_RCGCTIMER_R |= 0x08;
    timer_waitMillis(20);

    //Timer init
    TIMER3_CTL_R |= 0x00000C00;
    TIMER3_CFG_R = 0x04;
    TIMER3_TBMR_R = 0b00111;
    TIMER3_TBILR_R |= 0xFFFF;
    TIMER3_TBPR_R |= 0x00FF;
    TIMER3_ICR_R |= 0x00000400;
    TIMER3_IMR_R |= 0x00000400;
    TIMER3_CTL_R |= 0x00000100;


    //Input enabling
    GPIO_PORTB_AFSEL_R |= 0b00001000;
    GPIO_PORTB_PCTL_R  = 0x00007000 | (GPIO_PORTB_PCTL_R & ~0x0000F000);
    GPIO_PORTB_DEN_R   |= 0b00001000;
    GPIO_PORTB_DIR_R   &= 0b11110111;

    NVIC_EN1_R |= 0x00000010;

    IntRegister(INT_TIMER3B, TIMER3B_Handler);
    IntMasterEnable();
}

int ping_read() {
    //mask interrupt
    TIMER3_IMR_R &= ~0x00000400;
    //change to GPIO
    GPIO_PORTB_AFSEL_R &= ~0b00001000;
    GPIO_PORTB_DIR_R |= 0b00001000;
    timer_waitMillis(1);
    //Drive high
    GPIO_PORTB_DATA_R |= 0b00001000;
    //wait
    timer_waitMicros(6);
    //Drive low
    GPIO_PORTB_DATA_R &= 0b11110111;

   // TIMER3_CTL_R &= 0x00000100;

    //change to input
    GPIO_PORTB_AFSEL_R |= 0b00001000;
    GPIO_PORTB_DIR_R   &= 0b11110111;
    //clear interrupt and re-enable
    TIMER3_ICR_R |= 0x00000400;
    TIMER3_IMR_R |= 0x00000400;
    while(!edgeCount == 2){

    }
    edgeCount = 0;
    //TIMER3_CTL_R &= ~0x00000100;
    return rising_time - falling_time;
}

void TIMER3B_Handler(){
    if(edgeCount == 0){
        rising_time = TIMER3_TBR_R;
        edgeCount = 1;
    }
    else if(edgeCount == 1){
        falling_time = TIMER3_TBR_R;
        edgeCount = 2;
    }
    TIMER3_ICR_R |= 0x00000400;
}
