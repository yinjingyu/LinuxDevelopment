#ifndef CSharedConditionVariablePool_H
#define CSharedConditionVariablePool_H

#include <pthread.h>
#include "ISharedObjects.h"

struct SSharedConditionVariableItem
{
	SLSharedObjectHead head;
	pthread_cond_t cond;
};

class CSharedConditionVariablePool : public ISharedObjects
{
public:
	virtual CStatus InitializeSharedObject(SSharedObjectHead *pObject);
	virtual CStatus DestroySharedObject(SSharedObjectHead *pObject);

	friend  class CSharedConditionVariableManager;
private:
	CSharedConditionVariablePool();
	virtual ~CSharedConditionVariablePool();

	CSharedConditionVariablePool(const CSharedConditionVariablePool&);
	CSharedConditionVariablePool& operator=(const CSharedConditionVariablePool&);
};

#endif
