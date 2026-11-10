#include "timer.h"
#include <stdio.h>
#include <adc.h>
#include "lcd.h"/*
 * adc.c
 *
 *  Created on: Oct 20, 2026
 *      Author: kublk
 */
void adc_init(){

    SYSCTL_RCGCGPIO_R |= 0b011010;
    timer_waitMillis(10);
    GPIO_PORTB_AFSEL_R |= 0b00010000;
    GPIO_PORTB_AMSEL_R |= 0b00010000;
    GPIO_PORTB_DEN_R   &= 0b11101111;
    GPIO_PORTB_DIR_R   &= 0b11101111;
    GPIO_PORTB_PCTL_R &= 0xFFF0FFFF;
    SYSCTL_RCGCADC_R |= 0x00000001;
    timer_waitMillis(10);

    ADC0_EMUX_R &= 0x1110;
    ADC0_SSMUX0_R = 0xaaaaaaaa;
    ADC0_SSCTL0_R = 0x60000000;
    ADC0_ACTSS_R |= 0x00000001;
    ADC0_ISC_R |= 0x00000001;
}

int adc_read(){
    int x = 0;
    ADC0_PSSI_R |= 0b00000001;
    //ADC0_RIS_R & 0x00000001;
    //ADC0_SSFSTAT0
    while(!(ADC0_RIS_R & 0x00000001)){
    }
    x = (0x00000FFF & ADC0_SSFIFO0_R);
    ADC0_ISC_R |= 0x00000001;
    return x;
}
