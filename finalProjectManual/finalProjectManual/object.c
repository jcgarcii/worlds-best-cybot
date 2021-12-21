/*
 * object.c
 *
 *  Created on: Apr 8, 2021
 *      Author: jmhafele
 */

#include "object.h"

float width_conversion = 0;
bool width_calibrated;

void object_init(void){
    servo_init();
    adc_init();
    ping_init();
    servo_calibrate();
    width_calibrated = false;
}


int object_scan(Object objects[], int delay){
    int ir_cm = 0;
    int ir_cm_previous = 300;
    double object_angle_start = 0;
    double object_angle_stop = 0;
    int object_dist = 0;
    int object_index = 0;

    bool detected_object = false;

//    char message[100];
//    sprintf(message, "\n\rDegrees \tIR Dist (cm) \tSonar Dist(cm)");
//    uart_sendStr(message);

    int i;
    for(i = 0; i <= 180; i++){
        timer_waitMillis(delay);
        ir_cm = adc_read();
        servo_move(i);
//        sprintf(message, "\n\r%d \t\t%d \t\t%0.2f", i, ir_cm, ping_cm);
//        uart_sendStr(message);

        //Start of object
        if((ir_cm_previous >= ir_cm + 100 || ir_cm_previous <= ir_cm - 100) && !detected_object && ir_cm <= 120){
            object_angle_start = i;
            object_dist = ping_getDistance();
            detected_object = true;
        }

        //End of object
        else if((ir_cm_previous >= ir_cm + 100 || ir_cm_previous <= ir_cm - 100) && detected_object){
            object_angle_stop = i;
            detected_object = false;

            //process object data
            int object_angle = (object_angle_stop - object_angle_start)/2 + object_angle_start;
            double object_width;
            double object_width_raw = object_dist * tan((3.14159/180.0) * (object_angle_stop - object_angle_start));
            if(!width_calibrated){
                object_width = object_width_raw;
            }
            else {
                object_width = object_width_raw - object_dist * width_conversion;
            }


            //Builds object struct with data processed
            objects[object_index].angle = object_angle;
            objects[object_index].distance = object_dist;
            objects[object_index].width = object_width;

            object_index++;
        }

        ir_cm_previous = ir_cm;

    }
    print_objects(objects, object_index);
    return object_index;
}

void print_objects(Object objects[], int num_objects){
    char putty_message[100];
    char lcd_message[100];
    if(num_objects < 1){
        sprintf(putty_message, "\n\r No objects detected");
        uart_sendStr(putty_message);
        return;
    }
    sprintf(putty_message, "\n\r Index \t Angle \t Distance(cm) \t Width(cm)");
    uart_sendStr(putty_message);

    int i;
    for(i = 0; i < num_objects; i++){
        sprintf(putty_message,"\n\r %d \t %d \t %0.1f \t\t %0.1f", i, objects[i].angle, objects[i].distance, objects[i].width);
        uart_sendStr(putty_message);
        sprintf(lcd_message, "Index: %d \nAngle: %d \nDist(cm): %0.1f \nWidth(cm): %0.1f", i, objects[i].angle, objects[i].distance, objects[i].width);
        lcd_printf(lcd_message);
    }
}

void width_calibrate(Object objects[]){
    char putty[] = "\n\rPlace large tall object as close to 10 cm as possible at 90 degrees \n\r press any char when ready \n\r";
    uart_sendStr(putty);
    uart_receive();
    object_scan(objects, 50);
    double close_distance = objects[0].distance;
    double close_width = objects[0].width;

    char putty2[] = "\n\rMove large tall object as close to 100 cm as possible at 90 degrees \n\r press any char when ready \n\r";
    uart_sendStr(putty2);
    uart_receive();
    object_scan(objects, 50);
    double far_distance = objects[0].distance;
    double far_width = objects[0].width;

    width_conversion = (far_width - close_width)/(far_distance - close_distance);
    width_calibrated = true;
}
