/*
 * PA_map.h
 *
 *  Created on: Jul 27, 2020
 *      Author: fred_note
 */

#ifndef PA_MAP_H_
#define PA_MAP_H_


/**
 * �û�����
 */

/*
 * ע�⣬��ǰ֧�ֵĲ�����������
 * 1������֧��һ��PB��
 * 2��ÿ����Ĳ��������ܳ���254-16 = 238 ����
 * 3��֧�ֶ��TB��FB��
 *
 *
 * */
#define NUM_BLK_TB    1   //ת�������
#define NUM_BLK_FB_AI    4  //���ܿ����
#define NUM_BLK_FB_AO    4  //���ܿ����
#define NUM_BLK_FB_DI    4  //���ܿ����
#define NUM_BLK_FB_DO    4  //���ܿ����
#define NUM_BLK_LO    1   //���Ӷ������

#define NUM_BLK_FB 	(NUM_BLK_FB_AI+NUM_BLK_FB_AO+NUM_BLK_FB_DI+NUM_BLK_FB_DO)


#define TAB_ADDR_FLASH   0x807F000    //SIZE 2k

/*
 * �й�淶�����ɱ�
 */
#define NUM_BLK_PB    1   //��������,Ŀǰֻ֧��1�������ܸı�

#define FB_SLOT_START 		1  //Slot 1������1��FB��Slot 2������2��FB����������ģ��(GSD��)��FB��Slot֮����һ��һ��ӳ�䡣����j)��Ωһ�����⡣
#define FB_INDEX_START 		16 //c)����FB������ӦSlot������(Index)16��ʼ(��FB��Block��Object����λ��Indexl6)��
#define DIR_DEVICE_SLOT 	1  //�豸�����Ŀ¼���Ǵ�(Slot l��Index 0)��ʼ��
#define DIR_DEVICE_INDEX 	0  //�豸�����Ŀ¼���Ǵ�(Slot l��Index 0)��ʼ��
#define PB_SLOT_FIX    	    0  //e)�����豸��PB��Ӧλ��Slot 0�У���Index 16��ʼ��
#define PB_INDEX_START  	16 //e)�����豸��PB��Ӧλ��Slot 0�У���Index 16��ʼ��

#define TB_SLOT_START 		2
#define TB_INDEX_START 		16

#define DM_SLOT    1
#define DM_INDEX   0

#define get_PB_SLOT(x)    (PB_SLOT_START)
#define get_PB_INDEX(x)   (PB_INDEX_START + x - 1)
#define get_FB_SLOT(x)    (FB_SLOT_START + x -1)
#define get_FB_INDEX(x)   (FB_INDEX_START)


/**
 *  ��ѭ�����ݴ���ӳ��
 */
#define ADDR_PARA_(slot,index,Rel_Index) (slot)





#endif /* PA_MAP_H_ */
