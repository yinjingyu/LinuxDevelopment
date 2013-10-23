/*
 * =====================================================================================
 *
 *       Filename:  test.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年10月22日 10时57分24秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include "CMessage.h"
#include "CMessageQueueByUserDefined.h"
#include "CStatus.h"

using namespace std;

void * StartFunction(void * pContext)
{
	CMessageQueueByUserDefined * queue = (CMessageQueueByUserDefined *)pContext;
	cout << "child thread is going to sleep " << endl;
	sleep(5);
	queue->PushMessage(new CMessage(1));
	cout << "child already have sent the msg" << endl;
	return 0;
}

int main()
{
	CMessageQueueByUserDefined * queue = new CMessageQueueByUserDefined();
	
	pthread_t pthreadID;
	pthread_create(&pthreadID,0,StartFunction,queue);
	
	cout << "main thread start to invoke GetMessage()" << endl;
	CMessage * pMsg = queue->GetMessage();
	cout << "msg from the child thread is : " << pMsg->m_clMsgID << endl;

	return 0;
}
