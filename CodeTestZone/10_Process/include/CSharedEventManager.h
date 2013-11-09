#ifndef CSharedEventManager_H
#define CSharedEventManager_H

#include "CStatus.h"

struct SEventInfo;
class CSharedEventPool;

class CSharedEventManager
{
public:
	static CSharedEventManager* GetInstance();

	static SEventInfo* Get(const char *pstrEventName);
	static CStatus Release(const char *pstrEventName);

	SEventInfo *GetASharedEventInfo(const char *pstrEventName);
	CStatus ReleaseAEventInfo(const char *pstrEventName);

	friend class CLibExecutiveInitializer;

private:
	static CStatus Create();
	static CStatus Destroy();

private:
	CSharedEventManager();
	virtual ~CSharedEventManager();

	CSharedEventManager(const CSharedEventManager&);
	CSharedEventManager& operator=(const CSharedEventManager&);

private:
	static CSharedEventManager *m_pShEventMgr;
	CSharedEventPool *m_pShEventPool;
};

#endif
