#include "Config.h"
#include <jni.h>
#include <android/log.h>
#include <malloc.h>
#include <string.h>

#define LOG_TAG "Palm_Interface"
#include "Log.h"
#include "Format.h"
#include "AlvaThread.h"


#ifndef CLASSNAME
#error CLASSNAME was not defined, please define CLASSNAME first like -DCLASSNAME=com_alvasystems_fr_FRJni
#endif

#define JNIFUNC_NOTUSE2(CLZ, FUN) JNIEXPORT JNICALL Java_##CLZ##_##FUN
#define JNIFUNC_NOTUSE1(CLZ, FUN) JNIFUNC_NOTUSE2(CLZ, FUN)
#define JNIFUNC(FUNCNAME) JNIFUNC_NOTUSE1(CLASSNAME, FUNCNAME)




int g = 0;
sem_t_Alva sem0;
sem_t_Alva sem1;
pThread_mutex_t_Alva mutex0;
int first = 0;
int isbusy = 0;

void* IR_Thread1(void* arg) {

	for (int i = 0; i < 100; i++)
	{
		sem_post_Alva(sem0);
		mSleep_Alva(100);
		pThread_mutex_lock_Alva(&mutex0);
		g++;
		pThread_mutex_unlock_Alva(&mutex0);
		LOGE("thread1 -------------- g = %d\n", g);
		sem_wait_Alva(sem1);
	}
	exitThread_Alva(0);
	return NULL;
}
void* IR_Thread2(void* arg) {
	
	for (int i = 0; i < 100; i ++)
	{
		sem_wait_Alva(sem0);
		//mSleep_Alva(1000);
		pThread_mutex_lock_Alva(&mutex0);
		g++;
		LOGE("thread2 ++++++++++++++ g = %d\n", g);
		pThread_mutex_unlock_Alva(&mutex0);
		sem_post_Alva(sem1);
	}
	exitThread_Alva(0);
	return NULL;
}
atomic_int_Alva gAtomic;
void* IR_Thread3(void* arg) {

	for (int i = 0; i < 100; i++)
	{
	    int tValue = -1;
	    atomic_load_Alva(&gAtomic, tValue);
	    LOGE("thread3 ... gAtomic = %d\n", tValue);
		tValue ++;
		atomic_store_Alva(&gAtomic, tValue);
		mSleep_Alva(5);
	}
	exitThread_Alva(0);
	return NULL;
}
void* IR_Thread4(void* arg) {
	
	for (int i = 0; i < 100; i ++)
	{
	    int tValue = -1;
	    atomic_load_Alva(&gAtomic, tValue);
	    LOGE("thread4 ... gAtomic = %d\n", tValue);
		tValue ++;
		atomic_store_Alva(&gAtomic, tValue);
	}
	exitThread_Alva(0);
	return NULL;
}

extern "C" {

	jlong
		JNIFUNC(PalmInit)(JNIEnv * env, jobject obj) {

		LOGE("zhangli PalmInit PalmInit ");
		return (jlong)0;
	}

	jint 
		JNIFUNC(PalmSetDataInfo)(JNIEnv * env, jobject obj, jlong PalmInfo, jint width, jint height, jint format, jint mOrcation, jint mCameraId) {

		int ret = 0;

		//ret = PalmSetDataInfo(pInfo, width, height, format, mOrcation, mCameraId);
		LOGE("zhangli PalmSetDataInfo width height format Orcation mCamIc = %d %d %d %d %d", width, height, format, mOrcation, mCameraId);
		return ret;
	}

	jintArray
		JNIFUNC(PalmPreform)(JNIEnv * env, jobject obj, jlong PalmInfo, jbyteArray data, jint rectNum, jintArray srcRectCoord) {

		if (isbusy == 1)
		{
			return 0;

		}
		isbusy = 1;
		//void * pInfo = (void *)PalmInfo;
#if 0
		if (first == 0)
		{
			sem_init_Alva(&sem0, 0);
			sem_init_Alva(&sem1, 0);

			pThread_mutex_init_Alva(&mutex0);
			pThread_Alva mThread1;
			pThread_Alva mThread2;



			createThread_Alva(&mThread1, NULL, IR_Thread1, NULL);
			createThread_Alva(&mThread2, NULL, IR_Thread2, NULL);

			mSleep_Alva(10);

			mSleep_Alva(1000 * 5);

			LOGE("join ++++++++++++++ 0000000000000*************************"); 
			sem_destroy_Alva(&sem0);
			sem_destroy_Alva(&sem1);
			LOGE("join ++++++++++++++ 111111111111 *************************");
			pThreadJoin_Alva(mThread1, NULL);
			pThreadJoin_Alva(mThread2, NULL);

			LOGE("join ++++++++++++++ 22222222222222*************************");
			first = 1;
		}
		atomic_int_Alva new_v = 5;
		//int new_v = 5;
		int old = 5;
		int value = 7;
		int ret = 0;
		//__atomic_cmpxchg(old, value, &new_v);
		atomic_compare_exchange_Alva(&new_v, &old, value);

		atomic_store_Alva(&new_v, value);
		
		
		//atomic_compare_exchange_weak(&ppp, &old, value);
		LOGE("22222222222222 ret = %d new = %d \n", ret, new_v);

		isbusy = 0; 
#else
    pThread_Alva mThread3;
    pThread_Alva mThread4;

	int value = 0;
    atomic_store_Alva(&gAtomic, value);
	int tValue = -1;
	atomic_load_Alva(&gAtomic, tValue);
	LOGE("start ... gAtomic = %d\n", tValue);
    createThread_Alva(&mThread3, NULL, IR_Thread3, NULL);
    createThread_Alva(&mThread4, NULL, IR_Thread4, NULL);
	
    pThreadJoin_Alva(mThread3, NULL);
    pThreadJoin_Alva(mThread4, NULL);
#endif
		return 0;

		//LOGE("width = %d height = %d *****333333333333", width, height);
		//return 0;
	}

	jint
		JNIFUNC(PalmUint)(JNIEnv * env, jobject obj, jlong PalmInfo) {

	


		return 0;
	}

	jint
		JNIFUNC(GetPalmNum)(JNIEnv * env, jobject obj, jlong PalmInfo) {



		return 0;
	}

	jintArray
		JNIFUNC(GetPalmFlag)(JNIEnv * env, jobject obj, jlong PalmInfo) {



		return 0;
	}



	jint
		JNIFUNC(FRSetDataInfo)(JNIEnv * env, jobject obj, jint width, jint height, jint format, jint oraction, jint cameraId) {

		LOGE("zhangli jni FRSetDataInfo = %d %d %d %d %d", width, height, format, oraction, cameraId);
		return 0;
	}

	jint
		JNIFUNC(FRPerform)(JNIEnv * env, jobject obj, jint faceNumber, jfloatArray boxpoints, jbyteArray previewYuv, jbyteArray previewDepth) {

		LOGE("zhangli jni FRPerform");
		return 0;
	}


}