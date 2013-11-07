/*
 * =====================================================================================
 *
 *       Filename:  CSharedMutexPoolManager.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年11月07日 20时31分38秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <string.h>

#include "CSharedMutexPoolManager.h"

#include "CEnterCriticalSection.h"

#include "CMutex.h"

#include "CSharedMutexPool.h"

#include <iostream>

using namespace std;

//记录锁文件名
#define MUTEX_FOR_SHARED_MUTEX_ALLOCATOR "mutex_for_shared_mutex_allocator"

pthread_mutex_t CSharedMutexPoolManager::m_MutexForSharedMutex = PTHREAD_MUTEX_INITIALIZER;



CSharedMutexPoolManager *CSharedMutexPoolManager :: m_pShMutexPoolMgr = 0;


CSharedMutexPoolManager:: CSharedMutexPoolManager()
{
	m_pShMutexPool = new CSharedMutexPool;

	CMutex mutex(MUTEX_FOR_SHARED_MUTEX_ALLOCATOR, &m_MutexForSharedMutex);

	CEnterCriticalSection cs(&mutex);


	//共享互斥量池进行初始化工作，在初始化工作中会从共享存储中获取共享空间
	//然后将共享空间初始化称若干个共享互斥量
	if(!((m_pShMutexPool->Initialize()).IsSuccess()))
	{
		cout <<"In CLSharedMutexAllocator::CLSharedMutexAllocator(), m_pImpl->Initialize error"<<endl;
		throw "In CLSharedMutexAllocator::CLSharedMutexAllocator(), m_pImpl->Initialize error";
	}
}


//共享互斥两池的管理类注销函数
//管理类的注销，主要是要把他维护的共享互斥两池释放掉
//而共享互斥量池正好自己提供了销毁自己的destroy函数
CSharedMutexPoolManager::~CSharedMutexPoolManager()

{

	if(!((m_pShMutexPool->Destroy()).IsSuccess()))

	{

		delete m_pShMutexPool;

		throw "In CLSharedMutexAllocator::~CLSharedMutexAllocator(), m_pImpl->Destroy error";

	}

	delete m_pShMutexPool;

}



pthread_mutex_t* CSharedMutexPoolManager::Get(const char *pstrMutexName)

{

	CSharedMutexPoolManager *p = CSharedMutexPoolManager::GetInstance();

	if(p == 0)

		return 0;

	return p->GetASharedMutex(pstrMutexName);

}



CStatus CSharedMutexPoolManager::Release(const char *pstrMutexName)

{

	CSharedMutexPoolManager *p = CSharedMutexPoolManager::GetInstance();

	if(p == 0)

		return CStatus(-1, 0);

	return p->ReleaseASharedMutex(pstrMutexName);

}



CSharedMutexPoolManager* CSharedMutexPoolManager::GetInstance()

{

	return m_pShMutexPoolMgr;

}


//管理类不能随便的new，只能在执行体库的初始化函数中显示的调用Create
//才能创建管理类，保证只有一个管理类
CStatus CSharedMutexPoolManager::Create()

{

	if(m_pShMutexPoolMgr == 0)

	{

		try

		{

			m_pShMutexPoolMgr = new CSharedMutexPoolManager();

		}

		catch(const char *str)

		{
			cout << "In CSharedMutexPoolManager::Create, new CSharedMutexPoolManager failed!" <<endl;
			cout <<str<<endl;
			return CStatus(-1, 0);
		}

	}

	return CStatus(0, 0);
}



CStatus CSharedMutexPoolManager::Destroy()

{

	if(CSharedMutexPoolManager::m_pShMutexPoolMgr == 0)

		return CStatus(0, 0);



	CMutex mutex(MUTEX_FOR_SHARED_MUTEX_ALLOCATOR, &m_MutexForSharedMutex);

	CEnterCriticalSection cs(&mutex);



	try

	{

		delete CSharedMutexPoolManager::m_pShMutexPoolMgr;

	}

	catch(const char* str)

	{
		CSharedMutexPoolManager::m_pShMutexPoolMgr = 0;
		cout << "In CSharedMutexAllocator::Destroy , delete m_pShMutexPoolMgr failed!" << endl;
		return CStatus(-1, 0);
	}

	CSharedMutexPoolManager::m_pShMutexPoolMgr = 0;

	return CStatus(0, 0);
}



pthread_mutex_t *CSharedMutexPoolManager::GetASharedMutex(const char *pstrMutexName)

{

	if(pstrMutexName == 0)

		return 0;



	int len = strlen(pstrMutexName);

	if((len == 0) || (len >= LENGTH_OF_SHARED_OBJECT_NAME))

		return 0;



	CMutex mutex(MUTEX_FOR_SHARED_MUTEX_ALLOCATOR, &m_MutexForSharedMutex);

	CEnterCriticalSection cs(&mutex);



	if(m_pShMutexPoolMgr == 0)
	{
		cout << "In CLSharedMutexAllocator::GetSharedMutex(), m_pAllocator == 0" << endl;;
		return 0;
	}

	//线程池返回的是void* 类型的对象，所以在管理类中还要强制转化成pthread_mutex_t结构
	return (pthread_mutex_t *)m_pShMutexPool->GetASharedObject(pstrMutexName);

}



CStatus CSharedMutexPoolManager::ReleaseASharedMutex(const char *pstrMutexName)

{

	if(pstrMutexName == 0)

		return CStatus(-1, 0);



	int len = strlen(pstrMutexName);

	if((len == 0) || (len >= LENGTH_OF_SHARED_OBJECT_NAME))

		return CStatus(-1, 0);



	CMutex mutex(MUTEX_FOR_SHARED_MUTEX_ALLOCATOR, &m_MutexForSharedMutex);

	CEnterCriticalSection cs(&mutex);



	if(CSharedMutexPoolManager::m_pShMutexPoolMgr == 0)

	{

		cout << "In CLSharedMutexAllocator::ReleaseSharedMutex(), m_pAllocator == 0" << endl;

		return CStatus(-1, 0);

	}



	return m_pShMutexPool->ReleaseASharedObject(pstrMutexName);

}
