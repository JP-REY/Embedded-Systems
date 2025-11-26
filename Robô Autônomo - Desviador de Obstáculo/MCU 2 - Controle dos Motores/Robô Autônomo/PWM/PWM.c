/*======================================================================= Arquivo PWM.c ===============================================================================================
  
  Desenvolvimento das funções da biblioteca PWM.h
  
  Date: 07/11/2025
  
  Author: João Pedro Rey
  
 ===================================================================== Bibliotecas Utilizadas =========================================================================================*/

 #include "PWM.h"

/*======================================================================= Função PWM_INIT =============================================================================================*/

/*Inicialização dos registradores TCCRX e configuração de parâmetros de PWM*/

void PWM_INIT(PWM_CONFIG *PWM, PWM_MODE Mode, PWM_PHASE Phase, PRESCALER Prescale, TIMER_SELECT Timer)
{
	switch(Timer)
	{
		case TIMER0:
		PWM->TCCRxA			= &TCCR0A;
		PWM->TCCRxB			= &TCCR0B;
		PWM->OCRxA			= &OCR0A;
		*PWM->OCRxA			= 0x00;
		PWM->OCRxB			= &OCR0B;
		*PWM->OCRxB			= 0x00;
		PWM->COMxA_bit_1	= COM0A1;
		PWM->COMxA_bit_0	= COM0A0;
		PWM->COMxB_bit_1	= COM0B1;
		PWM->COMxB_bit_0	= COM0B0;
		PWM->WGMx0			= WGM00;
		PWM->WGMx1			= WGM01;
		PWM->WGMx2			= WGM02;
		PWM->CSx0			= CS00;
		PWM->CSx1			= CS01;
		PWM->CSx2			= CS02;
		
		break;

		case TIMER1:
		PWM->TCCRxA			= &TCCR1A;
		PWM->TCCRxB			= &TCCR1B;
		PWM->OCRxA			= &OCR1A;
		*PWM->OCRxA			= 0x00;
		PWM->OCRxB			= &OCR1B;
		*PWM->OCRxB			= 0x00;
		PWM->COMxA_bit_1	= COM1A1;
		PWM->COMxA_bit_0	= COM1A0;
		PWM->COMxB_bit_1	= COM1B1;
		PWM->COMxB_bit_0	= COM1B0;
		PWM->WGMx0			= WGM10;
		PWM->WGMx1			= WGM11;
		PWM->WGMx2			= WGM12;
		PWM->CSx0			= CS10;
		PWM->CSx1			= CS11;
		PWM->CSx2			= CS12;
		
		break;

		case TIMER2:
		PWM->TCCRxA			= &TCCR2A;
		PWM->TCCRxB			= &TCCR2B;
		PWM->OCRxA			= &OCR2A;
		*PWM->OCRxA			= 0x00;
		PWM->OCRxB			= &OCR2B;
		*PWM->OCRxB			= 0x00;
		PWM->COMxA_bit_1	= COM2A1;
		PWM->COMxA_bit_0	= COM2A0;
		PWM->COMxB_bit_1	= COM2B1;
		PWM->COMxB_bit_0	= COM2B0;
		PWM->WGMx0			= WGM20;
		PWM->WGMx1			= WGM21;
		PWM->WGMx2			= WGM22;
		PWM->CSx0			= CS20;
		PWM->CSx1			= CS21;
		PWM->CSx2			= CS22;
		
		break;
		
	}/*End switch timer*/
		
	switch(Mode)
	{
		case FAST_PWM:
		HAL_GPIO_SET_bit(PWM->TCCRxA,PWM->WGMx0);
		HAL_GPIO_SET_bit(PWM->TCCRxA,PWM->WGMx1);
		break;
			
		case PHASE_CORRECT_PWM:
		HAL_GPIO_SET_bit(PWM->TCCRxA,PWM->WGMx0);
		break;
			
	}/*End switch mode*/
			
	switch(Phase)
	{
		case NON_INVERTER:
		HAL_GPIO_SET_bit(PWM->TCCRxA,PWM->COMxA_bit_1);
		HAL_GPIO_SET_bit(PWM->TCCRxA,PWM->COMxB_bit_1);
		break;
				
		case INVERTED:
		HAL_GPIO_SET_bit(PWM->TCCRxA,PWM->COMxA_bit_1);
		HAL_GPIO_SET_bit(PWM->TCCRxA,PWM->COMxB_bit_1);
		HAL_GPIO_SET_bit(PWM->TCCRxA,PWM->COMxA_bit_0);
		HAL_GPIO_SET_bit(PWM->TCCRxA,PWM->COMxB_bit_0);
		break;
				
	}/*End switch phase*/
				
	switch(Prescale)
	{
		case PRESCALER_1:
		HAL_GPIO_SET_bit(PWM->TCCRxB,PWM->CSx0);
		break;
					
		case PRESCALER_8:
		HAL_GPIO_SET_bit(PWM->TCCRxB,PWM->CSx1);
		break;

		case PRESCALER_64:
		HAL_GPIO_SET_bit(PWM->TCCRxB,PWM->CSx1);
		HAL_GPIO_SET_bit(PWM->TCCRxB,PWM->CSx0);
		break;
					
		case PRESCALER_256:
		HAL_GPIO_SET_bit(PWM->TCCRxB,PWM->CSx2);
		break;

		case PRESCALER_1024:
		HAL_GPIO_SET_bit(PWM->TCCRxB,PWM->CSx2);
		HAL_GPIO_SET_bit(PWM->TCCRxB,PWM->CSx0);
		break;
					
	}/*End switch Prescaler*/

}/*End PWM_config*/

/*======================================================================= Função DUTY_UPDATE ===========================================================================================*/

/*Atualiza o valor de duty cycle do PWM*/

void DUTY_UPDATE(PWM_CONFIG *PWM, TIMER_SELECT Timer, OCRX_REGISTER OC_REG, uint8_t Duty_Value)
{
	switch(OC_REG)
	{
		case OCRA:
		
		*PWM->OCRxA = Duty_Value;
								
		break;
		
		case OCRB:
		
		*PWM->OCRxB = Duty_Value;
		
		break;
		
	}/*End switch*/
						
}/*End set_PWM_duty*/

/*=====================================================================================================================================================================================*/
