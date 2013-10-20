/*
 * =====================================================================================
 *
 *       Filename:  Client.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年10月12日 10时59分20秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "FileManipulator.h"
#include <unistd.h>

using namespace std;

int main()
{
	FileManipulator * pFM = FileManipulator::GetInstance("file.data");
	if(NULL == pFM)
	{
		cout << "error"<< endl;
		return -1;
	}

	pFM->WriteToFile("Hello World!");

	pFM->SetFileOperatePosition(0,SEEK_SET);	

	char * data;
	pFM->ReadFile(6,data);
	
	cout << data << endl;

	pFM->SetFileOperatePosition(-6,SEEK_END); 	
	char * data2;
	pFM->ReadFile(6,data2);
	cout << data2 << endl;

	return 0;
}
