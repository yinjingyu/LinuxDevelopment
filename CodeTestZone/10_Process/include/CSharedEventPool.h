#ifndef CSharedEventPool_H
#define CSharedEventPool_H

#include "CSharedObjectPool.h"
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

class CSharedEventPool : public CSharedObjectPool
{
public:
	virtual CStatus InitializeSharedObject(SLSharedObjectHead *pObject);
	virtual CStatus DestroySharedObject(SLSharedObjectHead *pObject);

	friend  class CSharedEventManager;

private:
	CSharedEventPool();
	virtual ~CSharedEventPool();

	CSharedEventPool(const CSharedEventPool&);
	CSharedEventPool& operator=(const CSharedEventPool&);
};

#endif
