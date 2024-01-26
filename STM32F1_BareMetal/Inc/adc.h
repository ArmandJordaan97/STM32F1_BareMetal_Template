/*
 * adc.h
 *
 *  Created on: 26 Jan 2024
 *      Author: Armand
 */

#ifndef ADC_H_
#define ADC_H_

#include "stm32f1xx.h"
#include "rcc.h"
#include "gpio.h"

#define MAX_DMA_BUFFER 1000

extern uint16_t dmaBuf[MAX_DMA_BUFFER];


/*
 * Typedef for the status of the ADC
 */
typedef enum
{
 ADC_OK = 0,
 ADC_FAIL
}F1_adc_status_t;

/*
 * Typedef to select the ADC
 */
typedef enum{
    ADC_1 = 0,
    ADC_2
}F1_adc_t;

/*
 * Typedef to select the channel to be sampled
 */
typedef enum{
    ADC_CH_0 = 0,
    ADC_CH_1,
    ADC_CH_2,
    ADC_CH_3,
    ADC_CH_4,
    ADC_CH_5,
    ADC_CH_6,
    ADC_CH_7,
    ADC_CH_8,
    ADC_CH_9,
    ADC_CH_10,
    ADC_CH_11,
    ADC_CH_12,
    ADC_CH_13,
    ADC_CH_14,
    ADC_CH_15,
    ADC_CH_TEMP
}F1_adc_channel_t;

/*
 * @brief:
 * 	Configure the ADC in general, but no channels are selected yet
 * @arg:
 * 	F1_adc_t		ADC selected (1 or 2)
 * @Return:
 *	F1_adc_status_t		Status
 */
F1_adc_status_t F1_ADC_Initialize(F1_adc_t adc);


/*
 * @brief:
 * 	Get and return the data for a single conversion in blocking mode
 * @arg:
 * 	F1_adc_t		ADC selected (1 or 2)
 * 	F1_adc_channel_t	ADC Channel
 * @Return:
 *	uint16_t		Raw data
 */
uint16_t F1_ADC_get_Data_Blocking(F1_adc_t adc, F1_adc_channel_t ch);

/*
 * @brief:
 * 	Start the conversion and enable the interrupt. Get value from the ISR
 * @arg:
 * 	F1_adc_t		ADC selected (1 or 2)
 * 	F1_adc_channel_t	ADC Channel
 * @Return:
 *	F1_adc_status_t		Status
 */
F1_adc_status_t F1_ADC_Start_Conversion_IT(F1_adc_t adc, F1_adc_channel_t ch);

/*
 * @brief:
 * 	Get the data from the data register
 * @arg:
 * 	F1_adc_t		ADC selected (1 or 2)
 * @Return:
 *	uint16_t		ADC Data
 */
uint16_t F1_ADC_Get_ADC_Data(F1_adc_t adc);

/*
 * @brief:
 * 	Start the conversion and enable the DMA on this channel
 * @arg:
 * 	F1_adc_t		ADC selected (1 or 2)
 * 	F1_adc_channel_t	ADC Channel
 * @Return:
 *	F1_adc_status_t		Status
 */
F1_adc_status_t F1_ADC_Start_DMA_ADC1(F1_adc_channel_t ch, uint16_t *adcDMAdata, uint16_t len);







#endif /* ADC_H_ */
