/*
 * =====================================================================================
 *
 *       Filename:  CAddMessage.cpp
 *
 *    Description:  jjjj
 *
 *        Version:  1.0
 *        Created:  2013年11月02日 22时58分57秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "CAddMessage.h"
#include "./include/CMessage.h"
CAddMessage:: CAddMessage(int opt1,int opt2):CMessage(ADD_MESSAGE),m_cOpt1(m_opt1),m_cOpt2(m_opt2)
{
	m_opt1 = opt1;
	m_opt2 = opt2;
}
