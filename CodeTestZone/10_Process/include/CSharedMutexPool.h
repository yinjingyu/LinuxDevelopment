

#ifndef CSharedMutex_H
#define CSharedMutex_H

#include "ISharedObjectPool.h"
#include "CStatus.h"

//共享互斥量池中单个共享互斥量的布局
struct SSharedMutexItem
{
	SSharedObjectHead head;
	pthread_mutex_t mutex;
};

class CSharedMutexPool : public ISharedObjectPool
{
public:
	virtual CStatus InitializeASharedObject(SSharedObjectHead * pObject);
	virtual CStatus DestroyASharedObject(SSharedObjectHead * pObject);

	//只有CSharedMutexAllocator 才能创建共享互斥量
	friend class CSharedMutexPoolManager;
private:
	CSharedMutexPool();
	virtual ~CSharedMutexPool();
};

#endif
