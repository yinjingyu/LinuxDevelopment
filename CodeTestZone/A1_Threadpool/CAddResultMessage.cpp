/*
 * =====================================================================================
 *
 *       Filename:  CAddResultMessage.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年11月04日 15时11分38秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "CAddResultMessage.h"
#include "MessageIDTable.h"

CAddResultMessage::CAddResultMessage(int iResult):CMessage(ADDRESULT_MESSAGE),m_ciResult(m_iResult)
{
	m_iResult = iResult;
}
