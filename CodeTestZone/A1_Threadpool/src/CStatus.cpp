/*
 * =====================================================================================
 *
 *       Filename:  CStatus.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年10月13日 11时42分09秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "CStatus.h"
#include <string.h>

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  InitializeMemberVars
 *  Description:  构造函数重用代码，用来初始化类的成员变量
 * =====================================================================================
 */
void CStatus::InitializeMemberVars(int iReturnCode, int iErrorCode, const char * pErrorMsg) 
{
	 m_iReturnCode = iReturnCode;
	 m_iErrorCode  = iErrorCode;
     //我们要给一个字符串分配空间，strlen函数并没有计算
	 //结束符\0的大小，而strcpy又会拷贝\0，所以必须+1
	 m_pErrorMsg = new char[strlen(pErrorMsg)+1];
	 strcpy(m_pErrorMsg,pErrorMsg);
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  CStatus
 *  Description:  双参构造函数，默认没有错误消息
 * =====================================================================================
 */
CStatus::CStatus(int iReturnCode, int iErrorCode):m_ciReturnCode(m_iReturnCode),m_ciErrorCode(m_iErrorCode)
{
	 InitializeMemberVars(iReturnCode,iErrorCode,"no error message");
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  CStatus
 *  Description:  三参构造函数
 * =====================================================================================
 */
CStatus::CStatus(int iReturnCode, int iErrorCode, const char * pErrorMsg):m_ciReturnCode(m_iReturnCode)
																			, m_ciErrorCode(m_iErrorCode)
{
	InitializeMemberVars(iReturnCode,iErrorCode,pErrorMsg);
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  CStatus
 *  Description:  拷贝构造函数，重写是为了防止默认拷贝构造函数会把引用变量指向错误对象
 * =====================================================================================
 */
CStatus::CStatus(const CStatus & sta):m_ciReturnCode(m_iReturnCode), m_ciErrorCode(m_iErrorCode)
{
	InitializeMemberVars(sta.m_ciReturnCode,sta.m_ciErrorCode,sta.m_pErrorMsg); 	
}

CStatus::~CStatus()
{
	//需要在析构函数中释放错误消息
//	if(0 != m_pErrorMsg)
//	{
//	 	delete m_pErrorMsg;
//		m_pErrorMsg = 0;
//	}
}

bool CStatus::IsSuccess()
{
	if(m_iReturnCode >= 0)
		return true;
	else
		return false;
}

char * CStatus::GetErrorMsg()
{
	return m_pErrorMsg;
}
