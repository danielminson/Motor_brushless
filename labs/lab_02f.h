#include "mbed.h"

DigitalIn button(PC_13);
DigitalOut led (PA_5);

int main() {

  while (true) {
      if(button.read()==0){ led = 1;}
      else{ led = 0;}
  }
}
