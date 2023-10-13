
#ifndef GLOBALS_H
#define	GLOBALS_H

char aux[20] = " ";
char pwm_flag = 0;
char vista = 0; // variable para elegir la vista : defecto(0), operacion(1), setpoint rpm(2), setpoint time(3))


// Variables asociadas a RPM
int rpm = 0;
int setpoint_rpm = 0;

// Variables asociadas a tiempo 
int horas = 0;  
int setpoint_horas = 0;
int minutos = 0;
int setpoint_minutos = 0;
int segundos = 0;
int setpoint_segundos = 0;
char fin_ciclo = 0;

// variables ccp1 para calcular rpm 
int tiempo_total = 0;
char pulsos_por_revolucion = 2;

// Variables RPM
char duty = 0;


#endif	/* GLOBALS_H */

