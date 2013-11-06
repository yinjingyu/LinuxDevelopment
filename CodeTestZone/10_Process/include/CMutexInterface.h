


#ifndef CMUTEXINTERFACE_H
#define CMUTEXINTERFACE_H

#include "LibHeadFileAllInOne.h"

class CMutexInterface
{
public:
	CMutexInterface();
	virtual ~CMutexInterface();

	virtual CStatus Initialize() = 0;
	virtual CStatus Uninitialize() = 0;

	virtual CStatus Lock() = 0;
	virtual CStatus Unlock() = 0;

private:
	CMutexInterface(const CMutexInterface &);
	CMutexInterface & operator=(const CMutexInterface &);
};

#endif
