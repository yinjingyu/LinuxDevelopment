/*
 * =====================================================================================
 *
 *       Filename:  CProcess.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年11月06日 09时52分40秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <sys/types.h>
#include <wait.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <syscall.h>

#include "CProcess.h"
#include "IUsrBizForExecObj.h"
#include "CExecutiveObject.h"

using namespace std;

#define LENGTH_OF_PROCESSID 25
#define LENGTH_OF_PATH 10245


CProcess::CProcess(IUsrBizForExecObj * pUsrBizForExecObj):CExecutiveObject(pUsrBizForExecObj)
{
	m_bProcessCreated = false;
	m_bWaitForDeath   = false;
	m_bExecSuccess    = true;
}

CProcess::CProcess(IUsrBizForExecObj * pUsrBizForExecObj, bool bWaitForDeath):CExecutiveObject(pUsrBizForExecObj)
{
	m_pUsrBizForExecObj = pUsrBizForExecObj;
	m_bProcessCreated   = false;
	m_bWaitForDeath     = bWaitForDeath;
	m_bExecSuccess      = true;
	
}

CProcess::~CProcess()
{
	//业务逻辑类会在基类中析构
}


void CProcess :: RunBizOfChildProcess(void * pstrCmdLine)
{
	//为了消除getpid有刷缓存的bug，我们直接通过系统调用来获取进程ID
	m_ProcessID = syscall(SYS_getpid); 	

	//帮助子进程在开始执行自己的业务逻辑前，关掉从父进程继承过来的
	//一些不相关文件
	CloseFileDescriptor();

	//开始在用户的业务逻辑类中调用exec方法，启动另一个可执行文件，
	//注意该方法正常情况下不返回
	m_pUsrBizForExecObj->RunClientBusiness(pstrCmdLine);

	//子进程调用exec失败 	 	 	
	m_bExecSuccess = false;

	_exit(0);
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  Run
 *  Description:  创建并执行进程，参数是执行子进程时的命令行参数
 * =====================================================================================
 */
CStatus CProcess :: Run(void * pstrCmdLine)
{
	//防止在父进程中用同一个CProcess的实例对象来重复调用Run（即重复创建进程）
	if(m_bProcessCreated)
	{
		std::cout << "In CProcess::Run , m_bProcessCreated is true!" <<std::endl;
		return CStatus(-1,0);
	}

	pid_t pid = vfork();
	if( pid == 0)
	{
		if(m_bWaitForDeath)
			RunBizOfChildProcess(pstrCmdLine);
		else
		{
			//子进程创建孙子进程，并在孙子进程中执行新的业务逻辑
			//子进程成功创建完孙子进程返回后，子进程立刻调用_exit返回
			//这样就可以把孙子进程托管给init进程了
			pid_t pid1 = vfork();
			if(0 == pid1)
			{
				cout << "In CProcess::Run, pid_t pid1 = vfork() failed" << endl;
				m_bExecSuccess = false;
				_exit(0);
			}
			//孙子进程创建成功
			if(pid1 == 0)
			{
				RunBizOfChildProcess(pstrCmdLine);
			}
			//子进程成功创建完孙子进程立即退出
			_exit(0);
		}
	}
	else if( pid == -1)
	{
		std::cout << "In CProcess::Run,vfork returned -1" << std::endl;
		return CStatus(-1,0);
	}
	else
	{

		//处理不需要等待孙子进程死亡的情况：
		//子进程调用_exit退出后，如果不回收他的资源会变成僵尸进程
		//所以这里我们还要为回收子进程
		if(!m_bWaitForDeath)
		{
			if(waitpid(pid,0,0) == -1)
				cout << "In CProcess::Runun , waitpid return -1" <<endl;
		}

		//子进程创建成功，但在子进程执行业务逻辑去调用exec时出错返回
		if(!m_bExecSuccess)
		{
			delete this;
			return CStatus(-1,0);
		}

		//子进程创建成功，exec也调用成功
		m_bProcessCreated = true;

		//如果父进程不需要等待子进程死亡，那么子进程创建成功后，CProcess直接被析够,因为CProcess 就是父进程用来创建子进程的一个接口
		if(!m_bWaitForDeath)
			delete this;

		return CStatus(0,0);
	}
}

CStatus CProcess :: WaitForDeath()
{
	if(!m_bWaitForDeath)
	{
		std::cout << "In CProcess::WaitForDeath,m_bWaitForDeath is false" << std::endl;
		return CStatus(-1,0);
	}

	if(!m_bProcessCreated)
	{
		std::cout << "In CProcess::WaitForDeath,m_bProcessCreated is false" <<std::endl;
		return CStatus(-1,0);
	}

	//等待进程死亡
	if(waitpid(m_ProcessID,0,0) == -1)
	{
		std::cout << "In CProcess::WaitForDeath,waitpid failed!" <<std::endl;
		return CStatus(-1,0);
	}

	//进程死亡后，那么代表进程的CProcess实例对象也应该被释放掉
	delete this;

	return CStatus(0,0);
}

CStatus CProcess::CloseFileDescriptor()
{
	string strPath = "/proc/";

	char strProcessID[LENGTH_OF_PROCESSID+1];
	snprintf(strProcessID,LENGTH_OF_PROCESSID,"%d",m_ProcessID);

	strPath += strProcessID;
	strPath += "/fd";

	string strFdPath = strPath;

	strPath += "/";

	DIR * pDir = opendir(strPath.c_str());
	if( 0 == pDir)
	{
		std::cout <<"In CProcess::CloseFileDescriptor, opendir failed,pDir is 0!"<< std::endl;
		return CStatus(-1,0);
	}

	//子进程关闭从父进程继承过来的无关文件
	while(struct dirent * pDirent = readdir(pDir))
	{
		//keep this file
 	 	char firstChar = pDirent->d_name[0];
		if(firstChar == '.' || firstChar == '0' || firstChar == '1' || firstChar == '2')
		 	continue;	

		//if pDirent->d_name is not a number,then skip
		int fd = atoi(pDirent->d_name);
		if(fd != 0)
		{
			//strTmpPath is a sympol link , now we will get the real path of it
			
			//获得符号链接文件的路径
			string strTmpPath = strPath+pDirent->d_name;
			char realPathName[LENGTH_OF_PATH+1];
			
			//通过readlink可以获取符号列键的真实文件的路径
			if(readlink(strTmpPath.c_str(),realPathName,LENGTH_OF_PATH + 1) == -1)
			{
				std::cout <<"In CProcess::CloseFileDescriptor, readlink == -1"<< std::endl;
				return CStatus(-1,0);
			}

			//判断该文件是否是表示当前目录 /proc/PROCESS_ID/fd/ 的文件，
			//如果是，显然我们不能关掉他,要等到目录遍历完成后再关闭目录
			if(strcmp(strFdPath.c_str(),realPathName) == 0)
				continue;
			
			if(close(fd) == -1)
			{
				std::cout <<"In CProcess::CloseFileDescriptor , close fd failed!"<< std::endl;
				return CStatus(-1,0);
			}
		}
	}

	//关闭/proc/ID/fd/ 目录
	if(closedir(pDir) == -1)
	{
		std::cout <<"In CProcess::CloseFileDescriptor,close dir failed!" << std::endl;
		return CStatus(-1,0);
	}
	
	return CStatus(0,0);
}


