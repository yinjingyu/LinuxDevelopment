/*
 * =====================================================================================
 * *       Filename:  test.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年10月22日 10时57分24秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "LibHeadFileAllInOne.h"

using namespace std;


int main()
{
	CExecutiveObject * p = new CProcess(new CUsrBizForProcess,true);
	if(!p->Run((void *)"./src/test/a.out").IsSuccess())
	{
		cout << "in main ,p->run failed"<<endl;
		return -1;
	}
	
 	return 0;
}
