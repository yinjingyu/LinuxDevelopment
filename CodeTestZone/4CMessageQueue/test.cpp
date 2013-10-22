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
#include "CMessage.h"
#include "CMessageQueueByUserDefined.h"
#include "CStatus.h"

using namespace std;

int main()
{
	CMessageQueueByUserDefined * queue = new CMessageQueueByUserDefined();

	int c;
	while((cin >> c,c) != 0)
	{
		if(c < 0)
		{
			CMessage * temp	= queue->Pop();
			cout << "pop out from the queue is : " << temp->m_clMsgID << endl;
		}
		else
			queue->Push(new CMessage(c));
	}

	cout << "======" << endl;
	cout << "the numbers of the msg in the queue is" << endl;
	while(!queue->IsEmpty())
 	{
 		CMessage * pm = queue->Pop();
		if(NULL == pm)
 		{
			cout <<"pop failed!" << endl;
 			return 0;
 		}
  		cout <<  pm->m_clMsgID << endl;
  	}
	return 0;
}
