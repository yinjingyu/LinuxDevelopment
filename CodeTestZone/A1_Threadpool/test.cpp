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


using namespace std;


int main()
{
	CThreadPool * pThreadPool = new CThreadPool(3);

	for(int i = 0; i < 3; i++)
	{
		pThreadPool->DispatchMessage(new CAddMessage(1,i));
	}
	
//	sleep(2);
	delete pThreadPool;

 	return 0;
}
