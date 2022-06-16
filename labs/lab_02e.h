#include "mbed.h"

AnalogIn porta_ana(PB_1);
AnalogOut led (PA_5);
Ticker tic_led;

bool flag_led = true;

void callback_led() { flag_led = true; }

int main() {

  tic_led.attach(&callback_led, 100ms);

  while (true) {
      if (flag_led){
          flag_led = false;
          led = porta_ana.read();
      }
  }
}
