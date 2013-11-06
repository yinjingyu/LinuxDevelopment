/* * ===================================================================================== * *       Filename:  CUsrBizForProcess.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年11月06日 12时22分08秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

//#include "CUsrBizForProcess.h"
//
//#include <string.h>
//#include <iostream>
//#include <unistd.h>
//#include <vector>

#include "LibHeadFileAllInOne.h"

using namespace std;

#define MAX_LENGTH_OF_PATH 1024

CUsrBizForProcess :: CUsrBizForProcess()
{

}

CUsrBizForProcess :: ~CUsrBizForProcess()
{

}

CStatus CUsrBizForProcess :: RunClientBusiness(void * pCmdLine)
{
	if( 0 == pCmdLine)
	{
		std::cout << "In CUsrBizForProcess::RunClientBusiness,pstrCmdLine is 0"  <<std::endl;
		return CStatus(-1,0); 	
	}

	char * pstrCmdLine = new char[strlen((char *)pCmdLine) + 1]; 
	
	strcpy(pstrCmdLine,(char *)pCmdLine);

	std::vector<char *> vstrArgs;

	while(char * p = strsep(&pstrCmdLine," "))
	{
		//处理遇到连续两个分割符时的情况
		if( *p == 0 )
			continue; 	
		
		vstrArgs.push_back(p);
	}

	//char * argv[]; --> typeof argv is : char **;
	char ** argv = new char * [vstrArgs.size()+1];
	
	for(int i = 0; i < vstrArgs.size(); i++)
	{
		argv[i] = vstrArgs[i];
	}
	
	argv[vstrArgs.size()] = NULL;

	//！！！！前面的所有都是为了构造argv数组，所白了最后还是为了调用execv

	try
	{
		char old_dir[MAX_LENGTH_OF_PATH];
		if(getcwd(old_dir,MAX_LENGTH_OF_PATH) == 0)
		{
			cout << "In CUsrBizForProcess::RunClientBusiness,getcwd failed" << endl;
			throw CStatus(-1,0);
		}
		
		if(!SetWorkDirectory(argv[0]).IsSuccess())
			throw CStatus(-1,0);
		
	 	execv(argv[0],argv);

		//如果能执行到下面的代码说明exec调用失败
		
		if(chdir(old_dir) == -1)
			cout << "In CUsrBizForProcess::RunClientBusiness,chdir failed！" << endl;
		
		//如果下面的错误输出是用日志的话，那么必须要放在chdir后面，这样我们才能回到原来的工作目录中
		//打印log
	 	std::cout << "In CUsrBizForProcess::RunClientBusiness , execv failed!" << std::endl;
	
		throw CStatus(-1,0);
	}
	catch(CStatus& s)
	{
	 	delete []argv;
	 	delete []pstrCmdLine;
		
		return s;
	}
}

CStatus CUsrBizForProcess :: SetWorkDirectory(char * pstrArgv0)
{
	string str = pstrArgv0;

	size_t pos = str.rfind("/");

	//如果子进程可执行文件路径中不带/，说明父进程的工作目录就是子进程的
	//工作目录
	if( pos == -1)
		return CStatus(0,0);

	//从0位置开始，截取pos+1长度大小的字符串
	string strDirectory = str.substr(0,pos+1);

	//当传进来的字符串类似于“./test/”时，下面的表达式成立
	//说明只有目录，没有指明文件
	if(str.length() == pos + 1)
	{
		cout << "In CUsrBizForProcess::SetWorkDirectory,str.length == pos+1"<<endl; 
		return CStatus(-1,0);
	}
	
	//截取从pos+1到字符串结束的所有字符，这样可以获得可执行文件名
	string strFileName = str.substr(pos+1);

	//文件名肯定比目录名的大小小，所以可以放心的strcpy
	//将去掉目录路径的文件名复制给argv的第一个元素
	strcpy(pstrArgv0,strFileName.c_str());
	
	//改变目录
	if(chdir(strDirectory.c_str()) == -1)
	{
		cout << "In CUsrBizForProcess::SetWorkDirectory,chdir failed" << endl;
 		return CStatus(-1,0);
	}

	return CStatus(0,0);
}


