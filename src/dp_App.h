/*
 * dpApp.h
 *
 *  Created on: 2022年9月21日
 *      Author: fred
 */

#ifndef DP_APP_H_
#define DP_APP_H_


#include "dp.h"
#include "dp_port.h"


#if !PA_PROFILE_ENABLE
#define _toUser_(format,...)    //SEGGER_RTT_printf(0,""format"\n", ##__VA_ARGS__)

typedef struct
{
	union {
		uint8_t b;
		struct {
			uint8_t hot:1;
			uint8_t cool:1;
			uint8_t resv:2;
			uint8_t cool_e:1;
			uint8_t hot_e:1;
		}sbit;
	}enable;
}_PACKED_ prm_control_t;

typedef struct
{
	uint16_t upper_limit;
	uint16_t lower_limit;
	prm_control_t control;
}_PACKED_ user_prm_t;

typedef struct
{
	prm_control_t ctrl;
}_PACKED_ output_t;

typedef struct
{
	uint16_t temperature;
	prm_control_t ctrlStatus;
}_PACKED_ input_t;

#define SLOT_TEMPLIMIT   1
#define INDEX_TEMPLIMIT_UPPER  2
#define INDEX_TEMPLIMIT_LOWER  3

#define SLOT_CONTROL   2
#define INDEX_CONTROL  2

typedef  struct{
	union {
		uint8_t b;
		struct {
			uint8_t upper:1;
			uint8_t lower:1;
		}sbit;
	}envent;
	uint16_t curTemperature; //当前curTemperature
}_PACKED_ alarmEnvent_t;

typedef struct
{
    uint8_t         NrOfModules;

    uint8_t         userUpdateInput; //用户将输入数据更新到input中的通知
    uint8_t         alarm_sn;//报警序号
    uint8_t         alarmType_send;//已经发送的报警的报警类型,0: 没有上报的报警
    alarmEnvent_t   alarmEvent_send;//已经发送的报警的事件

    input_t         input;
    output_t        output;
    user_prm_t      prm;

	uint8_t                   dpv1_rw_buffer[ C1_LEN  ];
		#ifdef DPV1_IM_SUPP
			uint16_t            awImIndex[0x10];

			sIM0				sIM_0;

			#ifdef IM1_SUPP
				sIM1			sIM_1;
			#endif//#ifdef IM1_SUPP

			#ifdef IM2_SUPP
				sIM1			sIM_2;
			#endif//#ifdef IM2_SUPP

			#ifdef IM3_SUPP
				sIM1			sIM_3;
			#endif//#ifdef IM3_SUPP

			#ifdef IM4_SUPP
				sIM1			sIM_4;
			#endif//#ifdef IM4_SUPP
		#endif//#ifdef DPV1_IM_SUPP

}user_opt_t;

extern user_opt_t  user;


#else

#include "PA.h"

typedef struct
{
	PA_Prm_Structure_t prmcmd;
}_PACKED_ user_prm_t;


typedef struct
{
    uint8_t         NrOfModules;

    uint8_t         userUpdateInput; //用户将输入数据更新到input中的通知
    //uint8_t         alarm_sn;//报警序号
    //uint8_t         alarmType_send;//已经发送的报警的报警类型,0: 没有上报的报警
    //alarmEnvent_t   alarmEvent_send;//已经发送的报警的事件

    uint8_t        input[PA_DIN_BUFSIZE];
    uint8_t        output[PA_DOUT_BUFSIZE];
    user_prm_t      prm;

	uint8_t                   dpv1_rw_buffer[ C1_LEN  ];
		#ifdef DPV1_IM_SUPP
			uint16_t            awImIndex[0x10];

			sIM0				sIM_0;

			#ifdef IM1_SUPP
				sIM1			sIM_1;
			#endif//#ifdef IM1_SUPP

			#ifdef IM2_SUPP
				sIM1			sIM_2;
			#endif//#ifdef IM2_SUPP

			#ifdef IM3_SUPP
				sIM1			sIM_3;
			#endif//#ifdef IM3_SUPP

			#ifdef IM4_SUPP
				sIM1			sIM_4;
			#endif//#ifdef IM4_SUPP
		#endif//#ifdef DPV1_IM_SUPP

}user_opt_t;

extern user_opt_t  user;
extern PA_DEV_t pa_Dev;

#endif

void Process_main(void);

#endif /* DP_APP_H_ */
