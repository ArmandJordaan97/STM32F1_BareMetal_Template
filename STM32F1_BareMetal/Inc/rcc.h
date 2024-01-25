/*
 * rcc.h
 *
 *  Created on: 24 Jan 2024
 *      Author: Armand
 */

#ifndef RCC_H_
#define RCC_H_

#include "stm32f1xx.h"

/*
 * Typedef to select the clock frequency of the MCU
 * Max frequency of the MCU with the HSI is 64MHz
 */
typedef enum{
    HSI_64MHZ = 0,
    HSE_72MHZ
}F1_clock_sel_t;

/*
 * Typedef for the status of the RCC
 */
 typedef enum{
     RCC_OK = 0,
     RCC_FAIL
 }F1_rcc_stat_t;

/*
 * Typedef to select the frequency of the Systick Timer Interrupts
 */
 typedef enum{
     SYSTICK_100HZ = 100,
     SYSTICK_1KHZ = 1000,
     SYSTICK_10KHZ = 10000,
     SYSTICK_100KHZ = 100000,
     SYSTICK_1MHZ = 1000000
 }F1_systick_sel_t;

/*
 * Struct with all the settings of the clock configuration
 */
 typedef struct{
     uint32_t clock_freq;	// Clock frequency of the system (Hz)
     uint32_t systick_freq;	// Frequency of the systic timer interrupts (Hz)
     uint32_t APB1_freq;	// Clock frequency of the APB1 peripherals
     uint32_t APB2_freq;	// Clock frequency of the APB2 peripherals
 }F1_rcc_config_t;


/*
 * Counter to be used across the entire program to create delays
 * It is incremented in the SysTick ISR
 */
extern uint32_t Systic_delay_counter;
/*
 * ********************************************************************************
 * Functions
 * ********************************************************************************
 */

/*
 * @brief:
 * 	Setup of the RCC for the MCU
 * @Input:
 * 	F1_clock_sel_t 		Clock frequency selection
 * 	F1_systick_sel_t  	Interrupt frequency of systick timer
 * @Return:
 * 	F1_rcc_stat_t		Status of the RCC
 *
 */
 F1_rcc_stat_t F1_System_Init(F1_clock_sel_t clock_freq, F1_systick_sel_t systick_freq);

 /*
  * @brief:
  * 	Delay in milliseconds
  * @arg:
  * 	uint16_t	Required delay in ms
  * @Return:
  *	None
  */
 void F1_delay_ms(uint16_t ms);

 /*
   * @brief:
   * 	Delay in microseconds
   * @arg:
   * 	uint16_t	Required delay in us
   * @Return:
   *	None
   */
 void F1_delay_us(uint16_t us);


/*
 * @brief:
 * 	Return the System clock frequency
 * @arg:
 * 	None
 * @Return:
 *	uint32_t	System Clock frequency
 */
uint32_t F1_RCC_get_sys_clock(void);

/*
 * @brief:
 * 	Return the SysTick clock frequency
 * @arg:
 * 	None
 * @Return:
 *	uint32_t	SysTick Clock frequency
 */
uint32_t F1_RCC_get_sysTick_clock(void);

/*
 * @brief:
 * 	Return the APB1 clock frequency
 * @arg:
 * 	None
 * @Return:
 *	uint32_t	APB1 Clock frequency
 */
uint32_t F1_RCC_get_APB1_clock(void);

/*
 * @brief:
 * 	Return the APB2 clock frequency
 * @arg:
 * 	None
 * @Return:
 *	uint32_t	APB2 Clock frequency
 */
uint32_t F1_RCC_get_APB2_clock(void);






#endif /* RCC_H_ */
