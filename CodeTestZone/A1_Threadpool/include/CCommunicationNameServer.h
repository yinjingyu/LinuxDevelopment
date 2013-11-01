

#ifndef CCOMMUNICATIONNAMESERVER_H
#define CCOMMUNICATIONNAMESERVER_H

#include "ICommunicationObject.h"
#include "CStatus.h"
#include <map>
#include <string>
#include "CMutex.h"

typedef struct
{
	ICommunicationObject * pCommObj;
	unsigned int iConnectionCount;
}SCommunicationPtrCount;

class CCommunicationNameServer
{
	private:
	//记录当前的名字服务器中所有的名字和对应的通信实体
	std::map<std::string,SCommunicationPtrCount *> m_NameTable;

	//名字服务必须是单例模式，不允许new
	CCommunicationNameServer();
	~CCommunicationNameServer();

	//用于互斥的创建通信服务器实例
	static CMutex  m_MutexForCreatingInstance;

	//用于在单例模式中保存名字服务器的实例对象
	static CCommunicationNameServer * m_pNameServer; 	

	//用于控制多线程中互斥的访问名字服务表
	CMutex m_MutexForNameTable;

	public:
	//获取名字服务的实例
	static CCommunicationNameServer * GetInstance();
	//向名字服务器中注册名字和对应的通信对象
	CStatus Register(const char * strCommObjName,ICommunicationObject * pCommObj);
	//根据名字获取通信对象
	ICommunicationObject * GetCommunicationObject(const char * strCommObjName);

	CStatus ReleaseCommunicationObject(const char * strCommObjName);

	static CStatus SendMessage(const char * strCommObjName, CMessage * pMessage);
};


#endif
