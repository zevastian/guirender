#include"OGLCommands.h"
#include"OGLReceiverPrivate.h"
#include<memory>

#ifndef _OGLSENDER_H_
#define _OGLSENDER_H_

class OGLSender
{
private:
    /*shared data*/
    std::shared_ptr<OGLReceiverPrivateData> mPrivateData;

public:
    /*create*/
    OGLSender(std::shared_ptr<OGLReceiverPrivateData> privateData);
    /*destroy*/
    ~OGLSender();

    /**/
    void postMessage(GLmsg message);
    /**/
    void syncThread();

    /**/
    void begin();
    /**/
    void pushMessage(GLmsg message);
    /**/
    void end();
};

#endif
