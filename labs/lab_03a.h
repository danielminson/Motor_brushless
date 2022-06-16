#include "mbed.h"
PwmOut IN_1 (PA_8); // pinos placa IHM07M1 - Usa para PWM
PwmOut IN_2 (PA_9);
PwmOut IN_3 (PA_10);
DigitalOut EN_1 (PC_10); // pinos placa IHM07M1 ENABLE
DigitalOut EN_2 (PC_11);
DigitalOut EN_3 (PC_12);

AnalogIn porta_ana(PB_1);

DigitalIn botao(PC_13);//definição do botão de usuário

Ticker tick_commutation;

int passo;

bool novo_passo;

float pwm;

void callback_passo_novo(){
    novo_passo = true;
}


void step1(){
    EN_1 = 1; IN_1 = pwm;
    EN_2 = 1; IN_2 = 0;
    EN_3 = 0; IN_3 = 0;
}
void step2(){
    EN_1 = 1; IN_1 = pwm;
    EN_2 = 0; IN_2 = 0;
    EN_3 = 1; IN_3 = 0;
}
void step3(){
    EN_1 = 0; IN_1 = 0;
    EN_2 = 1; IN_2 = pwm;
    EN_3 = 1; IN_3 = 0;
}
void step4(){
    EN_3 = 0; IN_3 = 0;
    EN_1 = 1; IN_1 = 0;
    EN_2 = 1; IN_2 = pwm;
}
void step5(){
    EN_1 = 1; IN_1 = 0;
    EN_2 = 0; IN_2 = 0;
    EN_3 = 1; IN_3 = pwm;
}
void step6(){
    EN_1 = 0; IN_1 = 0;
    EN_2 = 1; IN_2 = 0;
    EN_3 = 1; IN_3 = pwm;
}

void commutation( int step){
    switch (step) {
            case 0:    // sensor is covered
                printf("passo 1\n");
                step1();     
                break;
            case 1:    // sensor in dim light
                printf("passo 2\n");
                step2();
                break;    
            case 2:    // sensor in medium light
                printf("passo 3\n");
                step3();     
                break;   
            case 3:    // bright light shining on sensor
                printf("passo 4\n");
                step4();        
                break;
            case 4:    // bright light shining on sensor
                printf("passo 5\n");
                step5();      
                break;  
            case 5:    // bright light shining on sensor
                printf("passo 6\n");
                step6();     
                break;   
            default:
                printf("error\n");
}
}


// main() runs in its own thread in the OS
int main()
{
    passo = 0;
    tick_commutation.attach(&callback_passo_novo, 2000ms);
    novo_passo = false;

    while (true) {
        // pwm = porta_ana.read();
        pwm = 1.0;    
        if(novo_passo){
            novo_passo = false;
            commutation(passo);  
            passo++;
            if (passo == 6){
                passo = 0;
            }
        } 
    }
}

