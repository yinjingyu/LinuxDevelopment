/*
 * =====================================================================================
 *
 *       Filename:  CQuitMessage.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年11月03日 15时58分11秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "CQuitMessage.h"
#include "MessageIDTable.h"
CQuitMessage::CQuitMessage() : CMessage(QUIT_MESSAGE_LOOP)
{
	
}

CQuitMessage::~CQuitMessage()
{

}
