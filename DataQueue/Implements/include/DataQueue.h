#pragma once
#ifndef  __DATAQUEUE_H__
#define  __DATAQUEUE_H__

/**
 * @file  DataQueue.h
 * @brief �㷨����:��ע���㷨FL_regAlgorithm��Ȼ��ȡҪ���������FL_getData�����ݴ�����֮�����ô���
                   �Ľ��FL_setResultData��Ȼ����ⲿ�ӿڵ��û�ȡ������FL_updateResult�����ע
                   ���㷨FL_unRegAlgorithm������㷨ע��֮�󣬲������㷨���򶳽��㷨FL_freezeAlgor
                   ithm���������㷨�������㷨FL_activeAlgorithm���ɡ�

                   ע���㷨ʱ���㷨Ĭ�ϼ��ͬ���㷨Ҫ���㷨�����ٶȿ죬��������ٶ�����������һ
                   ��һ�������������ͬ���㷨�����ݺʹ�������ƥ��ģ�����ͬ���㷨ע��֮�󣬲�����
                   ���ݣ���ʱUI����ʾ���棬��ʱ����Զ����㷨������������ʾ���ȵ������㷨ʱ���
                   �첽�㷨���㷨�����ݺͽ���ǲ�ȷ��ƥ��ģ��п��ܴ˴�������ʾ�Ľ����֮ǰ����Ľ����

                   �л��㷨(FL_switchoverAlgorithm)��������ֱ�ӵ��ã�������㷨��FL_getData��FL_setRes
                   ultData֮����ã����Ƽ�ʹ�á�
                   FL_getData��FL_setResultData����ɶԵ���
                   FL_regAlgorithm��FL_unRegAlgorithmҲ����ɶԳ���
                   
                   @see FL_regAlgorithm         ��ע���㷨
                   @see FL_freezeAlgorithm      �����㷨(������Ҫ����)
                   @see FL_activeAlgorithm      �����㷨(�Ͷ����㷨���Ӧ)
                   @see FL_getData              �ٴ�������ȡ���ݴ���
                   @see FL_setResultData        Ȼ�����ô�����
                   @see FL_updateResult         ȡ���ݵĴ�����
                   @see FL_unRegAlgorithm       ��ע���Լ����㷨
                   @see FL_switchoverAlgorithm  �л��㷨(���Ƽ�ʹ��)

          ��������:���㷨ע��֮�����������㷨������������FL_setDepend��Ȼ����Ի�ȡ�������㷨�����С
                   FL_getDependResultSize���Լ����������㷨���FL_getDependResult��

                   @see FL_setDepend           �����������㷨,��ע���㷨֮����� 
                   @see FL_getDependResultSize ����������㷨�Ľ����С 
                   @see FL_getDependResult     ����������㷨�Ľ��

 */

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief ö������,Ϊע�ắ��FL_regAlgorithm�ĵڶ�������
     */
    typedef enum _AlgorithmType_ {
        AT_Sync = 0,           /*!< ͬ���㷨 */
        AT_Asyn = 1,           /*!< �첽�㷨 */
    }AlgorithmType;

    /**
     * @brief ע���㷨�� ע����㷨Ĭ��Ϊ����״̬
     * @param algorithmName: �㷨����
     * @param resultSize: �㷨����֮������С
     * @param algorithmType: AT_Sync ͬ��  AT_Asyn �첽 @see AlgorithmType
     * @param initResult: �㷨�������ĳ�ʼֵ
     * @return: int�ͣ�ע���㷨��������algorithmIndex
     *         -<em> < 0 </em> fail��������
     *         -<em> >=0 </em> succeed���㷨������
     */
    int FL_regAlgorithm(char algorithmName[256], int resultSize, AlgorithmType algorithmType, void* initResult);

    /**
     * @brief ��ע���㷨,�ͷ�ע���㷨ʱ����Ŀռ�
     * @param algorithmIndex: ע���㷨ʱFL_regAlgorithm�����ķ���ֵ @see FL_regAlgorithm
     * @return: int��
     *         -<em> <0 </em> fail��������
     *         -<em>  0 </em> succeed
     */
    int FL_unRegAlgorithm(int algorithmIndex);

    /**
     * @brief ����������һ֡�������ݴ���
     * @param algorithmIndex: ע���㷨ʱFL_regAlgorithm�����ķ���ֵ
     * @param frameData: ������������ڴ�ռ�,��СΪFrameInit����ImageSize��ֵ @see FrameInit
     * @param matrix[16]: ����ı仯����
     * @return: int��
     *         -<em> <0 </em> fail��������
     *         -<em>  0 </em> succeed
     */
    int FL_getData(int algorithmIndex, unsigned char* frameData, float matrix[16]);

    /**
     * @brief ����֡��Ӵ�����
     * @param algorithmIndex: ע���㷨ʱFL_regAlgorithm�����ķ���ֵ
     * @param frameResultData: ������������ڴ�ռ�,��СΪFL_regAlgorithm��������resultSize��ֵ @see FL_regAlgorithm
     * @return: int��
     *         -<em> <0 </em> fail��������
     *         -<em>  0 </em> succeed
     */
    int FL_setResultData(int algorithmIndex, void * frameResultData);

    /**
     * @brief UI��ȡ���ݽ������ʾ
     * @param algorithmIndex: ע���㷨ʱFL_regAlgorithm�����ķ���ֵ
     * @param frameResultData: ������������ڴ�ռ�,��СΪFL_regAlgorithm��������resultSize��ֵ @see FL_regAlgorithm
     * @return: int��
     *         -<em> <0 </em> fail��������
     *         -<em>  0 </em> succeed
     */
    int FL_updateResult(int algorithmIndex, void* frameResultData);

    /**
     * @brief �����㷨
     * @param algorithmIndex: ע���㷨ʱFL_regAlgorithm�����ķ���ֵ @see FL_regAlgorithm
     * @return: int��
     *         -<em> <0 </em> fail��������
     *         -<em>  0 </em> succeed
     */
    int FL_activeAlgorithm(int algorithmIndex);

    /**
     * @brief �����㷨
     * @param algorithmIndex: ע���㷨ʱFL_regAlgorithm�����ķ���ֵ @see FL_regAlgorithm
     * @return: int��
     *         -<em> <0 </em> fail��������
     *         -<em>  0 </em> succeed
     */
    int FL_freezeAlgorithm(int algorithmIndex);

    /**
     * @brief �л��㷨
     * @param algorithmIndex: ע���㷨ʱFL_regAlgorithm�����ķ���ֵ @see FL_regAlgorithm
     * @param algorithmType: AT_Sync ͬ��  AT_Asyn �첽 @see AlgorithmType
     * @return: int��
     *         -<em> <0 </em> fail��������
     *         -<em>  0 </em> succeed
     */
    int FL_switchoverAlgorithm(int algorithmIndex, AlgorithmType algorithmType);

#ifdef __cplusplus
}
#endif

#endif
