#include <xc.h>
#include "config.h"
#include "vistas.h"
#include "LIBRERIA_LCD.h"
#include "globals.h"
#include "PWM_LIB.h"
#include <stdio.h>

void vista_principal(void) {
    LCD_Cmd(_LCD_CLEAR);
    while (vista == 0) {
        sprintf(aux, "  RPM  -> %04d", setpoint_rpm);
        LCD_Out(2, 1, aux);
        sprintf(aux, "  Time -> %02d:%02d:%02d", setpoint_horas, setpoint_minutos, setpoint_segundos);
        LCD_Out(3, 1, aux);
    }
}

void vista_modo_operacion(void) {
    horas = setpoint_horas;
    minutos = setpoint_minutos;
    segundos = setpoint_segundos;
    LCD_Cmd(_LCD_CLEAR);
    __delay_ms(50);
    while (vista == 1) {
        if (fin_ciclo == 1) {
            LCD_Cmd(_LCD_CLEAR);
            __delay_ms(50);
            LCD_Out(2, 1, "    Fin De Ciclo");
            for (int i = 0; i < 10; i++) {
                LATCbits.LATC0 = 1;
                __delay_ms(500);
                LATCbits.LATC0 = 0;
                __delay_ms(500);
            }
            vista = 0;
            break;
        }
        sprintf(aux, "RPM_PV  -> %04d", rpm);
        LCD_Out(1, 1, aux);
        sprintf(aux, "RPM_SV  -> %04d", setpoint_rpm);
        LCD_Out(2, 1, aux);
        sprintf(aux, "Time_PV -> %02d:%02d:%02d", horas, minutos, segundos);
        LCD_Out(3, 1, aux);
        sprintf(aux, "Time_SV -> %02d:%02d:%02d", setpoint_horas, setpoint_minutos, setpoint_segundos);
        LCD_Out(4, 1, aux);
        //   pid(rpm);

    }
}

void vista_modo_setpoint_rpm(void) {
    LCD_Cmd(_LCD_CLEAR);
    while (vista == 2) {
        sprintf(aux, "   Set RPM -> %04d", setpoint_rpm);
        LCD_Out(2, 1, aux);
        LCD_XY(2, 18);
        LCD_Cmd(_LCD_UNDERLINE_ON);
        __delay_ms(150);
        LCD_Cmd(_LCD_CURSOR_OFF);
        while (!PORTBbits.RB4) {
            sprintf(aux, "   Set RPM -> %04d", setpoint_rpm);
            LCD_Out(2, 1, aux);
            setpoint_rpm += 10;
            __delay_ms(50);
        }
        while (!PORTBbits.RB5) {
            sprintf(aux, "   Set RPM -> %04d", setpoint_rpm);
            LCD_Out(2, 1, aux);
            setpoint_rpm -= 10;
            __delay_ms(50);
        }
    }
}

void vista_modo_setpoint_time(void) {
    LCD_Cmd(_LCD_CLEAR);
    char posicion_cursor = 15;
    while (vista == 3) {
        sprintf(aux, "  Time -> %02d:%02d:%02d", setpoint_horas, setpoint_minutos, setpoint_segundos);
        LCD_Out(2, 1, aux);
        LCD_XY(2, posicion_cursor);
        LCD_Cmd(_LCD_UNDERLINE_ON);
        __delay_ms(150);
        LCD_Cmd(_LCD_CURSOR_OFF);

        while (!PORTBbits.RB4) {
            sprintf(aux, "  Time -> %02d:%02d:%02d", setpoint_horas, setpoint_minutos, setpoint_segundos);
            LCD_Out(2, 1, aux);
            if (posicion_cursor == 15) {
                setpoint_minutos++;
                if (setpoint_minutos > 59) {
                    setpoint_minutos = 0;
                }
            } else if (posicion_cursor == 12) {
                setpoint_horas++;
                if (setpoint_horas > 3) {
                    setpoint_horas = 0;
                }
            }
            __delay_ms(50);
        }
        while (!PORTBbits.RB5) {
            sprintf(aux, "  Time -> %02d:%02d:%02d", setpoint_horas, setpoint_minutos, setpoint_segundos);
            LCD_Out(2, 1, aux);
            if (posicion_cursor == 15) {
                setpoint_minutos--;
                if (setpoint_minutos < 0) {
                    setpoint_minutos = 59;
                }
            } else if (posicion_cursor == 12) {
                setpoint_horas--;
                if (setpoint_horas < 0) {
                    setpoint_horas = 3;
                }
            }
            __delay_ms(50);
        }
        if (!PORTBbits.RB3 && posicion_cursor == 15) {
            posicion_cursor = 12;
        } else if (!PORTBbits.RB3 && posicion_cursor == 12) {
            posicion_cursor = 15;
        }
    }
}

void vista_test_pwm(void) {
    LCD_Cmd(_LCD_CLEAR);
    __delay_ms(50);
    pwm_flag = 1;
    while (vista == 4) {
        LCD_Out(2, 1, "      Test PWM");
        sprintf(aux, "  Duty Cycle -> %03d", duty);
        LCD_Out(3, 1, aux);
        LCD_XY(3, 19);
        LCD_Cmd(_LCD_UNDERLINE_ON);
        __delay_ms(150);
        LCD_Cmd(_LCD_CURSOR_OFF);
        while (!PORTBbits.RB4) {
            sprintf(aux, "  Duty Cycle -> %03d", duty);
            LCD_Out(3, 1, aux);
            duty++;
            if (duty > 100) {
                duty = 0;
            }
            __delay_ms(50);
        }
        while (!PORTBbits.RB5) {
            sprintf(aux, "  Duty Cycle -> %03d", duty);
            LCD_Out(3, 1, aux);
            duty--;
            if (duty < 0) {
                duty = 100;
            }
            __delay_ms(50);
        }
    }
}

void vista_sintonizar_pid(void) {

}