
#ifndef FUNCIONES_H
#define	FUNCIONES_H

void contador_on(void);
void contador_off(void);
void tmr0_init(void);
void tmr0_on(void);
void tmr0_off(void);

void operationModeOn(void);
void operationModeOff(void);
//void pid(float velocidad);

void guardar_rpm(int dato);

int leer_rpm(void);

void calcularRPM(void);
//void calcular_rpm();

//Funciones modulo CCP1
void ccp1_config(void);
void ccp1_on(void);
void ccp1_off(void);

//Funciones firing angle
void tmr2_init(void);
void tmr2_on(void);
void tmr2_off(void);
void set_firing_angle(void);

int pid(int velocidad);
#endif	/* FUNCIONES_H */

