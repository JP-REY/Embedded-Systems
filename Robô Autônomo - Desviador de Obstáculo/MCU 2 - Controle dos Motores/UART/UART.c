/*======================================================================= Arquivo UART.c ================================================================================================
  
  Desenvolvimento das funções da biblioteca UART.h

  Date: 26/11/2025
  
  Author: João Pedro Rey
  
==================================================================== Bibliotecas Utilizadas =============================================================================================*/

#include "UART.h"

/*=================================================================== Definição de Variáveis =============================================================================================*/

volatile uint8_t	RX_flag			= 0x00				,
					RX_write_index	= 0x00				,
					RX_read_index	= 0x00				,
					TX_write_index	= 0x00				,
					TX_read_index	= 0x00				,
					RX_BUFFER		[RX_BUFFER_size]	,
					TX_BUFFER		[TX_BUFFER_size]	;
					
/*=================================================================== Rotina de Interrupção =============================================================================================*/

ISR(USART_RX_vect)
{
	/*O vetor de interrupção USART_RX_vect é disparado
	  quando a flag RXC0 é setada. Esta flag é setada
	  sempre que a recepção do byte advindo da UART é
	  finalizada. Necessário habilitar o bit RXCIE0 do
	  registrador UCSR0B para habilitar a interrupção	
	*/
	
	/*Lógica RING BUFFER RX UART:
	
	ISR
		
	Insere o byte recebido da UART na respectiva posição do índice de escrita do BUFFER RX
		
	Função UART_RX_read
	
	Acessa o índice de leitura do BUFFER RX para armazenar o byte enviado pela UART
	
	*/
	
	uint8_t data = UDR0;
	
	uint8_t next_index = (RX_write_index + 1) & RX_MASK;
	
	/*Caso haja overflow do BUFFER_RX retorna erro*/
	
	if(next_index == RX_read_index)
	return;
	
	/*Escreve o byte recebido da UART na respectiva posição do vetor de BUFFER*/
	
	RX_BUFFER[RX_write_index] = (char)data;
	
	/*Avança o índice de leitura para a próxima posição do vetor RX_BUFFER*/
	
	RX_write_index = next_index;
	
	//Seta flag para sinalizar recebimento de byte
	RX_flag = 0x01;
	
		
}/*End ISR*/

/*=======================================================================================================================================================================================*/

uint8_t UART_RX_READ_BYTE(uint8_t *data_byte)
{	
	uint8_t rd_idx,
			wr_idx;
			
	cli();
	
	rd_idx = RX_read_index;
	
	wr_idx = RX_write_index;
	
	if(rd_idx == wr_idx)
	{
		/*Caso haja overflow do BUFFER_RX retorna erro*/
		
		sei();
		
		return 0;
		
	}/*End if*/
	
	*data_byte = (uint8_t)RX_BUFFER[RX_read_index];
	
	RX_read_index = (rd_idx + 1) & RX_MASK;
	
	sei();
	
	return 1;
	
}/*End UART_RX_READ_BYTE*/

/*=================================================================== Rotina de Interrupção =============================================================================================*/

ISR(USART_UDRE_vect)
{
	/*O vetor de interrupção USART_UDRE_vect é disparado
	  quando a flag UDRE0 é setada. Esta flag é setada
	  sempre que o registrador UDRO da UART está vazio.
	  Necessário habilitar o bit UDRIE0 do registrador 
	  UCSR0B para habilitar a interrupção.
	*/
	
	/*Lógica RING BUFFER TX UART:
	
	Função UART_TX_send
	
	Insere o byte a ser enviado na respectiva posição do índice de escrita do BUFFER TX
	
	ISR
	
	Lê o byte a ser enviado na respectiva posição do índice de leitura do BUFFER TX e atribui ao registrador de dados UDR0	
	
	*/
	
	/*Caso os índices de leitura e escrita forem iguais, não há bytes a serem transmitidos*/
	
	if(TX_read_index == TX_write_index)
	{
		/*Desabilita a interrupção por Empty Data Register*/
		
		UCSR0B &= ~(1 << UDRIE0);
		
		return;
	}
	
	/*Atribui ao registrador de dados o respectivo byte lido do TX_BUFFER do índice de leitura*/
	
	UDR0 = TX_BUFFER[TX_read_index];
	
	/*Avança o índice de leitura*/
	
	TX_read_index = (TX_read_index + 1) & TX_MASK;
	
	
	
}/*End ISR*/

/*=======================================================================================================================================================================================*/

uint8_t UART_TX_SEND_BYTE(char byte)
{
	uint8_t next_index;
	
	cli();
	
	next_index = (TX_write_index + 1) & TX_MASK;
	
	if(next_index == TX_read_index)
	{			
		/*Se o BUFFER estiver cheio a função retorna 0 e o byte é descartado*/
		
		sei();
		
		return 0;		
	}

	/*Insere o byte a ser enviado no índice de escrita do BUFFER TX*/

	TX_BUFFER[TX_write_index] = byte;
	
	/*Avança o índice de escrita para a próxima posição*/
	
	TX_write_index = next_index;
	
	UCSR0B |= (1 << UDRIE0);
	
	sei();
	
	return 1;
		
}/*End UART_TX_SEND_BYTE*/

/*=======================================================================================================================================================================================*/

uint8_t UART_TX_SEND_STRING(const char *str)
{
	uint8_t i = 0x00;
	
	/*Lógica sequencial para envio da string*/
	
	for(i = 0; str[i] != '\0';i++)
	{
		/*Caso o BUFFER_TX esteja cheio, descarta o byte de envio e retorna falha*/
		
		if (UART_TX_SEND_BYTE(str[i]) == 0) 
		
		return 0;
		
	}/*End for*/
	
	return 1;
	
}/*End UART_TX_SEND_STRING*/

/*=======================================================================================================================================================================================*/

void UART_CFG(UART_MODE MODE , uint16_t BPS, uint8_t SPEED, uint8_t PARITY)
{	
	/*Habilita pinos de RX e TX*/
	
	HAL_GPIO_SET_bit(&UCSR0B,RXEN0);
	HAL_GPIO_SET_bit(&UCSR0B,TXEN0);
	
	/*Habilita flag de interrupção de Received Completed */
	
	HAL_GPIO_SET_bit(&UCSR0B,RXCIE0);
	
	/*Configura frame format de 8 bits*/
	
	HAL_GPIO_SET_bit(&UCSR0C, UCSZ00);
	HAL_GPIO_SET_bit(&UCSR0C, UCSZ01);
	HAL_GPIO_CLEAR_bit(&UCSR0B, UCSZ02);
	
	switch(MODE)
	{
		case 0:
		HAL_GPIO_CLEAR_bit(&UCSR0C,UMSEL00);
		HAL_GPIO_CLEAR_bit(&UCSR0C,UMSEL01);
		break;
		
		case 1:
		HAL_GPIO_CLEAR_bit(&UCSR0C,UMSEL01);
		HAL_GPIO_SET_bit  (&UCSR0C,UMSEL00);
		break;
		
		case 2:
		HAL_GPIO_SET_bit  (&UCSR0C,UMSEL01);
		HAL_GPIO_SET_bit  (&UCSR0C,UMSEL00);
		break;		
		
	}/*End switch Mode*/
		
	switch(PARITY)
	{
		case 0:
		HAL_GPIO_CLEAR_bit(&UCSR0C,UPM00);
		HAL_GPIO_CLEAR_bit(&UCSR0C,UPM01);
		break;
						
		case 1:
		HAL_GPIO_SET_bit  (&UCSR0C,UPM01);
		break;
	
		case 2:
		HAL_GPIO_SET_bit  (&UCSR0C,UPM00);
		HAL_GPIO_SET_bit  (&UCSR0C,UPM01);
		break;
		
	}/*End switch Parity*/
	

	switch(SPEED)
	{
		/*Cálculo do Baud Rate*/
		case 1:
		
		HAL_GPIO_SET_bit(&UCSR0A,U2X0);
		
		UBRR0 = (F_CPU/(8UL*BPS))-1;
		
		break;
		
		case 0:
		
		HAL_GPIO_CLEAR_bit(&UCSR0A,U2X0);
		
		UBRR0 =   (F_CPU/(16UL*BPS))-1;	
		
		break;
	
	}/*End switch Speed*/
	
}/*End UART_CFG*/

/*=======================================================================================================================================================================================*/

void UART_INIT(void)
{
	
	UART_CFG(0,9600,0,0);
		
}/*End UART_INIT*/

/*=======================================================================================================================================================================================*/

