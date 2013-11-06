

#ifndef CCLIENTBUSINESSFOREXECUTIVE_H
#define CCLIENTBUSINESSFOREXECUTIVE_H

/*
 * =====================================================================================
 *        Class:  CClientBusinessForExecutive
 *  Description:  用户的业务逻辑，同时也是执行体要执行的方法
 * =====================================================================================
 */

#include "CStatus.h"

class IUsrBizForExecObj
{
	public:
	IUsrBizForExecObj();
	virtual ~IUsrBizForExecObj();

	//纯虚函数，执行用户的业务，需要由派生类重写
	virtual CStatus RunClientBusiness(void * pContext) = 0;

	private:
	IUsrBizForExecObj(const IUsrBizForExecObj &);
	IUsrBizForExecObj & operator = (const IUsrBizForExecObj &);
};

#endif
