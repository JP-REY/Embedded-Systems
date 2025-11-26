/*======================================================================= Arquivo MOTOR.c ==============================================================================================
  
  Desenvolvimento das funções da biblioteca MOTOR.h

  
  Date: 07/11/2025
  
  Author: João Pedro Rey
  
  
 ===================================================================== Bibliotecas Utilizadas =========================================================================================*/

#include "MOTOR.h"

 /*================================================================= Inicialização de Variáveis =======================================================================================*/


volatile uint16_t	base_timer		= 0x00,
					deflect_timer	= 0x00;
					
volatile uint8_t	increment_f		= 0x00;

/*==================================================================== Rotina de Interrupção ==========================================================================================*/

ISR(TIMER0_OVF_vect)
{
	base_timer++;
	deflect_timer++;
	
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

/*Função para giro no sentido direto com partida suave dos motores*/

void MOTOR_FORWARD(MOTOR *m, uint8_t step, uint8_t speed_t)
{	
	/*Configura os bits de direção*/	
	
	HAL_GPIO_SET_bit(&PORTC,IN1);
	HAL_GPIO_SET_bit(&PORTC,IN3);
	HAL_GPIO_CLEAR_bit(&PORTC,IN2);
	HAL_GPIO_CLEAR_bit(&PORTC,IN4);
	
	m->ramp_step = step;
	
	m->speed_target = speed_t;
	
	/*Lógica de partida gradual*/
	
	if(m->speed < m->speed_target && increment_f)
	{
		m->speed += m->ramp_step;
		
		increment_f = 0x00;
				
		if(m->speed > m->speed_target)
		{
			m->speed = m->speed_target;
			
		}/*End if*/
		
		DUTY_UPDATE(m->PWM_motor,m->Timer_PWM,OCRA,m->speed);	
		
		DUTY_UPDATE(m->PWM_motor,m->Timer_PWM,OCRB,m->speed);	
						
	}/*End if*/	
	
}/*End MOTOR_FORWARD*/

/*=====================================================================================================================================================================================*/

/*Função de parada suave dos motores*/

void MOTOR_CRITICAL(MOTOR *m, uint8_t step)
{
	m->ramp_step = step;
	
	/*Lógica de parada gradual*/
	
	if(m->speed > 0 && increment_f)
	{
		m->speed -= m->ramp_step;	
		
		increment_f = 0x00;
		
		if(m->speed == 0)
		{
			/*Desabilita os pinos de direção para evitar shoothrough durante reversão*/
			HAL_GPIO_CLEAR_bit	(&PORTC,IN1);
			HAL_GPIO_CLEAR_bit	(&PORTC,IN3);
			HAL_GPIO_CLEAR_bit	(&PORTC,IN2);
			HAL_GPIO_CLEAR_bit	(&PORTC,IN4);	
					
		}/*End if*/
			
		DUTY_UPDATE(m->PWM_motor,m->Timer_PWM,OCRA,m->speed);
		
		DUTY_UPDATE(m->PWM_motor,m->Timer_PWM,OCRB,m->speed);
								
	}/*End if*/
				
}/*End MOTOR_CRITICAL*/

/*=====================================================================================================================================================================================*/

/*Função para iniciar o movimento de desaceleração dos motores*/

void MOTOR_SLOWDOWN(MOTOR *m, uint8_t step, uint8_t speed_t, uint8_t *OCRX)
{
	
	m->ramp_step = step;
	m->speed_target = speed_t;
	m->speed = *OCRX;
	
	if(m->speed > 0 && increment_f)
	{
		m->speed -= m->ramp_step;
		
		increment_f = 0x00;
		
		if(m->speed <= speed_t)
		{
			m->speed = m->speed_target;

		}/*End if*/
				
	}/*End if*/
	
	DUTY_UPDATE(m->PWM_motor,m->Timer_PWM,OCRA,m->speed);
	DUTY_UPDATE(m->PWM_motor,m->Timer_PWM,OCRB,m->speed);
	
}/*End MOTOR_SLOWDOWN*/

/*=====================================================================================================================================================================================*/

void MOTOR_REVERSE(MOTOR *m, uint8_t step, uint8_t speed_t)
{
	/*Configura os bits de direção*/
	
	HAL_GPIO_SET_bit(&PORTC,IN2);
	HAL_GPIO_SET_bit(&PORTC,IN4);
	HAL_GPIO_CLEAR_bit(&PORTC,IN1);
	HAL_GPIO_CLEAR_bit(&PORTC,IN3);
	
	m->ramp_step = step;
	
	m->speed_target = speed_t;
	
	/*Lógica de partida gradual*/
	
	if(m->speed < m->speed_target && increment_f)
	{
		m->speed += m->ramp_step;
		
		increment_f = 0x00;
		
		if(m->speed > m->speed_target)
		{
			m->speed = m->speed_target;
			
		}/*End if*/
			
	DUTY_UPDATE(m->PWM_motor,m->Timer_PWM,OCRA,m->speed);
			
	DUTY_UPDATE(m->PWM_motor,m->Timer_PWM,OCRB,m->speed);
			
	}/*End if*/
	
}/*End MOTOR_REVERSE*/

/*=====================================================================================================================================================================================*/

/*Função de acionamento dos motores para realizar curva acentuada*/

void MOTOR_DEFLECT(MOTOR *m, DIRECTION dir, uint8_t step, uint8_t speed_t, uint16_t deflect_interval)
{
	m->ramp_step = step;
	
	m->speed_target = speed_t;	
	
	switch(dir)
	{
		case 0:
		
		/*Configura os bits de direção para movimento de desivo para a direita*/
		HAL_GPIO_SET_bit	(&PORTC,IN1);
		HAL_GPIO_CLEAR_bit	(&PORTC,IN2);
		HAL_GPIO_SET_bit	(&PORTC,IN4);
		HAL_GPIO_CLEAR_bit	(&PORTC,IN3);
		
		/*Aplica lógica de aceleração suave para curva*/
		
		if(deflect_timer == deflect_interval)
		{
			DUTY_UPDATE(m->PWM_motor,m->Timer_PWM,OCRA,0);	
			DUTY_UPDATE(m->PWM_motor,m->Timer_PWM,OCRB,0);			
		
		}/*End if*/
		
		else
		{
		
			if(m->speed < m->speed_target && increment_f)
			{
				m->speed += m->ramp_step;
				
				increment_f = 0x00;
			
				if(m->speed > m->speed_target)
				{
					m->speed = m->speed_target;
				
				}/*End if*/
		
			}/*End if*/
			
		}/*End else*/
		
		/*Atualiza somente o valor de um sinal PWM para que apenas um motor realize movimento enquanto o outro fica parado - Movimento de curva para a esquerda*/	
			
		DUTY_UPDATE(m->PWM_motor,m->Timer_PWM,OCRA,m->speed);	
		DUTY_UPDATE(m->PWM_motor,m->Timer_PWM,OCRB,m->speed);		
		
		break;
		
		case 1:
		/*Configura os bits de direção para movimento de desivo para a direita*/
		HAL_GPIO_SET_bit	(&PORTC,IN2);
		HAL_GPIO_CLEAR_bit	(&PORTC,IN1);
		HAL_GPIO_SET_bit	(&PORTC,IN3);
		HAL_GPIO_CLEAR_bit	(&PORTC,IN4);
		
		/*Aplica lógica de aceleração suave para curva*/
		
		if(deflect_timer == deflect_interval)
		{
			DUTY_UPDATE(m->PWM_motor,m->Timer_PWM,OCRB,0);
			DUTY_UPDATE(m->PWM_motor,m->Timer_PWM,OCRA,0);
					
		}/*End if*/
					
		else
		{
						
			if(m->speed < m->speed_target && increment_f)
			{
				m->speed += m->ramp_step;
				
				increment_f = 0x00;
							
				if(m->speed > m->speed_target)
				{
					m->speed = m->speed_target;
								
				}/*End if*/
								
			}/*End if*/
								
		}/*End else*/
								
		/*Atualiza somente o valor de um sinal PWM para que apenas um motor realize movimento enquanto o outro fica parado - Movimento de curva para a direita*/
								
		DUTY_UPDATE(m->PWM_motor,m->Timer_PWM,OCRB,m->speed);
		DUTY_UPDATE(m->PWM_motor,m->Timer_PWM,OCRA,m->speed);
								
		break;
				
	}/*End switch*/
		
}/*End MOTOR_DEFLECET*/

/*=====================================================================================================================================================================================*/

/*Função de inicialização da estrutura para manipulação dos parâmetros do motor*/

void MOTOR_INIT(MOTOR *m, PWM_CONFIG *PWM, TIMER_SELECT Timer)
{

	m->PWM_motor = PWM;			/* Atribui ao endereço do campo *PWM_motor da struct MOTOR m o endereço da struct PWM_set PWM
								   esta forma, é possível acessar os campos da struct PWM_set manipulando diretamente a struct MOTOR */
	
	m->Timer_PWM = Timer;		/* Atribui ao campo Timer_PWM da struct MOTOR o valor da variável Timer do tipo Timer_select (enum)  */
	
	m->speed = 0;
	
	m->ramp_step = 0;
	
	m->speed_target = 0;
	
	/*Seta os pinos de saída para controle dos bits de direção*/
		
	HAL_GPIO_OUTPUT_pin(&DDRC,&PORTC,IN1);
	HAL_GPIO_OUTPUT_pin(&DDRC,&PORTC,IN2);
	HAL_GPIO_OUTPUT_pin(&DDRC,&PORTC,IN3);
	HAL_GPIO_OUTPUT_pin(&DDRC,&PORTC,IN4);
		
}/*End motor_init*/

/*=====================================================================================================================================================================================*/

