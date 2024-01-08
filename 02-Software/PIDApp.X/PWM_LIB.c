#include <xc.h>
#include "config.h"
#include "PWM_LIB.h"

void PWM_Start() {
    T2CONbits.TMR2ON = 1;
}

void PWM_Stop() {
    T2CONbits.TMR2ON = 0;
}

void PWM_Init(int f_trabajo, char pre_config) {
    TRISCbits.TRISC2 = 0;
    float Tpwm = (float)(1.0 / f_trabajo);
    float Tosc = 1.0 / _XTAL_FREQ;
    // Tins = Tosc*4
    char press = pre_config;

    PR2 = ((Tpwm / (Tosc * 4 * press)) - 1);

    reg10bits = (int)(Tpwm / (Tosc * press));

    CCPR1L = 0;
    CCP1CONbits.DC1B = 0;

    T2CON = 0x00;

    switch (press) {
        case 1: T2CONbits.T2CKPS = 0b00;
            break;
        case 4: T2CONbits.T2CKPS = 0b01;
            break;
        case 16: T2CONbits.T2CKPS = 0b10;
            break;
    }

    TMR2 = 0;
    CCP1CONbits.CCP1M = 0b1100;

    return;
}

void PWM_Duty(float ciclo_t) {
    int nuevoCiclo = (int)(reg10bits * ciclo_t / 500.0);
    CCPR1L = (nuevoCiclo & 0b1111111100) >> 2;
    CCP1CONbits.DC1B = nuevoCiclo & 0b0000000011;
    return;
}

