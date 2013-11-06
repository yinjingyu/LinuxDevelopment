


#ifndef CPROCESS_H
#define CPROCESS_H

#include "CExecutiveObject.h"
#include "CStatus.h"
#include "CExecutiveObject.h"
#include "IUsrBizForExecObj.h"

#include <unistd.h>

class CProcess : public CExecutiveObject
{
	public:
	explicit CProcess(IUsrBizForExecObj * pUsrBizForExecObj);

	CProcess(IUsrBizForExecObj * pUsrBizForExecObj, bool bWaitForDeath);
	~CProcess();

	//重写基类的虚函数，run即创建具体的执行体并执行。
	virtual CStatus Run(void * pContext);

	//等待执行体死亡
	virtual CStatus WaitForDeath();

	private:
	
	//用vfork创建的子进程也会继承父进程的文件描述符，所以
	//要在子进程刚创建好的时候，删除所有的相干描述符
	CStatus CloseFileDescriptor(); 	

	//调用exec函数，这之前还会调用CloseFileDescriptor等其他函数
	void RunBizOfChildProcess(void * pstrCmdLine);

	private:

	//保存进程ID，其他打算与进程交互或操作进程的函数会用到
	pid_t m_ProcessID;

	//vfork 函数调用成功即进程被成功创建后 设置该值为true
	bool m_bProcessCreated;

	//标志是否需要等待进程死亡
	bool m_bWaitForDeath;

	//判断exec函数是否执行成功，初始值是true
	bool m_bExecSuccess;
};

#endif
