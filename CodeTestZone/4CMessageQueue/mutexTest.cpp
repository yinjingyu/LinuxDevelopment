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
#include "CEnterCriticalSection.h"
#include "CConditionVariable.h"
#include "CEvent.h"
#include <unistd.h>
#include <pthread.h>

using namespace std;

int globalInt = 0;

void * StartFunction(void * pContext)
{

	cout <<"sub thread is going to sleep" << endl;
	sleep(4);
	CEvent * pEvent = (CEvent *)pContext;

	globalInt++;

	pEvent->Set();
	cout << "sub thread is going to Set the event"<<endl;
	return 0;
}

int main()
{
	CEvent * pEvent = new CEvent();

	pthread_t threadID;

	pthread_create(&threadID,0,StartFunction,(void *)pEvent);

		cout << "main thread is going to wait the Event" << endl;
		pEvent->Wait();
		cout <<"main thread got the condition" << endl;
		cout << "globalint is :"<<globalInt <<endl;
	return 0;
}
