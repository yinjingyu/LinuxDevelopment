/*
 * =====================================================================================
 *
 *       Filename:  CMessageLoopManager.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年10月23日 10时59分22秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "CMessageLoopManager.h"
#include "MessageIDTable.h"

CStatus CMessageLoopManager::EnterMessageLoop(void * pContext)
{
 	CStatus s1 = Initialize();
	if(!s1.IsSuccess())
		return s1;

	while(true)
	{
		int failed_counter = 0;
		CMessage * pMsg = WaitForMessage();
		if(0 == pMsg)
		{
			//如果等待消息失败超过5次，就认为消息队列出问题
			//推出消息循环
			if(failed_counter++ < WaitForMessageFailedTimes)	
					continue;
			else
				return CStatus(-1,0,"in EnterMessageLoop of CMessageLoopManager : wait for message faild");
		}
		
		CStatus s2 = DispatchMessage(pMsg);
		if(!s2.IsSuccess())
			return s2;
		
		if(s2.m_ciReturnCode == QUIT_MESSAGE_LOOP)
			break;
	}

	CStatus s3 = Uninitialize();
	if(!s3.IsSuccess())
		return s3;
	return CStatus(0,0);
}

