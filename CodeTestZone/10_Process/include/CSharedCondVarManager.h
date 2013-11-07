#ifndef CSharedCondVarManager_H
#define CSharedCondVarManager_H

#include <pthread.h>
#include "CStatus.h"

class CSharedConditionVariablePool;

class CSharedCondVarManager
{
public:
	static CSharedCondVarManager* GetInstance();

	static pthread_cond_t* Get(const char *pstrCondName);
	static CStatus Release(const char *pstrCondName);

	pthread_cond_t *GetSharedCond(const char *pstrCondName);
	CStatus ReleaseSharedCond(const char *pstrCondName);

	friend class CLibExecutiveInitializer;

private:
	static CStatus Create();
	static CStatus Destroy();

private:
	CSharedCondVarManager();
	virtual ~CSharedCondVarManager();

	CSharedCondVarManager(const CSharedCondVarManager&);
	CSharedCondVarManager& operator=(const CSharedCondVarManager&);

private:
	static CSharedCondVarManager *m_pShCondVarMgr;
	CSharedConditionVariablePool *m_pShCondVarPool;
};

#endif
