#include "Config.h"
#include <stdio.h>
#include <stdlib.h>
#include "AlvaThread.h"



int g = 0;
sem_t_Alva sem0;
sem_t_Alva sem1;
pThread_mutex_t_Alva mutex0;

void* IR_Thread1(void* arg) {

    for (int i = 0; i < 100; i++)
    {
        sem_post_Alva(sem0);
        mSleep_Alva(1000);
        pThread_mutex_lock_Alva(&mutex0);
        g++;
        pThread_mutex_unlock_Alva(&mutex0);
        printf("thread1 -------------- g = %d\n", g);
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
        printf("thread2 ++++++++++++++ g = %d\n", g);
        pThread_mutex_unlock_Alva(&mutex0);
        sem_post_Alva(sem1);
    }
    exitThread_Alva(0);
    return NULL;
}

int main()
{
    int v = 0;
    
    sem_init_Alva(sem0, 0, "xxxx");
    sem_init_Alva(sem1, 0, "xxxx1");

    pThread_mutex_init_Alva(&mutex0);
    pThread_Alva mThread1;
    pThread_Alva mThread2;

    createThread_Alva(&mThread1, NULL, IR_Thread1, NULL);
    createThread_Alva(&mThread2, NULL, IR_Thread2, NULL);

    mSleep_Alva(10);

    mSleep_Alva(1000 * 5);
    
    sem_destroy_Alva(sem0, "xxxx");
    sem_destroy_Alva(sem1, "xxxx1");

    int new_v = 5;
    int old = 5;
    int value = 7;

    int ret = atomic_compare_exchange_Alva(&new_v, &old, value);    
    printf("ret = %d new = %d \n", ret, new_v);

    pThreadJoin_Alva(mThread1, NULL);
    pThreadJoin_Alva(mThread2, NULL);
    return 0;
}
