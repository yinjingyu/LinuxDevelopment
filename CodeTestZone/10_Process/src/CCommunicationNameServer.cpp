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
#include <string.h>

CMutex CCommunicationNameServer::m_MutexForCreatingInstance;
CCommunicationNameServer * CCommunicationNameServer::m_pNameServer = 0;

pthread_mutex_t CCommunicationNameServer::m_Mutex = PTHREAD_MUTEX_INITIALIZER; 

CCommunicationNameServer:: 	CCommunicationNameServer()
{

}

CCommunicationNameServer::~CCommunicationNameServer()
{

}

CCommunicationNameServer * CCommunicationNameServer:: GetInstance()
{
	return m_pNameServer;
}

CStatus CCommunicationNameServer::Create()
{
		
	CMutex mutex(&m_Mutex);
	CEnterCriticalSection cs(&mutex);

	if(m_pNameServer == 0)
	{
		m_pNameServer = new CCommunicationNameServer;
	}

	return CStatus(0,0);
}

CStatus CCommunicationNameServer::Destroy()
{
	CMutex mutex(&m_Mutex);
	CEnterCriticalSection cs(&mutex);

	delete m_pNameServer;

	m_pNameServer = 0;

	return CStatus(0,0);
}

CStatus CCommunicationNameServer:: Register(const char * strCommObjName, ICommunicationObject * pCommObj)
{
	if(0 == pCommObj)
	{
		std::cout <<"in Register of CCommunicationNameServer:paremeter is null"<<std::endl;
		return CStatus(-1,0);
	}

	if((strCommObjName == 0) || (strlen(strCommObjName) == 0))
	{
		delete pCommObj;
		std::cout <<"register error in CCommunicationNameServer"<<std::endl;
		return CStatus(-1,0);
	}

	if(m_pNameServer == 0)
	{
		std::cout << "In CCommunicationNameServer::Register, m_pNameServer is 0"<< std::endl;
		return CStatus(-1,0);	
	}

	CEnterCriticalSection cs(&m_MutexForNameTable);
	std::map<std::string,SCommunicationPtrCount *>::iterator it1;
	it1 = m_NameTable.find(strCommObjName);
 	if(it1 != m_NameTable.end())
 	{
		delete pCommObj;
		std::cout <<"in Register of CCommunicationNameServer : already registerd"<<std::endl;
		return CStatus(-1,1);
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
		std::cout <<"in GetCommunicationObject of ICommunicationObject : paremeter is null"<<std::endl;
		throw 0;
	}

	if(m_pNameServer == 0)
	{
		std::cout << "In CCommunicationNameServer::GetCommunicationObject , m_pNameServer is 0"<<std::endl;
		return 0;
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
		std::cout <<"in GetCommunicationObject of ICommunicationObject : paremeter is null"<< std::endl;
		return CStatus(-1,0);
	}
	
	ICommunicationObject * pTmp = 0;
	{
	 	CEnterCriticalSection cs(&m_MutexForNameTable);
	 	std::map<std::string, SCommunicationPtrCount *>::iterator it1;
	 	it1 = m_NameTable.find(strCommObjName);
	 	if(it1 == m_NameTable.end())
	 	{
			std::cout <<"in GetCommunicationObject of CCommunicationNameServer: can't find referenced comm obj"<< std::endl;
	 		return CStatus(-1,0);
	 	}
 	
 		it1->second->iConnectionCount--;
 		if(it1->second->iConnectionCount <= 0)
 		{
			pTmp = it1->second->pCommObj;
 			delete it1->second;
 			m_NameTable.erase(it1);
 		}
 	}
	if( 0 != pTmp)
	{
		delete pTmp;
	}
 	return CStatus(0,0);
}

CStatus CCommunicationNameServer::SendMessage(const char * strCommObjName, CMessage * pMessage)
{
	if(0 == pMessage)
	{
		std::cout << "bad paremeter in CCommunicationNameServer:: SendMessage" << std::endl;
		return CStatus(-1,0);
	}

	if((strCommObjName == 0) || (strlen(strCommObjName) == 0))
	{
		delete pMessage;
		std::cout << "in CCommunicationNameServer::SendMessage , strCommObjName is 0"<<std::endl;
		return CStatus(-1,0);
	}


	CCommunicationNameServer * pNameServer = CCommunicationNameServer::GetInstance();
	if(pNameServer == 0)
	{
		delete pMessage;
		std::cout << "Get Instance of nameserver failed" << std::endl;
		return CStatus(-1,0);
	}

	ICommunicationObject * pCommunicationObject = pNameServer->GetCommunicationObject(strCommObjName);
	if(0 == pCommunicationObject)
	{
		delete pMessage;
 	 	std::cout << "in CCommunicationNameServer SendMessage:GetCommunicationObject failed" << std::endl;
		return CStatus(-1,0);
	}
	
	CStatus s1 = pCommunicationObject->PostMessage(pMessage);
	if(!s1.IsSuccess())
	{
		std::cout << "In CCommunicationNameServer::SendMessage, pCommunicationObject->PostMessage failed" <<std::endl;
		CStatus s2 = pNameServer->ReleaseCommunicationObject(strCommObjName);
		if(!s2.IsSuccess())
		{
			std::cout<<"In CCommunicationNameServer::SendMessage , 1_pNameServer->ReleaseCommunicationObject failed!" <<std::endl;
			return CStatus(-1,0);
		}
		return CStatus(-1,0);
	}
	
	CStatus s3 = pNameServer->ReleaseCommunicationObject(strCommObjName);
	if(!s3.IsSuccess())
	{
		std::cout<<"In CCommunicationNameServer::SendMessage , 2_pNameServer->ReleaseCommunicationObject failed!"<<std::endl;
		return CStatus(-1,0);
	}

	return CStatus(0,0);	
}
