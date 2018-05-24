#include "Config.h"

#include <stdlib.h>
#include "stdio_fl.h"
#include "memory_fl.h"

#define LOG_TAG "ImageResize"
#include "Log.h"
#include "ErrorTools.h"

#include "ImageResize.h"

#define CLAMP(v, minv, maxv) (v) > (maxv) ? (maxv) : ((v) < (minv) ? (minv) : (v))

int ImageResize(unsigned char * srcImage, unsigned char * destImage, int iwidth, int iheight, int owidth, int oheight, int channels) {
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
                destImage[(i * owidth + j) * channels + k] = (unsigned char)(CLAMP(yP2 * (xP1 * srcImage[(smallY * iwidth + bigX)   * channels + k] +
                                                                                          xP2 * srcImage[(smallY * iwidth + smallX) * channels + k]) +
                                                                                   yP1 * (xP1 * srcImage[(bigY * iwidth + bigX)     * channels + k] +
                                                                                          xP2 * srcImage[(bigY * iwidth + smallX)   * channels + k]),
                                                                                   0, 255.f));
            }
        }
    }

    return 0;
}

int getSystemImageSize(int width, int height, int* owidth, int* oheight, int platform) {
    if (owidth == NULL || oheight == NULL) {
        return -1;
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