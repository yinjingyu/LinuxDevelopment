/*
 * =====================================================================================
 *
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
CMutex::CMutex()
{
	int r = pthread_mutex_init(&m_Mutex,0);
 	if(r !=0 )
		throw CStatus(-1,0,"error in CMutex of CMutex :fail to  init the mutex");
}

CMutex::~CMutex()
{
	int r = pthread_mutex_destroy(&m_Mutex);
 	if(r != 0 )
		throw CStatus(-1,0,"error in ~CMutex of CMutex :fail to  destroy the mutex");
}

CStatus CMutex::Lock()
{
	int r = pthread_mutex_lock(&m_Mutex);
	if(r !=0 )
		return CStatus(-1,0,"error in Lock of CMutex :fail to lock the mutex");

	return CStatus(0,0);
}

CStatus CMutex::Unlock()
{
	int r = pthread_mutex_unlock(&m_Mutex);
 	if(r !=0 )
		return CStatus(-1,0,"error in unlock of CMutex :fail to lock the mutex");

	return CStatus(0,0);
}











