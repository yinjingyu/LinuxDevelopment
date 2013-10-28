
#ifndef CENTERCRITICALSECTION_H
#define CENTERCRITICALSECTION_H

#include "CMutex.h"

class CEnterCriticalSection
{
	private:
	CMutex * m_pMutex;

	public:
	CEnterCriticalSection(CMutex * m_pMutex);
	virtual ~CEnterCriticalSection();
};

#endif
