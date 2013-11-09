#ifndef CLibExecutiveInitializer_H
#define CLibExecutiveInitializer_H

#include <pthread.h>
#include "CStatus.h"

class CLibExecutiveInitializer
{
public:
	static CStatus Initialize();
	static CStatus Destroy();

private:
	CLibExecutiveInitializer();
	~CLibExecutiveInitializer();
	CLibExecutiveInitializer(const CLibExecutiveInitializer&);
	CLibExecutiveInitializer& operator=(const CLibExecutiveInitializer&);

private:
	static bool m_bInitialized;
	static bool m_bDestroyed;

	static pthread_mutex_t m_MutexForInitializer;
};

#endif
