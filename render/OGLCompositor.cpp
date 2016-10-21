#include"OGLCompositor.h"
#include<new>
#include<algorithm>

OGLCompositor::OGLCompositor(Setting setting) :mCodeWaitMake(0), layerCount(0), mWaitShow(false), mWaitExitWindow(false)
{
    /*get best fbc*/
    Display *display;
    OGLContext::openConection(display);
    GLXFBConfig fbc;
    OGLContext::chooseVisual(fbc, display);

    /*init message sender*/
    mOglReceiver.createOGLSender(mOglSender);
    /*init window*/
    mOglWindow = std::unique_ptr<OGLWindow> (new OGLWindow(setting, fbc, display));
    /*init opengl context*/
    mOglContext = std::unique_ptr<OGLContext> (new OGLContext(mOglWindow, fbc, display));

    /*create opengl thread*/
    mThreadOpenglHandle = std::thread(threadOpengl, this);

    /*wait makecurrent*/
    std::unique_lock<std::mutex> lockWaitMake(mMutexWaitMake);
    while (!mCodeWaitMake) {
        mConVarWaitMake.wait(lockWaitMake);
    }

    /*check error*/
    if (mCodeWaitMake == -1) {
        std::runtime_error("make opengl context failed");
    }

    /*create window thread*/
    mThreadWindowHandle = std::thread(threadWindow, this);
}

OGLCompositor::~OGLCompositor()
{
    /*close thread*/
    closeWindow();

    /*wait opengl thread*/
    if (mThreadOpenglHandle.joinable()) {
        mThreadOpenglHandle.join();
    }
    /*wait window thread*/
    if (mThreadWindowHandle.joinable()) {
        mThreadWindowHandle.join();
    }
}

void OGLCompositor::threadWindow(OGLCompositor *self)
{
    /*wait show*/
    std::unique_lock<std::mutex> lockWaitShow(self->mMutexWaitShow);
    while (!self->mWaitShow) {
        self->mConVarWaitShow.wait(lockWaitShow);
    }

    bool redraw = false;
    Wmsg winMessage;
    GLmsg glMessage;
    int windowWidth = -1;
    int windowHeight = -1;

    while (true) {

        self->mOglWindow->getMessage(winMessage);
        switch (winMessage.id) {
        case WMSG_EXPOSE:
            redraw = true;
            break;

        case WMSG_SIZE:
            /*check size change*/
            if (winMessage.data.size.width > 0 && winMessage.data.size.height > 0 && (windowWidth != winMessage.data.size.width || windowHeight != winMessage.data.size.height)) {
                /*lock mutex*/
                self->mOglSender->begin();
                /*glViewport*/
                glMessage.id = GLMSG_VIEWPORT;
                glMessage.data.viewport.width = winMessage.data.size.width;
                glMessage.data.viewport.height = winMessage.data.size.height;
                self->mOglSender->pushMessage(glMessage);
                /*glMatrixMode*/
                glMessage.id = GLMSG_MATRIXMODE;
                glMessage.data.matrixMode.mode = GL_PROJECTION;
                self->mOglSender->pushMessage(glMessage);
                /*glLoadIdentity*/
                glMessage.id = GLMSG_LOADIDENTITY;
                self->mOglSender->pushMessage(glMessage);
                /*glOrtho*/
                glMessage.id = GLMSG_ORTHO;
                glMessage.data.ortho.left = 0.0f;
                glMessage.data.ortho.right = winMessage.data.size.width;
                glMessage.data.ortho.bottom = winMessage.data.size.height;
                glMessage.data.ortho.top = 0.0f;
                glMessage.data.ortho.near_val = 0.0f;
                glMessage.data.ortho.far_val = 1.0f;
                self->mOglSender->pushMessage(glMessage);
                /*unlock mutex*/
                self->mOglSender->end();
                /*wait update viewport*/
                self->mOglSender->syncThread();
                /*update*/
                windowWidth = winMessage.data.size.width;
                windowHeight = winMessage.data.size.height;

                /*call callback*/
                for (auto elem = self->mListCallback.begin(); elem != self->mListCallback.end(); elem++) {
                    if (elem->flags & FLAG_SIZE) {
                        elem->callback(winMessage);
                    }
                }
            }
            break;

        case WMSG_MOUSEMOVE:
            for (auto elem = self->mListCallback.begin(); elem != self->mListCallback.end(); elem++) {
                if (elem->flags & FLAG_MOUSEMOVE) {
                    elem->callback(winMessage);
                }
            }
            break;

        case WMSG_MOUSEENTER:
            for (auto elem = self->mListCallback.begin(); elem != self->mListCallback.end(); elem++) {
                if (elem->flags & FLAG_MOUSEENTER) {
                    elem->callback(winMessage);
                }
            }
            break;

        case WMSG_MOUSELEAVE:
            for (auto elem = self->mListCallback.begin(); elem != self->mListCallback.end(); elem++) {
                if (elem->flags & FLAG_MOUSELEAVE) {
                    elem->callback(winMessage);
                }
            }
            break;

        case WMSG_KEYPRESS:
            for (auto elem = self->mListCallback.begin(); elem != self->mListCallback.end(); elem++) {
                if (elem->flags & FLAG_KEYPRESS) {
                    elem->callback(winMessage);
                }
            }
            break;

        case WMSG_MOUSECLICK:
            for (auto elem = self->mListCallback.begin(); elem != self->mListCallback.end(); elem++) {
                if (elem->flags & FLAG_MOUSECLICK) {
                    elem->callback(winMessage);
                }
            }
            break;

        case WMSG_MOUSEWHEEL:
            for (auto elem = self->mListCallback.begin(); elem != self->mListCallback.end(); elem++) {
                if (elem->flags & FLAG_MOUSEWHEEL) {
                    elem->callback(winMessage);
                }
            }
            break;

        case WMSG_KILLFOCUS:
            for (auto elem = self->mListCallback.begin(); elem != self->mListCallback.end(); elem++) {
                if (elem->flags & FLAG_KILLFOCUS) {
                    elem->callback(winMessage);
                }
            }
            break;

        case WMSG_SETFOCUS:
            for (auto elem = self->mListCallback.begin(); elem != self->mListCallback.end(); elem++) {
                if (elem->flags & FLAG_SETFOCUS) {
                    elem->callback(winMessage);
                }
            }
            break;

        case WMSG_CLOSEWINDOW:
            glMessage.id = GLMSG_CLOSE;
            self->mOglSender->postMessage(glMessage);
            self->mOglSender->syncThread();

            self->mMutexWaitExitWindow.lock();
            self->mWaitExitWindow = true;
            self->mMutexWaitExitWindow.unlock();
            self->mConVarWaitExitWindow.notify_all();
            return;
        }

        if (redraw) {
            if (!self->mOglWindow->pendingMessage()) {
                glMessage.id = GLMSG_SWAPBUFFERS;
                self->mOglSender->postMessage(glMessage);
                self->mOglSender->syncThread();

                redraw = false;
            }
        }
    }
}

void OGLCompositor::threadOpengl(OGLCompositor *self)
{
    bool throwException = false;
    if (!self->mOglContext->makeContext()) {
        /*throw exception in constructor*/
        throwException = true;
    }

    /*notify thread*/
    self->mMutexWaitMake.lock();
    self->mCodeWaitMake = throwException ? -1 : 1;
    self->mMutexWaitMake.unlock();
    self->mConVarWaitMake.notify_one();

    Display *display;
    self->mOglWindow->getDisplay(display);
    Window window;
    self->mOglWindow->getWindow(window);

    unsigned int idLayer = 0;
    bool noCheckError = false;
    GLenum error;
    GLmsg message;
    while (true) {

        self->mOglReceiver.getMessage(message);
        switch (message.id) {
        case GLMSG_TEXCOORD2F:
            glTexCoord2f(message.data.texCoord2f.s, message.data.texCoord2f.t);
            break;

        case GLMSG_VERTEX2F:
            glVertex2f(message.data.vertex2f.x, message.data.vertex2f.y);
            break;

        case GLMSG_COLOR3F:
            glColor3f(message.data.color3f.red, message.data.color3f.green, message.data.color3f.blue);
            break;

        case GLMSG_COLOR4F:
            glColor4f(message.data.color4f.red, message.data.color4f.green, message.data.color4f.blue, message.data.color4f.alpha);
            break;

        case GLMSG_BEGIN:
            glBegin(message.data.begin.mode);
            noCheckError = true;
            break;

        case GLMSG_END:
            glEnd();
            noCheckError = false;
            break;

        case GLMSG_BINDTEXTURE:
            glBindTexture(message.data.bindTexture.target, message.data.bindTexture.texture);
            break;

        case GLMSG_ENABLE:
            glEnable(message.data.enable.cap);
            break;

        case GLMSG_INITCOMMANDS:
            idLayer = message.data.initCommnads.idLayer;
            break;

        case GLMSG_FINISHCOMMANDS:
            break;

        case GLMSG_GETERROR:
            /*get error*/
            if (!self->mLayerError[idLayer].empty()) {
                *message.data.getError.error = self->mLayerError[idLayer].front();
                self->mLayerError[idLayer].pop();
            } else {
                *message.data.getError.error = GL_NO_ERROR;
            }
            /*notify layer*/
            message.sync.mutexSync->lock();
            *message.sync.waitSync = false;
            message.sync.mutexSync->unlock();
            message.sync.conVarSync->notify_one();
            break;

        case GLMSG_CLEARCOLOR:
            glClearColor(message.data.clearColor.red, message.data.clearColor.green, message.data.clearColor.blue, message.data.clearColor.alpha);
            break;

        case GLMSG_CLEAR:
            glClear(message.data.clear.mask);
            break;

        case GLMSG_TEXSUBIMAGE2D:
            glTexSubImage2D(message.data.texSubImage2D.target, message.data.texSubImage2D.level, message.data.texSubImage2D.xoffset, message.data.texSubImage2D.yoffset, message.data.texSubImage2D.width, message.data.texSubImage2D.height, message.data.texSubImage2D.format, message.data.texSubImage2D.type, message.data.texSubImage2D.pixels);
            break;

        case GLMSG_VIEWPORT:
            glViewport(0, 0, message.data.viewport.width, message.data.viewport.height);
            self->mOglReceiver.notifyThread();
            break;

        case GLMSG_MATRIXMODE:
            glMatrixMode(message.data.matrixMode.mode);
            break;

        case GLMSG_LOADIDENTITY:
            glLoadIdentity();
            break;

        case GLMSG_ORTHO:
            glOrtho(message.data.ortho.left, message.data.ortho.right, message.data.ortho.bottom, message.data.ortho.top, message.data.ortho.near_val, message.data.ortho.far_val);
            break;

        case GLMSG_SWAPBUFFERS:
            glXSwapBuffers(display, window);
            self->mOglReceiver.notifyThread();
            break;

        case GLMSG_TEXPARAMETERI:
            glTexParameteri(message.data.texParameteri.target, message.data.texParameteri.pname, message.data.texParameteri.param);
            break;

        case GLMSG_TEXIMAGE2D:
            glTexImage2D(message.data.texImage2D.target, message.data.texImage2D.level, message.data.texImage2D.internalformat, message.data.texImage2D.width, message.data.texImage2D.height, message.data.texImage2D.border, message.data.texImage2D.format, message.data.texImage2D.type, message.data.texImage2D.pixels);
            break;

        case GLMSG_GENTEXTURES:
            glGenTextures(message.data.genTextures.n, *message.data.genTextures.textures);
            /*notify layer*/
            message.sync.mutexSync->lock();
            *message.sync.waitSync = false;
            message.sync.mutexSync->unlock();
            message.sync.conVarSync->notify_one();
            break;

        case GLMSG_DELETETEXTURES:
            glDeleteTextures(message.data.deleteTextures.n, message.data.deleteTextures.textures);
            break;

        case GLMSG_CLOSE:
            self->mOglReceiver.notifyThread();
            return;
        }

        if (idLayer && !noCheckError) {
            error = glGetError();
            if (error != GL_NO_ERROR) {
                /*remove layer commands*/
                while (message.id != GLMSG_FINISHCOMMANDS) {
                    self->mOglReceiver.getMessage(message);
                }
                /*save error*/
                self->mLayerError[idLayer].push(error);
            }
            if (message.id == GLMSG_FINISHCOMMANDS) {
                idLayer = 0;
            }
        }
    }
}

void OGLCompositor::showWindow()
{
    if (!mWaitShow) {
        mOglWindow->show();

        /*unlock window thread*/
        mMutexWaitShow.lock();
        mWaitShow = true;
        mMutexWaitShow.unlock();
        mConVarWaitShow.notify_one();
    }
}

void OGLCompositor::maximizeWindow()
{
    mMutexX11.lock();
    mOglWindow->maximize();
    mMutexX11.unlock();
}

void OGLCompositor::minimizeWindow()
{
    mMutexX11.lock();
    mOglWindow->minimize();
    mMutexX11.unlock();
}

void OGLCompositor::closeWindow()
{
    mMutexX11.lock();
    mOglWindow->close();
    mMutexX11.unlock();
}

void OGLCompositor::waitClose()
{
    /*wait exit window thread*/
    std::unique_lock<std::mutex> lockWaitExit(mMutexWaitExitWindow);
    while (!mWaitExitWindow) {
        mConVarWaitExitWindow.wait(lockWaitExit);
    }
}

void OGLCompositor::addCallback(int flags, void *userData, OGLCallback oglCallback)
{
    OGLDataCallback data;
    data.callback = oglCallback;
    data.flags = flags;
    data.userData = userData;
    /*add*/
    mListCallback.push_back(data);
}

void OGLCompositor::removeCallback(OGLCallback oglCallback)
{
    /*delete from function pointer*/
    std::remove_if(mListCallback.begin(), mListCallback.end(), [&oglCallback](OGLDataCallback data) {
        return data.callback == oglCallback;
    });
}

void OGLCompositor::createOGLLayer(std::shared_ptr<OGLLayer> &oglLayer)
{
    /*create sender*/
    std::shared_ptr<OGLSender> oglSender;
    mOglReceiver.createOGLSender(oglSender);
    /*create layer*/
    mLayerError.resize(layerCount + 2);
    oglLayer = std::shared_ptr<OGLLayer> (new OGLLayer(++layerCount, oglSender));
}
