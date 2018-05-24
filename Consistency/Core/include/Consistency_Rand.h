#ifndef _Consistency_Rand_h_
#define _Consistency_Rand_h_

#ifdef AlvaPublic
#include "Consistency_Hide.h"
#endif

#include "ComputeH_Direct.h"

typedef struct _Consistency_Rand_Info_{
    //�ⲿֱ�ӷ��ʲ���
    int* mConsLoc;
    int  mConsNum;
    float mH[9];

    //�ɵ���������(������Ӧ��������д����)
    int pRandTime;
    int pMaxPointNum;
    float pCoarseThreshold;
    float pFineThreshold;

    //��ʱ��������
    int* tConsensus1;
    int* tConsensus2;
    int  tSample[4];
    int* tSampled;
    int* tLabeled1;
    int* tLabeled2;
    int* tConsTimes;
    pComputeH_Direct_Data tComputeHData;
}Consistency_Rand_Info, *pConsistency_Rand_Info;

#ifdef __cplusplus
extern "C"
{
#endif
    /*
    * AllocConsistency_Rand_Data
    * һ���Լ����Ҫ�õĽṹ��ռ�
    * @param data: out, ����Ľṹ��
    * @param maxPointNum: in, ������
    * @return: ��
    */
    void AllocConsistency_Rand_Data(pConsistency_Rand_Info* data, int maxPointNum);

    /*
    * FreeConsistency_Rand_Data
    * �ͷ�һ��һ���Լ����Ҫ�õĽṹ��ռ䣬��������AllocConsistency_Rand_Data�����
    * @return: ��
    */
    void FreeConsistency_Rand_Data(pConsistency_Rand_Info* data);
    
    /*
     * setRandTime
     * �������������Ĭ��Ϊ300
     * @return: ��
     */
    void setRandTime(pConsistency_Rand_Info data, int time);

    /*
     * setCoarseThreshold
     * ���ô�ѡ��ֵ��Ĭ��Ϊ5
     */
    void setCoarseThreshold(pConsistency_Rand_Info data, float threshold);

    /*
     * setFineThreshold
     * ���ô�ѡ��ֵ��Ĭ��Ϊ2
     */
    void setFineThreshold(pConsistency_Rand_Info data, float threshold);

    /*
     * Consistency_Rand_Check
     * �������һ���Լ��
     * @srcPoints: ��һ������Ϣ,����x,y,x,y,x,y...˳������
     * @dstPoints: �ڶ�������Ϣ,����x,y,x,y,x,y...˳������
     * @num: ��Ե�����
     * @pPoseAnalysisInfo: һ���Լ��ʹ�õ�����,�ڴ����Ϣ,��AllocConsistency_Rand_Data�����õ�
     * @return:0,һ���Լ����������,��0,�����������С��6,��num < 6
     */
    int Consistency_Rand_Check(float* srcPoints, float* dstPoints, int num, pConsistency_Rand_Info pPoseAnalysisInfo);

    /*
     * Consistency_Rand_Check_disperse
     * �������һ���Լ��
     * @srcPoints: ��һ������Ϣ,����x,x,x,x,x..., y,y,y,y,y...˳������,x�̶�ռ�ÿռ�maxPointNum����y��ʼλ��ƫ��maxPointNum��
     * @dstPoints: �ڶ�������Ϣ,����x,x,x,x,x..., y,y,y,y,y...˳������,x�̶�ռ�ÿռ�maxPointNum����y��ʼλ��ƫ��maxPointNum��
     * @matchLoc: ƥ���Ե�����б������ã�x=points[matchLoc[i]], y=point[maxPointNum + matchLoc[i]]
     * @num: ��Ե�����
     * @pPoseAnalysisInfo: һ���Լ��ʹ�õ�����,�ڴ����Ϣ,��AllocConsistency_Rand_Data�����õ�
     * ����ֵΪ0,һ���Լ����������,��0,�����������С��6,��num < 6
     */
    int Consistency_Rand_Check_disperse(float* srcPoints, float* dstPoints, int* matchLoc, int num, pConsistency_Rand_Info pPoseAnalysisInfo);

    /*
     * Consistency_Rand_Check_RotationConsistency
     * �������һ���Լ�⣬�ȶ�ÿ�����ѡȡ��4�Ե���з���һ���Լ�⣬�ںϸ������(�����ת����һ��)�²Ž��н�һ��һ���Լ���
     * ������Consistency_Rand_Checkһ�£����ٶȻ��
     * @srcPoints: ��һ������Ϣ,����x,y,x,y,x,y...˳������
     * @dstPoints: �ڶ�������Ϣ,����x,y,x,y,x,y...˳������
     * @num: ��Ե�����
     * @pPoseAnalysisInfo: һ���Լ��ʹ�õ�����,�ڴ����Ϣ,��AllocConsistency_Rand_Data�����õ�
     * @return:0,һ���Լ����������,��0,�����������С��6,��num < 6
     */
    int Consistency_Rand_Check_RotationConsistency(float* srcPoints, float* dstPoints, int num, pConsistency_Rand_Info pPoseAnalysisInfo);
   
    /*
     * Consistency_Rand_Check
     * �������һ���Լ�⣬�ȶ�ÿ�����ѡȡ��4�Ե���з���һ���Լ�⣬�ںϸ������(�����ת����һ��)�²Ž��н�һ��һ���Լ���
     * @srcPoints: ��һ������Ϣ,����x,x,x,x,x..., y,y,y,y,y...˳������,x�̶�ռ�ÿռ�maxPointNum����y��ʼλ��ƫ��maxPointNum��
     * @dstPoints: �ڶ�������Ϣ,����x,x,x,x,x..., y,y,y,y,y...˳������,x�̶�ռ�ÿռ�maxPointNum����y��ʼλ��ƫ��maxPointNum��
     * @matchLoc: ƥ���Ե�����б������ã�x=points[matchLoc[i]], y=point[maxPointNum + matchLoc[i]]
     * @num: ��Ե�����
     * @pPoseAnalysisInfo: һ���Լ��ʹ�õ�����,�ڴ����Ϣ,��AllocConsistency_Rand_Data�����õ�
     * ����ֵΪ0,һ���Լ����������,��0,�����������С��6,��num < 6
     */
    int Consistency_Rand_Check_RotationConsistency_disperse(float* srcPoints, float* dstPoints, int* matchLoc, int num, pConsistency_Rand_Info pPoseAnalysisInfo);

    /*
    * Consistency_MLESAC
    * �������һ���Լ��(�������۲��������С�ķ�ʽ)���ڵ���ֵ����FineThreshold
    * @srcPoints: ��һ������Ϣ,����x,y,x,y,x,y...˳������
    * @dstPoints: �ڶ�������Ϣ,����x,y,x,y,x,y...˳������
    * @num: ��Ե�����
    * @pPoseAnalysisInfo: һ���Լ��ʹ�õ�����,�ڴ����Ϣ,��AllocConsistency_Rand_Data�����õ�
    * @return:0,һ���Լ����������,��0,�����������С��6,��num < 6
    */
    int Consistency_MLESAC(float* srcPoints, float* dstPoints, int num, pConsistency_Rand_Info pPoseAnalysisInfo);

    /*
    * Consistency_MLESAC_disperse
    * �������һ���Լ��(�������۲��������С�ķ�ʽ)���ڵ���ֵ����FineThreshold
    * @srcPoints: ��һ������Ϣ,����x,x,x,x,x..., y,y,y,y,y...˳������,x�̶�ռ�ÿռ�maxPointNum����y��ʼλ��ƫ��maxPointNum��
    * @dstPoints: �ڶ�������Ϣ,����x,x,x,x,x..., y,y,y,y,y...˳������,x�̶�ռ�ÿռ�maxPointNum����y��ʼλ��ƫ��maxPointNum��
    * @matchLoc: ƥ���Ե�����б������ã�x=points[matchLoc[i]], y=point[@maxPointNum + matchLoc[i]]
    * @num: ��Ե�����
    * @pPoseAnalysisInfo: һ���Լ��ʹ�õ�����,�ڴ����Ϣ,��AllocConsistency_Rand_Data�����õ�
    * ����ֵΪ0,һ���Լ����������,��0,�����������С��6,��num < 6
    */
    int Consistency_MLESAC_disperse(float* srcPoints, float* dstPoints, int* matchLoc, int num, pConsistency_Rand_Info pPoseAnalysisInfo);
#ifdef __cplusplus
}
#endif

#endif
