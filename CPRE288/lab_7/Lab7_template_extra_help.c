/**
 * lab7_template_extra_help.c
 *
 * Description: This is file is meant for those that would like a little
 *              extra help with formatting their code.
 *
 */
#define _SERVOCALIBRATE 0
#define _PART1 0
#define _PART2 0
#define _PART3 0
#define _PART4 1

#include "timer.h"
#include "lcd.h"
#include "uart_extra_help.h"
#include "cyBot_Scan.h"
#include "movement.h"
#include "open_interface.h"
#include "clean_data.h"
#include "count_objects.h"
#include <string.h>
#include <math.h>

// Adding global volatile varibles for communcating between 
// your Interupt Service Routine, and your non-interupt code.
volatile  char uart_data;  // Your UART interupt code can place read data here
volatile  char flag;       // Your UART interupt can update this flag
                           // to indicate that it has placed new data
                           // in uart_data                     
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
float ir_data_array[] = {
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
void scan(cyBOT_Scan_t scan);
void main()
{

  timer_init();
  lcd_init();
  lcd_clear();
  char buffer[21] = "";
  int num_chars = 0;

#if _PART1      // Receive and display text
  uart_init(115200);
  while (1) {
      char data = uart_receive();

      if(num_chars >= 20) { // When there are 20 characters
          buffer[num_chars] = '\0';
          lcd_clear();
          lcd_puts(buffer);
          buffer[0] = data; // first letter of next data
          num_chars = 1;
      } else if(data == '\r') { // When enter
          buffer[num_chars] = '\0';
          lcd_clear();
          lcd_puts(buffer);
          num_chars = 0;
      } else { // Don't display anything yet
          buffer[num_chars] = data;
          num_chars++;
      }

  }

#endif

#if _PART2      // Echo Received Character
  while(1){
      uart_init(115200);
      char data = uart_receive();
      if(data == '\r') {
          uart_sendChar('\n');
          uart_sendChar('\r');
      } else {
          uart_sendChar(data);
      }

  }

#endif

#if _PART3 // Receive characters using interrupts.
  uart_init(115200);
  uart_interrupt_init();
  while(1) {
    if(flag) { 
        flag = 0;
        char data = uart_data;
 
        if(data == '\r') {
            uart_sendChar('\n');
            uart_sendChar('\r');
        } else {
            uart_sendChar(data);
        }
    }

  }

 
#endif

#if _PART4
  uart_init(115200);
  cyBOT_init_Scan(0x7);

  cyBOT_Scan_t scan_data;
  right_calibration_value = 222250;
  left_calibration_value = 1193500;


  oi_t *sensor_data = oi_alloc();
  oi_init(sensor_data);

  while(1) {
          char byte = uart_receive();
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
          }
          else if(byte == 'C') { // Scan
              scan(scan_data);
          }
          else if(byte == 'I') { // IR sensor reading
              cyBOT_Scan(90, &scan_data);
              timer_waitMillis(200); // wait to get data
              int z;
              float average, IR_scaled;
              float accumulator = 0;
              float values[5];
              for (z = 0;z < 5; z++) {
                  if (scan_data.IR_raw_val < 1460)
                  {
                      IR_scaled = 0.00006067*pow(scan_data.IR_raw_val, 2)-.21196* scan_data.IR_raw_val +209.041 - 1;
                  }
                  else {
                      IR_scaled = (112.8276*pow(M_E, (-0.00108*scan_data.IR_raw_val))+4.831) - 1;
                  }
                  accumulator += IR_scaled;
                  values[z] = IR_scaled;
              }
              float middle_values[3];
              int biggest = values[0];
              int smallest = values[0];

              for(z = 1; z < 5; z++) {
                  if(values[z] > biggest) {
                      biggest = values[z];
                  }
                  if(values[z] < smallest) {
                      smallest = values[z];
                  }
              }
              int j = 0;
              for(z = 0; z < 5; z++) {
                  if(values[z] != biggest || values[z] != smallest) {
                      middle_values[j] = values[z];
                      j++;
                  }
              }
              average = (middle_values[0] + middle_values[1] +  middle_values[2]) / 3;
              if(average < 10) { // average when between 9 and 12
                  average += 2;
              }
              if(average > 37) {
                  average -= 1;
              }

              char heading[] = "IR Value\r\n";
              int  i;
              for(j = 0; heading[j] != '\0'; j++) {
                  uart_sendChar(heading[j]);
              }

              char line[10] = "";

              snprintf(line, sizeof(line), "%f", average);
              for(j = 0; line[j] != '\0'; j++) {
                  uart_sendChar(line[j]);
              }

              uart_sendChar('\r');
              uart_sendChar('\n');

          }
          else {
              oi_setWheels(0,0);
              }
          }


#endif

#if _SERVOCALIBRATE
  cyBOT_init_Scan(0x00000007);
  cyBOT_SERVO_cal();
#endif
}


  void scan(cyBOT_Scan_t scan) {
      char heading[] = "Angle(Degrees)    Distance(m)   IR(m)\r\n";


      int z, j, width, i;
      for(j = 0; heading[j] != '\0'; j++) {
          uart_sendChar(heading[j]);
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




          // IR portion
          int z;
          float average, IR_scaled;
          float accumulator = 0;
          float values[5];
          for (z = 0;z < 5; z++) {
              if (scan.IR_raw_val < 1460)
              {
                  IR_scaled = 0.00006067*pow(scan.IR_raw_val, 2)-.21196*scan.IR_raw_val+209.041 - 1;
              }
              else {
                  IR_scaled = (112.8276*pow(M_E, (-0.00108*scan.IR_raw_val))+4.831) - 1;
              }
              accumulator += IR_scaled;
              values[z] = IR_scaled;
          }
          float middle_values[3];
          int biggest = values[0];
          int smallest = values[0];

          for(z = 1; z < 5; z++) {
              if(values[z] > biggest) {
                  biggest = values[z];
              }
              if(values[z] < smallest) {
                  smallest = values[z];
              }
          }
          int j = 0;
          for(z = 0; z < 5; z++) {
              if(values[z] != biggest || values[z] != smallest) {
                  middle_values[j] = values[z];
                  j++;
              }
          }
          average = (middle_values[0] + middle_values[1] +  middle_values[2]) / 3;
          if(average < 10) { // average when between 9 and 12
              average += 2;
          }
          if(average > 37) {
              average -= 1;
          }


          ir_data_array[i] = average/100;







          if(i < 10) {
              width = 28;
          } else if(i >= 10 && i < 100) {
              width = 27;
          } else if (i >= 100 && i < 1000) {
              width = 26;
          }
          sensor_data_array[i/2] = average_distance/100;


          snprintf(line, sizeof(line), "%d%*.1f\t%f",i, width,average_distance/100, average );
          for(j = 0; line[j] != '\0'; j++) {
              uart_sendChar(line[j]);
          }

          uart_sendChar('\r');
          uart_sendChar('\n');
      }
      clean_data(sensor_data_array);
      clean_data(ir_data_array);
/*
      for(z = 0; z <= 180; z+=2) {
          if(z < 10) {
              width = 28;
          } else if(z >= 10 && z < 100) {
              width = 27;
          } else if (z >= 100 && z < 1000) {
              width = 26;
          }
          char line[35] = "";
          snprintf(line, sizeof(line), "%d%*.1f",i, width,scan.sound_dist/100 );
          for(j = 0; line[j] != '\0'; j++) {
              uart_sendChar(line[j]);
          }

          uart_sendChar('\r');
          uart_sendChar('\n');

      }
      */
      get_num_objects(ir_data_array, NULL);
      get_num_objects(sensor_data_array, NULL);

  }
