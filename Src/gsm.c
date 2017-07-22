/**
******************************************************************************
* @file    gsm.c
* @author  System Lab
* @version V1.0
* @date    14/10/2016
* @brief   GSM Module(SIM7100E) HAL File
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

/* Includes ------------------------------------------------------------------*/
#include "gsm.h"
#include "stdint.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "stm32l1xx_hal.h"
#include "user_config.h"

/** @addtogroup User
* @{ 
*/

/** @defgroup USER 
* @brief GSM Module  modules
* @{
*/ 

/** @defgroup GSM_Private_TypesDefinitions
* @{
*/f
UART_HandleTypeDef huart2;

/**
* @}
*/

void GSM_GpioConfig(void);

/** @defgroup GSM_Private_Defines
* @{
*/

/**
* @}
*/

/** @defgroup GSM_Private_Macros
* @{
*/

/**
* @}
*/

/** @defgroup GSM_Private_Variables
* @{
*/
/*********************** Command Strings - Modem Initialization  **************/

/* Commands to intialize the LTE Module */
const char GSM_AtCmdAtchk[]      = "AT\r\n";
const char GSM_AtCmdEchoDis[]    = "ATE0\r\n";
const char GSM_AtCmdDRAuto[]     = "AT+IPR=0\r\n";
const char GSM_AtCmdIdentity[]   = "ATI\r\n";
const char GSM_AtCmdDataFlCrtl[] = "AT+IFC?\r\n";
const char GSM_AtCmdReqCapablty[]= "AT+GCAP\r\n";
const char GSM_AtCmdModuleReset[]= "AT+CRESET\r\n";
const char GSM_AtCmdServicePro[] = "AT+CSPN?\r\n";
const char GSM_AtCmdNwkMode[]    = "AT+CNSMOD?\r\n";

/* Commands to check Network Registration */
const char GSM_AtCmdNetReg[] = "AT+CREG?\r\n";
const char GSM_AtCmdGPRSReg[]= "AT+CGREG?\r\n";
const char GSM_AtCmdSigQual[]= "AT+CSQ\r\n";

/* GPRS init Commands */
const char GSM_AtCmdSockPdpSet[]= "AT+CGSOCKCONT=1,\"IP\","APN",\"0.0.0.0\",0,0\r\n";
const char GSM_AtCmdSockPnSet[] = "AT+CSOCKSETPN=1\r\n";
const char GSM_AtCmdIpModeSet[] = "AT+CIPMODE=0\r\n";
const char GSM_AtCmdSockOpen[]  = "AT+NETOPEN\r\n"; 
const char GSM_AtCmdGetIpAddr[] = "AT+CDNSGIP="DOMAIN_NAME"\r\n";

/* GPRS Send Data via TCP */
const char GSM_AtCmdIpOpen[]   = "AT+CIPOPEN=0,\"TCP\",";
const char GSM_AtSendTcpData[] = "AT+CIPSEND=0,";
const char GSM_AtGetTcpData[]  = "AT+CIPRXGET=2,1,1500";
const char GSM_AtTcpConnClose[]= "AT+CIPCLOSE=0\r\n";

/* GPRS via HTTP */
const char GSM_AtHttpOpen[] = "AT+CHTTPACT="DOMAIN_NAME",80\r\n";
//const char GSM_AtDataPostHttpHeader[] = DOMAIN_HEADER;
char GSM_AtDataPostHttpHeader[] = DOMAIN_HEADER;


/************************ Command Strings - GPRS Send Data ********************/
const char *GSM_AtCmdInit[2]     = {GSM_AtCmdAtchk, GSM_AtCmdEchoDis};
const char *GSM_AtCmdNwReg[3]    = {GSM_AtCmdNetReg,GSM_AtCmdGPRSReg, GSM_AtCmdSigQual};
const char *GSM_AtCmdGprsInit[5] = {GSM_AtCmdSockPdpSet,GSM_AtCmdSockPnSet, GSM_AtCmdIpModeSet, GSM_AtCmdSockOpen, GSM_AtCmdGetIpAddr};
//const char *GSM_AtRespTCPData[4] = {GSM_AtRespIpOpen, GSM_AtRespSendTcpData, GSM_AtRespTCPDataSent, GSM_AtGetTcpData};
const char *GSM_AtCmdHttpReq[3]  = {GSM_AtCmdSockPnSet, GSM_AtHttpOpen, GSM_AtDataPostHttpHeader};
const char *GSM_AtRespHttpData[3]= {"OK", "REQUEST", "</HTML>"};
const char *GSM_AtCmdSysInfo[3] = {GSM_AtCmdServicePro, GSM_AtCmdNwkMode, GSM_AtCmdSigQual};

char numIndex = 49;     /* ASCII Value of 1 */
const char ctrlz = 26 ; /* ASCII Value of <Ctrl+Z> */
const char comma = 44;  /* ASCII Value of , */

uint8_t numCount = 0;
uint8_t tempCount = 0, toCount = 0, ccCount = 0, bccCount = 0;

GSM_CmdStatus_Typedef GSM_CmdStatus = GSM_CMD_IDLE;
char stout[45 * 31];
char GSM_RxBuf[1024];
char GSM_DataBuff[500];
char GPS_Data[100];
uint16_t GSM_RxBufCntr = 0;
uint16_t GSM_DataBufCntr = 0;
static uint8_t GSM_CmdCntr = 0;
static GSM_ModemState_Typedef GSM_ModemState = GSM_STATE_INIT;          // Declaration of GSM_ModemState           
static uint16_t GSM_RespTimer = 0;

GSM_SimInfo_TypeDef GSM_SimInfo;
GPS_TypeDef GPS;

uint8_t preCmdCntr = 0;
GSM_ModemState_Typedef preModemState = GSM_STATE_INIT;
extern uint8_t tempFlag;

volatile uint8_t UART_RxBuffer[10];

/**
* @}
*/

/** @defgroup GSM_Private_FunctionPrototypes
* @{
*/

static void GSM_GpioConfig(void);
static void GSM_StateInit(void);
//static void GSM_SysInfo(void);
static void GSM_StateHttpOpen(void);
static void GSM_SendString(const char *pATCmdstrng);
static void GSM_SendData(const char *pATCmdData, uint16_t count);
static void GSM_StateReset(void);
static void GSM_ClrBuf(void);
static void GSM_ClrDataBuf(void);
// Newly Declared
static void GSM_SysInfo(void);
static void GSM_SmsConfig(void);
static void GSM_SendSms(void);
static void GSM_EmailConfig(void);
static void GSM_SendEmail(void);
static void GSM_GpsLoc(void);


/* This table contains a pointer to the function to call in each state.*/

void (*GSM_state_table[9])(void) =  {
  GSM_StateInit,
  // Edit Started here
  GSM_SysInfo,
  GSM_SmsConfig,
  GSM_SendSms,
  GSM_EmailConfig,
  GSM_SendEmail,
  GSM_GpsLoc,
  GSM_StateHttpOpen,
  GSM_StateReset, // Edit Ended here
 // GSM_SysInfo, OLD Commented
  // GSM_StateHttpOpen, OLD declarations
 //  GSM_StateReset, OLD declarations
};

/* GSM_STATE_INIT = 0,
  GSM_SYS_INFO,
  GSM_SMS_CONFIG,
  GSM_SEND_SMS,
  GSM_EMAIL_CONFIG,
  GSM_SEND_EMAIL,
  GSM_GPS_LOC,
  GSM_STATE_HTTP_OPEN,
  GSM_STATE_RESET,
*/

/**
* @}
*/

/** @defgroup GSM_Private_Functions
* @{
*/


/**
* @brief  Configures GSM Module
* @param  None
* @retval None
*/
void GSM_UsartConfig(unsigned long commSpeed)
{
  GSM_GpioConfig(); /*Configure peripheral GPIO*/
  GSM_3V9_PWR_ON();
  GSM_3V3_PWR_ON();
  
  GSM_USART_CLK_ENABLE();/* Enable USART2 clock */
  
  /*##-1- Configure the UART peripheral ######################################*/
  /* Put the USART peripheral in the Asynchronous mode (UART Mode) */
  huart2.Instance        = GSM_USART;
  huart2.Init.BaudRate   = commSpeed;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits   = UART_STOPBITS_1;
  huart2.Init.Parity     = UART_PARITY_NONE;
  huart2.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
  huart2.Init.Mode       = UART_MODE_TX_RX;
  
  if(HAL_UART_Init(&huart2) != HAL_OK)
  {
    while(1);
  }
  /*##-3- Configure the NVIC for UART ########################################*/
  /* NVIC for USART */
  HAL_NVIC_SetPriority(GSM_USART_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(GSM_USART_IRQn);
  HAL_UART_Receive_IT(&huart2, (uint8_t*)UART_RxBuffer, 1);
  
}

/**
* @brief  UART Receiver Callback Function
* @param  None
* @retval None
*/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if(UART_RxBuffer[0] == 0x0A)
  {
    if(GSM_CmdStatus == GSM_CMD_RES_PENDING)
    {
      GSM_CmdStatus = GSM_CMD_RES_GOT;
    }
  }
  if(UART_RxBuffer[0] != 0x00)
  {  
    GSM_RxBuf[GSM_RxBufCntr++] = UART_RxBuffer[0];
  }
  HAL_UART_Receive_IT(&huart2, (uint8_t*)UART_RxBuffer, 1);
}

/**
* @brief  GSM State Machine
* @param  None
* @retval None
*/
void GSM_StateMachine(char MyHeader[])
{
  snprintf(GSM_AtDataPostHttpHeader, 256, "%s", MyHeader);
  GSM_state_table[GSM_ModemState]();   
}

/**
* @brief  GSM Modem State : Modem initialization 
* @param  None
* @retval None
*/
static void GSM_StateInit(void)
{ 
  /* Check to send Command to MODEM */
  if(GSM_CmdStatus == GSM_CMD_IDLE)
  {
    GSM_CmdStatus = GSM_CMD_SENDING;
    GSM_RespTimer = 0;
    GSM_SendString(GSM_AtCmdInit[GSM_CmdCntr]);
  }
  /* Check whether a response has been received  */
  else if(GSM_CmdStatus == GSM_CMD_RES_GOT)
  {
    if(strstr((const char*)GSM_RxBuf, "OK") != NULL) // CHeck if response is OK
    {
      GSM_CmdCntr++;
      if(GSM_CmdCntr == 2)
      {
        /* Shifting to next Modem State */
        GSM_ModemState = GSM_STATE_HTTP_OPEN;//GSM_SYS_INFO;//;//
        GSM_CmdCntr = 0;
      }
    }
    else if(strstr((const char*)GSM_RxBuf, "ERROR") != NULL)   // For error
    {
      GSM_ModemState = GSM_STATE_RESET;
    }
    // Clear all buffers and counters
    GSM_CmdStatus = GSM_CMD_IDLE;
    GSM_RxBufCntr = 0;
    GSM_ClrBuf();
  }
  if(GSM_RespTimer > 15) // if response time > 10 seconds then RESET
  {
    GSM_ModemState = GSM_STATE_RESET;
    GSM_RxBufCntr = 0;
    GSM_CmdStatus = GSM_CMD_IDLE;
    GSM_ClrBuf();
  }
  GSM_RespTimer++;
  HAL_Delay(500);
}


/**
* @brief  GSM Modem State : GPRS data Communication By HTTP Commands
* @param  None
* @retval None
*/
static void GSM_StateHttpOpen(void)
{
  if(GSM_CmdStatus == GSM_CMD_IDLE) 
  {
    GSM_CmdStatus = GSM_CMD_SENDING;
    GSM_RespTimer = 0;
    GSM_SendString(GSM_AtCmdHttpReq[GSM_CmdCntr]); // Sending Headers here
    if(GSM_CmdCntr == 2)
      GSM_SendData(&ctrlz,1);/* Terminating the Header */
    
  }
  /* Check whether a response has been received  */
  else if(GSM_CmdStatus == GSM_CMD_RES_GOT)
  {
    if(strstr((const char*)GSM_RxBuf, GSM_AtRespHttpData[GSM_CmdCntr]) != NULL)
    {
      GSM_CmdCntr++;
      if(GSM_CmdCntr == 3) //breakpoint
      {
          GSM_CmdCntr = 1; //Resetting Command Counter to 1
          HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_5);
          tempFlag = 0;
      }
    }
  else if(strstr((const char*)GSM_RxBuf, "ERROR") != NULL) // IF ERROR
    {
      GSM_ModemState = GSM_STATE_RESET;
      GSM_CmdStatus = GSM_CMD_IDLE;
      GSM_CmdCntr = 0;
      GSM_ModemState = GSM_STATE_HTTP_OPEN;
    }
    
      else if(strstr((const char*)GSM_RxBuf, "NO CARRIER") != NULL) // IF ERROR
    {
      GSM_ModemState = GSM_STATE_RESET;
      GSM_CmdStatus = GSM_CMD_IDLE;
      GSM_CmdCntr = 0;
      GSM_ModemState = GSM_STATE_HTTP_OPEN;
    }
    
   GSM_ClrBuf(); // Clr Buffer Everytime
   GSM_CmdStatus = GSM_CMD_IDLE; // Cmd Status set to IDLE
   GSM_RxBufCntr = 0;
  }
  if(GSM_RespTimer > 20) // If response time > 40 then RESET n Clr buffers 
  {
    GSM_ModemState = GSM_STATE_RESET;
    GSM_RxBufCntr = 0;
    GSM_CmdStatus = GSM_CMD_IDLE;
    GSM_ClrBuf();
    GSM_CmdCntr = 0;
  }
  GSM_RespTimer++;
  HAL_Delay(500);
  
}

/**
* @brief  GSM Modem State : Reset state when any error occurs  
* @param  None
* @retval None
*/
static void GSM_StateReset(void)
{
  static uint8_t bootingTime = 0;
  
  if(GSM_CmdStatus == GSM_CMD_IDLE)
  {
    GSM_CmdCntr = 0;
    GSM_RxBufCntr = 0;
    GSM_RespTimer = 0;
    bootingTime = 0;
    GSM_ClrBuf();
    GSM_ClrDataBuf();
    GSM_RST_PIN_LOW();
    HAL_Delay(1000);
    GSM_RST_PIN_HIGH();
    GSM_CmdStatus = GSM_CMD_RES_PENDING;
  }
  else if(GSM_CmdStatus == GSM_CMD_RES_GOT)
  {
    if(strstr(GSM_RxBuf, "Booting") != NULL || strstr(GSM_RxBuf, "START") != NULL) // FOR SIM5360E  - strstr(GSM_RxBuf, "START") != NULL
    {
      bootingTime++;
      if (bootingTime > 10)
      {
        GSM_CmdStatus = GSM_CMD_IDLE;
        GSM_ModemState = GSM_STATE_INIT;
        GSM_ClrBuf();
        GSM_RxBufCntr = 0;
      }
    }
  }
  if(GSM_RespTimer > 15)
  {
    GSM_3V9_PWR_OFF();
    HAL_Delay(1000);
    GSM_3V9_PWR_ON();
    GSM_RespTimer = 0;
    GSM_UsartConfig(115200);
    GSM_CmdStatus = GSM_CMD_RES_PENDING;
  }
  GSM_RespTimer++;
  HAL_Delay(500);
}

/**
* @brief  Sending a String 
* @param  None
* @retval None
*/
static void GSM_SendString(const char *pATCmdstrng)
{ 
  while(*pATCmdstrng != 0x00)
  {
    HAL_UART_Transmit(&huart2,(uint8_t*)pATCmdstrng++, 1, 500);
  } 
  GSM_CmdStatus = GSM_CMD_RES_PENDING;
}

/**
* @brief  Sending Data
* @param  None
* @retval None
*/
static void GSM_SendData(const char *pATCmdData, uint16_t count)
{
  GSM_CmdStatus = GSM_CMD_RES_PENDING;
  while(count--)
  {
    HAL_UART_Transmit(&huart2,(uint8_t*) pATCmdData, 1, 500);
    pATCmdData++;
  }
}

/**
* @brief  Clearing RxBuffer
* @param  None
* @retval None
*/
static void GSM_ClrBuf(void)
{
  uint16_t tempReg8;
  for(tempReg8 = 0; tempReg8 < 800; tempReg8++)
  {
    GSM_RxBuf[tempReg8] = 0;
  }
}

/**
* @brief  Clearing Data Buffer
* @param  None
* @retval None
*/
static void GSM_ClrDataBuf(void)
{
  uint16_t tempReg16;
  for(tempReg16 = 0; tempReg16 < 500; tempReg16++)
  {
    GSM_DataBuff[tempReg16] = 0;
  }
}


/**
* @brief  GSM module GPIO Pin Configuration 
* @param  None
* @retval None
*/
static void GSM_GpioConfig(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  /* Configures 3V9 supply control pin*/ 
  GSM_3V9_PORT_CLK_ENABLE(); 
  GPIO_InitStructure.Pin = GSM_3V9_PIN;
  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GSM_3V9_PORT, &GPIO_InitStructure);  
  
  /* Configures 3V3 supply control pin*/ 
  GSM_3V3_PORT_CLK_ENABLE(); 
  GPIO_InitStructure.Pin = GSM_3V3_PIN;
  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GSM_3V3_PORT, &GPIO_InitStructure); 
  
  /* Configures Tx Pin of Uart */ 
  GSM_TX_PORT_CLK_ENABLE(); 
  GPIO_InitStructure.Pin = GSM_TX_PIN;
  GPIO_InitStructure.Mode = GPIO_MODE_AF_OD;
  GPIO_InitStructure.Pull = GPIO_PULLUP;
  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStructure.Alternate = GSM_USART_TX_AF;
  HAL_GPIO_Init(GSM_TX_PORT, &GPIO_InitStructure); 
  
  /* Configures Rx Pin of Uart */ 
  GSM_RX_PORT_CLK_ENABLE(); 
  GPIO_InitStructure.Pin = GSM_RX_PIN;
  GPIO_InitStructure.Mode = GPIO_MODE_AF_OD;
  GPIO_InitStructure.Pull = GPIO_PULLUP;
  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStructure.Alternate = GSM_USART_RX_AF;
  HAL_GPIO_Init(GSM_RX_PORT, &GPIO_InitStructure); 
  
  /* Configures RTS Pin of Uart */ 
  GSM_RTS_PORT_CLK_ENABLE(); 
  GPIO_InitStructure.Pin = GSM_RTS_PIN;
  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructure.Pull = GPIO_PULLUP;
  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GSM_RTS_PORT, &GPIO_InitStructure); 
  
  /* Configures CTS Pin of Uart */ 
  GSM_CTS_PORT_CLK_ENABLE(); 
  GPIO_InitStructure.Pin = GSM_CTS_PIN;
  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructure.Pull = GPIO_PULLUP;
  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GSM_CTS_PORT, &GPIO_InitStructure); 
  
  /* Configures RST Pin of Uart */ 
  GSM_RST_PORT_CLK_ENABLE(); 
  GPIO_InitStructure.Pin = GSM_RST_PIN;
  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructure.Pull = GPIO_PULLUP;
  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GSM_RST_PORT, &GPIO_InitStructure); 
  
  HAL_GPIO_WritePin(GSM_CTS_PORT,GSM_CTS_PIN, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GSM_RTS_PORT,GSM_RTS_PIN, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GSM_RST_PORT,GSM_RST_PIN, GPIO_PIN_SET); 
  
  /*Configure GPIO pin : PC13 */
  GPIO_InitStructure.Pin = GPIO_PIN_13;
  GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
}

static void GSM_SysInfo(void){}
static void GSM_SmsConfig(void){}
static void GSM_SendSms(void){}
static void GSM_EmailConfig(void){}
static void GSM_SendEmail(void){}
static void GSM_GpsLoc(void){}

/**
* @}
*/

/**
* @}
*/

/**
* @}
*/

/******************* (C) COPYRIGHT 2012 STMicroelectronics *****END OF FILE****/