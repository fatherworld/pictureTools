#include "Config.h"

#include <stdlib.h>
#include "stdio_fl.h"
#include "memory_fl.h"

#define LOG_TAG "ImageResize"
#include "Log.h"
#include "ErrorTools.h"

#include "ImageResize.h"

#undef FILE_NUM
#define FILE_NUM 0xF600

#define CLAMP(v, minv, maxv) (v) > (maxv) ? (maxv) : ((v) < (minv) ? (minv) : (v))

/**
 * @brief ImageResize_Nearest_SC-基于最近邻插值的单通道的图像插值
 */
int ImageResize_Nearest_SC(unsigned char * srcImage, int iwidth, int iheight,
                           unsigned char * dstImage, int owidth, int oheight,
                           int channels) {

#undef FUNC_CODE
#define FUNC_CODE 0x01

    int i, j, oIndex, iXIndex, iYIndex;
    float fXIndex = 0.0f, fYIndex = 0.0f;

    float scale = owidth / (float)iwidth;
    if(scale <= 1.0f) { //下采样
        float step = 1.0f / scale;
        if((step - (int)step) < 0.01f) {//下采样整数倍，但这个判断条件感觉不正确
            int uStep = (int)step;
            oIndex = iXIndex = iYIndex = 0;

            for(j = 0; j < oheight; j++) {
                iXIndex = 0;
                for(i = 0; i < owidth; i++) {
                    dstImage[oIndex] = srcImage[iYIndex + iXIndex];
                    oIndex++;
                    iXIndex += uStep;
                }
                iYIndex += uStep * iwidth;
            }
        }
        else {
            oIndex = iXIndex = iYIndex = 0;
            fXIndex = fYIndex = 0.0f;

            for(j = 0; j < oheight; j++) {
                iXIndex = 0;
                fXIndex = 0.0f;

                for(i = 0; i < owidth; i++) {
                    dstImage[oIndex] = srcImage[iYIndex + iXIndex];
                    oIndex++;
                    fXIndex += step;
                    iXIndex  = (int)fXIndex;
                }
                fYIndex += step;
                iYIndex  = ((int)fYIndex) * iwidth;
            }
        }
    }
    else { //上采样
        float wScale = iwidth * 1.f / owidth;
        float hScale = iheight * 1.f / oheight;
        for(i = 0; i < oheight; i++) {
            for(j = 0; j < owidth; j++) {

                int outHY = (int)(i * hScale);
                int outWX = (int)(j * wScale);

                dstImage[i * owidth + j] = srcImage[outHY * iwidth + outWX];
            }
        }
    }

    return 0;
}

/**
* @brief ImageResize_Nearest_MC-基于最近邻插值的多通道的图像插值
*/
int ImageResize_Nearest_MC(unsigned char * srcImage, int iwidth, int iheight,
                           unsigned char * dstImage, int owidth, int oheight,
                           int channels) {

#undef FUNC_CODE
#define FUNC_CODE 0x02

    int i, j, k;

    float wScale = iwidth * 1.f / owidth;
    float hScale = iheight * 1.f / oheight;
    for(i = 0; i < oheight; i++) {
        for(j = 0; j < owidth; j++) {

            int outHY = (int)(i * hScale);
            int outWX = (int)(j * wScale);

            for(k = 0; k < channels; k++) {
                dstImage[(i * owidth + j) * channels + k] = srcImage[(outHY * iwidth + outWX)   * channels + k];
            }
        }
    }

    return 0;
}

/**
 * @brief ImageResize_Belinear_MC-基于双线性插值的多通道的图像插值
 */
int ImageResize_Belinear_MC(unsigned char * srcImage, int iwidth, int iheight,
                            unsigned char * dstImage, int owidth, int oheight,
                            int channels) {

#undef FUNC_CODE
#define FUNC_CODE 0x03

    int i, j, k;

    float wScale = iwidth * 1.f / owidth;
    float hScale = iheight * 1.f / oheight;
    for (i = 0; i < oheight; i++) {
        for (j = 0; j < owidth; j++) {

            float outHY = i * hScale;
            float outWX = j * wScale;

            int smallY = (int)outHY;
            int bigY = CLAMP(smallY + 1, 0, iheight - 1);
            int smallX = (int)outWX;
            int bigX = CLAMP(smallX + 1, 0, iwidth - 1);

            float xP1 = outWX - smallX;
            float xP2 = 1.f - xP1;
            float yP1 = outHY - smallY;
            float yP2 = 1.f - yP1;
            for(k = 0; k < channels; k ++){
                dstImage[(i * owidth + j) * channels + k] = (unsigned char)(CLAMP(yP2 * (xP1 * srcImage[(smallY * iwidth + bigX)   * channels + k] +
                                                                                         xP2 * srcImage[(smallY * iwidth + smallX) * channels + k]) +
                                                                                  yP1 * (xP1 * srcImage[(bigY * iwidth + bigX)     * channels + k] +
                                                                                         xP2 * srcImage[(bigY * iwidth + smallX)   * channels + k]),
                                                                                   0, 255.f));
            }
        }
    }

    return 0;
}

int ImageResize(unsigned char * srcImage, int srcW, int srcH,
                unsigned char * dstImage, int dstW, int dstH,
                int channels,
                ImageResizeType type) {

#undef FUNC_CODE
#define FUNC_CODE 0x04

    switch(type) {
        case IRT_Nearest:
            if(channels == 1) {
                return ImageResize_Nearest_SC(srcImage, srcW, srcH,
                                              dstImage, dstW, dstH,
                                              1);
            }
            else {
                return ImageResize_Nearest_MC(srcImage, srcW, srcH,
                                              dstImage, dstW, dstH,
                                              channels);
            }
            break;
        case IRT_Bilinear:
            return ImageResize_Belinear_MC(srcImage, srcW, srcH, dstImage, dstW, dstH, channels);
            break;
        case IRT_Bicubic:
            break;
        default:
            break;
    }

    return 0;
}

int getSystemImageSize(int width, int height, int* owidth, int* oheight, int platform) {

#undef FUNC_CODE
#define FUNC_CODE 0x05

    if (owidth == NULL || oheight == NULL) {
        OWN_ERROR_RETURN(0x001, "Null ptr!");
    }

    if (width > height) {
        if (platform == 0) {//android
            *owidth = 640;
            *oheight = height * *owidth / width;
        }
        else {//windows
            *owidth = 1280;
            *oheight = height * *owidth / width;
        }
    }else {
        if (platform == 0) {//android
            *oheight = 640;
            *owidth = width * *oheight / height;
        }else {//windows
            *oheight = 1280;
            *owidth = width * *oheight / height;
        }
    }

    return 0;
}