/*
 * dp_port.h
 *
 *  Created on: 2021��10��14��
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


#define DAC8740H          0  /* ����������Ƿ���DAC8740H */
#define TEST_DP_PORT      0
#define TEST_TIMER        0
/* �������š��˿� */
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

//���ʹ��Ӳ��RTS�����ε�����ĺ�
#define DP_RTS_ON()                     LL_GPIO_SetOutputPin(DPRTS_GPIO_Port, DPRTS_Pin)
#define DP_RTS_OFF()                    LL_GPIO_ResetOutputPin(DPRTS_GPIO_Port, DPRTS_Pin)

#define DP_PORT                        huart1


extern user_DP_call_t * pUserDPCall ;

void uartDMA_init(uint32_t baudrate);

void uartDMA_deinit(void);
/*----------------------------------------------
 * 2  DP Ӧ�ò���Ҫʵ�ֵĺ���
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
  * �ú�������DP ������ʱ��
  *
  */
 void eval_baseTimer_config(void);
/*
 * �ú�������DP У��������ݵ���Ч��
 * ��Ч����true������false
 */
 int usr_PrmData_Check(void*pPdu,uint8_t len);
/*
 * �ú�������DP У�����õ���Ч��
 * ��Ч����true������false
 */
 int usr_CfgData_Check(void*pToCfgData,uint8_t bCfgLength);

/*!  dp_updateNewOutPut
    \brief   fdl�յ��µ�������ݡ�
    �ú�����ʾ�û�������ȡ���µ��������
    \param[in]
      \arg
      \arg
    \param[out]
      \arg
    \retval none
*/
 void dp_updateNewOutPut(uint8_t* pOutput,uint8_t len);
/*
 * �ú������ڷ������ݵ�����˿���
 */
 void fdl_PortSend(fdl_frame_t* pFrame);

/*!  fdl_PortSendFinish
    \brief      ֡������ɵ��á�
    ��ص��øú���

    \param[in]
      \arg
      \arg
    \param[out]
    \retval

*/
void fdl_PortSendFinish(void* pSendFrame);
/*
 * �ú�����������MinTsdr
 */
 int setMinTsdr(uint16_t  MinTsdr);

/*
 * �ú������ڷ�����ʵ������
 */
 void  dp_Setup_Real_Cfg_Data(dp_CFG_t* pCfg);

/*
 * �ú������ڷ�����ʵ���������
 */
 void  dp_Setup_Real_Output_len(uint8_t* pLen);
/*
 * �ú������ڷ�����ʵ�����볤��
 */
 void  dp_Setup_Real_Input_len(uint8_t* pLen);

/*
 * �ú������ڷ�����ʵ�ĵ�ַ����
 */
 void  dp_Setup_Real_No_Add_Chg(uint8_t* pchg);

/*
 * �ú������ڷ�����ʵ��Identify
 */
 void  dp_Setup_Real_Ident(uint16_t* pIdent);

/*
 * �ú���������ʾ�û��洢�µĴ�վ��ַ
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
  * @Brief	   : ��ָ����ַ����һ���ֽ�����
  * @Parameter : [OUT] pByteData:  ָ��洢�����ֽڵ�������
  *              [IN]  AddrToRead��ָ����������ַ
  * @Return	   : ��
  ***************************************************************************
  */
void I2C_ReadByte(uint8_t *pByteData, uint8_t AddrToRead);
/**
  ***************************************************************************
  * @Brief	   : ��ָ����ַд��һ���ֽ�����
  * @Parameter : [IN] ByteData��   ��д����ֽ�����
  *				 [IN] AddrToWrite����д��ĵ�ַ
  * @Return	   : ��
  ***************************************************************************
  */
void I2C_WriteByte(uint8_t ByteData, uint8_t AddrToWrite);
/**
  ***************************************************************************
  * @Brief	   : ��ָ����ַ����ָ�����ȵ��ֽ����ݴ�ŵ�ָ������
  * @Parameter : [IN]  AddrToRead�� ָ��������ַ
  *			 	 [OUT] pDataBuffer��ָ��洢�������ݵ�����
  *				 [IN]  NumToRead��	ָ�����������ֽ����ݳ���
  * @Return	   : ��
  ***************************************************************************
  */
void I2C_ReadBuffer(uint8_t AddrToRead, uint8_t *pDataBuffer, uint16_t NumToRead);
/**
  ***************************************************************************
  * @Brief	   : ��ָ����ַд��ָ�����ȵ��ֽ�����
  * @Parameter : [IN] AddrToWrite��ָ����д���ַ
  *				 [IN] pDataBuffer��ָ���д�����ݵ�����
  *				 [IN] NumToWrite�� ָ����д���ֽ����ݵĳ���
  * @Return	   : ��
  ***************************************************************************
  */
void I2C_WriteBuffer(uint8_t AddrToWrite, uint8_t *pDataBuffer, uint16_t NumToWrite);


/*
 * ����������ӦӲ����ʼ��
 *
 * �����������׼��
 *
 * 1����ʼ��һ������ģʽtimer,timer�ļ���ʱ�Ӿ����ܴ�
 * 2����DP UART�ӿ�����Ϊ����ģʽ
 * 3����DP UART�ӿڵ�RX������ΪGPIO ����ģʽ
 * 4��ʵ�ֱ�Ҫ�ĺ���
 */

void phy_bps_detect_init(phy_bpsdvar_t ** pIntVar);

#endif /* SRC_DP_PORT_H_ */
