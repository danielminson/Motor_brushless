#include "mbed.h"

InterruptIn hall1(PA_15);
InterruptIn hall2(PB_3);
InterruptIn hall3(PB_10);

bool flag_commute;

void callback_commute()
{
    flag_commute = true;
}

int valor_hall_atual = 0;

int get_step(){
  if ((hall1 == 0) && (hall2 == 0) && (hall3 == 1)) {
    return 1;
  }
  else if ((hall1 == 0) && (hall2 == 1) && (hall3 == 1)) {
    return 2;
  }
  else if ((hall1 == 0) && (hall2 == 1) && (hall3 == 0)) {
    return 3;
  }
  else if ((hall1 == 1) && (hall2 == 1) && (hall3 == 0)) {
    return 4;
  }
  else if ((hall1 == 1) && (hall2 == 0) && (hall3 == 0)) {
    return 5;
  }
  else if ((hall1 == 1) && (hall2 == 0) && (hall3 == 1)) {
    return 6;
  }
    else{
        return 0;
    }

}

int main()
{
    hall1.rise(&callback_commute);
    hall2.rise(&callback_commute);
    hall3.rise(&callback_commute);
    hall1.fall(&callback_commute);
    hall2.fall(&callback_commute);
    hall3.fall(&callback_commute);
    while (true) 
    {
        if(flag_commute)
        {
            flag_commute = false;
            valor_hall_atual = get_step();
            printf("%d\n",valor_hall_atual);
        }
    } 
}

