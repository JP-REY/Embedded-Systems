/*======================================================================= Arquivo MOTOR.c ==============================================================================================
  
  Desenvolvimento das funções da biblioteca MOTOR.h

  
  Date: 26/11/2025
  
  Author: João Pedro Rey
  
  
 ===================================================================== Bibliotecas Utilizadas =========================================================================================*/

#include "MOTOR.h"

 /*================================================================= Inicialização de Variáveis =======================================================================================*/


volatile uint16_t	base_timer		= 0x00,	//Contador para base de tempo de 1ms
					deflect_timer	= 0x00,	//Contador auxiliar para tempo de mudança de direção
					idle_timer		= 0x00;	//Contador auxiliar para intervalo de verificação
					
volatile uint8_t	increment_f		= 0x00,	//Flag de incremento de duty cycle dos motores
					deflect_flag	= 0x00, //Flag para controle de tempo de desvio dos motores
					check_flag		= 0x00;	//Flag de indicação de status para partida dos motores

/*==================================================================== Rotina de Interrupção ==========================================================================================*/

ISR(TIMER0_OVF_vect)
{
	base_timer++;
	deflect_timer++;
	idle_timer++;
	
	
	/*Timer0 Prescaler = 64*/
	TCNT0 = 0x06;
	
	/*Base de tempo de 1ms*/	
	if(base_timer == 100)
	{
		base_timer	 = 0x00;
		increment_f	 = 0x01;
		
		HAL_GPIO_TOGGLE_pin(&PORTB,PORTB0);	
		
	}/*End if*/	

}/*End ISR*/

/*===================================================================== Configuração Timer 0 ==========================================================================================*/

void TIMER_INIT(void)
{
		
	TCCR0A = 0x00;

	HAL_GPIO_SET_bit(&TIMSK0,TOIE0);
	HAL_GPIO_SET_bit(&TCCR0B,CS00);
	HAL_GPIO_SET_bit(&TCCR0B,CS01);
	
	HAL_GPIO_OUTPUT_pin(&DDRB,&PORTB,PORTB0);	
		
}/*End timer_init*/


/*=====================================================================================================================================================================================*/

//Função para giro no sentido direto com partida suave dos motores

void MOTOR_FORWARD(MOTOR *M, uint8_t step, uint8_t speed_t)
{	
	//Configura os bits de direção
	
	HAL_GPIO_SET_bit(&PORTC,IN1);
	HAL_GPIO_SET_bit(&PORTC,IN3);
	HAL_GPIO_CLEAR_bit(&PORTC,IN2);
	HAL_GPIO_CLEAR_bit(&PORTC,IN4);
	
	M->ramp_step = step;
	
	M->speed_target = speed_t;
	
	//Lógica de partida gradual
	
	if(M->speed < M->speed_target && increment_f)
	{
		M->speed += M->ramp_step;
		
		increment_f = 0x00;
				
		if(M->speed > M->speed_target)
		{
			M->speed = M->speed_target;
			
		}/*End if*/
		
		DUTY_UPDATE(M->PWM_motor,OCRA,M->speed);	
		
		DUTY_UPDATE(M->PWM_motor,OCRB,M->speed);	
								
	}/*End if*/	
	
}/*End MOTOR_FORWARD*/

/*=====================================================================================================================================================================================*/

//Função de parada suave dos motores

void MOTOR_CRITICAL(MOTOR *M, uint8_t step)
{
	M->ramp_step = step;
	
	//Lógica de parada gradual
	
	if(M->speed > 0 && increment_f)
	{
		M->speed -= M->ramp_step;	
		
		increment_f = 0x00;
		
		if(M->speed == 0)
		{
			/*Desabilita os pinos de direção para evitar shoothrough durante reversão*/
			HAL_GPIO_CLEAR_bit	(&PORTC,IN1);
			HAL_GPIO_CLEAR_bit	(&PORTC,IN3);
			HAL_GPIO_CLEAR_bit	(&PORTC,IN2);
			HAL_GPIO_CLEAR_bit	(&PORTC,IN4);	
					
		}/*End if*/
			
		DUTY_UPDATE(M->PWM_motor,OCRA,M->speed);
		
		DUTY_UPDATE(M->PWM_motor,OCRB,M->speed);
								
	}/*End if*/
				
}/*End MOTOR_CRITICAL*/

/*=====================================================================================================================================================================================*/

//Função para iniciar o movimento de desaceleração dos motores apenas a detecção de obstáculo

void MOTOR_SLOWDOWN(MOTOR *M, uint8_t step, uint8_t speed_t, uint8_t *OCRX)
{	
	M->ramp_step = step;
	
	M->speed_target = speed_t;
	
	M->speed = *OCRX;
	
	if(M->speed > 0 && increment_f)
	{
		M->speed -= M->ramp_step;
		
		increment_f = 0x00;
		
		if(M->speed <= speed_t)
		{
			M->speed = M->speed_target;

		}/*End if*/
				
	}/*End if*/
	
	DUTY_UPDATE(M->PWM_motor,OCRA,M->speed);
	DUTY_UPDATE(M->PWM_motor,OCRB,M->speed);
	
}/*End MOTOR_SLOWDOWN*/

/*=====================================================================================================================================================================================*/

void MOTOR_REVERSE(MOTOR *M, uint8_t step, uint8_t speed_t)
{
	//Configura os bits de direção	
	HAL_GPIO_SET_bit(&PORTC,IN2);
	HAL_GPIO_SET_bit(&PORTC,IN4);
	HAL_GPIO_CLEAR_bit(&PORTC,IN1);
	HAL_GPIO_CLEAR_bit(&PORTC,IN3);
	
	M->ramp_step = step;
	
	M->speed_target = speed_t;
	
	//Lógica de partida gradual
	
	if(M->speed < M->speed_target && increment_f)
	{
		M->speed += M->ramp_step;
		
		increment_f = 0x00;
		
		if(M->speed > M->speed_target)
		{
			M->speed = M->speed_target;
			
		}/*End if*/
			
	DUTY_UPDATE(M->PWM_motor,OCRA,M->speed);
			
	DUTY_UPDATE(M->PWM_motor,OCRB,M->speed);
			
	}/*End if*/
	
}/*End MOTOR_REVERSE*/

/*=====================================================================================================================================================================================*/

//Função de acionamento dos motores para realizar giro no próprio eixo para mudança de direção

void MOTOR_DEFLECT(MOTOR *M, DIRECTION dir, uint8_t step, uint8_t speed_t, uint16_t deflect_interval)
{
	M->ramp_step = step;
	
	M->speed_target = speed_t;	
	
	switch(dir)
	{
		case 0:
		
		//Configura os bits de direção para movimento de desivo para a direita
		HAL_GPIO_SET_bit	(&PORTC,IN1);
		HAL_GPIO_CLEAR_bit	(&PORTC,IN2);
		HAL_GPIO_SET_bit	(&PORTC,IN4);
		HAL_GPIO_CLEAR_bit	(&PORTC,IN3);
		
		//Aplica lógica de aceleração suave para curva
		
		if(deflect_timer == deflect_interval)
		{
			DUTY_UPDATE(M->PWM_motor,OCRA,0);	
			DUTY_UPDATE(M->PWM_motor,OCRB,0);			
		
		}/*End if*/
		
		else
		{
		
			if(M->speed < M->speed_target && increment_f)
			{
				M->speed += M->ramp_step;
				
				increment_f = 0x00;
			
				if(M->speed > M->speed_target)
				{
					M->speed = M->speed_target;
				
				}/*End if*/
		
			}/*End if*/
			
		}/*End else*/
		
		//Atualiza somente o valor de um sinal PWM para que apenas um motor realize movimento enquanto o outro fica parado - Movimento de curva para a esquerda	
			
		DUTY_UPDATE(M->PWM_motor,OCRA,M->speed);	
		DUTY_UPDATE(M->PWM_motor,OCRB,M->speed);		
		
		break;
		
		case 1:
		
		//Configura os bits de direção para movimento de desivo para a direita
		HAL_GPIO_SET_bit	(&PORTC,IN2);
		HAL_GPIO_CLEAR_bit	(&PORTC,IN1);
		HAL_GPIO_SET_bit	(&PORTC,IN3);
		HAL_GPIO_CLEAR_bit	(&PORTC,IN4);
		
		//Aplica lógica de aceleração suave para curva
		
		if(deflect_timer == deflect_interval)
		{
			DUTY_UPDATE(M->PWM_motor,OCRB,0);
			DUTY_UPDATE(M->PWM_motor,OCRA,0);
					
		}/*End if*/
					
		else
		{
						
			if(M->speed < M->speed_target && increment_f)
			{
				M->speed += M->ramp_step;
				
				increment_f = 0x00;
							
				if(M->speed > M->speed_target)
				{
					M->speed = M->speed_target;
								
				}/*End if*/
								
			}/*End if*/
								
		}/*End else*/
								
		//Atualiza somente o valor de um sinal PWM para que apenas um motor realize movimento enquanto o outro fica parado - Movimento de curva para a direita
								
		DUTY_UPDATE(M->PWM_motor,OCRB,M->speed);
		DUTY_UPDATE(M->PWM_motor,OCRA,M->speed);
								
		break;
				
	}/*End switch*/
		
}/*End MOTOR_DEFLECT*/

/*=====================================================================================================================================================================================*/

//Função de configuraçãp dos parâmetros do motor

void MOTOR_CFG(MOTOR *M, PWM_CONFIG *PWM)
{

	M->PWM_motor = PWM;			/*Atribui ao endereço do campo *PWM_motor da struct MOTOR m o endereço da struct PWM_set PWM
								  esta forma, é possível acessar os campos da struct PWM_set manipulando diretamente a struct MOTOR*/	
	M->speed = 0;
	
	M->ramp_step = 0;
	
	M->speed_target = 0;
	
	//Seta os pinos de saída para controle dos bits de direção
		
	HAL_GPIO_OUTPUT_pin(&DDRC,&PORTC,IN1);
	HAL_GPIO_OUTPUT_pin(&DDRC,&PORTC,IN2);
	HAL_GPIO_OUTPUT_pin(&DDRC,&PORTC,IN3);
	HAL_GPIO_OUTPUT_pin(&DDRC,&PORTC,IN4);
		
}/*End MOTOR_CFG*/

/*=====================================================================================================================================================================================*/

//Função para inicialização dos parâmetros dos motores

void MOTOR_INIT(void)
{
	MOTOR_CFG(&M,&PWM);
		
}/*End MOTOR_INIT*/

/*=====================================================================================================================================================================================*/

