/*======================================================================= Arquivo UART.c ==================================================================================================
  
  Desenvolvimento das funções da biblioteca FSM.h

  Date: 26/11/2025
  
  Author: João Pedro Rey
  
==================================================================== Bibliotecas Utilizadas ==============================================================================================*/

#include "FSM.h"

/*=================================================================== Inicialização de Variáveis =========================================================================================*/

//Variável para gerenciamento de estados
FSM_STATE STATE;

/*========================================================================================================================================================================================*/

//Função de inicialização de estado
void FSM_INIT(void)
{
	//Estado inicial inativo
	SET_STATE(STATE_IDLE);

}/*End FSM_INIT*/

/*========================================================================================================================================================================================*/


//Função para atualizar a variável de estado conforme o byte recebido da UART
void FSM_UPDATE(uint8_t *data)
{
	//Lê o byte recebido na UART
	UART_RX_READ_BYTE(data);
	
	if(RX_flag)
	{
		//Gerencia o estado do robô	
		switch(*data)
		{
			case 'I': SET_STATE(STATE_IDLE);		break;
			case 'F': SET_STATE(STATE_FORWARD);		break;
			case 'S': SET_STATE(STATE_SLOWDOWN);	break;
			case 'C': SET_STATE(STATE_CRITICAL);	break;
			case 'R': SET_STATE(STATE_REVERSE);		break;
			case 'D': SET_STATE(STATE_DEFLECT);		break;
			default :								break;
				
		}/*End switch data_byte*/
		
		RX_flag = 0;
	
	}/*End if*/
	
	
}/*End FSM_INIT*/

/*========================================================================================================================================================================================*/

//Função para setar o estado do robô
void SET_STATE(FSM_STATE NEXT_STATE)
{
	if(STATE != NEXT_STATE )
	{
		STATE = NEXT_STATE;
		
		switch(NEXT_STATE)
		{
			case STATE_IDLE:
			idle_timer = 0;
			break;
			
			case STATE_DEFLECT:
			deflect_timer = 0;
			break;			
			
			default:
			break;
			
		}/*End switch NEXT_STATE*/
		
	}/*End if*/
		
}/*End SET_STATE*/

/*========================================================================================================================================================================================*/

//Função para atualizar os estados do robô
void STATE_UPDATE(FSM_STATE STATE)
{
	switch(STATE)
	{
		case STATE_IDLE:
		
		/*Intervalo de 3 segundos para inicialização do sistema e verificação de status de partida
		
		  Caso a distância retornada do sensor permaneça em um valor seguro durante 3 segundos a flag sinaliza que os motores podem iniciar o movimento*/
		
		if(idle_timer == 3000) check_flag = 1;
		
		if(check_flag)
		{
			check_flag = 0;
			
			SET_STATE(STATE_FORWARD);			
			
		}/*End if*/
		
		break;
		
		case STATE_FORWARD:
		
		//Inicia movimentos dos motores com 70% de Duty Cycle
		MOTOR_FORWARD(&M,25,178);
		
		break;
		
		case STATE_SLOWDOWN:
		
		//Inicia fase de desacelaração dos motores após detecção de obstáculo - Velocidade limite de 30% de Duty Cycle
		MOTOR_SLOWDOWN(&M,10,77,&OCR2A);
		
		break;
		
		case STATE_CRITICAL:
		
		//Aplica parada gradual dos motores após o robô detectar distância crítica
		MOTOR_CRITICAL(&M,10);
		
		break;
		
		case STATE_REVERSE:
		
		//Inicia movimento de reversão dos motores até uma distância segura do obstáculo para efetuar o desvio
		MOTOR_REVERSE(&M,5,77);
		
		break;
		
		case STATE_DEFLECT:
		
		//Robô realiza giro no próprio eixo por um período determinado para efetuar o desvio 
				
		//Após realizar o desvio os motores devem cessar o movimento e o robô deve checar o status de partida (caso esteja OK ele inicia o movimento para frente)
		MOTOR_DEFLECT(&M,0,10,77,deflect_timer);
		
		SET_STATE(STATE_IDLE);
		
		break;
		
	}/*End switch state*/	
	
}/*End STATE_UPDATE*/