/*======================================================================= Arquivo PWM.c ===============================================================================================
  
  Desenvolvimento das funções da biblioteca PWM.h
  
  Date: 07/11/2025
  
  Author: João Pedro Rey
  
 ===================================================================== Bibliotecas Utilizadas =========================================================================================*/

 #include "PWM.h"
 
 /*===================================================================================================================================================================================*/

//Inicializa struct PWM_CONFIG

PWM_CONFIG PWM; 

/*======================================================================= Função PWM_INIT =============================================================================================*/

//Inicialização dos registradores TCCRX e configuração de parâmetros de PWM

void PWM_CFG(PWM_CONFIG *PWM, PWM_MODE MODE, PWM_PHASE PHASE, PRESCALER SCALE, TIMER_SELECT TIMER)
{
	switch(TIMER)
	{
		case 0:
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

		case 1:
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

		case 2:
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
		
	}/*End switch Timer*/
		
	switch(MODE)
	{
		case 0:
		HAL_GPIO_SET_bit(PWM->TCCRxA,PWM->WGMx0);
		break;
		
		case 1:
		HAL_GPIO_SET_bit(PWM->TCCRxA,PWM->WGMx0);
		HAL_GPIO_SET_bit(PWM->TCCRxA,PWM->WGMx1);
		break;
			
	}/*End switch Mode*/
			
	switch(PHASE)
	{
		case 0:
		HAL_GPIO_SET_bit(PWM->TCCRxA,PWM->COMxA_bit_1);
		HAL_GPIO_SET_bit(PWM->TCCRxA,PWM->COMxB_bit_1);
		break;
				
		case 1:
		HAL_GPIO_SET_bit(PWM->TCCRxA,PWM->COMxA_bit_1);
		HAL_GPIO_SET_bit(PWM->TCCRxA,PWM->COMxB_bit_1);
		HAL_GPIO_SET_bit(PWM->TCCRxA,PWM->COMxA_bit_0);
		HAL_GPIO_SET_bit(PWM->TCCRxA,PWM->COMxB_bit_0);
		break;
				
	}/*End switch Phase*/
				
	switch(SCALE)
	{
		case 1:
		HAL_GPIO_SET_bit(PWM->TCCRxB,PWM->CSx0);
		break;
					
		case 8:
		HAL_GPIO_SET_bit(PWM->TCCRxB,PWM->CSx1);
		break;

		case 64:
		HAL_GPIO_SET_bit(PWM->TCCRxB,PWM->CSx1);
		HAL_GPIO_SET_bit(PWM->TCCRxB,PWM->CSx0);
		break;
					
		case 256:
		HAL_GPIO_SET_bit(PWM->TCCRxB,PWM->CSx2);
		break;

		case 1024:
		HAL_GPIO_SET_bit(PWM->TCCRxB,PWM->CSx2);
		HAL_GPIO_SET_bit(PWM->TCCRxB,PWM->CSx0);
		break;
					
	}/*End switch Prescale*/

}/*End PWM_config*/

/*======================================================================= Função DUTY_UPDATE ===========================================================================================*/

//Função para atualizat o valor de duty cycle do PWM

void DUTY_UPDATE(PWM_CONFIG *PWM, OCRX_REGISTER OC_REG, uint8_t Duty_Value)
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

/*======================================================================= Função PWM_INIT =============================================================================================*/

void PWM_INIT(void)
{
	//Função para inicializar PWM utilizando a função PWM_CFG
	
	PWM_CFG(&PWM,0,0,8,2);
	
}/*End PWM_INIT*/

/*=====================================================================================================================================================================================*/
