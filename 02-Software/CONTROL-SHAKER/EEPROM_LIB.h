/* 
 * File:   EEPROM_LIB.h
 * Author: ASUS
 *
 * Created on 30 de septiembre de 2023, 10:24 PM
 */

#ifndef EEPROM_LIB_H
#define	EEPROM_LIB_H

void EEPROM_Guardar(int dir, char data);

unsigned char EEPROM_Lectura(int dir);

#endif	/* EEPROM_LIB_H */

