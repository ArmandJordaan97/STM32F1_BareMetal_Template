/*
 * uart.c
 *
 *  Created on: Jan 25, 2024
 *      Author: Armand
 */

#include "uart.h"

static USART_TypeDef *_F1_uart_list[] = {USART1,
					USART2,
					USART3};

/*
 * Initializes the UART for the selected option
 */
F1_uart_stat_t F1_UART_Init(F1_uart_number_t uart, F1_uart_baud_t bd, F1_uart_pin_map_t pinmap)
{
    /* Map the pins */
    switch(pinmap)
    {
	case UART1_TX_PA9_RX_PA10:
	    F1_GPIO_Pin_Setup_OUT(GPIO_PORT_A, GPIO_PIN_9, GPIO_OUTPUT_2MHZ, GPIO_OUTPUT_AF_PP);	// Set the Tx pin as Push-Pull output
	    F1_GPIO_Pin_Setup_INPUT(GPIO_PORT_A, GPIO_PIN_10, GPIO_INPUT_FLOAT);	    		// Set the Rx pin sa Input floating
	    break;

	case UART1_TX_PB6_RX_PB7:
	    F1_GPIO_Pin_Setup_OUT(GPIO_PORT_B, GPIO_PIN_6, GPIO_OUTPUT_2MHZ, GPIO_OUTPUT_AF_PP);	// Set the Tx pin as Push-Pull output
	    F1_GPIO_Pin_Setup_INPUT(GPIO_PORT_B, GPIO_PIN_7, GPIO_INPUT_FLOAT);	    			// Set the Rx pin sa Input floating
	    AFIO->MAPR |= AFIO_MAPR_USART1_REMAP;							// Re-map the UART pins
	    break;

	case UART2_TX_PA2_RX_PA3:
	    F1_GPIO_Pin_Setup_OUT(GPIO_PORT_A, GPIO_PIN_2, GPIO_OUTPUT_2MHZ, GPIO_OUTPUT_AF_PP);	// Set the Tx pin as Push-Pull output
	    F1_GPIO_Pin_Setup_INPUT(GPIO_PORT_A, GPIO_PIN_3, GPIO_INPUT_FLOAT);	    			// Set the Rx pin sa Input floating
	    break;

	case UART3_TX_PB10_RX_PB11:
	    F1_GPIO_Pin_Setup_OUT(GPIO_PORT_B, GPIO_PIN_10, GPIO_OUTPUT_2MHZ, GPIO_OUTPUT_AF_PP);	// Set the Tx pin as Push-Pull output
	    F1_GPIO_Pin_Setup_INPUT(GPIO_PORT_B, GPIO_PIN_11, GPIO_INPUT_FLOAT);	    			// Set the Rx pin sa Input floating
	    break;

	default:
	    return UART_FAIL;
	    break;
    }

    if(uart == UART_1)
    {
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;		// Enable the UART1 clock
    }
    else if(uart == UART_2)
    {
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;		// Enable the UART2 clock
    }
    else if(uart == UART_3)
    {
	RCC->APB1ENR |= RCC_APB1ENR_USART3EN;		// Enable the UART3 clock
    }
    else
    {
	return UART_FAIL;
    }

    /* 1 Start bit, 8 Data bits, n Stop bit */
    _F1_uart_list[uart]->CR1 &= ~(USART_CR1_M);

    if(uart == UART_1)
    {
	/*
	 * Calculate the Baud Rate:
	 * UART1 gets it clock from APB2
	 */
	uint32_t sysClock = F1_RCC_get_APB2_clock();
	uint16_t baudVal = (uint16_t)(sysClock/bd);
	_F1_uart_list[uart]->BRR = baudVal;
    }
    else
    {
	/*
	 * Calculate the baud rate:
	 * UART2 and 3 get their clock from APB1
	 */
	uint32_t sysClock = F1_RCC_get_APB1_clock();
	uint16_t baudVal = (uint16_t)(sysClock/bd);
	_F1_uart_list[uart]->BRR = baudVal;
    }

    /* Set the TE (Transmission Enable) bit */
    _F1_uart_list[uart]->CR1 |= USART_CR1_TE;

    /* Enable the receiver RE */
    _F1_uart_list[uart]->CR1 |= USART_CR1_RE;

    /* Enable the UART */
    _F1_uart_list[uart]->CR1 |= USART_CR1_UE;

    /* Enable the receive interrupt */
    _F1_uart_list[uart]->CR1 |= USART_CR1_RXNEIE;
    if(uart == UART_1){NVIC_EnableIRQ(USART1_IRQn);}
    else if(uart == UART_2){NVIC_EnableIRQ(USART2_IRQn);}
    else if(uart == UART_3){NVIC_EnableIRQ(USART3_IRQn);}

    return UART_OK;
}


/*
 * Send a single character
 */
void F1_UART_send_char(F1_uart_number_t uart, char c)
{
    /* Load the data */
    _F1_uart_list[uart]->DR = c;

    /* Wait for the transmission to complete */
    while((_F1_uart_list[uart]->SR & USART_SR_TC) == 0);
}

/*
 * Send a string via UART
 */
void F1_UART_send_string(F1_uart_number_t uart, char* str)
{
    while(*str != '\0')
    {
	F1_UART_send_char(uart, *str);
	str++;
    }
}






