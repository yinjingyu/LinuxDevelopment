#ifndef CSharedEventPool_H
#define CSharedEventPool_H

#include "ISharedObjectPool.h"
#include "CStatus.h"

struct SEventInfo
{
	long Flag;
	long bSemaphore;
	long Context;
};

struct SSharedEventItem
{
	SSharedObjectHead head;
	SEventInfo EventInfo;
};

class CSharedEventPool : public ISharedObjectPool
{
public:
	virtual CStatus InitializeASharedObject(SSharedObjectHead *pObject);
	virtual CStatus DestroyASharedObject(SSharedObjectHead *pObject);

	friend  class CSharedEventManager;

private:
	CSharedEventPool();
	virtual ~CSharedEventPool();

	CSharedEventPool(const CSharedEventPool&);
	CSharedEventPool& operator=(const CSharedEventPool&);
};

#endif
