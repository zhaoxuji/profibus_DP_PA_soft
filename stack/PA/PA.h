/*
 * PA.h
 *
 *  Created on: 2020年7月10日
 *      Author: fred_note
 */

#ifndef PA_H_
#define PA_H_

#include <PA/PA_map.h>
#include "stdint.h"
#include "stddef.h"
#include "stdbool.h"
#include "dp.h"




/**
 * 常量
 */
#define Boolean uint8_t

#define PA_PROFILE_REVISION 0x0302


#define INVALID_TIMEVALUE  0Xffffffff
#define ASSIG_TIMEVALUE    0Xfffffffe
#define MAX_PA_TIMER  2


#define MAX_TAB_NUM   10


typedef struct
{
	uint32_t cnt;
	void (*funCb)(void* p1,void* p2);
	void * pArg1;
	void * pArg2;
}pa_timer_t;


/*
 * GBZ 25105.1-2010 工业通信网络 现场总线规范 类型10 PROFINET IO规范 第1部分：应用层服务定义
 * 5.3.1.4 Date 类型
 * 5.3.1.4.2 Date
 * CLASS:Data type
 *
 * ATTRIBUTES:
 * 1 Data type Numeric dentifier = 50
 * 2 Data type Name =Date
 * 3 Format =FIXEDLENGTH
 * 4.1 Octet Length 2一7
 * 此数据类型由6个无符号类型值的元意组成,表示公历的日期和时间。第1个元素是Unsigned16
 * 数据类型,它以毫秒为单位表示了分的小数部分。第2个元素是Unsigned8数据类型,它以分为单位
 * 表示了小时的小数部分。第3个元素是Unsigned8数据类型,它以小时为单位表示了天的小数部分,
 * 并用最高有效位说明它是标准时间还是夏时制时间,第4个元素是Unsigned8数据类型,它的前3个
 * 比特指出星期几,后5个比特指出月份中的日期。第5个元素是一个无符号类型8数据类型,指出月
 * 份。最后一个元素是Unsigned8数据类型,指出年份。值0～50对应于2000年至2050年,值51～99
 * 对应于1951年至1999年。
 */
#define DATEOCT6_DAYSOFWEEK_MASK   0xE0
#define DATEOCT6_DAYSOFMOUTH_MASK  0x1F

typedef struct
{
	uint16_t sub_ms;//毫秒为单位表示了分的小数部分
	uint8_t  sub_hour;//分为单位 表示了小时的小数部分
	uint8_t  sub_day;//小时为单位表示了天的小数部分,最高有效位说明它是标准时间还是夏时制时间
	uint8_t  mut_day;//前3个比特指出星期几,后5个比特指出月份中的日期
	uint8_t  month;//月份
	uint8_t  year;//年份，值0～50对应于2000年至2050年
}_PACKED_ Date_t;



/*--------------------------------------------
 * 5．2．3通用数据类型和结构
 *
 */



/*
 * 5.2.2.2 Parameter description table 参数描述表；
 * 本表包含每个块参数及其预计使用的描述。该描述包含参数的语义。
 * 例如，在表中定义了枚举型参数的编码。这些描述也被用作帮助字符串。
 * 如果一个参数具有枚举范围，则允许其支持制造商特定的可选代码子集。
 */
typedef  char*  PA_TabParameterDsc_t ;

/*
 * 5.2.2.3 Parameter attribute table 参数属性表；
 * 参数属性表规定块参数的特性。
 */
typedef  struct
{

	uint8_t relativeIndex; //相对索引(relative index)相对于本块第1个参数的索引偏移量。
	char*  pParameterName;//参数名称(parameter name)该参数的助记名称。指向参数名称常量.
	uint8_t objectType;	   //对象类型(object type)
						   //参数值的对象类型，包括：
						   //·Simple：简单变量；
	                       //·Record：若干个不同的简单变量组成的结构；
	                       //·Array：若干个简单变量组成的数组。
	uint8_t dataType;//数据类型(data type)
					//参数值的数据类型，包括：
					//·Name：简单变量或数组的基本数据类型；
					//· DS_n：数据结构(Record)编号n。
	uint8_t storeType;//存储(store)
					//要求的存储器类型，如下：
					//0,·N：非易失参数，应在整个上电周期内都被保存，但它不属于静态更新代码；
					//1,·S：静态，非易失，修改此参数将增加静态版本计数器ST—REV的值；
					//2,·D：动态，该值由块计算出，或从另一个块读取；
					//3,·Cst：常量，该参数在设备中保持不变。
#define STRORETYPE_N    0
#define STRORETYPE_S    1
#define STRORETYPE_D    2
#define STRORETYPE_C    3
	uint8_t size;//大小(size)
					//数据大小，按八位位组(字节)计。
	uint8_t access;//访问(access)
				   //·r：指示浚参数可被读；
	               //·w：指示该参数可被写。
	               //注：可写参数的范围被限定于当前存储的参数值，这是设备的有效行为。
#define ACCESS_R   0
#define ACCESS_W   1
#define ACCESS_RW  2

	uint8_t parameterUsage;//参数用法(parameter usage)
				  //·C：内含}
	              //·I：输人；
	              //·O：输出。
#define PARAMUSAGE_C 0
#define PARAMUSAGE_I 1
#define PARAMUSAGE_O 2
	uint8_t kindOfTransport;//传输类型(kind of transport)(按指示的最低要求)
							//·a：非循环的；
							//·cyc：循环的。
#define KINDTRANSPORT_A  0
#define KINDTRANSPORT_C  1
	uint8_t resetClass;//复位类别(reset class)
						//FACTORY—RESET(物理块参数)影响设备中块的不同参数集。参数的复位类别特性确定一
						//个参数是处于测量或执行通道的信号链中(功能性参数)，还是该参数包含附加信息(信息性
						//参数)。
						//·F：功能性；
						//·I：信息性；
						//· ：不适用。
#define RESETCLASS_F 0
#define RESETCLASS_I 1
	void* pDefaultValue;//缺省值(default value) , 指向其默认值，类型由其参数自身觉得，采用void*
						//在初始化过程中分配给参数的值。这对于一个未组态的块的初始化是必须的。参数的值符合
						//参数的数据类型。如果在块的属性表中存在参数值，则此值须被用作缺省值(行规缺省值)。
						//如果在块的属性表中没有参数值，则其缺省值是制造商特定的(制造商缺省值)。
	uint8_t downloadOrder;	//下载次序(download order)
							//设备中存在数据的一致性限制(例如，若干参数使用相同的工程单位)。修改一个参数可能会
							//引发设备内的某些计算。因此，固定的参数下载到设备的顺序避免了数据的不一致性。下载
							//是对一组参数的一系列写访问。此属性定义了应遵循的写访问顺序。
							//在一些复杂设备中，因为某些参数的相互依赖性非常高，因此有必要封装一组参数。为此，可
							//以采用“参数处理(parameter Transaction)”的方法(见5．4)。
							//注：每个参数可被单独写入。
	uint8_t mandatoryOptional;//必备／可选(mandatory／optional)
					//·M：指示该参数对于非循环访问是必备的。循环访问可被单独组态；
					//·O：指示该参数是可选的。
}_PACKED_ PA_TabAttribute_t ;


typedef struct {
	uint8_t slot;
	uint8_t index;
	uint8_t relativeIndex;//相对索引
	uint8_t size;//大小(size)
	uint8_t storeTypeDownOrder;//存储(store),//下载次序
	uint8_t APKR; //访问(access) //参数用法 //传输类型  //复位类别
	void* pInst ; //指向实例
}_PACKED_ ParameterAttr_t;

#define __Def_ParameterAttr(type,name)  ParameterAttr_t name

#define __Inst_ParameterAttr(Sslot,Sindex,atrr,Inst,Size,relIndx,storeType,access,parameterUsage,kindOfTransport,resetClass,DownOrder)\
		atrr.slot=(Sslot + (Sindex + relIndx)/255);\
		atrr.index=((Sindex + relIndx)%255);\
		atrr.pInst=(Inst);\
		atrr.size=(Size); \
		atrr.relativeIndex = relIndx; \
		atrr.storeTypeDownOrder = (((storeType & 0x03) << 6) | ((DownOrder & 0x7f)));\
		atrr.APKR = (((access & 0x03) << 6) | ((parameterUsage & 0x03) << 4) | ((kindOfTransport & 0x01) << 3) | ((resetClass & 0x01) << 2))

#define setParam_Attr_STRORType(attr,tp)  (attr.storeTypeDownOrder |= ((tp & 0x03) << 6))
#define getParam_Attr_STRORType(attr)     (attr.storeTypeDownOrder >> 6) & 0x03)
#define setParam_Attr_DownOrder(attr,ord) (attr.storeTypeDownOrder |= ((ord & 0x7f)))
#define getParam_Attr_DownOrder(attr)     (attr.storeTypeDownOrder & 0x7f)

#define setParam_Attr_Size(attr,size)     (attr.size  = size)
#define getParam_Attr_Size(attr)          (attr.size)

#define setParam_Attr_relIndex(attr,idx)     (attr.relativeIndex  = idx)
#define getParam_Attr_relIndex(attr)         (attr.relativeIndex)

#define getParam_Attr_access(attr)         ((attr.APKR >> 6) & 0x03)
#define getParam_Attr_parameterUsage(attr)         ((attr.APKR >> 4) & 0x03)
#define getParam_Attr_kindOfTransport(attr)         ((attr.APKR >> 3) & 0x01)
#define getParam_Attr_resetClass(attr)         ((attr.APKR >> 2) & 0x01)

#define getBlkParam_Number(blk)         (sizeof(blk)/sizeof(ParameterAttr_t))

/*
 * 5.2.2.4 Conformance table 一致性表
 *  每个设备从本行规规定的结构中选择必要的子集。子集的选择遵循在一致性声明中定义的某些规
 *  则。该表指出哪些结构是必备的(M)，哪些是选择的(s)，以及哪些是可选的(())。
 *  ——M：指示该行规特征(物理块、功能块、转换块、浓缩状况等)是必备的；
 *  ——o：指示该行规特征是可选的；
 *  ——s：指示行规设备可以支持一个或多个子特征的选择(例如：模拟输入、模拟输出、离散输入等
 *  是功能块的子特征)。存在两种选择的子特征：必备特征和可选特征。选择必备特征的子特征
 *  是指设备应支持一个或多个所列出的子特征。选择可选特征的子特征是指如果设备支持该特
 *  征，则该设备应支持一个或多个所列出的子特征。
 */
typedef  uint8_t  PA_TabConformance_t ;

/*
 * 5.2.2.5 View object table 视图对象表
 * 视图对象允许使用一个服务请求来读或写若干组参数值。提供这样的能力使得可以及时高效地传
 * 送一组信息。每个块可存在多个视图对象：View_1、View一2、View一3和View一4。
 */
typedef  void(* PA_TabViewobject_f)(uint8_t readWrite,void* pPararmeterList,void* pPararmeter,uint8_t num_parameters) ;



/*
 * 5．2．3．1．3时间值(TimeValue)数据类型(21)
 */
typedef  double  PA_TimeValue21_t  ;



//不支持该参数所涉及的功能，或在当前组态中未使用不使用该参数
#define PA_ENUM_NOTUSED8  250
#define PA_ENUM_NOTUSED16 65530
//不适用
#define PA_ENUM_NONE8 251
#define PA_ENUM_NONE16 65531
//不能被赋值，例如未初始化
#define PA_ENUM_UNKNOWN8 252
#define PA_ENUM_UNKNOWN16 65532
//附加信息是必要的\可用的
#define PA_ENUM_SPECIAL8 253
#define PA_ENUM_SPECIAL16 65533

/*
 * 5．2．3．2块(Block)结构(DS一32)
 * 此数据结构由块的属性组成(见5．2．5)。
 * 数据类型 Block(DS-32)
 * 属性 Number of Elements=12
 * 属性 List of Elements(见表4)
 */

#define     Block_Object_Physical       ((uint8_t)0x01)  // 01 Physical Block,
#define 	Block_Object_Function       ((uint8_t)0x02)  // 02 Function Block
#define 	Block_Object_Transducer     ((uint8_t)0x03)  // 03 Transducer Block

#define 	ParentClass_PB_Transmitter      ((uint8_t)1)     //变送器
#define 	ParentClass_PB_Actuator         ((uint8_t)2)     //执行器
#define 	ParentClass_PB_DiscreteIO       ((uint8_t)3)     //离散I／O
#define 	ParentClass_PB_Controller       ((uint8_t)4)     //控制器
#define 	ParentClass_PB_Analyzer         ((uint8_t)5)     //分析仪器
#define 	ParentClass_PB_LabDevice        ((uint8_t)6)     //实验室设备
#define 	ParentClass_PB_MultiVariable    ((uint8_t)127)   //多变量

#define 	ParentClass_FB_Input            ((uint8_t)1)     //输入
#define 	ParentClass_FB_Output           ((uint8_t)2)     //输出
#define 	ParentClass_FB_Control          ((uint8_t)3)     //控制
#define 	ParentClass_FB_AdvancedControl  ((uint8_t)4)     //先进控制
#define 	ParentClass_FB_Calculation      ((uint8_t)5)     //计算
#define 	ParentClass_FB_Auxiliary        ((uint8_t)6)     //辅助
#define 	ParentClass_FB_Alert            ((uint8_t)7)     //警报

#define 	ParentClass_TB_Pressure 		((uint8_t)1)     //压力
#define 	ParentClass_TB_Temperature 		((uint8_t)2)     //温度
#define 	ParentClass_TB_Flow 			((uint8_t)3)     //流量
#define 	ParentClass_TB_Level 			((uint8_t)4)     //物位
#define 	ParentClass_TB_Actuator 		((uint8_t)5)     //执行器
#define 	ParentClass_TB_DiscreteIO 		((uint8_t)6)     //离散I／O
#define 	ParentClass_TB_Analyzer 		((uint8_t)7)     //分析仪器
#define 	ParentClass_TB_Auxiliary 		((uint8_t)8)     //辅助功能
#define 	ParentClass_TB_Alarm 			((uint8_t)9)     //报警
#define 	ParentClass_TB_reserved 		((uint8_t)10)    //保留

//Input
#define 	Class_FB_AnalogInput   			((uint8_t)1)     //模拟输入
#define 	Class_FB_DiscreteInput 			((uint8_t)2)     //离散输入
//Output
#define 	Class_FB_AnalogOutput   		((uint8_t)1)     //模拟输出
#define 	Class_FB_DiscreteOutput 		((uint8_t)2)     //离散输出
//Control
#define 	Class_FB_PID 					((uint8_t)1)     //PID
#define 	Class_FB_SampleSelector 		((uint8_t)2)     //采样选择器
#define 	Class_FB_LabControlUnit 		((uint8_t)3)     //实验室控制单元
#define 	Class_FB_Thermostate 			((uint8_t)4)     //温度调节器
#define 	Class_FB_Stirrer 				((uint8_t)5)     //搅拌器
#define 	Class_FB_Shaker 				((uint8_t)6)     //混合器
#define 	Class_FB_BalanceScale 			((uint8_t)7)     //天平／秤
#define 	Class_FB_Centrifuge 			((uint8_t)8)     //离心机
#define 	Class_FB_DosingPump 			((uint8_t)9)     //计量泵
//Advanced Control
#define 	Class_FB_LabInstruments 		((uint8_t)1)     //实验室仪器
//Calculation
#define 	Class_FB_Totalizer 				((uint8_t)8)     //累加器
//Auxiliary
#define 	Class_FB_Ramp 					((uint8_t)1)     //斜坡
#define 	Class_FB_BMLogbook 				((uint8_t)2)     //BM日志
#define 	Class_FB_Sample 				((uint8_t)3)     //采样
//Alert

//Pressure
#define 	Class_TB_Differential 			((uint8_t)1)     //差压
#define 	Class_TB_Absolute 				((uint8_t)2)     //绝对压力
#define 	Class_TB_Gage 					((uint8_t)3)     //表压力
#define 	Class_TB_PressureLevelFlow		((uint8_t)4)     //压力+物位+流量
#define 	Class_TB_PressureLevel			((uint8_t)5)     //压力+物位
#define 	Class_TB_PressureFlow  			((uint8_t)6)     //压力+流量
#define 	Class_TB_MixedAbsoluteDiffPressure  ((uint8_t)7)     //混合绝对压力／差压
//Temperature
#define 	Class_TB_TC 					((uint8_t)1)     //热电偶(TC)
#define 	Class_TB_RTD  					((uint8_t)2)     //热电阻(RTD)
#define 	Class_TB_Pyrometer 				((uint8_t)3)     //高温计
#define 	Class_TB_TCDCU					((uint8_t)16)     //TC+DC u(直流电压)
#define 	Class_TB_RTDR 					((uint8_t)17)     //RTD+R(R电阻)
#define 	Class_TB_TCRTDRDCU  			((uint8_t)18)     //TC+RTD+R+DCU
//Flow
#define 	Class_TB_Electromagnetic 		((uint8_t)1)     //电磁
#define 	Class_TB_Vortex 				((uint8_t)2)     //涡街
#define 	Class_TB_Coriolis 				((uint8_t)3)     //科氏力质量
#define 	Class_TB_ThermalMass 			((uint8_t)4)     //热式质量
#define 	Class_TB_Ultrasonic 			((uint8_t)5)     //超声波
#define 	Class_TB_VariableArea 			((uint8_t)6)     //可变截面
#define 	Class_TB_DifferentialPressure 	((uint8_t)7)     //差压
//Level
#define 	Class_TB_Hydrostatic 			((uint8_t)1)     //流体静压
#define 	Class_TB_EchoLevel 				((uint8_t)2)     //回波液位
#define 	Class_TB_Radiometric 			((uint8_t)3)     //放射式
#define 	Class_TB_Capacity 				((uint8_t)4)     //电容式
//Actuator
#define 	Class_TB_Electric 				((uint8_t)1)     //电动
#define 	Class_TB_ElectroPneumatic 		((uint8_t)2)     //电气动
#define 	Class_TB_ElectroHydraulic 		((uint8_t)3)     //电液动
//Discrete I/O
#define 	Class_TB_SensorInput  			((uint8_t)1)     //传感器输入
#define 	Class_TB_Actuator 				((uint8_t)2)     //执行器
//Analyzer
#define 	Class_TB_Standard 				((uint8_t)1)     //标准
//Auxiliary
#define 	Class_TB_Transfer 				((uint8_t)1)     //传送
#define 	Class_TB_Control 				((uint8_t)2)     //控制
#define 	Class_TB_Limit 					((uint8_t)3)     //限值
//Alarm
#define 	Class_TB_BinaryMessage 			((uint8_t)1)     //二进制消息


//Profile OctetString
#define 	Profile_Code_HiByte 			((uint8_t)0x40)     //Bytel(MSB),在PI行规类型中PROFIBUS PA行规的编号为64，即0x40
#define 	Profile_Code_LoByteLClassA 		((uint8_t)0x01)     //Byte2(LSB), 最高比特不置位意味着所有标准参数都有其自己 的存储位置
#define 	Profile_Code_LoByteLClassB 		((uint8_t)0x02)     //Byte2(LSB), 最高比特不置位意味着所有标准参数都有其自己 的存储位置
#define 	Profile_Code_LoByteHClassA 		((uint8_t)0x81)     //Byte2(LSB), 最高比特置位意味着将标准参数ST—REV、TAGDESC、STRATEGY和ALERT—KEY映射在一个存储位置中
#define 	Profile_Code_LoByteHClassB 		((uint8_t)0x82)     //Byte2(LSB), 最高比特置位意味着将标准参数ST—REV、TAGDESC、STRATEGY和ALERT—KEY映射在一个存储位置中
#define 	Profile_Code_LoByteSpecial 		((uint8_t)253)      //Byte2(LSB), 制造商特定的块结构

#define MAKE_PROFILECODE(hi,lo) 			((hi << 8) + lo)

typedef  struct
{
	uint8_t Reserved;   		//保留缺省值=250(未使用)
	uint8_t Block_Object;		//块对象
	uint8_t Parent_Class;		//父类
	uint8_t Class;       		//分类
	uint16_t Dev_Rev ;          //见5．5．4．2．1，对于设备的所有块都相同
	uint16_t Dev_Rev_Comp;		//见5．5．4．2．1，对于设备的所有块都相同
	uint16_t DD_Revision;		//将来使用
	uint16_t Profile;           //编码见表9 Profile OctetString
	uint16_t Profile_Revision;  //编码如下：
								//Bytel(MSB)：十进制小数点之前的数字；范围为00～99
								//Byte2(LSB)：十进制小数点之后的数字；范围为o～255
								//MSB表示NAMUR NE53的第1个数字，I．SB表示NAMUR NE53的第2个和第3个
								//数字：
								//例如对于行规3．02：MsB—Ox03，LSB=0x02
	uint8_t Execution_Time;		//供将来使用
	uint16_t Number_of_Parameters;//块所用的相对索引(参数)的个数，包括：
									//…块中必备部分内的问隔(gap)；
									//——可选参数；
									//——保留参数；
									//一制造商特定参数；
									//块中制造商特定部分内的间隔(gap)。
									//Number_ofParameters不包含视图对象
	uint16_t Address_of_View_1;		//用于访问View_l参数(见5．2．1．1)的引用。此参数值的含义是通信特定的，并在本行规映射部分定义
	uint8_t Number_of_Views;		//如果块中存在除View一1视图对象以外的其他视图对象。则此参数包含该块中所有视图对象的个数，包括View一1和保留的视图对象
}_PACKED_ PA_BlockDS32_t ;

/*
 * 5．2．3．3值&状况一浮点(Value&Status-Floating Point)结构(101)
 */
typedef struct
{
	float Value;
	uint8_t Status;
}_PACKED_ PA_ValueStatusFP101_t;

/*
 * 5．2．3．4值＆状况一离散(Value&Status-Discrete)结构(102)
 */
typedef struct
{
	uint8_t Value;
	uint8_t Status;
}_PACKED_ PA_ValueStatusDS102_t;

/*
 * 5．2．3．5定标(Scaling)结构(DS一36)
 */
typedef struct
{
	float Eu_at_100;
	float Eu_at_0;
	uint16_t units_Index;//Units—Index的代码见5．3。
	int8_t  DecimalPoint;//Decimal～Point是备注，说明小数点后几位数字有效。它可用于主站工具和本地显示。
}_PACKED_ PA_ScalingDS36_t;

/*
 * 5．2．3．6模式(Mode)结构(DS一37)
 */
typedef struct
{
	uint8_t actual;
	uint8_t permitted;
	uint8_t normal;
}_PACKED_ PA_ModeDS37_t;

/*
 * 5．2．3．7报警浮点(Alarm Float)结构(DS-39)
 */
typedef struct
{
	uint8_t unacknowledged;
	uint8_t alarm_state;
	PA_TimeValue21_t time_stamp;
	uint16_t  subcode;
	float value;
}_PACKED_ PA_AlarmFloatDS39_t;

/*
 * 5．2．3．8报警汇总(Alarm Summary)结构(DS-42)
 */
#define PA_ALARMSUMB0_Discrete_Alarm  0x01 //仅具有离散眼值参数的功能块
#define PA_ALARMSUMB0_HI_HI_Alarm     0x02 //仅具有模拟限值参数的功能块
#define PA_ALARMSUMB0_HI_Alarm        0x04 //仅具有模拟限值参数的功能块
#define PA_ALARMSUMB0_LO_LO_Alarm     0x08 //仅具有模拟限值参数的功能块
#define PA_ALARMSUMB0_LO_Alarm        0x10 //仅具有模拟限值参数的功能块
#define PA_ALARMSUMB0_UpdateEvent     0x80 //具有存储属性S的块参数被修改

#define PA_ALARMSUMB1_BIT_OFFSET          (0x08) //16位的高字节

typedef struct
{
	uint8_t current[2]; //如果报警原因出现(1)或消失(o)，则将限值报警(Limit alarm)比特设为1或0。在任何
						//具有存储属性S的块参数被修改之后，应将更新事件(update event)比特设为1，并且在
						//20 S后(浓缩状况下)或10 S后(经典状况下)将该比特设为0。
						//注：某些报警原因被映射到循环状况报告
	uint8_t unacknowledged[2];
	uint8_t unreported[2];
	uint8_t disabled[2];
}_PACKED_ PA_AlarmSummaryDS42_t;

/*
 * 5．2．3．9功能块链接(FB Linkage)结构(DS-49)
 * 更多细节见5．2．9。
 */
typedef struct
{
	uint16_t Local_Index;//规定输出参数的FB_ID和参数相对偏移量，该输出参数被链接到Remote—Index标识的输人参数。这是该链接的源
	uint16_t Connection_Number;//设置为0，以标识这是设备内的一个链接
	uint16_t Remote_Index;//规定输入参数的FB ID和参数相对偏移量，该输人参数被链接到Local—Index标识的参数(仅对本地链接有效)。这是该链接的目标
	uint8_t Service_Operation;//决定链接的动作。
							//编码：
							//0： 无服务——链接是无效的；
							//1： 本地——设备内的链接；
							//2～127： 保留；
							//128～255：制造商特定
	uint8_t Stale_Count_Limit;//设置为0，不需要时效性检测
}_PACKED_ PA_FBLinkageDS49_t;

/*
 * 5．2．3．10仿真一浮点(Simulation—Floating Point)结构(Ds_50)
 */
typedef struct
{
	uint8_t Simulate_Status;
	float Simulate_Value;
	uint8_t Simulate_Enabled;
}_PACKED_ PA_SimuFloatPtDS50_t;


/*
 * 5．2．3．11仿真一离散(Simulation-Discrete)结构(DS一51)
 */
typedef struct
{
	uint8_t Simulate_Status;
	uint8_t Simulate_Value;
	uint8_t Simulate_Enabled;
}_PACKED_ PA_SimuDiscreteDS51_t;

/*
 * 5．2．3．12结果(Result)结构(DS一60)
 */
typedef struct
{
	float PVFloat;//包含转换块的结果的值。在相同转换块中，包含解释此值的伴随参数
	uint8_t Measurement_Status ;//值生成时结果的状态(见5．3．2／5．3 4)
	Date_t  PV_Time;//生成PV的时间
}_PACKED_ PA_ResultDS60_t;

/*
 * 5．2．3．13测量范围(Measurement Range)结构(Ds_61)
 */

typedef struct
{
	float Begin_of_Range;
	float End_of_Range;
}_PACKED_ PA_MeasurRangeDS61_t;

/*
 * 5．2．3．14二进制消息(Binary Megage)结构(DS一62)
 */

typedef struct
{
	uint16_t Status_Class;//共有16个状况类(status class)，本行规定义了前4类。每个二进制消息(BM)可在一个或多
							//个状况类中被引用。比特位置的编号(从1开始)是对该状况类的引用。Bitn=1且此BM是
							//有效的，则表示该状况类的总和比特(Bitl6)被设为1，在GLOBAL—STATUS中的相关Bitn
							//被设为1，且相关ACTIVE—BM中的BM比特也被设为1(见“分析仪器数据单”)
	Boolean Logbook_Entry;//二进制消息及其时阃戳可被一起保存在日志(Logbeok)FB中。此参数启用或禁用将BM存储在日志中。
							//编码：
							//False 不存储在Logbook中
							//True 存储在Logbook中
	uint8_t Output_Reference;//每个BM只可与一个离散输出(DO)相关。OUTPUT—REFERENCE值是设备中所连接的DO的编号
	uint8_t Supervision;//如果将此参数切换至打开监视，则该BM立即为有效的。关闭监视可选择将BM设为有效的
						//或无效的．与BM中有无消息无关。
						//编码：
						//0 关闭监视，消息是无效的
						//1 关闭监视，消息是有效的
						//2 打开监视
	char Text_VisibleString[16];//此参数包含ASCII文本，终端站或可视站可使用该文本来对所编码的消息提供解释和更多信
								//息。由于将来系统可能使用设备描述技术，所以此参数是可选的
}_PACKED_ PA_BinaryMegageDS62_t;

/*
 * 5．2．3．15采样选择(Sample Selection)结构(DS-63)
 */
typedef struct
{
	uint16_t Channel;//对正在向块提供测量值的转换块的引用
	uint32_t Active_Sample_Time ; //在设备中执行该采样的整个时间TimeDifrerance,以毫秒为单位
}_PACKED_ PA_SampleSelectionDS63_t;

/*
 * 5．2．3．16 日志(Logbook)结构(DS一64)
 */
typedef struct
{
	uint8_t Type; //编码：
					//0： GlobaL Status
					//1～16：类n的状况信息
					//255： Binary Message
	uint16_t Value; //该值的解释取决于Type的内容：
			//Type=0 )值=Global_Status
			//Type--l～16 )值一一种类的类状态的逻辑“或”运算的结果
			//Type=255 )值一二进制消息的编号
	Boolean Active;//编码：
					//True BM变为有效的
					//False BM变为无效的
	Date_t Time;
}_PACKED_ PA_SampleSelectionDS64_t;

/*
 * 5．2．3．17预计算(Precalculation)结构(DS-65)
 */
typedef struct
{
	uint8_t Function_Type;//此参数包含所使用的功能类型的选择，这些功能类型在预计算链中被激活。
							//编码；
							//0： 无预计算功能
							//1： 滤波
							//2： 平均值
							//3： 积分
							//4： 校正
							//5～127： 保留
							//128～255： 设备特定
	uint8_t Subtype;//包含在设各特定编码中特殊的预计算功能，如滤波、平均值、积分或校正。缺省值1表示该设
					//备的标准方法。设备使用手册包含该特定算法的描述。
					//编码：
					//0：无预计算
					//1：设备特定的标准算法
					//2～255：设备特定
	uint8_t Choice;//此参数选择校正功能是未被激活的，还是使用一个固定值或其他块的结果。
					//编码：
					//0： 功能未被激活
					//1： 功能使用预计算链的结果
					//2： 功能使用一个固定值
					//3： 功能使用一个功能块的值
					//4： 功能使用一个转换块的值
}_PACKED_ PA_PrecalculationDS65_t;

/*
 * 5．2．3．18顺序控制(Sequential Contr01)结构(DS-66)
 */
typedef struct
{
	Date_t Time; //决定相关块的第1个／下个执行时间。此参数可决定一个循环执行周期的开始
	uint32_t Cycle_Time;//决定相关块的自动执行的时间间隔。Cycle_Time值为0表示一个非循环执行, TimeDifference,单位ms
	uint16_t Command;//此参数包含影响相关块的命令的代码。
						//控制转换块还包含一个COMMAND参数，该块命令参数比此结构的命令参数具有更高优先级。
						//编码：
						//5： 开始(Start)
						//6： 停止(Stop)
						//7： 继续(重新开始)
						//8： 取消(Cancel)
						//9： 由设备内部事件触发的开始
						//10～127； 保留
						//128～255： 制造商特定
	Boolean Time_Control_Active;//此参数决定该命令是被自动执行，还是该命令无结果。
								//编码：
								//False： 停止执行
								//True： 执行
}_PACKED_ PA_SequeContr01DS66_t;

/*
 * 5．2．3．19批(Batch)结构(DS一67)
 */
typedef struct
{
	uint32_t Batch_ID;//标识某个批量以允许向该批量分配与设备有关的信息(例如：故障、报警等)。
	uint16_t Rup;//配方单元规程(Recipe Unit Procedure)或单元(Unit)婀编号：标识有效的控制配方单元规程
				//(ControlRecipe Unit Procedure)或相关单元(例如：反应器、离心机、干燥机)。(un．t在
				//GB／T 1 9892．1 2005中定义．但其作为参数UNIT具有不同的含义，即工程单位。)
	uint16_t Operation;//配方操作(Recipe Operation)的编号：标识有效的控制配方操作(Control Recipe Operation)
	uint16_t Phase;//配方阶段(Recipe Phase)的编号：标识有效的控制配方阶段(Control Recipe Phase)
}_PACKED_ PA_BatchDS67_t;


/*
 * 5．2．3．20特性(Feature)结构(DS 68)
 */
//表41所支持特性(Supported)的编码 八位位组1
#define FeatureSupportB0_CondensedStatus   0x01  //定义整个设备处理状况和诊断的一般方法。
										         //0： 不支持浓缩状况和诊断
											     //1： 浓缩状况和诊断信息符合5．3．4中的定义
#define FeatureSupportB1_ClassicStatusDiagnosis   0x02 //定义整个设备处理状况和诊断的一般方法。
												//0： 不支持5．3．2中定义的经典状况／诊断
												//1： 支持5．3．2中的定义
#define FeatureSupportB2_DxB   0x04 //0： 不支持广播数据交换
									//1： 支持广播数据交换
#define FeatureSupportB3_MS1AR   0x08 //0: 不支持MSl应用关系
									//1: 支持MSl应用关系
#define FeatureSupportB4_PROFISAFE   0x10 //0： 不支持PROFIsafe通信
											//1： 支持PROFlsafe通信
//表42所启用特性(Enabled)的编码
#define FeatureEnabledB0_CondensedStatus   0x01  //定义整个设备处理状况和诊断的一般方法。
										         //0： 不支持浓缩状况和诊断
											     //1： 浓缩状况和诊断信息符合5．3．4中的定义
#define FeatureEnabledB1_ClassicStatusDiagnosis   0x02 //定义整个设备处理状况和诊断的一般方法。
												//0： 不支持5．3．2中定义的经典状况／诊断
												//1： 支持5．3．2中的定义
#define FeatureEnabledB2_DxB   0x04 //0： 不支持广播数据交换
									//1： 支持广播数据交换
#define FeatureEnabledB3_MS1AR   0x08 //0: 不支持MSl应用关系
									//1: 支持MSl应用关系
#define FeatureEnabledB4_PROFISAFE   0x10 //0： 不支持PROFIsafe通信
											//1： 支持PROFlsafe通信
typedef struct
{
	uint8_t Supported[4];
	uint8_t Enabled[4];
}_PACKED_ PA_FeatureDS68_t;

/*
 * 5．2．3．21 诊断事件转换(Diag_Event_Switch)结构
 */
#define  DiagStaLK_GOODOK           	0x00 //好，ok
#define  DiagStaLK_GOODMaintenReq   	0x01 //好，需要维护
#define  DiagStaLK_GOODMaintenDem   	0x02 //好，必须维护
#define  DiagStaLK_UNCTMaintenDem   	0x03 //不确定，立即维护请求
#define  DiagStaLK_BADMaintenAlarm  	0x04 //坏，维护报警，故障
#define  DiagStaLK_UNCTProcNoMaint  	0x05 //不确定过程相关，无维护,无效的过程条件
#define  DiagStaLK_BADProcNoMaint  		0x06 //坏过程相关，无维护,无效的过程条件
#define  DiagStaLK_BADFuncChecLocalOv   0x07 //坏一功能检查／本地超驰,功能检查
#define  DiagStaLK_GOODFuncChec         0x08 //好功能检查,功能检查

#define  DiagStaLK_DIA_MAINTENANCE 			    0x10 //检查请求
#define  DiagStaLK_DIA_MAINTENANCE_DEMANDED 	0x20 //立即检查请求
#define  DiagStaLK_DIA_MAINTENANCE_ALARM 		0x30 //故障
#define  DiagStaLK_DIA_INV_PRO_COND 			0x40 //无效的过程条件
#define  DiagStaLK_DIA_FUNCTION_CHECK 			0x50 //功能检查

typedef struct
{
	uint8_t Diag_Status_Link[48]; //用于设备特定诊断事件的转换数组。映射到诊断比特和状况代码。每个诊断事件对应一个字
	//节。如果一个诊断事件发生，则此参数指出状况和诊断被如何影响。
	//在多变量设备或多通道设备的情况下，它(通常)是没有用的。这是因为在仅影响一个通道的传
	//感器故障的情况下(例如，温度传感器正常工作，而流量传感器报告一个故障)，所有FB都以相
	//同方式作出反应。该设置应仅影响相关通道(由制造商规定的)的状况。
	//状况代码(低四位)：
	//0：诊断事件对该状况无影响。
	//Status应是GooD-ok
	//1： 诊断事件被作为维护请求来处理。
	//Status应是GooD-maintenance required。
	//2：诊断事件被作为立即维护请求来处理。
	//Status应是GooD-maintenance demanded。
	//3：诊断事件被作为立即维护请求来处理。
	//Status应是UNCERTAIN—maintenance demanded。
	//4：诊断事件被作为故障来处理。
	//Status应是BADmaintenance alarm。
	//5： 诊断事件被作为无效过程条件来处理。该值是有条件使用的。
	//Status应是UNCERTAlN—process related，no maintenance。
	//6：诊断事件被作为无效过程条件来处理。该值不可用。
	//Status应是BAD-process related，no maintenance。
	//7： 诊断事件被作为无可用值的功能检查来处理。
	//Status应是BAD-function check／local override
	//8： 诊断事件被作为有可用值的功能检查来处理。
	//Status应是GOOD-function check。
	//9～15：保留
	//诊断(高四位)：
	//0： 诊断事件对该诊断无影响。
	//DIAGNOSIS：不将其他比特置位。
	//l：诊断事件被作为维护请求来处理。
	//D1AGNOSlS：应将DIA_MAINTENANCE置位。
	//2：诊断事件被作为立即维护请求来处理。
	//DIAGNOSIS：应将 DIA_MAINTENANCE_DEMAND置位。
	//3：诊断事件被作为故障来处理。
	//D1AGNOSIS：应将DIA_MAINTENANCE_ALARM置位。
	//4：诊断事件被作为无效过程条件来处理。
	//DIAGN()SIS：应将DIA_INV_PRO_COND置位。
	//5： 诊断事件被作为功能检查或仿真来处理。
	//DIAGNOSIS：应将DIA_FUNCTION_CHECK置位。
	//6～15：保留
	//如果设备具有要链接的诊断事件小于48个，则剩余的D1AG_STATUs_LINK字节全部设为0。
	//每个诊断事件所支持的枚举可由制造商来限定
	uint8_t Slot; //后续Diag_Event Switch的槽。指向下一个Diag_Event_Switch结构。
	//如果该Diag_Event Switches无后续，则元素Slot的值为0且同时元素Index值为0
	uint8_t Index_absolute;//后续Diag_Event_Switch的(绝对)索引。指向下一个Diag_Event_Switch结构。
	//如果该Diag—Event—Switches无后续，则元素SLot值为0且同时元素Index值为0
}_PACKED_ PA_DiagEventSwitch_t;



/*
 * -----------------------------------------
 * 5．2．4标准参数定义
 */
#define PA_DEVIECEMODE_OS    0x80  //Bit7： 非服务(Out of Service (O/S) - MSB
#define PA_DEVIECEMODE_IMAN  0x40  //Bit6： 手动初始化(Initialisation Manually，IMan)(在A类和B类中不使用)
#define PA_DEVIECEMODE_LO    0x20  //Bit5： 本地超驰(Local Override，LO)(在A类中不使用)
#define PA_DEVIECEMODE_MAN   0x10  //Bit4： 手动(Manual，MAN)
#define PA_DEVIECEMODE_AUT   0x08  //Bit3： 自动(Automatic，AUT())
#define PA_DEVIECEMODE_CAS   0x04  //Bit2： 级联(Cascade，Cas)(在A类和B类中不使用)
#define PA_DEVIECEMODE_RCAS  0x02  //Bitl： 远程级联(Remote-Cascade，RCas)
#define PA_DEVIECEMODE_ROUT  0x01  //Bit0： 远程输出(Remote-Output，ROut)——LSB(在A类和B类中不使用)

typedef struct {
	__Def_ParameterAttr(PA_BlockDS32_t* ,pBLOCK_OBJECT); //BLOCK_OBJECT参数是每个块的第1个参数。它包含该块的特性，例如，块类型和行规号

	__Def_ParameterAttr(uint16_t* ,		pST_REV) ;      //块具有不被过程修改的静态块参数，在组态或优化期间向这些参数赋值。如果块中至少有
									//一个静态参数被修改，则应增加相应的ST_REV(至少按l增加)。这就提供了对参数版本
									//的检查。在冷启动的情况下(即设置FACTORY—RESET=1)，则ST_REV应被复位为0或
									//至少按1增加．以指示静态参数的改变。另外，如果接受了一个表的修改，则ST_REV应被
									//增加。ST_REV的值可被组态设备用来判定存储在静态存储器中的块参数(在参数属性表
									//中具有"S"属性定义)是否其值巳被改变。
									//在ST_REV溢出的情况下应被设置为l
	__Def_ParameterAttr(char* ,			pTAG_DESC);//该标签描述是用户提供的块描述。可给每个块分配一个文本化标签描述
	__Def_ParameterAttr(uint16_t* ,		pSTRATEGY);//STRATEGY参数具有用户特定的值。此分配值可在组态或诊断中用作分类块信息的关键词
	__Def_ParameterAttr(uint8_t* ,		pALERT_KEY);//ALERT_KEY参数具有用户分配的值。此值可用于对由块产生的报警或事件进行分类。它
								//可包含装置单元的标识号，以帮助识别事件在装置单元中的位置
	__Def_ParameterAttr(uint8_t* ,		pTARGET_MODE);//TARGET_MODE参数指出对该块所期望的操作模式。它通常由控制应用或由操作员通过
								//人机接口应用来设置。块输人参数与块状态的结合用来判定该块能否实现要求的目标模式。
								//在MODE_BLK参数的Permitted元素所允许的模式中，只可请求其中一种模式。对此参
								//数进行多于一种模式的写访问超出了该参数的范围，必须予以拒绝。
								//编码：
								//Bit7： 非服务(Out of Service (O/S) - MSB
								//Bit6： 手动初始化(Initialisation Manually，IMan)(在A类和B类中不使用)
								//Bit5： 本地超驰(Local Override，LO)(在A类中不使用)
								//Bit4： 手动(Manual，MAN)
								//Bit3： 自动(Automatic，AUTO)
								//Bit2： 级联(Cascade，Cas)(在A类和B类中不使用)
								//Bitl： 远程级联(Remote-Cascade，RCas)
								//Bit0： 远程输出(Remote-Output，ROut)——LSB(在A类和B类中不使用)
								//在本行规中使用的“automatic”模式包括Auto和Rcas，“manual”模式包括LO和Man。在 O/S模式下，不再执行正常算法
	__Def_ParameterAttr(PA_ModeDS37_t*, 	pMODE_BLK);//MODE_BLK参数是一个结构化参数，由实际模式(actual mode)、正常模式(normal mode)
							//	和允许模式(permitted mode)组成。实际模式由该块在其执行期间进行设置(通过计算)，
							//	以反映块在执行期间所采用的模式。正常模式是对该块所期望的操作模式。
							//	允许模式向MODE_BLK参数的远程用户指出目标模式的哪种改变是对特定块有效的。
							//	模式对功能块操作的作用总结如下：
							//	——Out of Service(O／s)：
							//	·转换块：在o／s模式下，停止对测量值和输出值的赋值。
							//	·功能块：在o／s模式下，停止对测量值和输出值的赋值。
							//	·物理块：停止将DIAGNOSIS和DIAGNOSIS EXTENSION参数内容复制到PROFIBUS DP的Slave_Diag服务。
							//	非循环参数的访问不受影响。
							//	输人功能块的输出值应保持为上一个值。对于输出功能块，为转换块提供信息的输出
							//	参数应保持为掉电时所规定的值。这与故障安全处理的定义无关。
							//	——Local Override(LO)：适用于支持跟踪输人参数的控制块和输出块。同样，制造商可提
							//	供对设备的本地锁定开关以启用LO模式。在锁定模式下，块输出被设置为跟踪输入
							//	参数的值。应初始化算法(对模拟设备)，使得在从LO模式切换到目标模式时不会经
							//	历突变。
							//	——Manual(MAN)：尽管可以限制块输出，但不计算块输出。块输出由操作员通过接口设
							//	备直接设置。应初始化算法，使得在模式切换时不会经历突变。
							//	——Automatic(AUTO)：计算块输出。对于输入功能块，使用来自转换块的输人进行计算；
							//	对于输出功能块，使用由主机提供或操作员通过接口设备提供的设定值进行计算；对
							//	于物理块和转换块，此模式表示其块功能能够正常工作
							//——Remote Cascade(RCas)：块的设定值由控制应用通过远程级联参数RCAS—IN来设置。
							//根据该设定值，正常块算法决定主输出值。
							//功能块、物理块或转换块的行为通过MODE_BLK参数来指示。MODE_BLK元素的编码
							//同TARGET_MODE，并定义如下：
							//1．实际模式(Actual)：这是块的当前模式，可根据运行条件不同于TARGET MODE，该值
							//的计算是块执行的一部分。
							//当功能块处于阻止其运行于目标模式的条件下时，功能块的实际模式将自动改变。根据以
							//下列项进行实际模式的计算：
							//·TARGET_MODE参数；
							//·该块的模式计算。
							//当块计算不同于TARGET_MODE的实际模式时，应使用优先级概念。模式优先级定义如
							//下，其中0代表最低优先级：
							//·O／S：优先级为7，最高；
							//·IMan；优先级为6(未使用)；
							//·LO：优先级为5}
							//·MAN：优先级为4；
							//·AUT0：优先级为3；
							//·Cas：优先级为2(未使用)；
							//·RCas：优先级为1；
							//·ROut：优先级为0，最低(未使用)。
							//定义MODE计算细节的状态机是块类型特定的(见数据单)。
							//2．允许模式(Permitted)：定义块实例所允许的模式。允许模式由块的设计人员进行组态，
							//即在相应的数据单中对每个块进行定义。设备应检查任何模式改变的请求，以确保所请求
							//的目标模式在允许模式中有定义。
							//3．正常模式(Normal)：这是在正常操作条件下块使用的模式。此参数可由接口设备来读
							//取，但不被块算法使用。正常模式不在本行规范围内使用．供将来使用。
							//A类设备的块至少提供“AUTO”模式作为必备模式。模式计算仅对于B类设备的功能块
							//是必备的
	__Def_ParameterAttr(PA_AlarmSummaryDS42_t*, pALARM_SUM );//参数ALARM～SUM汇总了最多16个块报警的状况。对于每个报警都保存其当前状态、未
										//确认状态、未报告状态和禁用状态。
										//注：实际行规不完全支持此特性。对于本行规，只使用报警的当前状态部分
	//__Def_ParameterAttr(PA_BatchDS67_t*, pBATCH );//此参数旨在用于符合GB／T 19892．1—2005的批应用。仅功能块具有此参数。在功能块内
							//不需要算法。在分布式现场总线系统中BATCH参数是必备的，用以识别所用的和可用的
							//通道。此外．在警报的情况下还识别当前的批。详见5 2．3．19
} _PACKED_ IDX_STDParameters_t;

typedef struct {
	PA_BlockDS32_t 	BLOCK_OBJECT; //BLOCK_OBJECT参数是每个块的第1个参数。它包含该块的特性，例如，块类型和行规号

	uint16_t 		ST_REV ;      //块具有不被过程修改的静态块参数，在组态或优化期间向这些参数赋值。如果块中至少有
									//一个静态参数被修改，则应增加相应的ST_REV(至少按l增加)。这就提供了对参数版本
									//的检查。在冷启动的情况下(即设置FACTORY—RESET=1)，则ST_REV应被复位为0或
									//至少按1增加．以指示静态参数的改变。另外，如果接受了一个表的修改，则ST_REV应被
									//增加。ST_REV的值可被组态设备用来判定存储在静态存储器中的块参数(在参数属性表
									//中具有"S"属性定义)是否其值巳被改变。
									//在ST_REV溢出的情况下应被设置为l
	char 			TAG_DESC[32];//该标签描述是用户提供的块描述。可给每个块分配一个文本化标签描述
	uint16_t 		STRATEGY;//STRATEGY参数具有用户特定的值。此分配值可在组态或诊断中用作分类块信息的关键词
	uint8_t 		ALERT_KEY;//ALERT_KEY参数具有用户分配的值。此值可用于对由块产生的报警或事件进行分类。它
								//可包含装置单元的标识号，以帮助识别事件在装置单元中的位置
	uint8_t 		TARGET_MODE;//TARGET_MODE参数指出对该块所期望的操作模式。它通常由控制应用或由操作员通过
								//人机接口应用来设置。块输人参数与块状态的结合用来判定该块能否实现要求的目标模式。
								//在MODE_BLK参数的Permitted元素所允许的模式中，只可请求其中一种模式。对此参
								//数进行多于一种模式的写访问超出了该参数的范围，必须予以拒绝。
								//编码：
								//Bit7： 非服务(Out of Service (O/S) - MSB
								//Bit6： 手动初始化(Initialisation Manually，IMan)(在A类和B类中不使用)
								//Bit5： 本地超驰(Local Override，LO)(在A类中不使用)
								//Bit4： 手动(Manual，MAN)
								//Bit3： 自动(Automatic，AUTO)
								//Bit2： 级联(Cascade，Cas)(在A类和B类中不使用)
								//Bitl： 远程级联(Remote-Cascade，RCas)
								//Bit0： 远程输出(Remote-Output，ROut)——LSB(在A类和B类中不使用)
								//在本行规中使用的“automatic”模式包括Auto和Rcas，“manual”模式包括LO和Man。在 O/S模式下，不再执行正常算法
	PA_ModeDS37_t 	MODE_BLK;//MODE_BLK参数是一个结构化参数，由实际模式(actual mode)、正常模式(normal mode)
							//	和允许模式(permitted mode)组成。实际模式由该块在其执行期间进行设置(通过计算)，
							//	以反映块在执行期间所采用的模式。正常模式是对该块所期望的操作模式。
							//	允许模式向MODE_BLK参数的远程用户指出目标模式的哪种改变是对特定块有效的。
							//	模式对功能块操作的作用总结如下：
							//	——Out of Service(O／s)：
							//	·转换块：在o／s模式下，停止对测量值和输出值的赋值。
							//	·功能块：在o／s模式下，停止对测量值和输出值的赋值。
							//	·物理块：停止将DIAGNOSIS和DIAGNOSIS EXTENSION参数内容复制到PROFIBUS DP的Slave_Diag服务。
							//	非循环参数的访问不受影响。
							//	输人功能块的输出值应保持为上一个值。对于输出功能块，为转换块提供信息的输出
							//	参数应保持为掉电时所规定的值。这与故障安全处理的定义无关。
							//	——Local Override(LO)：适用于支持跟踪输人参数的控制块和输出块。同样，制造商可提
							//	供对设备的本地锁定开关以启用LO模式。在锁定模式下，块输出被设置为跟踪输入
							//	参数的值。应初始化算法(对模拟设备)，使得在从LO模式切换到目标模式时不会经
							//	历突变。
							//	——Manual(MAN)：尽管可以限制块输出，但不计算块输出。块输出由操作员通过接口设
							//	备直接设置。应初始化算法，使得在模式切换时不会经历突变。
							//	——Automatic(AUTO)：计算块输出。对于输入功能块，使用来自转换块的输人进行计算；
							//	对于输出功能块，使用由主机提供或操作员通过接口设备提供的设定值进行计算；对
							//	于物理块和转换块，此模式表示其块功能能够正常工作
							//——Remote Cascade(RCas)：块的设定值由控制应用通过远程级联参数RCAS—IN来设置。
							//根据该设定值，正常块算法决定主输出值。
							//功能块、物理块或转换块的行为通过MODE_BLK参数来指示。MODE_BLK元素的编码
							//同TARGET_MODE，并定义如下：
							//1．实际模式(Actual)：这是块的当前模式，可根据运行条件不同于TARGET MODE，该值
							//的计算是块执行的一部分。
							//当功能块处于阻止其运行于目标模式的条件下时，功能块的实际模式将自动改变。根据以
							//下列项进行实际模式的计算：
							//·TARGET_MODE参数；
							//·该块的模式计算。
							//当块计算不同于TARGET_MODE的实际模式时，应使用优先级概念。模式优先级定义如
							//下，其中0代表最低优先级：
							//·O／S：优先级为7，最高；
							//·IMan；优先级为6(未使用)；
							//·LO：优先级为5}
							//·MAN：优先级为4；
							//·AUT0：优先级为3；
							//·Cas：优先级为2(未使用)；
							//·RCas：优先级为1；
							//·ROut：优先级为0，最低(未使用)。
							//定义MODE计算细节的状态机是块类型特定的(见数据单)。
							//2．允许模式(Permitted)：定义块实例所允许的模式。允许模式由块的设计人员进行组态，
							//即在相应的数据单中对每个块进行定义。设备应检查任何模式改变的请求，以确保所请求
							//的目标模式在允许模式中有定义。
							//3．正常模式(Normal)：这是在正常操作条件下块使用的模式。此参数可由接口设备来读
							//取，但不被块算法使用。正常模式不在本行规范围内使用．供将来使用。
							//A类设备的块至少提供“AUTO”模式作为必备模式。模式计算仅对于B类设备的功能块
							//是必备的
	PA_AlarmSummaryDS42_t ALARM_SUM ;//参数ALARM～SUM汇总了最多16个块报警的状况。对于每个报警都保存其当前状态、未
										//确认状态、未报告状态和禁用状态。
										//注：实际行规不完全支持此特性。对于本行规，只使用报警的当前状态部分
	//PA_BatchDS67_t BATCH ;//此参数旨在用于符合GB／T 19892．1—2005的批应用。仅功能块具有此参数。在功能块内
							//不需要算法。在分布式现场总线系统中BATCH参数是必备的，用以识别所用的和可用的
							//通道。此外．在警报的情况下还识别当前的批。详见5 2．3．19
} _PACKED_ PA_STDParameters_t;

#define RLAINDX_BLOCK_OBJECT   0
#define RLAINDX_ST_REV         1
#define RLAINDX_TAG_DESC       2
#define RLAINDX_STAATEGY       3
#define RLAINDX_ALERT_KEY      4
#define RLAINDX_TARGET_MODE    5
#define RLAINDX_MODE_BLK       6
#define RLAINDX_ALARM_SUM      7
#define RLAINDX_BATCH          8



/*
 *表103首部(Slotl．Index0)的数据类型
 */

typedef struct
{
	uint16_t Dir_ID;//保留(Directory ID)：本行规中未使用；
	uint16_t Dir_Rev_No;//目录版本号；
	uint16_t No_Dir_Obj;//目录对象的个数：如果整个目录使用多于一个目录对象，则这些元素被连续定义，就如同使用
						//一个较大的对象。多个目录对象都被连续地列在该目录中。该对象计数整个目录所需的对
						//象。Header对象不计人其中；
	uint16_t No_Dir_Entries;//目录登录项的总数：应计算复合列表目录登录项和复合目录登录项的总个数；
	uint16_t FirstCompListDirEntry;//第1个复合列表目录登录项的目录登录项个数：该数用来计数目录内的登录项，而不包含该登
									//录项的参数地址。第1个目录登录项是在复合列表目录登录项中的物理块引用。在计数登录
									//项时，复合列表目录登录项与复合目录登录项之间无间隔；
	uint16_t NoCompListDirEntry;//复合列表目录登录项的个数：计数设备内的不同块类型(物理块、转换块和功能块)和对象类型
								//(在本标准范围内仅针对链接对象)。
}_PACKED_ PA_DirHeader_t;

/*
 * 5．2．6．2．3复合列表目录登录项(Composite_List_Directory_Entry)
 * a) 物理块的Directory—Entry_Number的指针／物理块个数；
 * b) 第1个转换块的Directory—Entry—Number的指针／转换块个数；
 * c) 第1个功能块的Directory—Entry—Number的指针／功能块个数；
 * d) 第1个链接对象的Directory—Entry—Number的指针／链接对象个数。
 */
typedef struct
{
	uint8_t BeginBlkiIndex;  //Index(目录对象编号）Index is the read and write service attribute Index of this parameter which contains the
							//according COMPOSITE_DIRECTORY_ENTRIES parameter. The valid range is 1 to 13.

	uint8_t BeginBlkOffset; //目录登录项编号Offset is the logical number of COMPOSITE_DIRECTORY_ENTRIES, which is counted within
							//the directory starting with the first COMPOSITE_LIST_DIRECTORY_ENTRY by 1.
	uint16_t Num_Blk;
}_PACKED_ PA_CompListDirectoryEntr_t;

/*
 *和复台目录登录项(Composite_Directory—Entry)
 */
typedef struct
{
	uint8_t  Slot;
	uint8_t  Index;
	uint16_t NumPramater;
}_PACKED_ PA_CompDirectoryEntr_t;


typedef struct
{
	//复合目录首部，定义在slot 1,index 0
	PA_DirHeader_t DirObjHd;
	//复合列表目录登录项，定义在slot 1 , index 1
	PA_CompListDirectoryEntr_t CompLstDirEntr_PB;
	PA_CompListDirectoryEntr_t CompLstDirEntr_TB;
	PA_CompListDirectoryEntr_t CompLstDirEntr_FB;
	PA_CompListDirectoryEntr_t CompLstDirEntr_LO; //可选
	//复合目录登录项，定义在slot 1 , index 2 开始
	PA_CompDirectoryEntr_t   CompDirEntr_PB[NUM_BLK_PB]; //
	PA_CompDirectoryEntr_t   CompDirEntr_TB[NUM_BLK_TB];
	PA_CompDirectoryEntr_t   CompDirEntr_FB[NUM_BLK_FB];
	PA_CompDirectoryEntr_t   CompDirEntr_LO[NUM_BLK_LO];

}_PACKED_ PA_DM_t;

typedef struct
{
	//复合目录首部，定义在slot 1,index 0
	__Def_ParameterAttr(PA_DirHeader_t, pDirObjHd);
	//复合列表目录登录项，定义在slot 1 , index 1
	__Def_ParameterAttr(PA_CompListDirectoryEntr_t,pCompLstDirEntr);
	//复合目录登录项，定义在slot 1 , index 2 开始
	__Def_ParameterAttr(PA_CompDirectoryEntr_t ,pCompDirEntr); //

}_PACKED_ IDX_PA_DM_t;

#define RLAINDX_DIROBJHD   			0
#define RLAINDX_COMPLISTDIR1ENTRY  	1
#define RLAINDX_COMPDIR2ENTRY  	    2

/*
 * 5．2．7表(Table)处理
 *
 */
#define TAB_MIN_NUMBER  10
#define TAB_MAX_NUMBER  10

typedef struct
{
	float x_val;
	float y_val;
}_PACKED_ PA_tab_value_t;

typedef struct {
	__Def_ParameterAttr(uint8_t*, pTab_entry);
		/*
		 * 参数TAB_ENTRY标识该表的哪个元素当前处于参数TAB X Y—VAI．UE中
		 */
	__Def_ParameterAttr(PA_tab_value_t*, pTab_xy_value);
		/*
		 * 参数TA／8_kY—VAI，UE包含该表的一对值
		 */
	__Def_ParameterAttr(uint8_t*, pTab_min_number);
		/*
		 * 由于没备的内部原因(例如为了计算)，有时至少要使用一定个数的登录项。该个数在参
		 * 数TAB-MIN—NUMBER中提供
		 */
	__Def_ParameterAttr(uint8_t*, pTab_max_number);
		/*
		 * TAB-MA)LNUMBER是设备的表中登录项的最大个数(TAB—x—VAI。UE和TAB—Y—
		 * VAI。UE值对的个数)
		 */
	__Def_ParameterAttr(uint8_t*, pTab_op_code);
		/*
		 * 修改设备中的表会影响该设备的测量或执行算法。因此，必须指出开始点和结束点。
			TA／3_OP CODE控制该表的处理：
			0：未初始化
			1； 新操作特性，第1个值(TAnENTRY一1)
			2：保留
			3：上一个值，传输结束，检查表，用新曲线替换旧曲线，更新TAB—ACTUALNUM
			BER
			4：删除由TAB_ENTRY规定的表中的点并减小TAB—AcTuAL—NUMBER(可
			选)。
			5：插入由TAB_x—Y—VALUE定义的点，按TAB-x—VALUE递增顺序对表排序，
			并增加TAB ACTUALNUMBER(可选)
			6： 用实际ENTRY替代表中的点(可选)
			无需开始和停止交互(TAB-oB_CODE 1和TAB—OB—CODE 3)就可能读取一个表或表
			中的某些部分。通过将TAB-ENTRY设为1来指示开始
		 */
	__Def_ParameterAttr(uint8_t*, pTab_status);
		/*
		 * 通常在设备中提供真实性检查。检查的结果在参数TAB_sTATus中指示。
			0：未初始化
			1：正确(新表有效)
			2：非单调递增(旧表有效)
			3：非单调递减(旧表有效)
			4： 所传输的值不够(旧表有效)
			5： 所传输的值太多(旧表有效)
			6：边界斜率(gradient)太高(旧表有效)
			7：值未被接受(旧值有效)
			8： 当前表被装载．在TAl8 OP CODE=l之后且在TAB-OP—CODE=3之前设置
			(对表的其他访问无效，旧值有效)。
			9：排序并检查表(对表的其他访问无效，旧值有效)。
			10～19：保留
			20：非单调递增(表未初始化)
			21： 非单调递减(表未初始化)
			22： 所传输的值不够(表未初始化)
			23： 所传输的值太多(表未初始化)
			24： 边界斜率太高(表未初始化)
			25：值未被接受(表未初始化)
			26： 当前表被装载，在TAB一0P—CODE=1之后且在TAB OP—CODE=3之前设
			置(对表的其他访问无效，表未初始化)
			27：排序并检查表(对表的其他访问无效，表未初始化)
			28～127：保留
			>128：制造商特定
		 */
	__Def_ParameterAttr(uint8_t*, pTab_actual_number);
		/*
		 * 包含表中登录项的实际个数。在完成该表的传输后应对其进行计算
		 */
	__Def_ParameterAttr(uint8_t*, pLin_type);
		/*
		 * 线性化类型。
			0： 非线性化(必备)
			1： 线性化表(可选)
			10： 平方根(可选)
			20： 圆柱形卧式容器(可选)
			21： 球形容器(可选)
			50： 等百分比1：33(可选)
			5l： 反向等百分比(快开)1：33(可选)
			52： 等百分比1：50(可选)
			53： 反向等百分比1：50(可选)
			54： 等百分比1t 25(可选)
			55： 反向等百分比1：25(可选)
			100： RTDPtl0 a—O．003 850(IEC 60751)
			101： RTD Pt50 a一0．003 850(IEC 60751)
			102： RTD Ptl00 a一0．003 850(IEC 60751)
			103： RTD Pt200 a一0．003 850(IEC 60751)
			104： RTD PtS00 a#0．003 850(IEC 60751)
			105： RTD Ptl000 a一0．003 850(IEC 60751)
			106： RTD Ptl0 a一0．003 916(JIS C1604—81)
			107： RTD PtS0 a=0．003 916(JIS C1604—81)
			108： RTD Ptl00 a一0．003 916(JISCl604—81)
			109： RTD Ptl0 a=0．003 920(MIL—T一24388)
			110：u RTD Pt50 a=0．003 920(MlL_T一24388)
			111： RTD Ptl00 a一0．003 920(MIoT-24388)
			112： RTD Pt200 a一0．003 920(MIL-T-24388)
			113： RTD Pt500 a一0．003 920(MII．一T-24388)
			114： RTD Ptl000 a一0．003 920(MIL T 24388)
			115： RTD Ptl00 a一0．003 923(SAMA RC2卜4—1 966)
			116： RTD Pt200 a一0．003 923(SAMA RC21—4—1966)
			117： RTD Ptl00 a一0．003 926(IPTs-68)
			118： RTD Ni50 a一0．006 720(Edison curve#7)
			119： RTD Nil00 a一0．006 720(Edison curve#7)
			120： RTD Nil20 a一0．006 720(Edison curve#7)
			121； RTD Nil000 a置0．006 720(Edison curve#7)
			122： RTD Ni50 a一0．006 180(DIN 43760)
			123： RTD Nil00 a一0．006 180(DIN 43760)
			124： RTD Nil20 a一0．006 180(DIN 43760)
			125： RTD Nil000 a霉0．006 180(DIN 43760)
			126： RTDCul0 a一0．004 270
			127： RTD Cul00 a一0．004 270
			128：TC Type B，Pt30Rh-Pt6Rh(IEC 60584，NIST MN 175，DIN 43710，BS 4937，
			ANSI MC96．1，jis C1602，NF C42—321)
			129：TC Type C(W5)，W5一W26Rh(ASTM E988)
			130：TC Type D(W3)．W3一W25Rh(ASTM E988)
			131： TCTypeE，N110C卜Cu45Ni(IEC 60584，NISTMNl75．DIN 43710．BS 4937，
			ANSI MC96．1，JIS C1602，NF C42—321)
			132：TC Type G(W)，w_W26Rh(ASTM E988)
			133： TC Type J，Fe—Cu45Ni(IEC 60584，NIST MN 175，DIN 43710，BS 4937．
			ANSI MC96．1，JIS C1602，NF C42—321)
			134： TC Type KtNil0C卜Ni5(IEC 60584，NIST MN 175，DIN 43710，BS 4937，
			ANSI MC96．1，JISCl602，NFC42 321)
			135：TC Type NtNil4CrSONlSi(IEC 60584，NIST MN 175，DIN 43710，BS 4937，
			ANSI MC96．1．JIS C1602．NF C42—321)
			136：TCType R，Ptl3Rh Pt(IEC 60584．NIST MN 175．DIN 43710．BS 4937，
			ANSI MC96．1，J1S C1602，NF C42—321)
			137：TCType S，Ptl0Rh—Pt(IEC 60584，N1ST MN 175，DIN 43710，BS 4937，
			ANSI MC96．1，JIS C1602．NF C42—321)
			138：TC Type T，Cu-Cu45Ni(IEC 60584，NIST MN 175，DIN 43710．BS 4937，
			ANSI MC96．1，JIS C1602，NF C42 321)
			139：TC Type L，Fe-CuNi(DIN 43710)
			140：TC Type U，Cu-CuNi(DIN 43710)
			141： TC Type Pt20／Pt40，Pt20Rh—Pt40Rh(ASTM E1751)
			142：TC Type lr／Ir40，Ir-Ir40Rh(ASTM E1751)
			143- TC Platinel II
			144：TC Ni／NiMo
			145～239：保留
			240： 制造商特定
			249： 制造商特定
			250： 未使用
			251： 无意义
			252： 未知
			253： 特殊
			254～255：保留
		 */
}_PACKED_ IDX_TAB_PARAMETER_t;
typedef struct {
	uint8_t Tab_entry;
		/*
		 * 参数TAB_ENTRY标识该表的哪个元素当前处于参数TAB X Y—VAI．UE中
		 */
	PA_tab_value_t Tab_xy_value[MAX_TAB_NUM];
		/*
		 * 参数TA／8_kY—VAI，UE包含该表的一对值
		 */
	uint8_t Tab_min_number;
		/*
		 * 由于没备的内部原因(例如为了计算)，有时至少要使用一定个数的登录项。该个数在参
		 * 数TAB-MIN—NUMBER中提供
		 */
	uint8_t Tab_max_number;
		/*
		 * TAB-MA)LNUMBER是设备的表中登录项的最大个数(TAB—x—VAI。UE和TAB—Y—
		 * VAI。UE值对的个数)
		 */
	uint8_t Tab_op_code;
		/*
		 * 修改设备中的表会影响该设备的测量或执行算法。因此，必须指出开始点和结束点。
			TA／3_OP CODE控制该表的处理：
			0：未初始化
			1； 新操作特性，第1个值(TAnENTRY一1)
			2：保留
			3：上一个值，传输结束，检查表，用新曲线替换旧曲线，更新TAB—ACTUALNUM
			BER
			4：删除由TAB_ENTRY规定的表中的点并减小TAB—AcTuAL—NUMBER(可
			选)。
			5：插入由TAB_x—Y—VALUE定义的点，按TAB-x—VALUE递增顺序对表排序，
			并增加TAB ACTUALNUMBER(可选)
			6： 用实际ENTRY替代表中的点(可选)
			无需开始和停止交互(TAB-oB_CODE 1和TAB—OB—CODE 3)就可能读取一个表或表
			中的某些部分。通过将TAB-ENTRY设为1来指示开始
		 */
	uint8_t Tab_status;
		/*
		 * 通常在设备中提供真实性检查。检查的结果在参数TAB_sTATus中指示。
			0：未初始化
			1：正确(新表有效)
			2：非单调递增(旧表有效)
			3：非单调递减(旧表有效)
			4： 所传输的值不够(旧表有效)
			5： 所传输的值太多(旧表有效)
			6：边界斜率(gradient)太高(旧表有效)
			7：值未被接受(旧值有效)
			8： 当前表被装载．在TAl8 OP CODE=l之后且在TAB-OP—CODE=3之前设置
			(对表的其他访问无效，旧值有效)。
			9：排序并检查表(对表的其他访问无效，旧值有效)。
			10～19：保留
			20：非单调递增(表未初始化)
			21： 非单调递减(表未初始化)
			22： 所传输的值不够(表未初始化)
			23： 所传输的值太多(表未初始化)
			24： 边界斜率太高(表未初始化)
			25：值未被接受(表未初始化)
			26： 当前表被装载，在TAB一0P—CODE=1之后且在TAB OP—CODE=3之前设
			置(对表的其他访问无效，表未初始化)
			27：排序并检查表(对表的其他访问无效，表未初始化)
			28～127：保留
			>128：制造商特定
		 */
	uint8_t Tab_actual_number;
		/*
		 * 包含表中登录项的实际个数。在完成该表的传输后应对其进行计算
		 */
	uint8_t Lin_type;
		/*
		 * 线性化类型。
			0： 非线性化(必备)
			1： 线性化表(可选)
			10： 平方根(可选)
			20： 圆柱形卧式容器(可选)
			21： 球形容器(可选)
			50： 等百分比1：33(可选)
			5l： 反向等百分比(快开)1：33(可选)
			52： 等百分比1：50(可选)
			53： 反向等百分比1：50(可选)
			54： 等百分比1t 25(可选)
			55： 反向等百分比1：25(可选)
			100： RTDPtl0 a—O．003 850(IEC 60751)
			101： RTD Pt50 a一0．003 850(IEC 60751)
			102： RTD Ptl00 a一0．003 850(IEC 60751)
			103： RTD Pt200 a一0．003 850(IEC 60751)
			104： RTD PtS00 a#0．003 850(IEC 60751)
			105： RTD Ptl000 a一0．003 850(IEC 60751)
			106： RTD Ptl0 a一0．003 916(JIS C1604—81)
			107： RTD PtS0 a=0．003 916(JIS C1604—81)
			108： RTD Ptl00 a一0．003 916(JISCl604—81)
			109： RTD Ptl0 a=0．003 920(MIL—T一24388)
			110：u RTD Pt50 a=0．003 920(MlL_T一24388)
			111： RTD Ptl00 a一0．003 920(MIoT-24388)
			112： RTD Pt200 a一0．003 920(MIL-T-24388)
			113： RTD Pt500 a一0．003 920(MII．一T-24388)
			114： RTD Ptl000 a一0．003 920(MIL T 24388)
			115： RTD Ptl00 a一0．003 923(SAMA RC2卜4—1 966)
			116： RTD Pt200 a一0．003 923(SAMA RC21—4—1966)
			117： RTD Ptl00 a一0．003 926(IPTs-68)
			118： RTD Ni50 a一0．006 720(Edison curve#7)
			119： RTD Nil00 a一0．006 720(Edison curve#7)
			120： RTD Nil20 a一0．006 720(Edison curve#7)
			121； RTD Nil000 a置0．006 720(Edison curve#7)
			122： RTD Ni50 a一0．006 180(DIN 43760)
			123： RTD Nil00 a一0．006 180(DIN 43760)
			124： RTD Nil20 a一0．006 180(DIN 43760)
			125： RTD Nil000 a霉0．006 180(DIN 43760)
			126： RTDCul0 a一0．004 270
			127： RTD Cul00 a一0．004 270
			128：TC Type B，Pt30Rh-Pt6Rh(IEC 60584，NIST MN 175，DIN 43710，BS 4937，
			ANSI MC96．1，jis C1602，NF C42—321)
			129：TC Type C(W5)，W5一W26Rh(ASTM E988)
			130：TC Type D(W3)．W3一W25Rh(ASTM E988)
			131： TCTypeE，N110C卜Cu45Ni(IEC 60584，NISTMNl75．DIN 43710．BS 4937，
			ANSI MC96．1，JIS C1602，NF C42—321)
			132：TC Type G(W)，w_W26Rh(ASTM E988)
			133： TC Type J，Fe—Cu45Ni(IEC 60584，NIST MN 175，DIN 43710，BS 4937．
			ANSI MC96．1，JIS C1602，NF C42—321)
			134： TC Type KtNil0C卜Ni5(IEC 60584，NIST MN 175，DIN 43710，BS 4937，
			ANSI MC96．1，JISCl602，NFC42 321)
			135：TC Type NtNil4CrSONlSi(IEC 60584，NIST MN 175，DIN 43710，BS 4937，
			ANSI MC96．1．JIS C1602．NF C42—321)
			136：TCType R，Ptl3Rh Pt(IEC 60584．NIST MN 175．DIN 43710．BS 4937，
			ANSI MC96．1，J1S C1602，NF C42—321)
			137：TCType S，Ptl0Rh—Pt(IEC 60584，N1ST MN 175，DIN 43710，BS 4937，
			ANSI MC96．1，JIS C1602．NF C42—321)
			138：TC Type T，Cu-Cu45Ni(IEC 60584，NIST MN 175，DIN 43710．BS 4937，
			ANSI MC96．1，JIS C1602，NF C42 321)
			139：TC Type L，Fe-CuNi(DIN 43710)
			140：TC Type U，Cu-CuNi(DIN 43710)
			141： TC Type Pt20／Pt40，Pt20Rh—Pt40Rh(ASTM E1751)
			142：TC Type lr／Ir40，Ir-Ir40Rh(ASTM E1751)
			143- TC Platinel II
			144：TC Ni／NiMo
			145～239：保留
			240： 制造商特定
			249： 制造商特定
			250： 未使用
			251： 无意义
			252： 未知
			253： 特殊
			254～255：保留
		 */
}_PACKED_ PA_TAB_PARAMETER_t;

//The modification of a table in a device influences the measurement or actuation algorithms
//of the device. Therefore an indication of a starting and an end point is necessary. The
//TAP_OP_CODE controls the transaction of the table.
#define TAB_OPCODE_NOINIT    0 // not initialized
#define TAB_OPCODE_NEWOPT    1 // new operation characteristic, first value (TAB_ENTRY=1)
#define TAB_OPCODE_RESEV     2 // reserved
#define TAB_OPCODE_LASTVAL   3 // last value, end of transmission, check table, swap the old curve with the new curve and update TAB_ACTUAL_NUMBER.
#define TAB_OPCODE_DELETE    4 // delete point of table specified by TAB_ENTRY and decrement TAB_ACTUAL_NUMBER. (optional)
#define TAB_OPCODE_INSERT    5 // insert point defined with TAB_X_Y_VALUE, sort table with increasing TAB_X_VALUE and increment TAB_ACTUAL_NUMBER. (optional)
#define TAB_OPCODE_REPLACE   6 // replace point of table with actual ENTRY (optional).
//It is possible to read a table or parts of the table without start and stop an interaction (TAB_OB_CODE 1 and 3). The start is indicated by set TAB_ENTRY to 1.

//It is common to provide a plausibility check in the device. The result of this check is indicated in the TAB_STATUS parameter.
#define TAB_STATUS_NOINIT        0// not initialized
#define TAB_STATUS_GOOD          1// good (new table is valid)正确(新表有效)
#define TAB_STATUS_NOMONOINC     2// not monotonous increasing (old table is valid)非单调递增(旧表有效)
#define TAB_STATUS_NOMONODEC     3// not monotonous decreasing (old table is valid)非单调递减(旧表有效)
#define TAB_STATUS_NOENOVAL      4// not enough values transmitted (old table is valid)所传输的值不够(旧表有效)
#define TAB_STATUS_TOOMANYVAL    5// too many values transmitted (old table is valid)所传输的值太多(旧表有效)
#define TAB_STATUS_GREDTOOHIGH   6// gradient of edge too high (old table is valid) 边界斜率(gradient)太高(旧表有效)
#define TAB_STATUS_VALNOTACC     7// Values not accepted (old values are valid)值未被接受(旧值有效)
#define TAB_STATUS_VALLOADING    8// Table is currently loaded, set after TAB_OP_CODE = 1 and before  TAB_OP_CODE = 3 (Additional access to table not valid, old values are valid)
#define TAB_STATUS_SORTCHECK     9// sorting and checking of Table (Additional access to table not valid, old  values are valid)
#define TAB_STATUS_NOMONOINC_NI   20// not monotonous increasing (table is not initialized)
#define TAB_STATUS_NOMONODEC_NI   21// not monotonous decreasing (table is not initialized)
#define TAB_STATUS_NOENOVA_NI     22// not enough values transmitted (table is not initialized)
#define TAB_STATUS_TOOMANYVAL_NI  23// too many values transmitted (table is not initialized)
#define TAB_STATUS_GREDTOOHIGH_NI  24// gradient of edge too high (table is not initialized)
#define TAB_STATUS_VALNOTACC_NI    25// Values not acceped (table is not initialized)
#define TAB_STATUS_VALLOADING_NI    26// Table is currently loaded, set after TAB_OP_CODE = 1 and before TAB_OP_CODE = 3 (Additional access to table not valid, table is not initialized)
#define TAB_STATUS_SORTCHECKG_NI    27// sorting and checking of Table (Additional access to table not valid, table is not initialized)

typedef enum
{
	//用于参数处理功能的控制参数。
	PTA_PASSIVE 		=(uint8_t)0,//	无动作(缺省值)
	PTA_START_STRICT	=(uint8_t)1,//	使用严格(strict)模式(对下载数据集的100％兼容性)来启动参数处理。
	PTA_START_SMOOTH	=(uint8_t)2,//	使用平滑(smooth)模式(非兼容性不会导致停止下载但通知给主机)来启动参数处理。
	PTA_ABORT			=(uint8_t)3,// 异常中止参数处理(停止处理和对先前数据集的激活)。仅对具有单独处理数据缓冲器的设备有效。没有单独处理数据缓冲器的设备必须拒绝此命令(错误代码：无效范围)。
	PTA_TERMINATE		=(uint8_t)4,//	参数处理结束。主站指出处理的所有数据都已被传输。设备开始验证该处理数据集。
	PTA_CONFIRM			=(uint8_t)5,//	部分非兼容性或错误数据集的用户证实。
	//6～127：PI保留。
	//128～249：制造商特定。
	//250～255：PI保留
}PTA_OP_CODE_e;

typedef enum
{
	//关于处理序列的当前状态和设备数据有效性的状况信息。
	//0：保留
	STPTA_TA_ACTIVE		=(uint8_t)1,//该处理当前是有效的。
	STPTA_VERIFICATION	=(uint8_t)2,//该处理数据集的验证是有效的(功能检查)。
	STPTA_DATA_COMVALID	=(uint8_t)3,//该数据集是完全有效(一致)的。
	STPTA_DATA_NOTVALID	=(uint8_t)4,//该数据集是无效(不一致)的。该数据集不允许启动预期的操作。
	STPTA_DATA_PARVALID	=(uint8_t)5,//该数据集可能仅部分有效(一致)。在使用平滑模式(使用PTA_START_SMOOTH来启动该处理)的情况下支持此状况。
	//6～127：PI保留。
	//128～249：制造商特定。
	//250～255：PI保留
}PTA_STATUS_e;

/*
 * @bref 获取写访问保护控制
 *
 */
typedef struct
{
	Boolean bLocalAccess; //本地访问禁止与否
	Boolean bRemoteAccess;//远程访问禁止与否
	Boolean bExtLocalAccess;// true: 表明尽管本地访问禁止，但对于AO／DO，参数TARGET MODE、OUT和OUT—D是可写的。
	Boolean bExtRemoteAccess;// true: 表明尽管远程访问禁止，但对于AO／DO，参数TARGET MODE、OUT和OUT—D是可写的。
}WriteAPCtrl_t;

typedef struct
{
	IDX_STDParameters_t Std;
	__Def_ParameterAttr(char*, pSOFTWARE_REVISION);//现场设备的软件版本号
	__Def_ParameterAttr(char*, pHARDWARE_REVISION);//现场设备的硬件版本号
	__Def_ParameterAttr(uint16_t*, pDEVICE_MAN_ID);//现场设备制造商的标识代码
	__Def_ParameterAttr(char*, pDEVICE_ID);//制造商特定的设备标识
	__Def_ParameterAttr(char*, pDEVICE_SER_NUM);//现场设备的序列号
	__Def_ParameterAttr(uint8_t*, pDIAGNOSIS);//设备的详细信息，按比特编码。同时可能包含多个消息。如果Byte4的MSB被设 为1，
	//则在参数DIAGNOSIS_EXTENSION内可提供更多诊断信息。	DIAGNOSIS参数的编码在5．3．3和5．3．4中规定
	__Def_ParameterAttr(uint8_t*, pDIAGNOSIS_EXTENSION);//附加的制造商特定的设备信息，按比特编码。同时可能包含多个消息
	__Def_ParameterAttr(uint8_t*, pDIAGNOSIS_MASK);//所支持的DIAGNOSIS信息比特的定义。0：不支持	l：支持
	__Def_ParameterAttr(uint8_t*, pDIAGNOSIS_MASK_EXTENSION);//所支持的DIAGNOSIS_ExTENSl(』)N信息比特的定义。	0：不支持	1：支持
	__Def_ParameterAttr(char*, pDEVICE_CERTIFICATION); //现场设备的认证，如EX认证
	__Def_ParameterAttr(uint16_t*, pWRITE_LOCKING);//软件写保护。
							//0：拒绝对所有参数的非循环写服务(访问被拒绝)，但WRITE-LocKING本
							//身除外
							//1～2456：保留
							//2457：缺省值，它表示设备的所有可写参数都是可写的
							//2458～32767；保留
							//32768～65535：制造商特定
							//下面的参数无任何写保护机制：
							//——TAB_ENTRY(线性化表)；
							//——AcTuALPosT—READLNUMBER(日志功能块)。
							//此外．无论处于何种写保护状态，设备都应接受不包含Execution—Argument的
							//CaII-REQ-PDU，例如读I＆M数据
	__Def_ParameterAttr(uint16_t*, pFACTORY_RESET); //编码：
							//1：(必备)是把设备复位为缺省值的命令。总线地址的设置不受影响。
							//2：(可选)是把设备信息性参数复位为缺省值的命令。对于具有
							//“informational”复位类别特征的参数，在每个块的参数属性表内对其进行定义。
							//总线地址的设置不受影响。
							//3：(可选)是把具有“functional”复位类别特征的设备参数复位为缺省值的命
							//令。总线地址的设置不受影响。
							//4～2505：保留。
							//2506：(可选)是重启设备的命令。所有非易失性参数保持不变，所有动态参
							//数被复位为缺省值。
							//2507～2711：保留。
							//2712：(可选)，将总线地址设为缺省地址；其他参数设置保持不变。即使设备
							//处于循环数据传输状态，总线地址也应立即改变。直至后续的上电周期／热启动，
							//复位才暂停。对应于Set_Slave—Add服务的No_Add—Chg_Flag被清除。
							//2713--32767：保留。
							//32768～65535：制造商特定。
							//允许用于其他复位结果的制造商特定命令。
							//参数IDENT—NUMBER—SELECTOR不受FACTORY—RESET影响。
							//注：由本地显示的地址处理不属于本标准的范围
	__Def_ParameterAttr(uint8_t*, pDESCRIPTOR);//用户可定义的文本(字符串)，用于描述应用内的设备
	__Def_ParameterAttr(uint8_t*, pDEVICE_MESSAGE);//用户可定义的MESSAGE(字符串)，用于描述应用内或装置中的设备
	__Def_ParameterAttr(uint8_t*, pDEVICE_INSTAL_DATE);//设备的安装日期
	__Def_ParameterAttr(uint8_t*, pLOCAL_OP_ENA);//启用本地操作。
						//0： 禁用(不允许本地操作，即：仅允许来自主机设备的FBMODE的修改)
						//1： 启用(允许本地操作)
						//主机操作比本地终端设备操作具有更高的优先级
						//如果通信失败的时间超过30 S，则自动启用本地操作。这里，通信失败的定义是指
						//在规定时间段内未发生循环通信。如果参数LOCAL—OP—ENA等于0(禁用)且
						//恢复通信，则设备切换回远程操作。见5．2．8．4
	__Def_ParameterAttr(uint8_t*, pIDENT_NUMBER_SELECTOR);//每个符合1EC 61784 1中CP 3／1的PRoFIBUS DP设备应具有一个由PI提供的
								//Ident Number。该ldent—Number规定了设备在相应GSD文件中描述的循环行为
								//特性。符合IEC 61784 1中CP 3／2的PROFIBUS PA设备应至少支持一个行规
								//特定ldent—Number。在本标准的6．4．1中定义了行规特定Ident_Number。如果
								//某设备被设为行规特定Ident_Number，则它应符合相应行规GSD文件的行规特
								//征。此外，PROFIBUS PA设备可支持制造商特定ldent_Number。
								//行规特定的GSD文件由PI提供。制造商特定的GSD文件由设备制造商提供。
								//用户能够通过设置参数IDENT_NUMBER_SELECTOR来选择有效的ldent_
								//Number。如果Ident_Number被改变，则没备的循环行为特性(例如：诊断内容／长
								//度、当前／已接受的组态数据等)也将被改变。
								//编码：
								//0：行规特定Ident—Number(PA行规V3．x)(必备)
								//l：制造商特定Ident—Number(PA行规V3 x)(可选)
								//2：制造商特定Ident—Number(PA行规V2．0)(可选)
								//3；多变量设备的行规特定Ident—Number(PA行规V3．x)(可选)
								//4～126：保留供行规使用(不允许)
								//127：适应模式(必备)
								//128～255：制造商特定(可选)
	__Def_ParameterAttr(uint8_t*, pHW_WRITE_PROTECTION);//指出不能被远程访问修改的写保护机制(例如。硬跳线或本地用户接口)的状态，
								//它防止修改设备的参数。
								//0：无保护(必备)
								//1：保护，允许手动操作(可选)
								//拒绝对所有参数的非循环写访问(写访问被拒绝)，但线性化表的参数TAB—
								//ENTRY，以及参数TARGET—MODE和0UT／OUT—D(仅对AO和DO有
								//效)除外。
								//2：保护，非手动操作(可选)
								//拒绝对所有参数的非循环写访问(写访问被拒绝)，但线性化表的参数TAB—
								//ENTRY除外。
								//3～127：保留
								//128～255：制造商特定
								//设备可支持代码1和／或代码2。
								//以下参数无任何写保护机制：
								//·TAB-ENTRY(线性化表)；
								//●ACTUALPOST_READ．NUMBER(日志功能块)。
								//此外，无论处于何种写保护状态，设备都应接受不包含Execution—Argument的
								//Call—REQ-PDU，例如读I&M数据
	__Def_ParameterAttr(PA_FeatureDS68_t*, pFEATURE);//指示设备中所实现的可选特性，以及是否支持这些特性的情况
	__Def_ParameterAttr(uint8_t*, pCOND_STATUS_DIAG);//指示可对状况和诊断行为进行组态的设备模式：
									//0：提供5．3．2中定义的状况和诊断；
									//l：提供浓缩状况和诊断信息(见5．3．4)
									//2～255：PI保留
	__Def_ParameterAttr(PA_DiagEventSwitch_t*, pDIAG_EVENT_SWITCH);//如果FEATURE．Enabled．Condensed—Status=1，则指示／控制该设备对设备特定
											//诊断事件的反应。
											//对诊断事件的登录项的引用是制造商／设备特定的

	__Def_ParameterAttr(PTA_OP_CODE_e*, pPTA_OP_CODE);
	__Def_ParameterAttr(PTA_STATUS_e* , pPTA_STATUS);
}_PACKED_ IDX_PB_BLOCK_t;

typedef struct
{
	PA_STDParameters_t std;
	char SOFTWARE_REVISION[16];//现场设备的软件版本号
	char HARDWARE_REVISION[16];//现场设备的硬件版本号
	uint16_t DEVICE_MAN_ID;//现场设备制造商的标识代码
	char DEVICE_ID[16];//制造商特定的设备标识
	char DEVICE_SER_NUM[16];//现场设备的序列号
	uint8_t DIAGNOSIS[4];//设备的详细信息，按比特编码。同时可能包含多个消息。如果Byte4的MSB被设 为1，
	//则在参数DIAGNOSIS_EXTENSION内可提供更多诊断信息。	DIAGNOSIS参数的编码在5．3．3和5．3．4中规定
	uint8_t DIAGNOSIS_EXTENSION[6];//附加的制造商特定的设备信息，按比特编码。同时可能包含多个消息
	uint8_t DIAGNOSIS_MASK[4];//所支持的DIAGNOSIS信息比特的定义。0：不支持	l：支持
	uint8_t DIAGNOSIS_MASK_EXTENSION[6];//所支持的DIAGNOSIS_ExTENSl(』)N信息比特的定义。	0：不支持	1：支持
	char DEVICE_CERTIFICATION[32]; //现场设备的认证，如EX认证
	uint16_t WRITE_LOCKING;//软件写保护。
							//0：拒绝对所有参数的非循环写服务(访问被拒绝)，但WRITE-LocKING本
							//身除外
							//1～2456：保留
							//2457：缺省值，它表示设备的所有可写参数都是可写的
							//2458～32767；保留
							//32768～65535：制造商特定
							//下面的参数无任何写保护机制：
							//——TAB_ENTRY(线性化表)；
							//——AcTuALPosT—READLNUMBER(日志功能块)。
							//此外．无论处于何种写保护状态，设备都应接受不包含Execution—Argument的
							//CaII-REQ-PDU，例如读I＆M数据
	uint16_t FACTORY_RESET; //编码：
							//1：(必备)是把设备复位为缺省值的命令。总线地址的设置不受影响。
							//2：(可选)是把设备信息性参数复位为缺省值的命令。对于具有
							//“informational”复位类别特征的参数，在每个块的参数属性表内对其进行定义。
							//总线地址的设置不受影响。
							//3：(可选)是把具有“functional”复位类别特征的设备参数复位为缺省值的命
							//令。总线地址的设置不受影响。
							//4～2505：保留。
							//2506：(可选)是重启设备的命令。所有非易失性参数保持不变，所有动态参
							//数被复位为缺省值。
							//2507～2711：保留。
							//2712：(可选)，将总线地址设为缺省地址；其他参数设置保持不变。即使设备
							//处于循环数据传输状态，总线地址也应立即改变。直至后续的上电周期／热启动，
							//复位才暂停。对应于Set_Slave—Add服务的No_Add—Chg_Flag被清除。
							//2713--32767：保留。
							//32768～65535：制造商特定。
							//允许用于其他复位结果的制造商特定命令。
							//参数IDENT—NUMBER—SELECTOR不受FACTORY—RESET影响。
							//注：由本地显示的地址处理不属于本标准的范围
	uint8_t DESCRIPTOR[32];//用户可定义的文本(字符串)，用于描述应用内的设备
	uint8_t DEVICE_MESSAGE[32];//用户可定义的MESSAGE(字符串)，用于描述应用内或装置中的设备
	uint8_t DEVICE_INSTAL_DATE[16];//设备的安装日期
	uint8_t LOCAL_OP_ENA;//启用本地操作。
						//0： 禁用(不允许本地操作，即：仅允许来自主机设备的FBMODE的修改)
						//1： 启用(允许本地操作)
						//主机操作比本地终端设备操作具有更高的优先级
						//如果通信失败的时间超过30 S，则自动启用本地操作。这里，通信失败的定义是指
						//在规定时间段内未发生循环通信。如果参数LOCAL—OP—ENA等于0(禁用)且
						//恢复通信，则设备切换回远程操作。见5．2．8．4
	uint8_t IDENT_NUMBER_SELECTOR;//每个符合1EC 61784 1中CP 3／1的PRoFIBUS DP设备应具有一个由PI提供的
								//Ident Number。该ldent—Number规定了设备在相应GSD文件中描述的循环行为
								//特性。符合IEC 61784 1中CP 3／2的PROFIBUS PA设备应至少支持一个行规
								//特定ldent—Number。在本标准的6．4．1中定义了行规特定Ident_Number。如果
								//某设备被设为行规特定Ident_Number，则它应符合相应行规GSD文件的行规特
								//征。此外，PROFIBUS PA设备可支持制造商特定ldent_Number。
								//行规特定的GSD文件由PI提供。制造商特定的GSD文件由设备制造商提供。
								//用户能够通过设置参数IDENT_NUMBER_SELECTOR来选择有效的ldent_
								//Number。如果Ident_Number被改变，则没备的循环行为特性(例如：诊断内容／长
								//度、当前／已接受的组态数据等)也将被改变。
								//编码：
								//0：行规特定Ident—Number(PA行规V3．x)(必备)
								//l：制造商特定Ident—Number(PA行规V3 x)(可选)
								//2：制造商特定Ident—Number(PA行规V2．0)(可选)
								//3；多变量设备的行规特定Ident—Number(PA行规V3．x)(可选)
								//4～126：保留供行规使用(不允许)
								//127：适应模式(必备)
								//128～255：制造商特定(可选)
	uint8_t HW_WRITE_PROTECTION;//指出不能被远程访问修改的写保护机制(例如。硬跳线或本地用户接口)的状态，
								//它防止修改设备的参数。
								//0：无保护(必备)
								//1：保护，允许手动操作(可选)
								//拒绝对所有参数的非循环写访问(写访问被拒绝)，但线性化表的参数TAB—
								//ENTRY，以及参数TARGET—MODE和0UT／OUT—D(仅对AO和DO有
								//效)除外。
								//2：保护，非手动操作(可选)
								//拒绝对所有参数的非循环写访问(写访问被拒绝)，但线性化表的参数TAB—
								//ENTRY除外。
								//3～127：保留
								//128～255：制造商特定
								//设备可支持代码1和／或代码2。
								//以下参数无任何写保护机制：
								//·TAB-ENTRY(线性化表)；
								//●ACTUALPOST_READ．NUMBER(日志功能块)。
								//此外，无论处于何种写保护状态，设备都应接受不包含Execution—Argument的
								//Call—REQ-PDU，例如读I&M数据
	PA_FeatureDS68_t FEATURE;//指示设备中所实现的可选特性，以及是否支持这些特性的情况
	uint8_t COND_STATUS_DIAG;//指示可对状况和诊断行为进行组态的设备模式：
									//0：提供5．3．2中定义的状况和诊断；
									//l：提供浓缩状况和诊断信息(见5．3．4)
									//2～255：PI保留
	PA_DiagEventSwitch_t DIAG_EVENT_SWITCH;//如果FEATURE．Enabled．Condensed—Status=1，则指示／控制该设备对设备特定
											//诊断事件的反应。
											//对诊断事件的登录项的引用是制造商／设备特定的

	PTA_OP_CODE_e PTA_OP_CODE;
	PTA_STATUS_e  PTA_STATUS;
}_PACKED_ PB_BLOCK_t;

/*
 * 5．3．2经典状况
 * 5．3．2．1状况属性
 */

/*
 * 表57状况(Status)字节的编码
 */
#define STATUS_QUALITY_BAD 			(0x00<<6) //BAD：坏
#define STATUS_QUALITY_UNCERTAIN 	(0x01<<6)  //UNCERTAIN：不确定
#define STATUS_QUALITY_GOODNOCAS	(0x02<<6)  //GOOD(Non Cascade)：好(无级联)
#define STATUS_QUALITY_GOODCAS		(0x03<<6)  //OOD(Cascade)：好(级联)
//当Quality=BAD时。Substatus的含义
#define SSTATUS_BAD_NONE			(0x00<<2)  //non specific：非特定的
#define SSTATUS_BAD_CFGERROR		(0x01<<2)  //configuration error:组态错误
#define SSTATUS_BAD_NOCONNECT		(0x02<<2)  //not connected：未连接
#define SSTATUS_BAD_DEVFAIL  		(0x03<<2)  //device failure：设备故障
#define SSTATUS_BAD_SENSFAIL  		(0x04<<2)  //sensor failure：传感器故障
#define SSTATUS_BAD_NOCOMMLAST  	(0x05<<2)  //no communication(last usable value)：无通信(上一个可用值)
#define SSTATUS_BAD_NOCOMMNO  		(0x06<<2)  //no communication(no usable value)：无通信(无可用值)
#define SSTATUS_BAD_OS		  		(0x07<<2)  //OUt Of service：非服务
/* 表75 NAMUR NEl07规定的状况编码 */
#define SSTATUS_BAD_MAINTALAR	  	(0x09<<2)  //符合NAMuRNEl07的含义Failure(F) 在PCS／DCS中的用法Failure 0x24～ 0x27  BAD-maintenance alarm，more disgnosis avaliabl
#define SSTATUS_BAD_FCHECKLOCOVR 	(0x0f<<2)  //符合NAMuRNEl07的含义Check(C) 在PCS／DCS中的用法Failure 0x3C～ 0x3F  BAD—function check／local override

//当Quality=UNCERTAIN时，Substatus的含义
#define SSTATUS_UNC_NONE		  	(0x10<<2)  //nod specific：非特定的
#define SSTATUS_UNC_LUV		  		(0x11<<2)  //last usable value)(LUV)：上一个可用值(LUV)
#define SSTATUS_UNC_SUBVAL	  		(0x12<<2)  //substitute value：替代值
#define SSTATUS_UNC_INIVAL	  		(0x13<<2)  //initial value：初始值
#define SSTATUS_UNC_SENCOVNOACC 	(0x14<<2)  //sensor conversion not accurate：传感器转换不精确
#define SSTATUS_UNC_UNITNOVALID 	(0x15<<2)  //engineering unit violation(unit not in the valid set)：工程单位违规(单位不在有效集内)
#define SSTATUS_UNC_SUBNORMAL 		(0x16<<2)  //sub nomal：低于正常情况
#define SSTATUS_UNC_CFGERROR 		(0x17<<2)  //configuration error：组态错误
#define SSTATUS_UNC_SIMULVAL 		(0x18<<2)  //simulated value：仿真值
#define SSTATUS_UNC_SENSCALIB 		(0x19<<2)  //sensor calibration：传感器校准
/* 表75 NAMUR NEl07规定的状况编码 */
#define SSTATUS_UNC_MAINTALAR	  	(0x1E<<2)  //符合NAMuRNEl07的含义Out of Specification(F) 在PCS／DCS中的用法Uncertain 0x78～0x7B UNCERTAIN—process related，no maitenance(限值检查见5．3．4．2．2．2)

//当Quality=GOOD(Non Cascade)时，Substatus的含义
#define SSTATUS_NGOOD_OK 			(0x20<<2)  //ok
#define SSTATUS_NGOOD_UPDATE		(0x21<<2)  //update event：更新事件
#define SSTATUS_NGOOD_ACTADVALARM	(0x22<<2)  //active advisory alarm：有效的警戒报警
#define SSTATUS_NGOOD_ACTCRIALARM	(0x23<<2)  //acitve critical alarm：有效的紧急报警
#define SSTATUS_NGOOD_UNACKUPDATE	(0x24<<2)  //unacknowledged update event：来确认的更新事件
#define SSTATUS_NGOOD_UNACKADVALA	(0x25<<2)  //unacknowledged advisory alarm：未确认的警戒报警
#define SSTATUS_NGOOD_UNACKCRIALA	(0x26<<2)  //unacknowledged critical alarm：未确认的紧急报警
#define SSTATUS_NGOOD_INIFAILSAFE	(0x28<<2)  //initiate fail safe：启动故障安全
#define SSTATUS_NGOOD_MAINREQ		(0x29<<2)  //maintenance required：需要维护


//当Quality=GOOD(Cascade)时．Substatus的含义
#define SSTATUS_CGOOD_OK 			(0x30<<2)  //ok
#define SSTATUS_CGOOD_INITACK		(0x31<<2)  //initialization acknowledged：已确认的初始化
#define SSTATUS_CGOOD_INITREQ 		(0x33<<2)  //initialization request：初始化请求
#define SSTATUS_CGOOD_NOINV			(0x33<<2)  //not invited：不被允许
#define SSTATUS_CGOOD_NOSELECT		(0x35<<2)  //do not select：不选择
#define SSTATUS_CGOOD_LOV			(0x36<<2)  //local overide：本地超驰
#define SSTATUS_CGOOD_INIFAILSAFE	(0x38<<2)  //initiate fail safe：启动故障安全

//Limit比特的含义
#define STATUS_LIMIT_OK				(0x00<<0)  //ok
#define STATUS_LIMIT_LOW			(0x01<<0)  //low limited：超下限
#define STATUS_LIMIT_HIGH			(0x02<<0)  //high limited：超上限
#define STATUS_LIMIT_CONST			(0x03<<0)  //constant：常量


//表60坏-传感器故障，超下限(BAD-sensor failure，low limited)
#define STATUS_BAD_SENSFAIL_LOLIM  (SSTATUS_BAD_SENSFAIL|STATUS_LIMIT_LOW)
//表61坏-传感器故障，超上限(BAD-sensor failure，high limited)
#define STATUS_BAD_SENSFAIL_HILIM  (SSTATUS_BAD_SENSFAIL|STATUS_LIMIT_HIGH)
//表62好(无级联)-有效的警戒报警，超下限(GOOD(Non Cascade)一active advisory alarm，low limited)
#define STATUS_NGOOD_ACTADVALARM_LOLIM  (SSTATUS_NGOOD_ACTADVALARM|STATUS_LIMIT_LOW)
//表63好(无级联)-有效的警戒报警，超上限(GOOD(Non Cascade)一active advisory alarm，high limited)
#define STATUS_NGOOD_ACTADVALARM_HILIM  (SSTATUS_NGOOD_ACTADVALARM|STATUS_LIMIT_HIGH)
//表64好(无级联)-有效的紧急报警，超下限(GOOD(Non Cascade)一active critical alarm，low limited)
#define STATUS_NGOOD_ACTCRIALARM_LOLIM  (SSTATUS_NGOOD_ACTCRIALARM|STATUS_LIMIT_LOW)
//表65好(无级联)-有效的紧急报警，超上限(GOOD(Non Cascade)一active critical alarm，high limited)
#define STATUS_NGOOD_ACTCRIALARM_HILIM  (SSTATUS_NGOOD_ACTCRIALARM|STATUS_LIMIT_HIGH)
//表66好(无级联)-更新事件(GOOD(Non Cascade)一update event)
#define STATUS_NGOOD_UPDATE_OK     (SSTATUS_NGOOD_UPDATE|STATUS_LIMIT_OK)
#define STATUS_NGOOD_UPDATE_LOLIM  (SSTATUS_NGOOD_UPDATE|STATUS_LIMIT_LOW)
#define STATUS_NGOOD_UPDATE_HILIM  (SSTATUS_NGOOD_UPDATE|STATUS_LIMIT_HIGH)
#define STATUS_NGOOD_UPDATE_CONST  (SSTATUS_NGOOD_UPDATE|STATUS_LIMIT_CONST)


/* 表75 NAMUR NEl07规定的状况编码 */
#define STATUS_MAINT_HILIM			(SSTATUS_NGOOD_ACTADVALARM|STATUS_LIMIT_HIGH)  //符合NAMuRNEl07的含义Maintenance(M) 在PCS／DCS中的用法GOOD 0xA4～0xAB GOOD-maintenance required／demanded (限值检查见5．3．4．2．2．2)
#define STATUS_MAINT_HIHILIM		(SSTATUS_NGOOD_ACTCRIALARM|STATUS_LIMIT_HIGH)   //符合NAMuRNEl07的含义Maintenance(M) 在PCS／DCS中的用法GOOD 0xA4～0xAB GOOD-maintenance required／demanded (限值检查见5．3．4．2．2．2)
#define STATUS_MAINT_LOLIM			(SSTATUS_NGOOD_ACTADVALARM|STATUS_LIMIT_LOW)   //符合NAMuRNEl07的含义Maintenance(M) 在PCS／DCS中的用法GOOD 0xA4～0xAB GOOD-maintenance required／demanded (限值检查见5．3．4．2．2．2)
#define STATUS_MAINT_LOLOLIM		(SSTATUS_NGOOD_ACTCRIALARM|STATUS_LIMIT_LOW)   //符合NAMuRNEl07的含义Maintenance(M) 在PCS／DCS中的用法GOOD 0xA4～0xAB GOOD-maintenance required／demanded (限值检查见5．3．4．2．2．2)

/* 表76在状况为好(GOOD)时功能检查／更新事件(Limit Checks／Update Events)的编码 */
#define STATUSG_GOOD_OK_OK			(SSTATUS_NGOOD_OK | STATUS_LIMIT_OK) 		//符合NAMuRNEl07的含义Good(G) 在PCS／DCS中的用法GOOD 0x80～0x80 Good OK
#define STATUSG_GOOD_OK_LOW			(SSTATUS_NGOOD_OK | STATUS_LIMIT_LOW) 		//符合NAMuRNEl07的含义Good(G) 在PCS／DCS中的用法GOOD 0x80～0x80 Good OK
#define STATUSG_GOOD_OK_HIG			(SSTATUS_NGOOD_OK | STATUS_LIMIT_HIGH) 		//符合NAMuRNEl07的含义Good(G) 在PCS／DCS中的用法GOOD 0x80～0x80 Good OK
#define STATUSG_GOOD_OK_CST			(SSTATUS_NGOOD_OK | STATUS_LIMIT_CONST) 		//符合NAMuRNEl07的含义Good(G) 在PCS／DCS中的用法GOOD 0x80～0x80 Good OK
#define STATUSG_GOOD_UPDATE_OK		(SSTATUS_NGOOD_UPDATE | STATUS_LIMIT_OK)  	//符合NAMuRNEl07的含义Good(G) 在PCS／DCS中的用法GOOD 0x84～0x87 GOOD uPate event
#define STATUSG_GOOD_UPDATE_LOW		(SSTATUS_NGOOD_UPDATE | STATUS_LIMIT_LOW) 	 //符合NAMuRNEl07的含义Good(G) 在PCS／DCS中的用法GOOD 0x84～0x87 GOOD uPate event
#define STATUSG_GOOD_UPDATE_HIG		(SSTATUS_NGOOD_UPDATE | STATUS_LIMIT_HIGH) 	 //符合NAMuRNEl07的含义Good(G) 在PCS／DCS中的用法GOOD 0x84～0x87 GOOD uPate event
#define STATUSG_GOOD_UPDATE_CST		(SSTATUS_NGOOD_UPDATE | STATUS_LIMIT_CONST)  //符合NAMuRNEl07的含义Good(G) 在PCS／DCS中的用法GOOD 0x84～0x87 GOOD uPate event
#define STATUSG_GOOD_ADVALARLOLIM	(SSTATUS_NGOOD_ACTADVALARM | STATUS_LIMIT_LOW)  //符合NAMuRNEl07的含义Good(G) 在PCS／DCS中的用法GOOD 0x89～0x89 GOOD-advisory alarm，low limit
#define STATUSG_GOOD_ADVALARHILIM	(SSTATUS_NGOOD_ACTADVALARM | STATUS_LIMIT_HIGH)  //符合NAMuRNEl07的含义Good(G) 在PCS／DCS中的用法GOOD 0x8A～0x8A GOOD-advisory alarm。high limit
#define STATUSG_GOOD_CRIALARLOLIM	(SSTATUS_NGOOD_ACTCRIALARM | STATUS_LIMIT_LOW)  //符合NAMuRNEl07的含义Good(G) 在PCS／DCS中的用法GOOD 0x8D～0x8D GOOD-cirtical alarm，low limit
#define STATUSG_GOOD_CRIALARHILIM	(SSTATUS_NGOOD_ACTCRIALARM | STATUS_LIMIT_HIGH)  //符合NAMuRNEl07的含义Good(G) 在PCS／DCS中的用法GOOD 0x8E～0x8E GOOD-cirtical alarm。high limit

/* 表77具有详细信息的浓缩状况的编码*/
#define STATUSF_BAD_NONE			    	(SSTATUS_BAD_NONE | STATUS_LIMIT_OK) 		//符合NAMuRNEl07的含义Failure(F) 在PCS／DCS中的用法Failure 维护站Failure 0x00～0x00 BAD-non specific
#define STATUSF_BAD_PASSIV				(0x20 | STATUS_LIMIT_CONST) 				//符合NAMuRNEl07的含义Failure(F) 在PCS／DCS中的用法Passivated 维护站GOOD 0x23～0x23 BAD-passivated(诊断警报被禁止）
#define STATUSF_BAD_MAINTALAR_OK			(SSTATUS_BAD_MAINTALAR | STATUS_LIMIT_OK) 	//符合NAMuRNEl07的含义Failure(F) 在PCS／DCS中的用法Failure 维护站Maintenance alarm 0x24～0x27 BAD-maintenance alarm．more diagnosls available
#define STATUSF_BAD_MAINTALAR_LOW		(SSTATUS_BAD_MAINTALAR | STATUS_LIMIT_LOW) 	//符合NAMuRNEl07的含义Failure(F) 在PCS／DCS中的用法Failure 维护站Maintenance alarm 0x24～0x27 BAD-maintenance alarm．more diagnosls available
#define STATUSF_BAD_MAINTALAR_HIG		(SSTATUS_BAD_MAINTALAR | STATUS_LIMIT_HIG) 	//符合NAMuRNEl07的含义Failure(F) 在PCS／DCS中的用法Failure 维护站Maintenance alarm 0x24～0x27 BAD-maintenance alarm．more diagnosls available
#define STATUSF_BAD_MAINTALAR_CST		(SSTATUS_BAD_MAINTALAR | STATUS_LIMIT_CONST) 	//符合NAMuRNEl07的含义Failure(F) 在PCS／DCS中的用法Failure 维护站Maintenance alarm 0x24～0x27 BAD-maintenance alarm．more diagnosls available

#define STATUSF_BAD_PROCESS_OK			(0x28 | STATUS_LIMIT_OK) 	//符合NAMuRNEl07的含义Failure(F) 在PCS／DCS中的用法Failure 维护站GOOD  0x28～0x2B BAD-process related-no maitenace
#define STATUSF_BAD_PROCESS_LOW			(0x28 | STATUS_LIMIT_LOW) 	//符合NAMuRNEl07的含义Failure(F) 在PCS／DCS中的用法Failure 维护站GOOD  0x28～0x2B BAD-process related-no maitenace
#define STATUSF_BAD_PROCESS_HIG			(0x28 | STATUS_LIMIT_HIG) 	//符合NAMuRNEl07的含义Failure(F) 在PCS／DCS中的用法Failure 维护站GOOD  0x28～0x2B BAD-process related-no maitenace
#define STATUSF_BAD_PROCESS_CST			(0x28 | STATUS_LIMIT_CONST) 	//符合NAMuRNEl07的含义Failure(F) 在PCS／DCS中的用法Failure 维护站GOOD  0x28～0x2B BAD-process related-no maitenace

#define STATUSC_BAD_FCHECKLOCOVR_OK			(SSTATUS_BAD_FCHECKLOCOVR | STATUS_LIMIT_OK) 	//符合NAMuRNEl07的含义Check(C) 在PCS／DCS中的用法Function Check 维护站Function Check  0x3c～0x3f BAD-function eheek／loeal override：value not usable
#define STATUSC_BAD_FCHECKLOCOVR_LOW		(SSTATUS_BAD_FCHECKLOCOVR | STATUS_LIMIT_LOW) 	//符合NAMuRNEl07的含义Check(C) 在PCS／DCS中的用法Function Check 维护站Function Check  0x3c～0x3f BAD-function eheek／loeal override：value not usable
#define STATUSC_BAD_FCHECKLOCOVR_HIG		(SSTATUS_BAD_FCHECKLOCOVR | STATUS_LIMIT_HIG) 	//符合NAMuRNEl07的含义Check(C) 在PCS／DCS中的用法Function Check 维护站Function Check  0x3c～0x3f BAD-function eheek／loeal override：value not usable
#define STATUSC_BAD_FCHECKLOCOVR_CST		(SSTATUS_BAD_FCHECKLOCOVR | STATUS_LIMIT_CONST) 	//符合NAMuRNEl07的含义Check(C) 在PCS／DCS中的用法Function Check 维护站Function Check  0x3c～0x3f BAD-function eheek／loeal override：value not usable

#define STATUSF_UNC_UNCSUBSTI_CST			(SSTATUS_UNC_SUBVAL | STATUS_LIMIT_CONST) 	//符合NAMuRNEl07的含义Failure(F) 在PCS／DCS中的用法Uncertain 维护站Maintenance Alarm  0x4b～0x4b UNCERTAIN substitute set
#define STATUSF_UNC_UNCINIT_CST				(SSTATUS_UNC_INIVAL | STATUS_LIMIT_CONST) 	//符合NAMuRNEl07的含义Failure(F) 在PCS／DCS中的用法Uncertain 维护站GOOD  0x4f～0x4f UNCERTAIN—initial value

#define STATUSM_UNC_MAINTDEMAD_OK			(0x68 | STATUS_LIMIT_OK) 	//符合NAMuRNEl07的含义Maintenance(M) 在PCS／DCS中的用法Uncertain 维护站Maintenance demanded  0x68～0x6b UNCERlAIN—maintenance demanded(限值检查见5．3．4．2．2．2)
#define STATUSM_UNC_MAINTDEMAD_LOW			(0x68 | STATUS_LIMIT_LOW) 	//符合NAMuRNEl07的含义Maintenance(M) 在PCS／DCS中的用法Uncertain 维护站Maintenance demanded  0x68～0x6b UNCERlAIN—maintenance demanded(限值检查见5．3．4．2．2．2)
#define STATUSM_UNC_MAINTDEMAD_HIG			(0x68 | STATUS_LIMIT_HIG) 	//符合NAMuRNEl07的含义Maintenance(M) 在PCS／DCS中的用法Uncertain 维护站Maintenance demanded  0x68～0x6b UNCERlAIN—maintenance demanded(限值检查见5．3．4．2．2．2)
#define STATUSM_UNC_MAINTDEMAD_CST			(0x68 | STATUS_LIMIT_CONST) 	//符合NAMuRNEl07的含义Maintenance(M) 在PCS／DCS中的用法Uncertain 维护站Maintenance demanded  0x68～0x6b UNCERlAIN—maintenance demanded(限值检查见5．3．4．2．2．2)

#define STATUSC_UNC_UNCSIMULST_CST			(0x70 | STATUS_LIMIT_CONST) 	//符合NAMuRNEl07的含义CHECK(C) 在PCS／DCS中的用法Uncertain 维护站function check  0x73～0x73 UNCERTAIN—simulated Value，start

#define STATUSC_UNC_UNCSIMULED_OK			(0x74 | STATUS_LIMIT_OK) 	//符合NAMuRNEl07的含义CHECK(C) 在PCS／DCS中的用法Uncertain 维护站GOOD  0x74～0x77 UNCERTAIN—simulated Value，end(限值检查见5．3．4．2．2．2)
#define STATUSC_UNC_UNCSIMULED_LOW			(0x74 | STATUS_LIMIT_LOW) 	//符合NAMuRNEl07的含义CHECK(C) 在PCS／DCS中的用法Uncertain 维护站GOOD  0x74～0x77 UNCERTAIN—simulated Value，end(限值检查见5．3．4．2．2．2)
#define STATUSC_UNC_UNCSIMULED_HIG			(0x74 | STATUS_LIMIT_HIG) 	//符合NAMuRNEl07的含义CHECK(C) 在PCS／DCS中的用法Uncertain 维护站GOOD  0x74～0x77 UNCERTAIN—simulated Value，end(限值检查见5．3．4．2．2．2)
#define STATUSC_UNC_UNCSIMULED_CST			(0x74 | STATUS_LIMIT_CONST) //符合NAMuRNEl07的含义CHECK(C) 在PCS／DCS中的用法Uncertain 维护站GOOD  0x74～0x77 UNCERTAIN—simulated Value，end(限值检查见5．3．4．2．2．2)

#define STATUSS_UNC_PROCESS_OK			(0x78 | STATUS_LIMIT_OK) 	//符合NAMuRNEl07的含义Out of specification(S) 在PCS／DCS中的用法Uncertain 维护站GOOD  0x78～0x7B UNCERTAIN—process related，no maitenace(限值检查见5．3．4．2．2．2)
#define STATUSS_UNC_PROCESS_LOW			(0x78 | STATUS_LIMIT_LOW) 	//符合NAMuRNEl07的含义Out of specification(S) 在PCS／DCS中的用法Uncertain 维护站GOOD  0x78～0x7B UNCERTAIN—process related，no maitenace(限值检查见5．3．4．2．2．2)
#define STATUSS_UNC_PROCESS_HIG			(0x78 | STATUS_LIMIT_HIG) 	//符合NAMuRNEl07的含义Out of specification(S) 在PCS／DCS中的用法Uncertain 维护站GOOD  0x78～0x7B UNCERTAIN—process related，no maitenace(限值检查见5．3．4．2．2．2)
#define STATUSS_UNC_PROCESS_CST			(0x78 | STATUS_LIMIT_CONST) //符合NAMuRNEl07的含义Out of specification(S) 在PCS／DCS中的用法Uncertain 维护站GOOD  0x78～0x7B UNCERTAIN—process related，no maitenace(限值检查见5．3．4．2．2．2)

#define STATUSG_GOOD_INIFAILSAFE_OK		(SSTATUS_NGOOD_INIFAILSAFE | STATUS_LIMIT_LOW)   //符合NAMuRNEl07的含义Good(G) 在PCS／DCS中的用法GOOD 维护站GOOD 0xA0～0xA3 GOOD-initiate fail safe(用于输出FB的输人参数的命令，与变送器无关)(限值检查见5．3．4．2．2 2)
#define STATUSG_GOOD_INIFAILSAFE_LOW	(SSTATUS_NGOOD_INIFAILSAFE | STATUS_LIMIT_HIGH)  //符合NAMuRNEl07的含义Good(G) 在PCS／DCS中的用法GOOD 维护站GOOD 0xA0～0xA3 GOOD-initiate fail safe(用于输出FB的输人参数的命令，与变送器无关)(限值检查见5．3．4．2．2 2)
#define STATUSG_GOOD_INIFAILSAFE_HIG	(SSTATUS_NGOOD_INIFAILSAFE | STATUS_LIMIT_LOW)   //符合NAMuRNEl07的含义Good(G) 在PCS／DCS中的用法GOOD 维护站GOOD 0xA0～0xA3 GOOD-initiate fail safe(用于输出FB的输人参数的命令，与变送器无关)(限值检查见5．3．4．2．2 2)
#define STATUSG_GOOD_INIFAILSAFE_CST	(SSTATUS_NGOOD_INIFAILSAFE | STATUS_LIMIT_HIGH)  //符合NAMuRNEl07的含义Good(G) 在PCS／DCS中的用法GOOD 维护站GOOD 0xA0～0xA3 GOOD-initiate fail safe(用于输出FB的输人参数的命令，与变送器无关)(限值检查见5．3．4．2．2 2)

#define STATUSM_GOOD_MAINREQ_OK		(SSTATUS_NGOOD_MAINREQ | STATUS_LIMIT_LOW)   //符合NAMuRNEl07的含义Maintenance(M) 在PCS／DCS中的用法GOOD 维护站Maintenance required 0xA4～0xA7 GOOD-maintenance required(限值检查见5．3．4．2．2 2)
#define STATUSM_GOOD_MAINREQ_LOW	(SSTATUS_NGOOD_MAINREQ | STATUS_LIMIT_HIGH)  //符合NAMuRNEl07的含义Maintenance(M) 在PCS／DCS中的用法GOOD 维护站Maintenance required 0xA4～0xA7 GOOD-maintenance required(限值检查见5．3．4．2．2 2)
#define STATUSM_GOOD_MAINREQ_HIG	(SSTATUS_NGOOD_MAINREQ | STATUS_LIMIT_LOW)   //符合NAMuRNEl07的含义Maintenance(M) 在PCS／DCS中的用法GOOD 维护站Maintenance required 0xA4～0xA7 GOOD-maintenance required(限值检查见5．3．4．2．2 2)
#define STATUSM_GOOD_MAINREQ_CST	(SSTATUS_NGOOD_MAINREQ | STATUS_LIMIT_HIGH)  //符合NAMuRNEl07的含义Maintenance(M) 在PCS／DCS中的用法GOOD 维护站Maintenance required 0xA4～0xA7 GOOD-maintenance required(限值检查见5．3．4．2．2 2)

#define STATUSM_GOOD_MAINDEMA_OK	(0xA8 | STATUS_LIMIT_LOW)   //符合NAMuRNEl07的含义Maintenance(M) 在PCS／DCS中的用法GOOD 维护站Maintenance demanded 0xA8～0xAB GOOD-maintenance demanded(限值检查见5．3．4．2．2 2)
#define STATUSM_GOOD_MAINDEMA_LOW	(0xA8 | STATUS_LIMIT_HIGH)  //符合NAMuRNEl07的含义Maintenance(M) 在PCS／DCS中的用法GOOD 维护站Maintenance demanded 0xA8～0xAB GOOD-maintenance demanded(限值检查见5．3．4．2．2 2)
#define STATUSM_GOOD_MAINDEMA_HIG	(0xA8 | STATUS_LIMIT_LOW)   //符合NAMuRNEl07的含义Maintenance(M) 在PCS／DCS中的用法GOOD 维护站Maintenance demanded 0xA8～0xAB GOOD-maintenance demanded(限值检查见5．3．4．2．2 2)
#define STATUSM_GOOD_MAINDEMA_CST	(0xA8 | STATUS_LIMIT_HIGH)  //符合NAMuRNEl07的含义Maintenance(M) 在PCS／DCS中的用法GOOD 维护站Maintenance demanded 0xA8～0xAB GOOD-maintenance demanded(限值检查见5．3．4．2．2 2)

#define STATUSG_GOOD_FUNCHK_OK		(0xBC | STATUS_LIMIT_LOW)   //符合NAMuRNEl07的含义GOOD(G) 在PCS／DCS中的用法GOOD 维护站Function Check 0xBC～0xBF GOOD-function check(限值检查见5．3．4．2．2 2)
#define STATUSG_GOOD_FUNCHK_LOW		(0xBC | STATUS_LIMIT_HIGH)  //符合NAMuRNEl07的含义GOOD(G) 在PCS／DCS中的用法GOOD 维护站Function Check 0xBC～0xBF GOOD-function check(限值检查见5．3．4．2．2 2)
#define STATUSG_GOOD_FUNCHK_HIG		(0xBC | STATUS_LIMIT_LOW)   //符合NAMuRNEl07的含义GOOD(G) 在PCS／DCS中的用法GOOD 维护站Function Check 0xBC～0xBF GOOD-function check(限值检查见5．3．4．2．2 2)
#define STATUSG_GOOD_FUNCHK_CST		(0xBC | STATUS_LIMIT_HIGH)  //符合NAMuRNEl07的含义GOOD(G) 在PCS／DCS中的用法GOOD 维护站Function Check 0xBC～0xBF GOOD-function check(限值检查见5．3．4．2．2 2)




/*
 * 5．3．3经典诊断
 * 表69物理块参数DIAGNOSIS的编码
 */

//DIAGNOSIS的八位位组1
#define B0_DIA_HW_ELECTR  		0x01  // 电子方面的硬件故障
#define B0_DIA_HW_MECH    		0x02  // 机械方面的硬件故障
#define B0_DIA_TEMP_MOTOR   	0x04  // 电机温度过高
#define B0_DIA_TEMP_ELECTR	    0x08  // 电子器件温度过高
#define B0_DIA_MEM_CHKSUM    	0x10  // 存储器错误
#define B0_DIA_MEASUREMENT	    0x20  // 测量失败
#define B0_DIA_NOT_INIT		    0x40  // 设备未初始化(无白校准)
#define B0_DIA_INIT_ERR		    0x80  // 白校准失败
//DIAGNOSIS的八位位组2
#define B1_DIA_ZERO_ERR  		0x01  // 零点错误(限值位置)
#define B1_DIA_SUPPLY    		0x02  // 电源故障(电动、气动)
#define B1_DIA_CONF_INVAL		0x04  // 组态无效
#define B1_DIA_WARMSTART   		0x08  // 上电后设置，或执行FACTORY_RESET=2506后将此比特置位
#define B1_DIA_COLDSTART	    0x10  // 执行FACTORY_RESET=1后将此比特置位
#define B1_DIA_MAINTAINANCE    	0x20  // 需要维护
#define B1_DIA_CHARACT		    0x40  // 特性化无效
#define B1_IDENT_NUMBER_VIOLATION    0x80  // 如果正运行的循环数据传输的Ident_Number与PB参数IDENT_NUMBER_SELECTOR的值不一
										//致，则设置为1。如果IDENT_NUMBER_SELECTOR=127(适应模式)，则DIAGNOSIS的比特被清除／不置位
//DIAGNOSIS的八位位组3  保留
#define B2_DIA_MAINTENANCE_ALARM  		0x01  // 设备或配套附件故障
#define B2_DIA_MAINTENANCE_DEMANDED   	0x02  // 必须维护
#define B2_DIA_FUNCTION_CHECKL			0x04  // 设备处于功能检查模式，或处于仿真，或在本地控制下，例如维护
#define B2_DIA_INV_PRO_COND   			0x08  // 过程条件不允许返回有效的值。如果值的质量为UNCERTAIN-Process related，no maintenance或BAD-Processrelated，no maintenance，则将其置位

//DIAGNOSIS的八位位组4
#define B3_EXTENSION_AVAILABLE  		0x80  // 在DIAGNOSIS—EXTENSION中，有更多的诊断信息可用



/*
 * 5．3．4浓缩状况和诊断
 */

/*
 * 5．3．4．3．3．2 DIAGNOSIS
 * 如果发生诊断事件，则DIAGNOSIS比特中的一个比特必须被置位。对于单个诊断事件至多一个比
特置位。不同的诊断事件能导致不同的诊断比特置位。如果在参数DIAGNOSIS_EXTENSION中存在更
多可用的信息(至少一个比特置位)，则应将比特EXTENSION_AVAILABLE单独置位(见表82)。
 */
//DIAGNOSIS的八位位组0
//保留B0_XX
//DIAGNOSIS的八位位组1

#define CB1_DIA_WARMSTART	    0x08  // 上电后或在执行FACTORY_RESET = 2506后，应被置位,A,Ext_Diag=0
#define CB1_DIA_COLDSTART    	0x10  // 在执行FACTORY_RESET=1后，应被置位,A,Ext_Diag=0
#define CB1_DIA_MAINTENANCE	    0x20  // 需要维护,R,Ext_Diag=0

#define CB1_IDENT_NUMBER_VIOLATION	  0x80  // 如果运行中的循环数据传输的ldent—
													//Number和物理块参数IDENT_NUMBER_SELECTOR的值不一致，则应置
													//位。如果IDENT_NUMBER_SELECTOR=127(适应模式)，则DIAGNOSIS
													//的比特IDENT_NUMBER_VIOLATION被清零／不置位,R,Ext_Diag=0
//DIAGNOSIS的八位位组2
#define CB2_DIA_MAINTENANCE_ALARM  		0x01  // 设备或配套附件故障,R,Ext_Diag=1
#define CB2_DIA_MAINTENANCE_DEMANDED   	0x02  // 必须维护,R,Ext_Diag=0
#define CB2_DIA_FUNCTION_CHECK		0x04  // 设备处于功能检查模式，或处于仿真，或在本地控制下，例如维护,,R,Ext_Diag=0
#define CB2_DIA_INV_PRO_COND   		0x08  // 过程条件不允许返回有效的值。如果值的质量为UNCERTAIN-Process related，
										  //no maintenance或BAD-Process related，no maintenance，则将其置位,R,Ext_Diag=0

//DIAGNOSIS的八位位组3
#define CB3_EXTENSION_AVAILABLE  		0x80  // 在DIAGNOSIS—EXTENSION中，有更多的诊断信息可用

// R指示：只要引起该消息的原因存在，该指示就一直保持有效。
// A指示：在10 s后自动复位。

/*
 *
 * 5．4参数处理
 * 参数处理(parameter transaction)是向设备传送连续数据集的一个写服务序列。该机制是面向参
 * 数的，可能传送视图对象或其他参数。在处理期间所传送参数的个数取决于运行情况。可传送设备的
 * 所有参数或仅一个子集。在处理序列期间，不进行设备参数间的一致性检查。
 * 如果设备处于写保护模式下，则该设备拒绝激活参数处理。通过软件或硬件方式实现的写保护机
 * 制会导致该拒绝行为的发生。
 * 在开始参数处理后，写保护状态的改变(受保护或不受保护)不会影响处理的继续进行。对于通过
 * 软件或硬件方式激活的写保护机制，该定义都有效。
 *
 */


typedef struct {
  uint16_t TB_ID;
  uint16_t relative_index;
}_PACKED_ CHANNEL_t;


typedef struct {
	IDX_STDParameters_t Std;

	__Def_ParameterAttr(PA_BatchDS67_t*, pBATCH );//属于标准参数部分，此参数旨在用于符合GB／T 19892．1—2005的批应用。仅功能块具有此参数。在功能块内
							//不需要算法。在分布式现场总线系统中BATCH参数是必备的，用以识别所用的和可用的
							//通道。此外．在警报的情况下还识别当前的批。详见5 2．3．19
	__Def_ParameterAttr(void*,pResv9);
	__Def_ParameterAttr(PA_ValueStatusFP101_t*, pOut);
		/*
		 * 在自动(AUTO)模式下，功能块参数OUT包含以制造商特定的或组态调整的工程单位计量的当前测量值以及所属状态。
		 * 在手动模式(MANMODE)，功能块参数OUT包含由操作员所设置的值和状态。
		 */
	__Def_ParameterAttr(float*, pPv_scale);//第一浮点值：100％的EU值(PV_SCALE,EU_at_100％)，第二浮点值：0%的EU值(PV_SCALE,EU_at_0%).
		/*
		 *  使用高、低标度值将过程变量转换成百分数。PV_SCALE的高、低标度值的工程单位与所组态的转换块(通过Channel参数进行组态)的PV—UNIT直接有关。
		*  PV_SCALE的高、低标度值自动地随着相关转换块中PV_UNIT的改变而改变，即转换块PV_UNIT的改变并不导致Al中OUT的扰动。
		 *   也可能存在例外情况，例如，清洗分析仪器会要求扰动
		 */
	__Def_ParameterAttr(PA_ScalingDS36_t*, pOut_scale);
		/*
		 * 过程变量的标度。
		 * OUT_SCALE 功能块参数OUT_SCALE包括上限值和下限值的有效范围、过程变量工程单位的代码
		 * 号，以及小数点右边的有效数字位数
		 */
	__Def_ParameterAttr(uint8_t*, pLinType);
		/*
		 * 线性化类型。详细内容见表50
		 */
	__Def_ParameterAttr(CHANNEL_t*, pChannel);
		/*
		 * 对有效转换块的引用，该转换块为功能块提供测量值。详见5．1．5“通用要求定义”
		 */
	__Def_ParameterAttr(void*,pResv15);
	__Def_ParameterAttr(float*, pPv_ftime);
		/*
		 * 过程变量的滤波时间。
		 * 功能块参数PV_FTIME包含当功能块输出达到63.21%时的上升时间，为时间常量，这
		 * 是输入值跃变的结果(PTI滤波器)。该参数的工程单位为s。
		 */

	__Def_ParameterAttr(uint8_t*, pFsafe_type);//如果此参数未被实现，则AIFB的行为如同FSAFE_TYPE=1。
		/*
			  定义当检测到故障时设备的反应。所计算的实际模式保持在AUTO。
			0：值FSAFE_VALUE被用作OUT
			状况=UNCERTAIN—substitute value b，
			1：使用上一次存储的有效OUT值
			状况=UNCERTAIN-last usable value
			(如果没有有效值可用，则应该使用UNCERTAIN—initial value；在此情况下，OUT值=
			初始值)“
			2：OUT具有错误的计算值和状况
			状况=BAD_(a)，(b)
			a,计算出的。
		    b,此处使用经典状况定义，如使用浓缩状况定义见表83。
		 */
	__Def_ParameterAttr(float*, pFsafe_value);
		/*
		 * 当检测到传感器或传感器电子元件故障时，用于OUT参数的缺省值。该参数的单位与
		 * OUT参数的单位相同
		 */
	__Def_ParameterAttr(float*, pAlarm_hys); //缺省值 0.5
		/*
		 * 滞后(Hysteresis)
			在PROFIBUS PA变送器规范的范围内，有一些功能用于监视可调限值的超限(偏离限
			值条件)。也许一个过程变量的值恰恰与限值相同，并且该变量围绕限值上下波动，由此
			它可导致多次超限。
			这将触发许多消息；因此，应有可能仅在超过可调的滞后之后才触发消息。触发报警消
			息的灵敏度是可调的。滞后值固定为ALARM_HYS，这对于参数HI_HI_LIM、HI_LIM、LO_LIM、LO_LO_LIM是一样的。滞后被表示为以xx_LlM为工程单位计量的低
			于上限和高于下限的值
		 */
	__Def_ParameterAttr(void*,pResv20);
	__Def_ParameterAttr(float*, pHihilim);
		/*
		 * 上上限报警(upper limit of alarm)值
			如果所测的变量等于或高于HI—HI—LIM值，则将OUT的状况字节的Limit比特设为
			“high limited”且FB参数ALARM—SUM中的HLHLAlarm比特必须变为1。该参数的
			单位与OUT参数的单位相同
		 */
	__Def_ParameterAttr(void*,pResv22);
	__Def_ParameterAttr(float*, pHilim);
		/*
		 * 上限报警(upper limit of warning)值
			如果所测的变量等于或高于HI_l。IM值，则将OUT的状况字节的Limit比特设为“high
			limited”且FB参数ALARM—SUM中的HI—Alarm比特必须变为1。该参数的单位与
			OUT参数的单位相同
		 */
	__Def_ParameterAttr(void*,pResv24);
	__Def_ParameterAttr(float*, pLolim);
		/*
		 * 下限报警(10wer limit of warning)值
			如果所测的变量等于或低于L(LLIM值，则将OUT的状况字节的Limit比特设为“low
			limited”和FB参数AI．ARM—SUM中的I．O—Alarm比特必须变为1。该参数的单位与
			OUT参数的单位相同
		 */
	__Def_ParameterAttr(void*,pResv26);
	__Def_ParameterAttr(float*, pLololim);
	__Def_ParameterAttr(void*,pResv28);
	__Def_ParameterAttr(void*,pResv29);
		/*
		 * 下下限报警(10wer limit of alarm)值
		 * 如果所测的变量等于或低于LO-I…O 1 IM值，则将OUT的状况字节的Limit比特设为
		 * “lowlimited”且FB参数AI，ARM SUM中的I，()-LO—Alarm比特必须变为1。该参数的
		单位与OUT参数的单位相同
		 */
	__Def_ParameterAttr(PA_AlarmFloatDS39_t*, pHihialm);
		/*
		 * 上上限报警的状态
		 * 该参数包含上上限报警的状态和相关的时间戳。该时间戳表示所测变量已经等于或高
			于上上限报警值时的时间
		 */
	__Def_ParameterAttr(PA_AlarmFloatDS39_t*, pHialm);
		/*
		 * 上限报警的状态
		 * 该参数包含上限报警的状态和相关的时间戳。该时间戳表示所溯变量已经等于或高于
			上限报警值时的时间
		 */
	__Def_ParameterAttr(PA_AlarmFloatDS39_t*, pLoalm);
		/*
		 * 下限报警的状态
		 * 该参数包含下限报警的状态和相关的时间戳。该时间戳表示所测变量已经等于或低于
			下限报警值时的时间
		 */
	__Def_ParameterAttr(PA_AlarmFloatDS39_t*, pLoloalm);
		/*
		 * 下下限报警的状态
		 * 该参数包括下下限报警的状态和相关的时间戳。该时间戳表示所测变量已经等于或低
			于下下限报警值时的时间
		 */
	__Def_ParameterAttr(PA_SimuFloatPtDS50_t*, pSimulate);
	/*
	 * 为了调试和测试的目的，可以修改从转换块进入模拟输入功能块AI FB的输入值。这意
	 * 味着转换块与AI FB间的连接被断开
	 */
	__Def_ParameterAttr(char*, pOut_unit_txt);
	/*
	 * 如果OUT参数的特定单位不在代码表内(见“通用要求”)，则用户可以在此参数中写人
	 * 特定文本。单位代码则等于“文本单位定义”
	 */

	__Def_ParameterAttr(uint8_t*, pOpt_status) ;
}_PACKED_ IDX_AIFB_t;

typedef struct {
	PA_STDParameters_t Std;
	PA_BatchDS67_t BATCH ;//属于标准参数部分，此参数旨在用于符合GB／T 19892．1—2005的批应用。仅功能块具有此参数。在功能块内
							//不需要算法。在分布式现场总线系统中BATCH参数是必备的，用以识别所用的和可用的
							//通道。此外．在警报的情况下还识别当前的批。详见5 2．3．19
	PA_ValueStatusFP101_t Out;
		/*
		 * 在自动(AUTO)模式下，功能块参数OUT包含以制造商特定的或组态调整的工程单位计量的当前测量值以及所属状态。
		 * 在手动模式(MANMODE)，功能块参数OUT包含由操作员所设置的值和状态。
		 */
	float      Pv_scale[2];//第一浮点值：100％的EU值(PV_SCALE,EU_at_100％)，第二浮点值：0%的EU值(PV_SCALE,EU_at_0%).
		/*
		 *  使用高、低标度值将过程变量转换成百分数。PV_SCALE的高、低标度值的工程单位与所组态的转换块(通过Channel参数进行组态)的PV—UNIT直接有关。
		*  PV_SCALE的高、低标度值自动地随着相关转换块中PV_UNIT的改变而改变，即转换块PV_UNIT的改变并不导致Al中OUT的扰动。
		 *   也可能存在例外情况，例如，清洗分析仪器会要求扰动
		 */
	PA_ScalingDS36_t      Out_scale;
		/*
		 * 过程变量的标度。
		 * OUT_SCALE 功能块参数OUT_SCALE包括上限值和下限值的有效范围、过程变量工程单位的代码
		 * 号，以及小数点右边的有效数字位数
		 */
	uint8_t            LinType;
		/*
		 * 线性化类型。详细内容见表50
		 */
	CHANNEL_t             Channel;
		/*
		 * 对有效转换块的引用，该转换块为功能块提供测量值。详见5．1．5“通用要求定义”
		 */
	float      Pv_ftime;
		/*
		 * 过程变量的滤波时间。
		 * 功能块参数PV_FTIME包含当功能块输出达到63.21%时的上升时间，为时间常量，这
		 * 是输入值跃变的结果(PTI滤波器)。该参数的工程单位为s。
		 */
	uint8_t      Fsafe_type;//如果此参数未被实现，则AIFB的行为如同FSAFE_TYPE=1。
		/*
			  定义当检测到故障时设备的反应。所计算的实际模式保持在AUTO。
			0：值FSAFE_VALUE被用作OUT
			状况=UNCERTAIN—substitute value b，
			1：使用上一次存储的有效OUT值
			状况=UNCERTAIN-last usable value
			(如果没有有效值可用，则应该使用UNCERTAIN—initial value；在此情况下，OUT值=
			初始值)“
			2：OUT具有错误的计算值和状况
			状况=BAD_(a)，(b)
			a,计算出的。
		    b,此处使用经典状况定义，如使用浓缩状况定义见表83。
		 */
	float Fsafe_value;
		/*
		 * 当检测到传感器或传感器电子元件故障时，用于OUT参数的缺省值。该参数的单位与
		 * OUT参数的单位相同
		 */
	float Alarm_hys; //缺省值 0.5
		/*
		 * 滞后(Hysteresis)
			在PROFIBUS PA变送器规范的范围内，有一些功能用于监视可调限值的超限(偏离限
			值条件)。也许一个过程变量的值恰恰与限值相同，并且该变量围绕限值上下波动，由此
			它可导致多次超限。
			这将触发许多消息；因此，应有可能仅在超过可调的滞后之后才触发消息。触发报警消
			息的灵敏度是可调的。滞后值固定为ALARM_HYS，这对于参数HI_HI_LIM、HI_LIM、LO_LIM、LO_LO_LIM是一样的。滞后被表示为以xx_LlM为工程单位计量的低
			于上限和高于下限的值
		 */
	float Hihilim;
		/*
		 * 上上限报警(upper limit of alarm)值
			如果所测的变量等于或高于HI—HI—LIM值，则将OUT的状况字节的Limit比特设为
			“high limited”且FB参数ALARM—SUM中的HLHLAlarm比特必须变为1。该参数的
			单位与OUT参数的单位相同
		 */
	float Hilim;
		/*
		 * 上限报警(upper limit of warning)值
			如果所测的变量等于或高于HI_l。IM值，则将OUT的状况字节的Limit比特设为“high
			limited”且FB参数ALARM—SUM中的HI—Alarm比特必须变为1。该参数的单位与
			OUT参数的单位相同
		 */
	float Lolim;
		/*
		 * 下限报警(10wer limit of warning)值
			如果所测的变量等于或低于L(LLIM值，则将OUT的状况字节的Limit比特设为“low
			limited”和FB参数AI．ARM—SUM中的I．O—Alarm比特必须变为1。该参数的单位与
			OUT参数的单位相同
		 */
	float Lololim;
		/*
		 * 下下限报警(10wer limit of alarm)值
		 * 如果所测的变量等于或低于LO-I…O 1 IM值，则将OUT的状况字节的Limit比特设为
		 * “lowlimited”且FB参数AI，ARM SUM中的I，()-LO—Alarm比特必须变为1。该参数的
		单位与OUT参数的单位相同
		 */
	PA_AlarmFloatDS39_t Hihialm;
		/*
		 * 上上限报警的状态
		 * 该参数包含上上限报警的状态和相关的时间戳。该时间戳表示所测变量已经等于或高
			于上上限报警值时的时间
		 */
	PA_AlarmFloatDS39_t Hialm;
		/*
		 * 上限报警的状态
		 * 该参数包含上限报警的状态和相关的时间戳。该时间戳表示所溯变量已经等于或高于
			上限报警值时的时间
		 */
	PA_AlarmFloatDS39_t Loalm;
		/*
		 * 下限报警的状态
		 * 该参数包含下限报警的状态和相关的时间戳。该时间戳表示所测变量已经等于或低于
			下限报警值时的时间
		 */
	PA_AlarmFloatDS39_t Loloalm;
		/*
		 * 下下限报警的状态
		 * 该参数包括下下限报警的状态和相关的时间戳。该时间戳表示所测变量已经等于或低
			于下下限报警值时的时间
		 */
	PA_SimuFloatPtDS50_t Simulate;
	/*
	 * 为了调试和测试的目的，可以修改从转换块进入模拟输入功能块AI FB的输入值。这意
	 * 味着转换块与AI FB间的连接被断开
	 */
	char Out_unit_txt[16];
	/*
	 * 如果OUT参数的特定单位不在代码表内(见“通用要求”)，则用户可以在此参数中写人
	 * 特定文本。单位代码则等于“文本单位定义”
	 */

	/**
	 * 操作变量，
	 */
#define AIFB_OPTStatus_Normal    0  //正常
#define AIFB_OPTStatus_OPTWrite  1  //操作员写

    uint8_t Opt_status ;
}_PACKED_ PA_AIFB_t;


typedef struct {
	IDX_STDParameters_t Std;
	__Def_ParameterAttr(void*,pResv8);
	__Def_ParameterAttr(PA_ValueStatusFP101_t*,pSp);
	/*
	 * 设定值。在AUTO模式下，定义行程跨度范围内(在位置OPEN与CI．OSE之间)
最终控制元件的位置，以Pv—SCALE的单位计
	 */
	__Def_ParameterAttr(void*,pResv10);
	__Def_ParameterAttr(PA_ScalingDS36_t*,pPvScale);
		/*
		 * 将以工程单位计的PV转换成以百分数计的PV，作为功能块的输入值。其组成
有：低标度值、高标度值、工程单位代码和十进制小数点右边的位数
		 */
	__Def_ParameterAttr(PA_ValueStatusFP101_t*,pReadback);
    /*
     * 在行程跨度范围内(在位置OPEN与CLOSE之间)最终控制元件的实际位置，以
PV_SCALE为单位
     */
	__Def_ParameterAttr(void*,pResv13);
	__Def_ParameterAttr(PA_ValueStatusFP101_t*,pRcasIn);
	/*
	 * 在RCas模式下，以PVSCALE为单位的目标设定值，以及由监控主机向模拟控
制或输出块提供的状况
	 */
	__Def_ParameterAttr(void*,pResv15);
	__Def_ParameterAttr(void*,pResv16);
	__Def_ParameterAttr(void*,pResv17);
	__Def_ParameterAttr(void*,pResv18);
	__Def_ParameterAttr(void*,pResv19);
	__Def_ParameterAttr(void*,pResv20);
	__Def_ParameterAttr(uint16_t*,pInChannel);
	/*
	 * 对有效的转换块及其参数的引用，它提供了最终控制元件(FEEDBACK—VALUE
D)的实际位置。更多描述见“通用要求”(CHANNEL)
	 */
	__Def_ParameterAttr(uint16_t*,pOutChannel);
	/*
	 * 对有效的转换块及其参数的引用，它提供了最终控制元件(pOSITIONING—
VALUE_D)的实际位置。更多描述见“通用要求”(CHANNEL)
	 */
	__Def_ParameterAttr(float*,pFsafeTime);
		/*
		 * 从检测出实际使用的设定值错误(SP—D—BAD或RCAS_IN<>GOOD)到功能
			块动作(该错误条件仍然存在)所需的时间，以s计。
			注：通信超时将使所传输的设定值状况改变为BAD(见映射)
		 */
	__Def_ParameterAttr(uint8_t*,pFsafeType);
	/*
	 * 在FsAFE—TIME之后仍然检测到实际使用的设定值错误的情况下。或者在实际
		使用的设定值的状况为“Initiate Fail Safe”情况下，定义设备的反应。
		计算的实际模式分别为AUTO或RCAs(见5．2．3．6)。
		编码：
		0：值FSAFE_VALUE_D用作设定值
			OuT的状况=UNCERTAIN—substitute value
		l： 使用上一个有效设定值
			0UT的状况=UNCERTAIN—Last Usable Value
			或BAD-No communication，no LUV
		2： 执行器进入由ACTUATOR—ACTION所定义的故障一安全位置(仅可
			用于具有弹性恢复的执行器)，OUT的状况=BAD-non specific
	 */
	__Def_ParameterAttr(float*,pFsafeValue);
      /*
       * 如果FSAFE_TYPE=0且FSAFE被激活，则使用该设定值
       */
	__Def_ParameterAttr(void*,pResv26);
	__Def_ParameterAttr(PA_ValueStatusFP101_t*,pRcasOut);
      /*
       *以PV_SCALE为单位的功能块的设定值和状况。将它们提供给监控主机，用于
监控／反向计算，以允许在受限条件或在模式改变情况下采取行动
       */
	__Def_ParameterAttr(void*,pResv28);
	__Def_ParameterAttr(void*,pResv29);
	__Def_ParameterAttr(void*,pResv30);
	__Def_ParameterAttr(PA_ValueStatusDS102_t*,pPosD);
      /*
       * 阀的当前位置(离散)。编码:
			0:未初始化
			1:关闭
			2:打开
			3:中间位置
       */
	__Def_ParameterAttr(float*,pSetpDeviation);
      /*
       * OUT信号与反馈位置之间的差异，以行程跨度的百分数计(在位置OPEN与
CLOSE之间)
       */
	__Def_ParameterAttr(char*,pCheckBack);
      /*
       * 设备的详细信息，按位编码。同时可以有多个信息
       */
	__Def_ParameterAttr(char*,pCheckBackMask);
      /*
       * 定义是否支持CHECK—BACK的信息比特。编码：
		0:不支持
		1:支持
       */
	__Def_ParameterAttr(PA_SimuFloatPtDS50_t*,pSimulate);
      /*
       * 由于调试和维护原因，可通过定义值和状况来仿真READBACK。此时，将断开从
转换块到AO FB之间的信号路径
       */
	__Def_ParameterAttr(uint8_t*,pIncreaseClose);
      /*
       * 在RCAS和AUTO模式下，定义的相对于设定值的执行器运动方向。
		编码：
		0:上升(设定值的增加导致阀开启--()PENNING)
		1:下降(设定值的增加导致阀关闭一cI。OSING)
		INCREASE—CLOSE的设置会影响下列参数：READBACK、POS—D、OUT
		和CHECKBACK
       */
	__Def_ParameterAttr(PA_ValueStatusFP101_t*,pOut);
      /*
       * 此参数是AUTO和RCas模式下模拟输出块的过程变量，以工程单位计。其值由
操作员／工程师在MAN和IO模式下设定
       */
	__Def_ParameterAttr(PA_ScalingDS36_t*,pOutScale);
      /*
       * 将功能块中以百分数计的OUT转换成以工程单位计的OUT，作为该功能块的输
			出值。其组成有：低标度值、高标度值、工程单位代码和十进制小数点右边的位数。
			至少应支持以下单位：mm、。(度)、％(取决于VALVE-TYPE)
       */
}_PACKED_ IDX_AOFB_t;

typedef struct {
	PA_STDParameters_t Std;

	PA_ValueStatusFP101_t Sp;
	/*
	 * 设定值。在AUTO模式下，定义行程跨度范围内(在位置OPEN与CI．OSE之间)
最终控制元件的位置，以Pv—SCALE的单位计
	 */
	PA_ScalingDS36_t      PvScale;
		/*
		 * 将以工程单位计的PV转换成以百分数计的PV，作为功能块的输入值。其组成
有：低标度值、高标度值、工程单位代码和十进制小数点右边的位数
		 */
	PA_ValueStatusFP101_t Readback;
    /*
     * 在行程跨度范围内(在位置OPEN与CLOSE之间)最终控制元件的实际位置，以
PV_SCALE为单位
     */
	PA_ValueStatusFP101_t RcasIn;
	/*
	 * 在RCas模式下，以PVSCALE为单位的目标设定值，以及由监控主机向模拟控
制或输出块提供的状况
	 */
	uint16_t InChannel;
	/*
	 * 对有效的转换块及其参数的引用，它提供了最终控制元件(FEEDBACK—VALUE
D)的实际位置。更多描述见“通用要求”(CHANNEL)
	 */
	uint16_t OutChannel;
	/*
	 * 对有效的转换块及其参数的引用，它提供了最终控制元件(pOSITIONING—
VALUE_D)的实际位置。更多描述见“通用要求”(CHANNEL)
	 */
	float      FsafeTime;
		/*
		 * 从检测出实际使用的设定值错误(SP—D—BAD或RCAS_IN<>GOOD)到功能
			块动作(该错误条件仍然存在)所需的时间，以s计。
			注：通信超时将使所传输的设定值状况改变为BAD(见映射)
		 */
	uint8_t FsafeType;
	/*
	 * 在FsAFE—TIME之后仍然检测到实际使用的设定值错误的情况下。或者在实际
		使用的设定值的状况为“Initiate Fail Safe”情况下，定义设备的反应。
		计算的实际模式分别为AUTO或RCAs(见5．2．3．6)。
		编码：
		0：值FSAFE_VALUE_D用作设定值
			OuT的状况=UNCERTAIN—substitute value
		l： 使用上一个有效设定值
			0UT的状况=UNCERTAIN—Last Usable Value
			或BAD-No communication，no LUV
		2： 执行器进入由ACTUATOR—ACTION所定义的故障一安全位置(仅可
			用于具有弹性恢复的执行器)，OUT的状况=BAD-non specific
	 */
      float FsafeValue;
      /*
       * 如果FSAFE_TYPE=0且FSAFE被激活，则使用该设定值
       */
      PA_ValueStatusFP101_t   RcasOut;
      /*
       *以PV_SCALE为单位的功能块的设定值和状况。将它们提供给监控主机，用于
监控／反向计算，以允许在受限条件或在模式改变情况下采取行动
       */
      PA_ValueStatusDS102_t PosD;
      /*
       * 阀的当前位置(离散)。编码:
			0:未初始化
			1:关闭
			2:打开
			3:中间位置
       */
      float SetpDeviation;
      /*
       * OUT信号与反馈位置之间的差异，以行程跨度的百分数计(在位置OPEN与
CLOSE之间)
       */
      char CheckBack[3];
      /*
       * 设备的详细信息，按位编码。同时可以有多个信息
       */
      char CheckBackMask[3];
      /*
       * 定义是否支持CHECK—BACK的信息比特。编码：
		0:不支持
		1:支持
       */
      PA_SimuFloatPtDS50_t Simulate;
      /*
       * 由于调试和维护原因，可通过定义值和状况来仿真READBACK。此时，将断开从
转换块到AO FB之间的信号路径
       */
      uint8_t IncreaseClose;
      /*
       * 在RCAS和AUTO模式下，定义的相对于设定值的执行器运动方向。
		编码：
		0:上升(设定值的增加导致阀开启--()PENNING)
		1:下降(设定值的增加导致阀关闭一cI。OSING)
		INCREASE—CLOSE的设置会影响下列参数：READBACK、POS—D、OUT
		和CHECKBACK
       */
      PA_ValueStatusFP101_t Out;
      /*
       * 此参数是AUTO和RCas模式下模拟输出块的过程变量，以工程单位计。其值由
操作员／工程师在MAN和IO模式下设定
       */
      PA_ScalingDS36_t OutScale;
      /*
       * 将功能块中以百分数计的OUT转换成以工程单位计的OUT，作为该功能块的输
			出值。其组成有：低标度值、高标度值、工程单位代码和十进制小数点右边的位数。
			至少应支持以下单位：mm、。(度)、％(取决于VALVE-TYPE)
       */
}_PACKED_ PA_AOFB_t;


typedef struct {
	IDX_STDParameters_t Std;
	__Def_ParameterAttr(void*,pResv8);
	__Def_ParameterAttr(void*,pResv9);
	__Def_ParameterAttr(PA_ValueStatusDS102_t*,pOutD);
	/*
	 * OUT—D是功能块的输出。在Man模式下，该值由操作员设置
	 */
	__Def_ParameterAttr(void*,pResv11);
	__Def_ParameterAttr(void*,pResv12);
	__Def_ParameterAttr(void*,pResv13);
	__Def_ParameterAttr(uint16_t*,pChannel);
	/*
	 * 向功能块提供测量值的有效转换块的索引。详见“通用要求定义”
	 */
	__Def_ParameterAttr(uint8_t*,pInvert);
	/*
	 * 在Pv-D的输入值被保存在OUT二D之前，指示是否应对其进行逻辑反转。
	 * 0：不反转
		1：反转
	 */
	__Def_ParameterAttr(void*,pResv16);
	__Def_ParameterAttr(void*,pResv17);
	__Def_ParameterAttr(void*,pResv18);
	__Def_ParameterAttr(void*,pResv19);
	__Def_ParameterAttr(uint8_t*,pFsafeType);
	/*
	 * 定义在检测到故障时设备的反应。
		编码：
		0：将值FSAFE-VAI。UE作为OUT_D使用
		状况=UNCERTAIN—substitute value b
		1：使用上一次保存的OUT_D有效值
		状况=UNCERTAIN—last usable value
		(如果无有效值可用，则应使用UNCERTAIN—Initial value) b
		2：OUT_D具有错误的计算值和状态
		状态=BAD一(a),(b)
		a: 如所计算的；
		b: 此处使用经典状况定义；如果使用浓缩状况见表83
	 */
	__Def_ParameterAttr(uint8_t*,pFsafeValD);
	/**
	 * 当检测出传感器故障或传感器电子故障时，参数OUT_D的缺省值
	 */
	__Def_ParameterAttr(void*,pResv22);
	__Def_ParameterAttr(void*,pResv23);
	__Def_ParameterAttr(PA_SimuDiscreteDS51_t*,pSimulate);
	/*
	 * 为了便于调试和测试，离散输人功能块DI FB中来自转换块的输入值可通过该参数进行修改。
	 * 此时，断开转换块与DI FB的连接
	 */
}_PACKED_ IDX_DIFB_t;

typedef struct {
	PA_STDParameters_t Std;
	PA_ValueStatusDS102_t OutD;
	/*
	 * OUT—D是功能块的输出。在Man模式下，该值由操作员设置
	 */
	uint16_t Channel;
	/*
	 * 向功能块提供测量值的有效转换块的索引。详见“通用要求定义”
	 */
	uint8_t Invert;
	/*
	 * 在Pv-D的输入值被保存在OUT二D之前，指示是否应对其进行逻辑反转。
	 * 0：不反转
		1：反转
	 */
	uint8_t FsafeType;
	/*
	 * 定义在检测到故障时设备的反应。
		编码：
		0：将值FSAFE-VAI。UE作为OUT_D使用
		状况=UNCERTAIN—substitute value b
		1：使用上一次保存的OUT_D有效值
		状况=UNCERTAIN—last usable value
		(如果无有效值可用，则应使用UNCERTAIN—Initial value) b
		2：OUT_D具有错误的计算值和状态
		状态=BAD一(a),(b)
		a: 如所计算的；
		b: 此处使用经典状况定义；如果使用浓缩状况见表83
	 */
	uint8_t FsafeValD;
	/**
	 * 当检测出传感器故障或传感器电子故障时，参数OUT_D的缺省值
	 */
	PA_SimuDiscreteDS51_t Simulate;
	/*
	 * 为了便于调试和测试，离散输人功能块DI FB中来自转换块的输入值可通过该参数进行修改。
	 * 此时，断开转换块与DI FB的连接
	 */
}_PACKED_ PA_DIFB_t;

typedef struct {
	IDX_STDParameters_t Std;
	__Def_ParameterAttr(void*,pResv8);
	__Def_ParameterAttr(PA_ValueStatusDS102_t*,pSpD);
	/*
	 * AUTO模式下所用的功能块的设定值
	 */
	__Def_ParameterAttr(PA_ValueStatusDS102_t*,pOutD);
	/*
	 * 在AUTO、RCas模式下该参数是离散输出功能块的过程变量，在MAN和LO模
		式下该参数是由操作员／m程师设定的值。在BAD状况下，阀进入ACTUATOR_
		ACTION所规定的位置
	 */
	__Def_ParameterAttr(void*,pResv11);
	__Def_ParameterAttr(PA_ValueStatusDS102_t*,pReadBackD);
	/*
	 * 最终控制元件及其传感器的实际位置
	 */
	__Def_ParameterAttr(void*,pResv13);
	__Def_ParameterAttr(PA_ValueStatusDS102_t*,pRcasInD);
	/*
	 * 监控主机提供给处于RCAS模式下的离散输出功能块的目标设定值和状况
	 */
	__Def_ParameterAttr(void*,pResv15);
	__Def_ParameterAttr(void*,pResv16);
	__Def_ParameterAttr(uint16_t*,pChannel);
	/*
	 * 对有效的转换块及其参数的引用，它提供了最终控制元件(POSITIONING_VALUE_D)的实际位置。更多描述见“通用要求”(CHANNEL)
	 */
	__Def_ParameterAttr(uint8_t*,pInvert);
	/*
	 * 指示在模式AUTO或RCAS下写OUT_D之前是否应将SP_D逻辑取反。
		编码：
		0：不取反
		1：取反
	 */
	__Def_ParameterAttr(float*,pFsafeTime);
	/*
	 * 从检测出实际使用的设定值错误(SP_D=BAD或RCAS_IN<>GOOD)到功能块
		动作(该错误条件仍然存在)所需的时间，以s计。
		注：通信超时将使所传输的设定值状况改变为BAD(见6．3．4)
	 */
	__Def_ParameterAttr(uint8_t*,pFsafeType);
	/*
	 * 在FSAFE_TIME之后仍然检测到实际使用的设定值错误的情况下，或者在实际
		使用的设定值的状况为“Initiate Fail Safe”情况下，定义设备的反应。
		计算的实际模式为AUTO(见5．2．3 6)。
		编码：
		0： 值FSAFE_VALUE_D用作设定值
		OUT_D的状况=UNCERTAlN—substitute value
		1：使用上一个有效设定值
		OUT_D的状况=UNCERTAIN—Last usable Value
		或BAD_No communication，no LUV
		2： 执行器进入由ACTUATOR_ACTION所定义的故障安全位置，
		OUT_D的状况=BAD - non specific
	 */
	__Def_ParameterAttr(uint8_t*,pFsafeValD);
	/*
	 * 如果FSAFE_TYPE=0且FSAFE被激活，则OUT_D传递该值
	 */
	__Def_ParameterAttr(PA_ValueStatusDS102_t*,pRcasOutD);
	/*
	 * 向监控主机提供的功能块的设定值和状况，用于监控／反向计算(back
		calculation)，以允许在受限条件或模式变化的情况下采取行动
	 */
	__Def_ParameterAttr(void*,pResv23);
	__Def_ParameterAttr(PA_SimuDiscreteDS51_t*,pSimulate);
	/*
	 * 用于调试和维护，可通过定义值和状况来仿真READBACK。此时．转换块和DO FB的连接被断开
	 */
	__Def_ParameterAttr(void*,pResv25);
	__Def_ParameterAttr(void*,pResv26);
	__Def_ParameterAttr(void*,pResv27);
	__Def_ParameterAttr(void*,pResv28);
	__Def_ParameterAttr(void*,pResv29);
	__Def_ParameterAttr(void*,pResv30);
	__Def_ParameterAttr(void*,pResv31);
	__Def_ParameterAttr(void*,pResv32);
	__Def_ParameterAttr(uint8_t*,pCheckBack);
	/*
	 * 设备的详细信息，按位编码。同时可能有多个消息
	 */
	__Def_ParameterAttr(uint8_t*,pCheckBackMask);
	/*
	 * 定义是否支持CHECK_BACK的信息比特。
		每个比特的编码：
		0：不支持
		1：支持
	 */
}_PACKED_ IDX_DOFB_t;

typedef struct {
	PA_STDParameters_t Std;
	PA_ValueStatusDS102_t SpD;
	/*
	 * AUTO模式下所用的功能块的设定值
	 */
	PA_ValueStatusDS102_t OutD;
	/*
	 * 在AUTO、RCas模式下该参数是离散输出功能块的过程变量，在MAN和LO模
		式下该参数是由操作员／m程师设定的值。在BAD状况下，阀进入ACTUATOR_
		ACTION所规定的位置
	 */
	PA_ValueStatusDS102_t ReadBackD;
	/*
	 * 最终控制元件及其传感器的实际位置
	 */
	PA_ValueStatusDS102_t RcasInD;
	/*
	 * 监控主机提供给处于RCAS模式下的离散输出功能块的目标设定值和状况
	 */
	uint16_t Channel;
	/*
	 * 对有效的转换块及其参数的引用，它提供了最终控制元件(POSITIONING_VALUE_D)的实际位置。更多描述见“通用要求”(CHANNEL)
	 */
	uint8_t Invert;
	/*
	 * 指示在模式AUTO或RCAS下写OUT_D之前是否应将SP_D逻辑取反。
		编码：
		0：不取反
		1：取反
	 */
	float FsafeTime;
	/*
	 * 从检测出实际使用的设定值错误(SP_D=BAD或RCAS_IN<>GOOD)到功能块
		动作(该错误条件仍然存在)所需的时间，以s计。
		注：通信超时将使所传输的设定值状况改变为BAD(见6．3．4)
	 */
	uint8_t FsafeType;
	/*
	 * 在FSAFE_TIME之后仍然检测到实际使用的设定值错误的情况下，或者在实际
		使用的设定值的状况为“Initiate Fail Safe”情况下，定义设备的反应。
		计算的实际模式为AUTO(见5．2．3 6)。
		编码：
		0： 值FSAFE_VALUE_D用作设定值
		OUT_D的状况=UNCERTAlN—substitute value
		1：使用上一个有效设定值
		OUT_D的状况=UNCERTAIN—Last usable Value
		或BAD_No communication，no LUV
		2： 执行器进入由ACTUATOR_ACTION所定义的故障安全位置，
		OUT_D的状况=BAD - non specific
	 */
	uint8_t FsafeValD;
	/*
	 * 如果FSAFE_TYPE=0且FSAFE被激活，则OUT_D传递该值
	 */
	PA_ValueStatusDS102_t RcasOutD;
	/*
	 * 向监控主机提供的功能块的设定值和状况，用于监控／反向计算(back
		calculation)，以允许在受限条件或模式变化的情况下采取行动
	 */
	PA_SimuDiscreteDS51_t Simulate;
	/*
	 * 用于调试和维护，可通过定义值和状况来仿真READBACK。此时．转换块和DO FB的连接被断开
	 */
	uint8_t CheckBack[3];
	/*
	 * 设备的详细信息，按位编码。同时可能有多个消息
	 */
	uint8_t CheckBackMask[3];
	/*
	 * 定义是否支持CHECK_BACK的信息比特。
		每个比特的编码：
		0：不支持
		1：支持
	 */
}_PACKED_ PA_DOFB_t;


typedef struct {
	IDX_STDParameters_t Std;
	__Def_ParameterAttr(PA_ValueStatusFP101_t*, pPrimary_value);
		/*
		 * PRIMARY_VALUE是转换块的过程值和状况，它是模拟输入块的输入。当线性化
		 * 类型=0时，PRIMARY_VALUE包含与物位相同的值。其单位在PRIMARY_VALUE_UNIT中定义
		 */
	__Def_ParameterAttr(uint16_t*, pPrimary_value_unit);
		/*
		 * 为PRIMARY_VALUE和LIN_VOLUME所选择的单位代码。
		 * 必备：%,m,ft
		 */
	__Def_ParameterAttr(float*, pLevel);
		/*
		 * 由SENSOR—VALUE经线性转换直接导出的物位，该线性转换通过使用LEVEL—
		 * HI、LEVEL—LO、CAL—POINT—HI、CAL—POINT—LO和LEVEL—oFFSET实现。
			其单位在LEVEL_UNIT中定义
		 */
	__Def_ParameterAttr(uint16_t*, pLevel_unit);
		/*
		 * 为LEVEl。、LEVEl…HI 1。EVEL_I，O和LIN—DIAMETER所选择的单位代码。
		 * 必备：％，m，ft
		 */
	__Def_ParameterAttr(float*, pSensor_value);
		/*
		 * 传感器的物理值
		 */
	__Def_ParameterAttr(uint16_t*, pSensor_unit);
		/*
		 * 用于SENSOR—VAI．UE、SENSOR—LOW—LIMIT、SENSOR—HIGH—LIMIT、AL—
			P【)INT—HI、CAL—POINT—LO、MAX—SENSOR—VALUE和M1N—SENSOR—
			VAI。UE的单位。
			必备的压力单位：Pa，mbar，psi；必备的距离单位；131，ft
		 */
	__Def_ParameterAttr(PA_ValueStatusFP101_t*, pSecondary_value1);
		/*
		 * secndary—value 是LEVEL+LEVELOFFSET，以及转换块的状况。其
		 * 单位在SECONDARLVALUE 1一UNIT中定义。它能与第2个模拟输入块连接
		 */
	__Def_ParameterAttr(uint16_t*, pSecondary_value1_unit);
		/*
		 * 为SECONDARY-VALUE—l所选择的单位代码，与在LEVEl．一UNIT中定义的
			相同。必备：％，m，ft
		 */
	__Def_ParameterAttr(PA_ValueStatusFP101_t*, pSecondary_value2);
		/*
		 * SECONDARY_VAI。UE一2是SENSOR—VALUE+SENSOR—OFFSET，以及转换
		 * 块的状况。其单位在SECONDARY—vALUE一2一UNIT中定义。它能与第3个模拟
			输入块连接
		 */
	__Def_ParameterAttr(uint16_t*, pSecondary_value2_unit);
		/*
		 * 为SECONDARlLVALUE-2所选择的单位代码，与在SENSOR—UNIT中定义的
			相同。必备的压力单位：Pa，mbar，psi；必备的距离单位：m，ft
		 */
	__Def_ParameterAttr(float*, pSensor_offset);
		/*
		 * SENSOR_OFFSET是给SENSOR—VALUE添加的常数偏移量。其单位在SENSOR—UNIT中定义
		 */
	__Def_ParameterAttr(uint8_t*, pCal_type);
		/*
		 * 定义校准类型。
		编码：
		0： 离线；传感器的值对物位校准无影响。对于雷达设备是必备的。
		1：在线；传感器的当前值决定物位校准
		 */
    __Def_ParameterAttr(float*, pCal_point_lo);
		/*
		 * cALPOINT_Lo是SENSOR—VALUE的低校准点。它对应于LEVEL—LO。其单
		 * 位在SENSOR—UNIT中定义
		 */
    __Def_ParameterAttr(float*, pCal_point_hi);
		/*
		 * CALPOINT HI是SENSOR—VALuE的高校准点。它对应于I点VEL—HI。其单
		 * 位在SENSOR UNIT中定义
		 */
    __Def_ParameterAttr(float*, pLevel_lo);
		/*
		 * LEVELLO是处于CAL—POINT—LO时的物位值。其单位在LEVEL—UNIT中定义。在写人LEVELLO和CAL TYPE一1时，CALPOINT—LO自动被设为SENSOR—
			VALUE
		 */
    __Def_ParameterAttr(float*, pLevel_hi);
		/*
		 * LEVELHI是处于CAL—PoINT—HI时的物位值。其单位在I，EvEL—UNIT中定义。
		 * 在写入LEVEL HI和cALTⅥ)E一1时，cALPoINT_HI自动被设为SENSOR—VALUE
		 */
    __Def_ParameterAttr(float*, pLevel_offset);
		/*
		 * LEVEL—OFFSET是在物位校准的传送功能之后添加的常数偏移量。其单位在LEVELUNlT中定义
		 */
    __Def_ParameterAttr(uint8_t*, pLin_type);
		/*
		 * 见表50
		 */
    __Def_ParameterAttr(float*, pLin_diameter);
		/*
		 * 以LEVEL_UNIT为单位的圆柱卧式容器或球状容器的直径。
		 * 当LIN—TYPE一20或2l时使用
		 */
    __Def_ParameterAttr(float*, pLin_volume);
		/*
		 * LIN—VOIUME是圆柱卧式容器或球状容器的整体容量。
		 * 当IIN-TYPE_20或21时使用。其单位在PRIMARY_VAI。UE—UNIT中定义
		 */
    __Def_ParameterAttr(float*, pSensor_high_limit);
		/*
		 * 传感器的过程上限，其单位在SENSOR—UNIT中定义
		 */
    __Def_ParameterAttr(float*, pSensor_low_limit);
		/*
		 * 传感器的过程下限，其单位在SENSOR—UNIT中定义
		 */
    __Def_ParameterAttr(float*, pMax_sensor_value);
		/*
		 * 最大的过程值SENSOR—VALUE。其单位在SENSOR—UNIT中定义
		 */
    __Def_ParameterAttr(float*, pMin_sensor_value);
		/*
		 * 最小的过程值SENSOR—VALUE。其单位在SENSOR UNIT中定义
		 */
    __Def_ParameterAttr(float*, pTemperature);
		/*
		 * 过程温度
		 */
    __Def_ParameterAttr(uint16_t*, pTemperature_unit);
		/*
		 * 温度单位。为TEMPERATURE、MAX_TEMPERATURE、MAX_TEMPERATURE所选择的单位编码
		 */
    __Def_ParameterAttr(float*, pMax_temperature);
		/*
		 * 最大的过程温度
		 */
    __Def_ParameterAttr(float*, pMin_temperature);
		/*
		 * 最小的过程温度
		 */
    IDX_TAB_PARAMETER_t pTab;
}_PACKED_ IDX_LevelTB_t;


typedef struct {
	PA_STDParameters_t Std;
	PA_ValueStatusFP101_t Primary_value;
		/*
		 * PRIMARY_VALUE是转换块的过程值和状况，它是模拟输入块的输入。当线性化
		 * 类型=0时，PRIMARY_VALUE包含与物位相同的值。其单位在PRIMARY_VALUE_UNIT中定义
		 */
	uint16_t Primary_value_unit;
		/*
		 * 为PRIMARY_VALUE和LIN_VOLUME所选择的单位代码。
		 * 必备：%,m,ft
		 */
	float Level;
		/*
		 * 由SENSOR—VALUE经线性转换直接导出的物位，该线性转换通过使用LEVEL—
		 * HI、LEVEL—LO、CAL—POINT—HI、CAL—POINT—LO和LEVEL—oFFSET实现。
			其单位在LEVEL_UNIT中定义
		 */
	uint16_t Level_unit;
		/*
		 * 为LEVEl。、LEVEl…HI 1。EVEL_I，O和LIN—DIAMETER所选择的单位代码。
		 * 必备：％，m，ft
		 */
	float Sensor_value;
		/*
		 * 传感器的物理值
		 */
	uint16_t Sensor_unit;
		/*
		 * 用于SENSOR—VAI．UE、SENSOR—LOW—LIMIT、SENSOR—HIGH—LIMIT、AL—
			P【)INT—HI、CAL—POINT—LO、MAX—SENSOR—VALUE和M1N—SENSOR—
			VAI。UE的单位。
			必备的压力单位：Pa，mbar，psi；必备的距离单位；131，ft
		 */
	PA_ValueStatusFP101_t Secondary_value1;
		/*
		 * secndary—value 是LEVEL+LEVELOFFSET，以及转换块的状况。其
		 * 单位在SECONDARLVALUE 1一UNIT中定义。它能与第2个模拟输入块连接
		 */
	uint16_t Secondary_value1_unit;
		/*
		 * 为SECONDARY-VALUE—l所选择的单位代码，与在LEVEl．一UNIT中定义的
			相同。必备：％，m，ft
		 */
	PA_ValueStatusFP101_t Secondary_value2;
		/*
		 * SECONDARY_VAI。UE一2是SENSOR—VALUE+SENSOR—OFFSET，以及转换
		 * 块的状况。其单位在SECONDARY—vALUE一2一UNIT中定义。它能与第3个模拟
			输入块连接
		 */
	uint16_t Secondary_value2_unit;
		/*
		 * 为SECONDARlLVALUE-2所选择的单位代码，与在SENSOR—UNIT中定义的
			相同。必备的压力单位：Pa，mbar，psi；必备的距离单位：m，ft
		 */
	float Sensor_offset;
		/*
		 * SENSOR_OFFSET是给SENSOR—VALUE添加的常数偏移量。其单位在SENSOR—UNIT中定义
		 */
    uint8_t Cal_type;
		/*
		 * 定义校准类型。
		编码：
		0： 离线；传感器的值对物位校准无影响。对于雷达设备是必备的。
		1：在线；传感器的当前值决定物位校准
		 */
    float Cal_point_lo;
		/*
		 * cALPOINT_Lo是SENSOR—VALUE的低校准点。它对应于LEVEL—LO。其单
		 * 位在SENSOR—UNIT中定义
		 */
    float Cal_point_hi;
		/*
		 * CALPOINT HI是SENSOR—VALuE的高校准点。它对应于I点VEL—HI。其单
		 * 位在SENSOR UNIT中定义
		 */
    float Level_lo;
		/*
		 * LEVELLO是处于CAL—POINT—LO时的物位值。其单位在LEVEL—UNIT中定义。在写人LEVELLO和CAL TYPE一1时，CALPOINT—LO自动被设为SENSOR—
			VALUE
		 */
    float Level_hi;
		/*
		 * LEVELHI是处于CAL—PoINT—HI时的物位值。其单位在I，EvEL—UNIT中定义。
		 * 在写入LEVEL HI和cALTⅥ)E一1时，cALPoINT_HI自动被设为SENSOR—VALUE
		 */
    float Level_offset;
		/*
		 * LEVEL—OFFSET是在物位校准的传送功能之后添加的常数偏移量。其单位在LEVELUNlT中定义
		 */
    uint8_t Lin_type;
		/*
		 * 见表50
		 */
    float Lin_diameter;
		/*
		 * 以LEVEL_UNIT为单位的圆柱卧式容器或球状容器的直径。
		 * 当LIN—TYPE一20或2l时使用
		 */
    float Lin_volume;
		/*
		 * LIN—VOIUME是圆柱卧式容器或球状容器的整体容量。
		 * 当IIN-TYPE_20或21时使用。其单位在PRIMARY_VAI。UE—UNIT中定义
		 */
    float Sensor_high_limit;
		/*
		 * 传感器的过程上限，其单位在SENSOR—UNIT中定义
		 */
    float Sensor_low_limit;
		/*
		 * 传感器的过程下限，其单位在SENSOR—UNIT中定义
		 */
    float Max_sensor_value;
		/*
		 * 最大的过程值SENSOR—VALUE。其单位在SENSOR—UNIT中定义
		 */
    float Min_sensor_value;
		/*
		 * 最小的过程值SENSOR—VALUE。其单位在SENSOR UNIT中定义
		 */
    float Temperature;
		/*
		 * 过程温度
		 */
    uint16_t Temperature_unit;
		/*
		 * 温度单位。为TEMPERATURE、MAX_TEMPERATURE、MAX_TEMPERATURE所选择的单位编码
		 */
    float Max_temperature;
		/*
		 * 最大的过程温度
		 */
    float Min_temperature;
		/*
		 * 最小的过程温度
		 */
    PA_TAB_PARAMETER_t tab;
}_PACKED_ PA_LevelTB_t;


typedef struct {
	uint8_t Structure_Length; //指示此Prm_Structure的字节个数，包括Structure Length字节本身。固定为5
	uint8_t Structure_Type; //固定为65一行规特定
	uint8_t Slot_Number; //固定为0。意味着用此Prm—Strcuture进行的设置对整个设备都是有效的
	uint8_t reserved;
	uint8_t Options; // Bit0：PRM—COND	0：禁用浓缩状况；l；启用浓缩状况。Bitl～Bit7：保留
}_PACKED_ PA_Prm_Structure_t;



typedef struct {
    uint8_t   head;   /* 状态报告诊断信息块的引导头 */        // equal to DIAG_DEV_SIGN
    uint8_t   status_type;
    uint8_t   slot_number;
    uint8_t   specifier;
	uint8_t DIAGNOSIS[4];//设备的详细信息，按比特编码。同时可能包含多个消息。如果Byte4的MSB被设 为1，
						//则在参数DIAGNOSIS_EXTENSION内可提供更多诊断信息。	DIAGNOSIS参数的编码在5．3．3和5．3．4中规定
	uint8_t DIAGNOSIS_EXTENSION[6];//附加的制造商特定的设备信息，按比特编码。同时可能包含多个消息
}_PACKED_ PA_DIAG_t;


/**
 * 从站配置数据参考比较链表。
 * 当pNext == null, 没有下一个参考。
 *
 */
typedef struct __cfg_refer__
{
	 const uint8_t* pRef_cfg_data;
	 uint8_t ref_cfg_len;
	 ParameterAttr_t* pRealData[8];
}_PACKED_ CFG_Refer_t;

/**
 * 配置数据结构化，用于模块化从站
 * 该结构一般是数组使用，数组长度为最大可配置的模块数量。
 * 在pa_user_chk_new_cfg_data 函数中，记录输入、输出的各自位置，长度
 */
typedef struct
{
	DPL_STRUC_LIST_CB list;
	CFG_Refer_t* pCFG ;
}_PACKED_ cyc_IO_t;

typedef struct {
	uint8_t               NrOfPB;
	uint8_t               NrOfFB;
	uint8_t               NrOfTB;
	uint8_t               NrOfLO;

	IDX_PA_DM_t     idx_dm;
	IDX_PB_BLOCK_t  idx_pb[NUM_BLK_PB];
	IDX_AIFB_t 	  	idx_fb_AI[NUM_BLK_FB_AI];
	IDX_AOFB_t 	  	idx_fb_AO[NUM_BLK_FB_AO];
	IDX_DIFB_t 	  	idx_fb_DI[NUM_BLK_FB_DI];
	IDX_DOFB_t 	  	idx_fb_DO[NUM_BLK_FB_DO];
	IDX_LevelTB_t   idx_tb[NUM_BLK_TB];


	PA_DM_t    dm;
	/*
	 * start block parameters
	 *
	 * 特别注意！！！！
	 * PA设备的参数存储，是按照网络字序存储的，也即是大端模式。
	 * 1、如果是LITTLE_ENDIAN 小端处理器，要使用非字节(char,uint8_t,int8_t)
	 * 类型的参数，应该使用__ntohs(x)和__ntohl(x)进行转换后，再使用。
	 *
	 * 2、同理，如果是LITTLE_ENDIAN 小端处理器，要修改参数，如果参数
	 * 非字节(char,uint8_t,int8_t)类型,应该使用__htons(x)和__htonl(x)进行转换后,
	 * 再赋值给变量
	 *
	 */
	PB_BLOCK_t      pb[NUM_BLK_PB];
	PA_AIFB_t 	  	fb_AI[NUM_BLK_FB_AI];
	PA_AOFB_t 	  	fb_AO[NUM_BLK_FB_AO];
	PA_DIFB_t 	  	fb_DI[NUM_BLK_FB_DI];
	PA_DOFB_t 	  	fb_DO[NUM_BLK_FB_DO];
	PA_LevelTB_t    tb[NUM_BLK_TB];
	PA_FBLinkageDS49_t    lo[NUM_BLK_LO];
    /*
     * end block parameters
     */
	DPL_STRUC_LIST_CB pCycIHead;
	DPL_STRUC_LIST_CB pCycOHead;
	cyc_IO_t pCFG_FB_AI[NUM_BLK_FB_AI];
	uint8_t cntCfgAI ;
	cyc_IO_t pCFG_FB_AO[NUM_BLK_FB_AO];
	uint8_t cntCfgAO ;
	cyc_IO_t pCFG_FB_DI[NUM_BLK_FB_DI];
	uint8_t cntCfgDI ;
	cyc_IO_t pCFG_FB_DO[NUM_BLK_FB_DO];
	uint8_t cntCfgDO ;

	PA_Prm_Structure_t prm;
	PA_DIAG_t  dig;
} PA_DEV_t;



void PA_init(PA_DEV_t* dev);
uint8_t PB_Block_ViewGet(PB_BLOCK_t* pBlk,uint8_t* pOut,uint8_t viewID);
uint8_t AIFB_Block_ViewGet(PA_AIFB_t* pBlk,uint8_t* pOut,uint8_t viewID);
uint8_t AIFB_Block_VSget(PA_AIFB_t* pBlk,uint8_t* pOut);
uint8_t LevelTB_Block_ViewGet(PA_LevelTB_t* pBlk,uint8_t* pOut,uint8_t viewID);

void AIFB_Block_calcAll(
		PB_BLOCK_t* pPb,
		PA_AIFB_t* pBlk,
		PA_SimuFloatPtDS50_t* pSimu, /* 非null，指出仿真模式，否则为正常模式 */
		uint8_t target_mode, /* 目标模式 */
		PA_ValueStatusFP101_t* pPV_TB, /* 来自TB的计数输出 */
		float optInValue /*在MAN，O/S模式下，操作员指定的值 */
		);
void LevelTB_Block_calcAll(
		PB_BLOCK_t* pPb,
		PA_LevelTB_t* pBlk,
		PA_ValueStatusFP101_t* pSensorValue, /* 来自传感器的值 */
		float optInValue /*在MAN，O/S模式下，操作员指定的值 */
		);

void PA_timerTick1ms(void);
ParameterAttr_t* PAFindParamBlock(PA_DEV_t* pDev,
		uint8_t slot_num,/* 槽号 */
		uint8_t index,/* 索引号 */
		uint8_t* pErro_Slot/* 错误槽标记 */);

int PTA_Read(PA_DEV_t* pDev,PB_BLOCK_t* pBlk,DPV1_UNION_PDU_t* pdu);
int PTA_Write(PA_DEV_t* pDev,PB_BLOCK_t* pBlk,DPV1_UNION_PDU_t* pdu);

uint8_t PA_chk_prm_data(PA_DEV_t* dev, uint8_t*pToPrmData,uint8_t bPrmLength );
uint8_t PA_chk_cfg_data(PA_DEV_t* dev, uint8_t*pToCfgData,uint8_t bCfgLength );
void PA_updata_Input(PA_DEV_t* dev,uint8_t* sInput ,uint8_t len);
void PA_updata_Output(PA_DEV_t* dev,uint8_t* sOutput ,uint8_t len);

/*
 * 用户必须实现如下函数
 */
/* 存储PA块参数到非易失存储器中 */
void userBlocksNvsSave(char* pdata, uint16_t len);
/* 从非易失存储器中读取PA块参数 */
void userBlocksNvsGet(char* pdata, uint16_t len);

#endif /* PA_H_ */
