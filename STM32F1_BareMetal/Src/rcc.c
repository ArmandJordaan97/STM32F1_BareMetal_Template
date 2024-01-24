/*
 * rcc.c
 *
 *  Created on: 24 Jan 2024
 *      Author: Armand
 */


#include "rcc.h"

static F1_rcc_config_t rccConfig = {64000000,	// Clock is default 64MHz on HSI
				    1000,	// Default Systick interrupt frequency is 1kHz
				    36000000,	// APB1 Clock
				    36000000	// APB2 Clock
				    };

/*
 * This "extern" variable is declared in the .h file. It is initialized here so
 * it can be used in other files as well
 */
uint32_t Systic_delay_counter = 0;


/*
 * ISR for the Systick timer
 */
void SysTick_Handler(void)
{
    if(Systic_delay_counter > 0xFFFFFFF0)
    {
	Systic_delay_counter = 0;
    }
    else
    {
	Systic_delay_counter += 1;
    }
}

/*
 * Initialize the MCU and the RCC
 */
F1_rcc_stat_t F1_System_Init(F1_clock_sel_t clock_freq, F1_systick_sel_t systick_freq)
{
    /* 8MHz internal HSI selected */
    if(clock_freq == HSI_64MHZ)
    {
	rccConfig.clock_freq = 64000000;
	/* Internal 8MHz clock selected */
	RCC->CR |= RCC_CR_HSION;			//Enable the HSI 8MHz clock
	while((RCC->CR & RCC_CR_HSIRDY) == 0);		//Wait for the clock to become ready

	/* Enable the power IF clock */
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;

	/* Configure the Flash Access Control */
	FLASH->ACR |= FLASH_ACR_PRFTBE;			// Enable the pre-fecth buffer
	FLASH->ACR |= FLASH_ACR_LATENCY_1;		// Set the latency to two wait states (required at this frequency)

	/* Configure the PLL */
	RCC->CFGR &= ~(RCC_CFGR_PLLSRC);		// Set the PLL source as the HSI/2
	RCC->CFGR |= RCC_CFGR_PPRE1_2;			// Set the APB2 to /2 for a 64MHz/2 (32MHz) clock (36MHz is max)
	RCC->CFGR |= RCC_CFGR_PLLMULL16;
	RCC->CR |= RCC_CR_PLLON;			// Enable the PLL
	while((RCC->CR & RCC_CR_PLLRDY) == 0);		// Wait for the PLL to become ready

	/* Set the PLL as the system clock */
	RCC->CFGR |= RCC_CFGR_SW_PLL;			// Set the PLL as the system clock

	/* Enable the Systick Timer */
	uint32_t systick_clock = 64000000 / systick_freq;
	SysTick_Config(systick_clock);

	rccConfig.systick_freq = systick_freq;
	rccConfig.APB1_freq = 32000000;
	rccConfig.APB2_freq = 64000000;
    }

    /* HSE selected */
    else if(clock_freq == HSE_72MHZ)
    {
	/* External 8MHz clock selected */
	rccConfig.clock_freq = 72000000;
	/* Internal 8MHz clock selected */
	RCC->CR &= ~(RCC_CR_HSEBYP);			// Crystal mode (bypass is if only the CLKIN pin is used). See page 94 of FRM
	RCC->CR |= RCC_CR_HSEON;			// Enable the HSE
	while((RCC->CR & RCC_CR_HSERDY) == 0);		//Wait for the clock to become ready

	/* Enable the power IF clock */
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;

	/* Configure the Flash Access Control */
	FLASH->ACR |= FLASH_ACR_PRFTBE;			// Enable the pre-fecth buffer
	FLASH->ACR |= FLASH_ACR_LATENCY_1;		// Set the latency to two wait states (required at this frequency)

	/* Configure the PLL */
	RCC->CFGR |= RCC_CFGR_PLLSRC;			// Set the PLL source as the HSE
	RCC->CFGR |= RCC_CFGR_PPRE1_2;			// Set the APB2 to /2 for a 72MHz/2 (36MHz) clock (36MHz is max)
	RCC->CFGR |= RCC_CFGR_PLLMULL9;
	RCC->CR |= RCC_CR_PLLON;			// Enable the PLL
	while((RCC->CR & RCC_CR_PLLRDY) == 0);		// Wait for the PLL to become ready

	/* Set the PLL as the system clock */
	RCC->CFGR |= RCC_CFGR_SW_PLL;			// Set the PLL as the system clock

	/* Enable the Systick Timer */
	uint32_t systick_clock = 72000000 / systick_freq;
	SysTick_Config(systick_clock);

	rccConfig.systick_freq = systick_freq;
	rccConfig.APB1_freq = 36000000;
	rccConfig.APB2_freq = 72000000;
    }
    else
    {
	/* Not a valid clock frequency */
	return RCC_FAIL;
    }
    return RCC_OK;
}


/*
 * Delay in milliseconds
 */
void F1_delay_ms(uint16_t ms)
{
    uint32_t counts_needed = (ms * rccConfig.systick_freq)/1000;
    Systic_delay_counter = 0;
    while(Systic_delay_counter < counts_needed);
}

/*
 * Delay in microseconds
 */
void F1_delay_us(uint16_t us)
{
    uint16_t counts_needed = (us * rccConfig.systick_freq)/1000000;
    Systic_delay_counter = 0;
    while(Systic_delay_counter < counts_needed);
}







