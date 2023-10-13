
#include <xc.h>
#include "LIBRERIA_LCD.h"
#include "PWM_LIB.h"
#include "config.h"
#include "EEPROM_LIB.h"
#include "globals.h"
#include "funciones.h"
#include "vistas.h"
#include <stdio.h>
#include "interrupts.h"

int contador_cruce = 0;
float salida;

//variables pid
//float kp=40,ki=5,kd=10;

float kp = 80; // Ganancia proporcional
float ki = 0; // Ganancia integral
float kd = 0; // // Ganancia derivativa

float error_acumulado = 0;
float error_anterior = 0.0;

float up = 0; // Accion proporcional
float ui = 0.0; // Accion Integral
float ui_ = 0.0; //  Accion derivativa

float ud = 0;
float ut = 0;

float set_point = 0;

void pid(float pid_in);


void main(void) {
    ADCON1 = 0x0F; //Desabilita todas las entradas analogicas
    INTCON2bits.RBPU = 1; // Habilita resistencias de pullup
    
    INTERRUPTS_CONFIG(); //Función para configurar interrupciones
    
    TRISCbits.RC0 = 0; // Pin Buzzer
    LATCbits.LATC0 = 0;
    TRISCbits.RC2 = 1; // Terminal CCP1 para capturar la señal del sensor de velocidad

    //PWM_Init(8000, _T2_PRE_16);

    // Se llama a método de configuracion CCP1
    ccp1_config();
    ccp1_init(); // Inicia modulo CCP1

    LCD_Init();
    LCD_Cmd(_LCD_CURSOR_OFF);

    //Inicializacion de Variables de Operación 

    setpoint_minutos = EEPROM_Lectura(3);
    if (setpoint_minutos > 59) {
        setpoint_minutos = 0;
    }
    setpoint_horas = EEPROM_Lectura(2);
    if (setpoint_horas > 3) {
        setpoint_horas = 0;
    }

    setpoint_rpm = leer_rpm();

    horas = setpoint_horas;
    minutos = setpoint_minutos;
    segundos = setpoint_segundos;

    while (1) {

        //set_point = 0.0635 * codigo + 5.0; // set point en ranfo de la frecuancia min y max 5 -70

        // salida = ut;
        //   PWM_Duty(ut);

        sprintf(aux, "RPM -> %04d", rpm);
        LCD_Out(2, 1, aux);
        //        if (vista == 0) {
        //            vista_principal();
        //        } else if (vista == 1) {
        //            vista_modo_operacion();
        //        } else if (vista == 2) {
        //            vista_modo_setpoint_rpm();
        //        } else if (vista == 3) {
        //            vista_modo_setpoint_time();
        //        } else if (vista == 4) {
        //            vista_test_pwm();
        //        }
    }
    return;
}

void pid(float velocidad) {
    const float max_x = 100; // salida max del PWM
    const float min_x = 0; // salida min del PWM
    const float t_muestreo = 1; //4e-3;

    //Calculando el error
    error_acumulado = set_point - velocidad; // error = consigna - velocidad
    up = kp*error_acumulado; // calculando accion proporcional u(p)
    ui = ui_ + (ki * t_muestreo * error_acumulado); // calculando accion integral u(i)
    ud = kd * (error_acumulado - error_anterior) / t_muestreo; // calculando accion integral u(d)
    ut = up + ui + ud; // Calculando acción de control u(t)
    if (ut > max_x) ut = max_x;
    if (ut < min_x) ut = min_x;
    ui_ = ui;
    error_anterior = error_acumulado;
}



