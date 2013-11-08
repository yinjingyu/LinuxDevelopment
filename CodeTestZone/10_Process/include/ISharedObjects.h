


#ifndef ISharedObjects_H
#define ISharedObjects_H

#include "CSharedMemory.h"
#include "CStatus.h"
#include <string>

using namespace std;

#define LENGTH_OF_SHARED_OBJECT_NAME 256

#define UNINITIALIZED_SHARED_OBJECT 0
#define INITIALIZED_SHARED_OBJECT 1
#define ALLOCATED_SHARED_OBJECT 2

#define NUMBER_OF_SHARED_OBJECT 1024
#define MAGIC_NUMBER_FOR_SHARED_OBJECT 0x12345678

//共享池中有多个共享对象，为了方便管理，抽象出所有对象的共同点
//作为共享对象的头部
struct SSharedObjectHead
{
	//共享对象的状态，共由3种状态
	//1、UNINITIALIZED_SHARED_OBJECT 没有初始化
	//2、INITIALIZED_SHARED_OBJECT 已经初始化
	//3、ALLOCATED_SHARED_OBJECT  已经分配
	int Status;

	//共享对象的引用计数 	
	int RefCount;

	//共享对象的名称
	char strSharedObjectName[];
};

class ISharedObjects
{
public:
	ISharedObjects();
	virtual ~ISharedObjects();

	//下面两个函数是接口自己要实现的函数
	//用来做所有类型的共享对象共有的初始化操作
	CStatus Initialize();
	CStatus Destroy();

	void * GetSharedObject(const char * pstrSharedObjectName);  
	CStatus ReleaseSharedObject(const char * pstrSharedObjectName);

	//共享池中的每一个共享对象针对不同类型的共享对象可能都还要做不同的初始化和销毁
	//工作，所以再提供两个虚函数给派生类，来实现具体的初始化和销毁
	virtual CStatus InitializeSharedObjectItem(SSharedObjectHead * pObject) = 0;
	virtual CStatus DestroySharedObjectItem(SSharedObjectHead * pObject) = 0;

protected:
	//指向共享池的首地址处 	
	CSharedMemory * m_pSharedMemory;

	//共享池中每个具体的共享对象的大小（要包括头部大小）
	//这个量要留给派生类来根据自己的情况设置
	unsigned int m_nItemSize;
	
	//共享对象池的名字,方便外部访问
	string m_strSharedSpaceName;

	//指向共享池中，第一个可以实际拿来用的共享对象的首地址
	SSharedObjectHead * m_pSharedObjects;
};

#endif
