
#ifndef FILE_MANIPULATOR_H
#define FILE_MANIPULATOR_H

#include <iostream>
#include "CStatus.h"
#include <unistd.h>

class FileManipulator
{
public:
	static FileManipulator * GetInstance(const char * filename);	
	static FileManipulator * GetInstance();	
    CStatus WriteToFile(const char * pContent);
	
	CStatus Flush();
	CStatus ReadFile(int nBytes,char * &pData);
 	CStatus SetFileOperatePosition(off_t offset, int whence);
	char * GetFileName();

private:
	//不带参数的构造函数
    FileManipulator();

	//单参构造函数
	FileManipulator(const char * filename);

	//需要在atexit中注册的类清理函数，进程退出时调用 	 	
	static void OnProcessExit();

    virtual ~FileManipulator();

private:

	//文件描述符
    int m_iFd;
	
	//指向缓冲区的指针
	char * m_pFileBuffer;
	//缓冲区已经使用的大小
	unsigned int m_uiUsedBytesOfBuffer;

	//单例模式下的文件操作对象
	static FileManipulator * m_pFM;

 	static char * m_pFileName; 	

	bool m_bProcessIsExit; 	
};

#endif
