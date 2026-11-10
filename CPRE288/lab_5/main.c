

/**
 * main.c
 */
#include "cyBot_Scan.h"
#include "cyBot_uart.h"
#include "Timer.h"
#include "lcd.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "sensor-data.h"
#include "clean_data.h"
#include "count_objects.h"
#include "open_interface.h"
//#include "movement.h"
#include "count_objects.h"
//void get_num_objects_file(char file_input[]);
//void get_num_objects (float data[]);
//285250 R
//1246000 L
//25 CyBot


//243250 R
//1204000 L
//12 CyBot

//295750 R
//1272250 L
//23 CyBot

//269500 R
//1267000 L
// 09 Cybot

//259000 R
//1235500 L
//24 CyBot

//301000 R
//1261759 L
//08 CyBot

//232750 R
//1288000 L
// 22 Cybot


//
void scan(cyBOT_Scan_t scan);
float sensor_data_array[] = {
    1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5,
    0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3,
    0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3,
    0.3, 0.3, 0.3,
    1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5,
    1.6,
    1.7, 1.7, 1.7, 1.7, 1.7, 1.7, 1.7, 1.7, 1.7, 1.7, 1.7, 1.7, 1.7, 1.7,
    1.9,
    0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6
};
int main(void)
{

    timer_init();
    lcd_init();
    cyBot_uart_init();
    cyBOT_init_Scan(0x00000007);
    cyBOT_SERVO_cal();

 //part 2
/*
    timer_init();
    lcd_init();
    cyBot_uart_init();
    cyBOT_init_Scan(0x00000007);


    right_calibration_value = 232750;
    left_calibration_value = 1288000;
    int smallest_position, num_objects;
    cyBOT_Scan_t scan_data;
    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);
    scan_data.IR_raw_val = 0;
    scan_data.sound_dist = 0;
    scan(scan_data);
    clean_data(sensor_data_array);
    num_objects = get_num_objects(sensor_data_array, &smallest_position);

    // noisy data
    /*
    if(smallest_position < 90) {// If object is to the right
        turn_right_degrees(sensor_data, 90-smallest_position);
    } else if(smallest_position > 90) {// If object is to the left
        turn_left_degrees(sensor_data, smallest_position-90);
    } else { // If object is in front of it

    }



    timer_waitMillis(200);
    // Clean Data
    if(smallest_position < 90) {// If object is to the right
        turn_right_degrees(sensor_data, 90-smallest_position);
    } else if(smallest_position > 90) {// If object is to the left
        turn_left_degrees(sensor_data, smallest_position-90);
    } else { // If object is in front of it
        return 0;
    }

*/






/*
    cyBOT_Scan_t scan;
    int i, j, width;
    char heading[] = "Angle(Degrees)    Distance(m)\r\n";
    for(j = 0; heading[j] != '\0'; j++) {
        cyBot_sendByte(heading[j]);
    }
    int z;
    for(z = 0; z < 5; z++) { // Delay to not collect garbage
        cyBOT_Scan(0, &scan);
            }


    for (i = 0; i <= 180; i+=2) {
        char line[35] = "                                ";
        cyBOT_Scan(i, &scan);

        if(i < 10) {
            width = 28;
        } else if(i >= 10 && i < 100) {
            width = 27;
        } else if (i >= 100 && i < 1000) {
            width = 26;
        }
        snprintf(line, sizeof(line), "%d%*.1f",i, width,scan.sound_dist/100 );
        for(j = 0; line[j] != '\0'; j++) {
            cyBot_sendByte(line[j]);
                }

                cyBot_sendByte('\r');
                cyBot_sendByte('\n');
            }
*/
/*
    get_num_objects(sensor_data_array);
    clean_data(sensor_data_array);
    get_num_objects(sensor_data_array);
*/
 //part 3
/*
    timer_init();
    lcd_init();
    cyBot_uart_init();
    cyBOT_init_Scan(0x7);

    cyBOT_Scan_t scan_data;
    right_calibration_value = 232750;
    left_calibration_value = 1288000;


    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);

    while(1) {
            int byte = cyBot_getByte();
            if(byte == 'W') {
                move_forward(sensor_data, .05);
            }
            else if(byte == 'A') {
                turn_left_degrees(sensor_data, 15);
            }
            else if(byte == 'S') {
                move_back_distance(sensor_data, .05);
            }
            else if(byte == 'D') {
                turn_right_degrees(sensor_data, 15);
            }
            else if(byte == 'F') {
                oi_free(sensor_data);
                return 0;
            }
            else if(byte == 'C') { // Scan
                scan(scan_data);
            }
            else {
                oi_setWheels(0,0);

            }
        }
*/
	return 0;
}

void scan(cyBOT_Scan_t scan) {
    char heading[] = "Angle(Degrees)    Distance(m)\r\n";


    int z, j, width, i;
    for(j = 0; heading[j] != '\0'; j++) {
        cyBot_sendByte(heading[j]);
    }
    for(z = 0; z < 5; z++) { // Delay to not collect garbage
        cyBOT_Scan(0, &scan);
    }


    for (i = 0; i <= 180; i+=2) {
        timer_waitMillis(100); // This might help collecting data
        char line[35] = "";

        // scan twice for each angle
        cyBOT_Scan(i, &scan);
        float first_distance = scan.sound_dist;
        cyBOT_Scan(i, &scan);
        float second_distance = scan.sound_dist;

        // get the average for cleaner data
        float average_distance = (first_distance + second_distance) / 2;

        if(i < 10) {
            width = 28;
        } else if(i >= 10 && i < 100) {
            width = 27;
        } else if (i >= 100 && i < 1000) {
            width = 26;
        }
        sensor_data_array[i/2] = average_distance/100;


        snprintf(line, sizeof(line), "%d%*.1f\t",i, width,average_distance/100 );
        for(j = 0; line[j] != '\0'; j++) {
            cyBot_sendByte(line[j]);
        }

        cyBot_sendByte('\r');
        cyBot_sendByte('\n');
    }
    clean_data(sensor_data_array);


    for(i = 0; i <= 180; i+=2) {
        if(i < 10) {
            width = 28;
        } else if(i >= 10 && i < 100) {
            width = 27;
        } else if (i >= 100 && i < 1000) {
            width = 26;
        }
        char line[35] = "";
        snprintf(line, sizeof(line), "%d%*.1f",i, width,scan.sound_dist/100 );
        for(j = 0; line[j] != '\0'; j++) {
            cyBot_sendByte(line[j]);
        }

        cyBot_sendByte('\r');
        cyBot_sendByte('\n');

    }
    get_num_objects(sensor_data_array, NULL);

}
