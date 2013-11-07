/*
 * =====================================================================================
 *
 *       Filename:  CMutexByRecordLocking.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年11月07日 02时17分57秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <string>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <string.h>

#include "CMutexByRecordLocking.h"
#include "CStatus.h"

using namespace std;

//记录锁存放的位置
#define FILE_PATH_FOR_RECORD_LOCKING "/tmp/"

CMutexByRecordLocking :: CMutexByRecordLocking(const char * pstrFileName)
{
	if((pstrFileName == 0) || (strlen(pstrFileName) == 0))
	{
		cout << "In CMutexByRecordLocking::constructor,pstrFileName bad!" << endl;
		throw "";
	}

	m_strFileName = FILE_PATH_FOR_RECORD_LOCKING;
	m_strFileName += pstrFileName;
}

CMutexByRecordLocking :: ~CMutexByRecordLocking()
{

}

CStatus CMutexByRecordLocking :: Initialize()
{
	return CStatus(0,0); 	
}

CStatus CMutexByRecordLocking :: Uninitialize()
{
	return CStatus(0,0);
}

CStatus CMutexByRecordLocking :: Lock()
{
	m_Fd = open(m_strFileName.c_str(),O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	if(m_Fd == -1)
	{
		cout << "In CMutexByRecordLocking :: Lock ,open return -1" << endl;
		return CStatus(-1,0);
	}

	//设置锁的属性（写锁）
	struct flock lock;
	lock.l_type   = F_WRLCK;
	lock.l_start  = 0;
	lock.l_whence = SEEK_SET;
	lock.l_len    = 0;

	//开始加锁
	if(fcntl(m_Fd,F_SETLKW,&lock) == -1)
	{
		cout << "In CMutexByRecordLocking::Lock,fcntl failed!" << endl;
		return CStatus(-1,0);
	}

	return CStatus(0,0);
}

CStatus CMutexByRecordLocking :: Unlock()
{
	struct flock lock;
	lock.l_type   = F_UNLCK;
	lock.l_start  = 0;
	lock.l_whence = SEEK_SET;
	lock.l_len    = 0;

	if(fcntl(m_Fd, F_SETLKW, &lock) == -1)
	{
		cout << "In CMutexByRecordLocking::Unlock , fcntl failed!" << endl;
		return CStatus(-1,0);
	}
	
	//为什么关闭文件失败也返回正确？？？
	if( close(m_Fd) == -1)
		cout << "In CMutexByRecordLocking::Unlock, fcntl failed!" << endl;		

	return CStatus(0,0);
}
















