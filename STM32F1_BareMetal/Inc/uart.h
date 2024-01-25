/*
 * uart.h
 *
 *  Created on: Jan 25, 2024
 *      Author: Armand
 *      How to use:
 *      1. Call F1_UART_Init() with the uart, baudd, and pins as arguments
 *      2. Use USART1_IRQHandler() to access the receive interrupt (which is enabled by default)
 *      3. Use F1_UART_send_string() and F1_UART_send_char() to send the data
 */

#ifndef UART_H_
#define UART_H_

#include "stm32f1xx.h"
#include "rcc.h"
#include "gpio.h"


/*
 * Typedef for the UART status
 */
typedef enum{
    UART_OK = 0,
    UART_FAIL
}F1_uart_stat_t;

/*
 * Typedef for the UART used
 */
typedef enum{
    UART_1 = 0,
    UART_2,
    UART_3
}F1_uart_number_t;

/*
 * Typedef for the baud rate of the uarts
 */
typedef enum
{
    UART_BAUD_2400 = 2400,
    UART_BAUD_9600 = 9600,
    UART_BAUD_19200 = 19200,
    UART_BAUD_57600 = 57600,
    UART_BAUD_115200 = 115200,
    UART_BAUD_230769 = 230769,
    UART_BAUD_461538 = 461538,
    UART_BAUD_923076 = 923076,
    UART_BAUD_2250000 = 2250000
}F1_uart_baud_t;

/*
 * Typedef for the pins that can be used
 */
 typedef enum{
     UART1_TX_PA9_RX_PA10 = 0,
     UART1_TX_PB6_RX_PB7,
     UART2_TX_PA2_RX_PA3,
     UART3_TX_PB10_RX_PB11,
 }F1_uart_pin_map_t;

/*
 * ********************************************************************************
 * Functions
 * ********************************************************************************
 */


/*
 * @brief:
 * 	Initialize the UART peripheral for each uart
 * @arg:
 * 	F1_uart_number_t 	UART_x
 * @return:
 * 	F1_uart_stat_t		Status
 */
F1_uart_stat_t F1_UART_Init(F1_uart_number_t uart, F1_uart_baud_t bd, F1_uart_pin_map_t pinmap);

/*
 * @brief:
 * 	Send a character via UART
 * @arg:
 * 	F1_uart_number_t	UART to be used
 * 	char 			Character to be sent
 * @return:
 * 	None
 */
void F1_UART_send_char(F1_uart_number_t uart, char c);

/*
 * @brief:
 * 	Send a string via UART
 * @arg:
 * 	F1_uart_number_t	UART to be used
 * 	char* 			String to be sent
 * @return:
 * 	None
 */
void F1_UART_send_string(F1_uart_number_t uart, char* str);


#endif /* UART_H_ */
