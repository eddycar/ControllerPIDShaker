
#include <xc.h>
#include "LIBRERIA_LCD.h"
#include "PWM_LIB.h"
#include "config.h"
#include "EEPROM_LIB.h"
#include "globals.h"
#include "funciones.h"
#include "vistas.h"
#include <stdio.h>
#include <pic18f4550.h>
#include "interrupts.h"

void main(void) {

    INTERRUPTS_CONFIG(); //Función para configurar interrupciones
    ADCON1 = 0x0F; //Desabilita todas las entradas analogicas
    //INTCON2bits.RBPU = 1; // Habilita resistencias de pullup

    TRISB = 0xff; //Puerto B como entrada;
    TRISCbits.RC0 = 0; // Pin Buzzer
    LATCbits.LATC0 = 0;

    TRISB = 0xff; // Todo el puerto B como entrada
    //  TRISDbits.RD2 = 0; // Salida Triac
    //PULSO = 0; // PULSO corresponde a LATDbits.LATD2

    //Modo Angulo de disparo
    //    TRISCbits.RC2 = 1; // Terminal CCP1 para capturar la señal del sensor de velocidad
    //    TRISCbits.RC7 = 0; // Terminal optoacoplador potencia

    //función para configurar PWM
    PWM_Init(10000, _T2_PRE_16);

    // Se llama a método de configuracion CCP1 para captura del sensor de velocidad
    //ccp1_config();

    // Inicialización de módulos 
    //ccp1_on();
    tmr0_init();
    //tmr2_init();
    LCD_Init();
    LCD_Cmd(_LCD_CURSOR_OFF);

    //Inicializacion de Variables de Operación 

    setpoint_minutos = EEPROM_Lectura(3);
    if (setpoint_minutos > 59 || setpoint_minutos < 0) {
        setpoint_minutos = 0;
    }
    setpoint_horas = EEPROM_Lectura(2);
    if (setpoint_horas > 3 || setpoint_horas < 0) {
        setpoint_horas = 0;
    }

    horas = setpoint_horas;
    minutos = setpoint_minutos;
    segundos = setpoint_segundos;

    kp = EEPROM_Lectura(4);
    if (kp > 100) {
        kp = 0;
    }
    ki = EEPROM_Lectura(5);
    if (ki > 100) {
        ki = 0;
    }
    kd = EEPROM_Lectura(6);
    if (kd > 100) {
        kd = 0;
    }

    setpoint_rpm = leer_rpm();



    while (1) {
  
        if (vista == 0) {
            vista_principal();
        } else if (vista == 1) {
            vista_modo_operacion();
        } else if (vista == 2) {
            vista_modo_setpoint_rpm();
        } else if (vista == 3) {
            vista_modo_setpoint_time();
        } else if (vista == 4) {
            vista_test_pwm();
        } else if (vista == 5) {
            vista_sintonizar_pid();
        }
    }
    return;
}


