/*
 * =====================================================================================
 *
 *       Filename:  CMutexByPThread.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年11月07日 01时53分40秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "LibHeadFileAllInOne.h"


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  CMutexByPThread
 *  Description:  无参构造函数，需要在类内部分配pthread_mutex_t类型变量
 * =====================================================================================
 */
CMutexByPThread :: CMutexByPThread()
{
	m_pMutex = new pthread_mutex_t;
	m_bNeedDestroy = true;

	int r = pthread_mutex_init(m_pMutex,0);
	if( r != 0)
	{
		//初始化失败，但pthread_mutex_t已经被分配了
		//所以需要在这类释放
		delete m_pMutex;

		cout << "In CMutexByPThread::Constructor,pthread_mutex_init failed!" << endl;

		throw "error";
	}
}

CMutexByPThread :: CMutexByPThread(pthread_mutex_t * pMutex)
{
	if( 0 == pMutex)
	{
		cout << "In CMutexByPThread(1),pMutex is 0" << endl; 
		throw "";
	}

	m_pMutex = pMutex;
	m_bNeedDestroy = false;
}

CMutexByPThread :: ~CMutexByPThread()
{
	if(m_bNeedDestroy)
	{
		int r = pthread_mutex_destroy(m_pMutex);
		if( r != 0)
			cout << "In ~CMutexByPThread,pthread_mutex_destroy failed!" << endl;
		delete m_pMutex;
	}
}

CStatus CMutexByPThread :: Initialize()
{
	return CStatus(0,0);
}

CStatus CMutexByPThread :: Uninitialize()
{
	return CStatus(0,0);
}

CStatus CMutexByPThread :: Lock()
{
	int r = pthread_mutex_lock(m_pMutex); 	
	if( r != 0)
	{
		cout << "In CMutexByPThread::Lock, pthread_mutex_lock failed!" << endl;
		return CStatus(-1,0);
	}
	return CStatus(0,0);
}

CStatus CMutexByPThread :: Unlock()
{
	int r = pthread_mutex_unlock(m_pMutex);
	if( r != 0)
	{
		cout << "In CMutexByPThread::Unlock,pthread_mutex_unlock failed!" << endl;
		return CStatus(-1,0);
	}
	
	return CStatus(0,0);
}

pthread_mutex_t * CMutexByPThread :: GetMutexPointer()
{
	return m_pMutex;
}











