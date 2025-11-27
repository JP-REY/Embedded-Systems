/*======================================================================= Biblioteca PWM.h ==============================================================================================
  
  Biblioteca destinada à configuração de PWM
  
  Date: 26/11/2025
  
  Author: João Pedro Rey

====================================================================== Definição da Biblioteca ==========================================================================================*/

#ifndef PWM_H_
#define PWM_H_

/*===================================================================== Frequência de Clock =============================================================================================*/

#define F_CPU 16000000UL

/*==================================================================== Bibliotecas Utilizadas ===========================================================================================*/

#include "HAL.h"

/*=================================================================== Definição de Variáveis ============================================================================================*/

typedef struct	//Estrutura para configuração de PWM
{
	uint8_t		*TCCRxA;
	uint8_t		*TCCRxB;
	uint16_t	*OCRxA;
	uint16_t	*OCRxB;
	uint8_t		COMxA_bit_1;
	uint8_t		COMxB_bit_1;
	uint8_t		COMxA_bit_0;
	uint8_t		COMxB_bit_0;
	uint8_t		WGMx0;
	uint8_t		WGMx1;
	uint8_t		WGMx2;
	uint8_t		CSx0;
	uint8_t		CSx1;
	uint8_t		CSx2;

}PWM_CONFIG;

typedef enum	//Tipo enum para listagem dos timers disponíveis
{
	TIMER0 = 0,
	TIMER1,
	TIMER2
	
}TIMER_SELECT;

typedef enum	//Tipo enum para listagem dos modos de PWM
{
	PHASE_CORRECT_PWM = 0,
	FAST_PWM

}PWM_MODE;

typedef enum	//Tipo enum para escolha de fase do PWM
{
	NON_INVERTER = 0,
	INVERTED

}PWM_PHASE;

typedef enum
{
	OCRA,
	OCRB
	
}OCRX_REGISTER;

typedef enum	//Tipo enum para listagem das opções de prescaler para frequência do PWM
{
	PRESCALER_1    = 1,
	PRESCALER_8    = 8,
	PRESCALER_32   = 32,
	PRESCALER_64   = 64,
	PRESCALER_128  = 128,
	PRESCALER_256  = 256,
	PRESCALER_1024 = 1024

}PRESCALER;

extern PWM_CONFIG	PWM; //Estrutura para configuração de PWM

/*==================================================================== Protótipos de Funções ============================================================================================*/

void PWM_INIT		(void);
void PWM_CFG		(PWM_CONFIG *PWM, PWM_MODE MODE, PWM_PHASE PHASE, PRESCALER SCALE, TIMER_SELECT TIMER);
void DUTY_UPDATE	(PWM_CONFIG *PWM, OCRX_REGISTER OC_REG, uint8_t Duty_Value);

#endif /* PWM_H_ */