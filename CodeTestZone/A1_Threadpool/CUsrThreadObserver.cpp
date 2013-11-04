/*
 * =====================================================================================
 *
 *       Filename:  CUsrThreadObserver.cpp
 *
 *    Description:  
 *        Version:  1.0
 *        Created:  2013年11月04日 11时24分04秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "CUsrThreadObserver.h"

#include "MessageIDTable.h"

CStatus CUsrThreadObserver::Initialize(CMsgLoopManager * pLoopMgr,void * pContext)
{
	pLoopMgr->Register(QUIT_MESSAGE_LOOP,(CallBackFunctionOfMsgObserver)(&CUsrThreadObserver::On_Quit)); 
	pLoopMgr->Register(USR_REQUESTRESULT_MESSAGE,(CallBackFunctionOfMsgObserver)(&CUsrThreadObserver::On_RequestResult));

	for (int i = 0; i < 10; i++) {
		/* 发送消息 */
		
	}

}

CStatus CUsrThreadObserver::On_RequestResult(CMessage * pMsg)
{
	if(pMsg == 0)
	{
		std::cout << << std::endl; 
	}
}


CStatus CUsrThreadObserver::On_Quit(CMessage * pMsg)
{
	return CStatus(QUIT_MESSAGE_LOOP,0); 
}
