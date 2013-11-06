

#ifndef CEXECUTIVE_H
#define CEXECUTIVE_H

/*
 * =====================================================================================
 *        Class:  CExecutive
 *  Description:  执行体，业务逻辑的动力驱动
 * =====================================================================================
 */
#include "CStatus.h"
#include "IUsrBizForExecObj.h"
class CExecutiveObject
{
	public:
	explicit CExecutiveObject(IUsrBizForExecObj * pUsrBizForExecObj);
	virtual ~CExecutiveObject();

	//调用系统API，创建执行体函数
	virtual CStatus Run(void * pContext) = 0;

	//等待执行体死亡
	virtual CStatus WaitForDeath() = 0;
	
	protected:

	//执行体需要执行的具体业务逻辑
	IUsrBizForExecObj * m_pUsrBizForExecObj;
};

#endif
