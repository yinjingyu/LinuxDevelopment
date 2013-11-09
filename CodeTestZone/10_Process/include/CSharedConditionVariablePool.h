#ifndef CSharedConditionVariablePool_H
#define CSharedConditionVariablePool_H

#include <pthread.h>
#include "ISharedObjectPool.h"

struct SSharedConditionVariableItem
{
	SSharedObjectHead head;
	pthread_cond_t cond;
};

class CSharedConditionVariablePool : public ISharedObjectPool
{
public:
	virtual CStatus InitializeASharedObject(SSharedObjectHead *pObject);
	virtual CStatus DestroyASharedObject(SSharedObjectHead *pObject);

	friend  class CSharedCondVarManager;
private:
	CSharedConditionVariablePool();
	virtual ~CSharedConditionVariablePool();

	CSharedConditionVariablePool(const CSharedConditionVariablePool&);
	CSharedConditionVariablePool& operator=(const CSharedConditionVariablePool&);
};

#endif
