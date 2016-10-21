#include"OGLSender.h"
#include<mutex>

OGLSender::OGLSender(std::shared_ptr<OGLReceiverPrivateData> privateData)
{
    /*shared*/
    mPrivateData = privateData;
}

OGLSender::~OGLSender()
{
    /*nothings*/
}

void OGLSender::postMessage(GLmsg message)
{
    /*send message*/
    begin();
    pushMessage(message);
    end();
}

void OGLSender::syncThread()
{
    /*wait process message*/
    std::unique_lock<std::mutex> lockWaitThread(mPrivateData->mMutexWaitThread);
    while (mPrivateData->mWaitThread) {
        mPrivateData->mConVarWaitThread.wait(lockWaitThread);
    }
    mPrivateData->mWaitThread = true;
}

void OGLSender::begin()
{
    /*lock queue*/
    mPrivateData->mMutexQueue.lock();
}

void OGLSender::pushMessage(GLmsg message)
{
    /*set message in queue*/
    mPrivateData->mQueueMessage.push(message);
}

void OGLSender::end()
{
    /*unlock queue*/
    mPrivateData->mMutexQueue.unlock();
    /*unlock getMessage*/
    mPrivateData->mMutexWaitMessage.lock();
    mPrivateData->mWaitMessage = false;
    mPrivateData->mMutexWaitMessage.unlock();
    mPrivateData->mConVarWaitMessage.notify_one();
}
