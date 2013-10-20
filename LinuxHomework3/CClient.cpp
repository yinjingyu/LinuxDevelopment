/*
 * =====================================================================================
 *
 *       Filename:  CClient.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年10月18日 15时48分03秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <iostream>

using namespace std;

#include "include/CClientBusinessForExecObj.h"
#include "include/CThread.h"
#include "include/CStatus.h"
#include <unistd.h>

class Client : public CClientBusinessForExecObj
{
	public:

	Client()
	{
	}
	virtual ~Client()
	{
	}
   
	virtual CStatus RunClientBusiness(void * pContext)
	{
		sleep(2);
		cout << "Hello world!" << endl;
	}
};


int main()
{
	CThread *ct = new CThread(new Client);
	ct->Run((void *)0);
	ct->WaitForDeath();
	
	cout << endl;
	return 0;
}
