

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
#include "clean_data.c"
void get_num_objects_file(char file_input[]);
void get_num_objects (float data[]);
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
int main(void)
{

    // part 3
/*
    timer_init();
    lcd_init();
    cyBot_uart_init();
    cyBOT_init_Scan(0x00000007);
    cyBOT_SERVO_cal();
*/

    // part 2
/*
    timer_init();
    lcd_init();
    cyBot_uart_init();
    cyBOT_init_Scan(0x00000007);


    right_calibration_value = 295750;
    left_calibration_value = 1272250;

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



    //part 5

    cyBot_uart_init();
    //get_num_objects("exp1_clean.txt");
    //clean_data(sensor_data_array);
    get_num_objects(sensor_data_array);





	return 0;

}
/*// Uncomment out after getting data
void get_num_objects_file(char file_input[]) {
    FILE *file = fopen(file_input, "r");
    if (file == NULL) {
        return;
    }
    int angle, j;
    float distance, last_distance;
    fscanf(file, "%*[^\n]\n"); // Skip first line?
    int current_obj_width = 0;
    int object_count = 0;
    while(fscanf(file,"%d%f", &angle, &distance) == 2) {
        if(distance < 1.8) { // When we see object
            current_obj_width += 2;
            last_distance = distance;

        }
        if ((current_obj_width > 0) && distance >= 2.0) { // At the end of an object
            object_count++;
            double radians = current_obj_width * (3.1415 / 180.0);
            float linear_width = sqrt(2*pow(distance, 2)*(1-cos(radians)));
            char line[35] = "                                ";
            snprintf(line, sizeof(line), "%d %d %f %d %f\n", object_count + 1,  angle - current_obj_width, last_distance, current_obj_width, linear_width);
            printf("%d %d %f %d %f\n", object_count + 1,  angle - current_obj_width, last_distance, current_obj_width, linear_width);
            for(j = 0; line[j] != '\0'; j++) {
                cyBot_sendByte(line[j]);
            }
            cyBot_sendByte('\r');
            cyBot_sendByte('\n');
            current_obj_width = 0;
        }
    }
    fclose(file);


}
*/

void get_num_objects (float data[]) {
    int angle = 0;
    float distance, last_distance;
    int current_obj_width = 0;
    int object_count = 0;
    int i = 0;
    float smallest = 10000;
    char heading[] = "Object#     Angle       Distance     Width\n\r";
    int z;
    for(z = 0; heading[z] != '\0'; z++) {
        cyBot_sendByte(heading[z]);
    }
    while(angle <= 180) {
        distance = data[i];
        if(distance < 1.3) { // When we see object
            current_obj_width += 2;
            last_distance = distance;
        }
        if (((current_obj_width > 0) && distance >= 1.3)) { // At the end of an object
            object_count++;
            double radians = current_obj_width * (3.1415 / 180.0);
            float linear_width = 2*last_distance*sin(radians/2);

            if(linear_width < smallest) {
                smallest = linear_width;
            }


            char line[50] = "                                ";
            snprintf(line, sizeof(line), "%d            %d            %.1f          %.1f\n", object_count,  angle - current_obj_width, last_distance, linear_width);

            int j;
            for(j = 0; line[j] != '\0'; j++) {
                cyBot_sendByte(line[j]);
            }




            cyBot_sendByte('\r');
            cyBot_sendByte('\n');
            current_obj_width = 0;
        }
        angle += 2;
        i++;
    }
    char line[50] = "                                ";
    snprintf(line, sizeof(line), "%.1f\r\n", smallest);
    int j;
    for(j = 0; line[j] != '\0'; j++) {
        cyBot_sendByte(line[j]);
    }
}



