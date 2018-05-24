#ifndef _Image_Resize_h_
#define _Image_Resize_h_

#ifdef AlvaPublic
#include "ImageHelper_Hide.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief ImageResizeType
     *        图像Resize的算法类型
     */
    typedef enum _ImageResizeType_ {
        IRT_Nearest                 = 0, //最近邻插值
        IRT_Bilinear                   , //双线性插值
        IRT_Bicubic                    , //双三次插值
    }ImageResizeType;

    /**
      * @brief 对unsigned char数据类型的图像进行临近插值
      * @param srcImage: 输入unsigned char数据类型的图像
      * @param srcW: 输入图像的宽
      * @param srcH: 输入图像的高
      * @param dstImage: 临近插值后输出的unsigned char数据类型的图像
      * @param dstW: 输出图像的宽
      * @param dstH: 输出图像的高
      * @param channels: 图像的维度
      * @param type: 插值算法类型，详见@ImageResizeType
      * @return: 0
      */
    int ImageResize(unsigned char *srcImage, int srcW, int srcH,
                    unsigned char *dstImage, int dstW, int dstH,
                    int channels,
                    ImageResizeType type);

    /**
      * @brief 宽或高最大为1280
      * @param width: 输入的宽
      * @param height: 输入的高
      * @param owidth: 输出图像的宽
      * @param srcH: 输出图像的高
      * @param platform: 平台0:android   其它windows
      * @return: 0
      */
    int getSystemImageSize(int width, int height, int* owidth, int* oheight, int platform);
#ifdef __cplusplus
}
#endif

#endif