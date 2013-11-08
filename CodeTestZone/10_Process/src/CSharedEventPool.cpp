#include "CSharedEventPool.h"

#define SHARED_SPACE_FOR_SHARED_EVENT_ALLOCATOR "shared_space_for_shared_event_allocator"

CSharedEventPool::CSharedEventPool()
{
	m_nItemSize = sizeof(SSharedEventItem);
	m_strSharedSpaceName = SHARED_SPACE_FOR_SHARED_EVENT_ALLOCATOR;
}

CSharedEventPool::~CSharedEventPool()
{
}

CStatus CSharedEventPool::InitializeSharedObject(SSharedObjectHead *pObject)
{
	SSharedEventItem *pEventItem = (SSharedEventItem *)pObject;

	pEventItem->EventInfo.Flag = 0;
	pEventItem->EventInfo.bSemaphore = 0;
	pEventItem->EventInfo.Context = 0;

	return CStatus(0, 0);
}

CStatus CSharedEventPool::DestroySharedObject(SLSharedObjectHead *pObject)
{
	SSharedEventItem *pEventItem = (SSharedEventItem *)pObject;
	
	pEventItem->EventInfo.Flag = 0;
	pEventItem->EventInfo.bSemaphore = 0;
	pEventItem->EventInfo.Context = 0;

	return CStatus(0, 0);
}
