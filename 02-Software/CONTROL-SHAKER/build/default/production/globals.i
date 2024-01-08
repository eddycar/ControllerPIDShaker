# 1 "globals.c"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 288 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "C:/Program Files/Microchip/MPLABX/v6.00/packs/Microchip/PIC18Fxxxx_DFP/1.3.36/xc8\\pic\\include\\language_support.h" 1 3
# 2 "<built-in>" 2
# 1 "globals.c" 2
# 1 "./globals.h" 1







char aux[20] = " ";
char pwm_flag = 0;
char operationModeFlag = 0;
char vista_firing_angle_flag = 0;
char vista = 0;


float kp = 0.0;
float ki = 0.0;
float kd = 0.0;

float error_acumulado = 0.0;
float error_anterior = 0.0;

float up = 0;
float ui = 0.0;
float ui_ = 0.0;

float ud = 0;
float ut = 0;



float cv;
float cv1;
float error;
float error1;
float error2;


char inc_pressed = 0;
char dec_pressed = 0;




int PV_RPM;
int setpoint_rpm = 0;


int horas = 0;
int setpoint_horas = 0;
int minutos = 0;
int setpoint_minutos = 0;
int segundos = 0;
int setpoint_segundos = 0;
char fin_ciclo = 0;


int uni = 0, dec = 0, cen = 0, mil = 0;

int duty = 0;
# 1 "globals.c" 2

