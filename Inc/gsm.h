/**
  ******************************************************************************
  * @file    gsm.h
  * @author  System Lab
  * @version V1.0
  * @date    6/2/2014
  * @brief   Header file gsm.c
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE 
  * TIME.AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY 
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING 
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE 
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * THIS SOURCE CODE IS PROTECTED BY A LICENSE.
  * FOR MORE INFORMATION PLEASE CAREFULLY READ THE LICENSE AGREEMENT FILE 
  * LOCATED IN THE ROOT DIRECTORY OF THIS FIRMWARE PACKAGE
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GSM_H
#define __GSM_H


/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
#include "user_config.h"

/** @addtogroup USER
  * @{
  */

/** @defgroup USER
  * @brief This file is the Header file for gsm.c
  * @{
  */ 

/** @defgroup GSM_Exported_Defines
  * @{
  */ 

    /********************* USART Configuration *****************************/

#define GSM_USART                               USART2
#define GSM_BAUD_RATE                           9600
#define GSM_USART_CLK_ENABLE()                  __USART2_CLK_ENABLE();
#define GSM_USART_TX_AF                         GPIO_AF7_USART2 
#define GSM_USART_RX_AF                         GPIO_AF7_USART2

#define GSM_USART_IRQn                          USART2_IRQn
#define GSM_USART_IRQHandler                    USART2_IRQHandler

#define GSM_TX_PIN                              GPIO_PIN_2
#define GSM_TX_PORT                             GPIOA
#define GSM_TX_PORT_CLK_ENABLE()                __HAL_RCC_GPIOA_CLK_ENABLE()  
#define GSM_TX_PORT_CLK_DISABLE()               __HAL_RCC_GPIOA_CLK_DISABLE() 

#define GSM_RX_PIN                              GPIO_PIN_3
#define GSM_RX_PORT                             GPIOA
#define GSM_RX_PORT_CLK_ENABLE()                __HAL_RCC_GPIOA_CLK_ENABLE()  
#define GSM_RX_PORT_CLK_DISABLE()               __HAL_RCC_GPIOA_CLK_DISABLE() 

#define GSM_CTS_PIN                             GPIO_PIN_11
#define GSM_CTS_PORT                            GPIOA
#define GSM_CTS_PORT_CLK_ENABLE()               __HAL_RCC_GPIOA_CLK_ENABLE()  
#define GSM_CTS_PORT_CLK_DISABLE()              __HAL_RCC_GPIOA_CLK_DISABLE() 

#define GSM_RTS_PIN                             GPIO_PIN_12
#define GSM_RTS_PORT                            GPIOA
#define GSM_RTS_PORT_CLK_ENABLE()               __HAL_RCC_GPIOA_CLK_ENABLE()  
#define GSM_RTS_PORT_CLK_DISABLE()              __HAL_RCC_GPIOA_CLK_DISABLE() 

#define GSM_3V3_PIN                             GPIO_PIN_8
#define GSM_3V3_PORT                            GPIOA
#define GSM_3V3_PORT_CLK_ENABLE()               __HAL_RCC_GPIOA_CLK_ENABLE()  
#define GSM_3V3_PORT_CLK_DISABLE()              __HAL_RCC_GPIOA_CLK_DISABLE() 

#define GSM_3V9_PIN                             GPIO_PIN_10
#define GSM_3V9_PORT                            GPIOA
#define GSM_3V9_PORT_CLK_ENABLE()               __HAL_RCC_GPIOA_CLK_ENABLE()  
#define GSM_3V9_PORT_CLK_DISABLE()              __HAL_RCC_GPIOA_CLK_DISABLE() 

#define GSM_RST_PIN                             GPIO_PIN_3
#define GSM_RST_PORT                            GPIOB
#define GSM_RST_PORT_CLK_ENABLE()               __HAL_RCC_GPIOB_CLK_ENABLE()  
#define GSM_RST_PORT_CLK_DISABLE()              __HAL_RCC_GPIOB_CLK_DISABLE() 
    
#define GSM_3V3_PWR_ON()      HAL_GPIO_WritePin(GSM_3V3_PORT,GSM_3V3_PIN, GPIO_PIN_SET)   
#define GSM_3V3_PWR_OFF()     HAL_GPIO_WritePin(GSM_3V3_PORT,GSM_3V3_PIN, GPIO_PIN_RESET) 
    
#define GSM_3V9_PWR_ON()      HAL_GPIO_WritePin(GSM_3V9_PORT,GSM_3V9_PIN, GPIO_PIN_SET)   
#define GSM_3V9_PWR_OFF()     HAL_GPIO_WritePin(GSM_3V9_PORT,GSM_3V9_PIN, GPIO_PIN_RESET) 
    
#define GSM_RST_PIN_HIGH()    HAL_GPIO_WritePin(GSM_RST_PORT,GSM_RST_PIN, GPIO_PIN_SET)   
#define GSM_RST_PIN_LOW()     HAL_GPIO_WritePin(GSM_RST_PORT,GSM_RST_PIN, GPIO_PIN_RESET) 

/**
  * @}
  */ 

/** @defgroup GSM_Exported_TypesDefinitions
* @{
*/

typedef enum
{
  GSM_CMD_IDLE = 0,
  GSM_CMD_SENDING,
  GSM_CMD_RES_PENDING,
  GSM_CMD_RES_GOT,
  GSM_DATA_READY
}GSM_CmdStatus_Typedef;

typedef enum{ // GSM Modem State Decs
  
  GSM_STATE_INIT = 0,
  GSM_SYS_INFO,
  GSM_SMS_CONFIG,
  GSM_SEND_SMS,
  GSM_EMAIL_CONFIG,
  GSM_SEND_EMAIL,
  GSM_GPS_LOC,
  GSM_STATE_HTTP_OPEN,
  GSM_STATE_RESET,
}GSM_ModemState_Typedef;

typedef struct
{
  char Rssi[4];                                             /* Received Signal Strength (rssi) Level of the SIM */
  char Name[12];                                            /* Name of the Service Provider */
  uint8_t SignalStrength;                                   /* Signal Strength of the Network */
  char Mode[30];                                            /* Network Mode 2G/3G/4G */
} GSM_SimInfo_TypeDef;

typedef struct
{
  char Latitude[15];                                        /* Latitude (ddmm.mmmmmm) */
  char NS[3];                                               /* North/South Indicator */
  char Longitude[15];                                       /* Longitude (dddmm.mmmmmm) */
  char EW[3];                                               /* East/West Indicator */
  char Date[10];                                            /* Today's Date (ddmmyy) */
  char Time[10];                                            /* Current time (hhmmss.s) */
  char Altitude[10];                                        /* Altitude (meters) */
  char Speed[10];                                           /* Speed (knots)*/
  char Course[5];                                           /* Course. Degrees */
} GPS_TypeDef;


/**
* @}
*/ 


/** @defgroup GSM_Exported_Macros
  * @{
  */
enum{
GPRS_CTRL = 0,
KEY_CTRL,
};
/**
  * @}
  */

/** @defgroup GSM_Exported_Variables
  * @{
  */ 


/**
  * @}
  */

/** @defgroup GSM_Exported_FunctionsPrototype
  * @{
  */ 

void GSM_StateMachine(char MyHeader[]); 
void GSM_UsartConfig(unsigned long commSpeed);

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#endif /* __GSM_H */
/******************* (C) COPYRIGHT 2014 STMicroelectronics *****END OF FILE****/
