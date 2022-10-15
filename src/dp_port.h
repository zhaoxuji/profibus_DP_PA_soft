/*
 * dp_port.h
 *
 *  Created on: 2021年10月14日
 *      Author: zhaox
 */

#ifndef SRC_DP_PORT_H_
#define SRC_DP_PORT_H_

#include "stm32l431xx.h"
#include "main.h"
#include <string.h>
#include <stdio.h>
#include "dp.h"
#include "platformDebug.h"


#define DAC8740H          0  /* 定义物理层是否是DAC8740H */
#define TEST_DP_PORT      0
#define TEST_TIMER        0
/* 定义引脚、端口 */
#if  DAC8740H
#define PADUP_Pin        LL_GPIO_PIN_15
#define PADUP_GPIO_Port  GPIOB

#define PACD_Pin         LL_GPIO_PIN_6
#define PACD_GPIO_Port   GPIOC

#define LED0_Pin         LL_GPIO_PIN_8
#define LED0_GPIO_Port   GPIOC

#define DEX_Pin          LL_GPIO_PIN_9
#define DEX_GPIO_Port    GPIOC

#define PARST_Pin        LL_GPIO_PIN_3
#define PARST_GPIO_Port  GPIOB
#endif

#define DPRTS_Pin        LL_GPIO_PIN_11
#define DPRTS_GPIO_Port  GPIOA

//如果使用硬件RTS，屏蔽掉下面的宏
#define DP_RTS_ON()                     LL_GPIO_SetOutputPin(DPRTS_GPIO_Port, DPRTS_Pin)
#define DP_RTS_OFF()                    LL_GPIO_ResetOutputPin(DPRTS_GPIO_Port, DPRTS_Pin)

#define DP_PORT                        huart1


extern user_DP_call_t * pUserDPCall ;

void uartDMA_init(uint32_t baudrate);

void uartDMA_deinit(void);
/*----------------------------------------------
 * 2  DP 应该层需要实现的函数
 *
 * ---------------------------------------------
 */
 int FMA12_SetValue_Req(uint8_t MSAP0,//The parameter MSAP_0 has the same meaning as described for the FMA1/2_RESET.	request primitive.
 void* pVariable_name,// The parameter Variable_name contains a list of one or more variables of the FDL and/or PHY. The selectable variables are specified in subclauses 5.2.3.2 and 5.3.3.2.
 void* pDesired_value,//The parameter Desired_value contains a list of one or more values for the specified variables. The permissible values or ranges of values are specified
 //in subclauses 5.2.3.2 and 5.3.3.2.
 uint32_t LocBaudrate
 );

 /*
  * 该函数用于DP 基础定时器
  *
  */
 void eval_baseTimer_config(void);
/*
 * 该函数用于DP 校验参数数据的有效性
 * 有效返回true，否则false
 */
 int usr_PrmData_Check(void*pPdu,uint8_t len);
/*
 * 该函数用于DP 校验配置的有效性
 * 有效返回true，否则false
 */
 int usr_CfgData_Check(void*pToCfgData,uint8_t bCfgLength);

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
 void dp_updateNewOutPut(uint8_t* pOutput,uint8_t len);
/*
 * 该函数用于发送数据到物理端口上
 */
 void fdl_PortSend(fdl_frame_t* pFrame);

/*!  fdl_PortSendFinish
    \brief      帧发送完成调用。
    务必调用该函数

    \param[in]
      \arg
      \arg
    \param[out]
    \retval

*/
void fdl_PortSendFinish(void* pSendFrame);
/*
 * 该函数用于设置MinTsdr
 */
 int setMinTsdr(uint16_t  MinTsdr);

/*
 * 该函数用于返回真实的配置
 */
 void  dp_Setup_Real_Cfg_Data(dp_CFG_t* pCfg);

/*
 * 该函数用于返回真实的输出长度
 */
 void  dp_Setup_Real_Output_len(uint8_t* pLen);
/*
 * 该函数用于返回真实的输入长度
 */
 void  dp_Setup_Real_Input_len(uint8_t* pLen);

/*
 * 该函数用于返回真实的地址设置
 */
 void  dp_Setup_Real_No_Add_Chg(uint8_t* pchg);

/*
 * 该函数用于返回真实的Identify
 */
 void  dp_Setup_Real_Ident(uint16_t* pIdent);

/*
 * 该函数用于提示用户存储新的从站地址
 */
 void  dp_setNewSlaveAddr(const dp_SSA_BLOCK_t * const pNewSSA);

/*
 * With the MSAL1S_Abort.req the user resets the Alarm-State-Machine.
In faulty conditions the Alarm-State-Machine informs the user with a
MSAL1S_Abort.ind and resets itself. In both cases the Alarm-StateMachine enters the state W-START.
 */
 void MSAL1S_Abort_Ind(void);
/*
 * With this function the Alarm-State-Machine indicates to the user the
supported alarm features on this MSAC_C1 communication relationship.
The indication occurs after the slave has entered the data exchange
state and the MSAC_C1 is in the OPEN state. Now the transfer of
alarm and status messages is possible.
 */
 void MSAL1S_Started_Ind(
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
 void MSAL1S_Stopped_Ind(void);
/*
 * By means of this function the Alarm-State-Machine indicates to the
User that due to fatal errors the Alarm-State-Machine MSAL1S, the
cyclic State Machine MSCY1S and the acyclic State Machine MSAC1S
enter the Power-On state.
 */
 void MSAL1S_Fault_Ind(uint8_t ASM_Status //This parameter indicates the reason for the failure.		Possible values: DDLM_Fault, Set_Slave_Add, MSAC1S_Fault
		);

/*
 * By means of this function the RM State Machine indicates to the User
that due to fatal errors all MSAC_C2 and the RM State Machine have
been stopped.
 */
 void MSRM2S_Stopped_Ind(uint8_t Status//This parameter indicates the reason for the failure.Possible Values: MSRM2S_Fault, MSAC2S_Fault
		);
/*
 * By means of this function the RM State Machine indicates a fatal
error to the User.
 */
 void MSRM2S_Fault_Ind(uint8_t Status//This parameter indicates the reason for the failure.Possible Values: FDL_Fault, MSRM2S_Fault
		);

 int FMA12_Reset_Req(uint8_t MSAP0 // The parameter MSAP_0 (Management Service Access Point_0) specifies the service access point of the Local FMA1/2 User. This MSAP is activated and configured depending on the implementation.
		);

/*
 * By means of this service a read request for a data block is
transferred to a DPV1-Slave. The service operates slot and index
related.
 */
 int user_dpv1_read_req( uint8_t sap, DPV1_UNION_PDU_t* pdu );
/*
 * By means of this service a write request is transferred to a DPV1-
Slave.
 */
 int user_dpv1_write_req( uint8_t sap, DPV1_UNION_PDU_t* pdu );
/*
 * By means of this service an alarm message is transferred from the
DPV1-Slave to the DPV1-Master (Class 1). By means of the
subordinated service MSAC1_Alarm_Ack (see chapter 6.2.1) the
acknowledgement of the alarm will be transmitted from the master to
the slave.
 */
 void MSAL1_Alarm_Con(uint8_t isPosive,DPV1_UNION_PDU_t* pAlarmCon,uint8_t Status);
 int MSAC2_Initiate_Ind(
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
 int MSAC2_Abort_Ind(
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
int user_c2_data_transport_req( uint8_t sap, DPV1_UNION_PDU_t* pdu );


void usart_dma_config_RX(uint32_t rx_buffer,uint32_t size);


void usart_dma_config_TX(uint32_t tx_buffer,uint32_t size);
/*!
    \brief      configure USART DMA RX
    \param[in]  none
    \param[out] none
    \retval     none
*/
void usart_dma_config_RX(uint32_t rx_buffer,uint32_t size);
/*!
    \brief      cofigure the I2C0  as master
    \param[in]  none
    \param[out] none
    \retval     none
*/
void I2C_config(void);
/*!
    \brief      close the I2C0  as master
    \param[in]  none
    \param[out] none
    \retval     none
*/
void I2C_close(void);

void I2C_WDinit(void);
/**
  ***************************************************************************
  * @Brief	   : 从指定地址读出一个字节数据
  * @Parameter : [OUT] pByteData:  指向存储读出字节的数据区
  *              [IN]  AddrToRead：指定待读出地址
  * @Return	   : 无
  ***************************************************************************
  */
void I2C_ReadByte(uint8_t *pByteData, uint8_t AddrToRead);
/**
  ***************************************************************************
  * @Brief	   : 向指定地址写入一个字节数据
  * @Parameter : [IN] ByteData：   待写入的字节数据
  *				 [IN] AddrToWrite：待写入的地址
  * @Return	   : 无
  ***************************************************************************
  */
void I2C_WriteByte(uint8_t ByteData, uint8_t AddrToWrite);
/**
  ***************************************************************************
  * @Brief	   : 从指定地址读出指定长度的字节数据存放到指定区域
  * @Parameter : [IN]  AddrToRead： 指定待读地址
  *			 	 [OUT] pDataBuffer：指向存储读出数据的区域
  *				 [IN]  NumToRead：	指定待读出的字节数据长度
  * @Return	   : 无
  ***************************************************************************
  */
void I2C_ReadBuffer(uint8_t AddrToRead, uint8_t *pDataBuffer, uint16_t NumToRead);
/**
  ***************************************************************************
  * @Brief	   : 向指定地址写入指定长度的字节数据
  * @Parameter : [IN] AddrToWrite：指定待写入地址
  *				 [IN] pDataBuffer：指向待写入数据的区域
  *				 [IN] NumToWrite： 指定待写入字节数据的长度
  * @Return	   : 无
  ***************************************************************************
  */
void I2C_WriteBuffer(uint8_t AddrToWrite, uint8_t *pDataBuffer, uint16_t NumToWrite);


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

void phy_bps_detect_init(phy_bpsdvar_t ** pIntVar);

#endif /* SRC_DP_PORT_H_ */
