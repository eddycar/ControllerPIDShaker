/* 
 * File:   funciones.h
 * Author: ASUS
 *
 * Created on 4 de octubre de 2023, 10:49 PM
 */

#ifndef FUNCIONES_H
#define	FUNCIONES_H

void tmr0_init(void);

void pid(float velocidad);

void guardar_rpm(int dato);

int leer_rpm(void);

void calcular_rpm();

//Funciones modulo CCP1
void ccp1_config();
void ccp1_init();

#endif	/* FUNCIONES_H */

