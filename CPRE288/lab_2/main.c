

/**
 * main.c
 */
#include "open_interface.h"
#include "movement.h"
#include "lcd.h"
#include "cyBot_uart.h"
#include <stdio.h>
#include <string.h>
#include "Timer.h"
int main(void)
{
    /*
    int i;
    for(i = 0; i < 4; i++) {
        move_distance(.5);
        turn_degrees(90);
    }

    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);
    move_forward(sensor_data, 1);
    oi_free(sensor_data);
}
*/


    cyBot_uart_init();
    //lcd_init();
    //lcd_clear();
    //char message[] = "Got a ";
    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);
    /*
    while(1) {
        int byte = cyBot_getByte();

        lcd_printf("Got a %c", byte);
                int i;
        for(i = 0; i < strlen(message); i++) {
            cyBot_sendByte(message[i]);
        }
        cyBot_sendByte(byte);
    }
    */
    // Part 4
    //move_forward(sensor_data, 1);



    // part 5
    while(1) {
        int byte = cyBot_getByte();
        if(byte == 'W') {
            oi_setWheels(250, 250);
            timer_waitMillis(200);
            //oi_setWheels(0,-0);
        }
        else if(byte == 'A') {
            oi_setWheels(-250, 250);
            timer_waitMillis(200);
            //oi_setWheels(0,-0);

        }
        else if(byte == 'S') {
            oi_setWheels(-250,-250);
            timer_waitMillis(200);
            //oi_setWheels(0,-0);
        }
        else if(byte == 'D') {
            oi_setWheels(250,-250);
            timer_waitMillis(200);
            //oi_setWheels(0,-0);
        }
        else if(byte == 'F') {
            oi_free(sensor_data);
            return 0;
        }
        else {
            oi_setWheels(0,0);

        }
    }



}
