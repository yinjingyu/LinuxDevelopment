#ifndef CSTATUS_H
#define CSTATUS_H


class CStatus
{
public:
	CStatus(int iReturnCode, int iErrorCode);
	CStatus(int iReturnCode, int iErrorCode, const char * msg);
 	CStatus(const CStatus & sta);
	virtual ~CStatus();
	
	//根据返回值(>=0)判断是否成功
	bool IsSuccess();
	char * GetErrorMsg();
public:
	const int & m_ciReturnCode;
	const int & m_ciErrorCode;

private:
	void InitializeMemberVars(int iReturnCode, int iErrorCode, const char * pErrorMsg);

private:
	int m_iReturnCode;
	int m_iErrorCode;
	//指向错误信息
	char * m_pErrorMsg;
};

#endif 
