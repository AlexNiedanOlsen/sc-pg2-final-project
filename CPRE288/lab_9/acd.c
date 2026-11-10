/*
 * acd.c
 *
 *  Created on: Oct 20, 2026
 *      Author: alex2168
 */
#include "adc.h"

void adc_init() {
    SYSCTL_RCGCADC_R |= 0b01; // Enable clock for ADC module 0

    // Configure GPIO PB4
    SYSCTL_RCGCGPIO_R |= 0b10; // Enable clock for port B
    GPIO_PORTB_AFSEL_R |= 0b10000; // Alternate mode for pin 4
    //GPIO_PORTB_ADCCTL_R = 0x10; // Allow pin B4 to trigger adc
    GPIO_PORTB_AMSEL_R |= 0b10000; // Enables analog mode for B4
    GPIO_PORTB_DEN_R &= ~0b10000; // Disable digital output for B4
    GPIO_PORTB_DIR_R &= ~0b10000; // Make B4 and input

    // Configure ADC
    ADC0_ACTSS_R &= ~0b01; // Disable SS0
    ADC0_EMUX_R &= ~0xF; // SS0 trigger
    ADC0_SSMUX0_R =  (ADC0_SSMUX0_R & ~0x000F) | 0x0A; // AIN10
    ADC0_SSCTL0_R = 0b110; // only one sample
    ADC0_ISC_R = 0x01; // clear completion flag


    ADC0_ACTSS_R = ADC_ACTSS_ASEN0; // Enable SS0
}

int adc_read() {
    
    int IR_Value;

    ADC0_PSSI_R |= 0b1; // Initiate SS0

    while((ADC0_RIS_R & 0b1) == 0);
    IR_Value = ADC0_SSFIFO0_R; // Read the sample
    ADC0_ISC_R |= 0b1;
    return IR_Value;
}


