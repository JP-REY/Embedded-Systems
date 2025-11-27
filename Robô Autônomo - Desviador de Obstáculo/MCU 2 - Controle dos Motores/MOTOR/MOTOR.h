/*===================================================================== Biblioteca MOTOR.h ==============================================================================================
  
  Biblioteca de Funções dos Motores DC
  
  Date: 26/11/2025
  
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
							deflect_timer,
							idle_timer;
							
extern volatile uint8_t		increment_f,
							deflect_flag,
							check_flag;

typedef struct
{
	PWM_CONFIG     *PWM_motor;
	uint8_t			speed;
	uint8_t			speed_target;
	uint8_t			ramp_step;
	
}MOTOR;

typedef enum
{
	RIGHT = 0,
	LEFT
		
}DIRECTION;

//Estrutura para manipulação dos parâmetros dos motores

MOTOR M;

/*==================================================================== Protótipos de Funções =============================================================================================*/

void MOTOR_CFG		(MOTOR *M, PWM_CONFIG *PWM);
void MOTOR_FORWARD	(MOTOR *M, uint8_t step, uint8_t speed_t);
void MOTOR_SLOWDOWN	(MOTOR *M, uint8_t step, uint8_t speed_t, uint8_t *OCRX);
void MOTOR_CRITICAL	(MOTOR *M, uint8_t step);
void MOTOR_REVERSE	(MOTOR *M, uint8_t step, uint8_t speed_t);
void MOTOR_DEFLECT	(MOTOR *M, DIRECTION dir, uint8_t step, uint8_t speed_t, uint16_t deflect_interval);
void MOTOR_INIT		(void);	
void TIMER_INIT		(void);

#endif /* MOTOR_H_ */