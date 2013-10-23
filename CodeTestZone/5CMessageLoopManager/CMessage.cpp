/*
 * =====================================================================================
 *
 *       Filename:  CMessage.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年10月21日 17时10分07秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include "CMessage.h"

CMessage::CMessage(unsigned long lMsgID):m_clMsgID(m_lMsgID)
{
	m_lMsgID = lMsgID;
}

CMessage:: ~CMessage()
{

}

CMessage:: CMessage(const CMessage &cm):m_clMsgID(m_lMsgID)
{
	m_lMsgID = cm.m_clMsgID;
}















