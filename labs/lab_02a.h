#include "mbed.h"

DigitalOut led(PA_5);

int main(){

    while(true){
        led = !led;

        thread_sleep_for(500);
    }
}