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
int contador;
int tempo;
int valor_pot;

bool novo_passo;

float pwm;

float map(float x, float in_min, float in_max, float out_min, float out_max)//definição de uma função de regra de 3
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void callback_passo_novo(){
    novo_passo = true;
    contador ++;
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
        // printf("entrou\n");
        contador = 0;
        switch (step) {
            case 0: // sensor is covered
                // printf("passo 1\n");
                step1();
                break;
            case 1: // sensor in dim light
                // printf("passo 2\n");
                step2();
                break;
            case 2: // sensor in medium light
                // printf("passo 3\n");
                step3();
                break;
            case 3: // bright light shining on sensor
                // printf("passo 4\n");
                step4();
                break;
            case 4: // bright light shining on sensor
                // printf("passo 5\n");
                step5();
                break;
            case 5: // bright light shining on sensor
                // printf("passo 6\n");
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
    tick_commutation.attach(&callback_passo_novo, 1ms);
    novo_passo = false;
    contador = 0;

    while (true) {
        valor_pot = porta_ana.read_u16();// leitura do valor do potenciometro com 16 bits
        // tempo = map(valor_pot, 0, 65535, 50, 20);
        tempo = 1;
        // printf("%d\n", tempo);
        pwm = map(valor_pot, 0, 65535, 0, 1);
        // pwm = 0.8;    
        if(novo_passo && contador >=tempo){
            printf("entrou, tempo: %d, pwm %f\n", tempo, pwm);
            contador = 0;
            novo_passo = false;
            commutation(passo);  
            passo++;
            if (passo == 6){
                passo = 0;
            }
        } 
    }
}

