/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include "stm32l4xx_ll_i2c.h"
#include "stm32l4xx_ll_tim.h"
#include "stm32l4xx_ll_bus.h"
#include "stm32l4xx_ll_cortex.h"
#include "stm32l4xx_ll_rcc.h"
#include "stm32l4xx_ll_system.h"
#include "stm32l4xx_ll_utils.h"
#include "stm32l4xx_ll_pwr.h"
#include "stm32l4xx_ll_gpio.h"
#include "stm32l4xx_ll_dma.h"

#include "stm32l4xx_ll_exti.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
#define DEMO_NUM_RESULTS  6
#define MSP_SPI_MSG_LENGTH sizeof(LSDemo_ChirpData)
/**
 * @brief
 *  Chirp output data
 *
 * @details
 *  The structure is contains the processed chirp data that will be transmitted
 *  from the IWR1443 to the MSP432.
 */

typedef struct LSDemo_ChirpData_t
{
    /*! * @brief   The top N distances captured by the radar chirp. */
    float                       distance[DEMO_NUM_RESULTS];

    /*! @brief   The raw power (squared) of the frequency bin. */
    uint32_t                    power_sqr[DEMO_NUM_RESULTS];
    /*! @brief   IWR1443 measurement error code. */
    int32_t                     err;
    int32_t                     pad;
} __attribute((aligned (4))) LSDemo_ChirpData;




/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void gpio_PGD24V_offpower(void);
void gpio_PGD24V_onpower(void);
void do_fmcw(void);
void output_measure(LSDemo_ChirpData *chirp_data);
void gpio_set_gpio1(uint32_t value);
void gpio_enter_ir1443_reset(void);
void gpio_release_ir1443_reset(void);
int32_t gpio_wait_on_ir14xx_gpio0(uint32_t pend_value);
LSDemo_ChirpData *perform_measurement(void);
void spi_swap_data(uint8_t *buffer, uint32_t length);
void *spi_get_transfer(uint32_t size);

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define IO_18B20_Pin GPIO_PIN_2
#define IO_18B20_GPIO_Port GPIOA
#define ADDR3_Pin GPIO_PIN_4
#define ADDR3_GPIO_Port GPIOA
#define ADDR2_Pin GPIO_PIN_5
#define ADDR2_GPIO_Port GPIOA
#define ADDR1_Pin GPIO_PIN_6
#define ADDR1_GPIO_Port GPIOA
#define ADDR0_Pin GPIO_PIN_7
#define ADDR0_GPIO_Port GPIOA
#define KEY3_Pin GPIO_PIN_0
#define KEY3_GPIO_Port GPIOB
#define HOTTING_Pin GPIO_PIN_1
#define HOTTING_GPIO_Port GPIOB
#define COOLING_Pin GPIO_PIN_2
#define COOLING_GPIO_Port GPIOB
#define HOTSET_Pin GPIO_PIN_10
#define HOTSET_GPIO_Port GPIOB
#define COOLSET_Pin GPIO_PIN_11
#define COOLSET_GPIO_Port GPIOB
#define LED_DEX_Pin GPIO_PIN_8
#define LED_DEX_GPIO_Port GPIOC
#define LED_RUN_Pin GPIO_PIN_9
#define LED_RUN_GPIO_Port GPIOC
#define UART1_RTS_Pin GPIO_PIN_11
#define UART1_RTS_GPIO_Port GPIOA
#define SPI_WP_Pin GPIO_PIN_6
#define SPI_WP_GPIO_Port GPIOB
#define SPI_HOLD_Pin GPIO_PIN_7
#define SPI_HOLD_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
//#define selfdeside 1    //self deside the top power
#define otherdeside 1   //other MCU deside the top power
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
