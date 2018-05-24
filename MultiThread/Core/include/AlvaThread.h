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
// ����һ���ź���
#define sem_t_Alva sem_t
// �ź�����һ
#define sem_post_Alva(m_sem_t) sem_post(&(m_sem_t))
// �ź�����һ
#define sem_wait_Alva(m_sem_t) sem_wait(&(m_sem_t))

#define sem_init_Alva(m_sem_t, sem_t_init_value, ptr_char) sem_init(&(m_sem_t), 0, sem_t_init_value)

#define sem_destroy_Alva(m_sem_t, ptr_char) sem_destroy(&(m_sem_t))

#if CPU_ALVA == CPU_ARM32

#if Alva_Android_PLATFORM == Alva_Low_PLATFORM
#define atomic_int_Alva volatile int

#define  atomic_store_Alva(ptr, m) (*ptr = m)
#define  atomic_load_Alva(ptr, m) (m = *ptr)
// ���*ptr == *ptr_old ��*ptr = new, ����ֵΪ0, ��֮*ptr���䣬����Ϊ��0
#define atomic_compare_exchange_Alva(ptr, ptr_old, new_value) (0 != __atomic_cmpxchg(*ptr_old, new_value, ptr))
#endif//Alva_Android_PLATFORM == Alva_Low_PLATFORM

#if Alva_Android_PLATFORM == Alva_High_PLATFORM

#define atomic_int_Alva atomic_int

#define  atomic_store_Alva(ptr, m) atomic_store(ptr, m)
#define  atomic_load_Alva(ptr, m) (m) = atomic_load(ptr)
// ���*ptr == *ptr_old ��*ptr = new, ����ֵΪ0, ��֮*ptr���䣬����Ϊ��0
#define atomic_compare_exchange_Alva(ptr, ptr_old, new_value) (!atomic_compare_exchange_weak(ptr, ptr_old, new_value))

#endif//Alva_Android_PLATFORM == Alva_High_PLATFORM
#endif//CPU_ALVA == CPU_ARM32


#if CPU_ALVA == CPU_ARM64

#define atomic_int_Alva atomic_int

#define  atomic_store_Alva(ptr, m) atomic_store(ptr, m)
#define  atomic_load_Alva(ptr, m) (m) = atomic_load(ptr)
// ���*ptr == *ptr_old ��*ptr = new, ����ֵΪ0, ��֮*ptr���䣬����Ϊ��0
#define atomic_compare_exchange_Alva(ptr, ptr_old, new_value) (!atomic_compare_exchange_weak(ptr, ptr_old, new_value))
#endif//CPU_ALVA == CPU_ARM64
#endif//OS_ALVA == OS_Android

#if OS_ALVA == OS_iOS
// ����һ���ź���
#define sem_t_Alva* sem_t
// �ź�����һ
#define sem_post_Alva(m_sem_t) sem_post(m_sem_t)
// �ź�����һ
#define sem_wait_Alva(m_sem_t) sem_wait(m_sem_t)

#define sem_init_Alva(m_sem_t, sem_t_init_value, ptr_char) m_sem_t = sem_open(ptr_char, O_CREAT, S_IRWXU, sem_t_init_value)

#define sem_destroy_Alva(m_sem_t, ptr_char) sem_close(m_sem_t); sem_unlink(ptr_char)

#define atomic_int_Alva atomic_int

#define  atomic_store_Alva(ptr, m) atomic_store(ptr, m)
#define  atomic_load_Alva(ptr, m) (m) = atomic_load(ptr)
// ���*ptr == *ptr_old ��*ptr = new, ����ֵΪ0, ��֮*ptr���䣬����Ϊ��0
#define atomic_compare_exchange_Alva(ptr, ptr_old, new_value) (!atomic_compare_exchange_strong(ptr, ptr_old, new_value))
#endif//OS_ALVA == OS_iOS

#if OS_ALVA == OS_Android || OS_ALVA == OS_iOS
// ����һ��������
#define pThread_mutex_t_Alva pthread_mutex_t
// ��ʼ����
#define pThread_mutex_init_Alva(ptr_pthread_mutex_t) pthread_mutex_init(ptr_pthread_mutex_t, NULL)
// ����������ַ
#define pThread_mutex_lock_Alva(ptr_pThread_Mutex_Alva) pthread_mutex_lock(ptr_pThread_Mutex_Alva)
// ����������ַ
#define pThread_mutex_unlock_Alva(ptr_pThread_Mutex_Alva) pthread_mutex_unlock(ptr_pThread_Mutex_Alva)
// ���ٻ�����
#define pThread_mutex_destroy_Alva(ptr_pThread_Mutex_Alva) pthread_mutex_destroy(ptr_pThread_Mutex_Alva)

#define pThread_Alva pthread_t
//�����߳�
#define createThread_Alva(pThread, threadAttr, thread, para) pthread_create(pThread, threadAttr, thread, para)
//�ͷ��߳�
#define exitThread_Alva(p) pthread_exit(NULL)

//������ʽ�ȴ��߳���Դ����
#define pThreadJoin_Alva(pThread, p) pthread_join(pThread, NULL)
// ����t����
#define mSleep_Alva(t) usleep(t * 1000)

//��ʼ���߳���
#define pThreadInitLock_Alva(pAlvaThreadLock) pthread_cond_init(&((pAlvaThreadLock)->cond), NULL);\
													    pthread_mutex_init(&((pAlvaThreadLock)->mutex), NULL)
//�ͷ��߳���
#define pThreadDestroyLock_Alva(pAlvaThreadLock) pthread_mutex_destroy(&((pAlvaThreadLock)->mutex));\
														   pthread_cond_destroy(&((pAlvaThreadLock)->cond))
//�̵߳ȴ�
#define pThreadWait_Alva(pAlvaThreadLock) pthread_mutex_lock(&((pAlvaThreadLock)->mutex)); \
											   pthread_cond_wait(&((pAlvaThreadLock)->cond), &((pAlvaThreadLock)->mutex));\
											   pthread_mutex_unlock(&((pAlvaThreadLock)->mutex))
//�ź�����
#define resetEvent_Alva(pAlvaThreadLock) NULL
//�����ź�
#define pThreadSignal_Alva(pAlvaThreadLock) pthread_mutex_lock(&((pAlvaThreadLock)->mutex)); \
											pthread_cond_signal(&((pAlvaThreadLock)->cond)); \
											pthread_mutex_unlock(&((pAlvaThreadLock)->mutex))
#endif //OS_ALVA == OS_Android || OS_ALVA == OS_iOS

#if OS_ALVA == OS_Windows
// ����һ���ź���
#define sem_t_Alva HANDLE
// ��ʼ���ź�
#define sem_init_Alva(m_sem_t, sem_t_init_value, ptr_char) m_sem_t = CreateSemaphoreA(NULL, sem_t_init_value, 64, ptr_char);  
// �ź�����һ
#define sem_post_Alva(m_sem_t) ReleaseSemaphore(m_sem_t, 1, NULL)
// �ź�����һ
#define sem_wait_Alva(m_sem_t) WaitForSingleObject(m_sem_t, INFINITE)
// �ź�������
#define sem_destroy_Alva(m_sem_t, ptr_char) CloseHandle(m_sem_t)
// ����һ��������
#define pThread_mutex_t_Alva HANDLE
// ��ʼ����
#define pThread_mutex_init_Alva(ptr_pthread_mutex_t) (*ptr_pthread_mutex_t) = CreateMutex(NULL, FALSE, NULL)
// ����������ַ
#define pThread_mutex_lock_Alva(pThread_Mutex_Alva_ptr) WaitForSingleObject(*pThread_Mutex_Alva_ptr,INFINITE)
// ����������ַ
#define pThread_mutex_unlock_Alva(pThread_Mutex_Alva_ptr) ReleaseMutex(*pThread_Mutex_Alva_ptr)
// ���ٻ�����
#define pThread_mutex_destroy_Alva(ptr_pThread_Mutex_Alva) CloseHandle(*ptr_pThread_Mutex_Alva)

#define pThread_Alva HANDLE
//�����߳�
#define createThread_Alva(pThread, threadAttr, thread, para) (*pThread)=CreateThread(threadAttr, 0, (LPTHREAD_START_ROUTINE)thread, (void*)para, 0, NULL)
//�ͷ��߳�
#define exitThread_Alva(p) ExitThread(0)

//������ʽ�ȴ��߳���Դ����
#define pThreadJoin_Alva(pThread, p) WaitForSingleObject(pThread, 0); CloseHandle(pThread)
// ����t����
#define mSleep_Alva(t) Sleep(t)

#define atomic_int_Alva volatile int

#define  atomic_store_Alva(ptr, m) (*ptr = m)
#define  atomic_load_Alva(ptr, m) (m = *ptr)
// ���*ptr == *ptr_old ��*ptr = new, ����ֵΪ0, ��֮*ptr���䣬����Ϊ��0
#define atomic_compare_exchange_Alva(ptr, ptr_old, new_value) ((*ptr_old) != InterlockedCompareExchange(ptr, new_value, *ptr_old))

//��ʼ���߳���
#define pThreadInitLock_Alva(pAlvaThreadLock) ((pAlvaThreadLock)->cond)=CreateEvent(NULL, TRUE, FALSE, NULL)
//�ͷ��߳���
#define pThreadDestroyLock_Alva(pAlvaThreadLock) CloseHandle(((pAlvaThreadLock)->cond))

//�̵߳ȴ�
#define pThreadWait_Alva(pAlvaThreadLock) WaitForSingleObject(((pAlvaThreadLock)->cond), INFINITE)
//�����ź�
#define resetEvent_Alva(pAlvaThreadLock) ResetEvent(((pAlvaThreadLock)->cond))
//�����ź�
#define pThreadSignal_Alva(pAlvaThreadLock) SetEvent(((pAlvaThreadLock)->cond))
#endif//OS_ALVA == OS_Windows

#endif//_Alva_Thread_h_