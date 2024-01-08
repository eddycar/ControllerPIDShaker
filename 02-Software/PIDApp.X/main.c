#include <xc.h>
#include <stdio.h>
#include "config.h"
#include "LIBRERIA_LCD.h"
#include "PWM_LIB.h"

#define PULSOS_POR_REVOLUCION 24

void tmr0_init(void);
void tmr0_on(void);
void tmr0_off(void);
float pid(int velocidad);

char aux[20] = " ";

//VAriables sensor de velocidad
unsigned int contador;
float factorDeConversion = 60 / PULSOS_POR_REVOLUCION;
int PV_RPM = 0;
int setpoint_rpm = 400;

//Variable PWM
float duty = 0.0;

//Variables PID 
float kp = 1.0; // Ganancia proporcional
float ki = 0.2; // Ganancia integral
float kd = 0.001; // // Ganancia derivativa
float cv;
float cv1;
float error;
float error1;
float error2;
char flagTimer0 = 0;

void __interrupt() INTERRUPTS() {
    if (INTCONbits.INT0IF) {
        contador++;
        INTCONbits.INT0IF = 0; // external interrupt flag is cleared
    }
    //Interrupcion timer 0
    if (TMR0IF) {
        // Reiniciar el Timer
        TMR0H = 0x3C;
        TMR0L = 0xB0;
        flagTimer0 ++;
                // Calcular la velocidad en RPM
      //  if (flagTimer0 == 10) {
            PV_RPM = (10*contador * factorDeConversion);
            contador = 0;
   //     }
        TMR0IF = 0;
    }
}

void main(void) {
    ADCON1 = 0x0F; //Desabilita todas las entradas analogicas
    TRISB = 0xff; //Puerto B como entrada;

    //Interrupciones Globales
    INTCONbits.GIE = 1; //Se habilitan las interupciones globales
    INTCONbits.PEIE = 1; // Se habilitan las interrupciones por periferico

    //Interrupcion Externa RB0
    INTCONbits.INT0IF = 0; // indicador o bandera de interrupcion externa RB0 se inicializa en 0
    INTCONbits.INT0IE = 1; // Se deshabilita la interrupcion  externa pin RB0
    INTCON2bits.INTEDG0 = 0; // habilita flanco de subida para de interrupcion externa RB0

    // Inicialización de módulos 
    LCD_Init();
    LCD_Cmd(_LCD_CURSOR_OFF);
    PWM_Init(20000, _T2_PRE_16); //función para configurar PWM
    PWM_Start();
    tmr0_init();
    tmr0_on();

    while (1) {
                sprintf(aux, "RPM_PV  -> %04d", PV_RPM);
                LCD_Out(2, 1, aux);
                sprintf(aux, "RPM_SV  -> %04d", setpoint_rpm);
                LCD_Out(3, 1, aux);
                duty = pid(PV_RPM);
                PWM_Duty(duty);
                LCD_Out(1, 1, "     Test PID       ");
        
 /*
        LCD_Out(1, 1, "      Test PWM");
        sprintf(aux, "  RPM_PV  -> %04d", PV_RPM);
        LCD_Out(2, 1, aux);
        sprintf(aux, "  Duty Cycle: %.2f", duty);
        LCD_Out(3, 1, aux);
        LCD_XY(3, 19);
        LCD_Cmd(_LCD_UNDERLINE_ON);
        __delay_ms(150);
        LCD_Cmd(_LCD_CURSOR_OFF);
        while (!PORTBbits.RB4) {
            sprintf(aux, "  Duty Cycle: %.2f", duty);
            LCD_Out(3, 1, aux);
            duty++;
            if (duty > 500) {
                duty = 0;
            }
            __delay_ms(50);
        }
        while (!PORTBbits.RB5) {
            sprintf(aux, "  Duty Cycle: %.2f", duty);
            LCD_Out(3, 1, aux);
            duty--;
            if (duty < 0) {
                duty = 500;
            }
            __delay_ms(50);
        }
        PWM_Duty(duty);*/
    }
    return;
}

/******* TMR0 ********/
void tmr0_init(void) {
    T0CONbits.T0PS = 0;
    T0CONbits.PSA = 0;
    T0CONbits.T0SE = 0;
    T0CONbits.T0CS = 0;
    T0CONbits.T08BIT = 0;

    TMR0H = 0x3C;
    TMR0L = 0xB0;
    TMR0IE = 1;
}

void tmr0_on(void) {
    T0CONbits.TMR0ON = 1;
}

void tmr0_off(void) {
    T0CONbits.TMR0ON = 0;
}

float pid(int velocidad) {

    float Tm = 0.01; // Tiempo de muestreo 100 milisegundos
    error = setpoint_rpm - velocidad;
    cv = cv1 + (kp + kd / Tm) * error + (-kp + ki * Tm - 2 * kd / Tm) * error1 + (kd / Tm) * error2;
    cv1 = cv;
    error2 = error1;
    error1 = error;

    if (cv > 500) cv = 500;
    if (cv < 150) cv = 150;

    return cv;
}