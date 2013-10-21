/*
 * =====================================================================================
 *
 *       Filename:  CEnterCriticalSection.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年10月21日 11时10分48秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "../include/CEnterCriticalSection.h"
#include "../include/CStatus.h"

CEnterCriticalSection::CEnterCriticalSection(CMutex * pMutex)
{
	if(NULL == pMutex)
	{
		throw CStatus(-1,0,"in CEnterCriticalSection of CEnterCriticalSection : the pMutex is NULL");
	}
	m_pMutex = pMutex;
	CStatus s = m_pMutex->Lock();
	if(!s.IsSuccess())
	{
		throw CStatus(-1,0,"in CEnterCriticalSection of CEnterCriticalSection: lock mutex failed!");
	}
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  ~CEnterCriticalSection
 *  Description:  注意不能在析构函数中调用destroy销毁锁，因为我们的CEnterCriticalSection 类只是用来方便我们使用锁来进行加锁和解锁操作，CEnterCriticalSection本身的锁是个引用，必须由外界传入。外界可能还要用到该锁，所以不能在此类中销毁锁
 * =====================================================================================
 */
CEnterCriticalSection::~CEnterCriticalSection()
{
	CStatus s = m_pMutex->Unlock();
	if(!s.IsSuccess())
	{
		throw CStatus(-1,0,"in ~CEnterCriticalSection of CEnterCriticalSection: unlock mutex failed!");
	}
}
