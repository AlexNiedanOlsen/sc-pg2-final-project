

/**
 * main.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
void get_num_objects(char file_input[]);
//285250 R
//1246000 L
//25 CyBot


//243250 R
//1204000 L
//12 CyBot
int main(void)
{
    /* part 4
    timer_init();
    lcd_init();
    cyBot_uart_init();
    cyBOT_init_Scan(0x00000007);
    //cyBOT_SERVO_cal();

    right_calibration_value = 243250;
    left_calibration_value = 1204000;

    cyBOT_Scan_t scan;
    int i, j, width;
    char heading[] = "Angle(Degrees)    Distance(m)\r\n";
    for(j = 0; heading[j] != '\0'; j++) {
        cyBot_sendByte(heading[j]);
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

*/





    int i;
    get_num_objects("exp4_clean.txt");
    




	return 0;
}

void get_num_objects(char file_input[]) {
    FILE *file = fopen(file_input, "r");
    int angle;
    float distance, last_distance, linear_width;
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
            printf("%d %d %f %d %f\n", object_count + 1,  angle - current_obj_width, last_distance, current_obj_width, linear_width);
            current_obj_width = 0;
        }
    }
    fclose(file);

    
}

