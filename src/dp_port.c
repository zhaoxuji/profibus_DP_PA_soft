/*
 * dp_port.c
 *
 *  Created on: 2021年9月21日
 *      Author: zhaox
 */
#include "stm32l431xx.h"
#include "main.h"
//#include "osPool.h"
#include <string.h>
#include <stdio.h>
#include "dp.h"
#include "platformDebug.h"
#include "dp_port.h"

#if TEST_DAC8740H
#define DAC8740H 1
#endif
#if DAC8740H
static uint8_t headDAC8470H[] = {0xEA,0x84,0,0xAE};
#endif

extern UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart1_tx;

extern user_DP_call_t * pUserDPCall ;
/*----------------------------------------------
 * 2  DP 应该实现的函数
 *
 * ---------------------------------------------
 */
int FMA12_SetValue_Req(uint8_t MSAP0,//The parameter MSAP_0 has the same meaning as described for the FMA1/2_RESET.	request primitive.
 void* pVariable_name,// The parameter Variable_name contains a list of one or more variables of the FDL and/or PHY. The selectable variables are specified in subclauses 5.2.3.2 and 5.3.3.2.
 void* pDesired_value,//The parameter Desired_value contains a list of one or more values for the specified variables. The permissible values or ranges of values are specified
 //in subclauses 5.2.3.2 and 5.3.3.2.
 uint32_t LocBaudrate
 )
{

    dbg_warning("set UART : %d pbs",LocBaudrate);
    uartDMA_deinit();
#if DAC8740H
    uartDMA_init(57600);
#else
    if(LocBaudrate > 0) {
    	uartDMA_init(LocBaudrate);
    }
#endif
	return 0;
}
int FMA12_Reset_Req(uint8_t MSAP0 // The parameter MSAP_0 (Management Service Access Point_0) specifies the service access point of the Local FMA1/2 User. This MSAP is activated and configured depending on the implementation.
		)
{
	return 0 ;
}

/*
 * 该函数用于发送数据到物理端口上
 */
void fdl_PortSend(fdl_frame_t* pFrame)
{
	void uartDMA_Tx(uint8_t * p ,uint32_t size);
	extern fdl_frame_t * pPortSend ;
	pPortSend = pFrame;
#if DAC8740H
	//uint8_t ds = pFrame->pkgLen*11*1000000*2/45450/2048+1;
//	if(pFrame-> pkgLen > 240)
//	{
//		dbg_warning("lll");
//	}
	/* 后移4个字节 */
	for(int i = pFrame-> pkgLen - 1 ; i >= 0; i--)
	{
		pFrame->arry[i + 4] = pFrame->arry[i];
	}
	/* 头部加入4字节DAC8470H 发送驱动序列  */
	for (int i = 0  ; i < 4 ; i++)
	{
		pFrame->arry[i] = headDAC8470H[i];
	}
	//pFrame->arry[2] = ds;
	pFrame-> pkgLen += 8;
#endif
    uartDMA_Tx(pFrame->arry,pFrame->pkgLen);
}

/*
 * 该函数用于设置MinTsdr
 */
int setMinTsdr(uint16_t  MinTsdr)
{

	return 0 ;
}



#define USART0_DATA_ADDRESS    ((uint32_t)&USART_DATA(DP_PORT))


typedef  fdl_frame_t uartDMAData_t;

static uartDMAData_t* pUartRxDP = NULL;
static uartDMAData_t* pRxTempDP = NULL;

fdl_frame_t * pPortSend = NULL;
void usart_dma_config_RX(uint32_t rx_buffer,uint32_t size);

#if TEST_DP_PORT
void *         pOsPool;//内存管理句柄
DPL_STRUC_LIST_CB RecvListHead;//接收队列链表
#endif


void RemoteCom_Frame_Intevl(uartDMAData_t**pRxBuf,uartDMAData_t**pRemoteRx_tmp,uint32_t maxlen,uint32_t RecvNum)
{
#if ! TEST_DAC8740H
	//__disable_irq();
	*pRemoteRx_tmp = *pRxBuf;
	if(*pRemoteRx_tmp)
	{
		((uartDMAData_t*)(*pRemoteRx_tmp))->pkgLen = RecvNum;
		if(((uartDMAData_t*)(*pRemoteRx_tmp))->pkgLen == 0 || ((uartDMAData_t*)(*pRemoteRx_tmp))->pkgLen == maxlen)
		{
			HAL_UARTEx_ReceiveToIdle_DMA(&DP_PORT,((uartDMAData_t*)(*pRxBuf))->arry,maxlen);
			//__enable_irq();//portCLEAR_INTERRUPT_MASK_FROM_ISR(rreg);//portENABLE_INTERRUPTS();
			return;
		}

	}
#if TEST_DP_PORT
	*pRxBuf = osPoolCAlloc(pOsPool);
#else
	*pRxBuf = pUserDPCall->fdl_PortGetReciveBuffer();
#endif
	if(*pRxBuf)
	{
		HAL_UARTEx_ReceiveToIdle_DMA(&DP_PORT,((uartDMAData_t*)(*pRxBuf))->arry,maxlen);
	} else {
		static int erro = 0 ;
		erro++;
		dbg_warning("no buffer ..");
		if(erro > 2)
			NVIC_SystemReset();
	}
	//__enable_irq();//portCLEAR_INTERRUPT_MASK_FROM_ISR(rreg);//portENABLE_INTERRUPTS();
	if(pRemoteRx_tmp && (*pRemoteRx_tmp))
	{
#if TEST_DP_PORT
	    dpl_put_blk_to_list_end__(&RecvListHead,*pRemoteRx_tmp);
#else
	    pUserDPCall->fdl_PortRecive(*pRemoteRx_tmp);
	    //dbg_infor("%d,%02X",(*pRemoteRx_tmp)->pkgLen,(*pRemoteRx_tmp)->arry[0]);
#endif
	}
#endif
}


/**
  * @brief Tx Transfer completed callback.
  * @param huart UART handle.
  * @retval None
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
  if(huart == &DP_PORT)
  {
	  DP_RTS_OFF();
  }
}
/**
  * @brief  UART Abort Complete callback.
  * @param  huart UART handle.
  * @retval None
  */
//void HAL_UART_AbortTransmitCpltCallback(UART_HandleTypeDef *huart)
//{
//	  if(huart == &DP_PORT)
//	  {
//			if(pUserDPCall && pUserDPCall->fdl_PortSendFinish)
//				pUserDPCall->fdl_PortSendFinish(pPortSend);
//	  }
//}
/**
  * @brief  Rx Transfer completed callback.
  * @param  huart UART handle.
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == &DP_PORT)
	{
		RemoteCom_Frame_Intevl(&pUartRxDP,&pRxTempDP,256,256);
	}
}
/**
  * @brief  Reception Event Callback (Rx event notification called after use of advanced reception service).
  * @param  huart UART handle
  * @param  Size  Number of data available in application reception buffer (indicates a position in
  *               reception buffer until which, data are available)
  * @retval None
  */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	if(huart == &DP_PORT){
		RemoteCom_Frame_Intevl(&pUartRxDP,&pRxTempDP,256,Size);
	}
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	if(huart == &DP_PORT){
		RemoteCom_Frame_Intevl(&pUartRxDP,&pRxTempDP,256,256);
	}
}

//uint8_t sd[256] = {0};
void uartDMA_Tx(uint8_t * p ,uint32_t size)
{
#if !DAC8740H
	DP_RTS_ON();
#endif
	HAL_UART_Transmit_DMA(&DP_PORT,p,size);

	if(pUserDPCall && pUserDPCall->fdl_PortSendFinish)
		pUserDPCall->fdl_PortSendFinish(pPortSend);
}




/*!
    \brief      initialize the uartDMA application
    \param[in]  none
    \param[out] none
    \retval     none
*/
void uartDMA_init(uint32_t baudrate)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};



#if DAC8740H
    /** Initializes the peripherals clock
    */
      PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
      PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
      if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
      {
        Error_Handler();
      }

      /* Peripheral clock enable */
      __HAL_RCC_USART1_CLK_ENABLE();

      __HAL_RCC_GPIOA_CLK_ENABLE();
      __HAL_RCC_GPIOB_CLK_ENABLE();
      /**USART1 GPIO Configuration
      PA9     ------> USART1_TX
      PB3 (JTDO-TRACESWO)     ------> USART1_DE
      PB4 (NJTRST)     ------> USART1_CTS
      PB7     ------> USART1_RX
      */
      GPIO_InitStruct.Pin = GPIO_PIN_9;
      GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
      GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
      HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

      GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_7;
      GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
      GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
      HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);


//      GPIO_InitStruct.Pin = GPIO_PIN_3;
//      GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//      GPIO_InitStruct.Pull = GPIO_NOPULL;
//      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//      //GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
//      HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

      DP_RTS_OFF();

      /* USART1 DMA Init */
      /* USART1_RX Init */
      hdma_usart1_rx.Instance = DMA1_Channel5;
      hdma_usart1_rx.Init.Request = DMA_REQUEST_2;
      hdma_usart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
      hdma_usart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
      hdma_usart1_rx.Init.MemInc = DMA_MINC_ENABLE;
      hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
      hdma_usart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
      hdma_usart1_rx.Init.Mode = DMA_NORMAL;
      hdma_usart1_rx.Init.Priority = DMA_PRIORITY_LOW;
      if (HAL_DMA_Init(&hdma_usart1_rx) != HAL_OK)
      {
        Error_Handler();
      }

      __HAL_LINKDMA(&huart1,hdmarx,hdma_usart1_rx);

      /* USART1_TX Init */
      hdma_usart1_tx.Instance = DMA1_Channel4;
      hdma_usart1_tx.Init.Request = DMA_REQUEST_2;
      hdma_usart1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
      hdma_usart1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
      hdma_usart1_tx.Init.MemInc = DMA_MINC_ENABLE;
      hdma_usart1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
      hdma_usart1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
      hdma_usart1_tx.Init.Mode = DMA_NORMAL;
      hdma_usart1_tx.Init.Priority = DMA_PRIORITY_LOW;
      if (HAL_DMA_Init(&hdma_usart1_tx) != HAL_OK)
      {
        Error_Handler();
      }

      __HAL_LINKDMA(&huart1,hdmatx,hdma_usart1_tx);

      /* USART1 interrupt Init */
      HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
      HAL_NVIC_EnableIRQ(USART1_IRQn);



	  huart1.Instance = USART1;
	  huart1.Init.BaudRate = baudrate;
	  huart1.Init.WordLength = UART_WORDLENGTH_8B;
	  huart1.Init.StopBits = UART_STOPBITS_1;
	  huart1.Init.Parity = UART_PARITY_NONE;
	  huart1.Init.Mode = UART_MODE_TX_RX;
	  huart1.Init.HwFlowCtl = UART_HWCONTROL_CTS;
	  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	  //if (HAL_RS485Ex_Init(&huart1, UART_DE_POLARITY_HIGH, 8, 8) != HAL_OK)
	  if(HAL_UART_Init(&DP_PORT)!= HAL_OK)
	  {
	    Error_Handler();
	  }

    /* enable the led clock */
	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB);
	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOC);
	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);

    /* configure PADUP_PIN GPIO port DUPLEX */
    //LL_GPIO_SetPinMode(PADUP_GPIO_Port, PADUP_Pin, LL_GPIO_MODE_INPUT);
    //LL_GPIO_SetPinOutputType(PADUP_GPIO_Port, PADUP_Pin, LL_GPIO_OUTPUT_PUSHPULL);
    //LL_GPIO_SetOutputPin(PADUP_GPIO_Port, PADUP_Pin);
    /* configure PARST_PIN GPIO port RST */
    LL_GPIO_SetPinMode(PARST_GPIO_Port, PARST_Pin,LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinOutputType(PARST_GPIO_Port, PARST_Pin, LL_GPIO_OUTPUT_PUSHPULL);
    LL_GPIO_ResetOutputPin(PARST_GPIO_Port, PARST_Pin);
    /* configure PACD_PIN GPIO port IN */
    //LL_GPIO_SetPinMode(PACD_GPIO_Port, PACD_Pin, LL_GPIO_MODE_INPUT);
#else
    /** Initializes the peripherals clock
    */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
    PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
      Error_Handler();
    }

    /* Peripheral clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    PA12     ------> USART1_DE
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);




//      GPIO_InitStruct.Pin = GPIO_PIN_3;
//      GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//      GPIO_InitStruct.Pull = GPIO_NOPULL;
//      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//      //GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
//      HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

      DP_RTS_OFF();

      /* USART1 DMA Init */
      /* USART1_RX Init */
      hdma_usart1_rx.Instance = DMA1_Channel5;
      hdma_usart1_rx.Init.Request = DMA_REQUEST_2;
      hdma_usart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
      hdma_usart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
      hdma_usart1_rx.Init.MemInc = DMA_MINC_ENABLE;
      hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
      hdma_usart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
      hdma_usart1_rx.Init.Mode = DMA_NORMAL;
      hdma_usart1_rx.Init.Priority = DMA_PRIORITY_LOW;
      if (HAL_DMA_Init(&hdma_usart1_rx) != HAL_OK)
      {
        Error_Handler();
      }

      __HAL_LINKDMA(&huart1,hdmarx,hdma_usart1_rx);

      /* USART1_TX Init */
      hdma_usart1_tx.Instance = DMA1_Channel4;
      hdma_usart1_tx.Init.Request = DMA_REQUEST_2;
      hdma_usart1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
      hdma_usart1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
      hdma_usart1_tx.Init.MemInc = DMA_MINC_ENABLE;
      hdma_usart1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
      hdma_usart1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
      hdma_usart1_tx.Init.Mode = DMA_NORMAL;
      hdma_usart1_tx.Init.Priority = DMA_PRIORITY_LOW;
      if (HAL_DMA_Init(&hdma_usart1_tx) != HAL_OK)
      {
        Error_Handler();
      }

      __HAL_LINKDMA(&huart1,hdmatx,hdma_usart1_tx);

      /* USART1 interrupt Init */
      HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
      HAL_NVIC_EnableIRQ(USART1_IRQn);



	  huart1.Instance = USART1;
	  huart1.Init.BaudRate = baudrate;
	  huart1.Init.WordLength = UART_WORDLENGTH_9B;
	  huart1.Init.StopBits = UART_STOPBITS_1;
	  huart1.Init.Parity = UART_PARITY_EVEN;
	  huart1.Init.Mode = UART_MODE_TX_RX;
	  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	  if (HAL_RS485Ex_Init(&huart1, UART_DE_POLARITY_HIGH, 8, 8) != HAL_OK)
	  //if(HAL_UART_Init(&DP_PORT)!= HAL_OK)
	  {
	    Error_Handler();
	  }
#endif

#if ! TEST_DAC8740H
    RemoteCom_Frame_Intevl(&pUartRxDP,&pRxTempDP,256,0);
#endif
#if DAC8740H
    LL_GPIO_SetOutputPin(PARST_GPIO_Port, PARST_Pin);
#endif
}

void uartDMA_deinit(void)
{


	HAL_UART_DeInit(&DP_PORT);

	if(pUartRxDP)
	{
		pUartRxDP->pkgLen = 0 ;
		if(pUserDPCall && pUserDPCall->fdl_PortFreeBuffer)
			pUserDPCall->fdl_PortFreeBuffer(pUartRxDP);
	    pUartRxDP = NULL;
	}
	if(pRxTempDP)
	{
		pRxTempDP->pkgLen = 0 ;
		if(pUserDPCall && pUserDPCall->fdl_PortFreeBuffer)
			pUserDPCall->fdl_PortFreeBuffer(pRxTempDP);
	    pRxTempDP = NULL;
	}
}


#if TEST_DP_PORT
void DP_PORT_TEST(void)
{
	fdl_frame_t * pRecvFrame = NULL ;
	osPoolDef_t pooldef = {
			  . pool_sz = 10,                  ///< number of items (elements) in the pool
			  . item_sz = sizeof(fdl_frame_t),    ///< size of an item
			  . pool = NULL                   ///< pointer to memory for pool
	};
	fdl_Init(NULL);
	pOsPool = osPoolCreate(&pooldef);
	if(pOsPool == NULL)
	{
		return;
	}
	dpl_init_list__(&RecvListHead);

    uartDMA_init(1500000);

    while(1)
    {
    	/* 检查接收队列 */
    	if(dpl_list_not_empty__(&RecvListHead))
    	{

    		dpl_get_blk_from_list__(&RecvListHead,&pRecvFrame);
    		if(pRecvFrame)
    		{
				dbg_infor("DP_PORT recv %d:",pRecvFrame->pkgLen);
				for(int i = 0 ; i< pRecvFrame->pkgLen ; i++)
				{
					dbg_infor_("%02X",pRecvFrame->arry[i]);
				}
				dbg_infor_("\n");
				//接收到的数据，发送出去
				dbg_infor("DP_PORT send %d:",pRecvFrame->pkgLen);
				uartDMA_Tx(pRecvFrame->arry,pRecvFrame->pkgLen);
				dbg_infor("osPoolFree\n");
				osPoolFree(pOsPool, pRecvFrame);

    		}
    		dpl_remove_blk__(pRecvFrame);
    	}
    }

}
#endif

void TIM1_UP_TIM16_IRQHandler(void)
{
	if(SET == LL_TIM_IsActiveFlag_UPDATE(TIM1))
	{

#if TEST_TIMER
		void test_1ms_call(void);
       test_1ms_call();
#else
       //1ms
       pUserDPCall->port_TimerTick1ms(1);
#endif
		LL_TIM_ClearFlag_UPDATE(TIM1);
	}
	NVIC_ClearPendingIRQ(TIM1_UP_TIM16_IRQn);
}





uint32_t getTimerCntValue(void)
{
   return LL_TIM_GetCounter(TIM2);
}
uint32_t getGPIORXInputBitValue(void)
{
	return LL_GPIO_IsInputPinSet(GPIOA, LL_GPIO_PIN_10);
}
void timerEnable(uint8_t enable)
{
	if(enable)
	{
		__disable_irq();
		LL_TIM_EnableCounter(TIM2);
	}
	else
	{
		LL_TIM_DisableCounter(TIM2);
		__enable_irq();
		//eval_baseTimer_config();
	}
}


phy_bpsdvar_t phy_bpsdvar = {
		.regTimerCnt = ((((((uint32_t)0x40000000U) + 0x00000000U) + 0x0000U)) + 0x24U), //返回当前timer 计数器的寄存器地址
	    .regGPIORXPort = GPIOB,//返回UART RX引脚端口寄存器地址
		.regGPIORXPin = LL_GPIO_PIN_7,//返回UART RX引脚
	    .timerEnable= timerEnable,//该函数实现Timer 启停
		.timerMaxCntValue = 65535 //定时器最大计数值
};
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

void phy_bps_detect_init(phy_bpsdvar_t ** pIntVar)
{

	LL_TIM_InitTypeDef TIM_InitStruct = {0};
	 GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* Peripheral clock enable */
	LL_APB2_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);

	TIM_InitStruct.Prescaler = 0;
	TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
	TIM_InitStruct.Autoreload = 65535;
	TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
	TIM_InitStruct.RepetitionCounter = 0;
	LL_TIM_Init(TIM2, &TIM_InitStruct);
	LL_TIM_DisableARRPreload(TIM2);
	LL_TIM_SetClockSource(TIM2, LL_TIM_CLOCKSOURCE_INTERNAL);
	LL_TIM_SetTriggerOutput(TIM2, LL_TIM_TRGO_RESET);
	LL_TIM_SetTriggerOutput2(TIM2, LL_TIM_TRGO2_RESET);
	LL_TIM_DisableMasterSlaveMode(TIM2);

	LL_TIM_DisableIT_UPDATE(TIM2);
	LL_TIM_DisableUpdateEvent(TIM2);
	LL_TIM_SetCounter(TIM2, 0);



	//PB7 rx to test
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PB3 (JTDO-TRACESWO)     ------> USART1_DE
    PB7     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
#if !DAC8740H
    DP_RTS_OFF(); //接收模式
#endif
    phy_bpsdvar.clkTimerHz = SystemCoreClock, //当前初始化的TIMER的计数频率，Hz为单位
   *pIntVar = &phy_bpsdvar;
}
#if 0
/*
 * Baud rate detection
 */
uint32_t phy_bps_detect(void)
{
    timer_oc_parameter_struct timer_ocintpara;
    timer_parameter_struct timer_initpara;
    rcu_periph_clock_enable(RCU_TIMER0);

    timer_deinit(TIMER0);

    /* TIMER1 configuration */
    uint32_t presc = 0; //1us
    timer_initpara.prescaler         = presc;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 65535;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER0,&timer_initpara);

    timer_autoreload_value_config(TIMER0,65535);
    timer_internal_clock_config(TIMER0);


	timer_interrupt_disable(TIMER0, TIMER_INT_UP);
	timer_update_event_disable(TIMER0);
	timer_counter_value_config(TIMER0, 0);


	//PA10 rx to test
    /* enable GPIO clock */
    rcu_periph_clock_enable(RCU_GPIOA);
    /* connect port to GPIO input */
    gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_MAX, GPIO_PIN_10);

    DP_RTS_OFF(); //接收模式


    int reservCnt = SAMPLE_NUM ; //
    int timerCnt[SAMPLE_NUM] = {0}; //
    int record = false;
    int recordIndex = 0 ;
    int t1 = 0,t2 = 0;
    FlagStatus pinState = gpio_input_bit_get(GPIOA, GPIO_PIN_10);
    __disable_irq();
    timer_enable(TIMER0);
    while(reservCnt)
    {
    	FlagStatus tpinState = gpio_input_bit_get(GPIOA, GPIO_PIN_10);
    	if(pinState != tpinState && tpinState == RESET)
    	{
    		//PIN 状态发生变化
			if(record)
			{
				t2 = TIMER_CNT(TIMER0);
				timerCnt[recordIndex] =t2 < t1 ? t2 + 65535 - t1 : t2 - t1;
				recordIndex ++;
			}
			t1 = t2;
			record = true;
			reservCnt--;
    	}
    	pinState = tpinState;
    }
    timer_disable(TIMER0);
    __enable_irq();
    dbg_infor("end sample ... ");
    for (int i = 0 ; i < SAMPLE_NUM; i++)
    {
    	dbg_infor_("%d ",timerCnt[i]);
    }
    return phy_bps_calculate(timerCnt,SAMPLE_NUM,SystemCoreClock);
}
#endif

/*
 * 该函数用于DP 基础定时器
 *
 */
void eval_baseTimer_config(void)
{


	LL_TIM_InitTypeDef TIM_InitStruct = {0};

	/* Peripheral clock enable */
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM1);

	TIM_InitStruct.Prescaler = SystemCoreClock/1000000 - 1; //1us
	TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
	TIM_InitStruct.Autoreload = 65535;
	TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
	TIM_InitStruct.RepetitionCounter = 0;
	LL_TIM_Init(TIM1, &TIM_InitStruct);
	LL_TIM_DisableARRPreload(TIM1);
	LL_TIM_SetClockSource(TIM1, LL_TIM_CLOCKSOURCE_INTERNAL);
	LL_TIM_SetTriggerOutput(TIM1, LL_TIM_TRGO_RESET);
	LL_TIM_SetTriggerOutput2(TIM1, LL_TIM_TRGO2_RESET);
	LL_TIM_DisableMasterSlaveMode(TIM1);

	LL_TIM_SetAutoReload(TIM1, 1000); //1ms
	NVIC_EnableIRQ(TIM1_UP_TIM16_IRQn);

	LL_TIM_DisableIT_UPDATE(TIM1);
	LL_TIM_DisableUpdateEvent(TIM1);
	LL_TIM_SetCounter(TIM1, 0);

	LL_TIM_EnableIT_UPDATE(TIM1);
	LL_TIM_EnableUpdateEvent(TIM1);
	LL_TIM_EnableCounter(TIM1);

}
#if TEST_DAC8740H
uint8_t dac_tst[] = {0x68,0x1D,0x1D,0x68,0x81,0x83,0x08,0x3E,0x3C,0x02,0x05,0x00,0xF,0x0C,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x69,0x88};
void DAC8740H_test(void)
{
	uint32_t i = 0 ;
	uartDMA_init(57600);
	fdl_frame_t sdtst = {0};

    uint8_t st= 0;
	while(1)
	{
		if ( i == 200000)
		{
#if 0
			sdtst.pkgLen = 4;
			sdtst.arry[0] = 0x55;
			sdtst.arry[1] = 0x55;
			sdtst.arry[2] = 0x55;
			sdtst.arry[3] = 0x55;
#else
#if 1
			sdtst.pkgLen = 6;
			sdtst.arry[0] = 0x10;
			sdtst.arry[1] = 0x04;
			sdtst.arry[2] = 0x02;
			sdtst.arry[3] = 0x00;
		    uint16_t crc = crc_profibus(sdtst.arry, 4);
		    sdtst.arry[4] = (crc >> 8) & 0xff;
		    sdtst.arry[5]  = (crc >> 0) & 0xff;
#else
			memcpy(sdtst.arry,dac_tst,sizeof(dac_tst));
			sdtst.pkgLen = sizeof(dac_tst);
#endif
#endif
//			sdtst.arry[2] = st;
//			sdtst.arry[3] = 0x00;
//		    uint16_t crc = crc_profibus(sdtst.arry, 4);
//		    sdtst.arry[4] = (crc >> 8) & 0xff;
//		    sdtst.arry[5]  = (crc >> 0) & 0xff;
			fdl_PortSend(&sdtst);
			i = 0 ;
			st++;
		}
		i++;
	}
}
#endif
#if TEST_TIMER
static dpTimer_t timer_test = {0};
static uint32_t test_sec = 0 ;
void test_1ms_call(void)
{
	if(timer_test.runFlag > 0)
	{
		timer_test.cnt_tick ++;
		if(timer_test.cnt_tick >= timer_test.compareVale)
		{
			if(timer_test.oneshortFlag)
				timer_test.runFlag = 0 ;
			else
				timer_test.cnt_tick = 0 ;
			test_sec++;
			dbg_infor("testTimer callback %d s",test_sec);
		}
	}
}
void TEST_TIMER_(void)
{
	void eval_baseTimer_config(void);
	eval_baseTimer_config();
	dbg_infor("testTimer set 1s");
	timer_test.runFlag = true;
	timer_test.cnt_tick = 0 ;
	timer_test.compareVale = 1000;
	timer_test.oneshortFlag = 0;

	while(1)
	{

	}
}
#endif

#define I2C0_DATA_ADDRESS        0x40005410
/**
  * @brief I2C接口设备器件地址
  */
#define DeviceAddr				((uint8_t)0x68)
/*!
    \brief      cofigure the I2C0  as master
    \param[in]  none
    \param[out] none
    \retval     none
*/
void I2C_config(void)
{

    LL_I2C_InitTypeDef I2C_InitStruct = {0};

    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

    LL_RCC_SetI2CClockSource(LL_RCC_I2C1_CLKSOURCE_PCLK1);

    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB);
    /**I2C1 GPIO Configuration
    PB8   ------> I2C1_SCL
    PB9   ------> I2C1_SDA
    */
    GPIO_InitStruct.Pin = LL_GPIO_PIN_8|LL_GPIO_PIN_9;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    GPIO_InitStruct.Alternate = LL_GPIO_AF_4;
    LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* Peripheral clock enable */
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C1);

    LL_I2C_EnableAutoEndMode(I2C1);
    LL_I2C_DisableOwnAddress2(I2C1);
    LL_I2C_DisableGeneralCall(I2C1);
    LL_I2C_EnableClockStretching(I2C1);
    I2C_InitStruct.PeripheralMode = LL_I2C_MODE_I2C;
    I2C_InitStruct.Timing = 0x00300F38;
    I2C_InitStruct.AnalogFilter = LL_I2C_ANALOGFILTER_ENABLE;
    I2C_InitStruct.DigitalFilter = 0;
    I2C_InitStruct.OwnAddress1 = 0;
    I2C_InitStruct.TypeAcknowledge = LL_I2C_ACK;
    I2C_InitStruct.OwnAddrSize = LL_I2C_OWNADDRESS1_7BIT;
    LL_I2C_Init(I2C1, &I2C_InitStruct);
    LL_I2C_SetOwnAddress2(I2C1, 0, LL_I2C_OWNADDRESS2_NOMASK);

}
/*!
    \brief      close the I2C0  as master
    \param[in]  none
    \param[out] none
    \retval     none
*/
void I2C_close(void)
{
    LL_I2C_DeInit(I2C1);
    LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_I2C1);
}

void I2C_WDinit(void)
{
    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB);
    LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_8, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinOutputType(GPIOB, LL_GPIO_PIN_8, LL_GPIO_OUTPUT_PUSHPULL);
}
/**
  ***************************************************************************
  * @Brief	   : 从指定地址读出一个字节数据
  * @Parameter : [OUT] pByteData:  指向存储读出字节的数据区
  *              [IN]  AddrToRead：指定待读出地址
  * @Return	   : 无
  ***************************************************************************
  */
void I2C_ReadByte(uint8_t *pByteData, uint8_t AddrToRead)
{
    /* wait until I2C bus is idle */
    while(LL_I2C_IsActiveFlag_BUSY(I2C1));
    /* send a start condition to I2C bus */
    LL_I2C_GenerateStartCondition(I2C1);
    /* wait until SBSEND bit is set */
//    while(!i2c_flag_get(I2C0, I2C_FLAG_SBSEND));

    /* send slave address to I2C bus */
    LL_I2C_SetSlaveAddr(I2C1, (DeviceAddr << 1) | 0x01);
    LL_I2C_SetTransferRequest(I2C1, LL_I2C_REQUEST_WRITE);
    /* wait until ADDSEND bit is set */
    while(!LL_I2C_IsActiveFlag_ADDR(I2C1));

    /* clear ADDSEND bit */
    LL_I2C_ClearFlag_ADDR(I2C1);


    /* data transmission */
    LL_I2C_TransmitData8(I2C1, AddrToRead);

    /* wait until the TBE bit is set */
    while(!LL_I2C_IsActiveFlag_TXE(I2C1));

    /* send a start condition to I2C bus */
    LL_I2C_GenerateStartCondition(I2C1);
    /* wait until SBSEND bit is set */
//    while(!i2c_flag_get(I2C0, I2C_FLAG_SBSEND));


    /* send slave address to I2C bus */
    LL_I2C_SetSlaveAddr(I2C1, (DeviceAddr << 1) | 0x01);
    LL_I2C_SetTransferRequest(I2C1, LL_I2C_REQUEST_READ);
    /* wait until ADDSEND bit is set */
    while(!LL_I2C_IsActiveFlag_ADDR(I2C1));
    /* clear ADDSEND bit */
    LL_I2C_ClearFlag_ADDR(I2C1);


    /* N=1,reset ACKEN bit before clearing ADDRSEND bit */
    LL_I2C_AcknowledgeNextData(I2C1, LL_I2C_NACK);

    /* wait until the RBNE bit is set */
    while(!LL_I2C_IsActiveFlag_RXNE(I2C1));

    /* read a data from I2C_DATA */
    *pByteData = LL_I2C_ReceiveData8(I2C1);


    /* send a stop condition to I2C bus */
    LL_I2C_GenerateStopCondition(I2C1);

    /* wait until stop condition generate */
    while(!LL_I2C_IsActiveFlag_STOP(I2C1));


    /* Enable Acknowledge */
    LL_I2C_AcknowledgeNextData(I2C1, LL_I2C_ACK);

}


/**
  ***************************************************************************
  * @Brief	   : 向指定地址写入一个字节数据
  * @Parameter : [IN] ByteData：   待写入的字节数据
  *				 [IN] AddrToWrite：待写入的地址
  * @Return	   : 无
  ***************************************************************************
  */
void I2C_WriteByte(uint8_t ByteData, uint8_t AddrToWrite)
{
    /* wait until I2C bus is idle */
    while(LL_I2C_IsActiveFlag_BUSY(I2C1));
    /* send a start condition to I2C bus */
    LL_I2C_GenerateStartCondition(I2C1);
    /* wait until SBSEND bit is set */
    //while(!i2c_flag_get(I2C0, I2C_FLAG_SBSEND));

    /* send slave address to I2C bus */
    LL_I2C_SetSlaveAddr(I2C1, (DeviceAddr << 1) | 0x01);
    LL_I2C_SetTransferRequest(I2C1, LL_I2C_REQUEST_WRITE);
    /* wait until ADDSEND bit is set */
    while(!LL_I2C_IsActiveFlag_ADDR(I2C1));
    /* clear ADDSEND bit */
    LL_I2C_ClearFlag_ADDR(I2C1);

    /* wait until the transmit data buffer is empty */
    while(!LL_I2C_IsActiveFlag_TXE(I2C1));

    /* data transmission */
    LL_I2C_TransmitData8(I2C1, AddrToWrite);

    /* wait until the TBE bit is set */
    while(!LL_I2C_IsActiveFlag_TXE(I2C1));

    /* data transmission */
    LL_I2C_TransmitData8(I2C1, ByteData);

    /* wait until the TBE bit is set */
    while(!LL_I2C_IsActiveFlag_TXE(I2C1));

    /* send a stop condition to I2C bus */
    LL_I2C_GenerateStopCondition(I2C1);
    /* wait until stop condition generate */
    while(!LL_I2C_IsActiveFlag_STOP(I2C1));

}


/**
  ***************************************************************************
  * @Brief	   : 从指定地址读出指定长度的字节数据存放到指定区域
  * @Parameter : [IN]  AddrToRead： 指定待读地址
  *			 	 [OUT] pDataBuffer：指向存储读出数据的区域
  *				 [IN]  NumToRead：	指定待读出的字节数据长度
  * @Return	   : 无
  ***************************************************************************
  */
void I2C_ReadBuffer(uint8_t AddrToRead, uint8_t *pDataBuffer, uint16_t NumToRead)
{
//	while (NumToRead--)
//	{
//		I2C_ReadByte(pDataBuffer, AddrToRead);
//		pDataBuffer ++;
//		AddrToRead ++;
//	}

    /* wait until I2C bus is idle */
    while(LL_I2C_IsActiveFlag_BUSY(I2C1));
    /* send a start condition to I2C bus */
    LL_I2C_GenerateStartCondition(I2C1);
    /* wait until SBSEND bit is set */
    //while(!i2c_flag_get(I2C0, I2C_FLAG_SBSEND));

    /* send slave address to I2C bus */
    LL_I2C_SetSlaveAddr(I2C1, (DeviceAddr << 1) | 0x01);
    LL_I2C_SetTransferRequest(I2C1, LL_I2C_REQUEST_WRITE);
    /* wait until ADDSEND bit is set */
    while(!LL_I2C_IsActiveFlag_ADDR(I2C1));
    /* clear ADDSEND bit */
    LL_I2C_ClearFlag_ADDR(I2C1);

    /* data transmission */
    LL_I2C_TransmitData8(I2C1, AddrToRead);
    /* wait until the TBE bit is set */
    while(!LL_I2C_IsActiveFlag_TXE(I2C1));

    /* send a start condition to I2C bus */
    LL_I2C_GenerateStartCondition(I2C1);
    /* wait until SBSEND bit is set */
    //while(!i2c_flag_get(I2C0, I2C_FLAG_SBSEND));

    /* send slave address to I2C bus */
    LL_I2C_SetSlaveAddr(I2C1, (DeviceAddr << 1) | 0x01);
    LL_I2C_SetTransferRequest(I2C1, LL_I2C_REQUEST_READ);
    /* wait until ADDSEND bit is set */
    while(!LL_I2C_IsActiveFlag_ADDR(I2C1));
    /* clear ADDSEND bit */
    LL_I2C_ClearFlag_ADDR(I2C1);

	while (NumToRead)
	{
		if(NumToRead == 1)
		{
		    /* N=1,reset ACKEN bit before clearing ADDRSEND bit */
		    LL_I2C_AcknowledgeNextData(I2C1, LL_I2C_NACK);
		}
	    /* wait until the RBNE bit is set */
	    while(!LL_I2C_IsActiveFlag_RXNE(I2C1));
	    /* read a data from I2C_DATA */
	    *pDataBuffer  = LL_I2C_ReceiveData8(I2C1);
	    pDataBuffer ++;
	    NumToRead--;
	}


    /* send a stop condition to I2C bus */
    LL_I2C_GenerateStopCondition(I2C1);
    /* wait until stop condition generate */
    while(!LL_I2C_IsActiveFlag_STOP(I2C1));
    /* Enable Acknowledge */
    LL_I2C_AcknowledgeNextData(I2C1, LL_I2C_ACK);

}


/**
  ***************************************************************************
  * @Brief	   : 向指定地址写入指定长度的字节数据
  * @Parameter : [IN] AddrToWrite：指定待写入地址
  *				 [IN] pDataBuffer：指向待写入数据的区域
  *				 [IN] NumToWrite： 指定待写入字节数据的长度
  * @Return	   : 无
  ***************************************************************************
  */
void I2C_WriteBuffer(uint8_t AddrToWrite, uint8_t *pDataBuffer, uint16_t NumToWrite)
{

    /* wait until I2C bus is idle */
    while(LL_I2C_IsActiveFlag_BUSY(I2C1));
    /* send a start condition to I2C bus */
    LL_I2C_GenerateStartCondition(I2C1);
    /* wait until SBSEND bit is set */
    //while(!i2c_flag_get(I2C0, I2C_FLAG_SBSEND));

    /* send slave address to I2C bus */
    LL_I2C_SetSlaveAddr(I2C1, (DeviceAddr << 1) | 0x01);
    LL_I2C_SetTransferRequest(I2C1, LL_I2C_REQUEST_WRITE);
    /* wait until ADDSEND bit is set */
    while(!LL_I2C_IsActiveFlag_ADDR(I2C1));
    /* clear ADDSEND bit */
    LL_I2C_ClearFlag_ADDR(I2C1);

    /* wait until the transmit data buffer is empty */
    while(!LL_I2C_IsActiveFlag_TXE(I2C1));

    /* data transmission */
    LL_I2C_TransmitData8(I2C1, AddrToWrite);
    /* wait until the TBE bit is set */
    while(!LL_I2C_IsActiveFlag_TXE(I2C1));

	while (NumToWrite--)
	{
	    /* data transmission */
		LL_I2C_TransmitData8(I2C1, *pDataBuffer);
	    pDataBuffer++;
	    /* wait until the TBE bit is set */
	    while(!LL_I2C_IsActiveFlag_TXE(I2C1));
	}

    /* send a stop condition to I2C bus */
    LL_I2C_GenerateStopCondition(I2C1);
    /* wait until stop condition generate */
    while(!LL_I2C_IsActiveFlag_STOP(I2C1));

}

