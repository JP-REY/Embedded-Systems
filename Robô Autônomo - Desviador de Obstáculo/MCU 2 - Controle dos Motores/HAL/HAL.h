/*======================================================================= Biblioteca HAL.h =============================================================================================
  
  Biblioteca destinada à criação de Camada de Abstração de Hardware
  
  Date: 26/11/2025
  
  Author: João Pedro Rey

/*==================================================================== Definição da Biblioteca ==========================================================================================*/

#ifndef HAL_H_
#define HAL_H_

/*==================================================================== Bibliotecas Utilizadas ==========================================================================================*/

#include <avr/io.h>
#include <stdint.h>

/*==================================================================== Protótipos de Funções ===========================================================================================*/

void	HAL_GPIO_OUTPUT_pin (volatile uint8_t *DDR, volatile uint8_t *PORT, volatile uint8_t PORT_bit);
void	HAL_GPIO_INPUT_pin  (volatile uint8_t *DDR, volatile uint8_t *PORT, volatile uint8_t PORT_bit);
void	HAL_GPIO_TOGGLE_pin (volatile uint8_t *PORT, volatile uint8_t PORT_bit);
void	HAL_GPIO_SET_bit	(volatile uint8_t *PORT, volatile uint8_t PORT_bit);
void	HAL_GPIO_CLEAR_bit  (volatile uint8_t *PORT, volatile uint8_t PORT_bit);
uint8_t HAL_GPIO_READ_pin   (volatile uint8_t *PIN, volatile uint8_t PIN_bit);

#endif /* HAL_H_ */