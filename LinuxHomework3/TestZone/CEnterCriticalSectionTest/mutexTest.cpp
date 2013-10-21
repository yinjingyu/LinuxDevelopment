/*
 * =====================================================================================
 *
 *       Filename:  mutexTest.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年10月20日 22时40分00秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <iostream>
#include "./include/CMutex.h"
#include "./include/CEnterCriticalSection.h"
#include <unistd.h>

using namespace std;
int globalInt = 0;
void * StartFunction(void * pContext)
{
	CMutex * pMutex = (CMutex *)pContext;
	{
		CEnterCriticalSection cs(pMutex);
		cout << "in subthread i will sleep 4s" << endl;
		sleep(4);
		globalInt++;
	}
	return 0;
}

int main()
{
	CMutex * pMutex = new CMutex();
	pthread_t threadID;
	pthread_create(&threadID,0,StartFunction,(void *)pMutex);
	sleep(1);
	{
		CEnterCriticalSection cs(pMutex);
		cout << "this is main thread ,the value of globalInt is:" << globalInt <<endl;
	}
	return 0;
}
