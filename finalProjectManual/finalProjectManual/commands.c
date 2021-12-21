#include "commands.h"

int move_bot(oi_t *sensor_data, Object objects[]) {
    char input;
    bool is_cancelled = false;
    print_instructions();
    input = uart_receive();

    if(input == 'w' || input == 's'){
        char instruction[100];
        if(input == 'w'){
            sprintf(instruction, "\n\rType in distance (cm) to go forward \n\renter: stop input \n\r");
        }
        else if(input == 's'){
            sprintf(instruction, "\n\rType in distance (cm) to go backward \n\renter: stop input \n\r");
        }
        double dist = 0;
        while(dist == 0){
            uart_sendStr(instruction);
            dist = get_double_uart();
            is_cancelled = verify_double(dist);
            if(is_cancelled){
                break;
            }
            if(dist > 400){
                char error[] = "\n\r Distance too long, please re-enter \n\r";
                uart_sendStr(error);
                dist = 0;
            }
        }

        if(input == 'w' && !is_cancelled){
            move_forward(sensor_data, dist * 10);
        }
        else if(input == 's' && !is_cancelled){
            move_backward(sensor_data, dist * 10);
        }
    }

    else if(input == 'd' || input == 'a'){
        char instruction[100];
        if(input == 'd'){
            sprintf(instruction, "\n\rType in angle (degrees) to turn right\n\renter: stop input \n\r");
        }
        else if(input == 's'){
            sprintf(instruction, "\n\rType in angle (degrees) to turn left\n\renter: stop input \n\r");
        }
        double angle = 0;
        while(angle == 0){
            uart_sendStr(instruction);
            angle = get_double_uart();
            is_cancelled = verify_double(angle);
            if(is_cancelled){
                break;
            }
            if(angle > 360){
                char error[] = "\n\r Angle too large, please re-enter \n\r";
                uart_sendStr(error);
                angle = 0;
            }
        }
        if(input == 'a' && !is_cancelled){
            turn_left(sensor_data, angle);
        }
        else if(input == 'd' && !is_cancelled){
            turn_right(sensor_data, angle);
        }
    }

    else if (input == 'e') { //scan surroundings
        char scanning[] = "\r\nScanning the clean..... please wait......";
        uart_sendStr(scanning);
        object_scan(objects, 50);
    }

    else if(input == 'r'){
        char scanning[] = "\r\nScanning the dirty..... please wait......";
        uart_sendStr(scanning);
        object_scan(objects, 10);
    }

    else if(input == 'q'){
        char fault_message[] = "\r\nChecking faults now...";
        uart_sendStr(fault_message);
        oi_update(sensor_data);
        if(!check_bump(sensor_data) && !check_border(sensor_data) && !check_pitfall(sensor_data)){
            char no_fault[] = "\r\nNo faults detected. Good job!";
            uart_sendStr(no_fault);
        }
    }

    else if (input == 'h'){
        char done[] = "\r\nObject delivered. Powering down now :)";
        uart_sendStr(done);
        return 1;
    }
    return 0;
}

double get_double_uart(void){
    char putty[] = "\n\r t: enter final input \n\r z: +1 \t x: -1 \t c: +10 v: -10 b: +90 n: -90\n\r";
    uart_sendStr(putty);
    char input;
    double value = 0;
    while(!command_flag){
        input = uart_receive();
        if(input == 'z'){
            value++;
        }
        else if(input == 'x'){
            value--;
        }
        else if(input == 'c'){
            value = value + 10;
        }
        else if(input == 'v'){
            value = value - 10;
        }
        else if(input == 'b'){
            value = value + 90;
        }
        else if(input == 'n'){
            value = value - 90;
        }

        if(value < 0){
            value = 0;
        }
        char putty2[100];
        sprintf(putty2, "\n\rValue: %.0f \n\r t: enter final input \t z: +1 \t x: -1 \t c: +10 v: -10 b: +90 n: -90\n\r", value);
        uart_sendStr(putty2);
    }
    command_flag = 0;
    return value;
}

double get_double_button(void){
    char putty[] = "\n\r t: enter final input \n\r b1: +1 \t b2: -1 \t b3: +10 \t b4: -10 \n\r";
    uart_sendStr(putty);
    double value = 0;
    while(!command_flag){
        int button = button_getButton();
        switch(button){
            case 1:
                value++;
            break;

            case 2:
                value--;
            break;

            case 3:
                value = value + 10;
            break;

            case 4:
                value = value - 10;
            break;
        }
        if(value < 0){
            value = 0;
        }
        timer_waitMillis(250);
        lcd_printf("Value: %f\nb1: +1  b2: -1\nb3: +10 b4: -10", value);
    }
    command_flag = 0;
    return value;
}

bool verify_double(double value){
    char message[100];
    sprintf(message, "\n\r Is %0.2f what you entered? (y/n) \n\r", value);
    uart_sendStr(message);
    char input = uart_receive();
    while(input != 'y' && input != 'n'){
        input = uart_receive();
    }
    return input != 'y';
}

void print_instructions(void){
    char instruction[] = "\n\r\n\r w: move forward \n\r s: move backward \n\r d: turn right \n\r a: turn left \n\r q: check faults \n\r e: accurate 180 degree scan \n\r r: fast 180 degree scan\n\r h: power down \n\r";
    uart_sendStr(instruction);
}
