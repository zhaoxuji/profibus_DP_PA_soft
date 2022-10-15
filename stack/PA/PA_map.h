/*
 * PA_map.h
 *
 *  Created on: Jul 27, 2020
 *      Author: fred_note
 */

#ifndef PA_MAP_H_
#define PA_MAP_H_


/**
 * 用户配置
 */

/*
 * 注意，当前支持的参数有所限制
 * 1、仅仅支持一个PB块
 * 2、每个块的参数，不能超过254-16 = 238 个。
 * 3、支持多个TB和FB。
 *
 *
 * */
#define NUM_BLK_TB    1   //转换块个数
#define NUM_BLK_FB_AI    4  //功能块个数
#define NUM_BLK_FB_AO    4  //功能块个数
#define NUM_BLK_FB_DI    4  //功能块个数
#define NUM_BLK_FB_DO    4  //功能块个数
#define NUM_BLK_LO    1   //链接对象个数

#define NUM_BLK_FB 	(NUM_BLK_FB_AI+NUM_BLK_FB_AO+NUM_BLK_FB_DI+NUM_BLK_FB_DO)


#define TAB_ADDR_FLASH   0x807F000    //SIZE 2k

/*
 * 行规规范，不可变
 */
#define NUM_BLK_PB    1   //物理块个数,目前只支持1个，不能改变

#define FB_SLOT_START 		1  //Slot 1包含第1个FB，Slot 2包含第2个FB……，即在模块(GSD中)、FB和Slot之间是一对一的映射。规则j)是惟一的例外。
#define FB_INDEX_START 		16 //c)所有FB都从相应Slot的索引(Index)16开始(即FB的Block—Object总是位于Indexl6)。
#define DIR_DEVICE_SLOT 	1  //设备管理的目录总是从(Slot l，Index 0)开始。
#define DIR_DEVICE_INDEX 	0  //设备管理的目录总是从(Slot l，Index 0)开始。
#define PB_SLOT_FIX    	    0  //e)所有设备的PB都应位于Slot 0中，从Index 16开始。
#define PB_INDEX_START  	16 //e)所有设备的PB都应位于Slot 0中，从Index 16开始。

#define TB_SLOT_START 		2
#define TB_INDEX_START 		16

#define DM_SLOT    1
#define DM_INDEX   0

#define get_PB_SLOT(x)    (PB_SLOT_START)
#define get_PB_INDEX(x)   (PB_INDEX_START + x - 1)
#define get_FB_SLOT(x)    (FB_SLOT_START + x -1)
#define get_FB_INDEX(x)   (FB_INDEX_START)


/**
 *  非循环数据传输映射
 */
#define ADDR_PARA_(slot,index,Rel_Index) (slot)





#endif /* PA_MAP_H_ */
