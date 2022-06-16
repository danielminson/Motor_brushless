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
DigitalOut zero_crossing (PA_5);
DigitalIn botao(PC_13);
AnalogIn BEMF_1(PC_3);
AnalogIn BEMF_2(PB_0);
AnalogIn BEMF_3(PB_15);

AnalogIn porta_ana(PB_1);

bool flag_commute = true;
bool flag_pwm;
bool flag_speed;
bool flag_print;
// bool estado_zero_crossing;

float omega_r;
float omega_e;
float omega_m;
float omega;
float kp = 5.2915e-04;
float ki = 0.006;
float alpha = 0.8;
float low_threshold = 0.05;
float high_threshold = 0.95;

int omega_e_int;
int valor_back_emf;

Ticker tic_pwm;
Ticker tic_speed;
Ticker tic_print;

void callback_commute() {
    flag_commute = true; 
    zero_crossing = !zero_crossing;
}
void callback_pwm(){ flag_pwm = true; }
void callback_speed(){ flag_speed = true; }
void callback_print(){ flag_print = true; }

float pwm;
float vel;
int contador;
int valor_hall_atual = 0;

int get_step(int step)
{
    switch (step) {
        case 1:
            if( BEMF_2.read()<low_threshold){
                step++;
            }//checar se é igual a 0

            break;
        case 2:
            if( BEMF_3.read()<high_threshold){
                step++;
            }
            break;
        case 3:
            if( BEMF_1.read()<low_threshold){
                    step++;
            }
            break;
        case 4:
            if( BEMF_2.read()<high_threshold){
                step++;
            }
            break;
        case 5:
            if( BEMF_3.read()<low_threshold){
                step++;
            }            
            break;
        case 6:
            if( BEMF_1.read()<high_threshold){
                step++;
            }
            break;
        
    return step;
}

//

void commutation(int step){
    switch (step) {
            case 1:    // pwm 1 e gnd 3
                EN_1 = 1; IN_1 = pwm;
                EN_2 = 0; IN_2 = 0;
                EN_3 = 1; IN_3 = 0;    
                break;
            case 2:    // pwm 1 e gnd 2
                EN_1 = 1; IN_1 = pwm;
                EN_2 = 1; IN_2 = 0;
                EN_3 = 0; IN_3 = 0;
                break;    
            case 3:    // pwm 3 e gnd 2
                EN_1 = 0; IN_1 = 0;
                EN_2 = 1; IN_2 = 0;   
                EN_3 = 1; IN_3 = pwm;     
                break;   
            case 4:    // pwm 3 e  gnd 1
                EN_3 = 1; IN_3 = pwm;
                EN_1 = 1; IN_1 = 0;
                EN_2 = 0; IN_2 = 0;       
                break;
            case 5:    // pwm 2 e gnd 1
                EN_1 = 1; IN_1 = 0;
                EN_2 = 1; IN_2 = pwm;
                EN_3 = 0; IN_3 = 0;    
                break;  
            case 6:    // pwm 2 e gnd 3
                EN_1 = 0; IN_1 = 0;
                EN_2 = 1; IN_2 = pwm;
                EN_3 = 1; IN_3 = 0;    
                break;   
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
    tic_speed.attach(&callback_speed, 10ms);
    
    zero_crossing = 0;
    
    contador = 0;
    vel = 0;

    while (true) 
    {
        if (flag_pwm) // Cada 100ms
        {
            flag_pwm = false;
            pwm = porta_ana.read();
            if (botao == 1 || pwm < 0.1) { pwm = 0; }
            commutation(valor_hall_atual);
        }
        if(flag_commute) // Sempre hall mudar
        {
            flag_commute = false;
            valor_hall_atual = get_step();
            commutation(valor_hall_atual);
            contador++; 
           // printf("%f\n", pwm);
        }

        if(flag_speed){
            flag_speed = false;
            vel = (contador*3.14/3.0)/0.01;
            //printf("%.2f rad/s\n",vel);
            // printf("%.0f rpm\n",vel*30/3.14);
            contador = 0;
        }
    }
}