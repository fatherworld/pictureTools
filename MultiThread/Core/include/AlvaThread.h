#ifndef _Alva_Thread_h_
#define _Alva_Thread_h_

#if OS_ALVA == OS_Android
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <malloc.h>

#if CPU_ALVA == CPU_ARM32
#if Alva_Android_PLATFORM == Alva_Low_PLATFORM
#include <sys/atomics.h>
#endif
#if Alva_Android_PLATFORM == Alva_High_PLATFORM
#include <stdatomic.h>
#endif
#endif//CPU_ALVA == CPU_ARM32

#if CPU_ALVA == CPU_ARM64
#include <stdatomic.h>
#endif//CPU_ALVA == CPU_ARM64
#endif//OS_ALVA == OS_Android

#if OS_ALVA == OS_Windows
#include <Windows.h>
#endif

#if OS_ALVA == OS_iOS
#include <unistd.h>
#include <pthread.h>
#include <stdatomic.h>
#include <semaphore.h> 
#endif

typedef struct _Alva_Thread_Lock_
{
#if OS_ALVA == OS_Android || OS_ALVA == OS_iOS
	pthread_cond_t cond;
	pthread_mutex_t mutex;
#endif

#if OS_ALVA == OS_Windows
	HANDLE cond;
#endif
}AlvaThreadLock;

#if OS_ALVA == OS_Android
// 创建一个信号量
#define sem_t_Alva sem_t
// 信号量加一
#define sem_post_Alva(m_sem_t) sem_post(&(m_sem_t))
// 信号量减一
#define sem_wait_Alva(m_sem_t) sem_wait(&(m_sem_t))

#define sem_init_Alva(m_sem_t, sem_t_init_value, ptr_char) sem_init(&(m_sem_t), 0, sem_t_init_value)

#define sem_destroy_Alva(m_sem_t, ptr_char) sem_destroy(&(m_sem_t))

#if CPU_ALVA == CPU_ARM32

#if Alva_Android_PLATFORM == Alva_Low_PLATFORM
#define atomic_int_Alva volatile int

#define  atomic_store_Alva(ptr, m) (*ptr = m)
#define  atomic_load_Alva(ptr, m) (m = *ptr)
// 如果*ptr == *ptr_old 则*ptr = new, 返回值为0, 反之*ptr不变，返回为非0
#define atomic_compare_exchange_Alva(ptr, ptr_old, new_value) (0 != __atomic_cmpxchg(*ptr_old, new_value, ptr))
#endif//Alva_Android_PLATFORM == Alva_Low_PLATFORM

#if Alva_Android_PLATFORM == Alva_High_PLATFORM

#define atomic_int_Alva atomic_int

#define  atomic_store_Alva(ptr, m) atomic_store(ptr, m)
#define  atomic_load_Alva(ptr, m) (m) = atomic_load(ptr)
// 如果*ptr == *ptr_old 则*ptr = new, 返回值为0, 反之*ptr不变，返回为非0
#define atomic_compare_exchange_Alva(ptr, ptr_old, new_value) (!atomic_compare_exchange_weak(ptr, ptr_old, new_value))

#endif//Alva_Android_PLATFORM == Alva_High_PLATFORM
#endif//CPU_ALVA == CPU_ARM32


#if CPU_ALVA == CPU_ARM64

#define atomic_int_Alva atomic_int

#define  atomic_store_Alva(ptr, m) atomic_store(ptr, m)
#define  atomic_load_Alva(ptr, m) (m) = atomic_load(ptr)
// 如果*ptr == *ptr_old 则*ptr = new, 返回值为0, 反之*ptr不变，返回为非0
#define atomic_compare_exchange_Alva(ptr, ptr_old, new_value) (!atomic_compare_exchange_weak(ptr, ptr_old, new_value))
#endif//CPU_ALVA == CPU_ARM64
#endif//OS_ALVA == OS_Android

#if OS_ALVA == OS_iOS
// 创建一个信号量
#define sem_t_Alva* sem_t
// 信号量加一
#define sem_post_Alva(m_sem_t) sem_post(m_sem_t)
// 信号量减一
#define sem_wait_Alva(m_sem_t) sem_wait(m_sem_t)

#define sem_init_Alva(m_sem_t, sem_t_init_value, ptr_char) m_sem_t = sem_open(ptr_char, O_CREAT, S_IRWXU, sem_t_init_value)

#define sem_destroy_Alva(m_sem_t, ptr_char) sem_close(m_sem_t); sem_unlink(ptr_char)

#define atomic_int_Alva atomic_int

#define  atomic_store_Alva(ptr, m) atomic_store(ptr, m)
#define  atomic_load_Alva(ptr, m) (m) = atomic_load(ptr)
// 如果*ptr == *ptr_old 则*ptr = new, 返回值为0, 反之*ptr不变，返回为非0
#define atomic_compare_exchange_Alva(ptr, ptr_old, new_value) (!atomic_compare_exchange_strong(ptr, ptr_old, new_value))
#endif//OS_ALVA == OS_iOS

#if OS_ALVA == OS_Android || OS_ALVA == OS_iOS
// 创建一个互斥锁
#define pThread_mutex_t_Alva pthread_mutex_t
// 初始化锁
#define pThread_mutex_init_Alva(ptr_pthread_mutex_t) pthread_mutex_init(ptr_pthread_mutex_t, NULL)
// 上锁，传地址
#define pThread_mutex_lock_Alva(ptr_pThread_Mutex_Alva) pthread_mutex_lock(ptr_pThread_Mutex_Alva)
// 解锁，传地址
#define pThread_mutex_unlock_Alva(ptr_pThread_Mutex_Alva) pthread_mutex_unlock(ptr_pThread_Mutex_Alva)
// 销毁互斥锁
#define pThread_mutex_destroy_Alva(ptr_pThread_Mutex_Alva) pthread_mutex_destroy(ptr_pThread_Mutex_Alva)

#define pThread_Alva pthread_t
//创建线程
#define createThread_Alva(pThread, threadAttr, thread, para) pthread_create(pThread, threadAttr, thread, para)
//释放线程
#define exitThread_Alva(p) pthread_exit(NULL)

//阻塞方式等待线程资源回收
#define pThreadJoin_Alva(pThread, p) pthread_join(pThread, NULL)
// 休眠t毫秒
#define mSleep_Alva(t) usleep(t * 1000)

//初始化线程锁
#define pThreadInitLock_Alva(pAlvaThreadLock) pthread_cond_init(&((pAlvaThreadLock)->cond), NULL);\
													    pthread_mutex_init(&((pAlvaThreadLock)->mutex), NULL)
//释放线程锁
#define pThreadDestroyLock_Alva(pAlvaThreadLock) pthread_mutex_destroy(&((pAlvaThreadLock)->mutex));\
														   pthread_cond_destroy(&((pAlvaThreadLock)->cond))
//线程等待
#define pThreadWait_Alva(pAlvaThreadLock) pthread_mutex_lock(&((pAlvaThreadLock)->mutex)); \
											   pthread_cond_wait(&((pAlvaThreadLock)->cond), &((pAlvaThreadLock)->mutex));\
											   pthread_mutex_unlock(&((pAlvaThreadLock)->mutex))
//信号重置
#define resetEvent_Alva(pAlvaThreadLock) NULL
//发送信号
#define pThreadSignal_Alva(pAlvaThreadLock) pthread_mutex_lock(&((pAlvaThreadLock)->mutex)); \
											pthread_cond_signal(&((pAlvaThreadLock)->cond)); \
											pthread_mutex_unlock(&((pAlvaThreadLock)->mutex))
#endif //OS_ALVA == OS_Android || OS_ALVA == OS_iOS

#if OS_ALVA == OS_Windows
// 创建一个信号量
#define sem_t_Alva HANDLE
// 初始化信号
#define sem_init_Alva(m_sem_t, sem_t_init_value, ptr_char) m_sem_t = CreateSemaphoreA(NULL, sem_t_init_value, 64, ptr_char);  
// 信号量加一
#define sem_post_Alva(m_sem_t) ReleaseSemaphore(m_sem_t, 1, NULL)
// 信号量减一
#define sem_wait_Alva(m_sem_t) WaitForSingleObject(m_sem_t, INFINITE)
// 信号量销毁
#define sem_destroy_Alva(m_sem_t, ptr_char) CloseHandle(m_sem_t)
// 创建一个互斥锁
#define pThread_mutex_t_Alva HANDLE
// 初始化锁
#define pThread_mutex_init_Alva(ptr_pthread_mutex_t) (*ptr_pthread_mutex_t) = CreateMutex(NULL, FALSE, NULL)
// 上锁，传地址
#define pThread_mutex_lock_Alva(pThread_Mutex_Alva_ptr) WaitForSingleObject(*pThread_Mutex_Alva_ptr,INFINITE)
// 解锁，传地址
#define pThread_mutex_unlock_Alva(pThread_Mutex_Alva_ptr) ReleaseMutex(*pThread_Mutex_Alva_ptr)
// 销毁互斥锁
#define pThread_mutex_destroy_Alva(ptr_pThread_Mutex_Alva) CloseHandle(*ptr_pThread_Mutex_Alva)

#define pThread_Alva HANDLE
//创建线程
#define createThread_Alva(pThread, threadAttr, thread, para) (*pThread)=CreateThread(threadAttr, 0, (LPTHREAD_START_ROUTINE)thread, (void*)para, 0, NULL)
//释放线程
#define exitThread_Alva(p) ExitThread(0)

//阻塞方式等待线程资源回收
#define pThreadJoin_Alva(pThread, p) WaitForSingleObject(pThread, 0); CloseHandle(pThread)
// 休眠t毫秒
#define mSleep_Alva(t) Sleep(t)

#define atomic_int_Alva volatile int

#define  atomic_store_Alva(ptr, m) (*ptr = m)
#define  atomic_load_Alva(ptr, m) (m = *ptr)
// 如果*ptr == *ptr_old 则*ptr = new, 返回值为0, 反之*ptr不变，返回为非0
#define atomic_compare_exchange_Alva(ptr, ptr_old, new_value) ((*ptr_old) != InterlockedCompareExchange(ptr, new_value, *ptr_old))

//初始化线程锁
#define pThreadInitLock_Alva(pAlvaThreadLock) ((pAlvaThreadLock)->cond)=CreateEvent(NULL, TRUE, FALSE, NULL)
//释放线程锁
#define pThreadDestroyLock_Alva(pAlvaThreadLock) CloseHandle(((pAlvaThreadLock)->cond))

//线程等待
#define pThreadWait_Alva(pAlvaThreadLock) WaitForSingleObject(((pAlvaThreadLock)->cond), INFINITE)
//重置信号
#define resetEvent_Alva(pAlvaThreadLock) ResetEvent(((pAlvaThreadLock)->cond))
//发送信号
#define pThreadSignal_Alva(pAlvaThreadLock) SetEvent(((pAlvaThreadLock)->cond))
#endif//OS_ALVA == OS_Windows

#endif//_Alva_Thread_h_