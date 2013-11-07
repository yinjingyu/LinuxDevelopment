

#ifndef CUSRBIZFORPROCESS_H
#define CUSRBIZFORPROCESS_H

#include "CStatus.h"
#include "IUsrBizForExecObj.h"

class CUsrBizForProcess : public IUsrBizForExecObj
{
	public:
	CUsrBizForProcess();
	virtual ~CUsrBizForProcess();

	virtual CStatus RunClientBusiness(void * pCmdLine);

	private:

	CStatus SetWorkDirectory(char * pstrArgv0);
};


#endif
