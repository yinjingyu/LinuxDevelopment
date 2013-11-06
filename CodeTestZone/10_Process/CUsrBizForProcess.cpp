/*
 * =====================================================================================
 *
 *       Filename:  CUsrBizForProcess.cpp
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

#include "CUsrBizForProcess.h"

#include <string.h>
#include <iostream>
#include <unistd.h>
#include <vector>

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

	execv(argv[0],argv);

	std::cout << "In CUsrBizForProcess::RunClientBusiness , execv failed!" << std::endl;
 	
	delete []argv;

	delete []pstrCmdLine;
}
