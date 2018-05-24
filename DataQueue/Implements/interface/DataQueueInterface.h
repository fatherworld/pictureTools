#pragma once
#ifndef  __DATAQUEUEINTERFACE_H__
#define  __DATAQUEUEINTERFACE_H__

/**
 * @file  DataQueueInterface.h
 * @brief 相机调用:先初始化链表FrameInit，然后相机往链表中添加数据UpdateImage，
                   调用getBackground使相机显示数据， 最后相机退出之前调用反初始化FrameUnit。

                   @see FrameInit     链表的初始化
                   @see UpdateImage   往链表中添加数据
                   @see getBackground UI获取数据并显示
                   @see FrameUnit     最后相机程序退出时调用反初始化

 */

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief 链表初始化
     * @param ImageSize: 每一帧数据大小,用于UpdateImage等函数的参数申请空间大小
     * @return: int型
     *         -<em> <0 </em> fail，错误码
     *         -<em>  0 </em> succeed
     */
    EXPORT_API int FrameInit(int ImageSize);

    /**
     * @brief 链表反初始化,释放初始化申请的空间
     * @return: int型
     *         -<em> <0 </em> fail，错误码
     *         -<em>  0 </em> succeed
     */
    EXPORT_API int FrameUnit();

    /**
     * @brief 向链表中添加一帧数据
     * @param frameData: 调用者申请的内存空间,大小为FL_frameListInit参数ImageSize的值
     * @param matrix[16]: 纹理的变化矩阵
     * @return: int型
     *         -<em> <0 </em> fail，错误码
     *         -<em>  0 </em> succeed
     */
    EXPORT_API int UpdateImage(unsigned char* frameData, float matrix[16]);

    /**
     * @brief UI获取数据并显示
     * @param frameData: 调用者申请的内存空间,大小为FL_frameListInit参数ImageSize的值
     * @return: int型
     *         -<em> <0 </em> fail，错误码
     *         -<em>  0 </em> succeed
     */
    EXPORT_API int getBackground(unsigned char* frameData);

#ifdef __cplusplus
}
#endif

#endif
