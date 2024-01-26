/*
 * adc.c
 *
 *  Created on: 26 Jan 2024
 *      Author: Armand
 */

#include "adc.h"


/*
 * **********************************************************************************************************
 * static (Private) Functions Start
 * **********************************************************************************************************
 */
/*
 * @brief:
 * 	Configure the selected pin as analog pin for ADC1
 * @arg:
 * 	F1_adc_channel_t	ADC Channel
 * @Return:
 *	F1_adc_status_t		Status
 */
static F1_adc_status_t _F1_ADC_configurePin(F1_adc_channel_t ch);



/*
 * **********************************************************************************************************
 * static (Private) Functions Ends
 * **********************************************************************************************************
 */

uint16_t dmaBuf[MAX_DMA_BUFFER];


F1_gpio_port_t _adc_channel_port[] = {GPIO_PORT_A,	// CH0
					GPIO_PORT_A,	// CH1
					GPIO_PORT_A,	// CH2
					GPIO_PORT_A,	// CH3
					GPIO_PORT_A,	// CH4
					GPIO_PORT_A,	// CH5
					GPIO_PORT_A,	// CH6
					GPIO_PORT_A,	// CH7
					GPIO_PORT_B,	// CH8
					GPIO_PORT_B,	// CH9
					GPIO_PORT_C,	// CH10
					GPIO_PORT_C,	// CH11
					GPIO_PORT_C,	// CH12
					GPIO_PORT_C,	// CH13
					GPIO_PORT_C,	// CH14
					GPIO_PORT_C,	// CH15
					GPIO_PORT_A};	// CHTEMP


F1_gpio_pin_t _adc_channel_pin[] = {GPIO_PIN_0,	// CH0
				    GPIO_PIN_1,	// CH1
				    GPIO_PIN_2,	// CH2
				    GPIO_PIN_3,	// CH3
				    GPIO_PIN_4,	// CH4
				    GPIO_PIN_5,	// CH5
				    GPIO_PIN_6,	// CH6
				    GPIO_PIN_7,	// CH7
				    GPIO_PIN_0,	// CH8
				    GPIO_PIN_1,	// CH9
				    GPIO_PIN_0,	// CH10
				    GPIO_PIN_1,	// CH11
				    GPIO_PIN_2,	// CH12
				    GPIO_PIN_3,	// CH13
				    GPIO_PIN_4,	// CH14
				    GPIO_PIN_5,	// CH15
				    GPIO_PIN_0};	// CHTEMP

ADC_TypeDef *_adc_selection[] = {ADC1, ADC2};

/*
 * Initialize the ADC
 */
F1_adc_status_t F1_ADC_Initialize(F1_adc_t adc)
{
    /*
     * Settings:
     * 		The data is right aligned by default
     */
    uint32_t apb2Clock = F1_RCC_get_APB2_clock();
    /*
    * Set the ADC pre-scaler to six or four:
    * If clock is 72MHz, ADC clock is 12MHz (6) (must be less than 14MHz)
    * if clock is 64MHz, ADC clock is 8MHz (8)
    */
    if(apb2Clock == 72000000)
    {
	RCC->CFGR |= RCC_CFGR_ADCPRE_DIV6;
    }
    else if (apb2Clock == 64000000)
    {
	RCC->CFGR |= RCC_CFGR_ADCPRE_DIV8;
    }


    if(adc == ADC_1)
    {
	/* Enable the clock */
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

	/* Enable the ADC */
	ADC1->CR2 |= ADC_CR2_ADON;

	/* Calibrate the ADC */
	ADC1->CR2 |= ADC_CR2_CAL;
	while(ADC1->CR2 & ADC_CR2_CAL);
    }
    else if (adc == ADC_2)
    {
	/* Enable the clock */
	RCC->APB2ENR |= RCC_APB2ENR_ADC2EN;
	/* Enable the ADC */
	ADC2->CR2 |= ADC_CR2_ADON;
	/* Calibrate the ADC */
	ADC2->CR2 |= ADC_CR2_CAL;
	while(ADC2->CR2 & ADC_CR2_CAL);
    }
    else
    {
	return ADC_FAIL;
    }
    return ADC_OK;
}


/*
 * Get the raw data from the ADC in blocking mode
 */
uint16_t F1_ADC_get_Data_Blocking(F1_adc_t adc, F1_adc_channel_t ch)
{
    /* Configure the pin as analog */
    _F1_ADC_configurePin(ch);

    /* Select independent mode (clear it to be sure since other channels could have set it) */
    _adc_selection[adc]->CR1 &= ~(ADC_CR1_DUALMOD);

    /* Clear the CONT bit to select single conversion (also clear if it was used in continuous mode before) */
    _adc_selection[adc]->CR2 &= ~(ADC_CR2_CONT);

    /* Clear this register to allow for a single conversion */
    _adc_selection[adc]->SQR1 &= ~(ADC_SQR1_L);

    if(ch > 9)
    {
	/* Set the sampling time to the longest possible */
	_adc_selection[adc]->SMPR1 |= (0x7 << (ch*3));
    }
    else
    {
	/* Set the sampling time to the longest possible */
	_adc_selection[adc]->SMPR2 |= (0x7 << (ch*3));
    }

    /* Since the amount of conversions is 1, set the SQ1 part of the register with the pin number */
    _adc_selection[adc]->SQR3 |= (ch << 0);

    /* Disable the interrupts */
    _adc_selection[adc]->CR1 &= ~(ADC_CR1_EOCIE);
    _adc_selection[adc]->SR &= ~(ADC_SR_EOC);
    NVIC_DisableIRQ(ADC1_2_IRQn);

    /* Start the conversion */
    _adc_selection[adc]->CR2 |= ADC_CR2_ADON;

    /* Wait until the conversion is complete */
    while((_adc_selection[adc]->SR & ADC_SR_EOC) == 0);

    /* Read the data from the register (all 32 bits) */
    uint32_t adcData = _adc_selection[adc]->DR;

    /* The top 16 bits are not used in single conversion mode, so clear it */
    adcData &= (0x0000FFFF);

    /* Cast the data back to 16 bits to return to the application */
    uint16_t dataToReturn = adcData;
    return dataToReturn;
}

/*
 * Start the ADC conversion for Interrupt
 */
F1_adc_status_t F1_ADC_Start_Conversion_IT(F1_adc_t adc, F1_adc_channel_t ch)
{
    /* Configure the pin as analog */
    _F1_ADC_configurePin(ch);

    /* Select independent mode (clear it to be sure since other channels could have set it) */
    _adc_selection[adc]->CR1 &= ~(ADC_CR1_DUALMOD);

    /* Clear the CONT bit to select single conversion (also clear if it was used in continuous mode before) */
    _adc_selection[adc]->CR2 &= ~(ADC_CR2_CONT);

    /* Clear this register to allow for a single conversion */
    _adc_selection[adc]->SQR1 &= ~(ADC_SQR1_L);

    if(ch > 9)
    {
	/* Set the sampling time to the longest possible */
	_adc_selection[adc]->SMPR1 |= (0x7 << (ch*3));
    }
    else
    {
	/* Set the sampling time to the longest possible */
	_adc_selection[adc]->SMPR2 |= (0x7 << (ch*3));
    }

    /* Since the amount of conversions is 1, set the SQ1 part of the register with the pin number */
    _adc_selection[adc]->SQR3 |= (ch << 0);

    /* Enable the interrupts */
    _adc_selection[adc]->CR1 |= ADC_CR1_EOCIE;
    NVIC_EnableIRQ(ADC1_2_IRQn);

    /* Start the conversion */
    _adc_selection[adc]->CR2 |= ADC_CR2_ADON;

    return ADC_OK;
}

/*
 * Start the DMA on this ADC pin
 */
F1_adc_status_t F1_ADC_Start_DMA_ADC1(F1_adc_channel_t ch, uint16_t *adcDMAdata, uint16_t len)
{
    /*
     * IMPORTANT!!!!!!!!!!
     *
     * ONLY ADC1 IS USED WITH DMA!!
     */

    /* Configure the pin as analog */
    _F1_ADC_configurePin(ch);

    /* Enable the DMA clock */
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;

    /* Select independent mode (clear it to be sure since other channels could have set it) */
    ADC1->CR1 &= ~(ADC_CR1_DUALMOD);

    /* Set the CONT bit to select continuous conversion for the DMA */
    ADC1->CR2 |= ADC_CR2_CONT;

    /* Clear this register to allow for a single conversion */
    ADC1->SQR1 &= ~(ADC_SQR1_L);

    if(ch > 9)
    {
	/* Set the sampling time to the longest possible */
	ADC1->SMPR1 |= (0x7 << (ch*3));
    }
    else
    {
	/* Set the sampling time to the longest possible */
	ADC1->SMPR2 |= (0x7 << (ch*3));
    }

    /* Since the amount of conversions is 1, set the SQ1 part of the register with the pin number */
    ADC1->SQR3 |= (ch << 0);

    /* Disable the interrupts */
    ADC1->CR1 &= ~(ADC_CR1_EOCIE);
    ADC1->SR &= ~(ADC_SR_EOC);
    NVIC_DisableIRQ(ADC1_2_IRQn);

    /* Enable DMA on the ADC */
    ADC1->CR2 |= ADC_CR2_DMA;

    /* Set the register of the peripheral */
    DMA1_Channel1->CPAR = (uint32_t)(&(ADC1->DR));

    /* Set the address of the memory */
    DMA1_Channel1->CMAR = (uint32_t)(adcDMAdata);

    /* Set the number of data to transfer */
    DMA1_Channel1->CNDTR = len;

    /* Set the priority to medium */
    DMA1_Channel1->CCR |= DMA_CCR_PL_0;

    /* Set memory and peripheral size to 16 bits */
    DMA1_Channel1->CCR |= DMA_CCR_MSIZE_0;
    DMA1_Channel1->CCR |= DMA_CCR_PSIZE_0;

    /* Set ONLY the increment of the memory, since we want to populate the buffer. */
    DMA1_Channel1->CCR |= DMA_CCR_MINC;

    /* Set the direction to read from the peripheral */
    DMA1_Channel1->CCR &= ~(DMA_CCR_DIR);

    /* Enable the transfer complete AND transfer error interrupt */
    DMA1_Channel1->CCR |= DMA_CCR_TCIE | DMA_CCR_TEIE;

    /* Enable the channel */
    DMA1_Channel1->CCR |= DMA_CCR_EN;
    NVIC_EnableIRQ(DMA1_Channel1_IRQn);

    /* Start the conversion */
    ADC1->CR2 |= ADC_CR2_ADON;

    return ADC_OK;
}


/*
 * Configure the ADC pin
 */
static F1_adc_status_t _F1_ADC_configurePin(F1_adc_channel_t ch)
{
    if(ch > 15)
    {
	return ADC_FAIL;
    }
    /* Set the pin as an input */
    F1_GPIO_Pin_Setup_INPUT(_adc_channel_port[ch], _adc_channel_pin[ch], GPIO_INPUT_ANALOG);
    return ADC_OK;
}

/*
 * Returns the value of the data register
 */
uint16_t F1_ADC_Get_ADC_Data(F1_adc_t adc)
{
    return _adc_selection[adc]->DR;
}






