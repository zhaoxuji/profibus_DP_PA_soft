/*
 * dpv0.c
 *
 *  Created on:
 *      Author: fred
 */

#include "dp_port.h"
#include "dp.h"
#include "dp_App.h"
#include "ds18b20.h"

#if !PA_PROFILE_ENABLE
user_opt_t  user;

#ifdef DPV1_IM_SUPP
const uint8_t IM_DEFAULT[64] = {     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Header, 10 Octets
                                        0x01, 0xBC,                                                 // MANUFACTURER_ID, 2 Octets
                                        0x50, 0x41, 0x30, 0x30, 0x36, 0x33, 0x30, 0x30, 0x20, 0x20, // Order ID, 20 Octets, PA006300
                                        0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                                        0x30, 0x30, 0x30, 0x31, 0x31, 0x32, 0x20, 0x20, 0x20, 0x20, // SerialNumber, 16 Octets, 000112
                                        0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                                        0x00, 0x01,                                                 // HardwareRevision, 2 Octets, 1
                                        0x56, 0x05, 0x00, 0x04,                                     // SoftwareRevision, 4 Octets, V504
                                        0x00, 0x00,                                                 // RevisionCounter
                                        0x00, 0x00,                                                 // PROFILE ID, 2 Octets
                                        0x00, 0x00,                                                 // PROFILE SPECIFIC TYPE, 2 Octets
                                        0x01, 0x01,                                                 // IM Version, 2 Octets
                                        0x00, 0x01                                                  // IM Supported, 2 Octets
                                 };

	#ifdef IM1_SUPP
		ROMCONST__ uint8_t IM1_DEFAULT[64] = {	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Header, 10 Octets
                                        		0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, // TagFunction
                                        		0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                                        		0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                                        		0x20, 0x20,
                                        		0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, // TagLocation
                                        		0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                                        		0x20, 0x20
                                           };
	#endif//#ifdef IM1_SUPP

	#ifdef IM2_SUPP
		ROMCONST__ uint8_t IM2_DEFAULT[64] = {	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Header, 10 Octets
                                        		0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, // InstallationDate
                                        		0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                                        		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Reserved
                                        		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                        		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                        		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                                           };
	#endif//#ifdef IM2_SUPP

	#ifdef IM3_SUPP
		ROMCONST__ uint8_t IM3_DEFAULT[64] = {	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Header, 10 Octets
                                        		0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, // Descriptor
                                        		0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                                        		0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                                        		0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                                        		0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                                        		0x20, 0x20, 0x20, 0x20
                                           };
	#endif//#ifdef IM3_SUPP

	#ifdef IM4_SUPP
		ROMCONST__ uint8_t IM4_DEFAULT[64] = {	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Header, 10 Octets
                                        		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Signature
                                        		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                        		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                        		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                        		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                        		0x00, 0x00, 0x00, 0x00
                                           };
	#endif//#ifdef IM4_SUPP
#endif//#ifdef DPV1_IM_SUPP

const user_DP_callback_t userDPCallBack = {
		.setPAmode = false, //true: PA模式FDL层；false: DP模式
		.fdl_PortSend = fdl_PortSend,
		.FMA12_SetValue_Req = FMA12_SetValue_Req,
		.FMA12_Reset_Req = FMA12_Reset_Req,
		.dp_baseTimer_config = eval_baseTimer_config,
        .setMinTsdr = setMinTsdr,
		.dp_setNewSlaveAddr = dp_setNewSlaveAddr,
        .usr_PrmData_Check = usr_PrmData_Check,
        .usr_CfgData_Check = usr_CfgData_Check,
		.dp_updateNewOutPut = dp_updateNewOutPut,
        .dp_Setup_Real_Cfg_Data  = dp_Setup_Real_Cfg_Data,
        .dp_Setup_Real_Output_len = dp_Setup_Real_Output_len,
        .dp_Setup_Real_Input_len = dp_Setup_Real_Input_len,
        .dp_Setup_Real_No_Add_Chg = dp_Setup_Real_No_Add_Chg,
        .dp_Setup_Real_Ident = dp_Setup_Real_Ident,
        .MSAL1S_Abort_Ind = MSAL1S_Abort_Ind,
        .MSAL1S_Started_Ind = MSAL1S_Started_Ind,
        .MSAL1S_Stopped_Ind = MSAL1S_Stopped_Ind,
        .MSAL1S_Fault_Ind = MSAL1S_Fault_Ind,
		.MSAL1_Alarm_Con = MSAL1_Alarm_Con,
        .MSRM2S_Stopped_Ind = MSRM2S_Stopped_Ind,
        .MSRM2S_Fault_Ind = MSRM2S_Fault_Ind,
        .user_dpv1_read_req = user_dpv1_read_req,
	    .user_dpv1_write_req = user_dpv1_write_req,
		.user_c2_data_transport_req = user_c2_data_transport_req,
        .MSAC2_Initiate_Ind = MSAC2_Initiate_Ind,
        .MSAC2_Abort_Ind = MSAC2_Abort_Ind,
		.I2C_Open = I2C_config,
		.I2C_ReadByte = I2C_ReadByte,
	    .I2C_WriteByte = I2C_WriteByte,
	    .I2C_Read = I2C_ReadBuffer,
	    .I2C_Write = I2C_WriteBuffer,
		.I2C_Close = I2C_close,
		.I2C_WDinit = I2C_WDinit,
	    .regGPIOSDAPort = GPIOB,//返回SCL引脚端口寄存器地址
	    .regGPIOSDAPin = LL_GPIO_PIN_9,//返回SCL引脚
		.phy_bps_detect_init = phy_bps_detect_init,
		.userWatchDogms = 4000 //要和主站组态进行匹配，不可过小
};

user_DP_call_t * pUserDPCall = NULL;
/*---------------------------------------------------------------------------*/
/* function: DpPrm_ChkDpv1StatusBytes                                        */
/*---------------------------------------------------------------------------*/
/**
 * @brief Check DP-V1 status bytes.
 *
 * Some rules are defined in IEC61158-6 ( summery RulesCheckingPrmData.pdf - <a href="RulesCheckingPrmData.pdf">click here ).</a>
 *
 * @attention In DP-V0 mode all bytes are fix to 0x00!
 *
 * @param[in]bDpv1Status1    DP-V1 status byte 1
 * @param[in]bDpv1Status2    DP-V1 status byte 2
 * @param[in]bDpv1Status3    DP-V1 status byte 3
 * @return DP_OK - The check of DP-V1 status bytes is OK.
 * @return DP_PRM_DPV1_STATUS - The check of DP-V1 status bytes isn't OK. @see DP_ERROR_CODE
 */
static int DpPrm_ChkDpv1StatusBytes( uint8_t bDpv1Status1, uint8_t bDpv1Status2, uint8_t bDpv1Status3 )
{
DP_ERROR_CODE eRetValue;

   eRetValue = DP_OK;

   //DPV0-Mode
   if(    (( bDpv1Status1 & 0xF8 ) != 0x00 )
       || (( bDpv1Status2 & 0xFF ) != 0x00 )
       || (( bDpv1Status3 & 0xFF ) != 0x00 )
     )
   {
      eRetValue = DP_PRM_DPV1_STATUS;
   }//if(    (( bDpv1Status1 & 0xF8 ) != 0x00 ) ...

   return eRetValue;
}//static DP_ERROR_CODE DpPrm_ChkDpv1StatusBytes( uint8_t bDpv1Status1, uint8_t bDpv1Status2, uint8_t bDpv1Status3 )

/*---------------------------------------------------------------------------*/
/* function: DpPrm_ChkNewPrmData                                             */
/*---------------------------------------------------------------------------*/
/**
 * @brief Checking parameter data.
 * The user has to program the function for checking the received parameter data.
 *
 * @param[in] pbPrmData - address of parameter data
 * @param[in] bPrmLength - length of parameter data
 *
 * @return DP_OK - The transferred parameterization is OK.
 * @return DP_NOK - The transferred parameterization isn't OK.
 */

DP_ERROR_CODE DpPrm_ChkNewPrmData( uint8_t* pbPrmData, uint8_t bPrmLength )
{
	dp_sPRM_t * psToPrmData;
   DP_ERROR_CODE     eRetValue;



   eRetValue = DP_OK;

   if( bPrmLength == PRM_LEN_DPV1 + sizeof(user_prm_t) )
   {
	  eRetValue = DpPrm_ChkDpv1StatusBytes( psToPrmData->bStationState.b, psToPrmData->bDpv1Status2.b, psToPrmData->bDpv1Status3.b );
      psToPrmData = ( dp_sPRM_t * )pbPrmData;
      user_prm_t UserPrm;
      UserPrm.upper_limit = (psToPrmData->bUserPrmData[0] << 8) + psToPrmData->bUserPrmData[1];
      UserPrm.lower_limit = (psToPrmData->bUserPrmData[2] << 8) + psToPrmData->bUserPrmData[3];
      UserPrm.control.enable.b = psToPrmData->bUserPrmData[4];
      if(eRetValue == DP_PRM_DPV1_STATUS && UserPrm.lower_limit >= -4000 && UserPrm.upper_limit <= 8000)
      {
		user.prm = UserPrm;
		eRetValue = DP_OK;
      }
   }//if( bPrmLength == PRM_LEN_DPV1 )
   else
   {
      eRetValue = DP_PRM_LEN_ERROR;
   }//else of if( bPrmLength == PRM_LEN_DPV1 )

   return eRetValue;
}//DP_ERROR_CODE DpPrm_ChkNewPrmData( MEM_UNSIGNED8_PTR pbPrmData, uint8_t bPrmLength )

/*---------------------------------------------------------------------------*/
 /* function: dp_CalculateInpOutpLength                                     */
 /*---------------------------------------------------------------------------*/
 /*!
    \brief Calculation of input- and output data.

    \param[in] pToCfgData - address of configuration data
    \param[in] bCfgLength - length of configuration data

    \retval DP_OK - Initialization OK
    \retval DP_CFG_LEN_ERROR - Error with CFG length
    \retval DP_CALCULATE_IO_ERROR - Error with DIN or DOUT length
    \retval DP_CFG_FORMAT_ERROR - Error in special configuration format
 */
 DP_ERROR_CODE dpCalculateInpOutpLength( uint8_t* pToCfgData, uint8_t bCfgLength,uint8_t* pRealInputLen,uint8_t* pRealOutputLen )
 {
 DP_ERROR_CODE bError;
 uint8_t       bSpecificDataLength;
 uint8_t       bTempOutputDataLength;
 uint8_t       bTempInputDataLength;
 uint8_t       bLength;
 uint8_t       bCount;

    bError = DP_OK;
    bTempOutputDataLength = 0;
    bTempInputDataLength  = 0;

    if( ( bCfgLength > 0 ) && ( bCfgLength <= CFG_BUFSIZE ) )
    {
       for( ; bCfgLength > 0; bCfgLength -= bCount )
       {
          bCount = 0;

          if( *pToCfgData & CFG_IS_BYTE_FORMAT )
          {
             /* general identifier format */
             bCount++;
             /* pToCfgData points to "Configurationbyte", CFG_BF means "CFG_IS_BYTE_FORMAT" */
             bLength = (uint8_t)( ( *pToCfgData & CFG_BF_LENGTH) + 1 );

             if( *pToCfgData & CFG_BF_OUTP_EXIST )
             {
                bTempOutputDataLength += ( *pToCfgData & CFG_LENGTH_IS_WORD_FORMAT ) ? ( 2 * bLength ) : bLength;
             }

             if( *pToCfgData & CFG_BF_INP_EXIST )
             {
                bTempInputDataLength += ( *pToCfgData & CFG_LENGTH_IS_WORD_FORMAT ) ? ( 2 * bLength ) : bLength;
             }

             pToCfgData++;
          }
          else
          {
             /* pToCfgData points to the headerbyte of "special identifier format */
             /* CFG_SF means "CFG_IS_SPECIAL_FORMAT" */
             if( *pToCfgData & CFG_SF_OUTP_EXIST )
             {
                bCount++;    /* next byte contains the length of outp_data */
                bLength = (uint8_t)( ( *( pToCfgData + bCount ) & CFG_SF_LENGTH ) + 1 );

                bTempOutputDataLength += ( *( pToCfgData + bCount ) & CFG_LENGTH_IS_WORD_FORMAT ) ? ( 2 * bLength ) : bLength;
             }

             if( *pToCfgData & CFG_SF_INP_EXIST )
             {
                bCount++;  /* next byte contains the length of inp_data */
                bLength = (uint8_t)( ( *( pToCfgData + bCount ) & CFG_SF_LENGTH ) + 1 );

                bTempInputDataLength += ( *( pToCfgData + bCount ) & CFG_LENGTH_IS_WORD_FORMAT ) ? ( 2 * bLength ) : bLength;
             }

             bSpecificDataLength = (uint8_t)( *pToCfgData & CFG_BF_LENGTH );

             if( bSpecificDataLength != 15 )
             {
                bCount = (uint8_t)( bCount + 1 + bSpecificDataLength );
                pToCfgData = pToCfgData + bCount;
             } /* if( bSpecificDataLength != 15 ) */
             else
             {
                /* specific data length = 15 not allowed */
            	 *pRealInputLen  = 0xFF;
            	 *pRealOutputLen = 0xFF;
                bError = DP_CFG_FORMAT_ERROR;
             } /* else of if( bSpecificDataLength != 15 ) */
          }
       } /* for( ; bCfgLength > 0; bCfgLength -= bCount ) */

       if( ( bCfgLength != 0 ) || ( bTempInputDataLength > DIN_BUFSIZE ) || ( bTempOutputDataLength > DOUT_BUFSIZE ) )
       {
    	   *pRealInputLen  = 0xFF;
    	   *pRealOutputLen = 0xFF;

          bError = DP_CALCULATE_IO_ERROR;
       } /* if( ( bCfgLength != 0 ) || ( bTempInputDataLength > DIN_BUFSIZE ) || ( bTempOutputDataLength > DOUT_BUFSIZE ) ) */
       else
       {
    	   *pRealInputLen  = bTempInputDataLength;
    	   *pRealOutputLen = bTempOutputDataLength;
          bError = DP_OK;
       } /* else of if( ( bCfgLength != 0 ) || ( bTempInputDataLength > DIN_BUFSIZE ) || ( bTempOutputDataLength > DOUT_BUFSIZE ) ) */
    } /* if( ( bCfgLength > 0 ) && ( bCfgLength <= CFG_BUFSIZE ) ) */
    else
    {
    	*pRealInputLen  = 0xFF;
    	*pRealOutputLen = 0xFF;
       bError = DP_CFG_LEN_ERROR;
    } /* else of if( ( bCfgLength > 0 ) && ( bCfgLength <= CFG_BUFSIZE ) ) */

    return bError;
 } /* DP_ERROR_CODE dpCalculateInpOutpLength( MEM_UNSIGNED8_PTR pToCfgData, uint8_t bCfgLength,dpSlave_t* pDpSlave  ) */

 void dp_Init(void)
 {
	   /*
	    * step 3
	    */
	   LsduLength_list_t AcycLengths;
	   AcycLengths.bMaxLsduLengthIndHigh = 244;
	   AcycLengths.bMaxLsduLengthIndLow = 244;
	   AcycLengths.bMaxLsduLengthReqHigh = 244;
	   AcycLengths.bMaxLsduLengthReqLow = 244;
	   pUserDPCall->MSCY1S_Enabel(true,true,true);
	   pUserDPCall->MSAL1S_Init_Req(&AcycLengths,
			   SEQC_MODE_OFF,
			   244,
			   false,
			   NULL,
			   DIAG_BUFSIZE,
			   PRM_BUFSIZE,
			   CFG_BUFSIZE,
			   DIN_BUFSIZE,
			   DOUT_BUFSIZE,
			   SSA_BUFSIZE);
	   /*
	    * step 4
	    */
	   pUserDPCall->MSRM2S_Reset_Req();
	   //RM_Registery_t RmReg;
	   //resSAPList_t ResSAPList;
	   pUserDPCall->MSRM2S_Init_Req(
	   		//&RmReg,//The parameter RM_Registry contains a list of RM_Registry_Entry	(List_of_RM_Registry_Entries).
	   		//&ResSAPList,//The parameter Res_SAP_List contains the FDL-SAPs available for	MSAC_C2 communication relationships.
	   		255//This parameter defines the minimum Send_Timeout supported in this station.
	   		);

 }
 typedef void* (*fdl_Init_ft)(const user_DP_callback_t * const pCallBack) ;
#if __RELEASE__
 void* fdl_Init(const user_DP_callback_t * const pCallBack)  ;
 volatile fdl_Init_ft pFDL_Init = fdl_Init;
#else
 void* fdl_Init(const user_DP_callback_t * const pCallBack)  ;
 volatile fdl_Init_ft pFDL_Init = fdl_Init;
 //volatile fdl_Init_ft pFDL_Init = (fdl_Init_ft)(0x08000000+100*1024 + 1);//thumb must +1
#endif
void dp_InitAll(uint8_t devID)
{
   /*
    * step 1
    */
   pUserDPCall = pFDL_Init(&userDPCallBack);

   /*
    * step 2
    */
   pUserDPCall->DDLM_Reset_Req();
   while(1)
   {
	   if( 0 == pUserDPCall->DDLM_Start_Req(devID, 45450, false))
	   {
		   /*
		    * step 3
		    */
		   pUserDPCall->MSAL1S_GoPowerOn_Req();

		   dp_Init();

		   /*
		    * step 5 endless loop call
		    * todo:
		    */
		   //fdl_StateProcess();
		   break;
	   }
   }


}


/*----------------------------------------------
 * 2  DP 应该实现的函数
 *
 * ---------------------------------------------
 */

/*
 * 该函数用于DP 校验参数数据的有效性
 * 有效返回true，否则false
 */
int usr_PrmData_Check(void*pPdu,uint8_t len)
{
	DpPrm_ChkNewPrmData(pPdu,len);
	return true;
}
/*
 * 该函数用于DP 校验配置的有效性
 * 有效返回true，否则false
 */
int usr_CfgData_Check(void*pToCfgData,uint8_t bCfgLength)
{
	uint8_t realInput,realOutput;
	dpCalculateInpOutpLength(pToCfgData, bCfgLength,&realInput,&realOutput);
	if( realInput == DIN_BUFSIZE && realOutput == DOUT_BUFSIZE)
		return true;
	return false;
}

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
void dp_updateNewOutPut(uint8_t* pOutput,uint8_t len)
{
	if(len == DOUT_BUFSIZE)
	{
		if(user.output.ctrl.enable.b != *pOutput)
		{
			HAL_GPIO_WritePin(LED_DEX_GPIO_Port, LED_DEX_Pin, GPIO_PIN_SET);
			/*
			 * 输出数据
			 * 此处处理控制命令
			 */
			user.output.ctrl.enable.b = *pOutput;
			if(user.output.ctrl.enable.sbit.cool)
			{
				HAL_GPIO_WritePin(COOLSET_GPIO_Port, COOLSET_Pin, GPIO_PIN_SET);//指示制冷
			} else {
				HAL_GPIO_WritePin(COOLSET_GPIO_Port, COOLSET_Pin, GPIO_PIN_RESET);
			}
			if(user.output.ctrl.enable.sbit.hot)
			{
				HAL_GPIO_WritePin(HOTSET_GPIO_Port, HOTSET_Pin, GPIO_PIN_SET);//指示加热
			} else {
				HAL_GPIO_WritePin(HOTSET_GPIO_Port, HOTSET_Pin, GPIO_PIN_RESET);
			}
			//_toUser_("OUTPUT: %02x . hot enable: %d . cool enable: %d",user.output.ctrl,user.output.ctrl.enable.sbit.hot,user.output.ctrl.enable.sbit.cool);
		}
	}

}

/*
 * 该函数用于返回真实的配置
 */
void  dp_Setup_Real_Cfg_Data(dp_CFG_t* pCfg)
{
	pCfg->bLength = CFG_BUFSIZE;
	pCfg->abData[0] = 0x50;
	pCfg->abData[1] = 0x10;
	pCfg->abData[2] = 0x20;
}

/*
 * 该函数用于返回真实的输出长度
 */
void  dp_Setup_Real_Output_len(uint8_t* pLen)
{
  *pLen = DOUT_BUFSIZE;
}
/*
 * 该函数用于返回真实的输入长度
 */
void  dp_Setup_Real_Input_len(uint8_t* pLen)
{
	*pLen = DIN_BUFSIZE;
}

/*
 * 该函数用于返回真实的地址设置
 */
void  dp_Setup_Real_No_Add_Chg(uint8_t* pchg)
{

}

/*
 * 该函数用于返回真实的Identify
 */
void  dp_Setup_Real_Ident(uint16_t* pIdent)
{
    *pIdent = IDENT_NR;
}

/*
 * 该函数用于提示用户存储新的从站地址
 */
void  dp_setNewSlaveAddr(const dp_SSA_BLOCK_t * const pNewSSA)
{

}

/*--------------------------------------------------------------------------*/
/* function: user_dpv1_read_req (called by DPV1)                            */
/*--------------------------------------------------------------------------*/
int user_dpv1_read_req( uint8_t sap, DPV1_UNION_PDU_t* pdu )
{
DP_ERROR_CODE    ret_value;
uint8_t           max_length;

#ifdef DPV1_IM_SUPP
    psIMCALL* ps_ImCall;
#endif//#ifdef DPV1_IM_SUPP

    ret_value = DP_OK;

    #ifdef DPV1_IM_SUPP
        if( pdu->read.index == 255 )
        {
            if( pdu->read.slot_num == 0 )
            {
                	//check_extended function number, reserved byte and IM_INDEX
                	if( user.awImIndex[ ( sap & 0x0F ) ] == IM0_INDEX_0 )
                	{
                	    ps_ImCall = (psIMCALL*)&pdu->read.pdu_data[0];
                	    ps_ImCall->bExtendedFunctionNumber = IM_FN_CALL;
                	    ps_ImCall->bReserved = 0x00;
                	    ps_ImCall->wIndex = user.awImIndex[ ( sap & 0x0F ) ];

                	    memcpy( &pdu->read.pdu_data[4], &user.sIM_0.abHeader[0], cSizeOfIM0 );
                	    if( pdu->read.length > (4 + cSizeOfIM0) )
                	    {
                	        pdu->read.length = 4 + cSizeOfIM0;
                	    }
                	}//if( user.awImIndex[ ( sap & 0x0F ) ] == IM0_INDEX_0 )
    			#ifdef IM1_SUPP
                	//check_extended function number, reserved byte and IM_INDEX
                	else
                	if( user.awImIndex[ ( sap & 0x0F ) ] == IM0_INDEX_1 )
                	{
                	    ps_ImCall = (psIMCALL)&pdu->read.pdu_data[0];
                	    ps_ImCall->bExtendedFunctionNumber = IM_FN_CALL;
                	    ps_ImCall->bReserved = 0x00;
                	    ps_ImCall->wIndex = user.awImIndex[ ( sap & 0x0F ) ];

                	    memcpy( &pdu->read.pdu_data[4], &user.sIM_1.abHeader[0], SizeOfIM1 );
                	    if( pdu->read.length > (4 + SizeOfIM1) )
                	    {
                	        pdu->read.length = 4 + SizeOfIM1;
                	    }
                	}//if( user.awImIndex[ ( sap & 0x0F ) ] == IM0_INDEX_1 )
    			#endif//#ifdef IM1_SUPP

    			#ifdef IM2_SUPP
                	//check_extended function number, reserved byte and IM_INDEX
                	else
                	if( user.awImIndex[ ( sap & 0x0F ) ] == IM0_INDEX_2 )
                	{
                	    ps_ImCall = (psIMCALL)&pdu->read.pdu_data[0];
                	    ps_ImCall->bExtendedFunctionNumber = IM_FN_CALL;
                	    ps_ImCall->bReserved = 0x00;
                	    ps_ImCall->wIndex = user.awImIndex[ ( sap & 0x0F ) ];

                	    memcpy( &pdu->read.pdu_data[4], &user.sIM_2.abHeader[0], SizeOfIM2 );
                	    if( pdu->read.length > (4 + SizeOfIM2) )
                	    {
                	        pdu->read.length = 4 + SizeOfIM2;
                	    }
                	}//if( user.awImIndex[ ( sap & 0x0F ) ] == IM0_INDEX_2 )
    			#endif//#ifdef IM2_SUPP

    			#ifdef IM3_SUPP
                	//check_extended function number, reserved byte and IM_INDEX
                	else
                	if( user.awImIndex[ ( sap & 0x0F ) ] == IM0_INDEX_3 )
                	{
                	    ps_ImCall = (psIMCALL)&pdu->read.pdu_data[0];
                	    ps_ImCall->bExtendedFunctionNumber = IM_FN_CALL;
                	    ps_ImCall->bReserved = 0x00;
                	    ps_ImCall->wIndex = user.awImIndex[ ( sap & 0x0F ) ];

                	    memcpy( &pdu->read.pdu_data[4], &user.sIM_3.abHeader[0], SizeOfIM3 );
                	    if( pdu->read.length > (4 + SizeOfIM3) )
                	    {
                	        pdu->read.length = 4 + SizeOfIM3;
                	    }
                	}//if( user.awImIndex[ ( sap & 0x0F ) ] == IM0_INDEX_3 )
    			#endif//#ifdef IM3_SUPP

    			#ifdef IM4_SUPP
                	//check_extended function number, reserved byte and IM_INDEX
                	else
                	if( user.awImIndex[ ( sap & 0x0F ) ] == IM0_INDEX_4 )
                	{
                	    ps_ImCall = (psIMCALL)&pdu->read.pdu_data[0];
                	    ps_ImCall->bExtendedFunctionNumber = IM_FN_CALL;
                	    ps_ImCall->bReserved = 0x00;
                	    ps_ImCall->wIndex = user.awImIndex[ ( sap & 0x0F ) ];

                	    memcpy( &pdu->read.pdu_data[4], &user.sIM_4.abHeader[0], SizeOfIM4 );
                	    if( pdu->read.length > (4 + SizeOfIM4) )
                	    {
                	        pdu->read.length = 4 + SizeOfIM4;
                	    }
                	}//if( user.awImIndex[ ( sap & 0x0F ) ] == IM0_INDEX_4 )
    			#endif//#ifdef IM4_SUPP
                	else
                	{
                	    //BugFix 503
                		pdu->negRes.err_decode = DPV1_ERRDC_DPV1;
                	    pdu->negRes.err_code1 = DPV1_ERRCL_ACCESS | DPV1_ERRCL_ACC_STATE;
                	    pdu->negRes.err_code2 = 0x00;
                	    ret_value = DP_NOK;
                	}//else of if( user.awImIndex[ ( sap & 0x0F ) ] == IM0_INDEX_0 )
            }//if( pdu->read.slot_num == 0 )
            else
            {
            	pdu->negRes.err_decode = DPV1_ERRDC_DPV1;
                pdu->negRes.err_code1 = DPV1_ERRCL_ACCESS | DPV1_ERRCL_ACC_INV_SLOT;
                pdu->negRes.err_code2 = 0x00;
                ret_value = DP_NOK;
            }//else of if( pdu->read.slot_num == 0 )
        }
    #else
        if( pdu->read.index == 255 )
        {
        	pdu->negRes.err_decode = DPV1_ERRDC_DPV1;
            pdu->negRes.err_code1 = DPV1_ERRCL_ACCESS | DPV1_ERRCL_ACC_INV_INDEX;
            pdu->negRes.err_code2 = 0x00;
            ret_value = DP_NOK;
        }
    #endif//#ifdef DPV1_IM_SUPP

        else
        if( ret_value == DP_OK )
        {
            switch( pdu->read.slot_num )
            {
                case SLOT_TEMPLIMIT:
                {
                	if(pdu->read.index == INDEX_TEMPLIMIT_UPPER)
                	{
                        pdu->read.pdu_data[0] = (user.prm.upper_limit >> 8) & 0xff;
                        pdu->read.pdu_data[1] = (user.prm.upper_limit >> 0) & 0xff;
                        max_length = 2;
                	}  else if(pdu->read.index == INDEX_TEMPLIMIT_LOWER)
                	{
                        pdu->read.pdu_data[0] = (user.prm.lower_limit >> 8) & 0xff;
                        pdu->read.pdu_data[1] = (user.prm.lower_limit >> 0) & 0xff;
                        max_length = 2;
                	}
                	else {
                        pdu->negRes.err_code1 = DPV1_ERRCL_ACCESS | DPV1_ERRCL_ACC_INV_INDEX;
                        pdu->negRes.err_code2 = 0x00;
                        ret_value = DP_NOK;
                	}
                    break;
                }

                case SLOT_CONTROL:
                {
                	if(pdu->read.index == INDEX_CONTROL)
                	{
                        pdu->read.pdu_data[0] = user.prm.control.enable.b;
                        max_length = 1;
                	} else {
                		pdu->negRes.err_decode = DPV1_ERRDC_DPV1;
                        pdu->negRes.err_code1 = DPV1_ERRCL_ACCESS | DPV1_ERRCL_ACC_INV_INDEX;
                        pdu->negRes.err_code2 = 0x00;
                        ret_value = DP_NOK;
                	}
                    break;
                }
                default:
                {
                	pdu->negRes.err_decode = DPV1_ERRDC_DPV1;
                    pdu->negRes.err_code1 = DPV1_ERRCL_ACCESS | DPV1_ERRCL_ACC_INV_SLOT;
                    pdu->negRes.err_code2 = 0x00;
                    ret_value = DP_NOK;
                    break;
                }

            }//switch( pdu->read.slot_num )
			if( pdu->read.length > max_length )
			{
				pdu->read.length = max_length;
			}
        }//else if(  ret_value == DP_OK  )


    return ret_value;

}//DP_ERROR_CODE user_dpv1_read_req( uint8_t sap, DPV1_UNION_PDU_t* pdu )

/*--------------------------------------------------------------------------*/
/* function: user_dpv1_write_req (called by DPV1)                           */
/*--------------------------------------------------------------------------*/
int user_dpv1_write_req( uint8_t sap, DPV1_UNION_PDU_t* pdu )
{
DP_ERROR_CODE ret_value;
uint8_t        max_length;
#ifdef DPV1_IM_SUPP
    psIMCALL* ps_ImCall;
#endif//#ifdef DPV1_IM_SUPP

    ret_value = DP_OK;

    #ifdef DPV1_IM_SUPP
        if( pdu->write.index == 255 )
        {
            if( pdu->write.slot_num == 0 )
            {
                //check_extended function number, reserved byte and IM_INDEX
                ps_ImCall = (psIMCALL*)&pdu->write.pdu_data[0];
                if(    ( ps_ImCall->bExtendedFunctionNumber == IM_FN_CALL )
                    && ( ps_ImCall->bReserved == 0x00                     )
                  )
                {
                    //BugFix 503
                	//check length
                	if( pdu->write.length == 0x04 )
                	{
                			//check IM_INDEX
                    		if( ps_ImCall->wIndex == IM0_INDEX_0 )
                    		{
                    	    	//OK, save IM_INDEX
                    	    	user.awImIndex[ ( sap & 0x0F ) ] = IM0_INDEX_0;
                    		}//if( ps_ImCall->wIndex == IM0_INDEX_0 )

    					#ifdef IM1_SUPP
                			//check IM_INDEX
							else
                    		if( ps_ImCall->wIndex == IM0_INDEX_1 )
                    		{
                    	    	//OK, save IM_INDEX
                    	    	user.awImIndex[ ( sap & 0x0F ) ] = IM0_INDEX_1;
                    		}//if( ps_ImCall->wIndex == IM0_INDEX_1 )
    					#endif//#ifdef IM1_SUPP

    					#ifdef IM2_SUPP
							else
                    		if( ps_ImCall->wIndex == IM0_INDEX_2 )
                    		{
                    	    	//OK, save IM_INDEX
                    	    	user.awImIndex[ ( sap & 0x0F ) ] = IM0_INDEX_2;
                    		}//if( ps_ImCall->wIndex == IM0_INDEX_2 )
    					#endif//#ifdef IM2_SUPP

    					#ifdef IM3_SUPP
							else
                    		if( ps_ImCall->wIndex == IM0_INDEX_3 )
                    		{
                    	    	//OK, save IM_INDEX
                    	    	user.awImIndex[ ( sap & 0x0F ) ] = IM0_INDEX_3;
                    		}//if( ps_ImCall->wIndex == IM0_INDEX_3 )
    					#endif//#ifdef IM3_SUPP

    					#ifdef IM4_SUPP
							else
                    		if( ps_ImCall->wIndex == IM0_INDEX_4 )
                    		{
                    	    	//OK, save IM_INDEX
                    	    	user.awImIndex[ ( sap & 0x0F ) ] = IM0_INDEX_4;
                    		}//if( ps_ImCall->wIndex == IM0_INDEX_4 )
    					#endif//#ifdef IM4_SUPP
                    		else
                    		{
                    			pdu->negRes.err_decode = DPV1_ERRDC_DPV1;
                    		    pdu->negRes.err_code1 = DPV1_ERRCL_ACCESS | DPV1_ERRCL_ACC_INV_INDEX;
                    		    pdu->negRes.err_code2 = 0x00;
                    		    ret_value = DP_NOK;
                    		}//else of if( ps_ImCall->wIndex == IM0_INDEX_0 )
                    }//if( pdu->write.length == 0x04 )

                	else
                	if( pdu->write.length == 0x44 )
                	{
    					#ifdef IM1_SUPP
                			//check IM_INDEX
                    		if( ps_ImCall->wIndex == IM0_INDEX_1 )
                    		{
                    	    	//OK, save IM_INDEX
                    	    	user.awImIndex[ ( sap & 0x0F ) ] = IM0_INDEX_1;
                	    		memcpy( &user.sIM_1.abHeader[0], &pdu->write.pdu_data[4], SizeOfIM1 );
								user.sIM_0.wRevCounter++;
                    		}//if( ps_ImCall->wIndex == IM0_INDEX_1 )
							else
    					#endif//#ifdef IM1_SUPP

    					#ifdef IM2_SUPP
                    		if( ps_ImCall->wIndex == IM0_INDEX_2 )
                    		{
                    	    	//OK, save IM_INDEX
                    	    	user.awImIndex[ ( sap & 0x0F ) ] = IM0_INDEX_2;
                	    		memcpy( &user.sIM_2.abHeader[0], &pdu->write.pdu_data[4], SizeOfIM2 );
								user.sIM_0.wRevCounter++;
                    		}//if( ps_ImCall->wIndex == IM0_INDEX_2 )
							else
    					#endif//#ifdef IM2_SUPP

    					#ifdef IM3_SUPP
                    		if( ps_ImCall->wIndex == IM0_INDEX_3 )
                    		{
                    	    	//OK, save IM_INDEX
                    	    	user.awImIndex[ ( sap & 0x0F ) ] = IM0_INDEX_3;
                	    		memcpy( &user.sIM_3.abHeader[0], &pdu->write.pdu_data[4], SizeOfIM3 );
								user.sIM_0.wRevCounter++;
                    		}//if( ps_ImCall->wIndex == IM0_INDEX_3 )
							else
    					#endif//#ifdef IM3_SUPP

    					#ifdef IM4_SUPP
                    		if( ps_ImCall->wIndex == IM0_INDEX_4 )
                    		{
                    	    	//OK, save IM_INDEX
                    	    	user.awImIndex[ ( sap & 0x0F ) ] = IM0_INDEX_4;
                	    		memcpy( &user.sIM_4.abHeader[0], &pdu->write.pdu_data[4], SizeOfIM4 );
								user.sIM_0.wRevCounter++;
                    		}//if( ps_ImCall->wIndex == IM0_INDEX_4 )
							else
    					#endif//#ifdef IM4_SUPP
                    		{
								pdu->negRes.err_decode = DPV1_ERRDC_DPV1;
                    		    pdu->negRes.err_code1 = DPV1_ERRCL_ACCESS | DPV1_ERRCL_ACC_ACCESS;
                    		    pdu->negRes.err_code2 = 0x00;
                    		    ret_value = DP_NOK;
                    		}//else of if( ps_ImCall->wIndex == IM0_INDEX_0 )
                    }//else if( pdu->write.length == 0x44 )

                    else
                    {
                    	pdu->negRes.err_decode = DPV1_ERRDC_DPV1;
                    	pdu->negRes.err_code1 = DPV1_ERRCL_ACCESS | DPV1_ERRCL_ACC_ACCESS;
                    	pdu->negRes.err_code2 = 0x00;
                    	ret_value = DP_NOK;
                    }//else of if( pdu->write.length == 0x04 )
                }//if(    ( ps_ImCall->bExtendedFunctionNumber == IM_FN_CALL ) ...
                else
                {
                	pdu->negRes.err_decode = DPV1_ERRDC_DPV1;
                    pdu->negRes.err_code1 = DPV1_ERRCL_ACCESS | DPV1_ERRCL_ACC_INV_PARAM;
                    pdu->negRes.err_code2 = 0x00;
                    ret_value = DP_NOK;
                }//else of if(    ( ps_ImCall->bExtendedFunctionNumber == IM_FN_CALL ) ...
            }//if( pdu->write.slot_num == 0 )
            else
            {
                //BugFix 503
            	pdu->negRes.err_decode = DPV1_ERRDC_DPV1;
                pdu->negRes.err_code1 = DPV1_ERRCL_ACCESS | DPV1_ERRCL_ACC_INV_INDEX;
                pdu->negRes.err_code2 = 0x00;
                ret_value = DP_NOK;
            }//if( pdu->write.slot_num == 0 )
        }
    #else
        if( pdu->write.index == 255 )
        {
        	pdu->negRes.err_decode = DPV1_ERRDC_DPV1;
            pdu->negRes.err_code1 = DPV1_ERRCL_ACCESS | DPV1_ERRCL_ACC_INV_INDEX;
            pdu->negRes.err_code2 = 0x00;
            ret_value = DP_NOK;
        }
    #endif//#ifdef DPV1_IM_SUPP

        else
		if( ret_value == DP_OK )
		{
			switch( pdu->read.slot_num )
			{
				case 0:
				{
			        if( pdu->write.index == 2 )
			        {
						max_length = ( sap == DP_C1_RD_WR_SAP_NR ) ? (C1_LEN-4) : (C2_LEN-4);
						if( pdu->write.length > max_length )
						{
							pdu->write.length = max_length;
						}

						memcpy( &user.dpv1_rw_buffer[0], &pdu->write.pdu_data[0], pdu->write.length );
					}//if( pdu->write.slot_num == 0x00 )
			        else {
			        	pdu->negRes.err_decode = DPV1_ERRDC_DPV1;
						pdu->negRes.err_code1 = DPV1_ERRCL_ACCESS | DPV1_ERRCL_ACC_INV_INDEX;
						pdu->negRes.err_code2 = 0x00;
						ret_value = DP_NOK;
			        }
			        break;
				}
				break;
				case SLOT_TEMPLIMIT:
				{
					if(pdu->write.index == INDEX_TEMPLIMIT_UPPER)
					{
						if(pdu->write.length == 2)
						{
							user.prm.upper_limit = (pdu->write.pdu_data[0] << 8) + pdu->write.pdu_data[1];
						} else
						{
							pdu->negRes.err_decode = DPV1_ERRDC_DPV1;
							pdu->negRes.err_code1 = DPV1_ERRCL_ACCESS | DPV1_ERRCL_ACC_WRITE_LEN;
							pdu->negRes.err_code2 = 0x00;
							ret_value = DP_NOK;
						}

					} else if(pdu->write.index == INDEX_TEMPLIMIT_LOWER)
					{
						if(pdu->write.length == 2)
						{
							user.prm.lower_limit = (pdu->write.pdu_data[0] << 8) + pdu->write.pdu_data[1];
					    }else
						{
					    	pdu->negRes.err_decode = DPV1_ERRDC_DPV1;
							pdu->negRes.err_code1 = DPV1_ERRCL_ACCESS | DPV1_ERRCL_ACC_WRITE_LEN;
							pdu->negRes.err_code2 = 0x00;
							ret_value = DP_NOK;
						}
					}
					else {
						pdu->negRes.err_decode = DPV1_ERRDC_DPV1;
						pdu->negRes.err_code1 = DPV1_ERRCL_ACCESS | DPV1_ERRCL_ACC_INV_INDEX;
						pdu->negRes.err_code2 = 0x00;
						ret_value = DP_NOK;
					}
					break;
				}

				case SLOT_CONTROL:
				{
					if(pdu->write.index == INDEX_CONTROL)
					{
						if(pdu->write.length == 1)
						{
							user.prm.control.enable.b = pdu->write.pdu_data[0];
						} else {
							pdu->negRes.err_decode = DPV1_ERRDC_DPV1;
							pdu->negRes.err_code1 = DPV1_ERRCL_ACCESS | DPV1_ERRCL_ACC_WRITE_LEN;
							pdu->negRes.err_code2 = 0x00;
							ret_value = DP_NOK;
						}

					} else {
						pdu->negRes.err_decode = DPV1_ERRDC_DPV1;
						pdu->negRes.err_code1 = DPV1_ERRCL_ACCESS | DPV1_ERRCL_ACC_INV_INDEX;
						pdu->negRes.err_code2 = 0x00;
						ret_value = DP_NOK;
					}
					break;
				}
				default:
				{
					pdu->negRes.err_decode = DPV1_ERRDC_DPV1;
					pdu->negRes.err_code1 = DPV1_ERRCL_ACCESS | DPV1_ERRCL_ACC_INV_SLOT;
					pdu->negRes.err_code2 = 0x00;
					ret_value = DP_NOK;
					break;
				}

			}//switch( pdu->read.slot_num )

		}//else if(  ret_value == DP_OK  )
    return ret_value;
}//DP_ERROR_CODE user_dpv1_write_req( uint8_t sap, DPV1_UNION_PDU_t* pdu )

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
int user_c2_data_transport_req( uint8_t sap, DPV1_UNION_PDU_t* pdu )
{
DP_ERROR_CODE ret_value;

    if( sap == DP_C1_RD_WR_SAP_NR )
    {
        pdu->negRes.err_code1 = DPV1_ERRCL_ACCESS | DPV1_ERRCL_ACC_INV_PARAM;
        pdu->negRes.err_code2 = 0x00;
        ret_value = DP_NOK;
    }//if( sap == DP_C1_RD_WR_SAP_NR )
    else
    {
        if( pdu->transport.length > (C2_LEN-4) )
        {
            pdu->transport.length = (C2_LEN-4);
        }

        if( pdu->transport.slot_num == 0x00 )
        {
            if( pdu->transport.index == 0x02 )
            {
                memcpy( &user.dpv1_rw_buffer[0], &pdu->transport.pdu_data[0], pdu->transport.length );
                ret_value = DP_OK;
            }//if( pdu->transport.index == 0x00 )
            else
            {
            	pdu->negRes.err_decode = DPV1_ERRDC_DPV1;
                pdu->negRes.err_code1 = DPV1_ERRCL_ACCESS | DPV1_ERRCL_ACC_INV_INDEX;
                pdu->negRes.err_code2 = 0x00;
                ret_value = DP_NOK;
            }//else of if( pdu->v.index == 0x00 )
        }//if( pdu->transport.slot_num == 0x00 )
        else
        {
        	pdu->negRes.err_decode = DPV1_ERRDC_DPV1;
            pdu->negRes.err_code1 = DPV1_ERRCL_ACCESS | DPV1_ERRCL_ACC_INV_SLOT;
            pdu->negRes.err_code2 = 0x00;
            ret_value = DP_NOK;
        }//else of if( pdu->transport.slot_num == 0x00 )
    }//else of if( sap == DP_C1_RD_WR_SAP_NR )

    return ret_value;
}//DP_ERROR_CODE user_c2_data_transport_req( uint8_t sap, DPV1_UNION_PDU_t* pdu )

/*
 * With the MSAL1S_Abort.req the user resets the Alarm-State-Machine.
In faulty conditions the Alarm-State-Machine informs the user with a
MSAL1S_Abort.ind and resets itself. In both cases the Alarm-StateMachine enters the state W-START.
 */
void MSAL1S_Abort_Ind(void)
{

}
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
		)
{

}
/*
 * This MSAL1S service indicates to the user that the slave has left
the data exchange state and that the MASC_C1 communication relationship is closed. In this case the transfer of alarm messages is no
longer possible and the MSAL1S enters the same state as after
MSAL1S_Init.
 */
void MSAL1S_Stopped_Ind(void)
{

}
/*
 * By means of this function the Alarm-State-Machine indicates to the
User that due to fatal errors the Alarm-State-Machine MSAL1S, the
cyclic State Machine MSCY1S and the acyclic State Machine MSAC1S
enter the Power-On state.
 */
void MSAL1S_Fault_Ind(uint8_t ASM_Status //This parameter indicates the reason for the failure.		Possible values: DDLM_Fault, Set_Slave_Add, MSAC1S_Fault
		)
{

}

/*
 * By means of this function the RM State Machine indicates to the User
that due to fatal errors all MSAC_C2 and the RM State Machine have
been stopped.
 */
void MSRM2S_Stopped_Ind(uint8_t Status//This parameter indicates the reason for the failure.Possible Values: MSRM2S_Fault, MSAC2S_Fault
		)
{

}
/*
 * By means of this function the RM State Machine indicates a fatal
error to the User.
 */
void MSRM2S_Fault_Ind(uint8_t Status//This parameter indicates the reason for the failure.Possible Values: FDL_Fault, MSRM2S_Fault
		)
{

}

/*
 * By means of this service an alarm message is transferred from the
DPV1-Slave to the DPV1-Master (Class 1). By means of the
subordinated service MSAC1_Alarm_Ack (see chapter 6.2.1) the
acknowledgement of the alarm will be transmitted from the master to
the slave.
 */
void MSAL1_Alarm_Con(uint8_t isPosive,DPV1_UNION_PDU_t* pAlarmCon,uint8_t Status)
{
    _toUser_("RecvAlarmAckCon: slot:%d . Specifier:%d . seqNr:%d . event: lower",pAlarmCon->alarmAck.bSlotNr,pAlarmCon->alarmAck.bSpecifier.sbit.Alarm_Specifier,pAlarmCon->alarmAck.bSpecifier.sbit.Seq_Nr);
}

MSAC_C2_INITIATE_RES_PDU_t gPdu = {0};
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
)
{

	gPdu.bFunction_num = DPV1_FC_INITIATE;
	gPdu.bFeaturesSupported1 = bFeaturesSupported1;
	gPdu.bFeaturesSupported2 = bFeaturesSupported2;
	gPdu.bProfileFeaturesSupported1 = bProfileFeaturesSupported1;
	gPdu.bProfileFeaturesSupported2 = bProfileFeaturesSupported2;
	gPdu.wProfileIdentNumber = wProfileIdentNumber;
	gPdu.bS_Type = bD_Type;
	gPdu.bS_Len = bD_Len;
	gPdu.bD_Type = bS_Type;
	gPdu.bD_Len = bS_Len;
	memcpy(gPdu.abAddrData,paAddrData, bD_Len + bS_Len);
	pUserDPCall->MSAC2_Initiate_Res(true, bResSAP,(DPV1_UNION_PDU_t*) &gPdu, MSAC_C2_LEN_INITIATE_RES_HEAD + bD_Len + bS_Len);
	return 0;
}
/*
 * The DPV1-Master (Class 2) or DPV1-Slave are using the service
MSAC2_Abort to abort a MSAC_C2 communication relationship.
 */
int MSAC2_Abort_Ind(
        uint8_t  bSap,//This parameter contains the station address of the requester. By means of this address the access protection in the responder is supplied
		uint8_t  bLocallyGenerated,//This parameter only exists in the indication and is not encoded in the PDU. It indicates that the Abort has been produced by the remote
		//partner (Locally_Generated=FALSE) or in the local station	(Locally_Generated=TRUE).
		uint8_t  bSubnet,//	This parameter is used to encode information about the location of the source of the Abort.req.
		uint8_t  bInstance,//This parameter is used to encode information about the protocol instance that detected a problem causing the Abort.req.
		uint8_t  bReasonCode//The Reason_Code indicates the reason for the Abort.req.
)
{
    //pdu = pdu;  //avoid warning

    #ifdef DPV1_IM_SUPP
        user.awImIndex[ ( bSap & 0x0F ) ] = 0x0000;
    #else
    	sap = sap;  //avoid warning
    #endif//#ifdef DPV1_IM_SUPP

    return DP_OK;

}

void dp_demo(void)
{
	/*
	 * 用户数据初始
	 */
	user.input.ctrlStatus.enable.b = 0 ;
	user.input.temperature = 1500;
	//读取ID
	uint8_t id = 0x00;
	if(HAL_GPIO_ReadPin(ADDR0_GPIO_Port, ADDR0_Pin) == GPIO_PIN_SET)
	{
		id |= (0x01 << 0);
	}
	if(HAL_GPIO_ReadPin(ADDR1_GPIO_Port, ADDR1_Pin) == GPIO_PIN_SET)
	{
		id |= (0x01 << 1);
	}
	if(HAL_GPIO_ReadPin(ADDR2_GPIO_Port, ADDR2_Pin) == GPIO_PIN_SET)
	{
		id |= (0x01 << 2);
	}
	if(HAL_GPIO_ReadPin(ADDR3_GPIO_Port, ADDR3_Pin) == GPIO_PIN_SET)
	{
		id |= (0x01 << 3);
	}
	//ds18b20初始化
    ds18B20_Init();
	/*
	 * 协议栈初始化
	 */
    dp_InitAll(id);
    while(1){
    	/*
    	 * 用户处理调用，该处理函数不能一直占用时间！！！
    	 * 有数据更新时，标记user.userUpdateInput = ture;
    	 */
    	//todo: user process.
    	//Process_main();
    	HAL_GPIO_TogglePin(LED_RUN_GPIO_Port, LED_RUN_Pin);
    	/*
    	 * 拨码开关模拟当前是否正在加热或制冷状态
    	 */
    	user.input.ctrlStatus.enable.b = 0;
    	if(user.output.ctrl.enable.sbit.cool_e){ //主机允许制冷控制
			if(HAL_GPIO_ReadPin(COOLING_GPIO_Port, COOLING_Pin) == GPIO_PIN_SET){
				user.input.ctrlStatus.enable.sbit.cool = 1;
			} else {
				user.input.ctrlStatus.enable.sbit.cool = 0;
			}
    	}
    	if(user.output.ctrl.enable.sbit.hot_e){//主机允许制热控制
			if(HAL_GPIO_ReadPin(HOTTING_GPIO_Port, HOTTING_Pin) == GPIO_PIN_SET){
				user.input.ctrlStatus.enable.sbit.hot = 1;
			} else {
				user.input.ctrlStatus.enable.sbit.hot = 0;
			}
    	}
    	user.userUpdateInput = true;
    	/*
    	 * 采集温度传感器获取当前实时温度
    	 */
    	user.input.temperature = (int16_t)(single_point_Read_Temperature_12bit(NULL) * 100);

    	/*
    	 * DP协议处理的模板，不能乱改
    	 */
    	pUserDPCall->fdl_StateProcess();
		if(pUserDPCall->dp_getMSCY1S_State() == sMSCY1S_DataEXCH)
		{
			HAL_GPIO_WritePin(LED_DEX_GPIO_Port, LED_DEX_Pin, GPIO_PIN_SET);
			/*
			 * 告知应用准备好
			 */
			if(pUserDPCall->dp_getApplicationReady() == false)
			{
				pUserDPCall->dp_updateNewInput((uint8_t*)&user.input, DIN_BUFSIZE);
				pUserDPCall->dp_updateApplicationReady(true);
			}
			/*
			 * 更新输入
			 */
			if(user.userUpdateInput)
			{
				user.userUpdateInput = false;
				/* 将控制返回 */
				user.input.ctrlStatus.enable.sbit.cool_e = user.output.ctrl.enable.sbit.cool_e;
				user.input.ctrlStatus.enable.sbit.hot_e = user.output.ctrl.enable.sbit.hot_e;
				uint8_t sInput[3] = {(user.input.temperature>> 8) & 0xff,
						user.input.temperature & 0xff ,
						user.input.ctrlStatus.enable.b
				}; /* 内存小端模式，转换为通信 大端模式 */
				pUserDPCall->dp_updateNewInput((uint8_t*)sInput, DIN_BUFSIZE);
				HAL_GPIO_WritePin(LED_DEX_GPIO_Port, LED_DEX_Pin, GPIO_PIN_RESET);

				/*
				 * 检查温度范围，是否告警
				 */
		        //pUserDPCall->MSAL1S_DiagUpd_Req(true, diagSend, sizeof(diagSend)); //DPV1 诊断数据上报调用
		        //pUserDPCall->dp_updateNewExtDiagData(true, diagSend, sizeof(diagSend));//DPV0 诊断数据上报调用
		        //pUserDPCall->MSAL1S_Status_Req(diagSend, sizeof(diagSend));//DPV1 状态上报调用
				if(user.input.temperature > user.prm.upper_limit)
				{
					/* 高温报警 */
			        Alarm_Status_PDU_queue_t alarm = {0};

			        alarm.bHeader.sbit.len  =  6;  //all length  = 4 + alarm.bUserDiagLength

			        alarm.bAlarmStatusType.sbit.statusFlag = false; //alarm  must set false;
			        alarm.bAlarmStatusType.sbit.type = ALARM_TYPE_DIAGNOSTIC;

			        alarm.bSlotNr = SLOT_TEMPLIMIT;

			        alarm.bSpecifier.sbit.Add_Ack = true;
			        alarm.bSpecifier.sbit.Alarm_Specifier = SPEC_APPEARS;
			        alarm.bSpecifier.sbit.Seq_Nr = user.alarm_sn++ ;

			        user.alarmEvent_send.envent.b = 0 ;
			        user.alarmEvent_send.envent.sbit.upper = true;
			        user.alarmEvent_send.curTemperature = user.input.temperature;
			        alarm.bUserDiagLength = sizeof(user.alarmEvent_send);
			        alarm.pToUserDiagData = (uint8_t*)&user.alarmEvent_send;

			        pUserDPCall->MSAL1_Alarm_Req(&alarm);

			        _toUser_("SendAlarmEvent: slot:%d . Specifier:APPEARS . seqNr:%d . event: upper",alarm.bSlotNr,alarm.bSpecifier.sbit.Seq_Nr);
				} else if(user.input.temperature < user.prm.lower_limit)
				{
					/* 低温报警 */
			        Alarm_Status_PDU_queue_t alarm = {0};

			        alarm.bHeader.sbit.len  =  6;  //all length  = 4 + alarm.bUserDiagLength

			        alarm.bAlarmStatusType.sbit.statusFlag = false; //alarm  must set false;
			        alarm.bAlarmStatusType.sbit.type = ALARM_TYPE_DIAGNOSTIC;

			        alarm.bSlotNr = SLOT_TEMPLIMIT;

			        alarm.bSpecifier.sbit.Add_Ack = true;
			        alarm.bSpecifier.sbit.Alarm_Specifier = SPEC_APPEARS;
			        alarm.bSpecifier.sbit.Seq_Nr = user.alarm_sn++ ; //

			        user.alarmEvent_send.envent.b = 0 ;
			        user.alarmEvent_send.envent.sbit.upper = true;
			        user.alarmEvent_send.curTemperature = user.input.temperature;
			        alarm.bUserDiagLength = sizeof(user.alarmEvent_send);
			        alarm.pToUserDiagData = (uint8_t*)&user.alarmEvent_send;

			        pUserDPCall->MSAL1_Alarm_Req(&alarm);
			        _toUser_("SendAlarmEvent: slot:%d . Specifier:APPEARS . seqNr:%d . event: lower",alarm.bSlotNr,alarm.bSpecifier.sbit.Seq_Nr);
				} else {
					/* 报警消失 */
					if(user.alarmEvent_send.envent.b != 0)
					{
				        Alarm_Status_PDU_queue_t alarm = {0};

				        alarm.bHeader.sbit.len  =  6;  //all length  = 4 + alarm.bUserDiagLength

				        alarm.bAlarmStatusType.sbit.statusFlag = false; //alarm  must set false;
				        alarm.bAlarmStatusType.sbit.type = ALARM_TYPE_DIAGNOSTIC;

				        alarm.bSlotNr = SLOT_TEMPLIMIT;

				        alarm.bSpecifier.sbit.Add_Ack = true;
				        alarm.bSpecifier.sbit.Alarm_Specifier = SPEC_DISAPPEARS;
				        alarm.bSpecifier.sbit.Seq_Nr = user.alarm_sn++ ; //

				        user.alarmEvent_send.envent.b = 0 ;
				        user.alarmEvent_send.curTemperature = user.input.temperature;
				        alarm.bUserDiagLength = sizeof(user.alarmEvent_send);
				        alarm.pToUserDiagData = (uint8_t*)&user.alarmEvent_send;

				        pUserDPCall->MSAL1_Alarm_Req(&alarm);
				        _toUser_("SendAlarmEvent: slot:%d . Specifier:DISAPPEARS . seqNr:%d . event: none",alarm.bSlotNr,alarm.bSpecifier.sbit.Seq_Nr);
					}
				}
			}

		} else
		if(pUserDPCall->dp_getMSCY1S_State() == sMSCY1S_PowerON)
		{
			/*
			 * 在sMSCY1S_PowerON状态下，必须调用
			 */
			dp_Init();
			HAL_GPIO_WritePin(LED_DEX_GPIO_Port, LED_DEX_Pin, GPIO_PIN_RESET);
		}
    }
}
#endif
