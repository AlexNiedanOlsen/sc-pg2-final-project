
#include "servo.h"
#include "Timer.h"
#include "lcd.h"
#include "button.h"
#include "movement.h"
#include "uart_extra_help.h"
#include "adc.h"
#include "ping.h"
#include <stdio.h>
#include <string.h>
#include "scanFunction.h"


volatile unsigned int rising_time;
volatile unsigned int falling_time;
volatile unsigned int edgeCount;

extern volatile int button_event;
volatile  char uart_data;
volatile  int flag;
volatile int tflag;
int puredataIR[180];
object objects[8];
/**
 * main.c
 */
int main(void)
{
    timer_init();
    lcd_init();
    button_init();
    init_button_interrupts();
    uart_init(115200);
    uart_interrupt_init();
    oi_t *sensor_data = oi_alloc();
    //oi_init(sensor_data);
    adc_init();
    ping_init();
    servo_init();
    calibrateServo();
    timer_waitMillis(200);



    scan180(puredataIR);




    /*
    while(1){
        servo_set(angle, dir);
        if(button_event){
            if(button_event == 1){
                if(dir){
                    angle += 1;
                }
                else{
                    angle -= 1;
                }
            }
            if(button_event == 2){
                if(dir){
                    angle += 5;
                }
                else{
                    angle -= 5;
                }
            }
            if(button_event == 3){
                if(dir){
                    dir = 0;
                }
                else{
                    dir = 1;
                }
            }
            if(button_event == 4){
                if(dir){
                    angle = 180;
                }
                else{
                    angle = 0;
                }
            }
            button_event = 0;
        }
        if(angle < 0){
            angle = 0;
        }
        else if(angle > 180){
            angle = 180;
        }
    }
    */
	return 0;
}
