

#ifndef IINITIALFINISHEDNOTIFIER_H
#define IINITIALFINISHEDNOTIFIER_H

#include "CStatus.h"


/*
 * =====================================================================================
 *        Class:  IINITIALFINISHEDNOTIFIER_H
 *  Description:  该接口封装了一整套继承体系，主要用来通知
 *  			  当某段程序执行了初始化函数后的结果
 * =====================================================================================
 */
class IInitFinishedNotifier
{
	public:
	
	//假设线程A调用了初始化函数，并把结果返回给B

	//该函数当A调用完初始化函数后由A调用，并把初始化后的结果通过bIsSuccess传递给B
	virtual CStatus NotifyInitialFinished(bool bIsSuccess) = 0;

	//该函数由B来调用，用来检查A初始化后的结果
	virtual bool IsInitialSuccess() = 0;
};

#endif
