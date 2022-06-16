#include "mbed.h"

InterruptIn hall1(PA_15);
InterruptIn hall2(PB_3);
InterruptIn hall3(PB_10);

bool flag_commute;

void callback_commute()
{
    flag_commute = true;
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
            printf("%d\t%d\t%d\n",hall1.read(),hall2.read(),hall3.read());
        }
    } 
}

