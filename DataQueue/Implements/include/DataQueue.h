#pragma once
#ifndef  __DATAQUEUE_H__
#define  __DATAQUEUE_H__

/**
 * @file  DataQueue.h
 * @brief 算法调用:先注册算法FL_regAlgorithm，然后取要处理的数据FL_getData，数据处理完之后设置处理
                   的结果FL_setResultData，然后给外部接口调用获取处理结果FL_updateResult，最后反注
                   册算法FL_unRegAlgorithm。如果算法注册之后，不运行算法，则冻结算法FL_freezeAlgor
                   ithm，在运行算法处激活算法FL_activeAlgorithm即可。

                   注册算法时，算法默认激活，同步算法要求算法处理速度快，如果处理速度慢，则会出现一
                   跳一跳的情况，而且同步算法的数据和处理结果是匹配的，但是同步算法注册之后，不处理
                   数据，此时UI不显示画面，此时你可以冻结算法，画面正常显示，等到处理算法时激活。
                   异步算法则算法的数据和结果是不确定匹配的，有可能此次数据显示的结果是之前处理的结果。

                   切换算法(FL_switchoverAlgorithm)，不激活直接调用，激活的算法在FL_getData和FL_setRes
                   ultData之后调用，不推荐使用。
                   FL_getData和FL_setResultData必须成对调用
                   FL_regAlgorithm和FL_unRegAlgorithm也必须成对出现
                   
                   @see FL_regAlgorithm         先注册算法
                   @see FL_freezeAlgorithm      冻结算法(根据需要调用)
                   @see FL_activeAlgorithm      激活算法(和冻结算法相对应)
                   @see FL_getData              再从链表中取数据处理
                   @see FL_setResultData        然后设置处理结果
                   @see FL_updateResult         取数据的处理结果
                   @see FL_unRegAlgorithm       反注册自己的算法
                   @see FL_switchoverAlgorithm  切换算法(不推荐使用)

          设置依赖:在算法注册之后，如有依赖算法，则设置依赖FL_setDepend，然后可以获取依赖的算法结果大小
                   FL_getDependResultSize，以及所依赖的算法结果FL_getDependResult。

                   @see FL_setDepend           设置依赖的算法,在注册算法之后调用 
                   @see FL_getDependResultSize 获得所依赖算法的结果大小 
                   @see FL_getDependResult     获得所依赖算法的结果

 */

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief 枚举类型,为注册函数FL_regAlgorithm的第二个参数
     */
    typedef enum _AlgorithmType_ {
        AT_Sync = 0,           /*!< 同步算法 */
        AT_Asyn = 1,           /*!< 异步算法 */
    }AlgorithmType;

    /**
     * @brief 注册算法， 注册的算法默认为激活状态
     * @param algorithmName: 算法名字
     * @param resultSize: 算法处理之后结果大小
     * @param algorithmType: AT_Sync 同步  AT_Asyn 异步 @see AlgorithmType
     * @param initResult: 算法处理结果的初始值
     * @return: int型，注册算法的索引号algorithmIndex
     *         -<em> < 0 </em> fail，错误码
     *         -<em> >=0 </em> succeed，算法索引号
     */
    int FL_regAlgorithm(char algorithmName[256], int resultSize, AlgorithmType algorithmType, void* initResult);

    /**
     * @brief 反注册算法,释放注册算法时申请的空间
     * @param algorithmIndex: 注册算法时FL_regAlgorithm函数的返回值 @see FL_regAlgorithm
     * @return: int型
     *         -<em> <0 </em> fail，错误码
     *         -<em>  0 </em> succeed
     */
    int FL_unRegAlgorithm(int algorithmIndex);

    /**
     * @brief 从链表中拿一帧最新数据处理
     * @param algorithmIndex: 注册算法时FL_regAlgorithm函数的返回值
     * @param frameData: 调用者申请的内存空间,大小为FrameInit参数ImageSize的值 @see FrameInit
     * @param matrix[16]: 纹理的变化矩阵
     * @return: int型
     *         -<em> <0 </em> fail，错误码
     *         -<em>  0 </em> succeed
     */
    int FL_getData(int algorithmIndex, unsigned char* frameData, float matrix[16]);

    /**
     * @brief 向处理帧添加处理结果
     * @param algorithmIndex: 注册算法时FL_regAlgorithm函数的返回值
     * @param frameResultData: 调用者申请的内存空间,大小为FL_regAlgorithm函数参数resultSize的值 @see FL_regAlgorithm
     * @return: int型
     *         -<em> <0 </em> fail，错误码
     *         -<em>  0 </em> succeed
     */
    int FL_setResultData(int algorithmIndex, void * frameResultData);

    /**
     * @brief UI获取数据结果并显示
     * @param algorithmIndex: 注册算法时FL_regAlgorithm函数的返回值
     * @param frameResultData: 调用者申请的内存空间,大小为FL_regAlgorithm函数参数resultSize的值 @see FL_regAlgorithm
     * @return: int型
     *         -<em> <0 </em> fail，错误码
     *         -<em>  0 </em> succeed
     */
    int FL_updateResult(int algorithmIndex, void* frameResultData);

    /**
     * @brief 激活算法
     * @param algorithmIndex: 注册算法时FL_regAlgorithm函数的返回值 @see FL_regAlgorithm
     * @return: int型
     *         -<em> <0 </em> fail，错误码
     *         -<em>  0 </em> succeed
     */
    int FL_activeAlgorithm(int algorithmIndex);

    /**
     * @brief 冻结算法
     * @param algorithmIndex: 注册算法时FL_regAlgorithm函数的返回值 @see FL_regAlgorithm
     * @return: int型
     *         -<em> <0 </em> fail，错误码
     *         -<em>  0 </em> succeed
     */
    int FL_freezeAlgorithm(int algorithmIndex);

    /**
     * @brief 切换算法
     * @param algorithmIndex: 注册算法时FL_regAlgorithm函数的返回值 @see FL_regAlgorithm
     * @param algorithmType: AT_Sync 同步  AT_Asyn 异步 @see AlgorithmType
     * @return: int型
     *         -<em> <0 </em> fail，错误码
     *         -<em>  0 </em> succeed
     */
    int FL_switchoverAlgorithm(int algorithmIndex, AlgorithmType algorithmType);

#ifdef __cplusplus
}
#endif

#endif
