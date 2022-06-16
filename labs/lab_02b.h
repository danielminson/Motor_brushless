#include "mbed.h"

DigitalOut led(PA_5);

Ticker tic_blink;

void callback_blink() { led = !led; }

int main() {

  tic_blink.attach(&callback_blink, 500ms);
  while (true) {
  }
}