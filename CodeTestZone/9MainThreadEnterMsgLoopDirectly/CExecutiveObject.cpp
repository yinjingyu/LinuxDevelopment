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
	m_pUsrBizForExecObj = pUsrBizForExecObj;
}

CExecutiveObject::~CExecutiveObject()
{

}
