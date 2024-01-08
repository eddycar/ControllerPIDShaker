#include <xc.h>
#include "EEPROM_LIB.h"
#include "config.h"
#include "globals.h"
#include <math.h>

void tmr0_init(void) {
    T0CON = 0x03;
    TMR0H = 0x0B;
    TMR0L = 0xDC;
    TMR0IE = 1;
}

void tmr0_on(void){
    T0CONbits.TMR0ON = 1;
}

void tmr0_off(void){
    T0CONbits.TMR0ON = 0;
}

void tmr2_init(void) {
    T2CONbits.T2CKPS = 0b01; //Prescale 1:4
    T2CONbits.TOUTPS = 0b0111; // Postcale 1:8
    //Timer 2 interrups 
    PIR1bits.TMR2IF = 0;
    PIE1bits.TMR2IE = 0;
    T2CONbits.TMR2ON = 0;
}

void tmr2_on(void){
    T2CONbits.TMR2ON = 1;
}

void tmr2_off(void){
    T2CONbits.TMR2ON = 0;
}

void guardar_rpm(int dato) {
    uint8_t byte1, byte2;

    byte1 = (dato & 0xFF); // Obtener los 8 bits más bajos
    byte2 = ((dato >> 8) & 0x0F); // Obtener los 8 bits más altos

    EEPROM_Guardar(0, byte1);
    EEPROM_Guardar(1, byte2);

}

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

//Calcular RPM por modulo CCP
//void calcular_rpm() {
//    if (tiempo_total > 0) {
//        float periodoSegundos = (float) tiempo_total / _XTAL_FREQ;
//        rpm = (pulsos_por_revolucion * 60) / periodoSegundos; 
//        tiempo_total = 0;
//    }
//}

//Funciones modulo CCP1
//
//void ccp1_config() {
//
//    // Configuración del timer 1
//    T1CONbits.T1CKPS = 0b11; //Preescaler del timer 1:8
//    T1CONbits.TMR1CS = 0; // Timer trabaja con el reloj interno
//    T1CONbits.RD16 = 1; // Timer se configura a 16 bits
//    TMR1H = 0; // Se inicializa registros de conteo del timer
//    TMR1L = 0;
//
//    //Registro modulo CCP1 captura por flanco de bajada
//    CCP1CONbits.CCP1M =  0b0110;  // Capture mode: every rising edge(0101) || Capture mode: every 4th rising edge(0110)  || Capture mode: every 16th rising edge (0111)
//}
//
//void ccp1_on() {
//    //    //Interrupciones modulo CCP1 
//    PIR1bits.CCP1IF = 0; //Indicador de bandera
//    PIE1bits.CCP1IE = 1; //Se habilita la interrupcion modulo CCP1
//    T1CONbits.TMR1ON = 1;
//}
//
//void ccp1_off(void){
//    //    //Interrupciones modulo CCP1 
//    PIR1bits.CCP1IF = 0; //Indicador de bandera
//    PIE1bits.CCP1IE = 0; //Se habilita la interrupcion modulo CCP1
//    T1CONbits.TMR1ON = 0;
//}
//
//void set_firing_angle(){
// 
//        if (valorAnguloDisparo <= 5) {
//            INTCONbits.INT0IE = 0; // External interrupt is disable
//            PULSO = 0; // apaga la carga
//        } else if (valorAnguloDisparo >= 95) {
//            INTCONbits.INT0IE = 0; // External interrupt is disable
//            PULSO = 1; // Mantiene encendida la carga
//        } else {
//            cargaTimer = round(valorAnguloDisparo * 2.6); // Expresión obtenida a partir del archivo excel cruce por cero
//            INTCONbits.INT0IE = 1;
//        }
//}

void pid(int velocidad) {
    const float max_x = 100; // salida max del PWM
    const float min_x = 0; // salida min del PWM
    const float t_muestreo = 1; //4e-3;

    //Calculando el error
    error_acumulado = setpoint_rpm - velocidad; // error = consigna - velocidad
    up = kp*error_acumulado; // calculando accion proporcional u(p)
    ui = ui_ + (ki * t_muestreo * error_acumulado); // calculando accion integral u(i)
    ud = kd * (error_acumulado - error_anterior) / t_muestreo; // calculando accion integral u(d)
    ut = up + ui + ud; // Calculando acción de control u(t)
    if (ut > max_x) ut = max_x;
    if (ut < min_x) ut = min_x;
    ui_ = ui;
    error_anterior = error_acumulado;
}
