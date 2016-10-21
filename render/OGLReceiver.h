#include"OGLCommands.h"
#include"OGLSender.h"
#include"OGLReceiverPrivate.h"
#include<memory>

#ifndef _OGLRECEIVER_H_
#define _OGLRECEIVER_H_

class OGLReceiver
{
private:
    /*shared data*/
    std::shared_ptr<OGLReceiverPrivateData> mPrivateData;

public:
    /*create opengl commands receiver*/
    OGLReceiver();
    /*destroy*/
    ~OGLReceiver();

    /**/
    void getMessage(GLmsg &message);
    /**/
    void notifyThread();

    /**/
    void createOGLSender(std::shared_ptr<OGLSender> &oglSender);
};

#endif
