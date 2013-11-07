/*
 * =====================================================================================
 *
 *       Filename:  ISharedObjects.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年11月07日 14时57分54秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "ISharedObjectPool.h"
#include "CStatus.h"
#include "CSharedMemory.h"

#include <string.h>
#include <iostream>
using namespace std;

ISharedObjectPool::ISharedObjectPool()
{
}

ISharedObjectPool :: ~ISharedObjectPool()
{
}

CStatus ISharedObjectPool ::Initialize()
{
	//创建一块共享存储区域,并返回映射到进程地址空间的首地址
	//共享存储区域大小 = （4bytes大小）幻数 + NUMBER_OF_SHARED_OBJECT 个m_nItemSize 大小的具体共享存储对象		 	 	 	 	 	 	 	 	 	
 	m_pSharedMemory = new CSharedMemory(m_strSharedSpaceName.c_str(), sizeof(int) + NUMBER_OF_SHARED_OBJECT * m_nItemSize);

	//将首地转换成int *,表明，首地址往下4个字节大小空间上的数据保存的是幻数
 	int *pMagicNumber = (int *)(m_pSharedMemory->GetAddress());

	//取得第一块共享数据对象的头部地址
    m_pSharedObjects = (SSharedObjectHead *)((char *)pMagicNumber + sizeof(int));

	//检查共享存储对象的幻数存储区域是否被赋值初始化过
	//如果被初始化过，跳过
    if((*pMagicNumber) == MAGIC_NUMBER_FOR_SHARED_OBJECT)
    	return CStatus(0, 0);

	//初始化每一块共享存储对象，有一块初始化失败就算整个存储池初始化失败
	bool bSuccess = true;
	
	//初始化每一块共享存储对象的头部数据
    for(int i = 0; i < NUMBER_OF_SHARED_OBJECT; i++)
    {
		//定位每一块共享存储区域的首地址，并取得头部结构体的指针
    	SSharedObjectHead *pItem = (SSharedObjectHead *)((char *)m_pSharedObjects + i * m_nItemSize);

		//如果头部地址的开始4个字节为0表示该数据头还没被初始化过
        if(pItem->Status == UNINITIALIZED_SHARED_OBJECT)
        {
        	if(InitializeASharedObject(pItem).IsSuccess())
            {
              	pItem->Status = INITIALIZED_SHARED_OBJECT;
                pItem->RefCount = 0;
                memset(pItem->strSharedObjectName, 0, LENGTH_OF_SHARED_OBJECT_NAME);
            }
            else
              bSuccess = false;
        }
   	}

    if(bSuccess)
       *pMagicNumber = MAGIC_NUMBER_FOR_SHARED_OBJECT;

    return CStatus(0, 0);
}

CStatus ISharedObjectPool::Destroy()
{
        delete m_pSharedMemory;
        return CStatus(0, 0);
}

void *ISharedObjectPool :: GetASharedObject(const char *pstrSharedObjectName)
{
	int empty_index = -1;
    bool bfind_empty = false;

    for(int i = 0; i < NUMBER_OF_SHARED_OBJECT; i++)
    {
     	SSharedObjectHead *pItem = (SSharedObjectHead *)((char *)m_pSharedObjects + i * m_nItemSize);

        if(pItem->Status == UNINITIALIZED_SHARED_OBJECT)
                continue;

        if(pItem->Status == INITIALIZED_SHARED_OBJECT)
        {
                if(bfind_empty)
                        continue;

                empty_index = i;
                bfind_empty = true;

                continue;
        }

        if(pItem->Status == ALLOCATED_SHARED_OBJECT)
        {
                if(strcmp(pstrSharedObjectName, pItem->strSharedObjectName) == 0)
                {
                        pItem->RefCount++;

                        return (char *)pItem + sizeof(SSharedObjectHead);
                }
        }
    }

    if(empty_index != -1)
    {
      	SSharedObjectHead *pItem = (SSharedObjectHead *)((char *)m_pSharedObjects + empty_index * m_nItemSize);

        pItem->Status = ALLOCATED_SHARED_OBJECT;
        pItem->RefCount = 1;

        strcpy(pItem->strSharedObjectName, pstrSharedObjectName);

        return (char *)pItem + sizeof(SSharedObjectHead);
    }
    else
    {
        cout<<"In CLSharedObjectsImpl::GetSharedObject(), shared memory is full" << endl;
        return 0;
    }
}

CStatus ISharedObjectPool::ReleaseASharedObject(const char *pstrSharedObjectName)
{
	for(int i = 0; i < NUMBER_OF_SHARED_OBJECT; i++)
    {
     	SSharedObjectHead *pItem = (SSharedObjectHead *)((char *)m_pSharedObjects + i * m_nItemSize);

        if(pItem->Status == ALLOCATED_SHARED_OBJECT)
        {
         	if(strcmp(pstrSharedObjectName, pItem->strSharedObjectName) == 0)
            {
             	pItem->RefCount--;

                if(pItem->RefCount == 0)
                {
                	DestroyASharedObject(pItem);

                    pItem->Status = UNINITIALIZED_SHARED_OBJECT;
                    memset(pItem->strSharedObjectName, 0, LENGTH_OF_SHARED_OBJECT_NAME);

                    if(InitializeASharedObject(pItem).IsSuccess())
                    {
                            pItem->Status = INITIALIZED_SHARED_OBJECT;
                    }
                }

                return CStatus(0, 0);
            }
        }
    }
    return CStatus(-1, 0);
}
