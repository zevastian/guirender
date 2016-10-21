#include"OGLCommands.h"
#include<queue>
#include<condition_variable>
#include<mutex>

#ifndef _OGLRECEIVERPRIVATE_H_
#define _OGLRECEIVERPRIVATE_H_

struct OGLReceiverPrivateData {
    /*for send message*/
    std::queue<GLmsg> mQueueMessage;
    std::mutex mMutexQueue;
    std::condition_variable mConVarWaitMessage;
    std::mutex mMutexWaitMessage;
    bool mWaitMessage;
    /*for sync thread*/
    std::condition_variable mConVarWaitThread;
    std::mutex mMutexWaitThread;
    bool mWaitThread;
};

#endif
