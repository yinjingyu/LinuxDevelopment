

#ifndef CUSRBIZFORPROCESS_H
#define CUSRBIZFORPROCESS_H

#include "IUsrBizForExecObj.h"

class CUsrBizForProcess : public IUsrBizForExecObj
{
	public:
	CUsrBizForProcess();
	virtual ~CUsrBizForProcess();

	virtual CStatus RunClientBusiness(void * pCmdLine);
};


#endif
