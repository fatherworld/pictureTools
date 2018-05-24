#ifndef _LOG_H_FL_
#define _LOG_H_FL_

/** 
 * @file Log.h
 * @brief 统一Log显示
 * @Details 警告：所有函数（除LOGS）的最终显示字符串最大不超过1024个字节
 * @author
 * @version   1.0.0.1 \n
 *---------------------------------------------------                  \n 
 * Change History :                                                    \n 
 * <Date>     | <Version> | <Author>    |   <Change>                   \n 
 *----------------------------------------------------                 \n 
 * 2018/01/04 | 1.0.0.1   |             |  Change Max size of Buffer   \n 
 */

#include "Config.h"

#if OS_ALVA==OS_Android

#include <jni.h>
#include <android/log.h>

#ifdef AlvaPublic
#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG AlvaTag
#else
#ifndef LOG_TAG
#define LOG_TAG "DEFAULT_TAG"
#endif
#endif

#if DebugInfo_Switch == Release_Alva

#define  LOGI(...)  
#define  LOGD(...)  
#define  LOGV(...)  
#define  LOGE(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
/** 
 * @brief 进度显示
 * @param ... 参数同printf
 *
 * @return 
 * @note 此函数将首先返回当前行首，再显示内容（即：显示内容前加\r）
 */
#define  LOGP(...) 
/** 
 * @brief 进度完成显示
 * @param ... 参数同printf
 *
 * @return 
 * @note 此函数将首先结束当前行，再显示内容（即：显示内容前加\n）
 */
#define  LOGPD(...) 
#define  LOGS(mString)
/** 
 * @brief 直接显示LOG，不添加任何前缀及后缀
 * @param ... 参数同printf
 *
 * @return 
 */
#define LOGN(...)

#else

#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define  LOGV(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
/** 
 * @brief 进度显示
 * @param ... 参数同printf
 *
 * @return 
 * @note 此函数将首先返回当前行首，再显示内容（即：显示内容前加\r）
 */
#define  LOGP(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
/** 
 * @brief 进度完成显示
 * @param ... 参数同printf
 *
 * @return 
 * @note 此函数将首先结束当前行，再显示内容（即：显示内容前加\n）
 */
#define  LOGPD(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#define  LOGS(mString)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG, "%s", mString)
/** 
 * @brief 直接显示LOG，不添加任何前缀及后缀
 * @param ... 参数同printf
 *
 * @return 
 */
#define LOGN(...)
#endif //DebugInfo_Switch
#endif //OS_ALVA

#if OS_ALVA==OS_Windows
#include <Windows.h>
#include "stdio_fl.h"
#define ALVALOGMAXLENGTH 1024
#if DebugInfo_Switch == Release_Alva

#define  LOGI(...)  
#define  LOGD(...)  
#define  LOGV(...)  
#define  LOGE(...)  {\
                        char t_log_temp_do_not_use[ALVALOGMAXLENGTH] = {0};      \
                        int t_log_num_do_not_use = sprintf_fl(t_log_temp_do_not_use, ALVALOGMAXLENGTH, "%-40s:", LOG_TAG); \
                        sprintf_fl(&(t_log_temp_do_not_use[t_log_num_do_not_use]), ALVALOGMAXLENGTH - t_log_num_do_not_use, __VA_ARGS__); \
                        printf_fl("%s", t_log_temp_do_not_use);OutputDebugStringA(t_log_temp_do_not_use);    \
                    }
/** 
 * @brief 进度显示
 * @param ... 参数同printf
 *
 * @return 
 * @note 此函数将首先返回当前行首，再显示内容（即：显示内容前加\r）
 */
#define  LOGP(...)  {\
                        char t_log_temp_do_not_use[ALVALOGMAXLENGTH] = {0};      \
                        int t_log_num_do_not_use = sprintf_fl(t_log_temp_do_not_use, ALVALOGMAXLENGTH, "\r%-40s:", LOG_TAG); \
                        sprintf_fl(&(t_log_temp_do_not_use[t_log_num_do_not_use]), ALVALOGMAXLENGTH - t_log_num_do_not_use, __VA_ARGS__); \
                        printf_fl("%s", t_log_temp_do_not_use);OutputDebugStringA(t_log_temp_do_not_use);    \
                    }
/** 
 * @brief 进度完成显示
 * @param ... 参数同printf
 *
 * @return 
 * @note 此函数将首先结束当前行，再显示内容（即：显示内容前加\n）
 */
#define  LOGPD(...) {\
                        char t_log_temp_do_not_use[ALVALOGMAXLENGTH] = {0};      \
                        int t_log_num_do_not_use = sprintf_fl(t_log_temp_do_not_use, ALVALOGMAXLENGTH, "\n%-40s:", LOG_TAG); \
                        sprintf_fl(&(t_log_temp_do_not_use[t_log_num_do_not_use]), ALVALOGMAXLENGTH - t_log_num_do_not_use, __VA_ARGS__); \
                        printf_fl("%s", t_log_temp_do_not_use);OutputDebugStringA(t_log_temp_do_not_use);    \
                    }
#define  LOGS(mString)
/** 
 * @brief 直接显示LOG，不添加任何前缀及后缀
 * @param ... 参数同printf
 *
 * @return 
 */
#define  LOGN(...)  {\
                        char t_log_temp_do_not_use[ALVALOGMAXLENGTH] = {0};      \
                        sprintf_fl(t_log_temp_do_not_use, ALVALOGMAXLENGTH, __VA_ARGS__); \
                        printf_fl("%s", t_log_temp_do_not_use);OutputDebugStringA(t_log_temp_do_not_use);    \
                    }
#else

#define  LOGI(...)  {\
                        char t_log_temp_do_not_use[ALVALOGMAXLENGTH] = {0};      \
                        int t_log_num_do_not_use = sprintf_fl(t_log_temp_do_not_use, ALVALOGMAXLENGTH, "%-40s:", LOG_TAG); \
                        sprintf_fl(&(t_log_temp_do_not_use[t_log_num_do_not_use]), ALVALOGMAXLENGTH - t_log_num_do_not_use, __VA_ARGS__); \
                        printf_fl("%s", t_log_temp_do_not_use);OutputDebugStringA(t_log_temp_do_not_use);    \
                    }
#define  LOGD(...)  {\
                        char t_log_temp_do_not_use[ALVALOGMAXLENGTH] = {0};      \
                        int t_log_num_do_not_use = sprintf_fl(t_log_temp_do_not_use, ALVALOGMAXLENGTH, "%-40s:", LOG_TAG); \
                        sprintf_fl(&(t_log_temp_do_not_use[t_log_num_do_not_use]), ALVALOGMAXLENGTH - t_log_num_do_not_use, __VA_ARGS__); \
                        printf_fl("%s", t_log_temp_do_not_use);OutputDebugStringA(t_log_temp_do_not_use);    \
                    }
#define  LOGV(...)  {\
                        char t_log_temp_do_not_use[ALVALOGMAXLENGTH] = {0};      \
                        int t_log_num_do_not_use = sprintf_fl(t_log_temp_do_not_use, ALVALOGMAXLENGTH, "%-40s:", LOG_TAG); \
                        sprintf_fl(&(t_log_temp_do_not_use[t_log_num_do_not_use]), ALVALOGMAXLENGTH - t_log_num_do_not_use, __VA_ARGS__); \
                        printf_fl("%s", t_log_temp_do_not_use);OutputDebugStringA(t_log_temp_do_not_use);    \
                    }
#define  LOGE(...)  {\
                        char t_log_temp_do_not_use[ALVALOGMAXLENGTH] = {0};      \
                        int t_log_num_do_not_use = sprintf_fl(t_log_temp_do_not_use, ALVALOGMAXLENGTH, "%-40s:", LOG_TAG); \
                        sprintf_fl(&(t_log_temp_do_not_use[t_log_num_do_not_use]), ALVALOGMAXLENGTH - t_log_num_do_not_use, __VA_ARGS__); \
                        printf_fl("%s", t_log_temp_do_not_use);OutputDebugStringA(t_log_temp_do_not_use);    \
                    }
/** 
 * @brief 进度显示
 * @param ... 参数同printf
 *
 * @return 
 * @note 此函数将首先返回当前行首，再显示内容（即：显示内容前加\r）
 */
#define  LOGP(...)  {\
                        char t_log_temp_do_not_use[ALVALOGMAXLENGTH] = {0};      \
                        int t_log_num_do_not_use = sprintf_fl(t_log_temp_do_not_use, ALVALOGMAXLENGTH, "\r%-40s:", LOG_TAG); \
                        sprintf_fl(&(t_log_temp_do_not_use[t_log_num_do_not_use]), ALVALOGMAXLENGTH - t_log_num_do_not_use, __VA_ARGS__); \
                        printf_fl("%s", t_log_temp_do_not_use);OutputDebugStringA(t_log_temp_do_not_use);    \
                    }
/** 
 * @brief 进度完成显示
 * @param ... 参数同printf
 *
 * @return 
 * @note 此函数将首先结束当前行，再显示内容（即：显示内容前加\n）
 */
#define  LOGPD(...) {\
                        char t_log_temp_do_not_use[ALVALOGMAXLENGTH] = {0};      \
                        int t_log_num_do_not_use = sprintf_fl(t_log_temp_do_not_use, ALVALOGMAXLENGTH, "\n%-40s:", LOG_TAG); \
                        sprintf_fl(&(t_log_temp_do_not_use[t_log_num_do_not_use]), ALVALOGMAXLENGTH - t_log_num_do_not_use, __VA_ARGS__); \
                        printf_fl("%s", t_log_temp_do_not_use);OutputDebugStringA(t_log_temp_do_not_use);    \
                    }
#define  LOGS(mString)  {\
                        printf_fl("%s", mString);OutputDebugStringA(mString);    \
                    }
/** 
 * @brief 直接显示LOG，不添加任何前缀及后缀
 * @param ... 参数同printf
 *
 * @return 
 */
#define  LOGN(...)  {\
                        char t_log_temp_do_not_use[ALVALOGMAXLENGTH] = {0};      \
                        sprintf_fl(t_log_temp_do_not_use, ALVALOGMAXLENGTH, __VA_ARGS__); \
                        printf_fl("%s", t_log_temp_do_not_use);OutputDebugStringA(t_log_temp_do_not_use);    \
                    }
#endif //DebugInfo_Switch
#endif //OS_ALVA

#if OS_ALVA==OS_Linux || OS_ALVA==OS_iOS

#if DebugInfo_Switch == Release_Alva

#define  LOGI(...)  
#define  LOGD(...)  
#define  LOGV(...)  
#define  LOGE(...) printf_fl(__VA_ARGS__);
/** 
 * @brief 进度显示
 * @param ... 参数同printf
 *
 * @return 
 * @note 此函数将首先返回当前行首，再显示内容（即：显示内容前加\r）
 */
#define  LOGP(...) 
/** 
 * @brief 进度完成显示
 * @param ... 参数同printf
 *
 * @return 
 * @note 此函数将首先结束当前行，再显示内容（即：显示内容前加\n）
 */
#define  LOGPD(...) 
#define  LOGS(mString)  
/** 
 * @brief 直接显示LOG，不添加任何前缀及后缀
 * @param ... 参数同printf
 *
 * @return 
 */
#define  LOGN(...)

#else

#define  LOGI(...)  printf_fl(__VA_ARGS__);
#define  LOGD(...)  printf_fl(__VA_ARGS__);
#define  LOGV(...)  printf_fl(__VA_ARGS__);
#define  LOGE(...)  printf_fl(__VA_ARGS__);
/** 
 * @brief 进度显示
 * @param ... 参数同printf
 *
 * @return 
 * @note 此函数将首先返回当前行首，再显示内容（即：显示内容前加\r）
 */
#define  LOGP(...)  printf_fl(__VA_ARGS__);
/** 
 * @brief 进度完成显示
 * @param ... 参数同printf
 *
 * @return 
 * @note 此函数将首先结束当前行，再显示内容（即：显示内容前加\n）
 */
#define  LOGPD(...) printf_fl(__VA_ARGS__);
#define  LOGS(mString) printf_fl("%s\n", mString);
/** 
 * @brief 直接显示LOG，不添加任何前缀及后缀
 * @param ... 参数同printf
 *
 * @return 
 */
#define LOGN(...)
#endif //DebugInfo_Switch
#endif //OS_ALVA

#endif //_LOG_H_FL_