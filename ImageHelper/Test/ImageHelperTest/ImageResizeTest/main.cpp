#include "Config.h"
#define LOG_TAG "main"
#include "Log.h"

#include "BmpHelper.h"
#include "ImageResize.h"

int main() {

    BMP mBmp;
    LoadBMP("Test_1.bmp", &mBmp, BIT24);

    int iWidth = mBmp.width;
    int iHeight = mBmp.height;
    int channels = mBmp.channels;
    int oWidth = 0, oHeight = 0;
    BMP oBmp;
    getSystemImageSize(iWidth, iHeight, &oWidth, &oHeight, 1);
    oBmp.channels = channels;
    oBmp.width = oWidth;
    oBmp.height = oHeight;
    oBmp.data = (unsigned char*)calloc(1, sizeof(unsigned char) * oWidth * oHeight * channels);
    ImageResize(mBmp.data, oBmp.data, iWidth, iHeight, oWidth, oHeight, channels);

    WriteBMP("Test_1_ImageResize.bmp", &oBmp, BIT24);

    FreeBMP(&mBmp);
    FreeBMP(&oBmp);
    return 0;
}