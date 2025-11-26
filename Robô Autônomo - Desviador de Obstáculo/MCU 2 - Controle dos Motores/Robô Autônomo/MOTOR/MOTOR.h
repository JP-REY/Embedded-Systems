/*===================================================================== Biblioteca MOTOR.h ==============================================================================================
  
  Biblioteca de Funções dos Motores DC
  
  Date: 07/11/2025
  
  Author: João Pedro Rey

======================================================================= Definição da Biblioteca ==========================================================================================*/

#ifndef MOTOR_H_
#define MOTOR_H_

/*===================================================================== Bibliotecas Utilizadas ===========================================================================================*/

#include <avr/interrupt.h>
#include <util/delay.h>
#include "PWM.h"

/*====================================================================== Mapeamento Hardware =============================================================================================*/

#define IN1 PORTC0
#define IN2 PORTC1
#define IN3 PORTC2
#define IN4 PORTC3

/*=================================================================== Definição de Variáveis =============================================================================================*/


extern volatile uint16_t	base_timer,
							deflect_timer;
							
extern volatile uint8_t		increment_f;

typedef struct
{
	PWM_CONFIG     *PWM_motor;
	TIMER_SELECT	Timer_PWM;
	uint8_t			speed;
	uint8_t			speed_target;
	uint8_t			ramp_step;
	
}MOTOR;

typedef enum
{
	RIGHT = 0,
	LEFT
		
}DIRECTION;


/*==================================================================== Protótipos de Funções =============================================================================================*/

void MOTOR_INIT		(MOTOR *m, PWM_CONFIG *PWM, TIMER_SELECT Timer);
void MOTOR_FORWARD	(MOTOR *m, uint8_t step, uint8_t speed_t);
void MOTOR_SLOWDOWN	(MOTOR *m, uint8_t step, uint8_t speed_t, uint8_t *OCRX);
void MOTOR_CRITICAL	(MOTOR *m, uint8_t step);
void MOTOR_REVERSE	(MOTOR *m, uint8_t step, uint8_t speed_t);
void MOTOR_DEFLECT	(MOTOR *m, DIRECTION dir, uint8_t step, uint8_t speed_t, uint16_t deflect_interval);
void TIMER_INIT		(void);

#endif /* MOTOR_H_ */