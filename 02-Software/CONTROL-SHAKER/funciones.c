#include <xc.h>
#include "EEPROM_LIB.h"
#include "config.h"
#include "globals.h"
#include "funciones.h"
#include <math.h>

void contador_on(void) {
    INTCONbits.INT0IE = 1; // Se habilita la interrupcion  externa pin RB0
}

void contador_off(void) {
    INTCONbits.INT0IE = 0; // Se deshabilita la interrupcion  externa pin RB0
}

void tmr0_init(void) {
    T0CON = 0x03;
    TMR0H = 0x0B;
    TMR0L = 0xDC;
    TMR0IE = 1;
}

void tmr0_on() {
    T0CONbits.TMR0ON = 1;
}

void tmr0_off() {
    T0CONbits.TMR0ON = 0;
}

void operationModeOn(void) {
    contador_on();
    tmr0_on();
    T2CONbits.TMR2ON = 1; // PWM start
}

void operationModeOff(void) {
    contador_off();
    tmr0_off();
    T2CONbits.TMR2ON = 0; // PWM stop
}

void guardar_rpm(int dato) {
    uint8_t byte1, byte2;

    byte1 = (dato & 0xFF); // Obtener los 8 bits más bajos
    byte2 = ((dato >> 8) & 0x0F); // Obtener los 8 bits más altos

    EEPROM_Guardar(0, byte1);
    EEPROM_Guardar(1, byte2);

}

/*void calcularRPM(){
    PV_RPM = (int)(contador * factorDeConversion);
}*/

int leer_rpm(void) {
    int dato;
    uint8_t byte1 = 0, byte2 = 0;

    byte1 = EEPROM_Lectura(0); // Obtener los 8 bits más bajos
    byte2 = EEPROM_Lectura(1); // Obtener los 8 bits más altos
    if (byte1 == 255 && byte2 == 255) {
        dato = 550;
    } else {
        dato = (byte2 << 8) | byte1;
    }
    return dato;
}

int pid(int velocidad) {
    /*   const float max_x = 100; // salida max del PWM
       const float min_x = 0; // salida min del PWM
       const float t_muestreo = 0.1; //4e-3;

       //Calculando el error
       error_acumulado = setpoint_rpm - velocidad; // error = consigna - velocidad
       up = kp*error_acumulado; // calculando accion proporcional u(p)
       ui = ui_ + (ki * t_muestreo * error_acumulado); // calculando accion integral u(i)
       ud = kd * (error_acumulado - error_anterior) / t_muestreo; // calculando accion integral u(d)
       ut = up + ui + ud; // Calculando acción de control u(t)
       if (ut > max_x) ut = max_x;
       if (ut < min_x) ut = min_x;
       ui_ = ui;
       error_anterior = error_acumulado;*/

    const float Tm = 0.1; // Tiempo de muestreo 100 milisegundos
    error = setpoint_rpm - velocidad;
    cv = cv1 + (kp + kd / Tm) * error + (-kp + ki * Tm - 2 * kd / Tm) * error1 + (kd / Tm) * error2;
    cv1 = cv;
    error2 = error1;
    error1 = error;

    if (cv > 100) cv = 100;
    if (cv < 0) cv = 40;

    return (int) cv;
}
