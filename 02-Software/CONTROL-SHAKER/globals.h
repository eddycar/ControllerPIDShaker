
#ifndef GLOBALS_H
#define	GLOBALS_H

//#define PULSO  LATDbits.LATD2 
#define PULSOS_POR_REVOLUCION 24

char aux[20] = " ";
char pwm_flag = 0;
char operationModeFlag = 0;
char vista_firing_angle_flag = 0;
char vista = 0; // variable para elegir la vista : defecto(0), operacion(1), setpoint rpm(2), setpoint time(3))

//Variables PID
float kp = 0.0; // Ganancia proporcional
float ki = 0.0; // Ganancia integral
float kd = 0.0; // // Ganancia derivativa

float error_acumulado = 0.0;
float error_anterior = 0.0;

float up = 0; // Accion proporcional
float ui = 0.0; // Accion Integral
float ui_ = 0.0; //  Accion derivativa

float ud = 0;
float ut = 0;


//Variables PID 2
float cv;
float cv1;
float error;
float error1;
float error2;

//Variables de interrupcion de cambio de estado
char inc_pressed = 0;
char dec_pressed = 0;

// Variables asociadas a RPM
//Sensor
//Pulsos por revolucion del sensor del motor
int PV_RPM;
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

int duty = 0;

#endif	/* GLOBALS_H */

