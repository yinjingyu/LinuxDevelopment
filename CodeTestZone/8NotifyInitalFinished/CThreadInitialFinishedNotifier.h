


#ifndef CTHREADINITIALFINISHEDNOTIFIER_H 
#define CTHREADINITIALFINISHEDNOTIFIER_H

#include "IInitialFinishedNotifier.h"
#include "CEvent.h"

class CThreadInitialFinishedNotifier : public IInitialFinishedNotifier
{
	private:
	
	//用来在线程间阻塞的进行事件通知
	CEvent * m_pEvent;
	//标志初始化是否成功
	bool m_bIsSucess;

	public:

	CThreadInitialFinishedNotifier(CEvent * pEvent);
	virtual ~CThreadInitialFinishedNotifier();

	virtual CStatus NotifyInitialFinished(bool bIsSuccess);
	virtual bool IsInitialSuccess();
};


#endif
