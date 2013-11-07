/*
 * =====================================================================================
  *       Filename:  CMutex.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年10月20日 22时30分49秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "CMutex.h"
#include "CStatus.h"
#include "CMutexByPThread.h"
#include "CMutexByRecordLocking.h"
#include "CMutexByRecordLockingAndPThread.h"
#include "CMutexBySharedPThread.h"

#include <string>
#include <iostream>

using namespace std;

CMutex::CMutex()
{
	m_pMutex = new CMutexByPThread();
	
	//因为m_pMutex 是一个接口指针，所以我们为了统一性
	//在下面调用了虚函数：用来初始化互斥量
	if(!(m_pMutex->Initialize()).IsSuccess())
	{
		delete m_pMutex;
		cout << "In CMutex::constructor(0), m_pMutex->Initialize failed!" << endl;
		throw "";
	}
}

//如果使用了单参构造函数，并传进来一个pthread_mutex_t变量
//说明要使用 CMutexByPThread 来构造互斥量
CMutex::CMutex(pthread_mutex_t * pMutex)
{
	m_pMutex = new CMutexByPThread(pMutex);

	if( !(m_pMutex->Initialize()).IsSuccess())
	{
		delete m_pMutex;
		cout << "In CMutex::constructor(1), m_pMutex->Initialize failed!" << endl;
		throw "";
	}
}

//根据nType来判断使用什么来构造互斥量
CMutex::CMutex(const char * pstrFileName, int nType)
{
	if( (pstrFileName == 0) || (strlen(pstrFileName) == 0))
	{
		cout << "In CMutex::constructor(2), pstrFileName is bad!" << endl;
		throw "";
	}

	//根据nType new不同的对象来给父类指针赋值
	if( nType == MUTEX_USE_RECORD_LOCK)
	{
		m_pMutex = new CMutexByRecordLocking(pstrFileName);
	}
	else if( nType == MUTEX_USE_RECORD_LOCK_AND_PTHREAD)
	{
		m_pMutex = new CMutexByRecordLockingAndPThread(pstrFileName);
	}
	else if( nType == MUTEX_USE_SHARED_PTHREAD )
	{
		m_pMutex = new CMutexBySharedPThread(pstrFileName);
	}
	else
	{
		cout << "In CMutex::constructor(2),nType error!" << endl;
		throw "";
	}

	//调用基类的虚函数，初始化
	if( !(m_pMutex->Initialize()).IsSuccess() )
	{
		delete m_pMutex;
		cout << "In CMutex::constructor(2),m_pMutex->Initialize failed!" << endl;
		throw "";
	}
}
//使用记录锁和pthread_mutex_t类型混合型的互斥量
//且pthread_mutex_t 由外部传入
CMutex::CMutex(const char * pstrFileName, pthread_mutex_t * pMutex)
{
	if( (pstrFileName == 0) || (strlen(pstrFileName) == 0))
	{
		cout << "In CMutex::constructor(3), pstrFileName is bad!" << endl;
		throw "";
	}

	m_pMutex = new CMutexByRecordLockingAndPThread(pstrFileName,pMutex);
	if( !(m_pMutex->Initialize()).IsSuccess())
	{
		delete m_pMutex;
		cout << "In CMutex::constructor(3),m_pMutex->Initialize failed!"<< endl;
		throw "";
	}
}

CMutex::~CMutex()
{
	m_pMutex->Uninitialize();
	delete m_pMutex;
}

CStatus CMutex::Lock()
{
	//调用基类的虚函数来加锁
	//加锁函数的具体实现由派生类来实现
	return m_pMutex->Lock();
}

CStatus CMutex::Unlock()
{
	//调用基类的虚函数来解锁
	//解锁函数的具体实现由派生类来实现
	return m_pMutex->Unlock();
}











