/*
 * =====================================================================================
 *
 *       Filename:  CExecutive.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年10月17日 16时52分47秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "CExecutiveObject.h"
#include "IUsrBizForExecObj.h"

//在构造函数中，初始化指向业务逻辑的成员变量
CExecutiveObject::CExecutiveObject(CUsrBizForExecObj * pUsrBizForExecObj)
{
	if(0 == pUsrBizForExecObj)
	{
		throw CStatus(-1,0,"In CExecutiveObject::Construction pUsrBizForExecObj is null");
	}
	m_pUsrBizForExecObj = pUsrBizForExecObj;
}

CExecutiveObject::~CExecutiveObject()
{
	if(0 != m_pUsrBizForExecObj)
	{
		delete m_pUsrBizForExecObj;
		m_pUsrBizForExecObj = 0;
	}
}
