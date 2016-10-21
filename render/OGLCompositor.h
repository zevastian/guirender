#include"OGLWindow.h"
#include"OGLContext.h"
#include"OGLCommands.h"
#include"OGLReceiver.h"
#include"OGLSender.h"
#include"OGLLayer.h"
#include<memory>
#include<thread>
#include<condition_variable>
#include<mutex>
#include<queue>
#include<list>

#ifndef _OGLCOMPOSITOR_H_
#define _OGLCOMPOSITOR_H_

/*opengl function types*/
typedef void (*glGenFramebuffersProc)(GLsizei, GLuint*);

typedef void (*glDeleteFramebuffersProc)(GLsizei, const GLuint*);

typedef void (*glBindFramebufferProc)(GLenum, GLuint);

typedef void (*glFramebufferTexture2DProc)(GLenum, GLenum, GLenum, GLuint, GLint);

typedef GLenum (*glCheckFramebufferStatusProc)(GLenum);

/*type callback*/
typedef void (*OGLCallback)(Wmsg message);

/*flags calback*/
#define FLAG_EXPOSE 1
#define FLAG_SIZE 2
#define FLAG_MOUSEMOVE 4
#define FLAG_MOUSEENTER 8
#define FLAG_MOUSELEAVE 16
#define FLAG_KEYPRESS 32
#define FLAG_MOUSECLICK 64
#define FLAG_MOUSEWHEEL 128
#define FLAG_KILLFOCUS 256
#define FLAG_SETFOCUS 512
#define FLAG_CLOSEWINDOW 1024

/*data callback*/
struct OGLDataCallback {
    void *userData;
    OGLCallback callback;
    int flags;
};

/*main class*/
class OGLCompositor
{
private:
    /*member var*/
    std::unique_ptr<OGLWindow> mOglWindow;
    std::unique_ptr<OGLContext> mOglContext;
    std::shared_ptr<OGLSender> mOglSender;
    OGLReceiver mOglReceiver;
    std::thread mThreadOpenglHandle;
    std::thread mThreadWindowHandle;
    std::condition_variable mConVarWaitMake;
    std::mutex mMutexWaitMake;
    int mCodeWaitMake;
    std::condition_variable mConVarWaitShow;
    std::mutex mMutexWaitShow;
    bool mWaitShow;
    std::mutex mMutexX11;
    std::condition_variable mConVarWaitExitWindow;
    std::mutex mMutexWaitExitWindow;
    bool mWaitExitWindow;
    std::list<OGLDataCallback> mListCallback;
    unsigned int layerCount;
    std::vector<std::queue<GLenum>> mLayerError;

    /*process window message*/
    static void threadWindow(OGLCompositor *self);
    /*process opengl commands*/
    static void threadOpengl(OGLCompositor *self);

public:
    /*init compositor*/
    OGLCompositor(Setting setting);
    /*destroy compositor*/
    ~OGLCompositor();

    /**/
    void showWindow();
    /**/
    void maximizeWindow();
    /**/
    void minimizeWindow();
    /**/
    void closeWindow();
    /**/
    void waitClose();

    /**/
    void addCallback(int flags, void *userData, OGLCallback oglCallback);
    /**/
    void removeCallback(OGLCallback oglCallback);

    /**/
    void createOGLLayer(std::shared_ptr<OGLLayer> &oglLayer);
};

#endif
