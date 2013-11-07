

#ifndef CSharedMutexPoolManager_H
#define CSharedMutexPoolManager_H

#include <pthread.h>
#include "CStatus.h"

class CSharedMutexPool;

class CSharedMutexPoolManager
{

public:

	//单例模式返回一个共享线程池的管理类
	static CSharedMutexPoolManager * GetInstance();

	//根据共享对象名，返回一个共享对象
	static pthread_mutex_t* Get(const char *pstrMutexName);

	//根据共享对象名，释放一个共享对象
	static CStatus Release(const char *pstrMutexName);


	pthread_mutex_t *GetASharedMutex(const char *pstrMutexName);

	CStatus ReleaseASharedMutex(const char *pstrMutexName);

	friend class CLibExecutiveInitializer;

private:

	//这里的单例模式，实际对象的创建要放到执行库的初始化函数中
	//下面两个函数分别用来在执行库的初始化函数中创建单例对象和释放对象
	static CStatus Create();

	static CStatus Destroy();

private:

	CSharedMutexPoolManager();

	virtual ~CSharedMutexPoolManager();

	CSharedMutexPoolManager(const CSharedMutexPoolManager&);

	CSharedMutexPoolManager& operator=(const CSharedMutexPoolManager&);

private:

	static CSharedMutexPoolManager *m_pShMutexPoolMgr;

	static pthread_mutex_t m_MutexForSharedMutex;

	CSharedMutexPool *m_pShMutexPool;

};
#endif

