/*
 * ds18b20.c
 *
 *  Created on: Aug 27, 2022
 *      Author: fred
 */


#include "ds18b20.h"
//#include "tim.h"
#include "main.h"

//
//void GPIO_AFSet(GPIO_TypeDef *gpio, uint32_t pin, uint8_t dat)
//{
//	if (pin < 8)
//	{
//		gpio->AFR[0] &= ~(0X07 << (4uL * pin));
//		gpio->AFR[0] |= dat << (4uL * pin);
//	}
//	else
//	{
//		pin -= 8;
//		gpio->AFR[1] &= ~(0X07 << (4uL * pin));
//		gpio->AFR[1] |= dat << (4uL * pin);
//	}
//}
//void GPIO_Set(GPIO_TypeDef *gpio,uint32_t pin, uint8_t mode,uint8_t oty,uint8_t pup,uint8_t speed)
//{
//	uint32_t tmp;
//
//	tmp = ~(0X03 << (pin * 2uL));
//
//	gpio->MODER &= tmp;
//	gpio->MODER |= mode << (pin * 2uL);
//
//	gpio->PUPDR &= tmp;
//	gpio->PUPDR |= pup << (pin * 2uL);
//
//	gpio->OSPEEDR &= tmp;
//	gpio->OSPEEDR |= speed << (pin * 2uL);
//
//	tmp = ~(0X01 << pin);
//	gpio->OTYPER |= oty << pin;
//}
//
//
//void GPIO_Out(GPIO_TypeDef *gpio, uint32_t pin, uint32_t out)
//{
//	if (out == 1)
//	{
//		gpio->ODR |= 1uL << pin;
//	}
//	else
//	{
//		gpio->ODR &= ~(1uL << pin);
//	}
//}
//GPIO_PinState GPIO_In(GPIO_TypeDef *gpio,uint32_t pin)
//{
//		if(((1uL << pin)&gpio->IDR) != 0)
//		{
//				return GPIO_PIN_SET;
//		}
//
//		return GPIO_PIN_RESET;
//}
/**
 * 此延时函数代码适用于HAL库
 */
static void delay_us(uint32_t delayUs)
{
	TIM6->SR &= ~TIM_SR_UIF;
	TIM6->ARR  = delayUs + 1;
	TIM6->CR1	|= (TIM_CR1_CEN|TIM_CR1_URS);
	TIM6->CNT = 0 ;
	while(!(TIM6->SR & TIM_SR_UIF));
	TIM6->CR1	&= ~(TIM_CR1_CEN);
	TIM6->SR &= ~TIM_SR_UIF;
}

static void ds18B20_IO_OUT(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	GPIO_InitStruct.Pin = IO_18B20_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(IO_18B20_GPIO_Port, &GPIO_InitStruct);

	//GPIO_Set(IO_18B20_GPIO_Port,10,MODE_OUTPUT,1,GPIO_NOPULL,GPIO_SPEED_FREQ_LOW);
}

static void ds18B20_IO_IN(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	GPIO_InitStruct.Pin = IO_18B20_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(IO_18B20_GPIO_Port, &GPIO_InitStruct);

//	GPIO_Set(IO_18B20_GPIO_Port,10,MODE_INPUT,1,GPIO_NOPULL,GPIO_SPEED_FREQ_LOW);
}

static void ds18B20_DQ_OUT(int state)
{
	HAL_GPIO_WritePin(IO_18B20_GPIO_Port, IO_18B20_Pin, state ? GPIO_PIN_SET : GPIO_PIN_RESET);
//	GPIO_Out(IO_18B20_GPIO_Port,10,state ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

static int ds18B20_DQ_IN(void)
{
	return HAL_GPIO_ReadPin(IO_18B20_GPIO_Port, IO_18B20_Pin) == GPIO_PIN_SET ? 1 : 0;
}


/**
 **********************************************************************************************
 *
 *                             ds18B20驱动
 *
 */


//复位ds18B20
void ds18B20_Rst(void)
{
#if 1
	ds18B20_IO_OUT(); //SET PA0 OUTPUT
    ds18B20_DQ_OUT(0); //拉低DQ
    delay_us(490);    //拉低750us
    ds18B20_DQ_OUT(1); //DQ=1
	delay_us(28);     //15US
#else
	uint16_t i;
	uint8_t tmp = 0;

	ds18B20_DQ_OUT(0);
	ds18B20_IO_OUT();

	delay_us(480);//480us
	//ds18B20_DQ_OUT(1);
	delay_us(60);//60us
	ds18B20_IO_IN();
	delay_us(30);//60us
	for(i = 0;i != 2000;i++)         //判断从机是否拉低
	{
			if(ds18B20_DQ_IN() == 0)
			{
				  tmp = 0;
				  break;
			}
	}

	for(i = 0;i != 2000;i++)          //判断从机是否释放
	{
			if(ds18B20_DQ_IN() == 1)
			{
					break;
			}
	}
	delay_us(2);//
#endif
}

//等待ds18B20的回应
//返回1:未检测到ds18B20的存在
//返回0:存在
uint8_t ds18B20_Check(void)
{
	uint8_t retry=0;
	ds18B20_IO_IN();//SET PA0 INPUT
    while (ds18B20_DQ_IN() && retry<200)
	{
		retry++;
		delay_us(1);
	};
	if(retry>=200)return 1;
	else retry=0;
    while (!ds18B20_DQ_IN()&&retry<240)
	{
		retry++;
		delay_us(1);
	};
	if(retry>=240)return 1;
 return 0;
}
//从ds18B20读取一个位
//返回值：1/0
uint8_t ds18B20_Read_Bit(void) 			 // read one bit
{
#if 1
    uint8_t data;
	ds18B20_IO_OUT();
    ds18B20_DQ_OUT(0);
	delay_us(4);
    ds18B20_DQ_OUT(1);
	ds18B20_IO_IN();
	delay_us(2);
	if(ds18B20_DQ_IN())data=1;
    else data=0;
    delay_us(55);
    return data;
#else

#endif
}
void ds18B20_Write_Bit(uint8_t bitVal)//写一位
{
	ds18B20_IO_OUT();
	ds18B20_DQ_OUT(0);
	delay_us(4);
	if(bitVal)
	{
		ds18B20_DQ_OUT(1);
	}
	else
	{
		ds18B20_DQ_OUT(0);
	}

	delay_us(58);
	ds18B20_IO_IN();
	delay_us(5);
}
//从ds18B20读取一个字节
//返回值：读到的数据
uint8_t ds18B20_Read_Byte(void)    // read one byte
{
	volatile uint8_t i,j,dat;
    dat=0;
	for (i=1;i<=8;i++)
	{
        j=ds18B20_Read_Bit();
        dat=(j<<7)|(dat>>1);
    }
    return dat;
}
//写一个字节到ds18B20
//dat：要写入的字节
void ds18B20_Write_Byte(uint8_t dat)
 {
#if 0
    uint8_t j;
    uint8_t testb;
	ds18B20_IO_OUT();//SET PA0 OUTPUT;
    for (j=1;j<=8;j++)
	{
        testb=dat&0x01;
        dat=dat>>1;
        if (testb)
        {
            ds18B20_DQ_OUT(0);// Write 1
            delay_us(8);
            ds18B20_DQ_OUT(1);
            delay_us(58);
            ds18B20_IO_IN();
            delay_us(5);
        }
        else
        {
            ds18B20_DQ_OUT(0);// Write 0
            delay_us(8);
            ds18B20_DQ_OUT(0);
            delay_us(58);
            ds18B20_IO_IN();
            delay_us(5);
        }
    }
#else
	unsigned char i;
	volatile unsigned char temp;
	for (i = 0; i < 8; i++)
	{
		temp = dat >> i;
		temp &= 0x01;   // 低位先行
		ds18B20_Write_Bit(temp);
	}
#endif
}


//开始温度转换
void ds18B20_Start(void)// ds1820 start convert
{
    ds18B20_Rst();
	ds18B20_Check();
    ds18B20_Write_Byte(0xcc);// skip rom
    ds18B20_Write_Byte(0x44);// convert
}



//初始化ds18B20的IO口 DQ 同时检测DS的存在
//返回1:不存在
//返回0:存在
uint8_t ds18B20_Init(void)
{
	//TIM3 for delay,1 uS
	__HAL_RCC_TIM6_CLK_ENABLE();
	TIM6->CR1 = 0X00 ;
	//TIM3->CR1 |= TIM_CR1_ARPE ;
	TIM6->PSC  = HAL_RCC_GetPCLK1Freq()/1000000-1;				//频率
	//TIM3->ARR  = 0XFFFF;
	TIM6->CR1	|= TIM_CR1_CEN;

	ds18B20_Rst();
	return ds18B20_Check();
}

//从ds18b20得到温度值
//精度：0.1C
//返回值：温度值 （-550~1250）
short ds18B20_Get_Temp(void)
{
    uint8_t temp;
    uint8_t TL,TH;
	short tem;
    ds18B20_Start ();                    // ds1820 start convert
    ds18B20_Rst();
    ds18B20_Check();
    ds18B20_Write_Byte(0xcc);// skip rom
    ds18B20_Write_Byte(0xbe);// convert
    TL=ds18B20_Read_Byte(); // LSB
    TH=ds18B20_Read_Byte(); // MSB
    if(TH>7)
    {
        TH=~TH;
        TL=~TL;
        temp=0;//温度为负
    }else temp=1;//温度为正
    tem=TH; //获得高八位
    tem<<=8;
    tem+=TL;//获得底八位
    tem=(float)tem*0.625;//转换
	if(temp)return tem; //返回温度值
	else return -tem;
}

// 计算字节序列的校验和
static uint8_t CRC8MY(uint8_t *serial, uint8_t length)
{
    uint8_t result = 0x00;
    uint8_t pDataBuf;
    uint8_t i;

    while(length--) {
        pDataBuf = *serial++;
        for(i=0; i<8; i++) {
            if((result^(pDataBuf))&0x01) {
                result ^= 0x18;
                result >>= 1;
                result |= 0x80;
            }
            else {
                result >>= 1;
            }
            pDataBuf >>= 1;
        }
    }
    return result;  //返回校验和
}

float single_point_Read_Temperature_12bit(int(*delayFuction)(uint32_t)) {
	uint8_t get[9];
	unsigned i;
	float f_tem = -100000;

	ds18B20_Rst();
	ds18B20_Check();
	ds18B20_Write_Byte(0xCC);  //Skip ROM，或使用匹配ROM序列号（见后文）
	ds18B20_Write_Byte(0x44);  // 转换温度
	if(delayFuction)
		delayFuction(10);
	else
		delay_us(10 * 1000);
// 温度转换时间，应根据配置选择10ms，114ms或500ms。

	ds18B20_Rst();
	ds18B20_Check();
	ds18B20_Write_Byte(0xCC); // Skip ROM，或使用匹配ROM序列号（见后文）
	ds18B20_Write_Byte(0xBE); // 读取Scratch Pad中数值

	for (i = 0; i < 9; i++)
		get[i] = ds18B20_Read_Byte();

	if (get[8] != CRC8MY(get, 8)) {
//		printf("\nCRC Error\n");
	} else {
		f_tem = (float) (get[1] << 8 | get[0]) / 16.0;
//		printf( "\nTempF= %f degrees C\n", f_tem); //打印摄氏温度
	}
	return f_tem;
}

float single_point_Read_Temperature_14bit(int(*delayFuction)(uint32_t))
{
	uint8_t get[9];
	uint8_t tpmsb, tplsb, i;
	short s_tem, temp;
	float f_tem = -10000;
	uint16_t lsb_tem;
	ds18B20_Rst();
	ds18B20_Check();
	ds18B20_Write_Byte(0xCC);  //Skip ROM，或使用匹配ROM序列号（见后文）
	ds18B20_Write_Byte(0x44);  // 转换温度
	if(delayFuction)
		delayFuction(20);
	else
		delay_us(20 * 1000);
	// 温度转换时间，应根据配置选择20ms或114ms。

	ds18B20_Rst();
	ds18B20_Check();
	ds18B20_Write_Byte(0xCC); // Skip ROM，或使用匹配ROM序列号（见后文）
	ds18B20_Write_Byte(0xBE); // 读取Scratch Pad中数值

	for (i = 0; i < 9; i++)
		get[i] = ds18B20_Read_Byte();

	if (get[8] != CRC8MY(get, 8)) {
		//printf("\nCRC Error\n");
	} else {
		tpmsb = get[1];   // 温度高字节
		tplsb = get[0];   // 温度低字节
		s_tem = tpmsb << 8;
		s_tem = s_tem | tplsb;
		lsb_tem = s_tem & 49152;
		lsb_tem = lsb_tem >> 14;

		s_tem = s_tem & 4095;
		temp = s_tem << 2;
		s_tem = temp | lsb_tem;
		if ((s_tem & 8192) == 8192) {
			s_tem = (s_tem ^ 16383) & 16383;
			f_tem = -1 * (s_tem + 1) * 0.015625;  //温度为负值
		} else
			f_tem = s_tem * 0.015625;
//	printf( "\nTempF= %f degrees C\n", f_tem); //打印摄氏温度
	}
	return f_tem;
}
/*

代码使用示例：

int main()
{
	short temp;

	while(ds18B20_Init())
	{
		printf(" ds18b20 init failed ! \r\n");
		HAL_Delay(1000);
	}

	while(1)
	{
		temp = ds18B20_Get_Temp();
		printf("当前温度:%0.2f \r\n", (float)temp / 10);
		HAL_Delay(1000);
	}
}

*/


