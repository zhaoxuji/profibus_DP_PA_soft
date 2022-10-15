/*
 *
 */
#ifndef __PLATFORM_DEBUG_H__
#define __PLATFORM_DEBUG_H__

#include "platformDebugOpt.h"
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include "SEGGER_RTT.h"


#define		PLTF_DEBUG
#define		PLTF_ENABLE_SMALL_PRINTF	1

#define		MAX_PRINT_STR_SIZE			(128)

/** lower two bits indicate debug level
 * - 0 all
 * - 1 warning
 * - 2 serious
 * - 3 severe
 */
#define PLTF_DBG_LEVEL_ALL     0x00
#define PLTF_DBG_LEVEL_OFF     PLTF_DBG_LEVEL_ALL /* compatibility define only */
#define PLTF_DBG_LEVEL_WARNING 0x01 /* bad checksums, dropped packets, ... */
#define PLTF_DBG_LEVEL_SERIOUS 0x02 /* memory allocation failures, ... */
#define PLTF_DBG_LEVEL_SEVERE  0x03
#define PLTF_DBG_MASK_LEVEL    0x03

//API
#if 0
#define dbg_warning(format,...)
#define dbg_infor(format,...)
#define dbg_infor_(format,...)
#define dbg_error(format,...)
#define dbg_infor_ble(format,...)
#else
#define dbg_warning(format,...)	SEGGER_RTT_printf(0,RTT_CTRL_TEXT_BRIGHT_YELLOW"warning: "format""RTT_CTRL_RESET"\r\n", ##__VA_ARGS__)
#define dbg_infor(format,...)	SEGGER_RTT_printf(0,"\ninfor: "format"\r\n", ##__VA_ARGS__)
#define dbg_infor_(format,...)	SEGGER_RTT_printf(0,""format" ", ##__VA_ARGS__)
#define dbg_error(format,...)	do { \
									SEGGER_RTT_printf(0,RTT_CTRL_TEXT_BRIGHT_RED"error: "format""RTT_CTRL_RESET"\r\n", ##__VA_ARGS__); \
								} while(1)
#endif
#endif /* __PLTF_DEBUG_H__ */

