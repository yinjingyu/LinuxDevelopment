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
#include "CMutex.h"
#include <unistd.h>

using namespace std;
int globalInt = 0;
void * StartFunction(void * pContext)
{
	CMutex * pMutex = (CMutex *)pContext;
	pMutex->Lock();
	cout << "in subthread i will sleep 4s" << endl;
	sleep(4);
	globalInt++;
	pMutex->Unlock();
	return 0;
}

int main()
{
	CMutex * pMutex = new CMutex();
	pthread_t threadID;
	pthread_create(&threadID,0,StartFunction,(void *)pMutex);
	sleep(1);
	pMutex->Lock();
	cout << "this is main thread ,the value of globalInt is:" << globalInt <<endl;
	pMutex->Unlock();
	
	return 0;
}
