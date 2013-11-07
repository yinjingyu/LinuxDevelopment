/*
 * =====================================================================================
 *
 *       Filename:  CSharedMutex.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年11月07日 16时58分00秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "CSharedMutexPool.h"
#include "CStatus.h"
#include <pthread.h>
#define SHARED_SPACE_FOR_SHARED_MUTEX_ALLOCATOR "shared_space_for_shared_mutex_allocator"

CSharedMutexPool::CSharedMutexPool()
{
        m_nItemSize = sizeof(SSharedMutexItem);
        m_strSharedSpaceName = SHARED_SPACE_FOR_SHARED_MUTEX_ALLOCATOR;
}

CSharedMutexPool::~CSharedMutexPool()
{
}

CStatus CSharedMutexPool::InitializeSharedObjectItem(SSharedObjectHead *pObject)
{
		//虽然传进来的是头部地址，但我们可以把该首地址强制转换成SSharedMutexItem类型的，
		//这就告诉了编译器，从SSharedObjectHead地址处开始大小为SSharedMutexItem大小的区域
		//被用来当成SSharedMutexItem对象来处理
        SSharedMutexItem *pMutexItem = (SSharedMutexItem *)pObject;

		//如果pthread_mutex_t 想要拿来当成共享互斥量来使用，需要设置互斥量的属性
        pthread_mutexattr_t attr;
        int r = pthread_mutexattr_init(&attr);
        if(r != 0)
        {
                cout << "In CLSharedMutexImpl::InitializeSharedObject(), pthread_mutexattr_init error" << endl;
                return CStatus(-1, r);
        }
		
		//根据初始化好的共享互斥量属性attr来设置互斥量为共享型互斥量
        r = pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
        if(r != 0)
        {
                cout << "In CLSharedMutexImpl::InitializeSharedObject(), pthread_mutexattr_setpshared error"<<endl;

                pthread_mutexattr_destroy(&attr);

                return CStatus(-1, r);
        }
		
		//调用互斥量的初始化函数来初始化
        r = pthread_mutex_init(&(pMutexItem->mutex), &attr);
        if(r != 0)
        {
                cout << "In CLSharedMutexImpl::InitializeSharedObject(), pthread_mutex_init error" << endl;

                pthread_mutexattr_destroy(&attr);

                return CStatus(-1, r);
        }

        r = pthread_mutexattr_destroy(&attr);
        if(r != 0)
        {
                cout << "In CLSharedMutexImpl::InitializeSharedObject(), pthread_mutexattr_destroy error" << endl;
                return CStatus(-1, r);
        }

        return CStatus(0, 0);
}

//销毁共享互斥量
//实际上就是封装了pthread_mutexattr_destroy函数,
CStatus CSharedMutexPool::DestroySharedObjectItem(SSharedObjectHead *pObject)
{
        SSharedMutexItem *pMutexItem = (SSharedMutexItem *)pObject;

        int r = pthread_mutex_destroy(&(pMutexItem->mutex));
        if(r != 0)
        {
                cout << "In CLSharedMutexImpl::DestroySharedObject(), pthread_mutex_destroy error" << endl;
                return CStatus(-1, r);
        }

        return CStatus(0, 0);
}
