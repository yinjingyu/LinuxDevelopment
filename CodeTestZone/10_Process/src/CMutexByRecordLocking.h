
#ifndef CMUTEXBYRECORDLOCKING_H
#define CMUTEXBYRECORDLOCKING_H

class CMutexByRecordLocking : public CMutexInterface
{
public:
	CMutexByRecordLocking(const char * pstrFileName);	
	virtual ~CMutexByRecordLocking();

	virtual CStatus Initialize();
	virtual CStatus Uninitialize();

	virtual CStatus Lock();
	virtual CStatus Unlock();

private:
	CMutexByRecordLocking(const CMutexByRecordLocking & ); 	
	CMutexByRecordLocking & operator=(const CMutexByRecordLocking &);

private:
	//用于作为记录锁的文件的文件描述符号
	int m_Fd;

	//记录锁的文件名	 	
	string m_strFileName;
};

#endif
