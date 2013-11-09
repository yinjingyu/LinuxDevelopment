


#include "CSharedConditionVariablePool.h"
#include <pthread.h>
#include "CStatus.h"

#include <iostream>

using namespace std;

#define SHARED_SPACE_FOR_SHARED_CONDITION_VARIABLE_ALLOCATOR "shared_space_for_shared_condition_variable_allocator"

CSharedConditionVariablePool::CSharedConditionVariablePool()
{
	m_nItemSize = sizeof(SSharedConditionVariableItem);
	m_strSharedSpaceName = SHARED_SPACE_FOR_SHARED_CONDITION_VARIABLE_ALLOCATOR;
}

CSharedConditionVariablePool::~CSharedConditionVariablePool()
{
}

CStatus CSharedConditionVariablePool::InitializeASharedObject(SSharedObjectHead *pObject)
{
	SSharedConditionVariableItem *pCondItem = (SSharedConditionVariableItem *)pObject;

	pthread_condattr_t attr;
	int r = pthread_condattr_init(&attr);
	if(r != 0)
	{
		cout << "In CSharedConditionVariablePool::InitializeSharedObject(), pthread_condattr_init error" << endl;
		return CStatus(-1, r);
	}

	r = pthread_condattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
	if(r != 0)
	{
		cout << "In CSharedConditionVariablePool::InitializeSharedObject(), pthread_condattr_setpshared error" <<endl;

		pthread_condattr_destroy(&attr);

		return CStatus(-1, r);
	}

	r = pthread_cond_init(&(pCondItem->cond), &attr);
	if(r != 0)
	{
		cout << "In CSharedConditionVariablePool::InitializeSharedObject(), pthread_cond_init error" << endl;

		pthread_condattr_destroy(&attr);

		return CStatus(-1, r);
	}

	r = pthread_condattr_destroy(&attr);
	if(r != 0)
	{
		cout << "In CSharedConditionVariablePool::InitializeSharedObject(), pthread_condattr_destroy error"<<endl;
		return CStatus(-1, r);
	}

	return CStatus(0, 0);
}

CStatus CSharedConditionVariablePool::DestroyASharedObject(SSharedObjectHead *pObject)
{
	SSharedConditionVariableItem *pCondItem = (SSharedConditionVariableItem *)pObject;

	int r = pthread_cond_destroy(&(pCondItem->cond));
	if(r != 0)
	{
		cout << "In CSharedConditionVariablePool::DestroySharedObject(), pthread_cond_destroy error" << endl;
		return CStatus(-1, r);
	}

	return CStatus(0, 0);
}
