


#ifndef CTHREADINITIALFINISHEDNOTIFIER_H 
#define CTHREADINITIALFINISHEDNOTIFIER_H

#include "IInitFinishedNotifier.h"
#include "CEvent.h"

class CThreadInitFinishedNotifier : public IInitFinishedNotifier
{
	private:
	
	//用来在线程间阻塞的进行事件通知
	CEvent * m_pEvent;
	//标志初始化是否成功
	bool m_bIsSucess;

	public:

	CThreadInitFinishedNotifier(CEvent * pEvent);
	virtual ~CThreadInitFinishedNotifier();

	virtual CStatus NotifyInitialFinished(bool bIsSuccess);
	virtual bool IsInitialSuccess();
};


#endif
