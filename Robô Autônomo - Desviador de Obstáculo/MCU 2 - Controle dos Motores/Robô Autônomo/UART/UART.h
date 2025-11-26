/*======================================================================= Biblioteca UART.h =============================================================================================
  
  Biblioteca para inicialização de protocolo UART utilizando Ring Buffer
  
  Date: 07/11/2025
  
  Author: João Pedro Rey

===================================================================== Definição da Biblioteca ==========================================================================================*/

#ifndef UART_H_
#define UART_H_

/*===================================================================== Frequência de Clock ============================================================================================*/

#define F_CPU 16000000UL

/*==================================================================== Parâmetros do Buffer ============================================================================================*/
	
#define TX_BUFFER_size	64											/*Tamanho do vetor de Buffer TX*/
#define RX_BUFFER_size	64											/*Tamanho do vetor de Buffer RX*/
#define TX_MASK			(TX_BUFFER_size-1)							/*Máscara de bits do Buffer TX*/
#define RX_MASK			(RX_BUFFER_size-1)							/*Máscara de bits do Buffer RX*/

/*==================================================================== Bibliotecas Utilizadas ==========================================================================================*/

#include "HAL.h"
#include <avr/interrupt.h>

/*=================================================================== Declaração de Variáveis ==========================================================================================*/

extern	volatile	uint8_t		RX_write_index				,		/*Índice de escrita do Ring Buffer RX*/
								RX_read_index				,		/*Índice de leitura do Ring Buffer RX*/
								TX_write_index				,		/*Índice de escrita do Ring Buffer TX*/
								TX_read_index				,		/*Índice de leitura do Ring Buffer TX*/
								RX_BUFFER [RX_BUFFER_size]	,		/*Ring Buffer UART RX*/
								TX_BUFFER [TX_BUFFER_size]	;		/*Ring Buffer UART TX */
								


					typedef enum									/*Definição de tipo enum para os tipos de modos de protoclo USART do ATmega328P*/
					{
						Asynchrounous = 0,
						Synchronous ,
						MasterSPI
	
					}UART_MODE;

/*==================================================================== Protótipos de Funções ===========================================================================================*/

void	UART_INIT				(UART_MODE Mode , uint16_t Bps, uint8_t Speed, uint8_t Parity);		/*Inicializa protocolo USART conforme os parâmetros atribuídos aos argumentos da função*/
uint8_t UART_RX_READ_BYTE		(uint8_t *data_byte);												/*Função para leitura de um byte recebido no pino RX*/
uint8_t UART_TX_SEND_BYTE		(char byte);														/*Função de transmissão de um byte no pino TX*/
uint8_t UART_TX_SEND_STRING		(const char *str);													/*Função para transmissão de uma string no pino TX*/

#endif /* UART_H_ */