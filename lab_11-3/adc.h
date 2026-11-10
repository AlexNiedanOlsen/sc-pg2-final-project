/*
 * adc.h
 *
 *  Created on: Oct 20, 2026
 *      Author: kublk
 */




#ifndef ADC_H_
#define ADC_H_

#include <stdint.h>
#include <stdbool.h>
#include <inc/tm4c123gh6pm.h>

void adc_init();

int adc_read();



#endif /* ADC_H_ */
