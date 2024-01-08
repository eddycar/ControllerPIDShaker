
#ifndef GLOBALS_H
#define	GLOBALS_H

//#define PULSO  LATDbits.LATD2 

char aux[20] = " ";
char pwm_flag = 0;
char operationModeFlag = 0;
char vista_firing_angle_flag = 0;
char vista = 0; // variable para elegir la vista : defecto(0), operacion(1), setpoint rpm(2), setpoint time(3))

//Variables PID
int kp = 0; // Ganancia proporcional
int ki = 0; // Ganancia integral
int kd = 0; // // Ganancia derivativa

float error_acumulado = 0.0;
float error_anterior = 0.0;

float up = 0; // Accion proporcional
float ui = 0.0; // Accion Integral
float ui_ = 0.0; //  Accion derivativa

float ud = 0;
float ut = 0;

//Variables de interrupcion de cambio de estado
char inc_pressed = 0;
char dec_pressed = 0;

// Variables asociadas a RPM
//Sensor
int contador = 0;
int PV_RPM = 0;

int setpoint_rpm = 0;

// Variables asociadas a tiempo 
int horas = 0;  
int setpoint_horas = 0;
int minutos = 0;
int setpoint_minutos = 0;
int segundos = 0;
int setpoint_segundos = 0;
char fin_ciclo = 0;


// Variables setpoint RPM
int uni = 0, dec = 0, cen = 0, mil = 0;
//variables de setpoint para angulo de disparo
//char set_units = 0;
//char set_tens = 0;
//char set_hundreds = 0;

// variables ccp1 para calcular rpm 
//float periodo = 0.0;
//int tiempo_total = 0;
//char pulsos_por_revolucion = 2; 

char duty = 0;

// Variables control de potencia 
//int valorAnguloDisparo = 0; // valor en porcentaje del angulo de disparo
//int cargaTimer = 0; // valor que se calcula para convertir porcentaje a carga en PR2 del timer2

#endif	/* GLOBALS_H */

