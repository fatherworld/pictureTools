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
     * 分配一个ComputeH需要用的结构体空间
     * @param data: out, 分配的结构体
     * @param maxPointNum: in, 最大点数
     * @return: 空
     */
    void AllocComputeH_Direct_Data(pComputeH_Direct_Data* data, int maxPointNum);

    /*
     * FreeComputeHData
     * 释放一个ComputeH需要用的结构体空间，必须是由AllocComputeHData分配的
     * @return: 空
     */
    void FreeComputeH_Direct_Data(pComputeH_Direct_Data* data);

    /*
     * setComputeHData
     * 根据点对信息,生成计算H矩阵所使用的数据
     * @param feat0: 第一组点对信息,按照x,y,x,y,x,y...顺序排列
     * @param feat1: 第二组点对信息,按照x,y,x,y,x,y...顺序排列
     * @param loc: loc不为NULL时,存储feat0,feat1匹配的点的序号,例如, feat0[loc[i] * 2]  feat1[loc[i] * 2]这两个点匹配
     * @param num: loc不为NULL时,loc中的数量. loc为NULL时,feat0和feat点对的数量
     * @param pComHData: 根据以上数据生成的,计算H矩阵的数据.在ComputeH函数中使用
     * @return: 0：正确，0x80019907: 输入点数大于最大可输入点
     */
    int setComputeH_Direct_Data(float* feat0, float* feat1, int* loc, int num, pComputeH_Direct_Data pComHData);
    int setComputeH_Direct_Data_disperse(float* feat0, float* feat1, int* matchLoc, int* consensus, int num, pComputeH_Direct_Data pComHData, int maxNum);

    /*
     * ComputeH
     * 根据点对计算H矩阵
     * @param pComHData: 计算H矩阵使用的数据,由setComputeHData 函数得到该数据
     * @param H_gauss[]: 计算出的H矩阵
     * @return: 0: 成功, 其他: 失败
     */
    int ComputeH_Direct(pComputeH_Direct_Data pComHData, float H_gauss[]);

    /*
     * Project2DPointsWithH
     * 计算H下srcPoints的投影点dstPoints
     * @param H: 投影矩阵
     * @param srcPoints: 需要投影的点, 按x1,y1,x2,y2...存储
     * @param dstPoints: 投影后的点，按x1',y1',x2',y2'...存储
     * @param num: 点数
     * @param cx: 旋转中心x坐标
     * @param cy: 旋转中心y坐标
     * @return: 0: 成功, 其他：计算某个点时出现mod为0的情况，其中错误码部分表示出错点序号
     */
    int Project2DPointsWithH(float H[],
                             float* srcPoints, float* dstPoints, int num,
                             float cx, float cy);

#ifdef __cplusplus
}
#endif


#endif
