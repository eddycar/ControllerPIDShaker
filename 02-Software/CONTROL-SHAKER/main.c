
#include <xc.h>
#include "config.h"
#include "LIBRERIA_LCD.h"
#include "PWM_LIB.h"
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
    TRISCbits.RC0 = 0; // Pin Buzzer como salida
    LATCbits.LATC0 = 0;

    // Inicialización de módulos 
     PWM_Init(20000, _T2_PRE_16); //función para configurar PWM
     tmr0_init();
//     LCD_Init();
//     LCD_Cmd(_LCD_CURSOR_OFF);

    //Inicializacion de Variables de Operación 
    /*  setpoint_minutos = EEPROM_Lectura(3);
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
     */

    // setpoint_rpm = leer_rpm();

    while (1) {
        if (vista == 0) {
            operationModeFlag = 0;
            operationModeOff();
            vista_principal();
        } else if (vista == 1) {
            // vista_modo_operacion();
        } else if (vista == 2) {
            // vista_modo_setpoint_rpm();
        } else if (vista == 3) {
            /*  setpoint_rpm = mil * 1000 + cen * 100 + dec * 10 + uni;
              guardar_rpm(setpoint_rpm);
              vista_modo_setpoint_time();*/
        } else if (vista == 4) {
            //vista_test_pwm();
        } else if (vista == 5) {
            // pwm_flag = 0;
            //  PWM_Duty(0);
            //   operationModeOff();
            // EEPROM_Guardar(3, setpoint_minutos);
            // EEPROM_Guardar(2, setpoint_horas);
            //  vista_sintonizar_pid();
        }
    }
    return;
}


