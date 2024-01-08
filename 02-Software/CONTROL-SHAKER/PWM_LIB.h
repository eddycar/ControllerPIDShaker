 
#ifndef LIBRERIA_PWM_H
#define	LIBRERIA_PWM_H

#define _T2_PRE_1  1 
#define _T2_PRE_4  4
#define _T2_PRE_16 16

int reg10bits;

void PWM_Init(int f_trabajo , char pre_config);
void PWM_Duty(char ciclo_t);
void PWM_Start(void);
void PWM_Stop(void);
#endif	/* XC_HEADER_TEMPLATE_H */

