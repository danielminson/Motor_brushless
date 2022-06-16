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

AnalogIn porta_ana(PB_1);

bool flag_commute = true;
bool flag_pwm;
bool flag_speed;

float omega_r;
float omega_e;
float omega_m;
float omega;
float kp = 5.2915e-04;
float ki = 0.006;
float alpha =0.8;

int omega_e_int;

Ticker tic_pwm;
Ticker tic_speed;

void callback_commute() { flag_commute = true; }
void callback_pwm(){ flag_pwm = true; }
void callback_speed(){ flag_speed = true; }

float pwm;
float vel;
int contador;
int valor_hall_atual = 0;

int get_step()
{
    int step = 0;
    if ((hall1 == 0) && (hall2 == 0) && (hall3 == 1)) { step = 1; }
    if ((hall1 == 0) && (hall2 == 1) && (hall3 == 1)) { step = 2; }
    if ((hall1 == 0) && (hall2 == 1) && (hall3 == 0)) { step = 3; }
    if ((hall1 == 1) && (hall2 == 1) && (hall3 == 0)) { step = 4; }
    if ((hall1 == 1) && (hall2 == 0) && (hall3 == 0)) { step = 5; }
    if ((hall1 == 1) && (hall2 == 0) && (hall3 == 1)) { step = 6; }
    return step;
}

void commutation( int step){
    switch (step) {
            case 1:    // sensor is covered
                EN_1 = 1; IN_1 = pwm;
                EN_2 = 0; IN_2 = 0;
                EN_3 = 1; IN_3 = 0;    
                break;
            case 2:    // sensor in dim light
                EN_1 = 1; IN_1 = pwm;
                EN_2 = 1; IN_2 = 0;
                EN_3 = 0; IN_3 = 0;
                break;    
            case 3:    // sensor in medium light
                EN_1 = 0; IN_1 = 0;
                EN_2 = 1; IN_2 = 0;   
                EN_3 = 1; IN_3 = pwm;     
                break;   
            case 4:    // bright light shining on sensor
                EN_3 = 1; IN_3 = pwm;
                EN_1 = 1; IN_1 = 0;
                EN_2 = 0; IN_2 = 0;       
                break;
            case 5:    // bright light shining on sensor
                EN_1 = 1; IN_1 = 0;
                EN_2 = 1; IN_2 = pwm;
                EN_3 = 0; IN_3 = 0;    
                break;  
            case 6:    // bright light shining on sensor
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

    tic_pwm.attach(&callback_pwm, 10ms);
    tic_speed.attach(&callback_speed, 10ms);
    
    contador = 0;
    vel = 0;

    while (true) 
    {
        if (flag_pwm) // Cada 100ms
        {
            flag_pwm = false;
            omega_r = porta_ana.read()*1800;//em rad/s
            if (botao == 1 || omega_r < 200) { omega_r = 0; }
            omega_e = omega_r-omega;
            omega_e_int += omega_e*0.01;
            pwm = kp*omega_e+ki*omega_e_int;
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
            omega_m = (contador*3.14/3.0)/0.01;
            omega = (1-alpha)*omega+alpha*omega_m;
            // printf("%.2f rad/s\n",vel);
            // printf("%.0f rpm\n",vel*30/3.14);
            printf("wr = %.0frad/s | w = %.0frad/s | pwm = %.2f\n", omega_r,omega,pwm);
            contador = 0;
        }
    }
}