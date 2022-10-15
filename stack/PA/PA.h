/*
 * PA.h
 *
 *  Created on: 2020��7��10��
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
 * ����
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
 * GBZ 25105.1-2010 ��ҵͨ������ �ֳ����߹淶 ����10 PROFINET IO�淶 ��1���֣�Ӧ�ò������
 * 5.3.1.4 Date ����
 * 5.3.1.4.2 Date
 * CLASS:Data type
 *
 * ATTRIBUTES:
 * 1 Data type Numeric dentifier = 50
 * 2 Data type Name =Date
 * 3 Format =FIXEDLENGTH
 * 4.1 Octet Length 2һ7
 * ������������6���޷�������ֵ��Ԫ�����,��ʾ���������ں�ʱ�䡣��1��Ԫ����Unsigned16
 * ��������,���Ժ���Ϊ��λ��ʾ�˷ֵ�С�����֡���2��Ԫ����Unsigned8��������,���Է�Ϊ��λ
 * ��ʾ��Сʱ��С�����֡���3��Ԫ����Unsigned8��������,����СʱΪ��λ��ʾ�����С������,
 * ���������Чλ˵�����Ǳ�׼ʱ�仹����ʱ��ʱ��,��4��Ԫ����Unsigned8��������,����ǰ3��
 * ����ָ�����ڼ�,��5������ָ���·��е����ڡ���5��Ԫ����һ���޷�������8��������,ָ����
 * �ݡ����һ��Ԫ����Unsigned8��������,ָ����ݡ�ֵ0��50��Ӧ��2000����2050��,ֵ51��99
 * ��Ӧ��1951����1999�ꡣ
 */
#define DATEOCT6_DAYSOFWEEK_MASK   0xE0
#define DATEOCT6_DAYSOFMOUTH_MASK  0x1F

typedef struct
{
	uint16_t sub_ms;//����Ϊ��λ��ʾ�˷ֵ�С������
	uint8_t  sub_hour;//��Ϊ��λ ��ʾ��Сʱ��С������
	uint8_t  sub_day;//СʱΪ��λ��ʾ�����С������,�����Чλ˵�����Ǳ�׼ʱ�仹����ʱ��ʱ��
	uint8_t  mut_day;//ǰ3������ָ�����ڼ�,��5������ָ���·��е�����
	uint8_t  month;//�·�
	uint8_t  year;//��ݣ�ֵ0��50��Ӧ��2000����2050��
}_PACKED_ Date_t;



/*--------------------------------------------
 * 5��2��3ͨ���������ͺͽṹ
 *
 */



/*
 * 5.2.2.2 Parameter description table ����������
 * �������ÿ�����������Ԥ��ʹ�õ��������������������������塣
 * ���磬�ڱ��ж�����ö���Ͳ����ı��롣��Щ����Ҳ�����������ַ�����
 * ���һ����������ö�ٷ�Χ����������֧���������ض��Ŀ�ѡ�����Ӽ���
 */
typedef  char*  PA_TabParameterDsc_t ;

/*
 * 5.2.2.3 Parameter attribute table �������Ա�
 * �������Ա�涨����������ԡ�
 */
typedef  struct
{

	uint8_t relativeIndex; //�������(relative index)����ڱ����1������������ƫ������
	char*  pParameterName;//��������(parameter name)�ò������������ơ�ָ��������Ƴ���.
	uint8_t objectType;	   //��������(object type)
						   //����ֵ�Ķ������ͣ�������
						   //��Simple���򵥱�����
	                       //��Record�����ɸ���ͬ�ļ򵥱�����ɵĽṹ��
	                       //��Array�����ɸ��򵥱�����ɵ����顣
	uint8_t dataType;//��������(data type)
					//����ֵ���������ͣ�������
					//��Name���򵥱���������Ļ����������ͣ�
					//�� DS_n�����ݽṹ(Record)���n��
	uint8_t storeType;//�洢(store)
					//Ҫ��Ĵ洢�����ͣ����£�
					//0,��N������ʧ������Ӧ�������ϵ������ڶ������棬���������ھ�̬���´��룻
					//1,��S����̬������ʧ���޸Ĵ˲��������Ӿ�̬�汾������ST��REV��ֵ��
					//2,��D����̬����ֵ�ɿ������������һ�����ȡ��
					//3,��Cst���������ò������豸�б��ֲ��䡣
#define STRORETYPE_N    0
#define STRORETYPE_S    1
#define STRORETYPE_D    2
#define STRORETYPE_C    3
	uint8_t size;//��С(size)
					//���ݴ�С������λλ��(�ֽ�)�ơ�
	uint8_t access;//����(access)
				   //��r��ָʾ�������ɱ�����
	               //��w��ָʾ�ò����ɱ�д��
	               //ע����д�����ķ�Χ���޶��ڵ�ǰ�洢�Ĳ���ֵ�������豸����Ч��Ϊ��
#define ACCESS_R   0
#define ACCESS_W   1
#define ACCESS_RW  2

	uint8_t parameterUsage;//�����÷�(parameter usage)
				  //��C���ں�}
	              //��I�����ˣ�
	              //��O�������
#define PARAMUSAGE_C 0
#define PARAMUSAGE_I 1
#define PARAMUSAGE_O 2
	uint8_t kindOfTransport;//��������(kind of transport)(��ָʾ�����Ҫ��)
							//��a����ѭ���ģ�
							//��cyc��ѭ���ġ�
#define KINDTRANSPORT_A  0
#define KINDTRANSPORT_C  1
	uint8_t resetClass;//��λ���(reset class)
						//FACTORY��RESET(��������)Ӱ���豸�п�Ĳ�ͬ�������������ĸ�λ�������ȷ��һ
						//�������Ǵ��ڲ�����ִ��ͨ�����ź�����(�����Բ���)�����Ǹò�������������Ϣ(��Ϣ��
						//����)��
						//��F�������ԣ�
						//��I����Ϣ�ԣ�
						//�� �������á�
#define RESETCLASS_F 0
#define RESETCLASS_I 1
	void* pDefaultValue;//ȱʡֵ(default value) , ָ����Ĭ��ֵ�������������������ã�����void*
						//�ڳ�ʼ�������з����������ֵ�������һ��δ��̬�Ŀ�ĳ�ʼ���Ǳ���ġ�������ֵ����
						//�������������͡�����ڿ�����Ա��д��ڲ���ֵ�����ֵ�뱻����ȱʡֵ(�й�ȱʡֵ)��
						//����ڿ�����Ա���û�в���ֵ������ȱʡֵ���������ض���(������ȱʡֵ)��
	uint8_t downloadOrder;	//���ش���(download order)
							//�豸�д������ݵ�һ��������(���磬���ɲ���ʹ����ͬ�Ĺ��̵�λ)���޸�һ���������ܻ�
							//�����豸�ڵ�ĳЩ���㡣��ˣ��̶��Ĳ������ص��豸��˳����������ݵĲ�һ���ԡ�����
							//�Ƕ�һ�������һϵ��д���ʡ������Զ�����Ӧ��ѭ��д����˳��
							//��һЩ�����豸�У���ΪĳЩ�������໥�����Էǳ��ߣ�����б�Ҫ��װһ�������Ϊ�ˣ���
							//�Բ��á���������(parameter Transaction)���ķ���(��5��4)��
							//ע��ÿ�������ɱ�����д�롣
	uint8_t mandatoryOptional;//�ر�����ѡ(mandatory��optional)
					//��M��ָʾ�ò������ڷ�ѭ�������Ǳر��ġ�ѭ�����ʿɱ�������̬��
					//��O��ָʾ�ò����ǿ�ѡ�ġ�
}_PACKED_ PA_TabAttribute_t ;


typedef struct {
	uint8_t slot;
	uint8_t index;
	uint8_t relativeIndex;//�������
	uint8_t size;//��С(size)
	uint8_t storeTypeDownOrder;//�洢(store),//���ش���
	uint8_t APKR; //����(access) //�����÷� //��������  //��λ���
	void* pInst ; //ָ��ʵ��
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
 * 5.2.2.4 Conformance table һ���Ա�
 *  ÿ���豸�ӱ��й�涨�Ľṹ��ѡ���Ҫ���Ӽ����Ӽ���ѡ����ѭ��һ���������ж����ĳЩ��
 *  �򡣸ñ�ָ����Щ�ṹ�Ǳر���(M)����Щ��ѡ���(s)���Լ���Щ�ǿ�ѡ��(())��
 *  ����M��ָʾ���й�����(����顢���ܿ顢ת���顢Ũ��״����)�Ǳر��ģ�
 *  ����o��ָʾ���й������ǿ�ѡ�ģ�
 *  ����s��ָʾ�й��豸����֧��һ��������������ѡ��(���磺ģ�����롢ģ���������ɢ�����
 *  �ǹ��ܿ��������)����������ѡ������������ر������Ϳ�ѡ������ѡ��ر�������������
 *  ��ָ�豸Ӧ֧��һ���������г�����������ѡ���ѡ��������������ָ����豸֧�ָ���
 *  ��������豸Ӧ֧��һ���������г�����������
 */
typedef  uint8_t  PA_TabConformance_t ;

/*
 * 5.2.2.5 View object table ��ͼ�����
 * ��ͼ��������ʹ��һ����������������д���������ֵ���ṩ����������ʹ�ÿ��Լ�ʱ��Ч�ش�
 * ��һ����Ϣ��ÿ����ɴ��ڶ����ͼ����View_1��Viewһ2��Viewһ3��Viewһ4��
 */
typedef  void(* PA_TabViewobject_f)(uint8_t readWrite,void* pPararmeterList,void* pPararmeter,uint8_t num_parameters) ;



/*
 * 5��2��3��1��3ʱ��ֵ(TimeValue)��������(21)
 */
typedef  double  PA_TimeValue21_t  ;



//��֧�ָò������漰�Ĺ��ܣ����ڵ�ǰ��̬��δʹ�ò�ʹ�øò���
#define PA_ENUM_NOTUSED8  250
#define PA_ENUM_NOTUSED16 65530
//������
#define PA_ENUM_NONE8 251
#define PA_ENUM_NONE16 65531
//���ܱ���ֵ������δ��ʼ��
#define PA_ENUM_UNKNOWN8 252
#define PA_ENUM_UNKNOWN16 65532
//������Ϣ�Ǳ�Ҫ��\���õ�
#define PA_ENUM_SPECIAL8 253
#define PA_ENUM_SPECIAL16 65533

/*
 * 5��2��3��2��(Block)�ṹ(DSһ32)
 * �����ݽṹ�ɿ���������(��5��2��5)��
 * �������� Block(DS-32)
 * ���� Number of Elements=12
 * ���� List of Elements(����4)
 */

#define     Block_Object_Physical       ((uint8_t)0x01)  // 01 Physical Block,
#define 	Block_Object_Function       ((uint8_t)0x02)  // 02 Function Block
#define 	Block_Object_Transducer     ((uint8_t)0x03)  // 03 Transducer Block

#define 	ParentClass_PB_Transmitter      ((uint8_t)1)     //������
#define 	ParentClass_PB_Actuator         ((uint8_t)2)     //ִ����
#define 	ParentClass_PB_DiscreteIO       ((uint8_t)3)     //��ɢI��O
#define 	ParentClass_PB_Controller       ((uint8_t)4)     //������
#define 	ParentClass_PB_Analyzer         ((uint8_t)5)     //��������
#define 	ParentClass_PB_LabDevice        ((uint8_t)6)     //ʵ�����豸
#define 	ParentClass_PB_MultiVariable    ((uint8_t)127)   //�����

#define 	ParentClass_FB_Input            ((uint8_t)1)     //����
#define 	ParentClass_FB_Output           ((uint8_t)2)     //���
#define 	ParentClass_FB_Control          ((uint8_t)3)     //����
#define 	ParentClass_FB_AdvancedControl  ((uint8_t)4)     //�Ƚ�����
#define 	ParentClass_FB_Calculation      ((uint8_t)5)     //����
#define 	ParentClass_FB_Auxiliary        ((uint8_t)6)     //����
#define 	ParentClass_FB_Alert            ((uint8_t)7)     //����

#define 	ParentClass_TB_Pressure 		((uint8_t)1)     //ѹ��
#define 	ParentClass_TB_Temperature 		((uint8_t)2)     //�¶�
#define 	ParentClass_TB_Flow 			((uint8_t)3)     //����
#define 	ParentClass_TB_Level 			((uint8_t)4)     //��λ
#define 	ParentClass_TB_Actuator 		((uint8_t)5)     //ִ����
#define 	ParentClass_TB_DiscreteIO 		((uint8_t)6)     //��ɢI��O
#define 	ParentClass_TB_Analyzer 		((uint8_t)7)     //��������
#define 	ParentClass_TB_Auxiliary 		((uint8_t)8)     //��������
#define 	ParentClass_TB_Alarm 			((uint8_t)9)     //����
#define 	ParentClass_TB_reserved 		((uint8_t)10)    //����

//Input
#define 	Class_FB_AnalogInput   			((uint8_t)1)     //ģ������
#define 	Class_FB_DiscreteInput 			((uint8_t)2)     //��ɢ����
//Output
#define 	Class_FB_AnalogOutput   		((uint8_t)1)     //ģ�����
#define 	Class_FB_DiscreteOutput 		((uint8_t)2)     //��ɢ���
//Control
#define 	Class_FB_PID 					((uint8_t)1)     //PID
#define 	Class_FB_SampleSelector 		((uint8_t)2)     //����ѡ����
#define 	Class_FB_LabControlUnit 		((uint8_t)3)     //ʵ���ҿ��Ƶ�Ԫ
#define 	Class_FB_Thermostate 			((uint8_t)4)     //�¶ȵ�����
#define 	Class_FB_Stirrer 				((uint8_t)5)     //������
#define 	Class_FB_Shaker 				((uint8_t)6)     //�����
#define 	Class_FB_BalanceScale 			((uint8_t)7)     //��ƽ����
#define 	Class_FB_Centrifuge 			((uint8_t)8)     //���Ļ�
#define 	Class_FB_DosingPump 			((uint8_t)9)     //������
//Advanced Control
#define 	Class_FB_LabInstruments 		((uint8_t)1)     //ʵ��������
//Calculation
#define 	Class_FB_Totalizer 				((uint8_t)8)     //�ۼ���
//Auxiliary
#define 	Class_FB_Ramp 					((uint8_t)1)     //б��
#define 	Class_FB_BMLogbook 				((uint8_t)2)     //BM��־
#define 	Class_FB_Sample 				((uint8_t)3)     //����
//Alert

//Pressure
#define 	Class_TB_Differential 			((uint8_t)1)     //��ѹ
#define 	Class_TB_Absolute 				((uint8_t)2)     //����ѹ��
#define 	Class_TB_Gage 					((uint8_t)3)     //��ѹ��
#define 	Class_TB_PressureLevelFlow		((uint8_t)4)     //ѹ��+��λ+����
#define 	Class_TB_PressureLevel			((uint8_t)5)     //ѹ��+��λ
#define 	Class_TB_PressureFlow  			((uint8_t)6)     //ѹ��+����
#define 	Class_TB_MixedAbsoluteDiffPressure  ((uint8_t)7)     //��Ͼ���ѹ������ѹ
//Temperature
#define 	Class_TB_TC 					((uint8_t)1)     //�ȵ�ż(TC)
#define 	Class_TB_RTD  					((uint8_t)2)     //�ȵ���(RTD)
#define 	Class_TB_Pyrometer 				((uint8_t)3)     //���¼�
#define 	Class_TB_TCDCU					((uint8_t)16)     //TC+DC u(ֱ����ѹ)
#define 	Class_TB_RTDR 					((uint8_t)17)     //RTD+R(R����)
#define 	Class_TB_TCRTDRDCU  			((uint8_t)18)     //TC+RTD+R+DCU
//Flow
#define 	Class_TB_Electromagnetic 		((uint8_t)1)     //���
#define 	Class_TB_Vortex 				((uint8_t)2)     //�н�
#define 	Class_TB_Coriolis 				((uint8_t)3)     //����������
#define 	Class_TB_ThermalMass 			((uint8_t)4)     //��ʽ����
#define 	Class_TB_Ultrasonic 			((uint8_t)5)     //������
#define 	Class_TB_VariableArea 			((uint8_t)6)     //�ɱ����
#define 	Class_TB_DifferentialPressure 	((uint8_t)7)     //��ѹ
//Level
#define 	Class_TB_Hydrostatic 			((uint8_t)1)     //���徲ѹ
#define 	Class_TB_EchoLevel 				((uint8_t)2)     //�ز�Һλ
#define 	Class_TB_Radiometric 			((uint8_t)3)     //����ʽ
#define 	Class_TB_Capacity 				((uint8_t)4)     //����ʽ
//Actuator
#define 	Class_TB_Electric 				((uint8_t)1)     //�綯
#define 	Class_TB_ElectroPneumatic 		((uint8_t)2)     //������
#define 	Class_TB_ElectroHydraulic 		((uint8_t)3)     //��Һ��
//Discrete I/O
#define 	Class_TB_SensorInput  			((uint8_t)1)     //����������
#define 	Class_TB_Actuator 				((uint8_t)2)     //ִ����
//Analyzer
#define 	Class_TB_Standard 				((uint8_t)1)     //��׼
//Auxiliary
#define 	Class_TB_Transfer 				((uint8_t)1)     //����
#define 	Class_TB_Control 				((uint8_t)2)     //����
#define 	Class_TB_Limit 					((uint8_t)3)     //��ֵ
//Alarm
#define 	Class_TB_BinaryMessage 			((uint8_t)1)     //��������Ϣ


//Profile OctetString
#define 	Profile_Code_HiByte 			((uint8_t)0x40)     //Bytel(MSB),��PI�й�������PROFIBUS PA�й�ı��Ϊ64����0x40
#define 	Profile_Code_LoByteLClassA 		((uint8_t)0x01)     //Byte2(LSB), ��߱��ز���λ��ζ�����б�׼�����������Լ� �Ĵ洢λ��
#define 	Profile_Code_LoByteLClassB 		((uint8_t)0x02)     //Byte2(LSB), ��߱��ز���λ��ζ�����б�׼�����������Լ� �Ĵ洢λ��
#define 	Profile_Code_LoByteHClassA 		((uint8_t)0x81)     //Byte2(LSB), ��߱�����λ��ζ�Ž���׼����ST��REV��TAGDESC��STRATEGY��ALERT��KEYӳ����һ���洢λ����
#define 	Profile_Code_LoByteHClassB 		((uint8_t)0x82)     //Byte2(LSB), ��߱�����λ��ζ�Ž���׼����ST��REV��TAGDESC��STRATEGY��ALERT��KEYӳ����һ���洢λ����
#define 	Profile_Code_LoByteSpecial 		((uint8_t)253)      //Byte2(LSB), �������ض��Ŀ�ṹ

#define MAKE_PROFILECODE(hi,lo) 			((hi << 8) + lo)

typedef  struct
{
	uint8_t Reserved;   		//����ȱʡֵ=250(δʹ��)
	uint8_t Block_Object;		//�����
	uint8_t Parent_Class;		//����
	uint8_t Class;       		//����
	uint16_t Dev_Rev ;          //��5��5��4��2��1�������豸�����п鶼��ͬ
	uint16_t Dev_Rev_Comp;		//��5��5��4��2��1�������豸�����п鶼��ͬ
	uint16_t DD_Revision;		//����ʹ��
	uint16_t Profile;           //�������9 Profile OctetString
	uint16_t Profile_Revision;  //�������£�
								//Bytel(MSB)��ʮ����С����֮ǰ�����֣���ΧΪ00��99
								//Byte2(LSB)��ʮ����С����֮������֣���ΧΪo��255
								//MSB��ʾNAMUR NE53�ĵ�1�����֣�I��SB��ʾNAMUR NE53�ĵ�2���͵�3��
								//���֣�
								//��������й�3��02��MsB��Ox03��LSB=0x02
	uint8_t Execution_Time;		//������ʹ��
	uint16_t Number_of_Parameters;//�����õ��������(����)�ĸ�����������
									//�����бر������ڵ��ʸ�(gap)��
									//������ѡ������
									//��������������
									//һ�������ض�������
									//�����������ض������ڵļ��(gap)��
									//Number_ofParameters��������ͼ����
	uint16_t Address_of_View_1;		//���ڷ���View_l����(��5��2��1��1)�����á��˲���ֵ�ĺ�����ͨ���ض��ģ����ڱ��й�ӳ�䲿�ֶ���
	uint8_t Number_of_Views;		//������д��ڳ�Viewһ1��ͼ���������������ͼ������˲��������ÿ���������ͼ����ĸ���������Viewһ1�ͱ�������ͼ����
}_PACKED_ PA_BlockDS32_t ;

/*
 * 5��2��3��3ֵ&״��һ����(Value&Status-Floating Point)�ṹ(101)
 */
typedef struct
{
	float Value;
	uint8_t Status;
}_PACKED_ PA_ValueStatusFP101_t;

/*
 * 5��2��3��4ֵ��״��һ��ɢ(Value&Status-Discrete)�ṹ(102)
 */
typedef struct
{
	uint8_t Value;
	uint8_t Status;
}_PACKED_ PA_ValueStatusDS102_t;

/*
 * 5��2��3��5����(Scaling)�ṹ(DSһ36)
 */
typedef struct
{
	float Eu_at_100;
	float Eu_at_0;
	uint16_t units_Index;//Units��Index�Ĵ����5��3��
	int8_t  DecimalPoint;//Decimal��Point�Ǳ�ע��˵��С�����λ������Ч������������վ���ߺͱ�����ʾ��
}_PACKED_ PA_ScalingDS36_t;

/*
 * 5��2��3��6ģʽ(Mode)�ṹ(DSһ37)
 */
typedef struct
{
	uint8_t actual;
	uint8_t permitted;
	uint8_t normal;
}_PACKED_ PA_ModeDS37_t;

/*
 * 5��2��3��7��������(Alarm Float)�ṹ(DS-39)
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
 * 5��2��3��8��������(Alarm Summary)�ṹ(DS-42)
 */
#define PA_ALARMSUMB0_Discrete_Alarm  0x01 //��������ɢ��ֵ�����Ĺ��ܿ�
#define PA_ALARMSUMB0_HI_HI_Alarm     0x02 //������ģ����ֵ�����Ĺ��ܿ�
#define PA_ALARMSUMB0_HI_Alarm        0x04 //������ģ����ֵ�����Ĺ��ܿ�
#define PA_ALARMSUMB0_LO_LO_Alarm     0x08 //������ģ����ֵ�����Ĺ��ܿ�
#define PA_ALARMSUMB0_LO_Alarm        0x10 //������ģ����ֵ�����Ĺ��ܿ�
#define PA_ALARMSUMB0_UpdateEvent     0x80 //���д洢����S�Ŀ�������޸�

#define PA_ALARMSUMB1_BIT_OFFSET          (0x08) //16λ�ĸ��ֽ�

typedef struct
{
	uint8_t current[2]; //�������ԭ�����(1)����ʧ(o)������ֵ����(Limit alarm)������Ϊ1��0�����κ�
						//���д洢����S�Ŀ�������޸�֮��Ӧ�������¼�(update event)������Ϊ1��������
						//20 S��(Ũ��״����)��10 S��(����״����)���ñ�����Ϊ0��
						//ע��ĳЩ����ԭ��ӳ�䵽ѭ��״������
	uint8_t unacknowledged[2];
	uint8_t unreported[2];
	uint8_t disabled[2];
}_PACKED_ PA_AlarmSummaryDS42_t;

/*
 * 5��2��3��9���ܿ�����(FB Linkage)�ṹ(DS-49)
 * ����ϸ�ڼ�5��2��9��
 */
typedef struct
{
	uint16_t Local_Index;//�涨���������FB_ID�Ͳ������ƫ��������������������ӵ�Remote��Index��ʶ�����˲��������Ǹ����ӵ�Դ
	uint16_t Connection_Number;//����Ϊ0���Ա�ʶ�����豸�ڵ�һ������
	uint16_t Remote_Index;//�涨���������FB ID�Ͳ������ƫ�����������˲��������ӵ�Local��Index��ʶ�Ĳ���(���Ա���������Ч)�����Ǹ����ӵ�Ŀ��
	uint8_t Service_Operation;//�������ӵĶ�����
							//���룺
							//0�� �޷��񡪡���������Ч�ģ�
							//1�� ���ء����豸�ڵ����ӣ�
							//2��127�� ������
							//128��255���������ض�
	uint8_t Stale_Count_Limit;//����Ϊ0������ҪʱЧ�Լ��
}_PACKED_ PA_FBLinkageDS49_t;

/*
 * 5��2��3��10����һ����(Simulation��Floating Point)�ṹ(Ds_50)
 */
typedef struct
{
	uint8_t Simulate_Status;
	float Simulate_Value;
	uint8_t Simulate_Enabled;
}_PACKED_ PA_SimuFloatPtDS50_t;


/*
 * 5��2��3��11����һ��ɢ(Simulation-Discrete)�ṹ(DSһ51)
 */
typedef struct
{
	uint8_t Simulate_Status;
	uint8_t Simulate_Value;
	uint8_t Simulate_Enabled;
}_PACKED_ PA_SimuDiscreteDS51_t;

/*
 * 5��2��3��12���(Result)�ṹ(DSһ60)
 */
typedef struct
{
	float PVFloat;//����ת����Ľ����ֵ������ͬת�����У��������ʹ�ֵ�İ������
	uint8_t Measurement_Status ;//ֵ����ʱ�����״̬(��5��3��2��5��3 4)
	Date_t  PV_Time;//����PV��ʱ��
}_PACKED_ PA_ResultDS60_t;

/*
 * 5��2��3��13������Χ(Measurement Range)�ṹ(Ds_61)
 */

typedef struct
{
	float Begin_of_Range;
	float End_of_Range;
}_PACKED_ PA_MeasurRangeDS61_t;

/*
 * 5��2��3��14��������Ϣ(Binary Megage)�ṹ(DSһ62)
 */

typedef struct
{
	uint16_t Status_Class;//����16��״����(status class)�����й涨����ǰ4�ࡣÿ����������Ϣ(BM)����һ�����
							//��״�����б����á�����λ�õı��(��1��ʼ)�ǶԸ�״��������á�Bitn=1�Ҵ�BM��
							//��Ч�ģ����ʾ��״������ܺͱ���(Bitl6)����Ϊ1����GLOBAL��STATUS�е����Bitn
							//����Ϊ1�������ACTIVE��BM�е�BM����Ҳ����Ϊ1(���������������ݵ���)
	Boolean Logbook_Entry;//��������Ϣ����ʱ�ʹ��ɱ�һ�𱣴�����־(Logbeok)FB�С��˲������û���ý�BM�洢����־�С�
							//���룺
							//False ���洢��Logbook��
							//True �洢��Logbook��
	uint8_t Output_Reference;//ÿ��BMֻ����һ����ɢ���(DO)��ء�OUTPUT��REFERENCEֵ���豸�������ӵ�DO�ı��
	uint8_t Supervision;//������˲����л����򿪼��ӣ����BM����Ϊ��Ч�ġ��رռ��ӿ�ѡ��BM��Ϊ��Ч��
						//����Ч�ģ���BM��������Ϣ�޹ء�
						//���룺
						//0 �رռ��ӣ���Ϣ����Ч��
						//1 �رռ��ӣ���Ϣ����Ч��
						//2 �򿪼���
	char Text_VisibleString[16];//�˲�������ASCII�ı����ն�վ�����վ��ʹ�ø��ı��������������Ϣ�ṩ���ͺ͸�����
								//Ϣ�����ڽ���ϵͳ����ʹ���豸�������������Դ˲����ǿ�ѡ��
}_PACKED_ PA_BinaryMegageDS62_t;

/*
 * 5��2��3��15����ѡ��(Sample Selection)�ṹ(DS-63)
 */
typedef struct
{
	uint16_t Channel;//����������ṩ����ֵ��ת���������
	uint32_t Active_Sample_Time ; //���豸��ִ�иò���������ʱ��TimeDifrerance,�Ժ���Ϊ��λ
}_PACKED_ PA_SampleSelectionDS63_t;

/*
 * 5��2��3��16 ��־(Logbook)�ṹ(DSһ64)
 */
typedef struct
{
	uint8_t Type; //���룺
					//0�� GlobaL Status
					//1��16����n��״����Ϣ
					//255�� Binary Message
	uint16_t Value; //��ֵ�Ľ���ȡ����Type�����ݣ�
			//Type=0 )ֵ=Global_Status
			//Type--l��16 )ֵһһ�������״̬���߼���������Ľ��
			//Type=255 )ֵһ��������Ϣ�ı��
	Boolean Active;//���룺
					//True BM��Ϊ��Ч��
					//False BM��Ϊ��Ч��
	Date_t Time;
}_PACKED_ PA_SampleSelectionDS64_t;

/*
 * 5��2��3��17Ԥ����(Precalculation)�ṹ(DS-65)
 */
typedef struct
{
	uint8_t Function_Type;//�˲���������ʹ�õĹ������͵�ѡ����Щ����������Ԥ�������б����
							//���룻
							//0�� ��Ԥ���㹦��
							//1�� �˲�
							//2�� ƽ��ֵ
							//3�� ����
							//4�� У��
							//5��127�� ����
							//128��255�� �豸�ض�
	uint8_t Subtype;//����������ض������������Ԥ���㹦�ܣ����˲���ƽ��ֵ�����ֻ�У����ȱʡֵ1��ʾ����
					//���ı�׼�������豸ʹ���ֲ�������ض��㷨��������
					//���룺
					//0����Ԥ����
					//1���豸�ض��ı�׼�㷨
					//2��255���豸�ض�
	uint8_t Choice;//�˲���ѡ��У��������δ������ģ�����ʹ��һ���̶�ֵ��������Ľ����
					//���룺
					//0�� ����δ������
					//1�� ����ʹ��Ԥ�������Ľ��
					//2�� ����ʹ��һ���̶�ֵ
					//3�� ����ʹ��һ�����ܿ��ֵ
					//4�� ����ʹ��һ��ת�����ֵ
}_PACKED_ PA_PrecalculationDS65_t;

/*
 * 5��2��3��18˳�����(Sequential Contr01)�ṹ(DS-66)
 */
typedef struct
{
	Date_t Time; //������ؿ�ĵ�1�����¸�ִ��ʱ�䡣�˲����ɾ���һ��ѭ��ִ�����ڵĿ�ʼ
	uint32_t Cycle_Time;//������ؿ���Զ�ִ�е�ʱ������Cycle_TimeֵΪ0��ʾһ����ѭ��ִ��, TimeDifference,��λms
	uint16_t Command;//�˲�������Ӱ����ؿ������Ĵ��롣
						//����ת���黹����һ��COMMAND�������ÿ���������ȴ˽ṹ������������и������ȼ���
						//���룺
						//5�� ��ʼ(Start)
						//6�� ֹͣ(Stop)
						//7�� ����(���¿�ʼ)
						//8�� ȡ��(Cancel)
						//9�� ���豸�ڲ��¼������Ŀ�ʼ
						//10��127�� ����
						//128��255�� �������ض�
	Boolean Time_Control_Active;//�˲��������������Ǳ��Զ�ִ�У����Ǹ������޽����
								//���룺
								//False�� ִֹͣ��
								//True�� ִ��
}_PACKED_ PA_SequeContr01DS66_t;

/*
 * 5��2��3��19��(Batch)�ṹ(DSһ67)
 */
typedef struct
{
	uint32_t Batch_ID;//��ʶĳ��������������������������豸�йص���Ϣ(���磺���ϡ�������)��
	uint16_t Rup;//�䷽��Ԫ���(Recipe Unit Procedure)��Ԫ(Unit)湱�ţ���ʶ��Ч�Ŀ����䷽��Ԫ���
				//(ControlRecipe Unit Procedure)����ص�Ԫ(���磺��Ӧ�������Ļ��������)��(un��t��
				//GB��T 1 9892��1 2005�ж��壮������Ϊ����UNIT���в�ͬ�ĺ��壬�����̵�λ��)
	uint16_t Operation;//�䷽����(Recipe Operation)�ı�ţ���ʶ��Ч�Ŀ����䷽����(Control Recipe Operation)
	uint16_t Phase;//�䷽�׶�(Recipe Phase)�ı�ţ���ʶ��Ч�Ŀ����䷽�׶�(Control Recipe Phase)
}_PACKED_ PA_BatchDS67_t;


/*
 * 5��2��3��20����(Feature)�ṹ(DS 68)
 */
//��41��֧������(Supported)�ı��� ��λλ��1
#define FeatureSupportB0_CondensedStatus   0x01  //���������豸����״������ϵ�һ�㷽����
										         //0�� ��֧��Ũ��״�������
											     //1�� Ũ��״���������Ϣ����5��3��4�еĶ���
#define FeatureSupportB1_ClassicStatusDiagnosis   0x02 //���������豸����״������ϵ�һ�㷽����
												//0�� ��֧��5��3��2�ж���ľ���״�������
												//1�� ֧��5��3��2�еĶ���
#define FeatureSupportB2_DxB   0x04 //0�� ��֧�ֹ㲥���ݽ���
									//1�� ֧�ֹ㲥���ݽ���
#define FeatureSupportB3_MS1AR   0x08 //0: ��֧��MSlӦ�ù�ϵ
									//1: ֧��MSlӦ�ù�ϵ
#define FeatureSupportB4_PROFISAFE   0x10 //0�� ��֧��PROFIsafeͨ��
											//1�� ֧��PROFlsafeͨ��
//��42����������(Enabled)�ı���
#define FeatureEnabledB0_CondensedStatus   0x01  //���������豸����״������ϵ�һ�㷽����
										         //0�� ��֧��Ũ��״�������
											     //1�� Ũ��״���������Ϣ����5��3��4�еĶ���
#define FeatureEnabledB1_ClassicStatusDiagnosis   0x02 //���������豸����״������ϵ�һ�㷽����
												//0�� ��֧��5��3��2�ж���ľ���״�������
												//1�� ֧��5��3��2�еĶ���
#define FeatureEnabledB2_DxB   0x04 //0�� ��֧�ֹ㲥���ݽ���
									//1�� ֧�ֹ㲥���ݽ���
#define FeatureEnabledB3_MS1AR   0x08 //0: ��֧��MSlӦ�ù�ϵ
									//1: ֧��MSlӦ�ù�ϵ
#define FeatureEnabledB4_PROFISAFE   0x10 //0�� ��֧��PROFIsafeͨ��
											//1�� ֧��PROFlsafeͨ��
typedef struct
{
	uint8_t Supported[4];
	uint8_t Enabled[4];
}_PACKED_ PA_FeatureDS68_t;

/*
 * 5��2��3��21 ����¼�ת��(Diag_Event_Switch)�ṹ
 */
#define  DiagStaLK_GOODOK           	0x00 //�ã�ok
#define  DiagStaLK_GOODMaintenReq   	0x01 //�ã���Ҫά��
#define  DiagStaLK_GOODMaintenDem   	0x02 //�ã�����ά��
#define  DiagStaLK_UNCTMaintenDem   	0x03 //��ȷ��������ά������
#define  DiagStaLK_BADMaintenAlarm  	0x04 //����ά������������
#define  DiagStaLK_UNCTProcNoMaint  	0x05 //��ȷ��������أ���ά��,��Ч�Ĺ�������
#define  DiagStaLK_BADProcNoMaint  		0x06 //��������أ���ά��,��Ч�Ĺ�������
#define  DiagStaLK_BADFuncChecLocalOv   0x07 //��һ���ܼ�飯���س���,���ܼ��
#define  DiagStaLK_GOODFuncChec         0x08 //�ù��ܼ��,���ܼ��

#define  DiagStaLK_DIA_MAINTENANCE 			    0x10 //�������
#define  DiagStaLK_DIA_MAINTENANCE_DEMANDED 	0x20 //�����������
#define  DiagStaLK_DIA_MAINTENANCE_ALARM 		0x30 //����
#define  DiagStaLK_DIA_INV_PRO_COND 			0x40 //��Ч�Ĺ�������
#define  DiagStaLK_DIA_FUNCTION_CHECK 			0x50 //���ܼ��

typedef struct
{
	uint8_t Diag_Status_Link[48]; //�����豸�ض�����¼���ת�����顣ӳ�䵽��ϱ��غ�״�����롣ÿ������¼���Ӧһ����
	//�ڡ����һ������¼���������˲���ָ��״������ϱ����Ӱ�졣
	//�ڶ�����豸���ͨ���豸������£���(ͨ��)��û���õġ�������Ϊ�ڽ�Ӱ��һ��ͨ���Ĵ�
	//�������ϵ������(���磬�¶ȴ�������������������������������һ������)������FB������
	//ͬ��ʽ������Ӧ��������Ӧ��Ӱ�����ͨ��(�������̹涨��)��״����
	//״������(����λ)��
	//0������¼��Ը�״����Ӱ�졣
	//StatusӦ��GooD-ok
	//1�� ����¼�����Ϊά������������
	//StatusӦ��GooD-maintenance required��
	//2������¼�����Ϊ����ά������������
	//StatusӦ��GooD-maintenance demanded��
	//3������¼�����Ϊ����ά������������
	//StatusӦ��UNCERTAIN��maintenance demanded��
	//4������¼�����Ϊ����������
	//StatusӦ��BADmaintenance alarm��
	//5�� ����¼�����Ϊ��Ч����������������ֵ��������ʹ�õġ�
	//StatusӦ��UNCERTAlN��process related��no maintenance��
	//6������¼�����Ϊ��Ч����������������ֵ�����á�
	//StatusӦ��BAD-process related��no maintenance��
	//7�� ����¼�����Ϊ�޿���ֵ�Ĺ��ܼ��������
	//StatusӦ��BAD-function check��local override
	//8�� ����¼�����Ϊ�п���ֵ�Ĺ��ܼ��������
	//StatusӦ��GOOD-function check��
	//9��15������
	//���(����λ)��
	//0�� ����¼��Ը������Ӱ�졣
	//DIAGNOSIS����������������λ��
	//l������¼�����Ϊά������������
	//D1AGNOSlS��Ӧ��DIA_MAINTENANCE��λ��
	//2������¼�����Ϊ����ά������������
	//DIAGNOSIS��Ӧ�� DIA_MAINTENANCE_DEMAND��λ��
	//3������¼�����Ϊ����������
	//D1AGNOSIS��Ӧ��DIA_MAINTENANCE_ALARM��λ��
	//4������¼�����Ϊ��Ч��������������
	//DIAGN()SIS��Ӧ��DIA_INV_PRO_COND��λ��
	//5�� ����¼�����Ϊ���ܼ������������
	//DIAGNOSIS��Ӧ��DIA_FUNCTION_CHECK��λ��
	//6��15������
	//����豸����Ҫ���ӵ�����¼�С��48������ʣ���D1AG_STATUs_LINK�ֽ�ȫ����Ϊ0��
	//ÿ������¼���֧�ֵ�ö�ٿ������������޶�
	uint8_t Slot; //����Diag_Event Switch�Ĳۡ�ָ����һ��Diag_Event_Switch�ṹ��
	//�����Diag_Event Switches�޺�������Ԫ��Slot��ֵΪ0��ͬʱԪ��IndexֵΪ0
	uint8_t Index_absolute;//����Diag_Event_Switch��(����)������ָ����һ��Diag_Event_Switch�ṹ��
	//�����Diag��Event��Switches�޺�������Ԫ��SLotֵΪ0��ͬʱԪ��IndexֵΪ0
}_PACKED_ PA_DiagEventSwitch_t;



/*
 * -----------------------------------------
 * 5��2��4��׼��������
 */
#define PA_DEVIECEMODE_OS    0x80  //Bit7�� �Ƿ���(Out of Service (O/S) - MSB
#define PA_DEVIECEMODE_IMAN  0x40  //Bit6�� �ֶ���ʼ��(Initialisation Manually��IMan)(��A���B���в�ʹ��)
#define PA_DEVIECEMODE_LO    0x20  //Bit5�� ���س���(Local Override��LO)(��A���в�ʹ��)
#define PA_DEVIECEMODE_MAN   0x10  //Bit4�� �ֶ�(Manual��MAN)
#define PA_DEVIECEMODE_AUT   0x08  //Bit3�� �Զ�(Automatic��AUT())
#define PA_DEVIECEMODE_CAS   0x04  //Bit2�� ����(Cascade��Cas)(��A���B���в�ʹ��)
#define PA_DEVIECEMODE_RCAS  0x02  //Bitl�� Զ�̼���(Remote-Cascade��RCas)
#define PA_DEVIECEMODE_ROUT  0x01  //Bit0�� Զ�����(Remote-Output��ROut)����LSB(��A���B���в�ʹ��)

typedef struct {
	__Def_ParameterAttr(PA_BlockDS32_t* ,pBLOCK_OBJECT); //BLOCK_OBJECT������ÿ����ĵ�1���������������ÿ�����ԣ����磬�����ͺ��й��

	__Def_ParameterAttr(uint16_t* ,		pST_REV) ;      //����в��������޸ĵľ�̬�����������̬���Ż��ڼ�����Щ������ֵ���������������
									//һ����̬�������޸ģ���Ӧ������Ӧ��ST_REV(���ٰ�l����)������ṩ�˶Բ����汾
									//�ļ�顣���������������(������FACTORY��RESET=1)����ST_REVӦ����λΪ0��
									//���ٰ�1���ӣ���ָʾ��̬�����ĸı䡣���⣬���������һ������޸ģ���ST_REVӦ��
									//���ӡ�ST_REV��ֵ�ɱ���̬�豸�����ж��洢�ھ�̬�洢���еĿ����(�ڲ������Ա�
									//�о���"S"���Զ���)�Ƿ���ֵ�ȱ��ı䡣
									//��ST_REV����������Ӧ������Ϊl
	__Def_ParameterAttr(char* ,			pTAG_DESC);//�ñ�ǩ�������û��ṩ�Ŀ��������ɸ�ÿ�������һ���ı�����ǩ����
	__Def_ParameterAttr(uint16_t* ,		pSTRATEGY);//STRATEGY���������û��ض���ֵ���˷���ֵ������̬������������������Ϣ�Ĺؼ���
	__Def_ParameterAttr(uint8_t* ,		pALERT_KEY);//ALERT_KEY���������û������ֵ����ֵ�����ڶ��ɿ�����ı������¼����з��ࡣ��
								//�ɰ���װ�õ�Ԫ�ı�ʶ�ţ��԰���ʶ���¼���װ�õ�Ԫ�е�λ��
	__Def_ParameterAttr(uint8_t* ,		pTARGET_MODE);//TARGET_MODE����ָ���Ըÿ��������Ĳ���ģʽ����ͨ���ɿ���Ӧ�û��ɲ���Աͨ��
								//�˻��ӿ�Ӧ�������á������˲������״̬�Ľ�������ж��ÿ��ܷ�ʵ��Ҫ���Ŀ��ģʽ��
								//��MODE_BLK������PermittedԪ���������ģʽ�У�ֻ����������һ��ģʽ���Դ˲�
								//�����ж���һ��ģʽ��д���ʳ����˸ò����ķ�Χ���������Ծܾ���
								//���룺
								//Bit7�� �Ƿ���(Out of Service (O/S) - MSB
								//Bit6�� �ֶ���ʼ��(Initialisation Manually��IMan)(��A���B���в�ʹ��)
								//Bit5�� ���س���(Local Override��LO)(��A���в�ʹ��)
								//Bit4�� �ֶ�(Manual��MAN)
								//Bit3�� �Զ�(Automatic��AUTO)
								//Bit2�� ����(Cascade��Cas)(��A���B���в�ʹ��)
								//Bitl�� Զ�̼���(Remote-Cascade��RCas)
								//Bit0�� Զ�����(Remote-Output��ROut)����LSB(��A���B���в�ʹ��)
								//�ڱ��й���ʹ�õġ�automatic��ģʽ����Auto��Rcas����manual��ģʽ����LO��Man���� O/Sģʽ�£�����ִ�������㷨
	__Def_ParameterAttr(PA_ModeDS37_t*, 	pMODE_BLK);//MODE_BLK������һ���ṹ����������ʵ��ģʽ(actual mode)������ģʽ(normal mode)
							//	������ģʽ(permitted mode)��ɡ�ʵ��ģʽ�ɸÿ�����ִ���ڼ��������(ͨ������)��
							//	�Է�ӳ����ִ���ڼ������õ�ģʽ������ģʽ�ǶԸÿ��������Ĳ���ģʽ��
							//	����ģʽ��MODE_BLK������Զ���û�ָ��Ŀ��ģʽ�����ָı��Ƕ��ض�����Ч�ġ�
							//	ģʽ�Թ��ܿ�����������ܽ����£�
							//	����Out of Service(O��s)��
							//	��ת���飺��o��sģʽ�£�ֹͣ�Բ���ֵ�����ֵ�ĸ�ֵ��
							//	�����ܿ飺��o��sģʽ�£�ֹͣ�Բ���ֵ�����ֵ�ĸ�ֵ��
							//	������飺ֹͣ��DIAGNOSIS��DIAGNOSIS EXTENSION�������ݸ��Ƶ�PROFIBUS DP��Slave_Diag����
							//	��ѭ�������ķ��ʲ���Ӱ�졣
							//	���˹��ܿ�����ֵӦ����Ϊ��һ��ֵ������������ܿ飬Ϊת�����ṩ��Ϣ�����
							//	����Ӧ����Ϊ����ʱ���涨��ֵ��������ϰ�ȫ����Ķ����޹ء�
							//	����Local Override(LO)��������֧�ָ������˲����Ŀ��ƿ������顣ͬ���������̿���
							//	�����豸�ı�����������������LOģʽ��������ģʽ�£������������Ϊ��������
							//	������ֵ��Ӧ��ʼ���㷨(��ģ���豸)��ʹ���ڴ�LOģʽ�л���Ŀ��ģʽʱ���ᾭ
							//	��ͻ�䡣
							//	����Manual(MAN)�����ܿ������ƿ��������������������������ɲ���Աͨ���ӿ���
							//	��ֱ�����á�Ӧ��ʼ���㷨��ʹ����ģʽ�л�ʱ���ᾭ��ͻ�䡣
							//	����Automatic(AUTO)�������������������빦�ܿ飬ʹ������ת��������˽��м��㣻
							//	����������ܿ飬ʹ���������ṩ�����Աͨ���ӿ��豸�ṩ���趨ֵ���м��㣻��
							//	��������ת���飬��ģʽ��ʾ��鹦���ܹ���������
							//����Remote Cascade(RCas)������趨ֵ�ɿ���Ӧ��ͨ��Զ�̼�������RCAS��IN�����á�
							//���ݸ��趨ֵ���������㷨���������ֵ��
							//���ܿ顢������ת�������Ϊͨ��MODE_BLK������ָʾ��MODE_BLKԪ�صı���
							//ͬTARGET_MODE�����������£�
							//1��ʵ��ģʽ(Actual)�����ǿ�ĵ�ǰģʽ���ɸ�������������ͬ��TARGET MODE����ֵ
							//�ļ����ǿ�ִ�е�һ���֡�
							//�����ܿ鴦����ֹ��������Ŀ��ģʽ��������ʱ�����ܿ��ʵ��ģʽ���Զ��ı䡣������
							//���������ʵ��ģʽ�ļ��㣺
							//��TARGET_MODE������
							//���ÿ��ģʽ���㡣
							//������㲻ͬ��TARGET_MODE��ʵ��ģʽʱ��Ӧʹ�����ȼ����ģʽ���ȼ�������
							//�£�����0����������ȼ���
							//��O��S�����ȼ�Ϊ7����ߣ�
							//��IMan�����ȼ�Ϊ6(δʹ��)��
							//��LO�����ȼ�Ϊ5}
							//��MAN�����ȼ�Ϊ4��
							//��AUT0�����ȼ�Ϊ3��
							//��Cas�����ȼ�Ϊ2(δʹ��)��
							//��RCas�����ȼ�Ϊ1��
							//��ROut�����ȼ�Ϊ0�����(δʹ��)��
							//����MODE����ϸ�ڵ�״̬���ǿ������ض���(�����ݵ�)��
							//2������ģʽ(Permitted)�������ʵ���������ģʽ������ģʽ�ɿ�������Ա������̬��
							//������Ӧ�����ݵ��ж�ÿ������ж��塣�豸Ӧ����κ�ģʽ�ı��������ȷ��������
							//��Ŀ��ģʽ������ģʽ���ж��塣
							//3������ģʽ(Normal)���������������������¿�ʹ�õ�ģʽ���˲������ɽӿ��豸����
							//ȡ�����������㷨ʹ�á�����ģʽ���ڱ��й淶Χ��ʹ�ã�������ʹ�á�
							//A���豸�Ŀ������ṩ��AUTO��ģʽ��Ϊ�ر�ģʽ��ģʽ���������B���豸�Ĺ��ܿ�
							//�Ǳر���
	__Def_ParameterAttr(PA_AlarmSummaryDS42_t*, pALARM_SUM );//����ALARM��SUM���������16���鱨����״��������ÿ�������������䵱ǰ״̬��δ
										//ȷ��״̬��δ����״̬�ͽ���״̬��
										//ע��ʵ���й治��ȫ֧�ִ����ԡ����ڱ��й棬ֻʹ�ñ����ĵ�ǰ״̬����
	//__Def_ParameterAttr(PA_BatchDS67_t*, pBATCH );//�˲���ּ�����ڷ���GB��T 19892��1��2005����Ӧ�á������ܿ���д˲������ڹ��ܿ���
							//����Ҫ�㷨���ڷֲ�ʽ�ֳ�����ϵͳ��BATCH�����Ǳر��ģ�����ʶ�����õĺͿ��õ�
							//ͨ�������⣮�ھ���������»�ʶ��ǰ���������5 2��3��19
} _PACKED_ IDX_STDParameters_t;

typedef struct {
	PA_BlockDS32_t 	BLOCK_OBJECT; //BLOCK_OBJECT������ÿ����ĵ�1���������������ÿ�����ԣ����磬�����ͺ��й��

	uint16_t 		ST_REV ;      //����в��������޸ĵľ�̬�����������̬���Ż��ڼ�����Щ������ֵ���������������
									//һ����̬�������޸ģ���Ӧ������Ӧ��ST_REV(���ٰ�l����)������ṩ�˶Բ����汾
									//�ļ�顣���������������(������FACTORY��RESET=1)����ST_REVӦ����λΪ0��
									//���ٰ�1���ӣ���ָʾ��̬�����ĸı䡣���⣬���������һ������޸ģ���ST_REVӦ��
									//���ӡ�ST_REV��ֵ�ɱ���̬�豸�����ж��洢�ھ�̬�洢���еĿ����(�ڲ������Ա�
									//�о���"S"���Զ���)�Ƿ���ֵ�ȱ��ı䡣
									//��ST_REV����������Ӧ������Ϊl
	char 			TAG_DESC[32];//�ñ�ǩ�������û��ṩ�Ŀ��������ɸ�ÿ�������һ���ı�����ǩ����
	uint16_t 		STRATEGY;//STRATEGY���������û��ض���ֵ���˷���ֵ������̬������������������Ϣ�Ĺؼ���
	uint8_t 		ALERT_KEY;//ALERT_KEY���������û������ֵ����ֵ�����ڶ��ɿ�����ı������¼����з��ࡣ��
								//�ɰ���װ�õ�Ԫ�ı�ʶ�ţ��԰���ʶ���¼���װ�õ�Ԫ�е�λ��
	uint8_t 		TARGET_MODE;//TARGET_MODE����ָ���Ըÿ��������Ĳ���ģʽ����ͨ���ɿ���Ӧ�û��ɲ���Աͨ��
								//�˻��ӿ�Ӧ�������á������˲������״̬�Ľ�������ж��ÿ��ܷ�ʵ��Ҫ���Ŀ��ģʽ��
								//��MODE_BLK������PermittedԪ���������ģʽ�У�ֻ����������һ��ģʽ���Դ˲�
								//�����ж���һ��ģʽ��д���ʳ����˸ò����ķ�Χ���������Ծܾ���
								//���룺
								//Bit7�� �Ƿ���(Out of Service (O/S) - MSB
								//Bit6�� �ֶ���ʼ��(Initialisation Manually��IMan)(��A���B���в�ʹ��)
								//Bit5�� ���س���(Local Override��LO)(��A���в�ʹ��)
								//Bit4�� �ֶ�(Manual��MAN)
								//Bit3�� �Զ�(Automatic��AUTO)
								//Bit2�� ����(Cascade��Cas)(��A���B���в�ʹ��)
								//Bitl�� Զ�̼���(Remote-Cascade��RCas)
								//Bit0�� Զ�����(Remote-Output��ROut)����LSB(��A���B���в�ʹ��)
								//�ڱ��й���ʹ�õġ�automatic��ģʽ����Auto��Rcas����manual��ģʽ����LO��Man���� O/Sģʽ�£�����ִ�������㷨
	PA_ModeDS37_t 	MODE_BLK;//MODE_BLK������һ���ṹ����������ʵ��ģʽ(actual mode)������ģʽ(normal mode)
							//	������ģʽ(permitted mode)��ɡ�ʵ��ģʽ�ɸÿ�����ִ���ڼ��������(ͨ������)��
							//	�Է�ӳ����ִ���ڼ������õ�ģʽ������ģʽ�ǶԸÿ��������Ĳ���ģʽ��
							//	����ģʽ��MODE_BLK������Զ���û�ָ��Ŀ��ģʽ�����ָı��Ƕ��ض�����Ч�ġ�
							//	ģʽ�Թ��ܿ�����������ܽ����£�
							//	����Out of Service(O��s)��
							//	��ת���飺��o��sģʽ�£�ֹͣ�Բ���ֵ�����ֵ�ĸ�ֵ��
							//	�����ܿ飺��o��sģʽ�£�ֹͣ�Բ���ֵ�����ֵ�ĸ�ֵ��
							//	������飺ֹͣ��DIAGNOSIS��DIAGNOSIS EXTENSION�������ݸ��Ƶ�PROFIBUS DP��Slave_Diag����
							//	��ѭ�������ķ��ʲ���Ӱ�졣
							//	���˹��ܿ�����ֵӦ����Ϊ��һ��ֵ������������ܿ飬Ϊת�����ṩ��Ϣ�����
							//	����Ӧ����Ϊ����ʱ���涨��ֵ��������ϰ�ȫ����Ķ����޹ء�
							//	����Local Override(LO)��������֧�ָ������˲����Ŀ��ƿ������顣ͬ���������̿���
							//	�����豸�ı�����������������LOģʽ��������ģʽ�£������������Ϊ��������
							//	������ֵ��Ӧ��ʼ���㷨(��ģ���豸)��ʹ���ڴ�LOģʽ�л���Ŀ��ģʽʱ���ᾭ
							//	��ͻ�䡣
							//	����Manual(MAN)�����ܿ������ƿ��������������������������ɲ���Աͨ���ӿ���
							//	��ֱ�����á�Ӧ��ʼ���㷨��ʹ����ģʽ�л�ʱ���ᾭ��ͻ�䡣
							//	����Automatic(AUTO)�������������������빦�ܿ飬ʹ������ת��������˽��м��㣻
							//	����������ܿ飬ʹ���������ṩ�����Աͨ���ӿ��豸�ṩ���趨ֵ���м��㣻��
							//	��������ת���飬��ģʽ��ʾ��鹦���ܹ���������
							//����Remote Cascade(RCas)������趨ֵ�ɿ���Ӧ��ͨ��Զ�̼�������RCAS��IN�����á�
							//���ݸ��趨ֵ���������㷨���������ֵ��
							//���ܿ顢������ת�������Ϊͨ��MODE_BLK������ָʾ��MODE_BLKԪ�صı���
							//ͬTARGET_MODE�����������£�
							//1��ʵ��ģʽ(Actual)�����ǿ�ĵ�ǰģʽ���ɸ�������������ͬ��TARGET MODE����ֵ
							//�ļ����ǿ�ִ�е�һ���֡�
							//�����ܿ鴦����ֹ��������Ŀ��ģʽ��������ʱ�����ܿ��ʵ��ģʽ���Զ��ı䡣������
							//���������ʵ��ģʽ�ļ��㣺
							//��TARGET_MODE������
							//���ÿ��ģʽ���㡣
							//������㲻ͬ��TARGET_MODE��ʵ��ģʽʱ��Ӧʹ�����ȼ����ģʽ���ȼ�������
							//�£�����0����������ȼ���
							//��O��S�����ȼ�Ϊ7����ߣ�
							//��IMan�����ȼ�Ϊ6(δʹ��)��
							//��LO�����ȼ�Ϊ5}
							//��MAN�����ȼ�Ϊ4��
							//��AUT0�����ȼ�Ϊ3��
							//��Cas�����ȼ�Ϊ2(δʹ��)��
							//��RCas�����ȼ�Ϊ1��
							//��ROut�����ȼ�Ϊ0�����(δʹ��)��
							//����MODE����ϸ�ڵ�״̬���ǿ������ض���(�����ݵ�)��
							//2������ģʽ(Permitted)�������ʵ���������ģʽ������ģʽ�ɿ�������Ա������̬��
							//������Ӧ�����ݵ��ж�ÿ������ж��塣�豸Ӧ����κ�ģʽ�ı��������ȷ��������
							//��Ŀ��ģʽ������ģʽ���ж��塣
							//3������ģʽ(Normal)���������������������¿�ʹ�õ�ģʽ���˲������ɽӿ��豸����
							//ȡ�����������㷨ʹ�á�����ģʽ���ڱ��й淶Χ��ʹ�ã�������ʹ�á�
							//A���豸�Ŀ������ṩ��AUTO��ģʽ��Ϊ�ر�ģʽ��ģʽ���������B���豸�Ĺ��ܿ�
							//�Ǳر���
	PA_AlarmSummaryDS42_t ALARM_SUM ;//����ALARM��SUM���������16���鱨����״��������ÿ�������������䵱ǰ״̬��δ
										//ȷ��״̬��δ����״̬�ͽ���״̬��
										//ע��ʵ���й治��ȫ֧�ִ����ԡ����ڱ��й棬ֻʹ�ñ����ĵ�ǰ״̬����
	//PA_BatchDS67_t BATCH ;//�˲���ּ�����ڷ���GB��T 19892��1��2005����Ӧ�á������ܿ���д˲������ڹ��ܿ���
							//����Ҫ�㷨���ڷֲ�ʽ�ֳ�����ϵͳ��BATCH�����Ǳر��ģ�����ʶ�����õĺͿ��õ�
							//ͨ�������⣮�ھ���������»�ʶ��ǰ���������5 2��3��19
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
 *��103�ײ�(Slotl��Index0)����������
 */

typedef struct
{
	uint16_t Dir_ID;//����(Directory ID)�����й���δʹ�ã�
	uint16_t Dir_Rev_No;//Ŀ¼�汾�ţ�
	uint16_t No_Dir_Obj;//Ŀ¼����ĸ������������Ŀ¼ʹ�ö���һ��Ŀ¼��������ЩԪ�ر��������壬����ͬʹ��
						//һ���ϴ�Ķ��󡣶��Ŀ¼���󶼱����������ڸ�Ŀ¼�С��ö����������Ŀ¼����Ķ�
						//��Header���󲻼������У�
	uint16_t No_Dir_Entries;//Ŀ¼��¼���������Ӧ���㸴���б�Ŀ¼��¼��͸���Ŀ¼��¼����ܸ�����
	uint16_t FirstCompListDirEntry;//��1�������б�Ŀ¼��¼���Ŀ¼��¼�������������������Ŀ¼�ڵĵ�¼����������õ�
									//¼��Ĳ�����ַ����1��Ŀ¼��¼�����ڸ����б�Ŀ¼��¼���е���������á��ڼ�����¼
									//��ʱ�������б�Ŀ¼��¼���븴��Ŀ¼��¼��֮���޼����
	uint16_t NoCompListDirEntry;//�����б�Ŀ¼��¼��ĸ����������豸�ڵĲ�ͬ������(����顢ת����͹��ܿ�)�Ͷ�������
								//(�ڱ���׼��Χ�ڽ�������Ӷ���)��
}_PACKED_ PA_DirHeader_t;

/*
 * 5��2��6��2��3�����б�Ŀ¼��¼��(Composite_List_Directory_Entry)
 * a) ������Directory��Entry_Number��ָ�룯����������
 * b) ��1��ת�����Directory��Entry��Number��ָ�룯ת���������
 * c) ��1�����ܿ��Directory��Entry��Number��ָ�룯���ܿ������
 * d) ��1�����Ӷ����Directory��Entry��Number��ָ�룯���Ӷ��������
 */
typedef struct
{
	uint8_t BeginBlkiIndex;  //Index(Ŀ¼�����ţ�Index is the read and write service attribute Index of this parameter which contains the
							//according COMPOSITE_DIRECTORY_ENTRIES parameter. The valid range is 1 to 13.

	uint8_t BeginBlkOffset; //Ŀ¼��¼����Offset is the logical number of COMPOSITE_DIRECTORY_ENTRIES, which is counted within
							//the directory starting with the first COMPOSITE_LIST_DIRECTORY_ENTRY by 1.
	uint16_t Num_Blk;
}_PACKED_ PA_CompListDirectoryEntr_t;

/*
 *�͸�̨Ŀ¼��¼��(Composite_Directory��Entry)
 */
typedef struct
{
	uint8_t  Slot;
	uint8_t  Index;
	uint16_t NumPramater;
}_PACKED_ PA_CompDirectoryEntr_t;


typedef struct
{
	//����Ŀ¼�ײ���������slot 1,index 0
	PA_DirHeader_t DirObjHd;
	//�����б�Ŀ¼��¼�������slot 1 , index 1
	PA_CompListDirectoryEntr_t CompLstDirEntr_PB;
	PA_CompListDirectoryEntr_t CompLstDirEntr_TB;
	PA_CompListDirectoryEntr_t CompLstDirEntr_FB;
	PA_CompListDirectoryEntr_t CompLstDirEntr_LO; //��ѡ
	//����Ŀ¼��¼�������slot 1 , index 2 ��ʼ
	PA_CompDirectoryEntr_t   CompDirEntr_PB[NUM_BLK_PB]; //
	PA_CompDirectoryEntr_t   CompDirEntr_TB[NUM_BLK_TB];
	PA_CompDirectoryEntr_t   CompDirEntr_FB[NUM_BLK_FB];
	PA_CompDirectoryEntr_t   CompDirEntr_LO[NUM_BLK_LO];

}_PACKED_ PA_DM_t;

typedef struct
{
	//����Ŀ¼�ײ���������slot 1,index 0
	__Def_ParameterAttr(PA_DirHeader_t, pDirObjHd);
	//�����б�Ŀ¼��¼�������slot 1 , index 1
	__Def_ParameterAttr(PA_CompListDirectoryEntr_t,pCompLstDirEntr);
	//����Ŀ¼��¼�������slot 1 , index 2 ��ʼ
	__Def_ParameterAttr(PA_CompDirectoryEntr_t ,pCompDirEntr); //

}_PACKED_ IDX_PA_DM_t;

#define RLAINDX_DIROBJHD   			0
#define RLAINDX_COMPLISTDIR1ENTRY  	1
#define RLAINDX_COMPDIR2ENTRY  	    2

/*
 * 5��2��7��(Table)����
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
		 * ����TAB_ENTRY��ʶ�ñ���ĸ�Ԫ�ص�ǰ���ڲ���TAB X Y��VAI��UE��
		 */
	__Def_ParameterAttr(PA_tab_value_t*, pTab_xy_value);
		/*
		 * ����TA��8_kY��VAI��UE�����ñ��һ��ֵ
		 */
	__Def_ParameterAttr(uint8_t*, pTab_min_number);
		/*
		 * ����û�����ڲ�ԭ��(����Ϊ�˼���)����ʱ����Ҫʹ��һ�������ĵ�¼��ø����ڲ�
		 * ��TAB-MIN��NUMBER���ṩ
		 */
	__Def_ParameterAttr(uint8_t*, pTab_max_number);
		/*
		 * TAB-MA)LNUMBER���豸�ı��е�¼���������(TAB��x��VAI��UE��TAB��Y��
		 * VAI��UEֵ�Եĸ���)
		 */
	__Def_ParameterAttr(uint8_t*, pTab_op_code);
		/*
		 * �޸��豸�еı��Ӱ����豸�Ĳ�����ִ���㷨����ˣ�����ָ����ʼ��ͽ����㡣
			TA��3_OP CODE���Ƹñ�Ĵ���
			0��δ��ʼ��
			1�� �²������ԣ���1��ֵ(TAnENTRYһ1)
			2������
			3����һ��ֵ������������������������滻�����ߣ�����TAB��ACTUALNUM
			BER
			4��ɾ����TAB_ENTRY�涨�ı��еĵ㲢��СTAB��AcTuAL��NUMBER(��
			ѡ)��
			5��������TAB_x��Y��VALUE����ĵ㣬��TAB-x��VALUE����˳��Ա�����
			������TAB ACTUALNUMBER(��ѡ)
			6�� ��ʵ��ENTRY������еĵ�(��ѡ)
			���迪ʼ��ֹͣ����(TAB-oB_CODE 1��TAB��OB��CODE 3)�Ϳ��ܶ�ȡһ������
			�е�ĳЩ���֡�ͨ����TAB-ENTRY��Ϊ1��ָʾ��ʼ
		 */
	__Def_ParameterAttr(uint8_t*, pTab_status);
		/*
		 * ͨ�����豸���ṩ��ʵ�Լ�顣���Ľ���ڲ���TAB_sTATus��ָʾ��
			0��δ��ʼ��
			1����ȷ(�±���Ч)
			2���ǵ�������(�ɱ���Ч)
			3���ǵ����ݼ�(�ɱ���Ч)
			4�� �������ֵ����(�ɱ���Ч)
			5�� �������ֵ̫��(�ɱ���Ч)
			6���߽�б��(gradient)̫��(�ɱ���Ч)
			7��ֵδ������(��ֵ��Ч)
			8�� ��ǰ��װ�أ���TAl8 OP CODE=l֮������TAB-OP��CODE=3֮ǰ����
			(�Ա������������Ч����ֵ��Ч)��
			9�����򲢼���(�Ա������������Ч����ֵ��Ч)��
			10��19������
			20���ǵ�������(��δ��ʼ��)
			21�� �ǵ����ݼ�(��δ��ʼ��)
			22�� �������ֵ����(��δ��ʼ��)
			23�� �������ֵ̫��(��δ��ʼ��)
			24�� �߽�б��̫��(��δ��ʼ��)
			25��ֵδ������(��δ��ʼ��)
			26�� ��ǰ��װ�أ���TABһ0P��CODE=1֮������TAB OP��CODE=3֮ǰ��
			��(�Ա������������Ч����δ��ʼ��)
			27�����򲢼���(�Ա������������Ч����δ��ʼ��)
			28��127������
			>128���������ض�
		 */
	__Def_ParameterAttr(uint8_t*, pTab_actual_number);
		/*
		 * �������е�¼���ʵ�ʸ���������ɸñ�Ĵ����Ӧ������м���
		 */
	__Def_ParameterAttr(uint8_t*, pLin_type);
		/*
		 * ���Ի����͡�
			0�� �����Ի�(�ر�)
			1�� ���Ի���(��ѡ)
			10�� ƽ����(��ѡ)
			20�� Բ������ʽ����(��ѡ)
			21�� ��������(��ѡ)
			50�� �Ȱٷֱ�1��33(��ѡ)
			5l�� ����Ȱٷֱ�(�쿪)1��33(��ѡ)
			52�� �Ȱٷֱ�1��50(��ѡ)
			53�� ����Ȱٷֱ�1��50(��ѡ)
			54�� �Ȱٷֱ�1t 25(��ѡ)
			55�� ����Ȱٷֱ�1��25(��ѡ)
			100�� RTDPtl0 a��O��003 850(IEC 60751)
			101�� RTD Pt50 aһ0��003 850(IEC 60751)
			102�� RTD Ptl00 aһ0��003 850(IEC 60751)
			103�� RTD Pt200 aһ0��003 850(IEC 60751)
			104�� RTD PtS00 a#0��003 850(IEC 60751)
			105�� RTD Ptl000 aһ0��003 850(IEC 60751)
			106�� RTD Ptl0 aһ0��003 916(JIS C1604��81)
			107�� RTD PtS0 a=0��003 916(JIS C1604��81)
			108�� RTD Ptl00 aһ0��003 916(JISCl604��81)
			109�� RTD Ptl0 a=0��003 920(MIL��Tһ24388)
			110��u RTD Pt50 a=0��003 920(MlL_Tһ24388)
			111�� RTD Ptl00 aһ0��003 920(MIoT-24388)
			112�� RTD Pt200 aһ0��003 920(MIL-T-24388)
			113�� RTD Pt500 aһ0��003 920(MII��һT-24388)
			114�� RTD Ptl000 aһ0��003 920(MIL T 24388)
			115�� RTD Ptl00 aһ0��003 923(SAMA RC2��4��1 966)
			116�� RTD Pt200 aһ0��003 923(SAMA RC21��4��1966)
			117�� RTD Ptl00 aһ0��003 926(IPTs-68)
			118�� RTD Ni50 aһ0��006 720(Edison curve#7)
			119�� RTD Nil00 aһ0��006 720(Edison curve#7)
			120�� RTD Nil20 aһ0��006 720(Edison curve#7)
			121�� RTD Nil000 a��0��006 720(Edison curve#7)
			122�� RTD Ni50 aһ0��006 180(DIN 43760)
			123�� RTD Nil00 aһ0��006 180(DIN 43760)
			124�� RTD Nil20 aһ0��006 180(DIN 43760)
			125�� RTD Nil000 aù0��006 180(DIN 43760)
			126�� RTDCul0 aһ0��004 270
			127�� RTD Cul00 aһ0��004 270
			128��TC Type B��Pt30Rh-Pt6Rh(IEC 60584��NIST MN 175��DIN 43710��BS 4937��
			ANSI MC96��1��jis C1602��NF C42��321)
			129��TC Type C(W5)��W5һW26Rh(ASTM E988)
			130��TC Type D(W3)��W3һW25Rh(ASTM E988)
			131�� TCTypeE��N110C��Cu45Ni(IEC 60584��NISTMNl75��DIN 43710��BS 4937��
			ANSI MC96��1��JIS C1602��NF C42��321)
			132��TC Type G(W)��w_W26Rh(ASTM E988)
			133�� TC Type J��Fe��Cu45Ni(IEC 60584��NIST MN 175��DIN 43710��BS 4937��
			ANSI MC96��1��JIS C1602��NF C42��321)
			134�� TC Type KtNil0C��Ni5(IEC 60584��NIST MN 175��DIN 43710��BS 4937��
			ANSI MC96��1��JISCl602��NFC42 321)
			135��TC Type NtNil4CrSONlSi(IEC 60584��NIST MN 175��DIN 43710��BS 4937��
			ANSI MC96��1��JIS C1602��NF C42��321)
			136��TCType R��Ptl3Rh Pt(IEC 60584��NIST MN 175��DIN 43710��BS 4937��
			ANSI MC96��1��J1S C1602��NF C42��321)
			137��TCType S��Ptl0Rh��Pt(IEC 60584��N1ST MN 175��DIN 43710��BS 4937��
			ANSI MC96��1��JIS C1602��NF C42��321)
			138��TC Type T��Cu-Cu45Ni(IEC 60584��NIST MN 175��DIN 43710��BS 4937��
			ANSI MC96��1��JIS C1602��NF C42 321)
			139��TC Type L��Fe-CuNi(DIN 43710)
			140��TC Type U��Cu-CuNi(DIN 43710)
			141�� TC Type Pt20��Pt40��Pt20Rh��Pt40Rh(ASTM E1751)
			142��TC Type lr��Ir40��Ir-Ir40Rh(ASTM E1751)
			143- TC Platinel II
			144��TC Ni��NiMo
			145��239������
			240�� �������ض�
			249�� �������ض�
			250�� δʹ��
			251�� ������
			252�� δ֪
			253�� ����
			254��255������
		 */
}_PACKED_ IDX_TAB_PARAMETER_t;
typedef struct {
	uint8_t Tab_entry;
		/*
		 * ����TAB_ENTRY��ʶ�ñ���ĸ�Ԫ�ص�ǰ���ڲ���TAB X Y��VAI��UE��
		 */
	PA_tab_value_t Tab_xy_value[MAX_TAB_NUM];
		/*
		 * ����TA��8_kY��VAI��UE�����ñ��һ��ֵ
		 */
	uint8_t Tab_min_number;
		/*
		 * ����û�����ڲ�ԭ��(����Ϊ�˼���)����ʱ����Ҫʹ��һ�������ĵ�¼��ø����ڲ�
		 * ��TAB-MIN��NUMBER���ṩ
		 */
	uint8_t Tab_max_number;
		/*
		 * TAB-MA)LNUMBER���豸�ı��е�¼���������(TAB��x��VAI��UE��TAB��Y��
		 * VAI��UEֵ�Եĸ���)
		 */
	uint8_t Tab_op_code;
		/*
		 * �޸��豸�еı��Ӱ����豸�Ĳ�����ִ���㷨����ˣ�����ָ����ʼ��ͽ����㡣
			TA��3_OP CODE���Ƹñ�Ĵ���
			0��δ��ʼ��
			1�� �²������ԣ���1��ֵ(TAnENTRYһ1)
			2������
			3����һ��ֵ������������������������滻�����ߣ�����TAB��ACTUALNUM
			BER
			4��ɾ����TAB_ENTRY�涨�ı��еĵ㲢��СTAB��AcTuAL��NUMBER(��
			ѡ)��
			5��������TAB_x��Y��VALUE����ĵ㣬��TAB-x��VALUE����˳��Ա�����
			������TAB ACTUALNUMBER(��ѡ)
			6�� ��ʵ��ENTRY������еĵ�(��ѡ)
			���迪ʼ��ֹͣ����(TAB-oB_CODE 1��TAB��OB��CODE 3)�Ϳ��ܶ�ȡһ������
			�е�ĳЩ���֡�ͨ����TAB-ENTRY��Ϊ1��ָʾ��ʼ
		 */
	uint8_t Tab_status;
		/*
		 * ͨ�����豸���ṩ��ʵ�Լ�顣���Ľ���ڲ���TAB_sTATus��ָʾ��
			0��δ��ʼ��
			1����ȷ(�±���Ч)
			2���ǵ�������(�ɱ���Ч)
			3���ǵ����ݼ�(�ɱ���Ч)
			4�� �������ֵ����(�ɱ���Ч)
			5�� �������ֵ̫��(�ɱ���Ч)
			6���߽�б��(gradient)̫��(�ɱ���Ч)
			7��ֵδ������(��ֵ��Ч)
			8�� ��ǰ��װ�أ���TAl8 OP CODE=l֮������TAB-OP��CODE=3֮ǰ����
			(�Ա������������Ч����ֵ��Ч)��
			9�����򲢼���(�Ա������������Ч����ֵ��Ч)��
			10��19������
			20���ǵ�������(��δ��ʼ��)
			21�� �ǵ����ݼ�(��δ��ʼ��)
			22�� �������ֵ����(��δ��ʼ��)
			23�� �������ֵ̫��(��δ��ʼ��)
			24�� �߽�б��̫��(��δ��ʼ��)
			25��ֵδ������(��δ��ʼ��)
			26�� ��ǰ��װ�أ���TABһ0P��CODE=1֮������TAB OP��CODE=3֮ǰ��
			��(�Ա������������Ч����δ��ʼ��)
			27�����򲢼���(�Ա������������Ч����δ��ʼ��)
			28��127������
			>128���������ض�
		 */
	uint8_t Tab_actual_number;
		/*
		 * �������е�¼���ʵ�ʸ���������ɸñ�Ĵ����Ӧ������м���
		 */
	uint8_t Lin_type;
		/*
		 * ���Ի����͡�
			0�� �����Ի�(�ر�)
			1�� ���Ի���(��ѡ)
			10�� ƽ����(��ѡ)
			20�� Բ������ʽ����(��ѡ)
			21�� ��������(��ѡ)
			50�� �Ȱٷֱ�1��33(��ѡ)
			5l�� ����Ȱٷֱ�(�쿪)1��33(��ѡ)
			52�� �Ȱٷֱ�1��50(��ѡ)
			53�� ����Ȱٷֱ�1��50(��ѡ)
			54�� �Ȱٷֱ�1t 25(��ѡ)
			55�� ����Ȱٷֱ�1��25(��ѡ)
			100�� RTDPtl0 a��O��003 850(IEC 60751)
			101�� RTD Pt50 aһ0��003 850(IEC 60751)
			102�� RTD Ptl00 aһ0��003 850(IEC 60751)
			103�� RTD Pt200 aһ0��003 850(IEC 60751)
			104�� RTD PtS00 a#0��003 850(IEC 60751)
			105�� RTD Ptl000 aһ0��003 850(IEC 60751)
			106�� RTD Ptl0 aһ0��003 916(JIS C1604��81)
			107�� RTD PtS0 a=0��003 916(JIS C1604��81)
			108�� RTD Ptl00 aһ0��003 916(JISCl604��81)
			109�� RTD Ptl0 a=0��003 920(MIL��Tһ24388)
			110��u RTD Pt50 a=0��003 920(MlL_Tһ24388)
			111�� RTD Ptl00 aһ0��003 920(MIoT-24388)
			112�� RTD Pt200 aһ0��003 920(MIL-T-24388)
			113�� RTD Pt500 aһ0��003 920(MII��һT-24388)
			114�� RTD Ptl000 aһ0��003 920(MIL T 24388)
			115�� RTD Ptl00 aһ0��003 923(SAMA RC2��4��1 966)
			116�� RTD Pt200 aһ0��003 923(SAMA RC21��4��1966)
			117�� RTD Ptl00 aһ0��003 926(IPTs-68)
			118�� RTD Ni50 aһ0��006 720(Edison curve#7)
			119�� RTD Nil00 aһ0��006 720(Edison curve#7)
			120�� RTD Nil20 aһ0��006 720(Edison curve#7)
			121�� RTD Nil000 a��0��006 720(Edison curve#7)
			122�� RTD Ni50 aһ0��006 180(DIN 43760)
			123�� RTD Nil00 aһ0��006 180(DIN 43760)
			124�� RTD Nil20 aһ0��006 180(DIN 43760)
			125�� RTD Nil000 aù0��006 180(DIN 43760)
			126�� RTDCul0 aһ0��004 270
			127�� RTD Cul00 aһ0��004 270
			128��TC Type B��Pt30Rh-Pt6Rh(IEC 60584��NIST MN 175��DIN 43710��BS 4937��
			ANSI MC96��1��jis C1602��NF C42��321)
			129��TC Type C(W5)��W5һW26Rh(ASTM E988)
			130��TC Type D(W3)��W3һW25Rh(ASTM E988)
			131�� TCTypeE��N110C��Cu45Ni(IEC 60584��NISTMNl75��DIN 43710��BS 4937��
			ANSI MC96��1��JIS C1602��NF C42��321)
			132��TC Type G(W)��w_W26Rh(ASTM E988)
			133�� TC Type J��Fe��Cu45Ni(IEC 60584��NIST MN 175��DIN 43710��BS 4937��
			ANSI MC96��1��JIS C1602��NF C42��321)
			134�� TC Type KtNil0C��Ni5(IEC 60584��NIST MN 175��DIN 43710��BS 4937��
			ANSI MC96��1��JISCl602��NFC42 321)
			135��TC Type NtNil4CrSONlSi(IEC 60584��NIST MN 175��DIN 43710��BS 4937��
			ANSI MC96��1��JIS C1602��NF C42��321)
			136��TCType R��Ptl3Rh Pt(IEC 60584��NIST MN 175��DIN 43710��BS 4937��
			ANSI MC96��1��J1S C1602��NF C42��321)
			137��TCType S��Ptl0Rh��Pt(IEC 60584��N1ST MN 175��DIN 43710��BS 4937��
			ANSI MC96��1��JIS C1602��NF C42��321)
			138��TC Type T��Cu-Cu45Ni(IEC 60584��NIST MN 175��DIN 43710��BS 4937��
			ANSI MC96��1��JIS C1602��NF C42 321)
			139��TC Type L��Fe-CuNi(DIN 43710)
			140��TC Type U��Cu-CuNi(DIN 43710)
			141�� TC Type Pt20��Pt40��Pt20Rh��Pt40Rh(ASTM E1751)
			142��TC Type lr��Ir40��Ir-Ir40Rh(ASTM E1751)
			143- TC Platinel II
			144��TC Ni��NiMo
			145��239������
			240�� �������ض�
			249�� �������ض�
			250�� δʹ��
			251�� ������
			252�� δ֪
			253�� ����
			254��255������
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
#define TAB_STATUS_GOOD          1// good (new table is valid)��ȷ(�±���Ч)
#define TAB_STATUS_NOMONOINC     2// not monotonous increasing (old table is valid)�ǵ�������(�ɱ���Ч)
#define TAB_STATUS_NOMONODEC     3// not monotonous decreasing (old table is valid)�ǵ����ݼ�(�ɱ���Ч)
#define TAB_STATUS_NOENOVAL      4// not enough values transmitted (old table is valid)�������ֵ����(�ɱ���Ч)
#define TAB_STATUS_TOOMANYVAL    5// too many values transmitted (old table is valid)�������ֵ̫��(�ɱ���Ч)
#define TAB_STATUS_GREDTOOHIGH   6// gradient of edge too high (old table is valid) �߽�б��(gradient)̫��(�ɱ���Ч)
#define TAB_STATUS_VALNOTACC     7// Values not accepted (old values are valid)ֵδ������(��ֵ��Ч)
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
	//���ڲ��������ܵĿ��Ʋ�����
	PTA_PASSIVE 		=(uint8_t)0,//	�޶���(ȱʡֵ)
	PTA_START_STRICT	=(uint8_t)1,//	ʹ���ϸ�(strict)ģʽ(���������ݼ���100��������)��������������
	PTA_START_SMOOTH	=(uint8_t)2,//	ʹ��ƽ��(smooth)ģʽ(�Ǽ����Բ��ᵼ��ֹͣ���ص�֪ͨ������)��������������
	PTA_ABORT			=(uint8_t)3,// �쳣��ֹ��������(ֹͣ����Ͷ���ǰ���ݼ��ļ���)�����Ծ��е����������ݻ��������豸��Ч��û�е����������ݻ��������豸����ܾ�������(������룺��Ч��Χ)��
	PTA_TERMINATE		=(uint8_t)4,//	���������������վָ��������������ݶ��ѱ����䡣�豸��ʼ��֤�ô������ݼ���
	PTA_CONFIRM			=(uint8_t)5,//	���ַǼ����Ի�������ݼ����û�֤ʵ��
	//6��127��PI������
	//128��249���������ض���
	//250��255��PI����
}PTA_OP_CODE_e;

typedef enum
{
	//���ڴ������еĵ�ǰ״̬���豸������Ч�Ե�״����Ϣ��
	//0������
	STPTA_TA_ACTIVE		=(uint8_t)1,//�ô���ǰ����Ч�ġ�
	STPTA_VERIFICATION	=(uint8_t)2,//�ô������ݼ�����֤����Ч��(���ܼ��)��
	STPTA_DATA_COMVALID	=(uint8_t)3,//�����ݼ�����ȫ��Ч(һ��)�ġ�
	STPTA_DATA_NOTVALID	=(uint8_t)4,//�����ݼ�����Ч(��һ��)�ġ������ݼ�����������Ԥ�ڵĲ�����
	STPTA_DATA_PARVALID	=(uint8_t)5,//�����ݼ����ܽ�������Ч(һ��)����ʹ��ƽ��ģʽ(ʹ��PTA_START_SMOOTH�������ô���)�������֧�ִ�״����
	//6��127��PI������
	//128��249���������ض���
	//250��255��PI����
}PTA_STATUS_e;

/*
 * @bref ��ȡд���ʱ�������
 *
 */
typedef struct
{
	Boolean bLocalAccess; //���ط��ʽ�ֹ���
	Boolean bRemoteAccess;//Զ�̷��ʽ�ֹ���
	Boolean bExtLocalAccess;// true: �������ܱ��ط��ʽ�ֹ��������AO��DO������TARGET MODE��OUT��OUT��D�ǿ�д�ġ�
	Boolean bExtRemoteAccess;// true: ��������Զ�̷��ʽ�ֹ��������AO��DO������TARGET MODE��OUT��OUT��D�ǿ�д�ġ�
}WriteAPCtrl_t;

typedef struct
{
	IDX_STDParameters_t Std;
	__Def_ParameterAttr(char*, pSOFTWARE_REVISION);//�ֳ��豸������汾��
	__Def_ParameterAttr(char*, pHARDWARE_REVISION);//�ֳ��豸��Ӳ���汾��
	__Def_ParameterAttr(uint16_t*, pDEVICE_MAN_ID);//�ֳ��豸�����̵ı�ʶ����
	__Def_ParameterAttr(char*, pDEVICE_ID);//�������ض����豸��ʶ
	__Def_ParameterAttr(char*, pDEVICE_SER_NUM);//�ֳ��豸�����к�
	__Def_ParameterAttr(uint8_t*, pDIAGNOSIS);//�豸����ϸ��Ϣ�������ر��롣ͬʱ���ܰ��������Ϣ�����Byte4��MSB���� Ϊ1��
	//���ڲ���DIAGNOSIS_EXTENSION�ڿ��ṩ���������Ϣ��	DIAGNOSIS�����ı�����5��3��3��5��3��4�й涨
	__Def_ParameterAttr(uint8_t*, pDIAGNOSIS_EXTENSION);//���ӵ��������ض����豸��Ϣ�������ر��롣ͬʱ���ܰ��������Ϣ
	__Def_ParameterAttr(uint8_t*, pDIAGNOSIS_MASK);//��֧�ֵ�DIAGNOSIS��Ϣ���صĶ��塣0����֧��	l��֧��
	__Def_ParameterAttr(uint8_t*, pDIAGNOSIS_MASK_EXTENSION);//��֧�ֵ�DIAGNOSIS_ExTENSl(��)N��Ϣ���صĶ��塣	0����֧��	1��֧��
	__Def_ParameterAttr(char*, pDEVICE_CERTIFICATION); //�ֳ��豸����֤����EX��֤
	__Def_ParameterAttr(uint16_t*, pWRITE_LOCKING);//���д������
							//0���ܾ������в����ķ�ѭ��д����(���ʱ��ܾ�)����WRITE-LocKING��
							//�����
							//1��2456������
							//2457��ȱʡֵ������ʾ�豸�����п�д�������ǿ�д��
							//2458��32767������
							//32768��65535���������ض�
							//����Ĳ������κ�д�������ƣ�
							//����TAB_ENTRY(���Ի���)��
							//����AcTuALPosT��READLNUMBER(��־���ܿ�)��
							//���⣮���۴��ں���д����״̬���豸��Ӧ���ܲ�����Execution��Argument��
							//CaII-REQ-PDU�������I��M����
	__Def_ParameterAttr(uint16_t*, pFACTORY_RESET); //���룺
							//1��(�ر�)�ǰ��豸��λΪȱʡֵ��������ߵ�ַ�����ò���Ӱ�졣
							//2��(��ѡ)�ǰ��豸��Ϣ�Բ�����λΪȱʡֵ��������ھ���
							//��informational����λ��������Ĳ�������ÿ����Ĳ������Ա��ڶ�����ж��塣
							//���ߵ�ַ�����ò���Ӱ�졣
							//3��(��ѡ)�ǰѾ��С�functional����λ����������豸������λΪȱʡֵ����
							//����ߵ�ַ�����ò���Ӱ�졣
							//4��2505��������
							//2506��(��ѡ)�������豸��������з���ʧ�Բ������ֲ��䣬���ж�̬��
							//������λΪȱʡֵ��
							//2507��2711��������
							//2712��(��ѡ)�������ߵ�ַ��Ϊȱʡ��ַ�������������ñ��ֲ��䡣��ʹ�豸
							//����ѭ�����ݴ���״̬�����ߵ�ַҲӦ�����ı䡣ֱ���������ϵ����ڣ���������
							//��λ����ͣ����Ӧ��Set_Slave��Add�����No_Add��Chg_Flag�������
							//2713--32767��������
							//32768��65535���������ض���
							//��������������λ������������ض����
							//����IDENT��NUMBER��SELECTOR����FACTORY��RESETӰ�졣
							//ע���ɱ�����ʾ�ĵ�ַ�������ڱ���׼�ķ�Χ
	__Def_ParameterAttr(uint8_t*, pDESCRIPTOR);//�û��ɶ�����ı�(�ַ���)����������Ӧ���ڵ��豸
	__Def_ParameterAttr(uint8_t*, pDEVICE_MESSAGE);//�û��ɶ����MESSAGE(�ַ���)����������Ӧ���ڻ�װ���е��豸
	__Def_ParameterAttr(uint8_t*, pDEVICE_INSTAL_DATE);//�豸�İ�װ����
	__Def_ParameterAttr(uint8_t*, pLOCAL_OP_ENA);//���ñ��ز�����
						//0�� ����(�������ز������������������������豸��FBMODE���޸�)
						//1�� ����(�����ز���)
						//���������ȱ����ն��豸�������и��ߵ����ȼ�
						//���ͨ��ʧ�ܵ�ʱ�䳬��30 S�����Զ����ñ��ز��������ͨ��ʧ�ܵĶ�����ָ
						//�ڹ涨ʱ�����δ����ѭ��ͨ�š��������LOCAL��OP��ENA����0(����)��
						//�ָ�ͨ�ţ����豸�л���Զ�̲�������5��2��8��4
	__Def_ParameterAttr(uint8_t*, pIDENT_NUMBER_SELECTOR);//ÿ������1EC 61784 1��CP 3��1��PRoFIBUS DP�豸Ӧ����һ����PI�ṩ��
								//Ident Number����ldent��Number�涨���豸����ӦGSD�ļ���������ѭ����Ϊ
								//���ԡ�����IEC 61784 1��CP 3��2��PROFIBUS PA�豸Ӧ����֧��һ���й�
								//�ض�ldent��Number���ڱ���׼��6��4��1�ж������й��ض�Ident_Number�����
								//ĳ�豸����Ϊ�й��ض�Ident_Number������Ӧ������Ӧ�й�GSD�ļ����й���
								//�������⣬PROFIBUS PA�豸��֧���������ض�ldent_Number��
								//�й��ض���GSD�ļ���PI�ṩ���������ض���GSD�ļ����豸�������ṩ��
								//�û��ܹ�ͨ�����ò���IDENT_NUMBER_SELECTOR��ѡ����Ч��ldent_
								//Number�����Ident_Number���ı䣬��û����ѭ����Ϊ����(���磺������ݣ���
								//�ȡ���ǰ���ѽ��ܵ���̬���ݵ�)Ҳ�����ı䡣
								//���룺
								//0���й��ض�Ident��Number(PA�й�V3��x)(�ر�)
								//l���������ض�Ident��Number(PA�й�V3 x)(��ѡ)
								//2���������ض�Ident��Number(PA�й�V2��0)(��ѡ)
								//3��������豸���й��ض�Ident��Number(PA�й�V3��x)(��ѡ)
								//4��126���������й�ʹ��(������)
								//127����Ӧģʽ(�ر�)
								//128��255���������ض�(��ѡ)
	__Def_ParameterAttr(uint8_t*, pHW_WRITE_PROTECTION);//ָ�����ܱ�Զ�̷����޸ĵ�д��������(���硣Ӳ���߻򱾵��û��ӿ�)��״̬��
								//����ֹ�޸��豸�Ĳ�����
								//0���ޱ���(�ر�)
								//1�������������ֶ�����(��ѡ)
								//�ܾ������в����ķ�ѭ��д����(д���ʱ��ܾ�)�������Ի���Ĳ���TAB��
								//ENTRY���Լ�����TARGET��MODE��0UT��OUT��D(����AO��DO��
								//Ч)���⡣
								//2�����������ֶ�����(��ѡ)
								//�ܾ������в����ķ�ѭ��д����(д���ʱ��ܾ�)�������Ի���Ĳ���TAB��
								//ENTRY���⡣
								//3��127������
								//128��255���������ض�
								//�豸��֧�ִ���1�ͣ������2��
								//���²������κ�д�������ƣ�
								//��TAB-ENTRY(���Ի���)��
								//��ACTUALPOST_READ��NUMBER(��־���ܿ�)��
								//���⣬���۴��ں���д����״̬���豸��Ӧ���ܲ�����Execution��Argument��
								//Call��REQ-PDU�������I&M����
	__Def_ParameterAttr(PA_FeatureDS68_t*, pFEATURE);//ָʾ�豸����ʵ�ֵĿ�ѡ���ԣ��Լ��Ƿ�֧����Щ���Ե����
	__Def_ParameterAttr(uint8_t*, pCOND_STATUS_DIAG);//ָʾ�ɶ�״���������Ϊ������̬���豸ģʽ��
									//0���ṩ5��3��2�ж����״������ϣ�
									//l���ṩŨ��״���������Ϣ(��5��3��4)
									//2��255��PI����
	__Def_ParameterAttr(PA_DiagEventSwitch_t*, pDIAG_EVENT_SWITCH);//���FEATURE��Enabled��Condensed��Status=1����ָʾ�����Ƹ��豸���豸�ض�
											//����¼��ķ�Ӧ��
											//������¼��ĵ�¼��������������̣��豸�ض���

	__Def_ParameterAttr(PTA_OP_CODE_e*, pPTA_OP_CODE);
	__Def_ParameterAttr(PTA_STATUS_e* , pPTA_STATUS);
}_PACKED_ IDX_PB_BLOCK_t;

typedef struct
{
	PA_STDParameters_t std;
	char SOFTWARE_REVISION[16];//�ֳ��豸������汾��
	char HARDWARE_REVISION[16];//�ֳ��豸��Ӳ���汾��
	uint16_t DEVICE_MAN_ID;//�ֳ��豸�����̵ı�ʶ����
	char DEVICE_ID[16];//�������ض����豸��ʶ
	char DEVICE_SER_NUM[16];//�ֳ��豸�����к�
	uint8_t DIAGNOSIS[4];//�豸����ϸ��Ϣ�������ر��롣ͬʱ���ܰ��������Ϣ�����Byte4��MSB���� Ϊ1��
	//���ڲ���DIAGNOSIS_EXTENSION�ڿ��ṩ���������Ϣ��	DIAGNOSIS�����ı�����5��3��3��5��3��4�й涨
	uint8_t DIAGNOSIS_EXTENSION[6];//���ӵ��������ض����豸��Ϣ�������ر��롣ͬʱ���ܰ��������Ϣ
	uint8_t DIAGNOSIS_MASK[4];//��֧�ֵ�DIAGNOSIS��Ϣ���صĶ��塣0����֧��	l��֧��
	uint8_t DIAGNOSIS_MASK_EXTENSION[6];//��֧�ֵ�DIAGNOSIS_ExTENSl(��)N��Ϣ���صĶ��塣	0����֧��	1��֧��
	char DEVICE_CERTIFICATION[32]; //�ֳ��豸����֤����EX��֤
	uint16_t WRITE_LOCKING;//���д������
							//0���ܾ������в����ķ�ѭ��д����(���ʱ��ܾ�)����WRITE-LocKING��
							//�����
							//1��2456������
							//2457��ȱʡֵ������ʾ�豸�����п�д�������ǿ�д��
							//2458��32767������
							//32768��65535���������ض�
							//����Ĳ������κ�д�������ƣ�
							//����TAB_ENTRY(���Ի���)��
							//����AcTuALPosT��READLNUMBER(��־���ܿ�)��
							//���⣮���۴��ں���д����״̬���豸��Ӧ���ܲ�����Execution��Argument��
							//CaII-REQ-PDU�������I��M����
	uint16_t FACTORY_RESET; //���룺
							//1��(�ر�)�ǰ��豸��λΪȱʡֵ��������ߵ�ַ�����ò���Ӱ�졣
							//2��(��ѡ)�ǰ��豸��Ϣ�Բ�����λΪȱʡֵ��������ھ���
							//��informational����λ��������Ĳ�������ÿ����Ĳ������Ա��ڶ�����ж��塣
							//���ߵ�ַ�����ò���Ӱ�졣
							//3��(��ѡ)�ǰѾ��С�functional����λ����������豸������λΪȱʡֵ����
							//����ߵ�ַ�����ò���Ӱ�졣
							//4��2505��������
							//2506��(��ѡ)�������豸��������з���ʧ�Բ������ֲ��䣬���ж�̬��
							//������λΪȱʡֵ��
							//2507��2711��������
							//2712��(��ѡ)�������ߵ�ַ��Ϊȱʡ��ַ�������������ñ��ֲ��䡣��ʹ�豸
							//����ѭ�����ݴ���״̬�����ߵ�ַҲӦ�����ı䡣ֱ���������ϵ����ڣ���������
							//��λ����ͣ����Ӧ��Set_Slave��Add�����No_Add��Chg_Flag�������
							//2713--32767��������
							//32768��65535���������ض���
							//��������������λ������������ض����
							//����IDENT��NUMBER��SELECTOR����FACTORY��RESETӰ�졣
							//ע���ɱ�����ʾ�ĵ�ַ�������ڱ���׼�ķ�Χ
	uint8_t DESCRIPTOR[32];//�û��ɶ�����ı�(�ַ���)����������Ӧ���ڵ��豸
	uint8_t DEVICE_MESSAGE[32];//�û��ɶ����MESSAGE(�ַ���)����������Ӧ���ڻ�װ���е��豸
	uint8_t DEVICE_INSTAL_DATE[16];//�豸�İ�װ����
	uint8_t LOCAL_OP_ENA;//���ñ��ز�����
						//0�� ����(�������ز������������������������豸��FBMODE���޸�)
						//1�� ����(�����ز���)
						//���������ȱ����ն��豸�������и��ߵ����ȼ�
						//���ͨ��ʧ�ܵ�ʱ�䳬��30 S�����Զ����ñ��ز��������ͨ��ʧ�ܵĶ�����ָ
						//�ڹ涨ʱ�����δ����ѭ��ͨ�š��������LOCAL��OP��ENA����0(����)��
						//�ָ�ͨ�ţ����豸�л���Զ�̲�������5��2��8��4
	uint8_t IDENT_NUMBER_SELECTOR;//ÿ������1EC 61784 1��CP 3��1��PRoFIBUS DP�豸Ӧ����һ����PI�ṩ��
								//Ident Number����ldent��Number�涨���豸����ӦGSD�ļ���������ѭ����Ϊ
								//���ԡ�����IEC 61784 1��CP 3��2��PROFIBUS PA�豸Ӧ����֧��һ���й�
								//�ض�ldent��Number���ڱ���׼��6��4��1�ж������й��ض�Ident_Number�����
								//ĳ�豸����Ϊ�й��ض�Ident_Number������Ӧ������Ӧ�й�GSD�ļ����й���
								//�������⣬PROFIBUS PA�豸��֧���������ض�ldent_Number��
								//�й��ض���GSD�ļ���PI�ṩ���������ض���GSD�ļ����豸�������ṩ��
								//�û��ܹ�ͨ�����ò���IDENT_NUMBER_SELECTOR��ѡ����Ч��ldent_
								//Number�����Ident_Number���ı䣬��û����ѭ����Ϊ����(���磺������ݣ���
								//�ȡ���ǰ���ѽ��ܵ���̬���ݵ�)Ҳ�����ı䡣
								//���룺
								//0���й��ض�Ident��Number(PA�й�V3��x)(�ر�)
								//l���������ض�Ident��Number(PA�й�V3 x)(��ѡ)
								//2���������ض�Ident��Number(PA�й�V2��0)(��ѡ)
								//3��������豸���й��ض�Ident��Number(PA�й�V3��x)(��ѡ)
								//4��126���������й�ʹ��(������)
								//127����Ӧģʽ(�ر�)
								//128��255���������ض�(��ѡ)
	uint8_t HW_WRITE_PROTECTION;//ָ�����ܱ�Զ�̷����޸ĵ�д��������(���硣Ӳ���߻򱾵��û��ӿ�)��״̬��
								//����ֹ�޸��豸�Ĳ�����
								//0���ޱ���(�ر�)
								//1�������������ֶ�����(��ѡ)
								//�ܾ������в����ķ�ѭ��д����(д���ʱ��ܾ�)�������Ի���Ĳ���TAB��
								//ENTRY���Լ�����TARGET��MODE��0UT��OUT��D(����AO��DO��
								//Ч)���⡣
								//2�����������ֶ�����(��ѡ)
								//�ܾ������в����ķ�ѭ��д����(д���ʱ��ܾ�)�������Ի���Ĳ���TAB��
								//ENTRY���⡣
								//3��127������
								//128��255���������ض�
								//�豸��֧�ִ���1�ͣ������2��
								//���²������κ�д�������ƣ�
								//��TAB-ENTRY(���Ի���)��
								//��ACTUALPOST_READ��NUMBER(��־���ܿ�)��
								//���⣬���۴��ں���д����״̬���豸��Ӧ���ܲ�����Execution��Argument��
								//Call��REQ-PDU�������I&M����
	PA_FeatureDS68_t FEATURE;//ָʾ�豸����ʵ�ֵĿ�ѡ���ԣ��Լ��Ƿ�֧����Щ���Ե����
	uint8_t COND_STATUS_DIAG;//ָʾ�ɶ�״���������Ϊ������̬���豸ģʽ��
									//0���ṩ5��3��2�ж����״������ϣ�
									//l���ṩŨ��״���������Ϣ(��5��3��4)
									//2��255��PI����
	PA_DiagEventSwitch_t DIAG_EVENT_SWITCH;//���FEATURE��Enabled��Condensed��Status=1����ָʾ�����Ƹ��豸���豸�ض�
											//����¼��ķ�Ӧ��
											//������¼��ĵ�¼��������������̣��豸�ض���

	PTA_OP_CODE_e PTA_OP_CODE;
	PTA_STATUS_e  PTA_STATUS;
}_PACKED_ PB_BLOCK_t;

/*
 * 5��3��2����״��
 * 5��3��2��1״������
 */

/*
 * ��57״��(Status)�ֽڵı���
 */
#define STATUS_QUALITY_BAD 			(0x00<<6) //BAD����
#define STATUS_QUALITY_UNCERTAIN 	(0x01<<6)  //UNCERTAIN����ȷ��
#define STATUS_QUALITY_GOODNOCAS	(0x02<<6)  //GOOD(Non Cascade)����(�޼���)
#define STATUS_QUALITY_GOODCAS		(0x03<<6)  //OOD(Cascade)����(����)
//��Quality=BADʱ��Substatus�ĺ���
#define SSTATUS_BAD_NONE			(0x00<<2)  //non specific�����ض���
#define SSTATUS_BAD_CFGERROR		(0x01<<2)  //configuration error:��̬����
#define SSTATUS_BAD_NOCONNECT		(0x02<<2)  //not connected��δ����
#define SSTATUS_BAD_DEVFAIL  		(0x03<<2)  //device failure���豸����
#define SSTATUS_BAD_SENSFAIL  		(0x04<<2)  //sensor failure������������
#define SSTATUS_BAD_NOCOMMLAST  	(0x05<<2)  //no communication(last usable value)����ͨ��(��һ������ֵ)
#define SSTATUS_BAD_NOCOMMNO  		(0x06<<2)  //no communication(no usable value)����ͨ��(�޿���ֵ)
#define SSTATUS_BAD_OS		  		(0x07<<2)  //OUt Of service���Ƿ���
/* ��75 NAMUR NEl07�涨��״������ */
#define SSTATUS_BAD_MAINTALAR	  	(0x09<<2)  //����NAMuRNEl07�ĺ���Failure(F) ��PCS��DCS�е��÷�Failure 0x24�� 0x27  BAD-maintenance alarm��more disgnosis avaliabl
#define SSTATUS_BAD_FCHECKLOCOVR 	(0x0f<<2)  //����NAMuRNEl07�ĺ���Check(C) ��PCS��DCS�е��÷�Failure 0x3C�� 0x3F  BAD��function check��local override

//��Quality=UNCERTAINʱ��Substatus�ĺ���
#define SSTATUS_UNC_NONE		  	(0x10<<2)  //nod specific�����ض���
#define SSTATUS_UNC_LUV		  		(0x11<<2)  //last usable value)(LUV)����һ������ֵ(LUV)
#define SSTATUS_UNC_SUBVAL	  		(0x12<<2)  //substitute value�����ֵ
#define SSTATUS_UNC_INIVAL	  		(0x13<<2)  //initial value����ʼֵ
#define SSTATUS_UNC_SENCOVNOACC 	(0x14<<2)  //sensor conversion not accurate��������ת������ȷ
#define SSTATUS_UNC_UNITNOVALID 	(0x15<<2)  //engineering unit violation(unit not in the valid set)�����̵�λΥ��(��λ������Ч����)
#define SSTATUS_UNC_SUBNORMAL 		(0x16<<2)  //sub nomal�������������
#define SSTATUS_UNC_CFGERROR 		(0x17<<2)  //configuration error����̬����
#define SSTATUS_UNC_SIMULVAL 		(0x18<<2)  //simulated value������ֵ
#define SSTATUS_UNC_SENSCALIB 		(0x19<<2)  //sensor calibration��������У׼
/* ��75 NAMUR NEl07�涨��״������ */
#define SSTATUS_UNC_MAINTALAR	  	(0x1E<<2)  //����NAMuRNEl07�ĺ���Out of Specification(F) ��PCS��DCS�е��÷�Uncertain 0x78��0x7B UNCERTAIN��process related��no maitenance(��ֵ����5��3��4��2��2��2)

//��Quality=GOOD(Non Cascade)ʱ��Substatus�ĺ���
#define SSTATUS_NGOOD_OK 			(0x20<<2)  //ok
#define SSTATUS_NGOOD_UPDATE		(0x21<<2)  //update event�������¼�
#define SSTATUS_NGOOD_ACTADVALARM	(0x22<<2)  //active advisory alarm����Ч�ľ��䱨��
#define SSTATUS_NGOOD_ACTCRIALARM	(0x23<<2)  //acitve critical alarm����Ч�Ľ�������
#define SSTATUS_NGOOD_UNACKUPDATE	(0x24<<2)  //unacknowledged update event����ȷ�ϵĸ����¼�
#define SSTATUS_NGOOD_UNACKADVALA	(0x25<<2)  //unacknowledged advisory alarm��δȷ�ϵľ��䱨��
#define SSTATUS_NGOOD_UNACKCRIALA	(0x26<<2)  //unacknowledged critical alarm��δȷ�ϵĽ�������
#define SSTATUS_NGOOD_INIFAILSAFE	(0x28<<2)  //initiate fail safe���������ϰ�ȫ
#define SSTATUS_NGOOD_MAINREQ		(0x29<<2)  //maintenance required����Ҫά��


//��Quality=GOOD(Cascade)ʱ��Substatus�ĺ���
#define SSTATUS_CGOOD_OK 			(0x30<<2)  //ok
#define SSTATUS_CGOOD_INITACK		(0x31<<2)  //initialization acknowledged����ȷ�ϵĳ�ʼ��
#define SSTATUS_CGOOD_INITREQ 		(0x33<<2)  //initialization request����ʼ������
#define SSTATUS_CGOOD_NOINV			(0x33<<2)  //not invited����������
#define SSTATUS_CGOOD_NOSELECT		(0x35<<2)  //do not select����ѡ��
#define SSTATUS_CGOOD_LOV			(0x36<<2)  //local overide�����س���
#define SSTATUS_CGOOD_INIFAILSAFE	(0x38<<2)  //initiate fail safe���������ϰ�ȫ

//Limit���صĺ���
#define STATUS_LIMIT_OK				(0x00<<0)  //ok
#define STATUS_LIMIT_LOW			(0x01<<0)  //low limited��������
#define STATUS_LIMIT_HIGH			(0x02<<0)  //high limited��������
#define STATUS_LIMIT_CONST			(0x03<<0)  //constant������


//��60��-���������ϣ�������(BAD-sensor failure��low limited)
#define STATUS_BAD_SENSFAIL_LOLIM  (SSTATUS_BAD_SENSFAIL|STATUS_LIMIT_LOW)
//��61��-���������ϣ�������(BAD-sensor failure��high limited)
#define STATUS_BAD_SENSFAIL_HILIM  (SSTATUS_BAD_SENSFAIL|STATUS_LIMIT_HIGH)
//��62��(�޼���)-��Ч�ľ��䱨����������(GOOD(Non Cascade)һactive advisory alarm��low limited)
#define STATUS_NGOOD_ACTADVALARM_LOLIM  (SSTATUS_NGOOD_ACTADVALARM|STATUS_LIMIT_LOW)
//��63��(�޼���)-��Ч�ľ��䱨����������(GOOD(Non Cascade)һactive advisory alarm��high limited)
#define STATUS_NGOOD_ACTADVALARM_HILIM  (SSTATUS_NGOOD_ACTADVALARM|STATUS_LIMIT_HIGH)
//��64��(�޼���)-��Ч�Ľ���������������(GOOD(Non Cascade)һactive critical alarm��low limited)
#define STATUS_NGOOD_ACTCRIALARM_LOLIM  (SSTATUS_NGOOD_ACTCRIALARM|STATUS_LIMIT_LOW)
//��65��(�޼���)-��Ч�Ľ���������������(GOOD(Non Cascade)һactive critical alarm��high limited)
#define STATUS_NGOOD_ACTCRIALARM_HILIM  (SSTATUS_NGOOD_ACTCRIALARM|STATUS_LIMIT_HIGH)
//��66��(�޼���)-�����¼�(GOOD(Non Cascade)һupdate event)
#define STATUS_NGOOD_UPDATE_OK     (SSTATUS_NGOOD_UPDATE|STATUS_LIMIT_OK)
#define STATUS_NGOOD_UPDATE_LOLIM  (SSTATUS_NGOOD_UPDATE|STATUS_LIMIT_LOW)
#define STATUS_NGOOD_UPDATE_HILIM  (SSTATUS_NGOOD_UPDATE|STATUS_LIMIT_HIGH)
#define STATUS_NGOOD_UPDATE_CONST  (SSTATUS_NGOOD_UPDATE|STATUS_LIMIT_CONST)


/* ��75 NAMUR NEl07�涨��״������ */
#define STATUS_MAINT_HILIM			(SSTATUS_NGOOD_ACTADVALARM|STATUS_LIMIT_HIGH)  //����NAMuRNEl07�ĺ���Maintenance(M) ��PCS��DCS�е��÷�GOOD 0xA4��0xAB GOOD-maintenance required��demanded (��ֵ����5��3��4��2��2��2)
#define STATUS_MAINT_HIHILIM		(SSTATUS_NGOOD_ACTCRIALARM|STATUS_LIMIT_HIGH)   //����NAMuRNEl07�ĺ���Maintenance(M) ��PCS��DCS�е��÷�GOOD 0xA4��0xAB GOOD-maintenance required��demanded (��ֵ����5��3��4��2��2��2)
#define STATUS_MAINT_LOLIM			(SSTATUS_NGOOD_ACTADVALARM|STATUS_LIMIT_LOW)   //����NAMuRNEl07�ĺ���Maintenance(M) ��PCS��DCS�е��÷�GOOD 0xA4��0xAB GOOD-maintenance required��demanded (��ֵ����5��3��4��2��2��2)
#define STATUS_MAINT_LOLOLIM		(SSTATUS_NGOOD_ACTCRIALARM|STATUS_LIMIT_LOW)   //����NAMuRNEl07�ĺ���Maintenance(M) ��PCS��DCS�е��÷�GOOD 0xA4��0xAB GOOD-maintenance required��demanded (��ֵ����5��3��4��2��2��2)

/* ��76��״��Ϊ��(GOOD)ʱ���ܼ�飯�����¼�(Limit Checks��Update Events)�ı��� */
#define STATUSG_GOOD_OK_OK			(SSTATUS_NGOOD_OK | STATUS_LIMIT_OK) 		//����NAMuRNEl07�ĺ���Good(G) ��PCS��DCS�е��÷�GOOD 0x80��0x80 Good OK
#define STATUSG_GOOD_OK_LOW			(SSTATUS_NGOOD_OK | STATUS_LIMIT_LOW) 		//����NAMuRNEl07�ĺ���Good(G) ��PCS��DCS�е��÷�GOOD 0x80��0x80 Good OK
#define STATUSG_GOOD_OK_HIG			(SSTATUS_NGOOD_OK | STATUS_LIMIT_HIGH) 		//����NAMuRNEl07�ĺ���Good(G) ��PCS��DCS�е��÷�GOOD 0x80��0x80 Good OK
#define STATUSG_GOOD_OK_CST			(SSTATUS_NGOOD_OK | STATUS_LIMIT_CONST) 		//����NAMuRNEl07�ĺ���Good(G) ��PCS��DCS�е��÷�GOOD 0x80��0x80 Good OK
#define STATUSG_GOOD_UPDATE_OK		(SSTATUS_NGOOD_UPDATE | STATUS_LIMIT_OK)  	//����NAMuRNEl07�ĺ���Good(G) ��PCS��DCS�е��÷�GOOD 0x84��0x87 GOOD uPate event
#define STATUSG_GOOD_UPDATE_LOW		(SSTATUS_NGOOD_UPDATE | STATUS_LIMIT_LOW) 	 //����NAMuRNEl07�ĺ���Good(G) ��PCS��DCS�е��÷�GOOD 0x84��0x87 GOOD uPate event
#define STATUSG_GOOD_UPDATE_HIG		(SSTATUS_NGOOD_UPDATE | STATUS_LIMIT_HIGH) 	 //����NAMuRNEl07�ĺ���Good(G) ��PCS��DCS�е��÷�GOOD 0x84��0x87 GOOD uPate event
#define STATUSG_GOOD_UPDATE_CST		(SSTATUS_NGOOD_UPDATE | STATUS_LIMIT_CONST)  //����NAMuRNEl07�ĺ���Good(G) ��PCS��DCS�е��÷�GOOD 0x84��0x87 GOOD uPate event
#define STATUSG_GOOD_ADVALARLOLIM	(SSTATUS_NGOOD_ACTADVALARM | STATUS_LIMIT_LOW)  //����NAMuRNEl07�ĺ���Good(G) ��PCS��DCS�е��÷�GOOD 0x89��0x89 GOOD-advisory alarm��low limit
#define STATUSG_GOOD_ADVALARHILIM	(SSTATUS_NGOOD_ACTADVALARM | STATUS_LIMIT_HIGH)  //����NAMuRNEl07�ĺ���Good(G) ��PCS��DCS�е��÷�GOOD 0x8A��0x8A GOOD-advisory alarm��high limit
#define STATUSG_GOOD_CRIALARLOLIM	(SSTATUS_NGOOD_ACTCRIALARM | STATUS_LIMIT_LOW)  //����NAMuRNEl07�ĺ���Good(G) ��PCS��DCS�е��÷�GOOD 0x8D��0x8D GOOD-cirtical alarm��low limit
#define STATUSG_GOOD_CRIALARHILIM	(SSTATUS_NGOOD_ACTCRIALARM | STATUS_LIMIT_HIGH)  //����NAMuRNEl07�ĺ���Good(G) ��PCS��DCS�е��÷�GOOD 0x8E��0x8E GOOD-cirtical alarm��high limit

/* ��77������ϸ��Ϣ��Ũ��״���ı���*/
#define STATUSF_BAD_NONE			    	(SSTATUS_BAD_NONE | STATUS_LIMIT_OK) 		//����NAMuRNEl07�ĺ���Failure(F) ��PCS��DCS�е��÷�Failure ά��վFailure 0x00��0x00 BAD-non specific
#define STATUSF_BAD_PASSIV				(0x20 | STATUS_LIMIT_CONST) 				//����NAMuRNEl07�ĺ���Failure(F) ��PCS��DCS�е��÷�Passivated ά��վGOOD 0x23��0x23 BAD-passivated(��Ͼ�������ֹ��
#define STATUSF_BAD_MAINTALAR_OK			(SSTATUS_BAD_MAINTALAR | STATUS_LIMIT_OK) 	//����NAMuRNEl07�ĺ���Failure(F) ��PCS��DCS�е��÷�Failure ά��վMaintenance alarm 0x24��0x27 BAD-maintenance alarm��more diagnosls available
#define STATUSF_BAD_MAINTALAR_LOW		(SSTATUS_BAD_MAINTALAR | STATUS_LIMIT_LOW) 	//����NAMuRNEl07�ĺ���Failure(F) ��PCS��DCS�е��÷�Failure ά��վMaintenance alarm 0x24��0x27 BAD-maintenance alarm��more diagnosls available
#define STATUSF_BAD_MAINTALAR_HIG		(SSTATUS_BAD_MAINTALAR | STATUS_LIMIT_HIG) 	//����NAMuRNEl07�ĺ���Failure(F) ��PCS��DCS�е��÷�Failure ά��վMaintenance alarm 0x24��0x27 BAD-maintenance alarm��more diagnosls available
#define STATUSF_BAD_MAINTALAR_CST		(SSTATUS_BAD_MAINTALAR | STATUS_LIMIT_CONST) 	//����NAMuRNEl07�ĺ���Failure(F) ��PCS��DCS�е��÷�Failure ά��վMaintenance alarm 0x24��0x27 BAD-maintenance alarm��more diagnosls available

#define STATUSF_BAD_PROCESS_OK			(0x28 | STATUS_LIMIT_OK) 	//����NAMuRNEl07�ĺ���Failure(F) ��PCS��DCS�е��÷�Failure ά��վGOOD  0x28��0x2B BAD-process related-no maitenace
#define STATUSF_BAD_PROCESS_LOW			(0x28 | STATUS_LIMIT_LOW) 	//����NAMuRNEl07�ĺ���Failure(F) ��PCS��DCS�е��÷�Failure ά��վGOOD  0x28��0x2B BAD-process related-no maitenace
#define STATUSF_BAD_PROCESS_HIG			(0x28 | STATUS_LIMIT_HIG) 	//����NAMuRNEl07�ĺ���Failure(F) ��PCS��DCS�е��÷�Failure ά��վGOOD  0x28��0x2B BAD-process related-no maitenace
#define STATUSF_BAD_PROCESS_CST			(0x28 | STATUS_LIMIT_CONST) 	//����NAMuRNEl07�ĺ���Failure(F) ��PCS��DCS�е��÷�Failure ά��վGOOD  0x28��0x2B BAD-process related-no maitenace

#define STATUSC_BAD_FCHECKLOCOVR_OK			(SSTATUS_BAD_FCHECKLOCOVR | STATUS_LIMIT_OK) 	//����NAMuRNEl07�ĺ���Check(C) ��PCS��DCS�е��÷�Function Check ά��վFunction Check  0x3c��0x3f BAD-function eheek��loeal override��value not usable
#define STATUSC_BAD_FCHECKLOCOVR_LOW		(SSTATUS_BAD_FCHECKLOCOVR | STATUS_LIMIT_LOW) 	//����NAMuRNEl07�ĺ���Check(C) ��PCS��DCS�е��÷�Function Check ά��վFunction Check  0x3c��0x3f BAD-function eheek��loeal override��value not usable
#define STATUSC_BAD_FCHECKLOCOVR_HIG		(SSTATUS_BAD_FCHECKLOCOVR | STATUS_LIMIT_HIG) 	//����NAMuRNEl07�ĺ���Check(C) ��PCS��DCS�е��÷�Function Check ά��վFunction Check  0x3c��0x3f BAD-function eheek��loeal override��value not usable
#define STATUSC_BAD_FCHECKLOCOVR_CST		(SSTATUS_BAD_FCHECKLOCOVR | STATUS_LIMIT_CONST) 	//����NAMuRNEl07�ĺ���Check(C) ��PCS��DCS�е��÷�Function Check ά��վFunction Check  0x3c��0x3f BAD-function eheek��loeal override��value not usable

#define STATUSF_UNC_UNCSUBSTI_CST			(SSTATUS_UNC_SUBVAL | STATUS_LIMIT_CONST) 	//����NAMuRNEl07�ĺ���Failure(F) ��PCS��DCS�е��÷�Uncertain ά��վMaintenance Alarm  0x4b��0x4b UNCERTAIN substitute set
#define STATUSF_UNC_UNCINIT_CST				(SSTATUS_UNC_INIVAL | STATUS_LIMIT_CONST) 	//����NAMuRNEl07�ĺ���Failure(F) ��PCS��DCS�е��÷�Uncertain ά��վGOOD  0x4f��0x4f UNCERTAIN��initial value

#define STATUSM_UNC_MAINTDEMAD_OK			(0x68 | STATUS_LIMIT_OK) 	//����NAMuRNEl07�ĺ���Maintenance(M) ��PCS��DCS�е��÷�Uncertain ά��վMaintenance demanded  0x68��0x6b UNCERlAIN��maintenance demanded(��ֵ����5��3��4��2��2��2)
#define STATUSM_UNC_MAINTDEMAD_LOW			(0x68 | STATUS_LIMIT_LOW) 	//����NAMuRNEl07�ĺ���Maintenance(M) ��PCS��DCS�е��÷�Uncertain ά��վMaintenance demanded  0x68��0x6b UNCERlAIN��maintenance demanded(��ֵ����5��3��4��2��2��2)
#define STATUSM_UNC_MAINTDEMAD_HIG			(0x68 | STATUS_LIMIT_HIG) 	//����NAMuRNEl07�ĺ���Maintenance(M) ��PCS��DCS�е��÷�Uncertain ά��վMaintenance demanded  0x68��0x6b UNCERlAIN��maintenance demanded(��ֵ����5��3��4��2��2��2)
#define STATUSM_UNC_MAINTDEMAD_CST			(0x68 | STATUS_LIMIT_CONST) 	//����NAMuRNEl07�ĺ���Maintenance(M) ��PCS��DCS�е��÷�Uncertain ά��վMaintenance demanded  0x68��0x6b UNCERlAIN��maintenance demanded(��ֵ����5��3��4��2��2��2)

#define STATUSC_UNC_UNCSIMULST_CST			(0x70 | STATUS_LIMIT_CONST) 	//����NAMuRNEl07�ĺ���CHECK(C) ��PCS��DCS�е��÷�Uncertain ά��վfunction check  0x73��0x73 UNCERTAIN��simulated Value��start

#define STATUSC_UNC_UNCSIMULED_OK			(0x74 | STATUS_LIMIT_OK) 	//����NAMuRNEl07�ĺ���CHECK(C) ��PCS��DCS�е��÷�Uncertain ά��վGOOD  0x74��0x77 UNCERTAIN��simulated Value��end(��ֵ����5��3��4��2��2��2)
#define STATUSC_UNC_UNCSIMULED_LOW			(0x74 | STATUS_LIMIT_LOW) 	//����NAMuRNEl07�ĺ���CHECK(C) ��PCS��DCS�е��÷�Uncertain ά��վGOOD  0x74��0x77 UNCERTAIN��simulated Value��end(��ֵ����5��3��4��2��2��2)
#define STATUSC_UNC_UNCSIMULED_HIG			(0x74 | STATUS_LIMIT_HIG) 	//����NAMuRNEl07�ĺ���CHECK(C) ��PCS��DCS�е��÷�Uncertain ά��վGOOD  0x74��0x77 UNCERTAIN��simulated Value��end(��ֵ����5��3��4��2��2��2)
#define STATUSC_UNC_UNCSIMULED_CST			(0x74 | STATUS_LIMIT_CONST) //����NAMuRNEl07�ĺ���CHECK(C) ��PCS��DCS�е��÷�Uncertain ά��վGOOD  0x74��0x77 UNCERTAIN��simulated Value��end(��ֵ����5��3��4��2��2��2)

#define STATUSS_UNC_PROCESS_OK			(0x78 | STATUS_LIMIT_OK) 	//����NAMuRNEl07�ĺ���Out of specification(S) ��PCS��DCS�е��÷�Uncertain ά��վGOOD  0x78��0x7B UNCERTAIN��process related��no maitenace(��ֵ����5��3��4��2��2��2)
#define STATUSS_UNC_PROCESS_LOW			(0x78 | STATUS_LIMIT_LOW) 	//����NAMuRNEl07�ĺ���Out of specification(S) ��PCS��DCS�е��÷�Uncertain ά��վGOOD  0x78��0x7B UNCERTAIN��process related��no maitenace(��ֵ����5��3��4��2��2��2)
#define STATUSS_UNC_PROCESS_HIG			(0x78 | STATUS_LIMIT_HIG) 	//����NAMuRNEl07�ĺ���Out of specification(S) ��PCS��DCS�е��÷�Uncertain ά��վGOOD  0x78��0x7B UNCERTAIN��process related��no maitenace(��ֵ����5��3��4��2��2��2)
#define STATUSS_UNC_PROCESS_CST			(0x78 | STATUS_LIMIT_CONST) //����NAMuRNEl07�ĺ���Out of specification(S) ��PCS��DCS�е��÷�Uncertain ά��վGOOD  0x78��0x7B UNCERTAIN��process related��no maitenace(��ֵ����5��3��4��2��2��2)

#define STATUSG_GOOD_INIFAILSAFE_OK		(SSTATUS_NGOOD_INIFAILSAFE | STATUS_LIMIT_LOW)   //����NAMuRNEl07�ĺ���Good(G) ��PCS��DCS�е��÷�GOOD ά��վGOOD 0xA0��0xA3 GOOD-initiate fail safe(�������FB�����˲����������������޹�)(��ֵ����5��3��4��2��2 2)
#define STATUSG_GOOD_INIFAILSAFE_LOW	(SSTATUS_NGOOD_INIFAILSAFE | STATUS_LIMIT_HIGH)  //����NAMuRNEl07�ĺ���Good(G) ��PCS��DCS�е��÷�GOOD ά��վGOOD 0xA0��0xA3 GOOD-initiate fail safe(�������FB�����˲����������������޹�)(��ֵ����5��3��4��2��2 2)
#define STATUSG_GOOD_INIFAILSAFE_HIG	(SSTATUS_NGOOD_INIFAILSAFE | STATUS_LIMIT_LOW)   //����NAMuRNEl07�ĺ���Good(G) ��PCS��DCS�е��÷�GOOD ά��վGOOD 0xA0��0xA3 GOOD-initiate fail safe(�������FB�����˲����������������޹�)(��ֵ����5��3��4��2��2 2)
#define STATUSG_GOOD_INIFAILSAFE_CST	(SSTATUS_NGOOD_INIFAILSAFE | STATUS_LIMIT_HIGH)  //����NAMuRNEl07�ĺ���Good(G) ��PCS��DCS�е��÷�GOOD ά��վGOOD 0xA0��0xA3 GOOD-initiate fail safe(�������FB�����˲����������������޹�)(��ֵ����5��3��4��2��2 2)

#define STATUSM_GOOD_MAINREQ_OK		(SSTATUS_NGOOD_MAINREQ | STATUS_LIMIT_LOW)   //����NAMuRNEl07�ĺ���Maintenance(M) ��PCS��DCS�е��÷�GOOD ά��վMaintenance required 0xA4��0xA7 GOOD-maintenance required(��ֵ����5��3��4��2��2 2)
#define STATUSM_GOOD_MAINREQ_LOW	(SSTATUS_NGOOD_MAINREQ | STATUS_LIMIT_HIGH)  //����NAMuRNEl07�ĺ���Maintenance(M) ��PCS��DCS�е��÷�GOOD ά��վMaintenance required 0xA4��0xA7 GOOD-maintenance required(��ֵ����5��3��4��2��2 2)
#define STATUSM_GOOD_MAINREQ_HIG	(SSTATUS_NGOOD_MAINREQ | STATUS_LIMIT_LOW)   //����NAMuRNEl07�ĺ���Maintenance(M) ��PCS��DCS�е��÷�GOOD ά��վMaintenance required 0xA4��0xA7 GOOD-maintenance required(��ֵ����5��3��4��2��2 2)
#define STATUSM_GOOD_MAINREQ_CST	(SSTATUS_NGOOD_MAINREQ | STATUS_LIMIT_HIGH)  //����NAMuRNEl07�ĺ���Maintenance(M) ��PCS��DCS�е��÷�GOOD ά��վMaintenance required 0xA4��0xA7 GOOD-maintenance required(��ֵ����5��3��4��2��2 2)

#define STATUSM_GOOD_MAINDEMA_OK	(0xA8 | STATUS_LIMIT_LOW)   //����NAMuRNEl07�ĺ���Maintenance(M) ��PCS��DCS�е��÷�GOOD ά��վMaintenance demanded 0xA8��0xAB GOOD-maintenance demanded(��ֵ����5��3��4��2��2 2)
#define STATUSM_GOOD_MAINDEMA_LOW	(0xA8 | STATUS_LIMIT_HIGH)  //����NAMuRNEl07�ĺ���Maintenance(M) ��PCS��DCS�е��÷�GOOD ά��վMaintenance demanded 0xA8��0xAB GOOD-maintenance demanded(��ֵ����5��3��4��2��2 2)
#define STATUSM_GOOD_MAINDEMA_HIG	(0xA8 | STATUS_LIMIT_LOW)   //����NAMuRNEl07�ĺ���Maintenance(M) ��PCS��DCS�е��÷�GOOD ά��վMaintenance demanded 0xA8��0xAB GOOD-maintenance demanded(��ֵ����5��3��4��2��2 2)
#define STATUSM_GOOD_MAINDEMA_CST	(0xA8 | STATUS_LIMIT_HIGH)  //����NAMuRNEl07�ĺ���Maintenance(M) ��PCS��DCS�е��÷�GOOD ά��վMaintenance demanded 0xA8��0xAB GOOD-maintenance demanded(��ֵ����5��3��4��2��2 2)

#define STATUSG_GOOD_FUNCHK_OK		(0xBC | STATUS_LIMIT_LOW)   //����NAMuRNEl07�ĺ���GOOD(G) ��PCS��DCS�е��÷�GOOD ά��վFunction Check 0xBC��0xBF GOOD-function check(��ֵ����5��3��4��2��2 2)
#define STATUSG_GOOD_FUNCHK_LOW		(0xBC | STATUS_LIMIT_HIGH)  //����NAMuRNEl07�ĺ���GOOD(G) ��PCS��DCS�е��÷�GOOD ά��վFunction Check 0xBC��0xBF GOOD-function check(��ֵ����5��3��4��2��2 2)
#define STATUSG_GOOD_FUNCHK_HIG		(0xBC | STATUS_LIMIT_LOW)   //����NAMuRNEl07�ĺ���GOOD(G) ��PCS��DCS�е��÷�GOOD ά��վFunction Check 0xBC��0xBF GOOD-function check(��ֵ����5��3��4��2��2 2)
#define STATUSG_GOOD_FUNCHK_CST		(0xBC | STATUS_LIMIT_HIGH)  //����NAMuRNEl07�ĺ���GOOD(G) ��PCS��DCS�е��÷�GOOD ά��վFunction Check 0xBC��0xBF GOOD-function check(��ֵ����5��3��4��2��2 2)




/*
 * 5��3��3�������
 * ��69��������DIAGNOSIS�ı���
 */

//DIAGNOSIS�İ�λλ��1
#define B0_DIA_HW_ELECTR  		0x01  // ���ӷ����Ӳ������
#define B0_DIA_HW_MECH    		0x02  // ��е�����Ӳ������
#define B0_DIA_TEMP_MOTOR   	0x04  // ����¶ȹ���
#define B0_DIA_TEMP_ELECTR	    0x08  // ���������¶ȹ���
#define B0_DIA_MEM_CHKSUM    	0x10  // �洢������
#define B0_DIA_MEASUREMENT	    0x20  // ����ʧ��
#define B0_DIA_NOT_INIT		    0x40  // �豸δ��ʼ��(�ް�У׼)
#define B0_DIA_INIT_ERR		    0x80  // ��У׼ʧ��
//DIAGNOSIS�İ�λλ��2
#define B1_DIA_ZERO_ERR  		0x01  // ������(��ֵλ��)
#define B1_DIA_SUPPLY    		0x02  // ��Դ����(�綯������)
#define B1_DIA_CONF_INVAL		0x04  // ��̬��Ч
#define B1_DIA_WARMSTART   		0x08  // �ϵ�����ã���ִ��FACTORY_RESET=2506�󽫴˱�����λ
#define B1_DIA_COLDSTART	    0x10  // ִ��FACTORY_RESET=1�󽫴˱�����λ
#define B1_DIA_MAINTAINANCE    	0x20  // ��Ҫά��
#define B1_DIA_CHARACT		    0x40  // ���Ի���Ч
#define B1_IDENT_NUMBER_VIOLATION    0x80  // ��������е�ѭ�����ݴ����Ident_Number��PB����IDENT_NUMBER_SELECTOR��ֵ��һ
										//�£�������Ϊ1�����IDENT_NUMBER_SELECTOR=127(��Ӧģʽ)����DIAGNOSIS�ı��ر����������λ
//DIAGNOSIS�İ�λλ��3  ����
#define B2_DIA_MAINTENANCE_ALARM  		0x01  // �豸�����׸�������
#define B2_DIA_MAINTENANCE_DEMANDED   	0x02  // ����ά��
#define B2_DIA_FUNCTION_CHECKL			0x04  // �豸���ڹ��ܼ��ģʽ�����ڷ��棬���ڱ��ؿ����£�����ά��
#define B2_DIA_INV_PRO_COND   			0x08  // ������������������Ч��ֵ�����ֵ������ΪUNCERTAIN-Process related��no maintenance��BAD-Processrelated��no maintenance��������λ

//DIAGNOSIS�İ�λλ��4
#define B3_EXTENSION_AVAILABLE  		0x80  // ��DIAGNOSIS��EXTENSION�У��и���������Ϣ����



/*
 * 5��3��4Ũ��״�������
 */

/*
 * 5��3��4��3��3��2 DIAGNOSIS
 * �����������¼�����DIAGNOSIS�����е�һ�����ر��뱻��λ�����ڵ�������¼�����һ����
����λ����ͬ������¼��ܵ��²�ͬ����ϱ�����λ������ڲ���DIAGNOSIS_EXTENSION�д��ڸ�
����õ���Ϣ(����һ��������λ)����Ӧ������EXTENSION_AVAILABLE������λ(����82)��
 */
//DIAGNOSIS�İ�λλ��0
//����B0_XX
//DIAGNOSIS�İ�λλ��1

#define CB1_DIA_WARMSTART	    0x08  // �ϵ�����ִ��FACTORY_RESET = 2506��Ӧ����λ,A,Ext_Diag=0
#define CB1_DIA_COLDSTART    	0x10  // ��ִ��FACTORY_RESET=1��Ӧ����λ,A,Ext_Diag=0
#define CB1_DIA_MAINTENANCE	    0x20  // ��Ҫά��,R,Ext_Diag=0

#define CB1_IDENT_NUMBER_VIOLATION	  0x80  // ��������е�ѭ�����ݴ����ldent��
													//Number����������IDENT_NUMBER_SELECTOR��ֵ��һ�£���Ӧ��
													//λ�����IDENT_NUMBER_SELECTOR=127(��Ӧģʽ)����DIAGNOSIS
													//�ı���IDENT_NUMBER_VIOLATION�����㣯����λ,R,Ext_Diag=0
//DIAGNOSIS�İ�λλ��2
#define CB2_DIA_MAINTENANCE_ALARM  		0x01  // �豸�����׸�������,R,Ext_Diag=1
#define CB2_DIA_MAINTENANCE_DEMANDED   	0x02  // ����ά��,R,Ext_Diag=0
#define CB2_DIA_FUNCTION_CHECK		0x04  // �豸���ڹ��ܼ��ģʽ�����ڷ��棬���ڱ��ؿ����£�����ά��,,R,Ext_Diag=0
#define CB2_DIA_INV_PRO_COND   		0x08  // ������������������Ч��ֵ�����ֵ������ΪUNCERTAIN-Process related��
										  //no maintenance��BAD-Process related��no maintenance��������λ,R,Ext_Diag=0

//DIAGNOSIS�İ�λλ��3
#define CB3_EXTENSION_AVAILABLE  		0x80  // ��DIAGNOSIS��EXTENSION�У��и���������Ϣ����

// Rָʾ��ֻҪ�������Ϣ��ԭ����ڣ���ָʾ��һֱ������Ч��
// Aָʾ����10 s���Զ���λ��

/*
 *
 * 5��4��������
 * ��������(parameter transaction)�����豸�����������ݼ���һ��д�������С��û����������
 * ���ģ����ܴ�����ͼ����������������ڴ����ڼ������Ͳ����ĸ���ȡ��������������ɴ����豸��
 * ���в������һ���Ӽ����ڴ��������ڼ䣬�������豸�������һ���Լ�顣
 * ����豸����д����ģʽ�£�����豸�ܾ������������ͨ�������Ӳ����ʽʵ�ֵ�д������
 * �ƻᵼ�¸þܾ���Ϊ�ķ�����
 * �ڿ�ʼ���������д����״̬�ĸı�(�ܱ������ܱ���)����Ӱ�촦��ļ������С�����ͨ��
 * �����Ӳ����ʽ�����д�������ƣ��ö��嶼��Ч��
 *
 */


typedef struct {
  uint16_t TB_ID;
  uint16_t relative_index;
}_PACKED_ CHANNEL_t;


typedef struct {
	IDX_STDParameters_t Std;

	__Def_ParameterAttr(PA_BatchDS67_t*, pBATCH );//���ڱ�׼�������֣��˲���ּ�����ڷ���GB��T 19892��1��2005����Ӧ�á������ܿ���д˲������ڹ��ܿ���
							//����Ҫ�㷨���ڷֲ�ʽ�ֳ�����ϵͳ��BATCH�����Ǳر��ģ�����ʶ�����õĺͿ��õ�
							//ͨ�������⣮�ھ���������»�ʶ��ǰ���������5 2��3��19
	__Def_ParameterAttr(void*,pResv9);
	__Def_ParameterAttr(PA_ValueStatusFP101_t*, pOut);
		/*
		 * ���Զ�(AUTO)ģʽ�£����ܿ����OUT�������������ض��Ļ���̬�����Ĺ��̵�λ�����ĵ�ǰ����ֵ�Լ�����״̬��
		 * ���ֶ�ģʽ(MANMODE)�����ܿ����OUT�����ɲ���Ա�����õ�ֵ��״̬��
		 */
	__Def_ParameterAttr(float*, pPv_scale);//��һ����ֵ��100����EUֵ(PV_SCALE,EU_at_100��)���ڶ�����ֵ��0%��EUֵ(PV_SCALE,EU_at_0%).
		/*
		 *  ʹ�øߡ��ͱ��ֵ�����̱���ת���ɰٷ�����PV_SCALE�ĸߡ��ͱ��ֵ�Ĺ��̵�λ������̬��ת����(ͨ��Channel����������̬)��PV��UNITֱ���йء�
		*  PV_SCALE�ĸߡ��ͱ��ֵ�Զ����������ת������PV_UNIT�ĸı���ı䣬��ת����PV_UNIT�ĸı䲢������Al��OUT���Ŷ���
		 *   Ҳ���ܴ���������������磬��ϴ����������Ҫ���Ŷ�
		 */
	__Def_ParameterAttr(PA_ScalingDS36_t*, pOut_scale);
		/*
		 * ���̱����ı�ȡ�
		 * OUT_SCALE ���ܿ����OUT_SCALE��������ֵ������ֵ����Ч��Χ�����̱������̵�λ�Ĵ���
		 * �ţ��Լ�С�����ұߵ���Ч����λ��
		 */
	__Def_ParameterAttr(uint8_t*, pLinType);
		/*
		 * ���Ի����͡���ϸ���ݼ���50
		 */
	__Def_ParameterAttr(CHANNEL_t*, pChannel);
		/*
		 * ����Чת��������ã���ת����Ϊ���ܿ��ṩ����ֵ�����5��1��5��ͨ��Ҫ���塱
		 */
	__Def_ParameterAttr(void*,pResv15);
	__Def_ParameterAttr(float*, pPv_ftime);
		/*
		 * ���̱������˲�ʱ�䡣
		 * ���ܿ����PV_FTIME���������ܿ�����ﵽ63.21%ʱ������ʱ�䣬Ϊʱ�䳣������
		 * ������ֵԾ��Ľ��(PTI�˲���)���ò����Ĺ��̵�λΪs��
		 */

	__Def_ParameterAttr(uint8_t*, pFsafe_type);//����˲���δ��ʵ�֣���AIFB����Ϊ��ͬFSAFE_TYPE=1��
		/*
			  ���嵱��⵽����ʱ�豸�ķ�Ӧ���������ʵ��ģʽ������AUTO��
			0��ֵFSAFE_VALUE������OUT
			״��=UNCERTAIN��substitute value b��
			1��ʹ����һ�δ洢����ЧOUTֵ
			״��=UNCERTAIN-last usable value
			(���û����Чֵ���ã���Ӧ��ʹ��UNCERTAIN��initial value���ڴ�����£�OUTֵ=
			��ʼֵ)��
			2��OUT���д���ļ���ֵ��״��
			״��=BAD_(a)��(b)
			a,������ġ�
		    b,�˴�ʹ�þ���״�����壬��ʹ��Ũ��״���������83��
		 */
	__Def_ParameterAttr(float*, pFsafe_value);
		/*
		 * ����⵽�������򴫸�������Ԫ������ʱ������OUT������ȱʡֵ���ò����ĵ�λ��
		 * OUT�����ĵ�λ��ͬ
		 */
	__Def_ParameterAttr(float*, pAlarm_hys); //ȱʡֵ 0.5
		/*
		 * �ͺ�(Hysteresis)
			��PROFIBUS PA�������淶�ķ�Χ�ڣ���һЩ�������ڼ��ӿɵ���ֵ�ĳ���(ƫ����
			ֵ����)��Ҳ��һ�����̱�����ֵǡǡ����ֵ��ͬ�����Ҹñ���Χ����ֵ���²������ɴ�
			���ɵ��¶�γ��ޡ�
			�⽫���������Ϣ����ˣ�Ӧ�п��ܽ��ڳ����ɵ����ͺ�֮��Ŵ�����Ϣ������������
			Ϣ���������ǿɵ��ġ��ͺ�ֵ�̶�ΪALARM_HYS������ڲ���HI_HI_LIM��HI_LIM��LO_LIM��LO_LO_LIM��һ���ġ��ͺ󱻱�ʾΪ��xx_LlMΪ���̵�λ�����ĵ�
			�����޺͸������޵�ֵ
		 */
	__Def_ParameterAttr(void*,pResv20);
	__Def_ParameterAttr(float*, pHihilim);
		/*
		 * �����ޱ���(upper limit of alarm)ֵ
			�������ı������ڻ����HI��HI��LIMֵ����OUT��״���ֽڵ�Limit������Ϊ
			��high limited����FB����ALARM��SUM�е�HLHLAlarm���ر����Ϊ1���ò�����
			��λ��OUT�����ĵ�λ��ͬ
		 */
	__Def_ParameterAttr(void*,pResv22);
	__Def_ParameterAttr(float*, pHilim);
		/*
		 * ���ޱ���(upper limit of warning)ֵ
			�������ı������ڻ����HI_l��IMֵ����OUT��״���ֽڵ�Limit������Ϊ��high
			limited����FB����ALARM��SUM�е�HI��Alarm���ر����Ϊ1���ò����ĵ�λ��
			OUT�����ĵ�λ��ͬ
		 */
	__Def_ParameterAttr(void*,pResv24);
	__Def_ParameterAttr(float*, pLolim);
		/*
		 * ���ޱ���(10wer limit of warning)ֵ
			�������ı������ڻ����L(LLIMֵ����OUT��״���ֽڵ�Limit������Ϊ��low
			limited����FB����AI��ARM��SUM�е�I��O��Alarm���ر����Ϊ1���ò����ĵ�λ��
			OUT�����ĵ�λ��ͬ
		 */
	__Def_ParameterAttr(void*,pResv26);
	__Def_ParameterAttr(float*, pLololim);
	__Def_ParameterAttr(void*,pResv28);
	__Def_ParameterAttr(void*,pResv29);
		/*
		 * �����ޱ���(10wer limit of alarm)ֵ
		 * �������ı������ڻ����LO-I��O 1 IMֵ����OUT��״���ֽڵ�Limit������Ϊ
		 * ��lowlimited����FB����AI��ARM SUM�е�I��()-LO��Alarm���ر����Ϊ1���ò�����
		��λ��OUT�����ĵ�λ��ͬ
		 */
	__Def_ParameterAttr(PA_AlarmFloatDS39_t*, pHihialm);
		/*
		 * �����ޱ�����״̬
		 * �ò������������ޱ�����״̬����ص�ʱ�������ʱ�����ʾ��������Ѿ����ڻ��
			�������ޱ���ֵʱ��ʱ��
		 */
	__Def_ParameterAttr(PA_AlarmFloatDS39_t*, pHialm);
		/*
		 * ���ޱ�����״̬
		 * �ò����������ޱ�����״̬����ص�ʱ�������ʱ�����ʾ���ݱ����Ѿ����ڻ����
			���ޱ���ֵʱ��ʱ��
		 */
	__Def_ParameterAttr(PA_AlarmFloatDS39_t*, pLoalm);
		/*
		 * ���ޱ�����״̬
		 * �ò����������ޱ�����״̬����ص�ʱ�������ʱ�����ʾ��������Ѿ����ڻ����
			���ޱ���ֵʱ��ʱ��
		 */
	__Def_ParameterAttr(PA_AlarmFloatDS39_t*, pLoloalm);
		/*
		 * �����ޱ�����״̬
		 * �ò������������ޱ�����״̬����ص�ʱ�������ʱ�����ʾ��������Ѿ����ڻ��
			�������ޱ���ֵʱ��ʱ��
		 */
	__Def_ParameterAttr(PA_SimuFloatPtDS50_t*, pSimulate);
	/*
	 * Ϊ�˵��ԺͲ��Ե�Ŀ�ģ������޸Ĵ�ת�������ģ�����빦�ܿ�AI FB������ֵ������
	 * ζ��ת������AI FB������ӱ��Ͽ�
	 */
	__Def_ParameterAttr(char*, pOut_unit_txt);
	/*
	 * ���OUT�������ض���λ���ڴ������(����ͨ��Ҫ��)�����û������ڴ˲�����д��
	 * �ض��ı�����λ��������ڡ��ı���λ���塱
	 */

	__Def_ParameterAttr(uint8_t*, pOpt_status) ;
}_PACKED_ IDX_AIFB_t;

typedef struct {
	PA_STDParameters_t Std;
	PA_BatchDS67_t BATCH ;//���ڱ�׼�������֣��˲���ּ�����ڷ���GB��T 19892��1��2005����Ӧ�á������ܿ���д˲������ڹ��ܿ���
							//����Ҫ�㷨���ڷֲ�ʽ�ֳ�����ϵͳ��BATCH�����Ǳر��ģ�����ʶ�����õĺͿ��õ�
							//ͨ�������⣮�ھ���������»�ʶ��ǰ���������5 2��3��19
	PA_ValueStatusFP101_t Out;
		/*
		 * ���Զ�(AUTO)ģʽ�£����ܿ����OUT�������������ض��Ļ���̬�����Ĺ��̵�λ�����ĵ�ǰ����ֵ�Լ�����״̬��
		 * ���ֶ�ģʽ(MANMODE)�����ܿ����OUT�����ɲ���Ա�����õ�ֵ��״̬��
		 */
	float      Pv_scale[2];//��һ����ֵ��100����EUֵ(PV_SCALE,EU_at_100��)���ڶ�����ֵ��0%��EUֵ(PV_SCALE,EU_at_0%).
		/*
		 *  ʹ�øߡ��ͱ��ֵ�����̱���ת���ɰٷ�����PV_SCALE�ĸߡ��ͱ��ֵ�Ĺ��̵�λ������̬��ת����(ͨ��Channel����������̬)��PV��UNITֱ���йء�
		*  PV_SCALE�ĸߡ��ͱ��ֵ�Զ����������ת������PV_UNIT�ĸı���ı䣬��ת����PV_UNIT�ĸı䲢������Al��OUT���Ŷ���
		 *   Ҳ���ܴ���������������磬��ϴ����������Ҫ���Ŷ�
		 */
	PA_ScalingDS36_t      Out_scale;
		/*
		 * ���̱����ı�ȡ�
		 * OUT_SCALE ���ܿ����OUT_SCALE��������ֵ������ֵ����Ч��Χ�����̱������̵�λ�Ĵ���
		 * �ţ��Լ�С�����ұߵ���Ч����λ��
		 */
	uint8_t            LinType;
		/*
		 * ���Ի����͡���ϸ���ݼ���50
		 */
	CHANNEL_t             Channel;
		/*
		 * ����Чת��������ã���ת����Ϊ���ܿ��ṩ����ֵ�����5��1��5��ͨ��Ҫ���塱
		 */
	float      Pv_ftime;
		/*
		 * ���̱������˲�ʱ�䡣
		 * ���ܿ����PV_FTIME���������ܿ�����ﵽ63.21%ʱ������ʱ�䣬Ϊʱ�䳣������
		 * ������ֵԾ��Ľ��(PTI�˲���)���ò����Ĺ��̵�λΪs��
		 */
	uint8_t      Fsafe_type;//����˲���δ��ʵ�֣���AIFB����Ϊ��ͬFSAFE_TYPE=1��
		/*
			  ���嵱��⵽����ʱ�豸�ķ�Ӧ���������ʵ��ģʽ������AUTO��
			0��ֵFSAFE_VALUE������OUT
			״��=UNCERTAIN��substitute value b��
			1��ʹ����һ�δ洢����ЧOUTֵ
			״��=UNCERTAIN-last usable value
			(���û����Чֵ���ã���Ӧ��ʹ��UNCERTAIN��initial value���ڴ�����£�OUTֵ=
			��ʼֵ)��
			2��OUT���д���ļ���ֵ��״��
			״��=BAD_(a)��(b)
			a,������ġ�
		    b,�˴�ʹ�þ���״�����壬��ʹ��Ũ��״���������83��
		 */
	float Fsafe_value;
		/*
		 * ����⵽�������򴫸�������Ԫ������ʱ������OUT������ȱʡֵ���ò����ĵ�λ��
		 * OUT�����ĵ�λ��ͬ
		 */
	float Alarm_hys; //ȱʡֵ 0.5
		/*
		 * �ͺ�(Hysteresis)
			��PROFIBUS PA�������淶�ķ�Χ�ڣ���һЩ�������ڼ��ӿɵ���ֵ�ĳ���(ƫ����
			ֵ����)��Ҳ��һ�����̱�����ֵǡǡ����ֵ��ͬ�����Ҹñ���Χ����ֵ���²������ɴ�
			���ɵ��¶�γ��ޡ�
			�⽫���������Ϣ����ˣ�Ӧ�п��ܽ��ڳ����ɵ����ͺ�֮��Ŵ�����Ϣ������������
			Ϣ���������ǿɵ��ġ��ͺ�ֵ�̶�ΪALARM_HYS������ڲ���HI_HI_LIM��HI_LIM��LO_LIM��LO_LO_LIM��һ���ġ��ͺ󱻱�ʾΪ��xx_LlMΪ���̵�λ�����ĵ�
			�����޺͸������޵�ֵ
		 */
	float Hihilim;
		/*
		 * �����ޱ���(upper limit of alarm)ֵ
			�������ı������ڻ����HI��HI��LIMֵ����OUT��״���ֽڵ�Limit������Ϊ
			��high limited����FB����ALARM��SUM�е�HLHLAlarm���ر����Ϊ1���ò�����
			��λ��OUT�����ĵ�λ��ͬ
		 */
	float Hilim;
		/*
		 * ���ޱ���(upper limit of warning)ֵ
			�������ı������ڻ����HI_l��IMֵ����OUT��״���ֽڵ�Limit������Ϊ��high
			limited����FB����ALARM��SUM�е�HI��Alarm���ر����Ϊ1���ò����ĵ�λ��
			OUT�����ĵ�λ��ͬ
		 */
	float Lolim;
		/*
		 * ���ޱ���(10wer limit of warning)ֵ
			�������ı������ڻ����L(LLIMֵ����OUT��״���ֽڵ�Limit������Ϊ��low
			limited����FB����AI��ARM��SUM�е�I��O��Alarm���ر����Ϊ1���ò����ĵ�λ��
			OUT�����ĵ�λ��ͬ
		 */
	float Lololim;
		/*
		 * �����ޱ���(10wer limit of alarm)ֵ
		 * �������ı������ڻ����LO-I��O 1 IMֵ����OUT��״���ֽڵ�Limit������Ϊ
		 * ��lowlimited����FB����AI��ARM SUM�е�I��()-LO��Alarm���ر����Ϊ1���ò�����
		��λ��OUT�����ĵ�λ��ͬ
		 */
	PA_AlarmFloatDS39_t Hihialm;
		/*
		 * �����ޱ�����״̬
		 * �ò������������ޱ�����״̬����ص�ʱ�������ʱ�����ʾ��������Ѿ����ڻ��
			�������ޱ���ֵʱ��ʱ��
		 */
	PA_AlarmFloatDS39_t Hialm;
		/*
		 * ���ޱ�����״̬
		 * �ò����������ޱ�����״̬����ص�ʱ�������ʱ�����ʾ���ݱ����Ѿ����ڻ����
			���ޱ���ֵʱ��ʱ��
		 */
	PA_AlarmFloatDS39_t Loalm;
		/*
		 * ���ޱ�����״̬
		 * �ò����������ޱ�����״̬����ص�ʱ�������ʱ�����ʾ��������Ѿ����ڻ����
			���ޱ���ֵʱ��ʱ��
		 */
	PA_AlarmFloatDS39_t Loloalm;
		/*
		 * �����ޱ�����״̬
		 * �ò������������ޱ�����״̬����ص�ʱ�������ʱ�����ʾ��������Ѿ����ڻ��
			�������ޱ���ֵʱ��ʱ��
		 */
	PA_SimuFloatPtDS50_t Simulate;
	/*
	 * Ϊ�˵��ԺͲ��Ե�Ŀ�ģ������޸Ĵ�ת�������ģ�����빦�ܿ�AI FB������ֵ������
	 * ζ��ת������AI FB������ӱ��Ͽ�
	 */
	char Out_unit_txt[16];
	/*
	 * ���OUT�������ض���λ���ڴ������(����ͨ��Ҫ��)�����û������ڴ˲�����д��
	 * �ض��ı�����λ��������ڡ��ı���λ���塱
	 */

	/**
	 * ����������
	 */
#define AIFB_OPTStatus_Normal    0  //����
#define AIFB_OPTStatus_OPTWrite  1  //����Աд

    uint8_t Opt_status ;
}_PACKED_ PA_AIFB_t;


typedef struct {
	IDX_STDParameters_t Std;
	__Def_ParameterAttr(void*,pResv8);
	__Def_ParameterAttr(PA_ValueStatusFP101_t*,pSp);
	/*
	 * �趨ֵ����AUTOģʽ�£������г̿�ȷ�Χ��(��λ��OPEN��CI��OSE֮��)
���տ���Ԫ����λ�ã���Pv��SCALE�ĵ�λ��
	 */
	__Def_ParameterAttr(void*,pResv10);
	__Def_ParameterAttr(PA_ScalingDS36_t*,pPvScale);
		/*
		 * ���Թ��̵�λ�Ƶ�PVת�����԰ٷ����Ƶ�PV����Ϊ���ܿ������ֵ�������
�У��ͱ��ֵ���߱��ֵ�����̵�λ�����ʮ����С�����ұߵ�λ��
		 */
	__Def_ParameterAttr(PA_ValueStatusFP101_t*,pReadback);
    /*
     * ���г̿�ȷ�Χ��(��λ��OPEN��CLOSE֮��)���տ���Ԫ����ʵ��λ�ã���
PV_SCALEΪ��λ
     */
	__Def_ParameterAttr(void*,pResv13);
	__Def_ParameterAttr(PA_ValueStatusFP101_t*,pRcasIn);
	/*
	 * ��RCasģʽ�£���PVSCALEΪ��λ��Ŀ���趨ֵ���Լ��ɼ��������ģ���
�ƻ�������ṩ��״��
	 */
	__Def_ParameterAttr(void*,pResv15);
	__Def_ParameterAttr(void*,pResv16);
	__Def_ParameterAttr(void*,pResv17);
	__Def_ParameterAttr(void*,pResv18);
	__Def_ParameterAttr(void*,pResv19);
	__Def_ParameterAttr(void*,pResv20);
	__Def_ParameterAttr(uint16_t*,pInChannel);
	/*
	 * ����Ч��ת���鼰����������ã����ṩ�����տ���Ԫ��(FEEDBACK��VALUE
D)��ʵ��λ�á�������������ͨ��Ҫ��(CHANNEL)
	 */
	__Def_ParameterAttr(uint16_t*,pOutChannel);
	/*
	 * ����Ч��ת���鼰����������ã����ṩ�����տ���Ԫ��(pOSITIONING��
VALUE_D)��ʵ��λ�á�������������ͨ��Ҫ��(CHANNEL)
	 */
	__Def_ParameterAttr(float*,pFsafeTime);
		/*
		 * �Ӽ���ʵ��ʹ�õ��趨ֵ����(SP��D��BAD��RCAS_IN<>GOOD)������
			�鶯��(�ô���������Ȼ����)�����ʱ�䣬��s�ơ�
			ע��ͨ�ų�ʱ��ʹ��������趨ֵ״���ı�ΪBAD(��ӳ��)
		 */
	__Def_ParameterAttr(uint8_t*,pFsafeType);
	/*
	 * ��FsAFE��TIME֮����Ȼ��⵽ʵ��ʹ�õ��趨ֵ���������¡�������ʵ��
		ʹ�õ��趨ֵ��״��Ϊ��Initiate Fail Safe������£������豸�ķ�Ӧ��
		�����ʵ��ģʽ�ֱ�ΪAUTO��RCAs(��5��2��3��6)��
		���룺
		0��ֵFSAFE_VALUE_D�����趨ֵ
			OuT��״��=UNCERTAIN��substitute value
		l�� ʹ����һ����Ч�趨ֵ
			0UT��״��=UNCERTAIN��Last Usable Value
			��BAD-No communication��no LUV
		2�� ִ����������ACTUATOR��ACTION������Ĺ���һ��ȫλ��(����
			���ھ��е��Իָ���ִ����)��OUT��״��=BAD-non specific
	 */
	__Def_ParameterAttr(float*,pFsafeValue);
      /*
       * ���FSAFE_TYPE=0��FSAFE�������ʹ�ø��趨ֵ
       */
	__Def_ParameterAttr(void*,pResv26);
	__Def_ParameterAttr(PA_ValueStatusFP101_t*,pRcasOut);
      /*
       *��PV_SCALEΪ��λ�Ĺ��ܿ���趨ֵ��״�����������ṩ���������������
��أ�������㣬��������������������ģʽ�ı�����²�ȡ�ж�
       */
	__Def_ParameterAttr(void*,pResv28);
	__Def_ParameterAttr(void*,pResv29);
	__Def_ParameterAttr(void*,pResv30);
	__Def_ParameterAttr(PA_ValueStatusDS102_t*,pPosD);
      /*
       * ���ĵ�ǰλ��(��ɢ)������:
			0:δ��ʼ��
			1:�ر�
			2:��
			3:�м�λ��
       */
	__Def_ParameterAttr(float*,pSetpDeviation);
      /*
       * OUT�ź��뷴��λ��֮��Ĳ��죬���г̿�ȵİٷ�����(��λ��OPEN��
CLOSE֮��)
       */
	__Def_ParameterAttr(char*,pCheckBack);
      /*
       * �豸����ϸ��Ϣ����λ���롣ͬʱ�����ж����Ϣ
       */
	__Def_ParameterAttr(char*,pCheckBackMask);
      /*
       * �����Ƿ�֧��CHECK��BACK����Ϣ���ء����룺
		0:��֧��
		1:֧��
       */
	__Def_ParameterAttr(PA_SimuFloatPtDS50_t*,pSimulate);
      /*
       * ���ڵ��Ժ�ά��ԭ�򣬿�ͨ������ֵ��״��������READBACK����ʱ�����Ͽ���
ת���鵽AO FB֮����ź�·��
       */
	__Def_ParameterAttr(uint8_t*,pIncreaseClose);
      /*
       * ��RCAS��AUTOģʽ�£������������趨ֵ��ִ�����˶�����
		���룺
		0:����(�趨ֵ�����ӵ��·�����--()PENNING)
		1:�½�(�趨ֵ�����ӵ��·��ر�һcI��OSING)
		INCREASE��CLOSE�����û�Ӱ�����в�����READBACK��POS��D��OUT
		��CHECKBACK
       */
	__Def_ParameterAttr(PA_ValueStatusFP101_t*,pOut);
      /*
       * �˲�����AUTO��RCasģʽ��ģ�������Ĺ��̱������Թ��̵�λ�ơ���ֵ��
����Ա������ʦ��MAN��IOģʽ���趨
       */
	__Def_ParameterAttr(PA_ScalingDS36_t*,pOutScale);
      /*
       * �����ܿ����԰ٷ����Ƶ�OUTת�����Թ��̵�λ�Ƶ�OUT����Ϊ�ù��ܿ����
			��ֵ��������У��ͱ��ֵ���߱��ֵ�����̵�λ�����ʮ����С�����ұߵ�λ����
			����Ӧ֧�����µ�λ��mm����(��)����(ȡ����VALVE-TYPE)
       */
}_PACKED_ IDX_AOFB_t;

typedef struct {
	PA_STDParameters_t Std;

	PA_ValueStatusFP101_t Sp;
	/*
	 * �趨ֵ����AUTOģʽ�£������г̿�ȷ�Χ��(��λ��OPEN��CI��OSE֮��)
���տ���Ԫ����λ�ã���Pv��SCALE�ĵ�λ��
	 */
	PA_ScalingDS36_t      PvScale;
		/*
		 * ���Թ��̵�λ�Ƶ�PVת�����԰ٷ����Ƶ�PV����Ϊ���ܿ������ֵ�������
�У��ͱ��ֵ���߱��ֵ�����̵�λ�����ʮ����С�����ұߵ�λ��
		 */
	PA_ValueStatusFP101_t Readback;
    /*
     * ���г̿�ȷ�Χ��(��λ��OPEN��CLOSE֮��)���տ���Ԫ����ʵ��λ�ã���
PV_SCALEΪ��λ
     */
	PA_ValueStatusFP101_t RcasIn;
	/*
	 * ��RCasģʽ�£���PVSCALEΪ��λ��Ŀ���趨ֵ���Լ��ɼ��������ģ���
�ƻ�������ṩ��״��
	 */
	uint16_t InChannel;
	/*
	 * ����Ч��ת���鼰����������ã����ṩ�����տ���Ԫ��(FEEDBACK��VALUE
D)��ʵ��λ�á�������������ͨ��Ҫ��(CHANNEL)
	 */
	uint16_t OutChannel;
	/*
	 * ����Ч��ת���鼰����������ã����ṩ�����տ���Ԫ��(pOSITIONING��
VALUE_D)��ʵ��λ�á�������������ͨ��Ҫ��(CHANNEL)
	 */
	float      FsafeTime;
		/*
		 * �Ӽ���ʵ��ʹ�õ��趨ֵ����(SP��D��BAD��RCAS_IN<>GOOD)������
			�鶯��(�ô���������Ȼ����)�����ʱ�䣬��s�ơ�
			ע��ͨ�ų�ʱ��ʹ��������趨ֵ״���ı�ΪBAD(��ӳ��)
		 */
	uint8_t FsafeType;
	/*
	 * ��FsAFE��TIME֮����Ȼ��⵽ʵ��ʹ�õ��趨ֵ���������¡�������ʵ��
		ʹ�õ��趨ֵ��״��Ϊ��Initiate Fail Safe������£������豸�ķ�Ӧ��
		�����ʵ��ģʽ�ֱ�ΪAUTO��RCAs(��5��2��3��6)��
		���룺
		0��ֵFSAFE_VALUE_D�����趨ֵ
			OuT��״��=UNCERTAIN��substitute value
		l�� ʹ����һ����Ч�趨ֵ
			0UT��״��=UNCERTAIN��Last Usable Value
			��BAD-No communication��no LUV
		2�� ִ����������ACTUATOR��ACTION������Ĺ���һ��ȫλ��(����
			���ھ��е��Իָ���ִ����)��OUT��״��=BAD-non specific
	 */
      float FsafeValue;
      /*
       * ���FSAFE_TYPE=0��FSAFE�������ʹ�ø��趨ֵ
       */
      PA_ValueStatusFP101_t   RcasOut;
      /*
       *��PV_SCALEΪ��λ�Ĺ��ܿ���趨ֵ��״�����������ṩ���������������
��أ�������㣬��������������������ģʽ�ı�����²�ȡ�ж�
       */
      PA_ValueStatusDS102_t PosD;
      /*
       * ���ĵ�ǰλ��(��ɢ)������:
			0:δ��ʼ��
			1:�ر�
			2:��
			3:�м�λ��
       */
      float SetpDeviation;
      /*
       * OUT�ź��뷴��λ��֮��Ĳ��죬���г̿�ȵİٷ�����(��λ��OPEN��
CLOSE֮��)
       */
      char CheckBack[3];
      /*
       * �豸����ϸ��Ϣ����λ���롣ͬʱ�����ж����Ϣ
       */
      char CheckBackMask[3];
      /*
       * �����Ƿ�֧��CHECK��BACK����Ϣ���ء����룺
		0:��֧��
		1:֧��
       */
      PA_SimuFloatPtDS50_t Simulate;
      /*
       * ���ڵ��Ժ�ά��ԭ�򣬿�ͨ������ֵ��״��������READBACK����ʱ�����Ͽ���
ת���鵽AO FB֮����ź�·��
       */
      uint8_t IncreaseClose;
      /*
       * ��RCAS��AUTOģʽ�£������������趨ֵ��ִ�����˶�����
		���룺
		0:����(�趨ֵ�����ӵ��·�����--()PENNING)
		1:�½�(�趨ֵ�����ӵ��·��ر�һcI��OSING)
		INCREASE��CLOSE�����û�Ӱ�����в�����READBACK��POS��D��OUT
		��CHECKBACK
       */
      PA_ValueStatusFP101_t Out;
      /*
       * �˲�����AUTO��RCasģʽ��ģ�������Ĺ��̱������Թ��̵�λ�ơ���ֵ��
����Ա������ʦ��MAN��IOģʽ���趨
       */
      PA_ScalingDS36_t OutScale;
      /*
       * �����ܿ����԰ٷ����Ƶ�OUTת�����Թ��̵�λ�Ƶ�OUT����Ϊ�ù��ܿ����
			��ֵ��������У��ͱ��ֵ���߱��ֵ�����̵�λ�����ʮ����С�����ұߵ�λ����
			����Ӧ֧�����µ�λ��mm����(��)����(ȡ����VALVE-TYPE)
       */
}_PACKED_ PA_AOFB_t;


typedef struct {
	IDX_STDParameters_t Std;
	__Def_ParameterAttr(void*,pResv8);
	__Def_ParameterAttr(void*,pResv9);
	__Def_ParameterAttr(PA_ValueStatusDS102_t*,pOutD);
	/*
	 * OUT��D�ǹ��ܿ���������Manģʽ�£���ֵ�ɲ���Ա����
	 */
	__Def_ParameterAttr(void*,pResv11);
	__Def_ParameterAttr(void*,pResv12);
	__Def_ParameterAttr(void*,pResv13);
	__Def_ParameterAttr(uint16_t*,pChannel);
	/*
	 * ���ܿ��ṩ����ֵ����Чת����������������ͨ��Ҫ���塱
	 */
	__Def_ParameterAttr(uint8_t*,pInvert);
	/*
	 * ��Pv-D������ֵ��������OUT��D֮ǰ��ָʾ�Ƿ�Ӧ��������߼���ת��
	 * 0������ת
		1����ת
	 */
	__Def_ParameterAttr(void*,pResv16);
	__Def_ParameterAttr(void*,pResv17);
	__Def_ParameterAttr(void*,pResv18);
	__Def_ParameterAttr(void*,pResv19);
	__Def_ParameterAttr(uint8_t*,pFsafeType);
	/*
	 * �����ڼ�⵽����ʱ�豸�ķ�Ӧ��
		���룺
		0����ֵFSAFE-VAI��UE��ΪOUT_Dʹ��
		״��=UNCERTAIN��substitute value b
		1��ʹ����һ�α����OUT_D��Чֵ
		״��=UNCERTAIN��last usable value
		(�������Чֵ���ã���Ӧʹ��UNCERTAIN��Initial value) b
		2��OUT_D���д���ļ���ֵ��״̬
		״̬=BADһ(a),(b)
		a: ��������ģ�
		b: �˴�ʹ�þ���״�����壻���ʹ��Ũ��״������83
	 */
	__Def_ParameterAttr(uint8_t*,pFsafeValD);
	/**
	 * ���������������ϻ򴫸������ӹ���ʱ������OUT_D��ȱʡֵ
	 */
	__Def_ParameterAttr(void*,pResv22);
	__Def_ParameterAttr(void*,pResv23);
	__Def_ParameterAttr(PA_SimuDiscreteDS51_t*,pSimulate);
	/*
	 * Ϊ�˱��ڵ��ԺͲ��ԣ���ɢ���˹��ܿ�DI FB������ת���������ֵ��ͨ���ò��������޸ġ�
	 * ��ʱ���Ͽ�ת������DI FB������
	 */
}_PACKED_ IDX_DIFB_t;

typedef struct {
	PA_STDParameters_t Std;
	PA_ValueStatusDS102_t OutD;
	/*
	 * OUT��D�ǹ��ܿ���������Manģʽ�£���ֵ�ɲ���Ա����
	 */
	uint16_t Channel;
	/*
	 * ���ܿ��ṩ����ֵ����Чת����������������ͨ��Ҫ���塱
	 */
	uint8_t Invert;
	/*
	 * ��Pv-D������ֵ��������OUT��D֮ǰ��ָʾ�Ƿ�Ӧ��������߼���ת��
	 * 0������ת
		1����ת
	 */
	uint8_t FsafeType;
	/*
	 * �����ڼ�⵽����ʱ�豸�ķ�Ӧ��
		���룺
		0����ֵFSAFE-VAI��UE��ΪOUT_Dʹ��
		״��=UNCERTAIN��substitute value b
		1��ʹ����һ�α����OUT_D��Чֵ
		״��=UNCERTAIN��last usable value
		(�������Чֵ���ã���Ӧʹ��UNCERTAIN��Initial value) b
		2��OUT_D���д���ļ���ֵ��״̬
		״̬=BADһ(a),(b)
		a: ��������ģ�
		b: �˴�ʹ�þ���״�����壻���ʹ��Ũ��״������83
	 */
	uint8_t FsafeValD;
	/**
	 * ���������������ϻ򴫸������ӹ���ʱ������OUT_D��ȱʡֵ
	 */
	PA_SimuDiscreteDS51_t Simulate;
	/*
	 * Ϊ�˱��ڵ��ԺͲ��ԣ���ɢ���˹��ܿ�DI FB������ת���������ֵ��ͨ���ò��������޸ġ�
	 * ��ʱ���Ͽ�ת������DI FB������
	 */
}_PACKED_ PA_DIFB_t;

typedef struct {
	IDX_STDParameters_t Std;
	__Def_ParameterAttr(void*,pResv8);
	__Def_ParameterAttr(PA_ValueStatusDS102_t*,pSpD);
	/*
	 * AUTOģʽ�����õĹ��ܿ���趨ֵ
	 */
	__Def_ParameterAttr(PA_ValueStatusDS102_t*,pOutD);
	/*
	 * ��AUTO��RCasģʽ�¸ò�������ɢ������ܿ�Ĺ��̱�������MAN��LOģ
		ʽ�¸ò������ɲ���Ա��m��ʦ�趨��ֵ����BAD״���£�������ACTUATOR_
		ACTION���涨��λ��
	 */
	__Def_ParameterAttr(void*,pResv11);
	__Def_ParameterAttr(PA_ValueStatusDS102_t*,pReadBackD);
	/*
	 * ���տ���Ԫ�����䴫������ʵ��λ��
	 */
	__Def_ParameterAttr(void*,pResv13);
	__Def_ParameterAttr(PA_ValueStatusDS102_t*,pRcasInD);
	/*
	 * ��������ṩ������RCASģʽ�µ���ɢ������ܿ��Ŀ���趨ֵ��״��
	 */
	__Def_ParameterAttr(void*,pResv15);
	__Def_ParameterAttr(void*,pResv16);
	__Def_ParameterAttr(uint16_t*,pChannel);
	/*
	 * ����Ч��ת���鼰����������ã����ṩ�����տ���Ԫ��(POSITIONING_VALUE_D)��ʵ��λ�á�������������ͨ��Ҫ��(CHANNEL)
	 */
	__Def_ParameterAttr(uint8_t*,pInvert);
	/*
	 * ָʾ��ģʽAUTO��RCAS��дOUT_D֮ǰ�Ƿ�Ӧ��SP_D�߼�ȡ����
		���룺
		0����ȡ��
		1��ȡ��
	 */
	__Def_ParameterAttr(float*,pFsafeTime);
	/*
	 * �Ӽ���ʵ��ʹ�õ��趨ֵ����(SP_D=BAD��RCAS_IN<>GOOD)�����ܿ�
		����(�ô���������Ȼ����)�����ʱ�䣬��s�ơ�
		ע��ͨ�ų�ʱ��ʹ��������趨ֵ״���ı�ΪBAD(��6��3��4)
	 */
	__Def_ParameterAttr(uint8_t*,pFsafeType);
	/*
	 * ��FSAFE_TIME֮����Ȼ��⵽ʵ��ʹ�õ��趨ֵ���������£�������ʵ��
		ʹ�õ��趨ֵ��״��Ϊ��Initiate Fail Safe������£������豸�ķ�Ӧ��
		�����ʵ��ģʽΪAUTO(��5��2��3 6)��
		���룺
		0�� ֵFSAFE_VALUE_D�����趨ֵ
		OUT_D��״��=UNCERTAlN��substitute value
		1��ʹ����һ����Ч�趨ֵ
		OUT_D��״��=UNCERTAIN��Last usable Value
		��BAD_No communication��no LUV
		2�� ִ����������ACTUATOR_ACTION������Ĺ��ϰ�ȫλ�ã�
		OUT_D��״��=BAD - non specific
	 */
	__Def_ParameterAttr(uint8_t*,pFsafeValD);
	/*
	 * ���FSAFE_TYPE=0��FSAFE�������OUT_D���ݸ�ֵ
	 */
	__Def_ParameterAttr(PA_ValueStatusDS102_t*,pRcasOutD);
	/*
	 * ���������ṩ�Ĺ��ܿ���趨ֵ��״�������ڼ�أ��������(back
		calculation)��������������������ģʽ�仯������²�ȡ�ж�
	 */
	__Def_ParameterAttr(void*,pResv23);
	__Def_ParameterAttr(PA_SimuDiscreteDS51_t*,pSimulate);
	/*
	 * ���ڵ��Ժ�ά������ͨ������ֵ��״��������READBACK����ʱ��ת�����DO FB�����ӱ��Ͽ�
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
	 * �豸����ϸ��Ϣ����λ���롣ͬʱ�����ж����Ϣ
	 */
	__Def_ParameterAttr(uint8_t*,pCheckBackMask);
	/*
	 * �����Ƿ�֧��CHECK_BACK����Ϣ���ء�
		ÿ�����صı��룺
		0����֧��
		1��֧��
	 */
}_PACKED_ IDX_DOFB_t;

typedef struct {
	PA_STDParameters_t Std;
	PA_ValueStatusDS102_t SpD;
	/*
	 * AUTOģʽ�����õĹ��ܿ���趨ֵ
	 */
	PA_ValueStatusDS102_t OutD;
	/*
	 * ��AUTO��RCasģʽ�¸ò�������ɢ������ܿ�Ĺ��̱�������MAN��LOģ
		ʽ�¸ò������ɲ���Ա��m��ʦ�趨��ֵ����BAD״���£�������ACTUATOR_
		ACTION���涨��λ��
	 */
	PA_ValueStatusDS102_t ReadBackD;
	/*
	 * ���տ���Ԫ�����䴫������ʵ��λ��
	 */
	PA_ValueStatusDS102_t RcasInD;
	/*
	 * ��������ṩ������RCASģʽ�µ���ɢ������ܿ��Ŀ���趨ֵ��״��
	 */
	uint16_t Channel;
	/*
	 * ����Ч��ת���鼰����������ã����ṩ�����տ���Ԫ��(POSITIONING_VALUE_D)��ʵ��λ�á�������������ͨ��Ҫ��(CHANNEL)
	 */
	uint8_t Invert;
	/*
	 * ָʾ��ģʽAUTO��RCAS��дOUT_D֮ǰ�Ƿ�Ӧ��SP_D�߼�ȡ����
		���룺
		0����ȡ��
		1��ȡ��
	 */
	float FsafeTime;
	/*
	 * �Ӽ���ʵ��ʹ�õ��趨ֵ����(SP_D=BAD��RCAS_IN<>GOOD)�����ܿ�
		����(�ô���������Ȼ����)�����ʱ�䣬��s�ơ�
		ע��ͨ�ų�ʱ��ʹ��������趨ֵ״���ı�ΪBAD(��6��3��4)
	 */
	uint8_t FsafeType;
	/*
	 * ��FSAFE_TIME֮����Ȼ��⵽ʵ��ʹ�õ��趨ֵ���������£�������ʵ��
		ʹ�õ��趨ֵ��״��Ϊ��Initiate Fail Safe������£������豸�ķ�Ӧ��
		�����ʵ��ģʽΪAUTO(��5��2��3 6)��
		���룺
		0�� ֵFSAFE_VALUE_D�����趨ֵ
		OUT_D��״��=UNCERTAlN��substitute value
		1��ʹ����һ����Ч�趨ֵ
		OUT_D��״��=UNCERTAIN��Last usable Value
		��BAD_No communication��no LUV
		2�� ִ����������ACTUATOR_ACTION������Ĺ��ϰ�ȫλ�ã�
		OUT_D��״��=BAD - non specific
	 */
	uint8_t FsafeValD;
	/*
	 * ���FSAFE_TYPE=0��FSAFE�������OUT_D���ݸ�ֵ
	 */
	PA_ValueStatusDS102_t RcasOutD;
	/*
	 * ���������ṩ�Ĺ��ܿ���趨ֵ��״�������ڼ�أ��������(back
		calculation)��������������������ģʽ�仯������²�ȡ�ж�
	 */
	PA_SimuDiscreteDS51_t Simulate;
	/*
	 * ���ڵ��Ժ�ά������ͨ������ֵ��״��������READBACK����ʱ��ת�����DO FB�����ӱ��Ͽ�
	 */
	uint8_t CheckBack[3];
	/*
	 * �豸����ϸ��Ϣ����λ���롣ͬʱ�����ж����Ϣ
	 */
	uint8_t CheckBackMask[3];
	/*
	 * �����Ƿ�֧��CHECK_BACK����Ϣ���ء�
		ÿ�����صı��룺
		0����֧��
		1��֧��
	 */
}_PACKED_ PA_DOFB_t;


typedef struct {
	IDX_STDParameters_t Std;
	__Def_ParameterAttr(PA_ValueStatusFP101_t*, pPrimary_value);
		/*
		 * PRIMARY_VALUE��ת����Ĺ���ֵ��״��������ģ�����������롣�����Ի�
		 * ����=0ʱ��PRIMARY_VALUE��������λ��ͬ��ֵ���䵥λ��PRIMARY_VALUE_UNIT�ж���
		 */
	__Def_ParameterAttr(uint16_t*, pPrimary_value_unit);
		/*
		 * ΪPRIMARY_VALUE��LIN_VOLUME��ѡ��ĵ�λ���롣
		 * �ر���%,m,ft
		 */
	__Def_ParameterAttr(float*, pLevel);
		/*
		 * ��SENSOR��VALUE������ת��ֱ�ӵ�������λ��������ת��ͨ��ʹ��LEVEL��
		 * HI��LEVEL��LO��CAL��POINT��HI��CAL��POINT��LO��LEVEL��oFFSETʵ�֡�
			�䵥λ��LEVEL_UNIT�ж���
		 */
	__Def_ParameterAttr(uint16_t*, pLevel_unit);
		/*
		 * ΪLEVEl����LEVEl��HI 1��EVEL_I��O��LIN��DIAMETER��ѡ��ĵ�λ���롣
		 * �ر�������m��ft
		 */
	__Def_ParameterAttr(float*, pSensor_value);
		/*
		 * ������������ֵ
		 */
	__Def_ParameterAttr(uint16_t*, pSensor_unit);
		/*
		 * ����SENSOR��VAI��UE��SENSOR��LOW��LIMIT��SENSOR��HIGH��LIMIT��AL��
			P��)INT��HI��CAL��POINT��LO��MAX��SENSOR��VALUE��M1N��SENSOR��
			VAI��UE�ĵ�λ��
			�ر���ѹ����λ��Pa��mbar��psi���ر��ľ��뵥λ��131��ft
		 */
	__Def_ParameterAttr(PA_ValueStatusFP101_t*, pSecondary_value1);
		/*
		 * secndary��value ��LEVEL+LEVELOFFSET���Լ�ת�����״������
		 * ��λ��SECONDARLVALUE 1һUNIT�ж��塣�������2��ģ�����������
		 */
	__Def_ParameterAttr(uint16_t*, pSecondary_value1_unit);
		/*
		 * ΪSECONDARY-VALUE��l��ѡ��ĵ�λ���룬����LEVEl��һUNIT�ж����
			��ͬ���ر�������m��ft
		 */
	__Def_ParameterAttr(PA_ValueStatusFP101_t*, pSecondary_value2);
		/*
		 * SECONDARY_VAI��UEһ2��SENSOR��VALUE+SENSOR��OFFSET���Լ�ת��
		 * ���״�����䵥λ��SECONDARY��vALUEһ2һUNIT�ж��塣�������3��ģ��
			���������
		 */
	__Def_ParameterAttr(uint16_t*, pSecondary_value2_unit);
		/*
		 * ΪSECONDARlLVALUE-2��ѡ��ĵ�λ���룬����SENSOR��UNIT�ж����
			��ͬ���ر���ѹ����λ��Pa��mbar��psi���ر��ľ��뵥λ��m��ft
		 */
	__Def_ParameterAttr(float*, pSensor_offset);
		/*
		 * SENSOR_OFFSET�Ǹ�SENSOR��VALUE��ӵĳ���ƫ�������䵥λ��SENSOR��UNIT�ж���
		 */
	__Def_ParameterAttr(uint8_t*, pCal_type);
		/*
		 * ����У׼���͡�
		���룺
		0�� ���ߣ���������ֵ����λУ׼��Ӱ�졣�����״��豸�Ǳر��ġ�
		1�����ߣ��������ĵ�ǰֵ������λУ׼
		 */
    __Def_ParameterAttr(float*, pCal_point_lo);
		/*
		 * cALPOINT_Lo��SENSOR��VALUE�ĵ�У׼�㡣����Ӧ��LEVEL��LO���䵥
		 * λ��SENSOR��UNIT�ж���
		 */
    __Def_ParameterAttr(float*, pCal_point_hi);
		/*
		 * CALPOINT HI��SENSOR��VALuE�ĸ�У׼�㡣����Ӧ��I��VEL��HI���䵥
		 * λ��SENSOR UNIT�ж���
		 */
    __Def_ParameterAttr(float*, pLevel_lo);
		/*
		 * LEVELLO�Ǵ���CAL��POINT��LOʱ����λֵ���䵥λ��LEVEL��UNIT�ж��塣��д��LEVELLO��CAL TYPEһ1ʱ��CALPOINT��LO�Զ�����ΪSENSOR��
			VALUE
		 */
    __Def_ParameterAttr(float*, pLevel_hi);
		/*
		 * LEVELHI�Ǵ���CAL��PoINT��HIʱ����λֵ���䵥λ��I��EvEL��UNIT�ж��塣
		 * ��д��LEVEL HI��cALT��)Eһ1ʱ��cALPoINT_HI�Զ�����ΪSENSOR��VALUE
		 */
    __Def_ParameterAttr(float*, pLevel_offset);
		/*
		 * LEVEL��OFFSET������λУ׼�Ĵ��͹���֮����ӵĳ���ƫ�������䵥λ��LEVELUNlT�ж���
		 */
    __Def_ParameterAttr(uint8_t*, pLin_type);
		/*
		 * ����50
		 */
    __Def_ParameterAttr(float*, pLin_diameter);
		/*
		 * ��LEVEL_UNITΪ��λ��Բ����ʽ��������״������ֱ����
		 * ��LIN��TYPEһ20��2lʱʹ��
		 */
    __Def_ParameterAttr(float*, pLin_volume);
		/*
		 * LIN��VOIUME��Բ����ʽ��������״����������������
		 * ��IIN-TYPE_20��21ʱʹ�á��䵥λ��PRIMARY_VAI��UE��UNIT�ж���
		 */
    __Def_ParameterAttr(float*, pSensor_high_limit);
		/*
		 * �������Ĺ������ޣ��䵥λ��SENSOR��UNIT�ж���
		 */
    __Def_ParameterAttr(float*, pSensor_low_limit);
		/*
		 * �������Ĺ������ޣ��䵥λ��SENSOR��UNIT�ж���
		 */
    __Def_ParameterAttr(float*, pMax_sensor_value);
		/*
		 * ���Ĺ���ֵSENSOR��VALUE���䵥λ��SENSOR��UNIT�ж���
		 */
    __Def_ParameterAttr(float*, pMin_sensor_value);
		/*
		 * ��С�Ĺ���ֵSENSOR��VALUE���䵥λ��SENSOR UNIT�ж���
		 */
    __Def_ParameterAttr(float*, pTemperature);
		/*
		 * �����¶�
		 */
    __Def_ParameterAttr(uint16_t*, pTemperature_unit);
		/*
		 * �¶ȵ�λ��ΪTEMPERATURE��MAX_TEMPERATURE��MAX_TEMPERATURE��ѡ��ĵ�λ����
		 */
    __Def_ParameterAttr(float*, pMax_temperature);
		/*
		 * ���Ĺ����¶�
		 */
    __Def_ParameterAttr(float*, pMin_temperature);
		/*
		 * ��С�Ĺ����¶�
		 */
    IDX_TAB_PARAMETER_t pTab;
}_PACKED_ IDX_LevelTB_t;


typedef struct {
	PA_STDParameters_t Std;
	PA_ValueStatusFP101_t Primary_value;
		/*
		 * PRIMARY_VALUE��ת����Ĺ���ֵ��״��������ģ�����������롣�����Ի�
		 * ����=0ʱ��PRIMARY_VALUE��������λ��ͬ��ֵ���䵥λ��PRIMARY_VALUE_UNIT�ж���
		 */
	uint16_t Primary_value_unit;
		/*
		 * ΪPRIMARY_VALUE��LIN_VOLUME��ѡ��ĵ�λ���롣
		 * �ر���%,m,ft
		 */
	float Level;
		/*
		 * ��SENSOR��VALUE������ת��ֱ�ӵ�������λ��������ת��ͨ��ʹ��LEVEL��
		 * HI��LEVEL��LO��CAL��POINT��HI��CAL��POINT��LO��LEVEL��oFFSETʵ�֡�
			�䵥λ��LEVEL_UNIT�ж���
		 */
	uint16_t Level_unit;
		/*
		 * ΪLEVEl����LEVEl��HI 1��EVEL_I��O��LIN��DIAMETER��ѡ��ĵ�λ���롣
		 * �ر�������m��ft
		 */
	float Sensor_value;
		/*
		 * ������������ֵ
		 */
	uint16_t Sensor_unit;
		/*
		 * ����SENSOR��VAI��UE��SENSOR��LOW��LIMIT��SENSOR��HIGH��LIMIT��AL��
			P��)INT��HI��CAL��POINT��LO��MAX��SENSOR��VALUE��M1N��SENSOR��
			VAI��UE�ĵ�λ��
			�ر���ѹ����λ��Pa��mbar��psi���ر��ľ��뵥λ��131��ft
		 */
	PA_ValueStatusFP101_t Secondary_value1;
		/*
		 * secndary��value ��LEVEL+LEVELOFFSET���Լ�ת�����״������
		 * ��λ��SECONDARLVALUE 1һUNIT�ж��塣�������2��ģ�����������
		 */
	uint16_t Secondary_value1_unit;
		/*
		 * ΪSECONDARY-VALUE��l��ѡ��ĵ�λ���룬����LEVEl��һUNIT�ж����
			��ͬ���ر�������m��ft
		 */
	PA_ValueStatusFP101_t Secondary_value2;
		/*
		 * SECONDARY_VAI��UEһ2��SENSOR��VALUE+SENSOR��OFFSET���Լ�ת��
		 * ���״�����䵥λ��SECONDARY��vALUEһ2һUNIT�ж��塣�������3��ģ��
			���������
		 */
	uint16_t Secondary_value2_unit;
		/*
		 * ΪSECONDARlLVALUE-2��ѡ��ĵ�λ���룬����SENSOR��UNIT�ж����
			��ͬ���ر���ѹ����λ��Pa��mbar��psi���ر��ľ��뵥λ��m��ft
		 */
	float Sensor_offset;
		/*
		 * SENSOR_OFFSET�Ǹ�SENSOR��VALUE��ӵĳ���ƫ�������䵥λ��SENSOR��UNIT�ж���
		 */
    uint8_t Cal_type;
		/*
		 * ����У׼���͡�
		���룺
		0�� ���ߣ���������ֵ����λУ׼��Ӱ�졣�����״��豸�Ǳر��ġ�
		1�����ߣ��������ĵ�ǰֵ������λУ׼
		 */
    float Cal_point_lo;
		/*
		 * cALPOINT_Lo��SENSOR��VALUE�ĵ�У׼�㡣����Ӧ��LEVEL��LO���䵥
		 * λ��SENSOR��UNIT�ж���
		 */
    float Cal_point_hi;
		/*
		 * CALPOINT HI��SENSOR��VALuE�ĸ�У׼�㡣����Ӧ��I��VEL��HI���䵥
		 * λ��SENSOR UNIT�ж���
		 */
    float Level_lo;
		/*
		 * LEVELLO�Ǵ���CAL��POINT��LOʱ����λֵ���䵥λ��LEVEL��UNIT�ж��塣��д��LEVELLO��CAL TYPEһ1ʱ��CALPOINT��LO�Զ�����ΪSENSOR��
			VALUE
		 */
    float Level_hi;
		/*
		 * LEVELHI�Ǵ���CAL��PoINT��HIʱ����λֵ���䵥λ��I��EvEL��UNIT�ж��塣
		 * ��д��LEVEL HI��cALT��)Eһ1ʱ��cALPoINT_HI�Զ�����ΪSENSOR��VALUE
		 */
    float Level_offset;
		/*
		 * LEVEL��OFFSET������λУ׼�Ĵ��͹���֮����ӵĳ���ƫ�������䵥λ��LEVELUNlT�ж���
		 */
    uint8_t Lin_type;
		/*
		 * ����50
		 */
    float Lin_diameter;
		/*
		 * ��LEVEL_UNITΪ��λ��Բ����ʽ��������״������ֱ����
		 * ��LIN��TYPEһ20��2lʱʹ��
		 */
    float Lin_volume;
		/*
		 * LIN��VOIUME��Բ����ʽ��������״����������������
		 * ��IIN-TYPE_20��21ʱʹ�á��䵥λ��PRIMARY_VAI��UE��UNIT�ж���
		 */
    float Sensor_high_limit;
		/*
		 * �������Ĺ������ޣ��䵥λ��SENSOR��UNIT�ж���
		 */
    float Sensor_low_limit;
		/*
		 * �������Ĺ������ޣ��䵥λ��SENSOR��UNIT�ж���
		 */
    float Max_sensor_value;
		/*
		 * ���Ĺ���ֵSENSOR��VALUE���䵥λ��SENSOR��UNIT�ж���
		 */
    float Min_sensor_value;
		/*
		 * ��С�Ĺ���ֵSENSOR��VALUE���䵥λ��SENSOR UNIT�ж���
		 */
    float Temperature;
		/*
		 * �����¶�
		 */
    uint16_t Temperature_unit;
		/*
		 * �¶ȵ�λ��ΪTEMPERATURE��MAX_TEMPERATURE��MAX_TEMPERATURE��ѡ��ĵ�λ����
		 */
    float Max_temperature;
		/*
		 * ���Ĺ����¶�
		 */
    float Min_temperature;
		/*
		 * ��С�Ĺ����¶�
		 */
    PA_TAB_PARAMETER_t tab;
}_PACKED_ PA_LevelTB_t;


typedef struct {
	uint8_t Structure_Length; //ָʾ��Prm_Structure���ֽڸ���������Structure Length�ֽڱ����̶�Ϊ5
	uint8_t Structure_Type; //�̶�Ϊ65һ�й��ض�
	uint8_t Slot_Number; //�̶�Ϊ0����ζ���ô�Prm��Strcuture���е����ö������豸������Ч��
	uint8_t reserved;
	uint8_t Options; // Bit0��PRM��COND	0������Ũ��״����l������Ũ��״����Bitl��Bit7������
}_PACKED_ PA_Prm_Structure_t;



typedef struct {
    uint8_t   head;   /* ״̬���������Ϣ�������ͷ */        // equal to DIAG_DEV_SIGN
    uint8_t   status_type;
    uint8_t   slot_number;
    uint8_t   specifier;
	uint8_t DIAGNOSIS[4];//�豸����ϸ��Ϣ�������ر��롣ͬʱ���ܰ��������Ϣ�����Byte4��MSB���� Ϊ1��
						//���ڲ���DIAGNOSIS_EXTENSION�ڿ��ṩ���������Ϣ��	DIAGNOSIS�����ı�����5��3��3��5��3��4�й涨
	uint8_t DIAGNOSIS_EXTENSION[6];//���ӵ��������ض����豸��Ϣ�������ر��롣ͬʱ���ܰ��������Ϣ
}_PACKED_ PA_DIAG_t;


/**
 * ��վ�������ݲο��Ƚ�����
 * ��pNext == null, û����һ���ο���
 *
 */
typedef struct __cfg_refer__
{
	 const uint8_t* pRef_cfg_data;
	 uint8_t ref_cfg_len;
	 ParameterAttr_t* pRealData[8];
}_PACKED_ CFG_Refer_t;

/**
 * �������ݽṹ��������ģ�黯��վ
 * �ýṹһ��������ʹ�ã����鳤��Ϊ�������õ�ģ��������
 * ��pa_user_chk_new_cfg_data �����У���¼���롢����ĸ���λ�ã�����
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
	 * �ر�ע�⣡������
	 * PA�豸�Ĳ����洢���ǰ�����������洢�ģ�Ҳ���Ǵ��ģʽ��
	 * 1�������LITTLE_ENDIAN С�˴�������Ҫʹ�÷��ֽ�(char,uint8_t,int8_t)
	 * ���͵Ĳ�����Ӧ��ʹ��__ntohs(x)��__ntohl(x)����ת������ʹ�á�
	 *
	 * 2��ͬ�������LITTLE_ENDIAN С�˴�������Ҫ�޸Ĳ������������
	 * ���ֽ�(char,uint8_t,int8_t)����,Ӧ��ʹ��__htons(x)��__htonl(x)����ת����,
	 * �ٸ�ֵ������
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
		PA_SimuFloatPtDS50_t* pSimu, /* ��null��ָ������ģʽ������Ϊ����ģʽ */
		uint8_t target_mode, /* Ŀ��ģʽ */
		PA_ValueStatusFP101_t* pPV_TB, /* ����TB�ļ������ */
		float optInValue /*��MAN��O/Sģʽ�£�����Աָ����ֵ */
		);
void LevelTB_Block_calcAll(
		PB_BLOCK_t* pPb,
		PA_LevelTB_t* pBlk,
		PA_ValueStatusFP101_t* pSensorValue, /* ���Դ�������ֵ */
		float optInValue /*��MAN��O/Sģʽ�£�����Աָ����ֵ */
		);

void PA_timerTick1ms(void);
ParameterAttr_t* PAFindParamBlock(PA_DEV_t* pDev,
		uint8_t slot_num,/* �ۺ� */
		uint8_t index,/* ������ */
		uint8_t* pErro_Slot/* ����۱�� */);

int PTA_Read(PA_DEV_t* pDev,PB_BLOCK_t* pBlk,DPV1_UNION_PDU_t* pdu);
int PTA_Write(PA_DEV_t* pDev,PB_BLOCK_t* pBlk,DPV1_UNION_PDU_t* pdu);

uint8_t PA_chk_prm_data(PA_DEV_t* dev, uint8_t*pToPrmData,uint8_t bPrmLength );
uint8_t PA_chk_cfg_data(PA_DEV_t* dev, uint8_t*pToCfgData,uint8_t bCfgLength );
void PA_updata_Input(PA_DEV_t* dev,uint8_t* sInput ,uint8_t len);
void PA_updata_Output(PA_DEV_t* dev,uint8_t* sOutput ,uint8_t len);

/*
 * �û�����ʵ�����º���
 */
/* �洢PA�����������ʧ�洢���� */
void userBlocksNvsSave(char* pdata, uint16_t len);
/* �ӷ���ʧ�洢���ж�ȡPA����� */
void userBlocksNvsGet(char* pdata, uint16_t len);

#endif /* PA_H_ */
