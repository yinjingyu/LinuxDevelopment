 /*
 * =====================================================================================
 * *       Filename:  test.cpp
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

#include "CThreadPool.h"
#include "CAddMessage.h"
#include "CQuitMessage.h"
#include "CThreadUsingMsgLoop.h"
#include "CUsrThreadObserver.h"
#include "./include/CCommunicationNameServer.h"

using namespace std;

int main()
{
	CThreadPool * pThreadPool = new CThreadPool(3);

	CThreadUsingMsgLoop * pUsrThread = new CThreadUsingMsgLoop("UsrThread",new CUsrThreadObserver(pThreadPool),true);
	pUsrThread->Run(0);

	delete pThreadPool;
	
	CCommunicationNameServer::SendMessage("UsrThread",new CQuitMessage());
	delete pUsrThread;

 	return 0;
}
