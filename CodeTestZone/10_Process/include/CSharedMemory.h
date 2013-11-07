

#ifndef CSharedMemory_H
#define CSharedMemory_H

#include "CStatus.h"
#include "CMutex.h"

class CSharedMemory
{
public:
	//共享存储的名字，和共享存储的大小
	explicit CSharedMemory(const char * pstrFileName, size_t nSize = 0);
	virtual ~CSharedMemory();

	//获取共享存储的首地址
	void * GetAddress();

	//获取共享存储的引用计数
	int GetRefCount();

private:
	//删除共享存储
	CStatus DeleteSharedMemory();

private:
	CSharedMemory(const CSharedMemory &);
	CSharedMemory & operator=(const CSharedMemory &);

private:
	//共享区域的首地址
	void * m_pSharedMemory;

	//共享存储的内部名 	
	int m_SharedMemoryID;

	//互斥的创建和销毁 “共享存储”
	CMutex m_Mutex;
};

#endif
