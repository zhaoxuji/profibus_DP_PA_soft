/*
 * ds18b20.h
 *
 *  Created on: Aug 27, 2022
 *      Author: fred
 */

#ifndef INC_ds18B20_H_
#define INC_ds18B20_H_


#include <stdint.h>

uint8_t ds18B20_Init(void);			//初始化ds18B20
short ds18B20_Get_Temp(void);		//获取温度
void ds18B20_Start(void);			//开始温度转换
void ds18B20_Write_Byte(uint8_t dat);//写入一个字节
uint8_t ds18B20_Read_Byte(void);	//读出一个字节
uint8_t ds18B20_Read_Bit(void);		//读出一个位
uint8_t ds18B20_Check(void);		//检测是否存在ds18B20
void ds18B20_Rst(void);				//复位ds18B20
float single_point_Read_Temperature_12bit(int(*delayFuction)(uint32_t));
float single_point_Read_Temperature_14bit(int(*delayFuction)(uint32_t));


#endif /* INC_ds18B20_H_ */
