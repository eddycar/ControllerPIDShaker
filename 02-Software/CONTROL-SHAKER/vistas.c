#include <xc.h>
#include "config.h"
#include "vistas.h"
#include "LIBRERIA_LCD.h"
#include "globals.h"
#include "PWM_LIB.h"
#include"funciones.h"
#include <stdio.h>

unsigned char botonPresionado = 0;

void vista_principal(void) {
    LCD_Cmd(_LCD_CLEAR);
    while (vista == 0) {
        sprintf(aux, "  RP   -> %04d", setpoint_rpm);
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
    // if (setpoint_rpm > 0 && (setpoint_horas > 0 || setpoint_minutos > 0)) {
    if (operationModeFlag) {
        operationModeOn();
    } else {
        LCD_Cmd(_LCD_CLEAR);
        __delay_ms(50);
        LCD_Out(2, 1, "RPM or Time is zero");
        __delay_ms(1000);
    }
    while (vista == 1 && operationModeFlag) {
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
            PWM_Stop();
            tmr0_off();
            fin_ciclo = 0;
            operationModeFlag = 0;
            vista = 0;
            break;
        }
        sprintf(aux, "RPM_PV  -> %04d", PV_RPM);
        LCD_Out(1, 1, aux);
        sprintf(aux, "RPM_SV  -> %04d", setpoint_rpm);
        LCD_Out(2, 1, aux);
        sprintf(aux, "Time_PV -> %02d:%02d:%02d", horas, minutos, segundos);
        LCD_Out(3, 1, aux);
        sprintf(aux, "Time_SV -> %02d:%02d:%02d", setpoint_horas, setpoint_minutos, setpoint_segundos);
        LCD_Out(4, 1, aux);
        duty = pid(PV_RPM);
        PWM_Duty(duty);
    }
}

void vista_modo_setpoint_rpm(void) {

    LCD_Cmd(_LCD_CLEAR);
    char posicion_cursor = 18;

    uni = setpoint_rpm % 10;
    dec = (setpoint_rpm / 10) % 10;
    cen = (setpoint_rpm / 100) % 10;
    mil = (setpoint_rpm / 1000) % 10;

    while (vista == 2) {

        sprintf(aux, "   Set RPM -> %d%d%d%d", mil, cen, dec, uni);
        LCD_Out(2, 1, aux);
        LCD_XY(2, posicion_cursor);
        LCD_Cmd(_LCD_UNDERLINE_ON);
        __delay_ms(150);
        LCD_Cmd(_LCD_CURSOR_OFF);
        while (!PORTBbits.RB4) {
            sprintf(aux, "   Set RPM -> %d%d%d%d", mil, cen, dec, uni);
            LCD_Out(2, 1, aux);
            if (posicion_cursor == 18) {
                uni++;
                if (uni > 9) {
                    uni = 0;
                }
            } else if (posicion_cursor == 17) {
                dec++;
                if (dec > 9) {
                    dec = 0;
                }
            } else if (posicion_cursor == 16) {
                cen++;
                if (cen > 9) {
                    cen = 0;
                }
            } else if (posicion_cursor == 15) {
                mil++;
                if (mil > 9) {
                    mil = 0;
                }
            }
            __delay_ms(50);
        }
        while (!PORTBbits.RB5) {
            sprintf(aux, "   Set RPM -> %d%d%d%d", mil, cen, dec, uni);
            LCD_Out(2, 1, aux);
            if (posicion_cursor == 18) {
                uni--;
                if (uni < 0) {
                    uni = 9;
                }
            } else if (posicion_cursor == 17) {
                dec--;
                if (dec < 0) {
                    dec = 9;
                }
            } else if (posicion_cursor == 16) {
                cen--;
                if (cen < 0) {
                    cen = 9;
                }
            } else if (posicion_cursor == 15) {
                mil--;
                if (mil < 0) {
                    mil = 9;
                }
            }
            __delay_ms(50);
        }
        if (!PORTBbits.RB3 && posicion_cursor == 18) {
            posicion_cursor = 17;
        } else if (!PORTBbits.RB3 && posicion_cursor == 17) {
            posicion_cursor = 16;
        } else if (!PORTBbits.RB3 && posicion_cursor == 16) {
            posicion_cursor = 15;
        } else if (!PORTBbits.RB3 && posicion_cursor == 15) {
            posicion_cursor = 18;
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
    operationModeOn();
    while (vista == 4) {
        LCD_Out(1, 1, "      Test PWM");
        sprintf(aux, "  RPM_PV  -> %04d", PV_RPM);
        LCD_Out(2, 1, aux);
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
        PWM_Duty(duty);
    }
}

void vista_sintonizar_pid(void) {
    LCD_Cmd(_LCD_CLEAR);
    char posicion_cursor = 20;
    while (vista == 5) {
        sprintf(aux, "kp=%03d ki=%03d kd=%03d", (int)kp, (int)ki, (int)kd);
        LCD_Out(2, 1, aux);
        LCD_XY(2, posicion_cursor);
        LCD_Cmd(_LCD_UNDERLINE_ON);
        __delay_ms(150);
        LCD_Cmd(_LCD_CURSOR_OFF);

        while (!PORTBbits.RB4) {
            sprintf(aux, "kp=%03d ki=%03d kd=%03d", (int)kp, (int)ki, (int)kd);
            LCD_Out(2, 1, aux);
            if (posicion_cursor == 20) {
                kd++;
                if (kd > 100) {
                    kd = 0;
                }
            } else if (posicion_cursor == 13) {
                ki++;
                if (ki > 100) {
                    ki = 0;
                }
            } else if (posicion_cursor == 6) {
                kp++;
                if (kp > 100) {
                    kp = 0;
                }
            }
            __delay_ms(50);
        }
        while (!PORTBbits.RB5) {
            sprintf(aux, "kp=%03d ki=%03d kd=%03d", (int)kp, (int)ki, (int)kd);
            LCD_Out(2, 1, aux);
            if (posicion_cursor == 20) {
                kd--;
                if (kd < 0) {
                    kd = 100;
                }
            } else if (posicion_cursor == 13) {
                ki--;
                if (ki < 0) {
                    ki = 100;
                }
            } else if (posicion_cursor == 6) {
                kp--;
                if (kp < 0) {
                    kp = 100;
                }
            }
            __delay_ms(50);
        }
        if (!PORTBbits.RB3 && posicion_cursor == 20) {
            posicion_cursor = 13;
        } else if (!PORTBbits.RB3 && posicion_cursor == 13) {
            posicion_cursor = 6;
        } else if (!PORTBbits.RB3 && posicion_cursor == 6) {
            posicion_cursor = 20;
        }
    }
}