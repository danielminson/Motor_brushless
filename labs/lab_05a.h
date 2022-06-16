#include "mbed.h"

InterruptIn hall1(PA_15);
InterruptIn hall2(PB_3);
InterruptIn hall3(PB_10);
PwmOut IN_1 (PA_8); // pinos placa IHM07M1 - Usa para PWM
PwmOut IN_2 (PA_9);
PwmOut IN_3 (PA_10);
DigitalOut EN_1 (PC_10); // pinos placa IHM07M1 ENABLE
DigitalOut EN_2 (PC_11);
DigitalOut EN_3 (PC_12);
DigitalIn botao(PC_13);

bool flag_commute;

bool flag_pwm;

AnalogIn porta_ana(PB_1);

float pwm;

Ticker tic_pwm;

void callback_commute()
{
    flag_commute = true;
}

void callback_pwm(){
    flag_pwm = true;
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

void step1(){
    EN_1 = 1; IN_1 = pwm;
    EN_2 = 0; IN_2 = 0;
    EN_3 = 1; IN_3 = 0;
}
void step2(){
    EN_1 = 1; IN_1 = pwm;
    EN_2 = 1; IN_2 = 0;
    EN_3 = 0; IN_3 = 0;
}
void step3(){
    EN_1 = 0; IN_1 = 0;
    EN_2 = 1; IN_2 = 0;   
    EN_3 = 1; IN_3 = pwm;
}
void step4(){
    EN_3 = 1; IN_3 = pwm;
    EN_1 = 1; IN_1 = 0;
    EN_2 = 0; IN_2 = 0;
}
void step5(){
    EN_1 = 1; IN_1 = 0;
    EN_2 = 1; IN_2 = pwm;
    EN_3 = 0; IN_3 = 0;
}
void step6(){
    EN_1 = 0; IN_1 = 0;
    EN_2 = 1; IN_2 = pwm;
    EN_3 = 1; IN_3 = 0;
}

void step_disabled(){
    EN_1 = 0; IN_1 = 0;
    EN_2 = 0; IN_2 = 0;
    EN_3 = 0; IN_3 = 0;
}

void commutation( int step){
    switch (step) {
            case 1:    // sensor is covered
                //printf("passo 1\n");
                step1();     
                break;
            case 2:    // sensor in dim light
                //printf("passo 2\n");
                step2();
                break;    
            case 3:    // sensor in medium light
                //printf("passo 3\n");
                step3();     
                break;   
            case 4:    // bright light shining on sensor
                //printf("passo 4\n");
                step4();        
                break;
            case 5:    // bright light shining on sensor
                //printf("passo 5\n");
                step5();      
                break;  
            case 6:    // bright light shining on sensor
                //printf("passo 6\n");
                step6();     
                break;   
            default:
                printf("error\n");
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
    IN_1.period_us(50);     
    IN_2.period_us(50);     
    IN_3.period_us(50);   

    tic_pwm.attach(&callback_pwm, 100ms);
  
    while (true) 
    {
        if (flag_pwm) // Cada 100ms
        {
            flag_pwm = false;
            pwm = porta_ana.read();
            if (botao == 1 || pwm < 0.1) { pwm = 0; }
            commutation(valor_hall_atual);
        }
        else if(flag_commute) // Sempre hall mudar
        {
            flag_commute = false;
            valor_hall_atual = get_step();
            commutation(valor_hall_atual);
           // printf("%f\n", pwm);
        }
    }
}

