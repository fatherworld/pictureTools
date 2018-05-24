#ifndef _Consistency_Rand_h_
#define _Consistency_Rand_h_

#ifdef AlvaPublic
#include "Consistency_Hide.h"
#endif

#include "ComputeH_Direct.h"

typedef struct _Consistency_Rand_Info_{
    //外部直接访问部分
    int* mConsLoc;
    int  mConsNum;
    float mH[9];

    //可调参数部分(必须相应方法进行写访问)
    int pRandTime;
    int pMaxPointNum;
    float pCoarseThreshold;
    float pFineThreshold;

    //临时变量部分
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
    * 一致性检测需要用的结构体空间
    * @param data: out, 分配的结构体
    * @param maxPointNum: in, 最大点数
    * @return: 空
    */
    void AllocConsistency_Rand_Data(pConsistency_Rand_Info* data, int maxPointNum);

    /*
    * FreeConsistency_Rand_Data
    * 释放一个一致性检测需要用的结构体空间，必须是由AllocConsistency_Rand_Data分配的
    * @return: 空
    */
    void FreeConsistency_Rand_Data(pConsistency_Rand_Info* data);
    
    /*
     * setRandTime
     * 设置随机次数，默认为300
     * @return: 空
     */
    void setRandTime(pConsistency_Rand_Info data, int time);

    /*
     * setCoarseThreshold
     * 设置粗选域值，默认为5
     */
    void setCoarseThreshold(pConsistency_Rand_Info data, float threshold);

    /*
     * setFineThreshold
     * 设置粗选域值，默认为2
     */
    void setFineThreshold(pConsistency_Rand_Info data, float threshold);

    /*
     * Consistency_Rand_Check
     * 进行随机一致性检测
     * @srcPoints: 第一组点对信息,按照x,y,x,y,x,y...顺序排列
     * @dstPoints: 第二组点对信息,按照x,y,x,y,x,y...顺序排列
     * @num: 点对的数量
     * @pPoseAnalysisInfo: 一致性检测使用的数据,内存等信息,由AllocConsistency_Rand_Data函数得到
     * @return:0,一致性检测正常运行,非0,表明点对数量小于6,即num < 6
     */
    int Consistency_Rand_Check(float* srcPoints, float* dstPoints, int num, pConsistency_Rand_Info pPoseAnalysisInfo);

    /*
     * Consistency_Rand_Check_disperse
     * 进行随机一致性检测
     * @srcPoints: 第一组点对信息,按照x,x,x,x,x..., y,y,y,y,y...顺序排列,x固定占用空间maxPointNum个，y起始位置偏移maxPointNum个
     * @dstPoints: 第二组点对信息,按照x,x,x,x,x..., y,y,y,y,y...顺序排列,x固定占用空间maxPointNum个，y起始位置偏移maxPointNum个
     * @matchLoc: 匹配点对的序号列表，计算用：x=points[matchLoc[i]], y=point[maxPointNum + matchLoc[i]]
     * @num: 点对的数量
     * @pPoseAnalysisInfo: 一致性检测使用的数据,内存等信息,由AllocConsistency_Rand_Data函数得到
     * 返回值为0,一致性检测正常运行,非0,表明点对数量小于6,即num < 6
     */
    int Consistency_Rand_Check_disperse(float* srcPoints, float* dstPoints, int* matchLoc, int num, pConsistency_Rand_Info pPoseAnalysisInfo);

    /*
     * Consistency_Rand_Check_RotationConsistency
     * 进行随机一致性检测，先对每次随机选取的4对点进行方向一致性检测，在合格的条件(点的旋转方向一致)下才进行进一步一致性计算
     * 功能与Consistency_Rand_Check一致，但速度会快
     * @srcPoints: 第一组点对信息,按照x,y,x,y,x,y...顺序排列
     * @dstPoints: 第二组点对信息,按照x,y,x,y,x,y...顺序排列
     * @num: 点对的数量
     * @pPoseAnalysisInfo: 一致性检测使用的数据,内存等信息,由AllocConsistency_Rand_Data函数得到
     * @return:0,一致性检测正常运行,非0,表明点对数量小于6,即num < 6
     */
    int Consistency_Rand_Check_RotationConsistency(float* srcPoints, float* dstPoints, int num, pConsistency_Rand_Info pPoseAnalysisInfo);
   
    /*
     * Consistency_Rand_Check
     * 进行随机一致性检测，先对每次随机选取的4对点进行方向一致性检测，在合格的条件(点的旋转方向一致)下才进行进一步一致性计算
     * @srcPoints: 第一组点对信息,按照x,x,x,x,x..., y,y,y,y,y...顺序排列,x固定占用空间maxPointNum个，y起始位置偏移maxPointNum个
     * @dstPoints: 第二组点对信息,按照x,x,x,x,x..., y,y,y,y,y...顺序排列,x固定占用空间maxPointNum个，y起始位置偏移maxPointNum个
     * @matchLoc: 匹配点对的序号列表，计算用：x=points[matchLoc[i]], y=point[maxPointNum + matchLoc[i]]
     * @num: 点对的数量
     * @pPoseAnalysisInfo: 一致性检测使用的数据,内存等信息,由AllocConsistency_Rand_Data函数得到
     * 返回值为0,一致性检测正常运行,非0,表明点对数量小于6,即num < 6
     */
    int Consistency_Rand_Check_RotationConsistency_disperse(float* srcPoints, float* dstPoints, int* matchLoc, int num, pConsistency_Rand_Info pPoseAnalysisInfo);

    /*
    * Consistency_MLESAC
    * 进行随机一致性检测(最优评价采用误差最小的方式)，内点域值采用FineThreshold
    * @srcPoints: 第一组点对信息,按照x,y,x,y,x,y...顺序排列
    * @dstPoints: 第二组点对信息,按照x,y,x,y,x,y...顺序排列
    * @num: 点对的数量
    * @pPoseAnalysisInfo: 一致性检测使用的数据,内存等信息,由AllocConsistency_Rand_Data函数得到
    * @return:0,一致性检测正常运行,非0,表明点对数量小于6,即num < 6
    */
    int Consistency_MLESAC(float* srcPoints, float* dstPoints, int num, pConsistency_Rand_Info pPoseAnalysisInfo);

    /*
    * Consistency_MLESAC_disperse
    * 进行随机一致性检测(最优评价采用误差最小的方式)，内点域值采用FineThreshold
    * @srcPoints: 第一组点对信息,按照x,x,x,x,x..., y,y,y,y,y...顺序排列,x固定占用空间maxPointNum个，y起始位置偏移maxPointNum个
    * @dstPoints: 第二组点对信息,按照x,x,x,x,x..., y,y,y,y,y...顺序排列,x固定占用空间maxPointNum个，y起始位置偏移maxPointNum个
    * @matchLoc: 匹配点对的序号列表，计算用：x=points[matchLoc[i]], y=point[@maxPointNum + matchLoc[i]]
    * @num: 点对的数量
    * @pPoseAnalysisInfo: 一致性检测使用的数据,内存等信息,由AllocConsistency_Rand_Data函数得到
    * 返回值为0,一致性检测正常运行,非0,表明点对数量小于6,即num < 6
    */
    int Consistency_MLESAC_disperse(float* srcPoints, float* dstPoints, int* matchLoc, int num, pConsistency_Rand_Info pPoseAnalysisInfo);
#ifdef __cplusplus
}
#endif

#endif
