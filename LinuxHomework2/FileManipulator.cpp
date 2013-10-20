/*
 * =====================================================================================
 *
 *       Filename:  FileManipulator.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年10月12日 10时53分36秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <iostream>

#include <string.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "FileManipulator.h"

#define FILEMANIPULATOR_BUFFER_SIZE 4096

FileManipulator::FileManipulator()
{
}

FileManipulator::FileManipulator(const char * pFileName)
{
	m_iFd = open(pFileName,O_CREAT|O_RDWR|O_APPEND,S_IRUSR|S_IWUSR);
	if(-1 == m_iFd)
		throw CStatus(-1,0,"error in FileManipulator:open file failed");
	
	//为缓冲区分配空间
	m_pFileBuffer = new char[FILEMANIPULATOR_BUFFER_SIZE];

	if(NULL == m_pFileBuffer)
		throw CStatus(-1,0,"error in FileManipulator:new buffer space failed!");

	//设置缓冲区的使用记录大小为0 	 	 	
	m_uiUsedBytesOfBuffer = 0;

	//保存文件名
	int filename_length = strlen(pFileName);
	m_pFileName = new char[filename_length+1];
	strcpy(m_pFileName,pFileName);
}

char * FileManipulator::GetFileName()
{
	return m_pFileName;
}

FileManipulator::~FileManipulator()
{
	//关闭文件
	if(-1 != m_iFd)
		close(m_iFd);

	//回收文件缓冲区
	if(NULL != m_pFileBuffer)
		delete m_pFileBuffer;
	
	//回收文件名空间
	if(NULL != m_pFileName)
		delete m_pFileName;
}

FileManipulator * FileManipulator::m_pFM = NULL;
char * FileManipulator::m_pFileName = NULL;
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  GetInstance
 *  Description:  单例模式获取文件操作对象
 * return value:  失败返回空，如果不为空说明文件已打开，可以直接通过成员变量m_pFM
 * 				  操作文件，不需要在检查m_pFM == -1
 * =====================================================================================
 */
FileManipulator * FileManipulator::GetInstance(const char * pFileName)
{
	if(NULL == m_pFM)
	{
		try
		{
			if(pFileName == NULL)
				throw CStatus(-1,0,"error in GetInstance of FileManipulator:pFileName is NULL");
		 	m_pFM = new FileManipulator(pFileName);

			if(atexit(OnProcessExit) != 0)
				m_pFM->m_bProcessIsExit = true;
			else
				m_pFM->m_bProcessIsExit = false;
		} 
		catch(CStatus status)
		{
			std::cout << status.GetErrorMsg() << std::endl;
			return NULL; 
		}
	}

	return m_pFM;
}

FileManipulator * FileManipulator::GetInstance()
{
	if(NULL != m_pFileName)
		return FileManipulator::GetInstance(FileManipulator::m_pFileName);
	else
	{
		std::cout << "can't find relevent file,please give a filename first!" << std::endl;
		return NULL; 	 	
	}
}

CStatus FileManipulator::WriteToFile(const char * pContent)
{
	try
	{
	 	if(NULL == pContent)
			throw CStatus(-1,0,"error in WriteToFile of class FileManipulator:pContent is invalid!");
		
		int content_size = strlen(pContent);
		if(content_size == 0)
			throw CStatus(-1,0,"error in WriteToFile of class FileManipulator : content_size is zero!");

		if(content_size > FILEMANIPULATOR_BUFFER_SIZE || m_bProcessIsExit)
		{
			//直接写文件前，要先刷缓存
			CStatus s = Flush();
			if(!s.IsSuccess())
				throw CStatus(-1,0,"error in WriteToFile of FileManipulator:Flush failed!");
			ssize_t r = write(m_iFd,pContent,content_size);	
			if(r == -1)
				throw CStatus(-1,0,"error in WriteToFile of class FileManipulator : write failed!");
			return CStatus(1,0);
		}

		int iLeftRoomOfBuffer = FILEMANIPULATOR_BUFFER_SIZE - m_uiUsedBytesOfBuffer;
		if(content_size > iLeftRoomOfBuffer)
		{
			CStatus s = Flush();
			if(!s.IsSuccess())
				throw CStatus(-1,1,"error in WriteToFile of FileManipulator:Flush failed");
		}

		memcpy(m_pFileBuffer,pContent,content_size);
		m_uiUsedBytesOfBuffer += content_size;
		return CStatus(1,0);
	}
	catch(CStatus status)
	{
		std::cout << status.GetErrorMsg() << std::endl;
		return status;
	}
}

CStatus FileManipulator::Flush()
{
	//如果缓存是空的，就不需要刷
	if(m_uiUsedBytesOfBuffer == 0)
		return CStatus(0,0);
	
	ssize_t r = write(m_iFd,m_pFileBuffer,m_uiUsedBytesOfBuffer);
	
	if(-1 == r)
		return CStatus(-1,0,"error in Flush of FileManipulator:write failed!");
	
	m_uiUsedBytesOfBuffer = 0;
	
	return CStatus(1,0);
}

CStatus FileManipulator::SetFileOperatePosition(off_t offset, int whence)
{
	//设置文件的操作位置前，为了保证读写一致要先刷缓存
	CStatus s = Flush();
	if(!s.IsSuccess())
		return s;
	
	off_t r = lseek(m_iFd,offset,whence);

	if(-1 == r)
		return CStatus(-1,0,"error in SetFileOperatePosition of FileManipulator:lseek failed");
	
	return CStatus(1,0);
}

CStatus FileManipulator::ReadFile(int nBytes,char * &pData)
{
	if(nBytes <= 0)
		return CStatus(-1,0,"error in ReadFile of FileManipulator:nBytes <= 0");

	//读文件前，要先把缓存中的内容刷新到文件中去
	CStatus s = Flush();
	if(!s.IsSuccess())
		return s;
	
	//根据用户指定的读取的字节数，判断读取的是字符串还是字符
	//如果读取的是字符串，要考虑字符串结束符的问题
	if(nBytes > 1)
		pData = new char[nBytes+1];
	else
		pData = new char[nBytes];

	ssize_t r = read(m_iFd,pData,nBytes);
	if(-1 == r)
		return CStatus(-1,0,"error in ReadFile of FileManipulator : read failed!");
	
	if(nBytes > 1)
		pData[nBytes] = '\0';

	return CStatus(1,0);
}

void FileManipulator::OnProcessExit()
{
	FileManipulator * pFM = FileManipulator::GetInstance(FileManipulator::m_pFileName);
	if(pFM != NULL)
	{
 	 	CStatus s = pFM->Flush();
 		if(!s.IsSuccess())
 			std::cout << s.GetErrorMsg() << std::endl;
 		pFM->m_bProcessIsExit = true;
	}
}









