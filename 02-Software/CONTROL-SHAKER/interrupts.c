#include <xc.h>
#include "config.h"
#include "globals.h"
#include "interrupts.h"
#include "funciones.h"
#include "PWM_LIB.h"
#include "EEPROM_LIB.h"

int contador = 0;
float factorDeConversion = 60 / PULSOS_POR_REVOLUCION;

void INTERRUPTS_CONFIG() {
    //Interrupciones Globales
    INTCONbits.GIE = 1; //Se habilitan las interupciones globales
    INTCONbits.PEIE = 1; // Se habilitan las interrupciones por periferico

    //Interrupción por cambio de estado
    INTCONbits.RBIE = 0; // Habilita las interrupciones por cambio en PORTB
    INTCONbits.RBIF = 0; // Limpia la bandera de interrupción

    //Interrupcion Externa RB0
    INTCONbits.INT0IF = 0; // indicador o bandera de interrupcion externa RB0 se inicializa en 0
    INTCONbits.INT0IE = 0; // Se deshabilita la interrupcion  externa pin RB0
    INTCON2bits.INTEDG0 = 1; // habilita flanco de subida para de interrupcion externa RB0

    //Interrupcion Externa RB1
    INTCON3bits.INT1IF = 0; // indicador o bandera de interrupcion externa RB1 se inicializa en 0
    INTCON3bits.INT1IE = 1; // Se habilita la interrupcio externa pin RB1
    INTCON2bits.INTEDG1 = 0; // habilita flanco de bajada para de interrupcion externa RB1

    //Interrupcion Externa RB2
    INTCON3bits.INT2IF = 0; // indicador o bandera de interrupcion externa RB2 se inicializa en 0
    INTCON3bits.INT2IE = 1; // Se habilita la interrupcio externa pin RB2
    INTCON2bits.INTEDG2 = 0; // habilita flanco de bajada para de interrupcion externa RB2

}

void __interrupt() INTERRUPTS() {
    // Speed Sensor Interrupt / 
    if (INTCONbits.INT0IF && (operationModeFlag || pwm_flag)) {
//    if (INTCONbits.INT0IF && operationModeFlag){
        contador++;
        INTCONbits.INT0IF = 0; // external interrupt flag is cleared
    }


    //Interrupcion timer 0
    if (TMR0IF == 1 && (operationModeFlag || pwm_flag)) {
//    if (TMR0IF == 1 && operationModeFlag){
        // Reiniciar el Timer
        TMR0H = 0x0B;
        TMR0L = 0xDC;

        // Calcular la velocidad en RPM

        PV_RPM = (int) (contador * factorDeConversion);
        contador = 0;

        // Manejo del tiempo: horas, minutos, segundos
        if (vista == 1) {
            segundos--;

            if (segundos < 0) {
                minutos--;
                segundos = 59;

                if (minutos < 0) {
                    horas--;
                    minutos = 59;
                }
            }

            // Verificar si se alcanzó el final del ciclo
            if (horas == 0 && minutos == 0 && segundos == 0) {
                fin_ciclo = 1;
            }
        }
        TMR0IF = 0;
    }

    //interrupcion de externa RB1 - Boton Start/Stop
    if (INTCON3bits.INT1IF) {

        if (vista == 1) {
            vista = 0;
        } else if (vista == 0) {
            if (setpoint_rpm > 0 && (setpoint_horas > 0 || setpoint_minutos > 0)) {
                operationModeFlag = 1;
            }
            vista = 1;
        }
        INTCON3bits.INT1IF = 0;
    }
    //Interrupcion de externa RB2 -> Boton SET
    if (INTCON3bits.INT2IF) {
        __delay_ms(150);
        if (vista == 1) {
            vista = 0;
        } else if (vista == 0) {
            vista = 2;
        } else if (vista == 2) {
            vista = 3;
        } else if (vista == 3) {
            pwm_flag = 1;
            vista = 4;
        } else if (vista == 4) {
            vista = 5;
        } else if (vista == 5) {
            vista = 0;
        }
        INTCON3bits.INT2IF = 0;
    }

    // Interrupción para incrementar el setpoint 
    if (INTCONbits.RBIF && (!PORTBbits.RB4)) {
        __delay_ms(20); // Debounce
        if (!PORTBbits.RB4) {
            inc_pressed = 1;
        }
        INTCONbits.RBIF = 0; // Limpia la bandera de interrupción
    }

    // Interrupción para decrementar el setpoint
    if (INTCONbits.RBIF && (!PORTBbits.RB5)) {
        __delay_ms(20); // Debounce
        if (!PORTBbits.RB5) {
            dec_pressed = 1;
        }
        INTCONbits.RBIF = 0; // Limpia la bandera de interrupción
    }
}