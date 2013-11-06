/*
 * =====================================================================================
 *
 *       Filename:  CLibExecInitializer.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年11月06日 18时40分39秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "CLibExecInitializer.h"
#include "CStatus.h"

pthread_mutex_t CLibExecInitializer :: m_MutexForInitializer = PTHREAD_MUTEX_INITIALIZER;

bool CLibExecInitializer :: m_bInitialized = false;
bool CLibExecInitializer :: m_bDestroyed   = false;

CStatus CLibExecInitializer :: Initialize()
{
	
}
