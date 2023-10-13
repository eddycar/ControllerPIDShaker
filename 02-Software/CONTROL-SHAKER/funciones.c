#include <xc.h>
#include "EEPROM_LIB.h"
#include "config.h"
#include "globals.h"

void tmr0_init(void) {
    T0CON = 0x83;
    TMR0H = 0x0B;
    TMR0L = 0xDC;
    TMR0IE = 1;
}

void guardar_rpm(int dato){
    uint8_t byte1, byte2;
    
    byte1 = (dato & 0xFF);  // Obtener los 8 bits más bajos
    byte2 = ((dato >> 8) & 0x0F);  // Obtener los 8 bits más altos
    
    EEPROM_Guardar(0, byte1);
    EEPROM_Guardar(1, byte2);

}

int leer_rpm(void){
    int dato;
    uint8_t byte1= 0, byte2 = 0;
    
    byte1 = EEPROM_Lectura(0);  // Obtener los 8 bits más bajos
    byte2 = EEPROM_Lectura(1);  // Obtener los 8 bits más altos
    if (byte1 == 255 && byte2 ==255){
        dato = 550;
    }else {
        dato = (byte2 << 8) | byte1;
    }
    return dato;
}

void calcular_rpm() {
    if (tiempo_total > 0) {
        float periodoSegundos = (float) tiempo_total / _XTAL_FREQ;
        rpm = (pulsos_por_revolucion * 60) / periodoSegundos;
        // Reinicia el tiempo total y muestra las RPM
        tiempo_total = 0;
    }
}

//Funciones modulo CCP1
void ccp1_config() {

    // Configuración del timer 1
    T1CONbits.T1CKPS = 0;
    T1CONbits.TMR1CS = 0;
    T1CONbits.RD16 = 1;
    TMR1H = 0;
    TMR1L = 0;

    TMR1IE = 1;

    //Registro modulo CCP1 captura por flanco de bajada
    CCP1CONbits.CCP1M = 0b0100;
}

void ccp1_init() {
    //Se habilita el timer 1 para que funcione CCP!
    T1CONbits.TMR1ON = 1;
}
