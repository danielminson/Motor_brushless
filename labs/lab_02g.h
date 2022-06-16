#include "mbed.h"

InterruptIn button(PC_13);
DigitalOut led(PA_5);

bool flag_blink = true;

void callback_blink() { flag_blink = true; }

int main() {
  button.rise(&callback_blink);
  while (true) {
    if (flag_blink) {
      flag_blink = false;
      led = !led;
    }
  }
}
