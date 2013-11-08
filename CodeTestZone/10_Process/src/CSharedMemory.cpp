/*
 * =====================================================================================
 *
 *       Filename:  CSharedMemory.cpp
 *
 *    Description:  
 *        Version:  1.0
 *        Created:  2013年11月07日 04时44分14秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <string>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <iostream>

#include "CSharedMemory.h"
#include "CStatus.h"
#include "CEnterCriticalSection.h"

using namespace std;

#define ID_FOR_KEY 32
#define FILE_PATH_FOR_SHARED_MEMORY "/tmp/"

CSharedMemory :: CSharedMemory(const char * pstrFileName, size_t nSize) : m_Mutex(pstrFileName,MUTEX_USE_RECORD_LOCK)
{
	string strPathName = FILE_PATH_FOR_SHARED_MEMORY;
	strPathName += pstrFileName;

	//1、
	//由于共享存储外部名的构建依赖于一个已经存在的文件的索引节点号
	//所以这里我们确定确实存在一个文件，但不需要对文件做任何读写操作
	//所以打开之后马上再关上
	int fd = open(strPathName.c_str(), O_RDWR | O_CREAT, S_IRUSR, S_IWUSR);
	if( fd == -1)
	{
		cout << "In CSharedMemory::constructor ,open failed！" << endl;
		throw "";
	}
	if( close(fd) == -1)
	{
		cout << "In CSharedMemory::constructor,close failed!" << endl;
		throw "";
	}

	//2、根据文件的索引节点号和传入的整型常量来生成共享存储的外部名
	//，外部名是一个Key_t 类型的数据
	key_t key = ftok(strPathName.c_str(), ID_FOR_KEY); 	
	if( key == -1)
	{
		cout << "In CSharedMemory::constructor,ftok failed!" << endl;
		throw "";
	}

	try
	{
		//防止获取共享时，又会有其他进程删除共享存储
		CEnterCriticalSection cs(&m_Mutex);

		//调用API获取共享存储
		m_SharedMemoryID = shmget(key, nSize, IPC_CREAT);
		if(m_SharedMemoryID == -1)
		{
			cout<< "In CSharedMemory::constructor,shmget failed!" << endl;
			throw "";
		}
		
		//让系统帮我们从当前进程空间中找一段地址来映射共享内存
 	 	m_pSharedMemory = shmat(m_SharedMemoryID,0,0);
		if((long)m_SharedMemoryID == -1)
		{
			cout << "In CSharedMemory::constructor shmat failed!" << endl;
			//共享存储已经创建，但映射失败，所以要释放
			DeleteSharedMemory();
			throw "";
		}
	}
	catch(...)
	{
		cout << "error in CSharedMemory::constructor,catch error in try" << endl;
	}
}

CSharedMemory :: ~CSharedMemory()
{
	CEnterCriticalSection cs(&m_Mutex);

	//解除进程地址空间和共享存储之间的映射
	//PS.但没有删除共享存储
	if(shmdt(m_pSharedMemory) == -1)
	{
		cout<<"In CSharedMemory::~CSharedMemory,shmdt failed"<<endl;
 	 	return;
	}
		
	DeleteSharedMemory();
}

void * CSharedMemory :: GetAddress()
{
	return m_pSharedMemory;
}

CStatus CSharedMemory :: DeleteSharedMemory()
{
	//只有当引用计数为零时，才去调用api释放共享内存
	if(GetRefCount() == 0)
	{
		if(shmctl(m_SharedMemoryID,IPC_RMID,0) == -1)
		{
			cout << "In CSharedMemory::DeleteSharedMemory,shmctl failed!" << endl;
			return CStatus(-1,0);
		}
	}

	return CStatus(0,0);
}

int CSharedMemory :: GetRefCount()
{
	shmid_ds buf;
	if(shmctl(m_SharedMemoryID, IPC_STAT, &buf) == -1)
 	{
		cout << "In CSharedMemory::GetRefCount,shmctl failed!" << endl;
 	 	return -1;
	}
	
	return buf.shm_nattch;
}
