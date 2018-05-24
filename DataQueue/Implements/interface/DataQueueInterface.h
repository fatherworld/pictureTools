#pragma once
#ifndef  __DATAQUEUEINTERFACE_H__
#define  __DATAQUEUEINTERFACE_H__

/**
 * @file  DataQueueInterface.h
 * @brief �������:�ȳ�ʼ������FrameInit��Ȼ��������������������UpdateImage��
                   ����getBackgroundʹ�����ʾ���ݣ� �������˳�֮ǰ���÷���ʼ��FrameUnit��

                   @see FrameInit     ����ĳ�ʼ��
                   @see UpdateImage   ���������������
                   @see getBackground UI��ȡ���ݲ���ʾ
                   @see FrameUnit     �����������˳�ʱ���÷���ʼ��

 */

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief �����ʼ��
     * @param ImageSize: ÿһ֡���ݴ�С,����UpdateImage�Ⱥ����Ĳ�������ռ��С
     * @return: int��
     *         -<em> <0 </em> fail��������
     *         -<em>  0 </em> succeed
     */
    EXPORT_API int FrameInit(int ImageSize);

    /**
     * @brief ������ʼ��,�ͷų�ʼ������Ŀռ�
     * @return: int��
     *         -<em> <0 </em> fail��������
     *         -<em>  0 </em> succeed
     */
    EXPORT_API int FrameUnit();

    /**
     * @brief �����������һ֡����
     * @param frameData: ������������ڴ�ռ�,��СΪFL_frameListInit����ImageSize��ֵ
     * @param matrix[16]: ����ı仯����
     * @return: int��
     *         -<em> <0 </em> fail��������
     *         -<em>  0 </em> succeed
     */
    EXPORT_API int UpdateImage(unsigned char* frameData, float matrix[16]);

    /**
     * @brief UI��ȡ���ݲ���ʾ
     * @param frameData: ������������ڴ�ռ�,��СΪFL_frameListInit����ImageSize��ֵ
     * @return: int��
     *         -<em> <0 </em> fail��������
     *         -<em>  0 </em> succeed
     */
    EXPORT_API int getBackground(unsigned char* frameData);

#ifdef __cplusplus
}
#endif

#endif
