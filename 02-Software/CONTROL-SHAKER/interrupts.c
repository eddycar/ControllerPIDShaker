#include <xc.h>
#include "config.h"
#include "globals.h"
#include "interrupts.h"
#include "funciones.h"
#include "PWM_LIB.h"
#include "EEPROM_LIB.h"

void INTERRUPTS_CONFIG(){
    //Configuracion de interrupciones 
INTCONbits.GIE = 1; //Se habilitan las interupciones globales
INTCONbits.PEIE = 1;

//Interrupcion Externa RB0
INTCONbits.INT0IF = 0; // indicador o bandera de interrupcion externa RB0 se inicializa en 0
INTCONbits.INT0IE = 1; // Se habilita la interrupcio externa pin RB0
INTCON2bits.INTEDG0 = 1; // habilita flanco de subida para de interrupcion externa RB0

//Interrupcion Externa RB1
INTCON3bits.INT1IF = 0; // indicador o bandera de interrupcion externa RB1 se inicializa en 0
INTCON3bits.INT1IE = 1; // Se habilita la interrupcio externa pin RB1
INTCON2bits.INTEDG1 = 0; // habilita flanco de bajada para de interrupcion externa RB1

//Interrupcion Externa RB2
INTCON3bits.INT2IF = 0; // indicador o bandera de interrupcion externa RB2 se inicializa en 0
INTCON3bits.INT2IE = 1; // Se habilita la interrupcio externa pin RB2
INTCON2bits.INTEDG2 = 0; // habilita flanco de bajada para de interrupcion externa RB2

//Interrupciones modulo CCP1
PIR1bits.CCP1IF = 0; //Indicador de bandera
PIE1bits.CCP1IE = 1; //Se habilita la interrupcio modulo CCP1
}

void __interrupt() INTERRUPTS() {

    //interrupcion de externa RB0 
    if (INTCONbits.INT0IF) {
        //contador_cruce++;
        INTCONbits.INT0IF = 0;
    }
    //interrupcion de externa RB1 
    if (INTCON3bits.INT1IF) {
        if (vista == 0) {
            vista = 1;
        } else if (vista == 1) {
            vista = 0;
        } else if (vista == 4 && pwm_flag == 1) {
            PWM_Start();
            PWM_Duty(duty);
        }
        INTCON3bits.INT1IF = 0;
        __delay_ms(100);
    }
    //Interrupcion de externa RB2 
    if (INTCON3bits.INT2IF) {
        if (vista == 0) {
            vista = 2;
        } else if (vista == 2) {
            guardar_rpm(setpoint_rpm);
            vista = 3;
        } else if (vista == 3) {
            EEPROM_Guardar(3, setpoint_minutos);
            EEPROM_Guardar(2, setpoint_horas);
            vista = 4;
        } else if (vista == 4) {
            duty = 0;
            PWM_Stop();
            pwm_flag = 0;
            vista = 0;
        }
        INTCON3bits.INT2IF = 0;
        __delay_ms(100);
    }
    //Interrupcion timer 0
    if (TMR0IF == 1) {
        TMR0H = 0x0B;
        TMR0L = 0xDC;
        //     rpm = contador_sensor * 30;
        //     contador_sensor = 0;
        segundos--;
        if (horas == 0 && minutos == 0 && segundos == 0 && vista == 1) {
            fin_ciclo = 1;
        } else if (segundos < 0 && vista == 1) {
            minutos--;
            segundos = 59;
            if (minutos < 0 && vista == 1) {
                horas--;
                minutos = 59;
                //                if (horas < 0 && vista == 1) {
                //                    horas = 1 ;
                //                }
            }
        }
        TMR0IF = 0;
    }
    if (PIR1bits.CCP1IF) {
        tiempo_total = CCPR1H << 8 | CCPR1L;
        calcular_rpm();
        TMR1H = 0;
        TMR1L = 0;
        PIR1bits.CCP1IF = 0;
    }
}