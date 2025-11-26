/*===================================================================== Projeto Robô Autoônomo Embarcado ================================================================================================
  
  Projeto de Firmware para Controle do Sistema do Robô Autônomo Desviador de Obstáculos
  
  First Comitt Date: 21/11/2025
  
  Last Firmware Update Data: 25/11/2011
  
  Firware Version: 1.1.0
  
  Author: João Pedro Rey
  
============================================================================= Bibliotecas Utilizadas ===================================================================================================*/

#include "UART.h"
#include "MOTOR.h"

/*============================================================================= Variáveis Globais ======================================================================================================*/

uint8_t		UART_BYTE;	/*Variável de armazenamento do byte recebido no pino RX*/
PWM_CONFIG	PWM;		/*Estrutura para configuração de PWM*/
MOTOR		M;			/*Estrutura para manipulação dos parâmetros dos motores*/

/*=============================================================================== Main Function=========================================================================================================*/


int main(void)
{
	cli();
	
	TIMER_INIT	();	
	MOTOR_INIT	(&M,&PWM,TIMER2);	
	UART_INIT	(0,9600,0,0);	
	PWM_INIT	(&PWM,0,0,PRESCALER_8,TIMER2);
		
	sei();	
	
		
    while (1) 
    {
		
		
    }/*End while*/
	
	return 0;
		
}/*End main function*/

/*======================================================================================================================================================================================================*/
