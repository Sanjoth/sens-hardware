/**
******************************************************************************
* @file    user_config.h 
* @author  SLAB
* @version V1.0.0
* @date    14-Oct-2016
* @brief   Header
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
*
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
*   1. Redistributions of source code must retain the above copyright notice,
*      this list of conditions and the following disclaimer.
*   2. Redistributions in binary form must reproduce the above copyright notice,
*      this list of conditions and the following disclaimer in the documentation
*      and/or other materials provided with the distribution.
*   3. Neither the name of STMicroelectronics nor the names of its contributors
*      may be used to endorse or promote products derived from this software
*      without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
******************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/

/* Includes ------------------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Select the module which is to be used (ONLY ONE) */
//#define SIM7100E
#define SIM5360E

/* Enable SMS Function only if Phone Number is defined in #define PhoneNum# */
//#define USE_SMS_FUNC 

/* Enable Email Function only if EmailTo is defined in #define EmailTo# */
//#define USE_EMAIL_FUNC

/* Enable GPS Function only if to get GPS Coordinates */
//#define USE_GPS_FUNC



#define APN "\"airtelgprs.com\"" 
    
#define DOMAIN_NAME "\"sens-agriculture.herokuapp.com\""
#define DOMAIN_HEADER "GET /insertData?pid=p0001&temp=32&humid=70&ph=6&co=0.59&uv=7 HTTP/1.1\r\nHost: sens-agriculture.herokuapp.com\r\nConnection: close\r\n\r\n"
  
//#define DOMAIN_NAME "\"www.google.com\""
//#define DOMAIN_HEADER "GET / HTTP/1.1\r\nHost: www.google.com\r\nConnection: close\r\n\r\n"


#define MSG "Hello, this is SIM7100E"                          /* Message to be send **NOTE : Not More than 150 Characters */ 

#define EMAIL_SUBJECT "Test Mail from LTE\r\n"                   /* Subject of Email */
#define EMAIL_BODY    "Hello, this is LTE Expansion board\r\n"   /* Body of Email */
    
#define SMTPS_SERVER "smtp.gmail.com"
#define SMTPS_PORT "465"
#define EMAIL_USERNAME "\"sanjothshaw@gmail.com\""              /* Login - Username */
#define EMAIL_PASSWORD "\"blabasdasdf\""                      /* Login - Password */
#define EMAIL_FROM "\"SLAB\""                                    /* Name of Email Sender */ 
    

/* Enter Max. 10 Phone Numbers in the following #define */
#define PhoneNum1 "+917011667121"
#define PhoneNum2 ""
#define PhoneNum3 ""
#define PhoneNum4 ""
#define PhoneNum5 ""
#define PhoneNum6 ""
#define PhoneNum7 ""
#define PhoneNum8 ""
#define PhoneNum9 ""
#define PhoneNum10 ""

/* Enetr recipient Email IDs and there name under respective recipient mode */
/* Email IDs for TO Recipient */
#define EmailToID1 ""
#define EmailToName1 ""
#define EmailToID2 ""
#define EmailToName2 ""
#define EmailToID3 ""
#define EmailToName3 ""
#define EmailToID4 ""
#define EmailToName4 ""
#define EmailToID5 ""
#define EmailToName5 ""

/* Email IDs for Cc Recipient */
#define EmailCcID1 "systemlab123@gmail.com"
#define EmailCcName1 "SYS LAB"
#define EmailCcID2 ""
#define EmailCcName2 ""
#define EmailCcID3 ""
#define EmailCcName3 ""
#define EmailCcID4 ""
#define EmailCcName4 ""
#define EmailCcID5 ""
#define EmailCcName5 ""

/* Email IDs for Bcc Recipient */
#define EmailBccID1 ""
#define EmailBccName1 ""
#define EmailBccID2 ""
#define EmailBccName2 ""
#define EmailBccID3 ""
#define EmailBccName3 ""
#define EmailBccID4 ""
#define EmailBccName4 ""
#define EmailBccID5 ""
#define EmailBccName5 ""

/* Exported constants --------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
