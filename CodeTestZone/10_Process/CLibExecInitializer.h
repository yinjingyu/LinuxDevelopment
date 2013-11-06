


#ifndef CLIBEXECINITIALIZER_H
#define CLIBEXECINITIALIZER_H

#include "CStatus.h"


//用于执行体库的整体初始化，采用单例模式防止多次初始化
class CLibExecInitializer
{
public:
	static CStatus Initialize();
	static CStatus Destroy();

private:
	CLibExecInitializer();
	~CLibExecInitializer();
	CLibExecInitializer(const CLibExecInitializer &);
	CLibExecInitializer& operator=(const CLibExecInitializer &);

private:
 	//初始化和销毁都只需要执行一次，
	static bool m_bInitialized;
	static bool m_bDestroyed;

	//执行体库在初始化时必须互斥的调用,防止多线程情况同时调用该函数
	static pthread_mutex_t m_MutexForInitializer;
};

#endif
