

#ifndef LIBHEADFILEALLINONE_H
#define LIBHEADFILEALLINONE_H

#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/wait.h>
#include <syscall.h>

#include <string.h>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#include <pthread.h>

#include <map>
#include <vector>


#include "IUsrBizForExecObj.h"
#include "CMessage.h"
#include "CUsrDefMsgQueue.h"
#include "CMsgLoopManager.h"
#include "CMsgLoopMgrUsingUsrDefQueue.h"
#include "MessageIDTable.h"
#include "CThread.h"
#include "CUsrBizUsingMsgLoop.h"
#include "CCommunicationNameServer.h"
#include "CThreadUsingMsgLoop.h"
#include "CUseMsgLoopWithoutThread.h"
#include "CStatus.h"
#include "CExecutiveObject.h"
#include "CProcess.h"


#include "CMutexInterface.h"
#include "CMutexByPThread.h"
#include "CMutexByRecordLocking.h"
#include "CMutexByRecordLockingAndPThread.h"

#include "CSharedMemory.h"

//进程相关用到的头文件

#include "CUsrBizForProcess.h"

#endif
