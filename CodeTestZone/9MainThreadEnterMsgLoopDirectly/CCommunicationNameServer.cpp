/*
 * =====================================================================================
 *
 *       Filename:  CCommunicationNameServer.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年10月29日 09时43分47秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "CCommunicationNameServer.h"
#include "CEnterCriticalSection.h"
#include "CMutex.h"
#include <iostream>

CMutex CCommunicationNameServer::m_MutexForCreatingInstance;
CCommunicationNameServer * CCommunicationNameServer::m_pNameServer = 0;

CCommunicationNameServer:: 	CCommunicationNameServer()
{

}

CCommunicationNameServer::~CCommunicationNameServer()
{

}

CCommunicationNameServer * CCommunicationNameServer:: GetInstance()
{
	try
	{
		CEnterCriticalSection cs(&m_MutexForCreatingInstance);
 	 	if(m_pNameServer == 0)
 		{
 			m_pNameServer = new CCommunicationNameServer;
 		}
	}
	catch(...)
	{
		throw CStatus(-1,0,"in GetInstance of CCommunicationNameServer:catch exception when new CCommunicationNameServer");
 	}

	return m_pNameServer;
}


CStatus CCommunicationNameServer:: Register(const char * strCommObjName, ICommunicationObject * pCommObj)
{
	if(0 == pCommObj)
	{
		return CStatus(-1,0,"in Register of CCommunicationNameServer:paremeter is null");
	}

	if((strCommObjName == 0) || (strlen(strCommObjName) == 0))
	{
		delete pCommObj;
		return CStatus(-1,0,"register error in CCommunicationNameServer");
	}

	CEnterCriticalSection cs(&m_MutexForNameTable);
	std::map<std::string,SCommunicationPtrCount *>::iterator it1;
	it1 = m_NameTable.find(strCommObjName);
 	if(it1 != m_NameTable.end())
 	{
		delete pCommObj;
		return CStatus(-1,1,"in Register of CCommunicationNameServer : already registerd");
 	}

	SCommunicationPtrCount * p = new SCommunicationPtrCount;
	p->pCommObj = pCommObj;

	//调用注册函数的线程，肯定持有通信对新对象的引用，所以引用计数要从1开始
	//实际上，在本系统中调用Register函数是消息队列的拥有者，它会从通信对象对象（消息队列）
	//中读取消息，所以必须保证消息队列有效，所以引用计数也必须是1
	p->iConnectionCount = 1;

	m_NameTable[strCommObjName] = p;
	
	return CStatus(0,0);
}

ICommunicationObject * CCommunicationNameServer::GetCommunicationObject(const char * strCommObjName)
{
	if(0 == strCommObjName || (strlen(strCommObjName) == 0))
	{
		throw CStatus(-1,0,"in GetCommunicationObject of ICommunicationObject : paremeter is null");
	}

	CEnterCriticalSection cs(&m_MutexForNameTable);
	std::map<std::string, SCommunicationPtrCount *>::iterator it1;
	it1 = m_NameTable.find(strCommObjName);
	if(it1 == m_NameTable.end())
	{
		return NULL;
	}

	it1->second->iConnectionCount++;

	return it1->second->pCommObj;
}

CStatus CCommunicationNameServer::ReleaseCommunicationObject(const char * strCommObjName)
{
	if(0 == strCommObjName || (strlen(strCommObjName) == 0))
	{
		return CStatus(-1,0,"in GetCommunicationObject of ICommunicationObject : paremeter is null");
	}
	
	ICommunicationObject * pTmp = 0;
	
	{
	CEnterCriticalSection cs(&m_MutexForNameTable);
	std::map<std::string, SCommunicationPtrCount *>::iterator it1;
	it1 = m_NameTable.find(strCommObjName);
	if(it1 == m_NameTable.end())
	{
		return CStatus(-1,0,"in GetCommunicationObject of CCommunicationNameServer: can't find referenced comm obj");
	}

	it1->second->iConnectionCount--;
	if(it1->second->iConnectionCount <= 0)
	{
		delete it1->second->pCommObj;
		delete it1->second;
		m_NameTable.erase(it1);
	}
	}
	return CStatus(0,0);
}

CStatus CCommunicationNameServer::SendMessage(const char * strCommObjName, CMessage * pMessage)
{
	if(0 == pMessage)
	{
		std::cout << "bad paremeter in CCommunicationNameServer:: SendMessage" << std::endl;

		return CStatus(-1,0,"bad paremeter in CCommunicationNameServer:: SendMessage");
	}

	if((strCommObjName == 0) || (strlen(strCommObjName) == 0))
	{
		delete pMessage;
		return CStatus(-1,0,"sendmessgae error");
	}


	CCommunicationNameServer * pNameServer = CCommunicationNameServer::GetInstance();
	if(pNameServer == 0)
	{
		delete pMessage;
		std::cout << "Get Instance of nameserver failed" << std::endl;
		return CStatus(-1,0,"in SendMessage of CCommunicationNameServer : get instance failed");
	}

	ICommunicationObject * pCommunicationObject = pNameServer->GetCommunicationObject(strCommObjName);
	
	if(0 == pCommunicationObject)
	{
		delete pMessage;
 	 	std::cout << "in CCommunicationNameServer SendMessage:GetCommunicationObject failed" << std::endl;
		return CStatus(-1,0,"in SendMessage of CCommunicationNameServer : GetCommunicationObject failed");
	}
	
	CStatus s_pm = pCommunicationObject->PostMessage(pMessage);
	if(!s_pm.IsSuccess())
	{
		CStatus s_pm_1 = pNameServer->ReleaseCommunicationObject(strCommObjName);
		if(!s_pm_1.IsSuccess())
		{
			return s_pm_1;
		}
		return s_pm;
	}
	
	CStatus s_rco = pNameServer->ReleaseCommunicationObject(strCommObjName);
	if(!s_rco.IsSuccess())
	{
		return s_rco;
	}

	return CStatus(0,0);	
}
