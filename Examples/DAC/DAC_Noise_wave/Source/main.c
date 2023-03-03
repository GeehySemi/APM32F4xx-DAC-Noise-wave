/*!
 * @file        main.c
 *
 * @brief       Main program body
 *
 * @version     V1.0.2
 *
 * @date        2022-06-23
 *
 * @attention
 *
 *  Copyright (C) 2021-2022 Geehy Semiconductor
 *
 *  You may not use this file except in compliance with the
 *  GEEHY COPYRIGHT NOTICE (GEEHY SOFTWARE PACKAGE LICENSE).
 *
 *  The program is only for reference, which is distributed in the hope
 *  that it will be usefull and instructional for customers to develop
 *  their software. Unless required by applicable law or agreed to in
 *  writing, the program is distributed on an "AS IS" BASIS, WITHOUT
 *  ANY WARRANTY OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the GEEHY SOFTWARE PACKAGE LICENSE for the governing permissions
 *  and limitations under the License.
 */

/* Includes */
#include "main.h"
#include "Board.h"
#include "stdio.h"
#include "apm32f4xx_gpio.h"
#include "apm32f4xx_dac.h"
#include "apm32f4xx_adc.h"
#include "apm32f4xx_misc.h"
#include "apm32f4xx_usart.h"

/** @addtogroup Examples
  @{
  */

/** @addtogroup DAC_ADC
  @{
  */

/** @defgroup DAC_ADC_Macros Macros
  @{
*/

/* printf function configs to USART1*/
#define DEBUG_USART  USART1

/**@} end of group DAC_ADC_Macros*/

/** @defgroup DAC_ADC_Functions Functions
  @{
  */

/* use to generate analog signal for DAC*/
uint16_t dacData = 0;

/* Delay */
void Delay(uint32_t count);
/* DAC init */
void DAC_Init(void);
/* ADC init */
void ADC_Init(void);

/*!
 * @brief     Main program
 *
 * @param     None
 *
 * @retval    None
 */
int main(void)
{
    USART_Config_T usartConfigStruct;

    /* Configures LED2 and LED3 */
    APM_MINI_LEDInit(LED2);
    APM_MINI_LEDInit(LED3);

    /* USART configuration */
    USART_ConfigStructInit(&usartConfigStruct);
    usartConfigStruct.baudRate = 115200;
    usartConfigStruct.mode = USART_MODE_TX_RX;
    usartConfigStruct.parity = USART_PARITY_NONE;
    usartConfigStruct.stopBits = USART_STOP_BIT_1;
    usartConfigStruct.wordLength = USART_WORD_LEN_8B;
    usartConfigStruct.hardwareFlow = USART_HARDWARE_FLOW_NONE;

    /* COM1 init*/
    APM_MINI_COMInit(COM1, &usartConfigStruct);

    /* DAC init*/
    DAC_Init();



    while (1)
    {
		DAC_EnableSoftwareTrigger(DAC_CHANNEL_1);
    }
}

/*!
 * @brief     DAC Init
 *
 * @param     None
 *
 * @retval    None
 */
void DAC_Init()
{
    GPIO_Config_T   gpioConfig;
    DAC_Config_T    dacConfig;

    /* Enable GPIOA clock */
    RCM_EnableAHB1PeriphClock(RCM_AHB1_PERIPH_GPIOA);

    /* DAC out PA4 pin configuration */
    GPIO_ConfigStructInit(&gpioConfig);
    gpioConfig.mode    = GPIO_MODE_AN;
    gpioConfig.pupd    = GPIO_PUPD_NOPULL;
    gpioConfig.pin     = GPIO_PIN_4;
    GPIO_Config(GPIOA, &gpioConfig);

    /* Enable DAC clock */
    RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_DAC);

    /* DAC channel 1 configuration */
    DAC_ConfigStructInit(&dacConfig);
    dacConfig.trigger             = DAC_TRIGGER_SOFT;
    dacConfig.waveGeneration      = DAC_WAVE_GENERATION_NOISE;
    dacConfig.maskAmplitudeSelect = DAC_LFSR_MASK_BIT11;
    dacConfig.outputBuffer        = DAC_OUTPUT_BUFFER_ENBALE;
    DAC_Config(DAC_CHANNEL_1, &dacConfig);

     DAC_Enable(DAC_CHANNEL_1);
   
    DAC_ConfigChannel1Data(DAC_ALIGN_12BIT_L,0x7FF0);
	
	
	/* Enable DAC channel 1 */
   
	
}




/*!
 * @brief     Delay
 *
 * @param     count:  delay count
 *
 * @retval    None
 */
void Delay(uint32_t count)
{
    volatile uint32_t delay = count;

    while (delay--);
}

/*!
 * @brief     Redirect C Library function printf to serial port.
 *            After Redirection, you can use printf function.
 *
 * @param     ch:  The characters that need to be send.
 *
 * @param     *f:  pointer to a FILE that can recording all information
 *            needed to control a stream
 *
 * @retval    The characters that need to be send.
 */
int fputc(int ch, FILE* f)
{
    /* send a byte of data to the serial port */
    USART_TxData(DEBUG_USART, (uint8_t)ch);

    /* wait for the data to be send  */
    while (USART_ReadStatusFlag(DEBUG_USART, USART_FLAG_TXBE) == RESET);

    return (ch);
}

/**@} end of group DAC_ADC_Functions */
/**@} end of group DAC_ADC */
/**@} end of group Examples */
