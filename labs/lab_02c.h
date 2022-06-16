#include "mbed.h"

DigitalOut led(PA_5);

Ticker tic_blink;

bool flag_blink = true;

void callback_blink() { flag_blink = true; }

int main() {

  tic_blink.attach(&callback_blink, 500ms);

  while (true) {
      if (flag_blink){
          flag_blink = false;

          led = !led;
      }
  }
}