#include "Config.h"

#include <stdlib.h>
#include <jni.h>
#include <android/log.h>

#define  LOG_TAG    "SharedModule_Jni"
#include "Log.h"

#include "SharedMachineCodeInterface.h"

#ifndef CLASSNAME
#error CLASSNAME was not defined, please define CLASSNAME first like -DCLASSNAME=com_alvasystems_ar
#endif

#define JNIFUNC_NOTUSE2(CLZ, FUN) JNIEXPORT JNICALL Java_##CLZ##_##FUN
#define JNIFUNC_NOTUSE1(CLZ, FUN) JNIFUNC_NOTUSE2(CLZ, FUN)
#define JNIFUNC(FUNCNAME) JNIFUNC_NOTUSE1(CLASSNAME, FUNCNAME)

extern "C" {
    jint
    JNIFUNC(GetMachineCode)(JNIEnv* env, jobject obj, jbyteArray code) {
        int err = 0;
        jbyte *mcode = env->GetByteArrayElements(code, NULL);
        if (mcode == NULL) {
            return -1;
        }

        err = cGetMachineCode((unsigned char*)mcode);

        env->ReleaseByteArrayElements(code, mcode, 0);

        return err;
    }
}