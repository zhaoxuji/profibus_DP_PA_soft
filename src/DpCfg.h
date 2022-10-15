#ifndef DP_CFG_H
#define DP_CFG_H

#include "main.h"
/*--------------------------------------------------------------------------*/
/* profibus services                                                        */
/*--------------------------------------------------------------------------*/
#define PA_PROFILE_ENABLE     0 //使能PA行规
#define LIMIT_FOR_SP_APP      1 //限制库针对特定的一个应用
#define LIMIT_FOR_RUN_TIME    1 //限制库运行时间

/* todo: setup required services */
#ifndef LITTLE_ENDIAN
#define LITTLE_ENDIAN  1
#endif

#define C1_LEN           240
#define C2_LEN           240
#define C2_NUM_SAPS      4            //must <= 4
/*-----------------------------------------------------------------------*/
/* general slave parameter                                               */
/*-----------------------------------------------------------------------*/
/*!
   \def DP_ADDR
   \brief Set PROFIBUS slave address.

   Use following values:
   - 1 to 126
*/
#define DP_ADDR                     ((uint8_t)0x02)     // Slave address
#define PA_ADDR                     ((uint8_t)0x03)     // Slave address

/*!
   \def IDENT_NR
   \brief Set PROFIBUS ident number.

   Request your own number (www.profibus.com).
*/
#define IDENT_NR                    ((uint16_t)0x0008)   // PROFIBUS Ident Number
#define PA_IDENT_NR                 ((uint16_t)0x0c03)   // PROFIBUS Ident Number

/*!
   \def USER_WD
   \brief Set PROFIBUS user watchdog.
   The UserWatchdog is only active in DataExchange. The UserWatchdog
   isn't timebased, it's a counter of DataExchange-telegrams.
*/
#define USER_WD                     ((uint16_t)0x01FF)   // User Watchdog

/*-----------------------------------------------------------------------*/
/* define buffer length                                                  */
/*-----------------------------------------------------------------------*/
/*!
   \def DIN_BUFSIZE
   \brief Set length of the DIn buffer (data from slave to master).

   Use following values:
   - 0 to 244
*/
#define DIN_BUFSIZE                 ((uint8_t)0x03)     // Length of the DIn Buffer (Data Slave to Master)                0..244
#define PA_DIN_BUFSIZE              ((uint8_t)28)     // Length of the DIn Buffer (Data Slave to Master)
/*!
   \def DOUT_BUFSIZE
   \brief Set length of the DOut buffer (data from master to slave).

   Use following values:
   - 0 to 244
*/
#define DOUT_BUFSIZE                ((uint8_t)0x01)     // Length of the DOut Buffer     (Data Master to Slave)           0..244
#define PA_DOUT_BUFSIZE             ((uint8_t)28)     // Length of the DOut Buffer     (Data Master to Slave)
/*!
   \def PRM_BUFSIZE
   \brief Set length of the paramater buffer.

   The first seven byte are mandatory.

   Use following values:
   - 7 to 244
*/
#define PRM_BUFSIZE                 ((uint8_t)15)     // Length of the Parameter Buffer                                 7..244
#define PA_PRM_BUFSIZE              ((uint8_t)15)     // Length of the Parameter Buffer
/*!
   \def DIAG_BUFSIZE
   \brief Set length of the diagnosis buffer.

   The first 6 bytes are mandatory.

   Use following values:
   - 6 to 244
*/
#define DIAG_BUFSIZE                ((uint8_t)0x18)     // Length of the Diagnosis Buffer                                 6..244
#define PA_DIAG_BUFSIZE             ((uint8_t)0x18)     // Length of the Diagnosis Buffer                                 6..244

/*!
   \def CFG_BUFSIZE
   \brief Set length of the configuration buffer.

   Use following values:
   - 1 to 244
*/
#define CFG_BUFSIZE                 ((uint8_t)0x03)     // Length of the Configuration Buffer                             1..244
#define PA_CFG_BUFSIZE              ((uint8_t)40)     // Length of the Configuration Buffer                             1..244

/*!
   \def SSA_BUFSIZE
   \brief Set length of the set slave address buffer.

   Use following values:
   - 0 - deactivates the service set slave address
   - 4 to 244 - service is activated
*/
#define SSA_BUFSIZE                 ((uint8_t)0x4)     // Length of the Input Data in the Set_Slave_Address-Buffer 0 and 4..244
                                                      // 0: SetSlaveAddress will be deactivated!

#define DP_MSAC_C1 1
#define DP_MSAC_C2 1
#define DP_ALARM   1
#define DPV1_IM_SUPP 1
//#define DP_SUBSCRIBER 0
//#define REDUNDANCY 0
//#define RS232_REDUNDANCY 0
//#define DP_TIMESTAMP 0
//#define DP_ISOCHRONOUS_MODE 0

// == MSAC2 ============================================================================
#if DP_MSAC_C2
   #define DP_C2_NUM_SAPS           C2_NUM_SAPS
   #define DP_C2_LEN                C2_LEN
#else
   #define DP_C2_NUM_SAPS           ((uint8_t)0x00)
   #define DP_C2_LEN                ((uint8_t)0x00)
#endif//#if DP_MSAC_C2



#endif

