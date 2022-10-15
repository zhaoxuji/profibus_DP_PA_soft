/************************** Filename: dp_if.h ********************************/
/*****************************************************************************/
/* contents:

*/
/*****************************************************************************/


#ifndef DP_IF_H
#define DP_IF_H
#include "stdint.h"
#include "ctype.h"
#include "stdbool.h"
#include "string.h"
#include "stm32l431xx.h"
#include "platformDebug.h"

#include "DpCfg.h"
#include "dpl_list.h"

/*
 * 传输过程中，高字节在前，低字节在后
 * NBO(Network Byte Order)：按照从高到低的顺序存储。
 *
 */

#define _PACKED_    __packed


typedef struct
{
	DPL_STRUC_LIST_CB list;
	uint8_t           pkgLen;
	uint8_t           arry[256];
} _PACKED_ fdl_frame_t;

/*-----------------------------------------------------------------------------------------------------------*/
/* 2.0 parameter telegram                                                                                    */
/*-----------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* 2.1 structure of prm data                                                 */
/*---------------------------------------------------------------------------*/

/*
|--------------|------------|---------------|-----------------------|------------|-----------|------------------|-------------------|
|      7       |      6     |       5       |           4           |      3     |     2     |         1        |          0        |
|--------------|------------|---------------|-----------------------|------------|-----------|------------------|-------------------|
|   Lock_Req   | Unlock_Req |   Sync_Req    |       Freeze_Req      |    WD_On   |     0     |         0        |          0        |
|--------------|------------|---------------|-----------------------|------------|-----------|------------------|-------------------|
| WD_Fact_1 (1 bis 255)                                                                                                             |
|-----------------------------------------------------------------------------------------------------------------------------------|
| WD_Fact_2 (1 bis 255)                                                                                                             |
|-----------------------------------------------------------------------------------------------------------------------------------|
| Min. Station Delay Responder (min Tsdr)                                                                                           |
|-----------------------------------------------------------------------------------------------------------------------------------|
| Ident_Number (high)                                                                                                               |
|-----------------------------------------------------------------------------------------------------------------------------------|
| Ident_Number (low)                                                                                                                |
|-----------------------------------------------------------------------------------------------------------------------------------|
| Group_Ident                                                                                                                       |
|--------------|------------|---------------|-----------------------|------------|-----------|------------------|-------------------|
| DPV1_Enable  | Fail_Safe  | Publisher_En  |       reserved        |  reserved  |  WD_Base  | Dis_Stop_Control | Dis_Start_Control |
|--------------|------------|---------------|-----------------------|------------|-----------|------------------|-------------------|
| En_Pull_Plug | En_Process | En_Diagnostic | En_Manufacturer_Spec. | En_Status  | En_Update |     reserved     | Chk_Cfg_Mode      |
|    Alarm     |    Alarm   |    Alarm      |         Alarm         |   Alarm    |   Alarm   |                  |                   |
|--------------|------------|---------------|-----------------------|------------|-----------|------------------|-------------------|
|   PrmCmd     |  reserved  |   reserved    |       IsoM_Req        | Prm_Struct |                 Alarm_Mode                       |
|--------------|------------|---------------|-----------------------|------------|-----------|------------------|-------------------|
|                                                            User_Prm_Data                                                          |
|-----------------------------------------------------------------------------------------------------------------------------------|
*/

typedef struct
{
   union{
	   uint8_t b;
	   struct {
		   uint8_t resv:3;//
		   uint8_t WDOn:1;//(监视器接通)如果此位被设置为 0，那么监视器控制就被解除激活。如果此位被设置为 1，那么监视器控制就被激活。
		   uint8_t freezeReq:1;//一旦由 DDLM_Glogal_Control 功能传送了此控制命令，就用此位指示 DP 从站把
		   //在冻结模式之中操作。如果一个 DP 从站不支持冻结控制命令，它就在诊断信息之中设置
		   //Diag.Not_Supported 位。由于在参数化阶段中进行了检查，因此在用户数据交换模式中就避免了错误。
		   uint8_t syncReq:1;//一旦由 DDLM_Glogal_Control 功能传送了此控制命令，就用此位指示 DP 从站在同步模式之中操作。
		   //如果一个 DP 从站不支持同步控制命令，它就在诊断信息之中设置Diag.Not_Supported 位。由于在参数化阶段中进行了检查，因此在用户数据交换模式中就避免了错误。
		   uint8_t unlockReq:1;//
		   uint8_t LockReq:1;//
	   }sbit;
   }bStationState;
   uint8_t bWdFactor1;//范围： 1 到 255,TWD （以 s 为单位） = 10 ms * WD_Fact_1* WD_Fact_2
   uint8_t bWdFactor2;//范围： 1 到 255,这些时间能在 10 ms 与 650 s 之间，与波特率无关。监视器控制由位 WD_On 来打开或关闭
   uint8_t bMinTsdr;//响应方最小站延迟（min TSDR ）
   uint8_t bIdentHigh;
   uint8_t bIdentLow;
   uint8_t bGroupIdent;//每一位代表一个组。Group_Ident 只有在 Lock_Req 位被设置时才被接受。
   union {
	   uint8_t b;
	   struct {
		   uint8_t 	reserved0:1;//Bit 0: reserved
		   uint8_t 	reserved1:1;//Bit 1: reserved
		   uint8_t 	WDBase1ms:1;//Bit 2: WD_Base_1ms   This bit is set by the DP-Master if the watchdog control time   has to be calculated based on the following formula:
		                          // TWD in s = 1ms × WD_Fact_1 × WD_Fact_2    otherwise: 	   TWD in s = 10ms × WD_Fact_1 × WD_Fact_2
		   uint8_t  reserved3:1;// Bit 5 - Bit 3: reserved
		   uint8_t  reserved4:1;
		   uint8_t  Publisher_Enable:1;
		   uint8_t  failSafe:1;//Bit 6: Fail_Safe   This bit is set by the DPV1-Master (Class 1) to indicate that  this DP-Slave is operated in Fail_Safe mode. If the DP-Slave
		                         //requires the Fail_Safe mode and the master does not set this bit the DP-Slave may respond with a Diag.Prm_Fault.
		   uint8_t  DPV1Enable:1;//Bit 7: DPV1_Enable   This bit is set by the DPV1-Master (Class 1) to use the DPV1 functionality of a DPV1-Slave. This bit set to zero forces the
		                          //DPV1-Slave to work in a DP-Slave compatibility mode if possible.
	   }sbit;
   } bDpv1Status1;
   union {
	   uint8_t b;
	   struct {
          uint8_t checkCfgMode:1;//Bit 0: Check_Cfg_Mode By means of this bit the DP-Master is able to influence the  reaction to the service DDLM_Chk_Cfg. If this bit is zero the
		                          //check of the Cfg_Data is done as discribed in EN 50170. If the   bit is set the check of the Cfg_Data might be done in a
		                         //different user specific way. For example a temporary not plugged  module might be accepted even if the Cfg_Data contains the respective Cfg identifier.
	      uint8_t reserved1:1;//Bit 1: reserved;
	      uint8_t enableUpdateAlarm:1;//Bit 2: Enable_Update_Alarm This bit is set by the DPV1-Master (Class 1) to enable the transmission of the Alarm_Type Update_Alarm.
	      uint8_t enableStatusAlarm:1;//Bit 3: Enable_Status_Alarm This bit is set by the DPV1-Master (Class 1) to enable the transmission of the Alarm_Type Status_Alarm.
	      uint8_t enableManufacturerSpecificAlarm:1;//Bit 4: Enable_Manufacturer_Specific_Alarm This bit is set by the DPV1-Master (Class 1) to enable the transmission of all manufacturer specifc alarms.
	      uint8_t enableDiagnosticAlarm:1;//Bit 5: Enable_Diagnostic_Alarm This bit is set by the DPV1-Master (Class 1) to enable the transmission of the Alarm_Type Diagnostic_Alarm.
	      uint8_t enableProcessAlarm:1;//Bit 6: Enable_Process_Alarm This bit is set by the DPV1-Master (Class 1) to enable the transmission of the Alarm_Type Process_Alarm.
	      uint8_t enablePullPlugAlarm:1;//Bit 7: Enable_Pull_Plug_Alarm This bit is set by the DPV1-Master (Class 1) to enable the transmission of the Alarm_Type Pull_Plug_Alarm.
	   }sbit;
   } bDpv1Status2;
   union {
	   uint8_t b;
	   struct {
		   uint8_t Alarm_Mode:3;//Bits 0 to 2: Alarm_Mode  By meams of this parameter the DPV1-Master (Class 1) indicates its  own alarm capabilities to the DP-Slave.
		                        //The parameter limits the number of active alarms on the MSAC_C1 communication relationship. Range:
								//0 = 1 alarm of each type
								//1 = 2 alarms in total
								//2 = 4 alarms in total
								//3 = 8 alarms in total
								//4 = 12 alarms in total
								//5 = 16 alarms in total
								//6 = 24 alarms in total
								//7 = 32 alarms in total
		   uint8_t  Prm_Structure:1;//Bits 3
		   uint8_t  IsoM_Req:1;//Bits 4
		   uint8_t  resv:2;//Bits 5 -6
		   uint8_t  PrmCmd:1;//Bits 7
	   }sbit;
   } bDpv1Status3;

   uint8_t bUserPrmData[236];

} _PACKED_ dp_sPRM_t;
typedef struct
{
   union{
	   uint8_t b;
	   struct {
		   uint8_t resv:3;//
		   uint8_t WDOn:1;//(监视器接通)如果此位被设置为 0，那么监视器控制就被解除激活。如果此位被设置为 1，那么监视器控制就被激活。
		   uint8_t freezeReq:1;//一旦由 DDLM_Glogal_Control 功能传送了此控制命令，就用此位指示 DP 从站把
		   //在冻结模式之中操作。如果一个 DP 从站不支持冻结控制命令，它就在诊断信息之中设置
		   //Diag.Not_Supported 位。由于在参数化阶段中进行了检查，因此在用户数据交换模式中就避免了错误。
		   uint8_t syncReq:1;//一旦由 DDLM_Glogal_Control 功能传送了此控制命令，就用此位指示 DP 从站在同步模式之中操作。
		   //如果一个 DP 从站不支持同步控制命令，它就在诊断信息之中设置Diag.Not_Supported 位。由于在参数化阶段中进行了检查，因此在用户数据交换模式中就避免了错误。
		   uint8_t unlockReq:1;//
		   uint8_t LockReq:1;//
	   }sbit;
   }bStationState;
   uint8_t bWdFactor1;//范围： 1 到 255,TWD （以 s 为单位） = 10 ms * WD_Fact_1* WD_Fact_2
   uint8_t bWdFactor2;//范围： 1 到 255,这些时间能在 10 ms 与 650 s 之间，与波特率无关。监视器控制由位 WD_On 来打开或关闭
   uint8_t bMinTsdr;//响应方最小站延迟（min TSDR ）
   uint8_t bIdentHigh;
   uint8_t bIdentLow;
   uint8_t bGroupIdent;//每一位代表一个组。Group_Ident 只有在 Lock_Req 位被设置时才被接受。
   union {
	   uint8_t b;
	   struct {
		   uint8_t 	reserved0:1;//Bit 0: reserved
		   uint8_t 	reserved1:1;//Bit 1: reserved
		   uint8_t 	WDBase1ms:1;//Bit 2: WD_Base_1ms   This bit is set by the DP-Master if the watchdog control time   has to be calculated based on the following formula:
		                          // TWD in s = 1ms × WD_Fact_1 × WD_Fact_2    otherwise: 	   TWD in s = 10ms × WD_Fact_1 × WD_Fact_2
		   uint8_t  reserved3:1;// Bit 5 - Bit 3: reserved
		   uint8_t  reserved4:1;
		   uint8_t  reserved5:1;
		   uint8_t  failSafe:1;//Bit 6: Fail_Safe   This bit is set by the DPV1-Master (Class 1) to indicate that  this DP-Slave is operated in Fail_Safe mode. If the DP-Slave
		                         //requires the Fail_Safe mode and the master does not set this bit the DP-Slave may respond with a Diag.Prm_Fault.
		   uint8_t  DPV1Enable:1;//Bit 7: DPV1_Enable   This bit is set by the DPV1-Master (Class 1) to use the DPV1 functionality of a DPV1-Slave. This bit set to zero forces the
		                          //DPV1-Slave to work in a DP-Slave compatibility mode if possible.
	   }sbit;
   } bDpv1Status1;
   union {
	   uint8_t b;
	   struct {
          uint8_t checkCfgMode:1;//Bit 0: Check_Cfg_Mode By means of this bit the DP-Master is able to influence the  reaction to the service DDLM_Chk_Cfg. If this bit is zero the
		                          //check of the Cfg_Data is done as discribed in EN 50170. If the   bit is set the check of the Cfg_Data might be done in a
		                         //different user specific way. For example a temporary not plugged  module might be accepted even if the Cfg_Data contains the respective Cfg identifier.
	      uint8_t reserved1:1;//Bit 1: reserved;
	      uint8_t enableUpdateAlarm:1;//Bit 2: Enable_Update_Alarm This bit is set by the DPV1-Master (Class 1) to enable the transmission of the Alarm_Type Update_Alarm.
	      uint8_t enableStatusAlarm:1;//Bit 3: Enable_Status_Alarm This bit is set by the DPV1-Master (Class 1) to enable the transmission of the Alarm_Type Status_Alarm.
	      uint8_t enableManufacturerSpecificAlarm:1;//Bit 4: Enable_Manufacturer_Specific_Alarm This bit is set by the DPV1-Master (Class 1) to enable the transmission of all manufacturer specifc alarms.
	      uint8_t enableDiagnosticAlarm:1;//Bit 5: Enable_Diagnostic_Alarm This bit is set by the DPV1-Master (Class 1) to enable the transmission of the Alarm_Type Diagnostic_Alarm.
	      uint8_t enableProcessAlarm:1;//Bit 6: Enable_Process_Alarm This bit is set by the DPV1-Master (Class 1) to enable the transmission of the Alarm_Type Process_Alarm.
	      uint8_t enablePullPlugAlarm:1;//Bit 7: Enable_Pull_Plug_Alarm This bit is set by the DPV1-Master (Class 1) to enable the transmission of the Alarm_Type Pull_Plug_Alarm.
	   }sbit;
   } bDpv1Status2;
   union {
	   uint8_t b;
	   struct {
		   uint8_t Alarm_Mode:3;//Bits 0 to 2: Alarm_Mode  By meams of this parameter the DPV1-Master (Class 1) indicates its  own alarm capabilities to the DP-Slave.
		                        //The parameter limits the number of active alarms on the MSAC_C1 communication relationship. Range:
								//0 = 1 alarm of each type
								//1 = 2 alarms in total
								//2 = 4 alarms in total
								//3 = 8 alarms in total
								//4 = 12 alarms in total
								//5 = 16 alarms in total
								//6 = 24 alarms in total
								//7 = 32 alarms in total
		   uint8_t reserved:5;//Bits 3 to 7: reserved
	   }sbit;
   } bDpv1Status3;
   uint8_t bUserPrmData[1];

} _PACKED_ dp_sPRM_HEAD_t;
typedef struct
{
   uint8_t bLength;                  /*!< length of configuration data */
   dp_sPRM_t sData;
} _PACKED_ dp_PRM_t;


/*---------------------------------------------------------------------------*/
/*  DPV1 Status 1                                            */
/*---------------------------------------------------------------------------*/
#define DPV1_STATUS_1_RESERVED_1          ((uint8_t)0x03)
#define DPV1_STATUS_1_WD_BASE_1MS         ((uint8_t)0x04)
#define DPV1_STATUS_1_RESERVED_2          ((uint8_t)0x08)
#define DPV1_STATUS_1_RESERVED_3          ((uint8_t)0x10)
#define DPV1_STATUS_1_PUBLISHER_ENABLE    ((uint8_t)0x20)
#define DPV1_STATUS_1_FAIL_SAFE           ((uint8_t)0x40)
#define DPV1_STATUS_1_DPV1_ENABLE         ((uint8_t)0x80)

/*---------------------------------------------------------------------------*/
/*  DPV1 Status 2                                            */
/*---------------------------------------------------------------------------*/
#define DPV1_STATUS_2_CHECK_CFG_MODE      ((uint8_t)0x01)
#define CHECK_CFG                         ((uint8_t)0x00)
#define NO_CHECK_CFG                      ((uint8_t)0x01)
#define DPV1_STATUS_2_RESERVED_4          ((uint8_t)0x02)

#define ALARM_TYPE_UPDATE_VALUE           ((uint8_t)0x04)
#define ALARM_TYPE_STATUS_VALUE           ((uint8_t)0x08)
#define ALARM_TYPE_MANUFACTURER_VALUE     ((uint8_t)0x10)
#define ALARM_TYPE_DIAGNOSTIC_VALUE       ((uint8_t)0x20)
#define ALARM_TYPE_PROCESS_VALUE          ((uint8_t)0x40)
#define ALARM_TYPE_PULLPLUG_VALUE         ((uint8_t)0x80)

#define DPV1_STATUS_2_ALARM_TYPE_MASK     (   ALARM_TYPE_UPDATE_VALUE       \
                                            | ALARM_TYPE_STATUS_VALUE       \
                                            | ALARM_TYPE_MANUFACTURER_VALUE \
                                            | ALARM_TYPE_DIAGNOSTIC_VALUE   \
                                            | ALARM_TYPE_PROCESS_VALUE      \
                                            | ALARM_TYPE_PULLPLUG_VALUE     \
                                          )
#define ALARM_TYPE_NONE_VALUE             ((uint8_t)0x00)
#define ALARM_TYPE_ALL_VALUE              ((uint8_t)0xFF)

/*---------------------------------------------------------------------------*/
/* DPV1 Status 3                                            */
/*---------------------------------------------------------------------------*/
#define DPV1_STATUS_3_ALARM_MODE_MASK     ((uint8_t)0x07)
#define DPV1_STATUS_3_PRM_STRUCTURE       ((uint8_t)0x08)
#define DPV1_STATUS_3_ISOM_REQUEST        ((uint8_t)0x10)
#define DPV1_STATUS_3_RESERVED_UNUSED5    ((uint8_t)0x70)
#define DPV1_STATUS_3_PRM_CMD             ((uint8_t)0x80)

/*---------------------------------------------------------------------------*/
/* general defines for prm data                                          */
/*---------------------------------------------------------------------------*/
#define PRM_LEN_NORM                      ((uint8_t)0x07)
#define PRM_LEN_DPV1                      ((uint8_t)0x0A)

/*---------------------------------------------------------------------------*/
/* structured prm data                                      */
/*---------------------------------------------------------------------------*/
#define PBC_PRMCMD                        ((uint8_t)0x02)
#define PBC_DXB_LINKTABLE                 ((uint8_t)0x03)
#define PBC_ISOCHRON                      ((uint8_t)0x04)
#define PBC_DXB_SUBSCRIBERTABLE           ((uint8_t)0x07)
#define PBC_TIME_AR                       ((uint8_t)0x08)
#define PBC_USER_PRM_DATA                 ((uint8_t)0x81)

typedef struct
{
   uint8_t bLength;
   uint8_t bCommand;
   uint8_t bSlotNr;
   uint8_t bIndex;
   uint8_t bUserPrmData;
} _PACKED_ dp_sPRM_BLOCK_t;

typedef struct
{
   uint8_t bLength;
   uint8_t bCommand;
   uint8_t bSlotNr;
   uint8_t bIndex;
   uint8_t abCsInterval[2];
   uint8_t abCsDelayInterval[8];
}  dp_sPRM_BLOCK_CS_t;

typedef struct
{
   uint8_t  bLength;
   uint8_t  bCommand;
   uint8_t  bSlotNr;
   uint8_t  bSpecifier;
   uint8_t  bFunction;
   uint8_t  bProperties;
   uint16_t wOutputHoldTime;
} _PACKED_ dp_sPRM_BLOCK_PRMCMD_t;

#define cSizeOfRedPrmCmd                  ((uint8_t)0x08)

/*-----------------------------------------------------------------------------------------------------------*/
/* 3.0 configuration telegram                                                                                */
/*-----------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* 3.1 standard format of cfg byte                                           */
/*---------------------------------------------------------------------------*/
/*
          MSB                         LSB
         +---+---+---+---+---+---+---+---+
  Bit-No | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
         +---+---+---+---+---+---+---+---+
           |   |   |   |   |   |   |   |      meaning:
           |   |   |   |   |   |   |   |
           |   |   |   |   +---+---+---+----  length of data
           |   |   |   |                      00 1 byte resp. 1 word
           |   |   |   |                      .
           |   |   |   |                      .
           |   |   |   |                      15 16 byte resp. 16 words
           |   |   |   |
           |   |   |   |
           |   |   +---+--------------------  input/output
           |   |                              00 specific identifier formats
           |   |                              01 input
           |   |                              10 output
           |   |                              11 input-output
           |   |
           |   +----------------------------  length format 1)
           |                                   0 byte byte structure
           |                                   1 word word structure
           |
           +--------------------------------  consistency over
                                               0 byte or word
                                               1 whole length

*/

/*---------------------------------------------------------------------------*/
/* 3.2 special format of cfg byte                                            */
/*---------------------------------------------------------------------------*/
/*
          MSB                         LSB
         +---+---+---+---+---+---+---+---+
  Bit-No | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
         +---+---+---+---+---+---+---+---+
           |   |   |   |   |   |   |   |      meaning:
           |   |   |   |   |   |   |   |
           |   |   |   |   |   |   |   |
           |   |   |   |   +---+---+---+----- length of manufacturer
           |   |   |   |                      specific data 2)
           |   |   |   |
           |   |   +---+--------------------- fixed to 00
           |   |
           +---+----------------------------- input/output
                                              00 free place
                                              01 it follows 1 length byte for inputs
                                              10 it follows 1 length byte for outputs
                                              11 it follows - 1 length byte for outputs
                                                            - 1 length byte for inputs

   -----------------------------------------------------------------------------
   2) The length information of the manufacturer specific data shall be interpreted as follows:

   In case of Chk_Cfg:

     0             No manufacturer specific data follow; no data in Real_Cfg_Data.
     1 to 14       Manufacturer specific data of specified length follow;
                   these shall be identical with the data in Real_Cfg_Data.
     15            No manufacturer specific data follow;
                   the verification can be omitted

   In case of DDLM_Get_Cfg :

     0             No manufacturer specific data follow
     1 to 14       Manufacturer specific data with specified length follow
     15            Not allowed

   The structure of the length bytes is as follows:

          MSB                         LSB
         +---+---+---+---+---+---+---+---+
  Bit-No | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
         +---+---+---+---+---+---+---+---+
           |   |   |   |   |   |   |   |      meaning:
           |   |   |   |   |   |   |   |
           |   |   +---+---+---+---+---+----- length of inputs/outputs
           |   |                              00 1 byte resp. 1 word
           |   |                              .
           |   |                              .
           |   |                              63 64 byte resp. 64 words
           |   |
           |   +----------------------------- Length format 3)
           |                                   0 byte byte structure
           |                                   1 word word structure
           |
           +--------------------------------- consistency over
                                               0 byte or word
                                               1 whole length
*/

/*---------------------------------------------------------------------------*/
/* 3.3 literals for cfg-bytes                                                */
/*---------------------------------------------------------------------------*/
#define CFG_IS_BYTE_FORMAT           ((uint8_t)0x30)
#define CFG_BF_LENGTH                ((uint8_t)0x0f)
#define CFG_LENGTH_IS_WORD_FORMAT    ((uint8_t)0x40)
#define CFG_BF_INP_EXIST             ((uint8_t)0x10)
#define CFG_BF_OUTP_EXIST            ((uint8_t)0x20)
#define CFG_SF_OUTP_EXIST            ((uint8_t)0x80)
#define CFG_SF_INP_EXIST             ((uint8_t)0x40)
#define CFG_SF_LENGTH                ((uint8_t)0x3f)

/*---------------------------------------------------------------------------*/
/* 3.4 returncodes cfg data                                                  */
/*---------------------------------------------------------------------------*/
#define CFG_FINISHED                 ((uint8_t)0x00)
#define CFG_CONFLICT                 ((uint8_t)0x01)
#define CFG_NOT_ALLOWED              ((uint8_t)0x03)


/*---------------------------------------------------------------------------*/
/* 3.7 structure for real cfg data                                           */
/*---------------------------------------------------------------------------*/
/* -- cfg structure ----------------------------------------------------------- */
/*!
  \brief Structure for configuration data.
*/
typedef struct
{
   uint8_t bLength;                  /*!< length of configuration data */
   uint8_t abData[ 246 ];    /*!< array of configuration data  */
} _PACKED_ dp_CFG_t;



/*-----------------------------------------------------------------------------------------------------------*/
/* 4.0 input / output                                                                                        */
/*-----------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* 4.1 structure for calculated input-/output-length                         */
/*---------------------------------------------------------------------------*/
typedef struct
{
   uint8_t bInputDataLength;               /*  calculated input length  (VPC3->DP-Master) */
   uint8_t bOutputDataLength;              /*  calculated output length (DP-Master->VPC3) */
} _PACKED_  dp_IO_DATA_LEN_t;

typedef struct
{
   uint8_t bLength;                  /*!< length of configuration data */
   uint8_t abData[ 246 ];    /*!< array of input data,  to master */
} _PACKED_ dp_Input_t;

typedef struct
{
   uint8_t bLength;                  /*!< length of configuration data */
   uint8_t abData[ 246 ];    /*!< array of output data ,form master */
} _PACKED_ dp_Output_t;

/*-----------------------------------------------------------------------------------------------------------*/
/* 5.0 set slave address                                                                                     */
/*-----------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* 5.1 structure                                                             */
/*---------------------------------------------------------------------------*/
typedef struct
{
   uint8_t bTsAddr;
   uint8_t bIdentHigh;
   uint8_t bIdentLow;
   uint8_t bNoAddressChanged;
} _PACKED_ dp_SSA_BLOCK_t;

/*---------------------------------------------------------------------------*/
/* 6.2 macros for global control                                             */
/*---------------------------------------------------------------------------*/
/*  -- read Global-Control-Command ----------------------------------------- */

   /*!
      \brief Fetch the global control command.

      \retval Bit 0 - reserved
      \retval Bit 1 - Clear_Data - With this command, the output data is cleared and made available to the user.
      \retval Bit 2 - Unfreeze - With "Unfreeze", freezing the input data is cancelled.
      \retval Bit 3 - Freeze - The input data is "frozen". The application will fetch new input data only if the master sends the next "freeze" command.
      \retval Bit 4 - Usync - The command "Unsync" cancels the "Sync"command.
      \retval Bit 5 - Sync - The output data received last is made availabe to the application. The output
                        data transmitted subsequently is passed to the application only after the next
                        "Sync" command is issued.
      \retval Bit 6 - reserved
      \retval Bit 7 - reserved
   */
typedef struct
{
   union{
	   uint8_t b;
	   struct {
		   uint8_t resv:1;//
		   uint8_t clearData:1;//所有的输出被清除。
		   uint8_t unfreeze:1;//冻结输入将被撤消。
		   uint8_t freeze:1;//读出并冻结输入的状态。此后的冻结控制命令重复这过程。支持冻结模式的 DP 从
		   //站必须保证：在冻结控制命令之后已经在下一个数据交换循环中，上次冻结的输入值必须被传送。
		   uint8_t unsync:1;//非同步控制命令撤消了同步命令。
		   uint8_t sync:1;//给出并且冻结使用功能 DDLM_Data_Exchange 传送的输出状态。只有在接收到下一个同步控制命令时，才给出所跟随的输出数据。
	       uint8_t resv2:2;
	   }sbit;
   }bControlCommand;
   uint8_t bGroupSelect;//此参数决定哪一个（些）组应被寻址。如果在 Group_Ident（在参数数据中被传递）与 Group_Select 之间的 AND 操作产生一个不为零的值，那么，控制命令就有效。如果参
   //数 Group_Select 为零，那么，所有的 DP-从站被寻址。
} _PACKED_ dp_sGloaCtrl_t;



/*-----------------------------------------------------------------------------------------------------------*/
/* 7.0 diagnostic telegram                                                                                   */
/*-----------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* 7.1 defines                                                               */
/*---------------------------------------------------------------------------*/
#define EXT_DIAG_RESET                    ((uint8_t)0x00)
#define DIAG_RESET                        ((uint8_t)0x00)
#define EXT_DIAG_SET                      ((uint8_t)0x01)
#define STAT_DIAG_RESET                   ((uint8_t)0x00)
#define STAT_DIAG_SET                     ((uint8_t)0x02)
#define EXT_STAT_DIAG_SET                 ((uint8_t)0x03)
#define OVF_DIAG_SET                      ((uint8_t)0x04)
#define DIAG_WAIT_FOR_ALARM               ((uint8_t)0x80)

#define DIAG_REV_SIGN                     ((uint8_t)0xC0)
#define DIAG_KEN_SIGN                     ((uint8_t)0x40)
#define DIAG_CHN_SIGN                     ((uint8_t)0x80)
#define DIAG_DEV_SIGN                     ((uint8_t)0x00)

#define DIAG_TYPE_MASK                    ((uint8_t)0xC0)
#define DIAG_TYPE_DEV                     ((uint8_t)0x00)
#define DIAG_TYPE_KEN                     ((uint8_t)0x40)
#define DIAG_TYPE_CHN                     ((uint8_t)0x80)
#define DIAG_TYPE_REV                     ((uint8_t)0xC0)

#define DIAG_NORM_DIAG_SIZE               ((uint8_t)0x06)
#define DIAG_TYPE_CHN_SIZE                ((uint8_t)0x03)
#define DIAG_TYPE_REV_SIZE                ((uint8_t)0x01)

#define STATUS_DIAG_HEAD_SIZE             ((uint8_t)0x04)
#define STATUS_TYPE_SIGN                  ((uint8_t)0x80)
#define STATUS_TYPE_STATUS_MESSAGE        ((uint8_t)0x01)
#define STATUS_TYPE_MODUL_STATUS          ((uint8_t)0x02)
#define STATUS_TYPE_PRM_COMMAND_ACK       ((uint8_t)0x1E)
#define STATUS_TYPE_H_STATUS_MESSAGE      ((uint8_t)0x1F)
#define STATUS_TYPE_MANU_MIN              ((uint8_t)0xA0)
#define STATUS_TYPE_MANU_MAX              ((uint8_t)0xFE)

#define ALARM_TYPE_SIGN                   ((uint8_t)0x00)
#define ALARM_DIAG_HEAD_SIZE              ((uint8_t)0x04)

/*---------------------------------------------------------------------------*/
/* 7.2 structures                                                            */
/*---------------------------------------------------------------------------*/
/* -- revision diagnosis data ---------------------------------------------- */
typedef struct
{
   uint8_t   bRevision;
} _PACKED_ dp_REV_DIAG_t;

/* -- slot signed diagnosis data ------------------------------------------- */
typedef struct
{
   uint8_t   bHeader;
   uint8_t   abIdentifierDiagnosis[1];
} _PACKED_ dp_KEN_DIAG_t;

/* defines for channel related diagnostic */
/*
          MSB                         LSB
         +---+---+---+---+---+---+---+---+
  Bit-No | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
         +---+---+---+---+---+---+---+---+
           |   |   |   |   |   |   |   |    meaning:
           |   |   |   |   |   |   |   |
           |   |   |   +---+---+---+---+--- error type
           |   |   |
           +---+---+----------------------- channel type
*/

/* channel type of statuscode */
#define STATUS_UNSPECIFIC                 ((uint8_t)0x00)
#define STATUS_1BIT                       ((uint8_t)0x20)
#define STATUS_2BIT                       ((uint8_t)0x40)
#define STATUS_4BIT                       ((uint8_t)0x60)
#define STATUS_BYTE                       ((uint8_t)0x80)
#define STATUS_WORD                       ((uint8_t)0xA0)
#define STATUS_DWORD                      ((uint8_t)0xC0)

/* error type of statuscode (PROFIBUS SPECIFICATION) */
#define STATUS_SHORT_CIRCUIT              ((uint8_t)0x01)
#define STATUS_UNDERVOLTAGE               ((uint8_t)0x02)
#define STATUS_OVERVOLTAGE                ((uint8_t)0x03)
#define STATUS_OVERLOAD                   ((uint8_t)0x04)
#define STATUS_OVERTEMPERATURE            ((uint8_t)0x05)
#define STATUS_LINE_BREAK                 ((uint8_t)0x06)
#define STATUS_UPPER_LIMIT_VALUE          ((uint8_t)0x07)
#define STATUS_LOWER_LIMIT_VALUE          ((uint8_t)0x08)
#define STATUS_ERROR                      ((uint8_t)0x09)
#define STATUS_RESERVED_10                ((uint8_t)0x0A)
#define STATUS_RESERVED_11                ((uint8_t)0x0B)
#define STATUS_RESERVED_12                ((uint8_t)0x0C)
#define STATUS_RESERVED_13                ((uint8_t)0x0D)
#define STATUS_RESERVED_14                ((uint8_t)0x0E)
#define STATUS_RESERVED_15                ((uint8_t)0x0F)

/* manufacturer specific between 16 and 31 */
/* Siemens Error Codes */
#define STATUS_PRM_ERROR                  ((uint8_t)0x10)
#define STATUS_VOLTAGE                    ((uint8_t)0x11)
#define STATUS_FUSE_ERROR                 ((uint8_t)0x12)
#define STATUS_GND_ERROR                  ((uint8_t)0x13)
#define STATUS_REF_ERROR                  ((uint8_t)0x14)
#define STATUS_PROC_ALARM_LOST            ((uint8_t)0x16)
#define STATUS_UNKNOWN_ERROR              ((uint8_t)0x1B)

/* defines for channel number in/out */
#define CH_NUMBER_INOUT                   ((uint8_t)0xC0)
#define CH_NUMBER_OUT                     ((uint8_t)0x80)
#define CH_NUMBER_IN                      ((uint8_t)0x40)
#define CH_RESERVED                       ((uint8_t)0x00)

/* -- channel diagnosis data -------------------------------------------------- */
typedef struct
{
   uint8_t   bHeader;
   uint8_t   bChannel;
   uint8_t   bTypeOfDiagnosis;
} _PACKED_ dp_CHN_DIAG_t;

/* -- device diagnosis data --------------------------------------------------- */
typedef struct
{
   uint8_t   bHeader;
   uint8_t   abDeviceRelatedDiagnosis[1];
} _PACKED_ dp_DEV_DIAG_t;

/* -- status message data (coded as device diagnosis (see above) -------------- */
typedef struct
{
   uint8_t   bHeader;           /*  equal to DIAG_DEV_SIGN */
   uint8_t   bAlarmType;
   uint8_t   bSlotNr;
   uint8_t   bSpecifier;
   uint8_t   abAlarmDataDescription[1];
} _PACKED_ dp_STATUS_DIAG_t;

/** @brief possible values for the diagnostic type identifier related diagnosis */
typedef enum {
   DIAG_IR_OK = 0          /**< Module ok, data valid */
   ,DIAG_IR_FAULT          /**< Module not ok, data invalid due to fault (e.g. wire break) */
} E_DP_DIAG_IR;

/** @brief possible values for the diagnostic type module status */
typedef enum {
   DIAG_MS_OK = 0          /**< Module ok, data valid */
   ,DIAG_MS_FAULT          /**< Module correct, data invalid due to fault (e.g. wire break) */
   ,DIAG_MS_WRONG          /**< Wrong module in use, data invalid */
   ,DIAG_MS_NO_MODULE      /**< No module in use, data invalid */
} E_DP_DIAG_MS;

/* alarm data (coded as device diagnosis (see above) -------------------------- */
typedef struct
{
   uint8_t   bHeader;           /* equal to DIAG_DEV_SIGN */
   uint8_t   bAlarmType;
   uint8_t   bSlotNr;
   uint8_t   bSpecifier;
   uint8_t   abAlarmData[1];
} _PACKED_ dp_ALARM_DIAG_t;


typedef struct
{	   union{
		   uint8_t b;
		   struct{
			   uint8_t stationNonExistent:1;//如果相应的DP从站不能通过总线达到，此位就由该DP主站所设置。如果此位被设置，则诊断位包含上一个诊断报文的状态或其初始值。DP 从站设置此位为 0。
			   uint8_t stationNotReady:1;//如果 DP 从站为数据传递还未作好准备，则此位由此 DP 从站设置。
			   uint8_t cfgFault:1;//当从 DP 主站收到的上一个组态数据与 DP 从站已确定的组态数据不同时，则此位就由该 DP从站设置
			   uint8_t extDiag:1;//此位由该 DP 从站设置。它指示：如果该位被设置为 1，那么在从站专用的诊断区域
			   //中（Ex_Diag_Data）有一个诊断登入项；如果该位被设置为 0，则在从站专用的诊断区域内（Ext_Diag_Data）可能存在一个状态报文。该状态报文的意义取决于应用，它在本规范中未作规定。
			   uint8_t notSupported:1;//当请求一个并非由此 DP 从站所支持的功能时，此位由该 DP 从站设置。
			   uint8_t invaldSlaveResponse:1;//当从一个被寻址的 DP 从站处接到一个不那么合理的响应时，此位就由 DP 主站设置。DP 从站设置此位为 0
			   uint8_t prmFault:1;//如果上一个参数帧是错误的，例如，错误的长度、错误的 Ident_Number、失效的参数，那么，此位由 DP 从站设置。
			   uint8_t masterLock:1;//DP 从站已被另一个主站参数化了。如果在八位位组 4 中的地址不同于 255 也不同于自己的地址，那么，此位被 DP 主站（1 类）所设置。DP 从站设置此位为 0。
		   }_PACKED_ sbit;
	   }bStationStatus1;
	   union{
		   uint8_t b;
		   struct {
			   uint8_t prmReq:1;//如果 DP 从站设置此位，那么相应的 DP 从站应被重新参数化和重新组态。该位保留设置直到参数化结束。该位由 DP 从站设置。如果设置了位 1 和位 0，则位 0 具有较高的优先权。
			   uint8_t statDiag:1;//静态诊断,如果 DP 从站设置此位，那么 DP 主站将获取诊断信息直到该位复位为止。例如，如果它不能提供有效的用户数据的话，DP 从站设置此位。
			   uint8_t slaveSet:1;//此位由 DP 从站设置为 1
			   uint8_t WDOn:1;//(监视器(watchdog)接通),当其监视器控制已被激活后，此位由 DP 从站设置。
			   uint8_t freezeMode:1;//当相应的 DP 从站收到了冻结控制命令后，此位由 DP 从站设置。
			   uint8_t syncMode:1;//当相应的 DP 从站已收到了同步控制命令后，此位由 DP 从站设置。
			   uint8_t rsv:1;//
			   uint8_t deactivated:1;//当 DP 从站在 DP 从站参数集中已被标记为停用并已从循环处理中去掉时，则此位由DP 主站设置。从站总是设置此位为 0。
		   }_PACKED_ sbit;
	   }bStationStatus2;
	   union{
		   uint8_t b;
		   struct{
			   uint8_t resv:7;//
			   uint8_t extDiagOverflow:1;//如果此位被设置，那么在 Ext_Diag_Data 中有比指定的信息更多的诊断信息。例如，
			   //如果有比 DP 从站登入在其发送缓存器中更多的通道诊断，则 DP 从站设置此位；或者，
			   //如果 DP 从站发送比 DP 主站登入在其诊断缓存器中有更多的诊断信息，则 DP 主站设置此位。
		   }sbit;
	   }bStationStatus3;
	   uint8_t bMasterAdd;//在此八位位组中登入已参数化此 DP 从站的 DP 主站的地址。如果无 DP 主站已参数化此 DP 从站，则 DP 从站将地址 255 登入此八位位组
	   uint8_t bIdentHigh;//给出 DP 设备的制造商的标识符。此标识符一方面能用于认证的目的，另一方面用于确切地识别。
	   uint8_t bIdentLow;
	   uint8_t bExtData[240];

}_PACKED_ dp_sDIAG_t;
typedef struct
{
   uint8_t bLength;                  /*!< length of Diag data */
   dp_sDIAG_t sData;
} _PACKED_ dp_DIAG_t;

/*-----------------------------------------------------------------------------------------------------------*/
/* 10.0 fdl layer                                                                                            */
/*-----------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* 10.1 SAPs (service access points) for MSAC1                               */
/*---------------------------------------------------------------------------*/
#define DP_C2_RM_SAP_NR                   ((uint8_t)0x31)
#define DP_C1_ALARM_SAP_NR                ((uint8_t)0x32)
#define DP_C1_RD_WR_SAP_NR                ((uint8_t)0x33)

/*---------------------------------------------------------------------------*/
/* 10.2 SAPs (service access points) for MSAC2                               */
/*---------------------------------------------------------------------------*/
#define MSAC_C2_MAX_INPUT_ITEMS           ((uint8_t)((2*DP_C2_NUM_SAPS)+1)) /* number of elements in the input queue */
#define MSAC_C2_SAP_NR_LOW                ((uint8_t)0x00)                   /* lowest possible SAP for MSAC_C2 */
#define MSAC_C2_SAP_NR_HIGH               ((uint8_t)0x30)                   /* highest possible SAP for MSAC_C2 (dp_mode =1) */


/*-----------------------------------------------------------------------------------------------------------*/
/* 11.0 DPV1                                                                                                 */
/*-----------------------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* 11.2 errorcodes of DPV1 for ERROR_DECODE                                   */
/*----------------------------------------------------------------------------*/
#define DPV1_ERRDC_DPV1                   ((uint8_t)0x80) /* Error_decode, DPV1 fixed value */
#define DPV1_ERRDC_FMS                    ((uint8_t)0xFE) /* Error_decode, FMS  fixed value */
#define DPV1_ERRDC_HART                   ((uint8_t)0xFF) /* Error_decode, HART fixed value */

/*----------------------------------------------------------------------------*/
/* 11.3 errorcodes of DPV1 for ERROR_CODE_1                                   */
/*----------------------------------------------------------------------------*/
/* error_class */
#define DPV1_ERRCL_APPLICATION            ((uint8_t)0xA0)
#define DPV1_ERRCL_ACCESS                 ((uint8_t)0xB0)
#define DPV1_ERRCL_RESSOURCE              ((uint8_t)0xC0)
#define DPV1_ERRCL_USER                   ((uint8_t)0xD0)
/* error_code for  DPV1_ERRCL_APPLICATION */
#define DPV1_ERRCL_APP_READ               ((uint8_t)0x00)
#define DPV1_ERRCL_APP_WRITE              ((uint8_t)0x01)
#define DPV1_ERRCL_APP_MODULE             ((uint8_t)0x02)
#define DPV1_ERRCL_APP_VERSION            ((uint8_t)0x08)
#define DPV1_ERRCL_APP_NOTSUPP            ((uint8_t)0x09)
#define DPV1_ERRCL_APP_USER               ((uint8_t)0x0A)
/* error_code for  DPV1_ERRCL_ACCESS */
#define DPV1_ERRCL_ACC_INV_INDEX          ((uint8_t)0x00)
#define DPV1_ERRCL_ACC_WRITE_LEN          ((uint8_t)0x01)
#define DPV1_ERRCL_ACC_INV_SLOT           ((uint8_t)0x02)
#define DPV1_ERRCL_ACC_TYPE               ((uint8_t)0x03)
#define DPV1_ERRCL_ACC_INV_AREA           ((uint8_t)0x04)
#define DPV1_ERRCL_ACC_STATE              ((uint8_t)0x05)
#define DPV1_ERRCL_ACC_ACCESS             ((uint8_t)0x06)
#define DPV1_ERRCL_ACC_INV_RANGE          ((uint8_t)0x07)
#define DPV1_ERRCL_ACC_INV_PARAM          ((uint8_t)0x08)
#define DPV1_ERRCL_ACC_INV_TYPE           ((uint8_t)0x09)
#define DPV1_ERRCL_ACC_USER               ((uint8_t)0x0A)
#define DPV1_ERRCL_ACC_IM_BACKUP          ((uint8_t)0x0A)
/* error_code for  DPV1_ERRCL_RESOURCE */
#define DPV1_ERRCL_RES_READ_CONSTRAIN     ((uint8_t)0x00)
#define DPV1_ERRCL_RES_WRITE_CONSTRAIN    ((uint8_t)0x01)
#define DPV1_ERRCL_RES_BUSY               ((uint8_t)0x02)
#define DPV1_ERRCL_RES_UNAVAIL            ((uint8_t)0x03)
#define DPV1_ERRCL_RES_USER               ((uint8_t)0x08)
/* error_code for  msac_c1_transmit_delay */
#define DPV1_NO_VALID_SAP                 ((uint8_t)0x01)
#define DPV1_ERR_INV_PARAM                ((uint8_t)0x02)
#define DPV1_ERR_NO_JOB                   ((uint8_t)0x03)

/*----------------------------------------------------------------------------*/
/* 11.4 coding of abort                                                       */
/*----------------------------------------------------------------------------*/
/* subnet */
#define MSAC_C2_SUBNET_NO                 ((uint8_t)0x00)
#define MSAC_C2_SUBNET_LOCAL              ((uint8_t)0x01)
#define MSAC_C2_SUBNET_REMOTE             ((uint8_t)0x02)
/* instance */
#define MSAC_C2_INSTANCE_FDL              ((uint8_t)0x00)
#define MSAC_C2_INSTANCE_MSAC_C2          ((uint8_t)0x10)
#define MSAC_C2_INSTANCE_USER             ((uint8_t)0x20)
#define MSAC_C2_INSTANCE_RESERVED         ((uint8_t)0x30)
/* reason */
#define MSAC_C2_ABT_SE                    ((uint8_t)0x01)   /* sequence error */
#define MSAC_C2_ABT_FE                    ((uint8_t)0x02)   /* invalid request PDU received  */
#define MSAC_C2_ABT_TO                    ((uint8_t)0x03)   /* timeout of the connection */
#define MSAC_C2_ABT_RE                    ((uint8_t)0x04)   /* invalid response PDU received */
#define MSAC_C2_ABT_IV                    ((uint8_t)0x05)   /* invalid service from USER */
#define MSAC_C2_ABT_STO                   ((uint8_t)0x06)   /* Send_Timeout requested was too small */
#define MSAC_C2_ABT_IA                    ((uint8_t)0x07)   /* invalid additional address information */
#define MSAC_C2_ABT_OC                    ((uint8_t)0x08)   /* waiting for FDL_DATA_REPLY.con */
#define MSAC_C2_ABT_RES                   ((uint8_t)0x0F)   /* resource error */

/*----------------------------------------------------------------------------*/
/* 11.5 Function Codes for DPV1                                               */
/*----------------------------------------------------------------------------*/
#define DPV1_FC_ERROR                     ((uint8_t)0x80)
#define DPV1_FC_INITIATE                  ((uint8_t)0x57)
#define DPV1_FC_ABORT                     ((uint8_t)0x58)
#define DPV1_FC_READ                      ((uint8_t)0x5E)
#define DPV1_FC_WRITE                     ((uint8_t)0x5F)
#define DPV1_FC_ALARM_ACK                 ((uint8_t)0x5C)
#define DPV1_FC_IDLE                      ((uint8_t)0x48)
#define DPV1_FC_TRANSPORT                 ((uint8_t)0x51)
#define DPV1_FC_RM                        ((uint8_t)0x56)
/* mask for function code */
#define MSAC_C2_FN_MASK                   ((uint8_t)0x1F)
#define MSAC_C2_FN_CONNECT                ((uint8_t)0x17)
#define MSAC_C2_FN_DATA                   ((uint8_t)0x11)
#define MSAC_C2_FN_DISCONNECT             ((uint8_t)0x18)
#define MSAC_C2_FN_IDLE                   ((uint8_t)0x08)
#define MSAC_C2_FN_DS_READ                ((uint8_t)0x1E)
#define MSAC_C2_FN_DS_WRITE               ((uint8_t)0x1F)

/*----------------------------------------------------------------------------*/
/* 11.6 general defines for DPV1                                              */
/*----------------------------------------------------------------------------*/
#define MSAC_C2_TIMEOUT_12MBAUD           ((uint16_t)0x0064)
#define MSAC_C2_TIMEOUT_6MBAUD            ((uint16_t)0x0064)
#define MSAC_C2_TIMEOUT_3MBAUD            ((uint16_t)0x0064)
#define MSAC_C2_TIMEOUT_1_5MBAUD          ((uint16_t)0x0064)
#define MSAC_C2_TIMEOUT_500KBAUD          ((uint16_t)0x00C8)
#define MSAC_C2_TIMEOUT_187_50KBAUD       ((uint16_t)0x0190)

#define MSAC_C2_MIN_POLL_TIMEOUT          ((uint16_t)0x0001)
#define MSAC_C2_MAX_POLL_TIMEOUT          ((uint16_t)0x7FFF)

#define MSAC_C2_MIN_PDU_LEN               ((uint8_t)0x04)
#define MSAC_C2_LEN_DISCONNECT_PDU        ((uint8_t)0x04)
#define MSAC_C2_LEN_IDLE_PDU              ((uint8_t)0x02)
#define MSAC_C2_LEN_RESSOURCE_PDU         ((uint8_t)0x04)
#define MSAC_C2_MIN_CC_PDU_SIZE           ((uint8_t)0x10)
#define MSAC_C2_MAX_PDU_SIZE              ((uint8_t)0xF0)
#define MSAC_C2_MIN_CR_PDU_SIZE           ((uint8_t)0x14)

#define MSAC_C2_MAX_PDU                   ((uint8_t)0xF4)   /* PDU with DPV1-Header (4Byte) + 240 User Data */
#define MSAC_C2_LEN_INITIATE_RES_HEAD     ((uint8_t)0x0C)   /* Initiate response, no addr_data[] */
#define DPV1_LEN_NEG_RESPONSE             ((uint8_t)0x04)   /* Negative Response PDU */
#define DPV1_LEN_HEAD_DATA                ((uint8_t)0x04)   /* Fct,Slot,Index,Len */

#define MSAC_C2_FUNCTION_NUM_EXTENSION    ((uint8_t)0x40)
#define MSAC_C2_ERROR_FRAME               ((uint8_t)0xC0)
#define DPV1_ERROR_BIT_RESPONSE           ((uint8_t)0x80)

#define MSAC_C2_IND_RECV                  ((uint8_t)0x01)   /* indication receive */
#define MSAC_C2_RESP_SENT                 ((uint8_t)0x02)   /* response sent */
#define MSAC_C2_IND_DISABLE_DONE          ((uint8_t)0x10)   /* ind_disable_done */


/*----------------------------------------------------------------------------*/
/* 11.7 structures for DPV1                                                   */
/*----------------------------------------------------------------------------*/
typedef struct
{
   uint8_t function_num;
   uint8_t slot_num;
   uint8_t index;
   uint8_t length;
   uint8_t pdu_data[1];
} _PACKED_ DPV1_PDU_t;



typedef struct
{
   uint8_t function_num;
   uint8_t err_decode;
   uint8_t err_code1;
   uint8_t err_code2;
} _PACKED_ DPV1_NEG_RES_PDU_t; //c1 and c2. for read,write,transport,alarm ack,


typedef struct
{
   uint8_t function_num;
   uint8_t slot_num;
   uint8_t index;
   uint8_t length;
   uint8_t pdu_data[1];
} _PACKED_ DPV1_READ_PDU_t;//req and res(include data)

typedef DPV1_NEG_RES_PDU_t DPV1_READ_NRS_PDU_t;

typedef struct
{
   uint8_t function_num;
   uint8_t slot_num;
   uint8_t index;
   uint8_t length;
   uint8_t pdu_data[1];
} _PACKED_ DPV1_WRITE_PDU_t; //req(include data) and res

typedef DPV1_NEG_RES_PDU_t DPV1_WRITE_NRS_PDU_t;



typedef struct
{
	   uint8_t bFunction_num;
	   uint8_t bSlotNr;
	   union{
		   uint8_t b;
		   struct {
			   uint8_t type:7;//Alarm_Type:
								//0 = reserved
								//1 = Diagnostic_Alarm
								//2 = Process_Alarm
								//3 = Pull_Alarm
								//4 = Plug_Alarm
								//5 = Status_Alarm
								//6 = Update_Alarm
								//7 - 31 = reserved
								//32 - 126 = manufacturer-specific
								//127 reserved
							//Status_Type:
							//0 reserved
							//1 Status_Message
							//2 Modul_Status
							//3 to 31 reserved
							//32 to 126 manufacturer-specific
							//127 reserved
			   uint8_t statusFlag:1;//Bit 7 fixed to 0 (Alarm) fix to 1(Status)
		   }sbit;
	   }bAlarmType;
	   union{
		   uint8_t b;
		   struct {
			   uint8_t Alarm_Specifier:2;
									//00 no further differentiation
									//01 Error appears and Slot disturbed the slot genrates an alarm due to an error
									//10 Error disappears and Slot is okay the slot generates an alarm and indicates that the slot has no further errors
									//11 Error disappears and Slot is still disturbed the slot generates an alarm and indicates that the slot has still further errors
	           uint8_t Add_Ack:1;// When setting this bit the slave indicates to the DPV1-Master (Class 1) that this alarm requires in addition to the
			   	   	   	   	   //MSAC1_Alarm_Ack a separate user acknowledgement. This can be done for instance by means of a Write service.
	           uint8_t Seq_Nr:5;// By means of the Seq_Nr an unique identification of an alarm message is accomplished.
		   }sbit;
	   }bSpecifier;
} _PACKED_ MSAC_C1_ALARM_ACK_PDU_t;//req and res

typedef DPV1_NEG_RES_PDU_t MSAC_C1_ALARM_ACK_NRS_PDU_t;

typedef struct
{
   uint8_t bFunction_num;
} _PACKED_ MSAC_C2_Idel_PDU_t;//req and res

typedef struct
{
   uint8_t  bFunction_num;
   uint8_t  slot_num;
   uint8_t  index;
   uint8_t  length;
   uint8_t  pdu_data[1];
} _PACKED_ MSAC_C2_TRANSPORT_PDU_t; //req and res

typedef DPV1_NEG_RES_PDU_t MSAC_C2_TRANSPORT_NRES_PDU_t;

typedef struct
{
   uint8_t  bFunction_num;
   uint8_t  bServerSAP;
   uint16_t wSendTimeOut;
} _PACKED_ MSAC_C2_RM_REQ_PDU_t;


typedef struct
{
   uint8_t  bFunction_num;
   uint8_t  bReserved1;
   uint8_t  bReserved2;
   uint8_t  bReserved3;
   uint16_t wSendTimeOut;
   uint8_t  bFeaturesSupported1;
   uint8_t  bFeaturesSupported2;
   uint8_t  bProfileFeaturesSupported1;
   uint8_t  bProfileFeaturesSupported2;
   uint16_t wProfileIdentNumber;
   uint8_t  bS_Type;
   uint8_t  bS_Len;
   uint8_t  bD_Type;
   uint8_t  bD_Len;
   uint8_t  abAddrData[ 244 - 16 ];  /* max length from DpCfg.h */
} _PACKED_ MSAC_C2_INITIATE_REQ_PDU_t;

typedef struct
{
   uint8_t  bFunction_num;
   uint8_t  bMaxLenDataUnit;
   uint8_t  bFeaturesSupported1;
   uint8_t  bFeaturesSupported2;
   uint8_t  bProfileFeaturesSupported1;
   uint8_t  bProfileFeaturesSupported2;
   uint16_t wProfileIdentNumber;
   uint8_t  bS_Type;
   uint8_t  bS_Len;
   uint8_t  bD_Type;
   uint8_t  bD_Len;
   uint8_t  abAddrData[ 244 - 12 ];  /* max length from DpCfg.h */
} _PACKED_ MSAC_C2_INITIATE_RES_PDU_t;

typedef DPV1_NEG_RES_PDU_t MSAC_C2_INITIATE_NRES_PDU_t;




typedef struct
{
   uint8_t  function_num;
   uint8_t  subnet;
   uint8_t  instance_reason;
   uint8_t  reserved; /* MSAC_C2 + 1 Byte, no additional Detail */
} _PACKED_ MSAC_C2_ABORT_PDU_t;

typedef struct
{
   uint8_t  bApi;
   uint8_t  bScl;
   uint8_t  abNetAddr[6];
   uint8_t  abMacAddr[1]; /* len is defined by S_len or D_len !! */
} _PACKED_ MSAC_C2_INITIATE_SUB_PARAM_t;



/*----------------------------------------------------------------------------*/
/* 11.8 defines for ALARM                                                     */
/*----------------------------------------------------------------------------*/
/* max length of alarm-fifo */
#define ALARM_MAX_FIFO                    ((uint8_t)0x20)
/* defines for Status-Types */
#define STATUS_TYPE                       ((uint8_t)0x80)
#define STATUS_MESSAGE                    ((uint8_t)0x81)
#define MODUL_STATUS                      ((uint8_t)0x82)
#define STATUS_PRM_COMMAND_ACK            ((uint8_t)0x9E)
#define STATUS_RED_STATE                  ((uint8_t)0x9F)

#define SET_ALARM_AL_STATE_CLOSED         ((uint8_t)0x01)
#define SET_ALARM_SEQ_NR_ERROR            ((uint8_t)0x02)
#define SET_ALARM_SPECIFIER_ERROR         ((uint8_t)0x03)
#define SET_ALARM_LIMIT_EXPIRED           ((uint8_t)0x04)
#define SET_ALARM_PENDING                 ((uint8_t)0x05)
#define SET_ALARM_ALARMTYPE_NOTSUPP       ((uint8_t)0x06)
#define SET_ALARM_NO_BUFFER               ((uint8_t)0x07)
#define SET_ALARM_OK                      ((uint8_t)0x00)

#define SET_STATUS_NOT_ENABLED            ((uint8_t)0x01)
#define SET_STATUS_OK                     ((uint8_t)0x00)

/* defines for alarm-type */
#define ALARM_TYPE_MAX                    ((uint8_t)0x07)

#define ALARM_TYPE_DIAGNOSTIC             ((uint8_t)0x01)
#define ALARM_TYPE_PROCESS                ((uint8_t)0x02)
#define ALARM_TYPE_PULL                   ((uint8_t)0x03)
#define ALARM_TYPE_PLUG                   ((uint8_t)0x04)
#define ALARM_TYPE_STATUS                 ((uint8_t)0x05)
#define ALARM_TYPE_UPDATE                 ((uint8_t)0x06)
#define ALARM_TYPE_MANU_MIN               ((uint8_t)0x20)
#define ALARM_TYPE_MANU_MAX               ((uint8_t)0x7E)

/* defines for sequence_mode */
#define SEQC_MODE_TOTAL_00                ((uint8_t)0x00)
#define SEQC_MODE_OFF                     ((uint8_t)0x01)
#define SEQC_MODE_TOTAL_02                ((uint8_t)0x02)
#define SEQC_MODE_TOTAL_04                ((uint8_t)0x04)
#define SEQC_MODE_TOTAL_08                ((uint8_t)0x08)
#define SEQC_MODE_TOTAL_12                ((uint8_t)0x0C)
#define SEQC_MODE_TOTAL_16                ((uint8_t)0x10)
#define SEQC_MODE_TOTAL_24                ((uint8_t)0x18)
#define SEQC_MODE_TOTAL_32                ((uint8_t)0x20)

/* defines for specifier */
#define SPEC_GENERAL                      ((uint8_t)0x00)
#define SPEC_APPEARS                      ((uint8_t)0x01)
#define SPEC_DISAPPEARS                   ((uint8_t)0x02)
#define SPEC_DISAPP_SLOT_NOT_OK           ((uint8_t)0x03)

#define SPEC_MASK                         ((uint8_t)0x03)
#define SPEC_SEQ_START                    ((uint8_t)0x03)   /* use it to shift the sequence number down to bit 0 or up to bit 3 */
#define MAX_SEQ_NR                        ((uint8_t)0x20)
#define SEQUENCE_NUMBER_ALL               ((uint8_t)0xFF)
#define SEQ_NR_MASK                       ((uint8_t)0xF8)

#define SPEC_ADD_ACK                      ((uint8_t)0x04)

#define cSizeOfAlarmHeader                ((uint8_t)0x04)


typedef struct
{
   uint8_t bAlarmType;
   uint8_t bSlotNr;
   uint8_t bSeqNr;
   uint8_t bSpecifier;
   uint8_t bAddAck;
   uint8_t bUDiagDate[0];
} _PACKED_ MSAL_ALARM_PDU_t;

typedef struct
{
   uint8_t bAlarmType;
   uint8_t bSlotNr;
   uint8_t bSeqNr;
} _PACKED_ MSAL_ALARM_Res_P_PDU_t;

typedef struct
{
   uint8_t bAlarmType;
   uint8_t bSlotNr;
   uint8_t bSeqNr;
   uint8_t bStatus;
} _PACKED_ MSAL_ALARM_Res_N_PDU_t;

typedef struct {
   union{
	   uint8_t b;
	   struct {
		   uint8_t len:6;//block length in bytes,(including headerbyte) 4 to 63
		   uint8_t rev:2;//Bit 6,7 fixed to 0
	   }sbit;
   }bHeader;
   union{
	   uint8_t b;
	   struct {
		   uint8_t type:7;//Alarm_Type:
							//0 = reserved
							//1 = Diagnostic_Alarm
							//2 = Process_Alarm
							//3 = Pull_Alarm
							//4 = Plug_Alarm
							//5 = Status_Alarm
							//6 = Update_Alarm
							//7 - 31 = reserved
							//32 - 126 = manufacturer-specific
							//127 reserved
						//Status_Type:
						//0 reserved
						//1 Status_Message
						//2 Modul_Status
						//3 to 31 reserved
						//32 to 126 manufacturer-specific
						//127 reserved
		   uint8_t statusFlag:1;//Bit 7 fixed to 0 (Alarm) fix to 1(Status)
	   }sbit;
   }bAlarmStatusType;
   uint8_t              bSlotNr;//Number of the slot
   union{
	   uint8_t b;
	   struct {
		   uint8_t Alarm_Specifier:2;
								//00 no further differentiation
								//01 Error appears and Slot disturbed the slot genrates an alarm due to an error
								//10 Error disappears and Slot is okay the slot generates an alarm and indicates that the slot has no further errors
								//11 Error disappears and Slot is still disturbed the slot generates an alarm and indicates that the slot has still further errors
           uint8_t Add_Ack:1;// When setting this bit the slave indicates to the DPV1-Master (Class 1) that this alarm requires in addition to the
		   	   	   	   	   //MSAC1_Alarm_Ack a separate user acknowledgement. This can be done for instance by means of a Write service.
           uint8_t Seq_Nr:5;// By means of the Seq_Nr an unique identification of an alarm message is accomplished.
	   }sbit;
   }bSpecifier;
   uint8_t              bUserDiagLength;
   uint8_t              bCallback;
   uint8_t*             pToUserDiagData;

} _PACKED_ Alarm_Status_PDU_t;
typedef struct {
	   DPL_STRUC_LIST_CB    dplListHead;
	   union{
		   uint8_t b;
		   struct {
			   uint8_t len:6;//block length in bytes,(including headerbyte) 4 to 63
			   uint8_t rev:2;//Bit 6,7 fixed to 0
		   }sbit;
	   }bHeader;
	   union{
		   uint8_t b;
		   struct {
			   uint8_t type:7;//Alarm_Type:
								//0 = reserved
								//1 = Diagnostic_Alarm
								//2 = Process_Alarm
								//3 = Pull_Alarm
								//4 = Plug_Alarm
								//5 = Status_Alarm
								//6 = Update_Alarm
								//7 - 31 = reserved
								//32 - 126 = manufacturer-specific
								//127 reserved
							//Status_Type:
							//0 reserved
							//1 Status_Message
							//2 Modul_Status
							//3 to 31 reserved
							//32 to 126 manufacturer-specific
							//127 reserved
			   uint8_t statusFlag:1;//Bit 7 fixed to 0 (Alarm) fix to 1(Status)
		   }sbit;
	   }bAlarmStatusType;
	   uint8_t              bSlotNr;//Number of the slot
	   union{
		   uint8_t b;
		   struct {
			   uint8_t Alarm_Specifier:2;
									//00 no further differentiation
									//01 Error appears and Slot disturbed the slot genrates an alarm due to an error
									//10 Error disappears and Slot is okay the slot generates an alarm and indicates that the slot has no further errors
									//11 Error disappears and Slot is still disturbed the slot generates an alarm and indicates that the slot has still further errors
	           uint8_t Add_Ack:1;// When setting this bit the slave indicates to the DPV1-Master (Class 1) that this alarm requires in addition to the
			   	   	   	   	   //MSAC1_Alarm_Ack a separate user acknowledgement. This can be done for instance by means of a Write service.
	           uint8_t Seq_Nr:5;// By means of the Seq_Nr an unique identification of an alarm message is accomplished.
		   }sbit;
	   }bSpecifier;
	   uint8_t              bUserDiagLength;
	   //uint8_t              bCallback;
	   uint8_t*             pToUserDiagData;
}Alarm_Status_PDU_queue_t;

typedef union
{
   uint8_t                     uint8_t_ptr;
   MSAC_C1_ALARM_ACK_PDU_t     alarmAck;
   DPV1_NEG_RES_PDU_t          negRes; //- 响应
   DPV1_PDU_t                  posRes; // read,write,transport + 响应
   DPV1_PDU_t                  read; // read
   DPV1_PDU_t                  write; // write
   DPV1_PDU_t                  transport; // transport
   MSAL_ALARM_PDU_t            alarm;
   MSAL_ALARM_Res_P_PDU_t      alarmReq_P;
   MSAL_ALARM_Res_N_PDU_t      alarmReq_N;
   Alarm_Status_PDU_t          alarmStatus;
   MSAC_C2_INITIATE_REQ_PDU_t  c2InitReq;
   MSAC_C2_INITIATE_RES_PDU_t  c2InitRes;
   MSAC_C2_ABORT_PDU_t         c2Abort;
   MSAC_C2_RM_REQ_PDU_t        c2Rm;

} _PACKED_ DPV1_UNION_PDU_t;
/*---------------------------------------------------------------------------*/
/* 13.0 defines, structure for IM                                            */
/*---------------------------------------------------------------------------*/
/* structure for I&M CALL */
#define IM_FN_CALL                        ((uint8_t)0x08)

/* -- defines for I&M call service */
#define IM_CS_WRITE_RECORD                ((uint8_t)0x01)

#if LITTLE_ENDIAN
   #define IM0_INDEX_0                    ((uint16_t)0xE8FD)
   #define IM0_INDEX_1                    ((uint16_t)0xE9FD)
   #define IM0_INDEX_2                    ((uint16_t)0xEAFD)
   #define IM0_INDEX_3                    ((uint16_t)0xEBFD)
   #define IM0_INDEX_4                    ((uint16_t)0xECFD)

   #define IM_INDEX_65100                 ((uint16_t)0x4CFE)
#else
   #define IM0_INDEX_0                    ((uint16_t)0xFDE8)
   #define IM0_INDEX_1                    ((uint16_t)0xFDE9)
   #define IM0_INDEX_2                    ((uint16_t)0xFDEA)
   #define IM0_INDEX_3                    ((uint16_t)0xFDEB)
   #define IM0_INDEX_4                    ((uint16_t)0xFDEC)

   #define IM_INDEX_65100                 ((uint16_t)0xFE4C)
#endif /* #if LITTLE_ENDIAN */

typedef struct
{
   uint8_t  bExtendedFunctionNumber;
   uint8_t  bReserved;
   uint16_t wIndex;
   uint8_t  abData[1];
} _PACKED_ sIMCALL;
#define psIMCALL   sIMCALL

#define cSizeOfImCall   ((uint8_t)0x04)

/* defines for I&M supported */
#if LITTLE_ENDIAN
   #define IM00_SUPPORTED                 ((uint16_t)0x0000)
   #define IM01_SUPPORTED                 ((uint16_t)0x0200)
   #define IM02_SUPPORTED                 ((uint16_t)0x0400)
   #define IM03_SUPPORTED                 ((uint16_t)0x0800)
   #define IM04_SUPPORTED                 ((uint16_t)0x1000)
   #define IM05_SUPPORTED                 ((uint16_t)0x2000)
   #define IM06_SUPPORTED                 ((uint16_t)0x4000)
   #define IM07_SUPPORTED                 ((uint16_t)0x8000)
   #define IM08_SUPPORTED                 ((uint16_t)0x0001)
   #define IM09_SUPPORTED                 ((uint16_t)0x0002)
   #define IM10_SUPPORTED                 ((uint16_t)0x0004)
   #define IM11_SUPPORTED                 ((uint16_t)0x0008)
   #define IM12_SUPPORTED                 ((uint16_t)0x0010)
   #define IM13_SUPPORTED                 ((uint16_t)0x0020)
   #define IM14_SUPPORTED                 ((uint16_t)0x0040)
   #define IM15_SUPPORTED                 ((uint16_t)0x0080)
#else
   #define IM00_SUPPORTED                 ((uint16_t)0x0000)
   #define IM01_SUPPORTED                 ((uint16_t)0x0002)
   #define IM02_SUPPORTED                 ((uint16_t)0x0004)
   #define IM03_SUPPORTED                 ((uint16_t)0x0008)
   #define IM04_SUPPORTED                 ((uint16_t)0x0010)
   #define IM05_SUPPORTED                 ((uint16_t)0x0020)
   #define IM06_SUPPORTED                 ((uint16_t)0x0040)
   #define IM07_SUPPORTED                 ((uint16_t)0x0080)
   #define IM08_SUPPORTED                 ((uint16_t)0x0100)
   #define IM09_SUPPORTED                 ((uint16_t)0x0200)
   #define IM10_SUPPORTED                 ((uint16_t)0x0400)
   #define IM11_SUPPORTED                 ((uint16_t)0x0800)
   #define IM12_SUPPORTED                 ((uint16_t)0x1000)
   #define IM13_SUPPORTED                 ((uint16_t)0x2000)
   #define IM14_SUPPORTED                 ((uint16_t)0x4000)
   #define IM15_SUPPORTED                 ((uint16_t)0x8000)
#endif /* #if LITTLE_ENDIAN */

/* structure for I&M0 (mandatory) */
typedef struct
{
   uint8_t  abHeader[10];
   uint16_t wManufacturerID;
   uint8_t  abOrderID[20];
   uint8_t  abSerialNumber[16];
   uint8_t  abHardwareRevision[2];
   uint8_t  abSoftwareRevision[4];
   uint16_t wRevCounter;
   uint16_t wProfileID;
   uint16_t wProfileSpecificType;
   uint8_t  abIMVersion[2];
   uint16_t wIMSupported;
} _PACKED_ sIM0;

#define cSizeOfIM0 ((uint8_t)0x40)
#define psIM_0 sIM0  VPC3_PTR

/* structure for I&M1 (optional) */
typedef struct
{
   uint8_t abHeader[10];
   uint8_t abTagFunction[32];
   uint8_t abTagLocation[22];
} _PACKED_ sIM1;

#define cSizeOfIM1 ((uint8_t)0x40)

/* structure for I&M2 (optional) */
typedef struct
{
   uint8_t abHeader[10];
   uint8_t abInstallationDate[16];
   uint8_t abReserved[38];
} _PACKED_ sIM2;

#define cSizeOfIM2 ((uint8_t)0x40)

/* structure for I&M3 (optional) */
typedef struct
{
   uint8_t abHeader[10];
   uint8_t abDescriptor[54];
} _PACKED_ sIM3;

#define cSizeOfIM3 ((uint8_t)0x40)

/* structure for I&M4 (optional) */
typedef struct
{
   uint8_t abHeader[10];
   uint8_t abSignature[54];
} _PACKED_ sIM4;

#define cSizeOfIM4 ((uint8_t)0x40)



/*-----------------------------------------------------------------------------------------------------------*/
/* 16.0 ERROR defines                                                                                        */
/*-----------------------------------------------------------------------------------------------------------*/
typedef enum
{
   DP_OK                               = 0x00, /* OK */
   DP_NOK                              = 0x80,

   DP_ADDRESS_ERROR                    = 0x11, /* Slave Address Error */
   DP_CALCULATE_IO_ERROR               = 0x12,
   DP_DOUT_LEN_ERROR                   = 0x13,
   DP_DIN_LEN_ERROR                    = 0x14,
   DP_DIAG_LEN_ERROR                   = 0x15,
   DP_PRM_LEN_ERROR                    = 0x16,
   DP_SSA_LEN_ERROR                    = 0x17,
   DP_CFG_LEN_ERROR                    = 0x18,
   DP_CFG_FORMAT_ERROR                 = 0x19,
   DP_LESS_MEM_ERROR                   = 0x1A,
   DP_LESS_MEM_FDL_ERROR               = 0x1B,
   DP_PRM_RETRY_ERROR                  = 0x1C,
   DP_SPEC_PRM_NOT_SUPP_ERROR          = 0x1D,

   DP_PRM_ENTRY_ERROR                  = 0x20,
   DP_PRM_SERVICE_NOT_SUPPORTED        = 0x21,
   DP_PRM_DPV1_STATUS                  = 0x22,
   DP_PRM_DPV0_NOT_SUPP                = 0x23,
   DP_PRM_DPV1_NOT_SUPP                = 0x24,
   DP_PRM_BLOCK_ERROR                  = 0x25,
   DP_PRM_BLOCK_CMD_NOT_SUPP           = 0x26,
   DP_PRM_ALARM_ERROR                  = 0x27,
   DP_PRMCMD_LEN_ERROR                 = 0x28,
   DP_PRM_SOLL_IST_ERROR               = 0x29,

   DP_PRM_USER_PRM_BLOCK_ERROR         = 0x2A,

   DP_PRM_DXB_PRM_BLOCK_LENGTH_ERROR   = 0x2B,
   DP_PRM_DXB_MAX_LINK_ERROR           = 0x2C,
   DP_PRM_DXB_ERROR                    = 0x2D,
   DP_PRM_DXB_WD_ERROR                 = 0x2E,

   DP_PRM_CS_LENGTH_ERROR              = 0x30,
   DP_PRM_CS_INTERVAL_ERROR            = 0x31,

   DP_PRM_ISO_PRM_BLOCK_ERROR          = 0x40,
   DP_PRM_DPV1_STATUS_3_ISOM_REQ_ERROR = 0x41,
   DP_PRM_ISO_T_BASE_DP_ERROR          = 0x42,
   DP_PRM_ISO_T_BASE_IO_ERROR          = 0x43,

   DP_PRM_UNKNOWN_MOD_REF              = 0x4A,

   DP_CFG_ENTRY_ERROR                  = 0x50,
   DP_CFG_UPDATE_ERROR                 = 0x51,

   DP_DIAG_BUFFER_ERROR                = 0x60,
   DP_DIAG_SEQUENCE_ERROR              = 0x61,
   DP_DIAG_OLD_DIAG_NOT_SEND_ERROR     = 0x62,
   DP_DIAG_NOT_POSSIBLE_ERROR          = 0x63,
   DP_DIAG_NO_BUFFER_ERROR             = 0x64,
   DP_DIAG_BUFFER_LENGTH_ERROR         = 0x65,
   DP_DIAG_CONTROL_BYTE_ERROR          = 0x66,
   DP_DIAG_SAME_DIAG                   = 0x67,
   DP_DIAG_ACTIVE_DIAG                 = 0x68,

   C1_DATA_LEN_ERROR                   = 0x70,

   C2_DATA_LEN_ERROR                   = 0x80,
   C2_DATA_POLL_TIMEOUT_ERROR          = 0x81,
   C2_DATA_SAP_ERROR                   = 0x82,
   C2_NO_CONN_RESOURCE                 = 0x83,
   C2_INV_LOWER_LAYER                  = 0x84,
   C2_ENABLED_ERROR                    = 0x85,
   C2_RESOURCE_ERROR                   = 0x86,
   C2_INV_CN_ID                        = 0x87,
   C2_USER_ERR                         = 0x88,
   C2_DOUBLE_REQUEST                   = 0x89,
   C2_ALREADY_DISCONNECTED             = 0x8A,

   SSC_MAX_DATA_PER_LINK               = 0x90,

   DP_EEPROM_ERROR                     = 0xF1, /* Hardware errors */


} DP_ERROR_CODE;


typedef enum{
	sMSCY1S_PowerON = (uint8_t)0,
	sMSCY1S_WaitIniCon ,
	sMSCY1S_WaitPRM ,
	sMSCY1S_WaitCFG ,
	sMSCY1S_DataEXCH ,
	sMSCY1S_CheckFreeze ,
	sMSCY1S_CheckClear ,
	sMSCY1S_WaitdeactCon ,
	sMSCY1S_WaitSlaveDeact ,
}MSCY1S_State;

typedef struct {
	uint8_t bMaxLsduLengthReqLow; //请求从站的，即从站发送给主站，去除DA\SA\FC，后的PDU长度
	uint8_t bMaxLsduLengthReqHigh;
	uint8_t bMaxLsduLengthIndLow;//发送给从站的，即主站发送给从站，去除DA\SA\FC，后的PDU长度
	uint8_t bMaxLsduLengthIndHigh;
}LsduLength_list_t;


typedef struct
{
	uint32_t regTimerCnt; //返回当前timer 计数器的寄存器地址
    uint32_t regGPIORXPort;//返回UART RX引脚端口寄存器地址
    uint32_t regGPIORXPin;//返回UART RX引脚
    void (*timerEnable)(uint8_t enable);//该函数实现Timer 启停
    uint32_t clkTimerHz;//当前初始化的TIMER的计数频率，Hz为单位
    uint32_t timerMaxCntValue;//定时器最大计数值
}phy_bpsdvar_t;

/*
 * 用户层需要实现的函数
 *
 */
typedef struct {
	int (*FMA12_SetValue_Req)(uint8_t MSAP0,//The parameter MSAP_0 has the same meaning as described for the FMA1/2_RESET.	request primitive.
	 void* pVariable_name,// The parameter Variable_name contains a list of one or more variables of the FDL and/or PHY. The selectable variables are specified in subclauses 5.2.3.2 and 5.3.3.2.
	 void* pDesired_value,//The parameter Desired_value contains a list of one or more values for the specified variables. The permissible values or ranges of values are specified
	 //in subclauses 5.2.3.2 and 5.3.3.2.
	 uint32_t LocBaudrate
	 );
	/*
	 * 该函数用于DP 基础定时器
	 *
	 */
	void (*dp_baseTimer_config)(void);

	/*
	 * 该函数用于DP 校验参数数据的有效性
	 * 有效返回true，否则false
	 */
	int (*usr_PrmData_Check)(void*pPdu,uint8_t len);
	/*
	 * 该函数用于DP 校验配置的有效性
	 * 有效返回true，否则false
	 */
	int (*usr_CfgData_Check)(void*pToCfgData,uint8_t bCfgLength);

	/*!  dp_updateNewOutPut
	    \brief   fdl收到新的输出数据。
	    该函数提示用户继续提取最新的输出数据
	    \param[in]
	      \arg
	      \arg
	    \param[out]
	      \arg
	    \retval none
	*/
	void (*dp_updateNewOutPut)(uint8_t* pOutput,uint8_t len);
	/*
	 * 该函数用于发送数据到物理端口上
	 */
	void (*fdl_PortSend)(fdl_frame_t* pFrame);


	/*
	 * 该函数用于设置MinTsdr
	 */
	int (*setMinTsdr)(uint16_t  MinTsdr);

	/*
	 * 该函数用于返回真实的配置
	 */
	void  (*dp_Setup_Real_Cfg_Data)(dp_CFG_t* pCfg);

	/*
	 * 该函数用于返回真实的输出长度
	 */
	void  (*dp_Setup_Real_Output_len)(uint8_t* pLen);
	/*
	 * 该函数用于返回真实的输入长度
	 */
	void  (*dp_Setup_Real_Input_len)(uint8_t* pLen);

	/*
	 * 该函数用于返回真实的地址设置
	 */
	void  (*dp_Setup_Real_No_Add_Chg)(uint8_t* pchg);

	/*
	 * 该函数用于返回真实的Identify
	 */
	void  (*dp_Setup_Real_Ident)(uint16_t* pIdent);

	/*
	 * 该函数用于提示用户存储新的从站地址
	 */
	void  (*dp_setNewSlaveAddr)(const dp_SSA_BLOCK_t * const pNewSSA);

	/*
	 * With the MSAL1S_Abort.req the user resets the Alarm-State-Machine.
	In faulty conditions the Alarm-State-Machine informs the user with a
	MSAL1S_Abort.ind and resets itself. In both cases the Alarm-StateMachine enters the state W-START.
	 */
	void (*MSAL1S_Abort_Ind)(void);
	/*
	 * With this function the Alarm-State-Machine indicates to the user the
	supported alarm features on this MSAC_C1 communication relationship.
	The indication occurs after the slave has entered the data exchange
	state and the MSAC_C1 is in the OPEN state. Now the transfer of
	alarm and status messages is possible.
	 */
	void (*MSAL1S_Started_Ind)(
			uint8_t bActualEnabledAlarms,//This parameter indicates which alarm types are supported by the DPV1-Master (Class 1).
			uint8_t bAlarm_Sequence,//
			uint8_t Alarm_Limit
			);
	/*
	 * This MSAL1S service indicates to the user that the slave has left
	the data exchange state and that the MASC_C1 communication relationship is closed. In this case the transfer of alarm messages is no
	longer possible and the MSAL1S enters the same state as after
	MSAL1S_Init.
	 */
	void (*MSAL1S_Stopped_Ind)(void);
	/*
	 * By means of this function the Alarm-State-Machine indicates to the
	User that due to fatal errors the Alarm-State-Machine MSAL1S, the
	cyclic State Machine MSCY1S and the acyclic State Machine MSAC1S
	enter the Power-On state.
	 */
	void (*MSAL1S_Fault_Ind)(uint8_t ASM_Status //This parameter indicates the reason for the failure.		Possible values: DDLM_Fault, Set_Slave_Add, MSAC1S_Fault
			);

	/*
	 * By means of this function the RM State Machine indicates to the User
	that due to fatal errors all MSAC_C2 and the RM State Machine have
	been stopped.
	 */
	void (*MSRM2S_Stopped_Ind)(uint8_t Status//This parameter indicates the reason for the failure.Possible Values: MSRM2S_Fault, MSAC2S_Fault
			);
	/*
	 * By means of this function the RM State Machine indicates a fatal
	error to the User.
	 */
	void (*MSRM2S_Fault_Ind)(uint8_t Status//This parameter indicates the reason for the failure.Possible Values: FDL_Fault, MSRM2S_Fault
			);

	int (*FMA12_Reset_Req)(uint8_t MSAP0 // The parameter MSAP_0 (Management Service Access Point_0) specifies the service access point of the Local FMA1/2 User. This MSAP is activated and configured depending on the implementation.
			);

	/*
	 * By means of this service a read request for a data block is
	transferred to a DPV1-Slave. The service operates slot and index
	related.
	 */
	int (*user_dpv1_read_req)( uint8_t sap, DPV1_UNION_PDU_t* pdu );
	/*
	 * By means of this service a write request is transferred to a DPV1-
	Slave.
	 */
	int (*user_dpv1_write_req)( uint8_t sap, DPV1_UNION_PDU_t* pdu );
	/*
	 * By means of this service an alarm message is transferred from the
	DPV1-Slave to the DPV1-Master (Class 1). By means of the
	subordinated service MSAC1_Alarm_Ack (see chapter 6.2.1) the
	acknowledgement of the alarm will be transmitted from the master to
	the slave.
	 */
	void (*MSAL1_Alarm_Con)(uint8_t isPosive,DPV1_UNION_PDU_t* pAlarmCon,uint8_t Status);


	int (*MSAC2_Initiate_Ind)(
			uint8_t bReqAdd,//The parameter Rem_Add (Remote_Address) specifies the FDL address of the remote station.
	        uint8_t bResSAP,//This parameter contains the station address of the requester. By means of this address the access protection in the responder is supplied
			uint8_t  bFeaturesSupported1,//Bit 0: DPV1_RW	This bit is set if the services MSAC2_Read and MSAC2_Write are supported (Mandantory for DPV1-Slaves)
			uint8_t  bFeaturesSupported2,
			//DPV1-Master and DPV1-Slave inform each other about the supported	service functionality regarding the used profile definition. The meaning of the defined bits is profile or vendor specific.
			uint8_t  bProfileFeaturesSupported1,
			uint8_t  bProfileFeaturesSupported2,
			//By means of this parameter a profile definition is identified	uniquely. The Profile_Ident_Number will be taken from the pool of
			//Ident_Numbers handled by the PNO. All devices using the same profile	definition have to use the same Profile_Ident_Number. The
			//Profile_Ident_Number is independent from the Ident_Number.
			//If the requested profile is supported by the slave, the Profile_Ident_Number is mirrored in the response PDU.
			uint16_t wProfileIdentNumber,
			//Add_Addr_Param consists of two parts, additional address information	of the source and additional address information of the destination.
			//The additional address information contains the parameter API	(Application Process Instance) plus an optional Network/MAC address.
			//D_Addr and S_Addr (API, SCL, Network_Address, MAC_Address) are related to the direction of the telegrams, which means the source	becomes destination and vice versa.
			uint8_t  bS_Type,
			uint8_t  bS_Len,
			uint8_t  bD_Type,
			uint8_t  bD_Len,
			uint8_t*  paAddrData  /* to page 55 */
	);
	/*
	 * The DPV1-Master (Class 2) or DPV1-Slave are using the service
	MSAC2_Abort to abort a MSAC_C2 communication relationship.
	 */
	int (*MSAC2_Abort_Ind)(
	        uint8_t  bResSAP,//This parameter contains the station address of the requester. By means of this address the access protection in the responder is supplied
			uint8_t  bLocallyGenerated,//This parameter only exists in the indication and is not encoded in the PDU. It indicates that the Abort has been produced by the remote
			//partner (Locally_Generated=FALSE) or in the local station	(Locally_Generated=TRUE).
			uint8_t  bSubnet,//	This parameter is used to encode information about the location of the source of the Abort.req.
			uint8_t  bInstance,//This parameter is used to encode information about the protocol instance that detected a problem causing the Abort.req.
			uint8_t  bReasonCode//The Reason_Code indicates the reason for the Abort.req.
	);

	/*
	 * By means of this service data blocks are transferred
	- from the DPV1-Master to the DPV1-Slave
	- from the DPV1-Slave to the DPV1-Master
	- or from the DPV1-Master to the DPV1-Slave and back.
	The data and the interpretation of these data are user specific. In
	special application areas (e. g. drives) user groups can define a
	profile which guarantees the same interpretation of the transferred
	data.
	The differentiation of these profiles will be done by means of the
	Profile_Ident_Number contained in the Initiate-Service (see chapter
	7.1).
	 */
	int (*user_c2_data_transport_req)( uint8_t sap, DPV1_UNION_PDU_t* pdu );


	/**
	 * I2C 功能区
	 */
	void (*I2C_Open)(void);
	void (*I2C_ReadByte)(uint8_t *pByteData, uint8_t AddrToRead);
    void (*I2C_WriteByte)(uint8_t ByteData, uint8_t AddrToWrite);
    void (*I2C_Read)(uint8_t AddrToRead, uint8_t *pData, uint16_t Length);
    void (*I2C_Write)(uint8_t AddrToWrite, uint8_t *pData, uint16_t Length);
	void (*I2C_Close)(void);
	void (*I2C_WDinit)(void);
    uint32_t regGPIOSDAPort;//返回SCL引脚端口寄存器地址
    uint32_t regGPIOSDAPin;//返回SCL引脚


	/*
	 * 波特率自适应硬件初始化
	 *
	 * 必须完成如下准备
	 *
	 * 1、初始化一个计数模式timer,timer的计数时钟尽可能大
	 * 2、将DP UART接口设置为输入模式
	 * 3、将DP UART接口的RX引脚置为GPIO 输入模式
	 * 4、实现必要的函数
	 */
	void (*phy_bps_detect_init)(phy_bpsdvar_t ** pIntVar);
	/*
	 * 该定时器监测总线波特率
	 * 当总线在该定时器内没有收到任何有效数据，则进入自动波特率探测模式，直到锁定总线波特率！
	 * 要和主站组态进行匹配，不可过小，也不可过大。
	 */
	uint32_t userWatchDogms;// 定义用户看门狗时间，单位ms 。
	/*
	 * PA模式的FDL层
	 */
	uint32_t setPAmode;

}user_DP_callback_t;



/*
 *
 * 用户层调用的函数
 */

typedef struct
{

	void* (*fdl_Init)(const user_DP_callback_t * const pCallBack);
	/*
	 * 1.1 user call
	 */
	/*!
	 * fdl_StateProcess
	    \brief      数据链路层，状态机处理。
	    需要周期性调用该函数，已实现协议的完整处理。

	    \param[in]
	      \arg
	      \arg
	    \param[out]
	    \retval

	*/
	void (* fdl_StateProcess)(void);
	/*
	 * DP 每一ms调用一次该函数
	 *
	 * 该函数实现了一个1ms精度的timers
	 * ms 指出每次调用间隔ms 时间
	 */
	void (* port_TimerTick1ms)(uint32_t ms);

	/*!  fdl_PortGetReciveBuffer
	    \brief      当链路层接收到一帧数据时，应该调用该函数 。
	    \param[in]
	      \arg        pPdu: 数据帧
	      \arg        len: 长度
	    \param[out]
	      \arg
	    \retval none
	*/
	void*(*fdl_PortGetReciveBuffer)(void);

	/*!  fdl_PortFreeBuffer
	    \brief      用户主动给是否buffer ,应该调用该函数 。
	    \param[in]
	      \arg        pPdu: 数据帧
	      \arg        len: 长度
	    \param[out]
	      \arg
	    \retval none
	*/
	void (*fdl_PortFreeBuffer)(void*);

	/*!  fdl_PortRecive
	    \brief      当链路层接收到一帧数据时，应该调用该函数 。
	    \param[in]
	      \arg        pFrame: 数据帧
	      \arg
	    \param[out]
	      \arg
	    \retval none
	*/
	void (*fdl_PortRecive)(void* pFrame);


	/*!  fdl_PortSendFinish
	    \brief      帧发送完成调用。
	    务必调用该函数

	    \param[in]
	      \arg
	      \arg
	    \param[out]
	    \retval

	*/
	void (*fdl_PortSendFinish)(void* pSendFrame);
	/*
	 * DP 用户准备好后调用
	 *
	 * 该函数实现对提示FDL层，应用准备好了
	 */
	void (*dp_updateApplicationReady)(uint8_t ready);

	/*
	 * DP 用户查询ApplicationReady状态
	 *
	 *
	 */
	uint8_t (*dp_getApplicationReady)(void);


	/*!  dp_updateNewDiagData
	    \brief   新的诊断数据更新 提示。
	    用户有新的扩展数据，必须调用该函数完成更新操作
	    \param[in]
	      \arg
	      \arg
	    \param[out]
	      \arg
	    \retval none
	*/

	void (*dp_updateNewExtDiagData)(
			uint8_t bExtDiagFlag,//This parameter will be mapped on the Diag.Ext_Diag-Flag (see	EN 50170).
			uint8_t* pDiagBlock,//This parameter will be mapped to the Ext_Diag_Data (see EN 50170).
	        uint8_t bDiagLen
			);

	//用户更新配置数据时调用
	/*!  dp_updateNewCfgData
	    \brief   本地事件“组态变化”
	    用户调用该函数完成组态数据的更新操作
	    \param[in]
	      \arg
	      \arg
	    \param[out]
	      \arg
	    \retval none
	*/
	void (*dp_updateNewCfgData)(dp_CFG_t* pCfg);


	/*!  dp_updateNewInput
	    \brief   新的输入数据更新 提示。
	    用户调用该函数完成输入数据的更新操作
	    \param[in]
	      \arg
	      \arg
	    \param[out]
	      \arg
	    \retval none
	*/
	void (*dp_updateNewInput)(uint8_t* pInput,uint8_t len);

	/*
	 * DP 设置从站地址
	 *
	 *
	 */
	void (*dp_setStationAddr)(uint8_t addr);

	/*
	 * DP 用户获取MSCY1S状态
	 *
	 *
	 */


	MSCY1S_State (*dp_getMSCY1S_State)(void);


	/*
	 * 1.2 Local Functions between User and MSAL1S
	 */

	/*
	 * By means of this function the User initializes the Alarm-StateMachine.
	 */
	int (*MSAL1S_Init_Req)(	LsduLength_list_t* pbAcycLengths,//This parameter will be mapped onto the parameter L_sdu_length_list 	of the FMA1/2_RSAP_ACTIVATE.req
							uint8_t bAlarmModeSlave,// This parameter specifies the maximum number of possible active	alarms.Range: 0 to 7
							uint8_t bMaxAlarmLen,//This parameter specifies the maximum length of an Alarm-PDU. Range: 4 to 63
							uint8_t bDefaultExtDiag,// This parameter contains the value of the Ext_Diag-Flag at the start	up phase.false or true
							uint8_t* paDefaultExtDiagData,//This parameter fixes the contents of the Ext_Diag_Data at the start	up phase of the DPV1-Slave.
							//These parameters define the maximum length of the respective 	telegrams which can be used during operation.
							uint8_t bDiagLen,//
							uint8_t bPrmLen,
							uint8_t bCfgLen,
							uint8_t bInpLen,
							uint8_t bOutpLen,
							uint8_t bSAdLen
	);

	/*
	 * By means of this service new status data are indicated to the User of the DPV1 Master(Class 1) in the Diag_Data area.
	*/
	void (*MSAL1S_Status_Req)(uint8_t* pStatus,//A set of Status-Blocks. Each Status-Block is coded as described in the Diagnoctic Model.Type: Octet String Range: 0 to 238
			uint8_t len);


	/*
	 * With the MSAL1S_Abort.req the user resets the Alarm-State-Machine.
	In faulty conditions the Alarm-State-Machine informs the user with a
	MSAL1S_Abort.ind and resets itself. In both cases the Alarm-StateMachine enters the state W-START.
	 */
	void (*MSAL1S_Abort_Req)(void);

	/*
	 * By means of this function the User causes the Alarm-State-Machine
	MSAL1S, the cyclic State Machine MSCY1S and the acyclic State
	Machine MSAC1S to enter the Power-On state.
	 */
	void (*MSAL1S_GoPowerOn_Req)(void);


	/*
	 * By means of this service diagnostic data are transferred from the
	DPV1-Slave to the DPV1-Master(Class 1).
	 */
	int (*MSAL1S_DiagUpd_Req)(uint8_t Diag_Flag,//This parameter is mapped to the Diag.Ext_Diag Flag described in EN 50170.
			uint8_t* pUpd_Diag_Buffer,//This parameter is mapped to Ext_Diag_Data described in EN 50170.
			uint8_t  diag_len
	);

	/*
	 * 1.3 Local Functions between User and DDLM
	 */
	/*
	 * By means of this function the User starts the DDLM and will be
	mapped to the FMA1/2_RESET.
	 */
	int (*DDLM_Start_Req)(
			uint8_t bStationAddress,//This parameter contains the local FDL address of this DPV1-Slave
			uint32_t lBaudrate,//This parameter comprises the locally defined baudrate.
			uint8_t bMedium_red//This parameter defines whether the system supports redundant media	or not.
			);


	void (*MSCY1S_Enabel)(uint8_t freezeSupported,
			uint8_t syncSupported,
			uint8_t Fail_Safe
			);

	/*
	 * By means of this function the User resets the DDLM.
	 */
	int (*DDLM_Reset_Req)(void);


	/*
	 * 1.4 Local Functions between User and MSRM2S
	 */
	/*
	 * By means of this function the user initializes the Resource Manager.
	 */
	int (*MSRM2S_Init_Req)(
			//RM_Registery_t * pRmReg,//The parameter RM_Registry contains a list of RM_Registry_Entry	(List_of_RM_Registry_Entries).
			//resSAPList_t * pResSAPList,//The parameter Res_SAP_List contains the FDL-SAPs available for	MSAC_C2 communication relationships.
			uint16_t bMinSendTimeout//This parameter defines the minimum Send_Timeout supported in this station.
			);

	/*
	 * By means of this function the User causes the RM State Machine to
	enter the Power-On mode.
	 */
	void (*MSRM2S_Reset_Req)(void);

	/*
	 * By means of this service an alarm message is transferred from the
	DPV1-Slave to the DPV1-Master (Class 1). By means of the
	subordinated service MSAC1_Alarm_Ack (see chapter 6.2.1) the
	acknowledgement of the alarm will be transmitted from the master to
	the slave.
	 */
	int (*MSAL1_Alarm_Req)(
			Alarm_Status_PDU_queue_t* pPDU);

	/*
	 * The DPV1-Master (Class 2) uses the service MSAC2_Initiate to
	establish and the MSAC2_Abort service to abort a communication
	relationship to the DPV1-Slave.
	 */
	int (*MSAC2S_Initiate_Req)(
			uint8_t bResSAP,//Service Access Points (SAPs) used at the server side for this	connection.
			uint8_t bReqAdd,//The parameter Req_Add (Requester Address) specifies the FDL address of the remote station.
			uint8_t bMaxLenDataUnit,//This parameter defines the maximum L_sdu length to be applied in the FMA1/2_SAP_ACTIVATE.req and to be set in the Initiate-RES-PDU.
			//The remaining service parameters correspond to the service parameters of the MSAC2_Initiate (see 7.1).
			uint16_t bSendTimeout,//
			uint8_t  bFeaturesSupported1,//Bit 0: DPV1_RW	This bit is set if the services MSAC2_Read and MSAC2_Write are supported (Mandantory for DPV1-Slaves)
			uint8_t  bFeaturesSupported2,
			//DPV1-Master and DPV1-Slave inform each other about the supported	service functionality regarding the used profile definition. The meaning of the defined bits is profile or vendor specific.
			uint8_t  bProfileFeaturesSupported1,
			uint8_t  bProfileFeaturesSupported2,
			//By means of this parameter a profile definition is identified	uniquely. The Profile_Ident_Number will be taken from the pool of
			//Ident_Numbers handled by the PNO. All devices using the same profile	definition have to use the same Profile_Ident_Number. The
			//Profile_Ident_Number is independent from the Ident_Number.
			//If the requested profile is supported by the slave, the Profile_Ident_Number is mirrored in the response PDU.
			uint16_t wProfileIdentNumber,
			//Add_Addr_Param consists of two parts, additional address information	of the source and additional address information of the destination.
			//The additional address information contains the parameter API	(Application Process Instance) plus an optional Network/MAC address.
			//D_Addr and S_Addr (API, SCL, Network_Address, MAC_Address) are related to the direction of the telegrams, which means the source	becomes destination and vice versa.
			uint8_t  bS_Type,
			uint8_t  bS_Len,
			uint8_t  bD_Type,
			uint8_t  bD_Len,
			uint8_t*  paAddrData  /* to page 55 */
			);

	int (*MSAC2_Initiate_Res)(
			uint8_t  isPositive,//1:+,0,-
	        uint8_t  bResSAP,//This parameter contains the station address of the requester. By means of this address the access protection in the responder is supplied
			DPV1_UNION_PDU_t* pPdu,
			uint8_t len
	);
	/*
	 * The DPV1-Master (Class 2) or DPV1-Slave are using the service
	MSAC2_Abort to abort a MSAC_C2 communication relationship.
	 */
	int (*MSAC2_Abort_Req)(
	        uint8_t  bResSAP,//This parameter contains the station address of the requester. By means of this address the access protection in the responder is supplied
			uint8_t  bSubnet,//	This parameter is used to encode information about the location of the source of the Abort.req.
			uint8_t  bInstance,//This parameter is used to encode information about the protocol instance that detected a problem causing the Abort.req.
			uint8_t  bReasonCode//The Reason_Code indicates the reason for the Abort.req.
	);

	/*
	 * By means of this service data blocks are transferred
	- from the DPV1-Master to the DPV1-Slave
	- from the DPV1-Slave to the DPV1-Master
	- or from the DPV1-Master to the DPV1-Slave and back.
	The data and the interpretation of these data are user specific. In
	special application areas (e. g. drives) user groups can define a
	profile which guarantees the same interpretation of the transferred
	data.
	The differentiation of these profiles will be done by means of the
	Profile_Ident_Number contained in the Initiate-Service (see chapter
	7.1).
	 */
	int (*MSAC2_DataTransportRes)(
	        uint8_t  bResSAP,
			uint8_t  isPositive,//1:+,0,-
			DPV1_UNION_PDU_t* pPdu,
			uint8_t len//总报文长度
	);
}user_DP_call_t;

#endif



