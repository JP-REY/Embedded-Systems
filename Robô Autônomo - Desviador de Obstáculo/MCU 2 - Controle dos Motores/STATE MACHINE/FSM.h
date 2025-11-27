/*===================================================================== Biblioteca FSM.h ================================================================================================
  
  Biblioteca destinada à estruturação da máquina de estados responsável pela tomada de decisões do robô
  
  Date: 26/11/2025
  
  Author: João Pedro Rey

======================================================================= Definição da Biblioteca ==========================================================================================*/

#ifndef FSM_H_
#define FSM_H_

/*=================================================================== Definição de Variáveis =============================================================================================*/

typedef enum
{
	STATE_IDLE,
	STATE_FORWARD,
	STATE_SLOWDOWN,
	STATE_CRITICAL,
	STATE_REVERSE,
	STATE_DEFLECT
	
}FSM_STATE;

FSM_STATE STATE;	//Declara stuct para gerenciamento de estado

/*===================================================================== Bibliotecas Utilizadas ===========================================================================================*/

#include "MOTOR.h"
#include "UART.h"

/*==================================================================== Protótipos de Funções =============================================================================================*/

void FSM_INIT		(void);
void FSM_UPDATE		(uint8_t *data);
void STATE_UPDATE	(FSM_STATE STATE);
void SET_STATE		(FSM_STATE NEXT_STATE);;

#endif /* FSM_H_ */