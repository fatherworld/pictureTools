#ifndef _ComputerH_Direct_h_
#define _ComputerH_Direct_h_

#ifdef AlvaPublic
#include "Project_Hide.h"
#endif

typedef struct _COMPUTEH_Direct_Coord_ {
    float x;
    float y;
}ComputeH_Direct_Coord, *pComputeH_Direct_Coord;

typedef struct _ComputeH_Direct_Data_{
    pComputeH_Direct_Coord mTMCoor0;
    pComputeH_Direct_Coord mTMCoor1;
    float* mTMxy;
    float* mTMH;
    float* mTMHTra;
    float mTMHMul[8 * 8];
    float mTMHxy[8 * 1];
    int pairNum;
    int maxPointNum;
}ComputeH_Direct_Data, *pComputeH_Direct_Data;

#ifdef __cplusplus
extern "C"
{
#endif
    /*
     * AllocComputeHData
     * ����һ��ComputeH��Ҫ�õĽṹ��ռ�
     * @param data: out, ����Ľṹ��
     * @param maxPointNum: in, ������
     * @return: ��
     */
    void AllocComputeH_Direct_Data(pComputeH_Direct_Data* data, int maxPointNum);

    /*
     * FreeComputeHData
     * �ͷ�һ��ComputeH��Ҫ�õĽṹ��ռ䣬��������AllocComputeHData�����
     * @return: ��
     */
    void FreeComputeH_Direct_Data(pComputeH_Direct_Data* data);

    /*
     * setComputeHData
     * ���ݵ����Ϣ,���ɼ���H������ʹ�õ�����
     * @param feat0: ��һ������Ϣ,����x,y,x,y,x,y...˳������
     * @param feat1: �ڶ�������Ϣ,����x,y,x,y,x,y...˳������
     * @param loc: loc��ΪNULLʱ,�洢feat0,feat1ƥ��ĵ�����,����, feat0[loc[i] * 2]  feat1[loc[i] * 2]��������ƥ��
     * @param num: loc��ΪNULLʱ,loc�е�����. locΪNULLʱ,feat0��feat��Ե�����
     * @param pComHData: ���������������ɵ�,����H���������.��ComputeH������ʹ��
     * @return: 0����ȷ��0x80019907: ��������������������
     */
    int setComputeH_Direct_Data(float* feat0, float* feat1, int* loc, int num, pComputeH_Direct_Data pComHData);
    int setComputeH_Direct_Data_disperse(float* feat0, float* feat1, int* matchLoc, int* consensus, int num, pComputeH_Direct_Data pComHData, int maxNum);

    /*
     * ComputeH
     * ���ݵ�Լ���H����
     * @param pComHData: ����H����ʹ�õ�����,��setComputeHData �����õ�������
     * @param H_gauss[]: �������H����
     * @return: 0: �ɹ�, ����: ʧ��
     */
    int ComputeH_Direct(pComputeH_Direct_Data pComHData, float H_gauss[]);

    /*
     * Project2DPointsWithH
     * ����H��srcPoints��ͶӰ��dstPoints
     * @param H: ͶӰ����
     * @param srcPoints: ��ҪͶӰ�ĵ�, ��x1,y1,x2,y2...�洢
     * @param dstPoints: ͶӰ��ĵ㣬��x1',y1',x2',y2'...�洢
     * @param num: ����
     * @param cx: ��ת����x����
     * @param cy: ��ת����y����
     * @return: 0: �ɹ�, ����������ĳ����ʱ����modΪ0����������д����벿�ֱ�ʾ��������
     */
    int Project2DPointsWithH(float H[],
                             float* srcPoints, float* dstPoints, int num,
                             float cx, float cy);

#ifdef __cplusplus
}
#endif


#endif
