#include <xc.h>
#include "config.h"
#include "globals.h"
#include "interrupts.h"
#include "funciones.h"
#include "PWM_LIB.h"
#include "EEPROM_LIB.h"

void INTERRUPTS_CONFIG() {
    //Interrupciones Globales
    INTCONbits.GIE = 1; //Se habilitan las interupciones globales
    INTCONbits.PEIE = 1; // Se habilitan las interrupciones por periferico

    //Interrupción por cambio de estado
    INTCONbits.RBIE = 0; // Habilita las interrupciones por cambio en PORTB
    INTCONbits.RBIF = 0; // Limpia la bandera de interrupción

    //Interrupcion Externa RB0
    INTCONbits.INT0IF = 0; // indicador o bandera de interrupcion externa RB0 se inicializa en 0
    INTCONbits.INT0IE = 1; // Se deshabilita la interrupcio externa pin RB0
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

    // zero crossing interrupt 
    //    if (INTCONbits.INT0IF && vista_firing_angle_flag == 1) {
    //        LATCbits.LATC0 = !LATCbits.LATC0;
    //        PULSO = 0;
    //        PR2 = cargaTimer; // carga el tiempo a temporizar en el timer2
    //        PIR1bits.TMR2IF = 0; // limpia bandera timer2
    //        PIE1bits.TMR2IE = 1; // activa interrupcion de timer cero
    //        T2CONbits.TMR2ON = 1;
    //        INTCONbits.INT0IF = 0; // external interrupt flag is cleared
    //    }
    //    if (PIR1bits.TMR2IF) {
    //        PULSO = 1;
    //        __delay_us(10);
    //        PULSO = 0;
    //        PIR1bits.TMR2IF = 0; // limpia bandera timer2
    //        PIE1bits.TMR2IE = 0; // desactiva interrupcion de timer cero
    //        T2CONbits.TMR2ON = 0;
    //    }
    // Speed Sensor Interrupt / 
    if (INTCONbits.INT0IF && operationModeFlag) {
        contador++;
        INTCONbits.INT0IF = 0; // external interrupt flag is cleared
    }

    //Interrupcion timer 0
    if (TMR0IF == 1 && operationModeFlag) {
        TMR0H = 0x0B;
        TMR0L = 0xDC;
        PV_RPM = contador * 30;
        contador = 0;
        segundos--;
        if (horas == 0 && minutos == 0 && segundos == 0 && vista == 1) {
            fin_ciclo = 1;
        } else if (segundos < 0 && vista == 1) {
            minutos--;
            segundos = 59;
            if (minutos < 0 && vista == 1) {
                horas--;
                minutos = 59;
            }
        }
        TMR0IF = 0;
    }

    //Interrupcion modulo CCP1 para señal se sensor
    //    if (PIR1bits.CCP1IF) {
    //        tiempo_total = CCPR1H << 8 | CCPR1L;
    //        TMR1H = 0;
    //        TMR1L = 0;
    //        //T = Tins * preescaler * #pasos
    //        //#pasos = (1/4x10e6 ) * 4 * 8 = 1us entonces:
    //        //   periodo = (tiempo_total * 0.000008)/4;
    //        if (tiempo_total > 0) {
    //            periodo = ((tiempo_total * 0.000008) / 2); //el uC cuenta cada 4 pulsos pero como cada revolucion son dos pulsos se divide entre 2
    //            rpm = (60) / periodo;
    //            tiempo_total = 0;
    //        }
    //        PIR1bits.CCP1IF = 0;
    //    }
    //interrupcion de externa RB1 - Boton Start/Stop
    if (INTCON3bits.INT1IF) {
        //  __delay_ms(50);
        if (vista == 0) {
            vista = 1;
            if (horas > 0 || minutos > 0 && setpoint_rpm > 0) {
                tmr0_on();
                PWM_Start();
            }
        } else if (vista == 1) {
            PWM_Stop();
            PWM_Duty(0);
            tmr0_off();
            operationModeFlag = 0;
            vista = 0;
        } else if (vista == 4 && pwm_flag == 1) {
            //PWM_Start();
            PWM_Duty(duty);
        }
        //        else if (vista == 5 && vista_firing_angle_flag == 1) {
        //            set_firing_angle();
        //        }
        INTCON3bits.INT1IF = 0;
    }
    //Interrupcion de externa RB2 -> Boton SET
    if (INTCON3bits.INT2IF) {
        // __delay_ms(50);
        if (vista == 0) {
            vista = 2;
        } else if (vista == 2) {
            setpoint_rpm = mil * 1000 + cen * 100 + dec * 10 + uni;
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
            vista = 5;
        } else if (vista == 5) {
            EEPROM_Guardar(4, kp);
            EEPROM_Guardar(5, ki);
            EEPROM_Guardar(6, kd);
            vista = 0;
        }
        /*else if (vista == 5) {
            vista_firing_angle_flag = 0;
            vista = 0;
        }*/
        INTCON3bits.INT2IF = 0;
        __delay_ms(100);
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