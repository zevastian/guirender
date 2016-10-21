#include"OGLReceiver.h"

OGLReceiver::OGLReceiver()
{
    /*make shared data*/
    mPrivateData = std::shared_ptr<OGLReceiverPrivateData> (new OGLReceiverPrivateData);
    mPrivateData->mWaitMessage = true;
    mPrivateData->mWaitThread = true;
}

OGLReceiver::~OGLReceiver()
{
    /*nothings*/
}

void OGLReceiver::getMessage(GLmsg &message)
{
    bool emptyQueue;
    while (true) {
        mPrivateData->mMutexQueue.lock();
        emptyQueue = mPrivateData->mQueueMessage.empty();
        if (!emptyQueue) {
            message = mPrivateData->mQueueMessage.front();
            mPrivateData->mQueueMessage.pop();
        }
        mPrivateData->mMutexQueue.unlock();
        if (!emptyQueue) {
            break;
        }

        /*wait message*/
        std::unique_lock<std::mutex> lockWaitMessage(mPrivateData->mMutexWaitMessage);
        while(mPrivateData->mWaitMessage) {
            mPrivateData->mConVarWaitMessage.wait(lockWaitMessage);
        }
        mPrivateData->mWaitMessage = true;
    }
}

void OGLReceiver::notifyThread()
{
    /*unlock syncThread*/
    mPrivateData->mMutexWaitThread.lock();
    mPrivateData->mWaitThread = false;
    mPrivateData->mMutexWaitThread.unlock();
    mPrivateData->mConVarWaitThread.notify_one();
}

void OGLReceiver::createOGLSender(std::shared_ptr<OGLSender> &oglSender)
{
    /*create new*/
    oglSender = std::shared_ptr<OGLSender> (new OGLSender(mPrivateData));
}

