#include"OGLWindow.h"
#include<memory>

#ifndef _OGLCONTEXT_H_
#define _OGLCONTEXT_H_

class OGLContext
{
private:
    /*member var*/
    GLXContext mContext;
    Display *mDisplay;
    Window mWindow;

    /*init opengl context*/
    void createContext(GLXFBConfig &fbc);
    /*check if available extension*/
    void checkExtension();

public:
    /*create context*/
    OGLContext(std::unique_ptr<OGLWindow> &oglWindow, GLXFBConfig &fbc, Display* &display);
    /*destroy context*/
    ~OGLContext();

    /*get best fbconfig*/
    static void chooseVisual(GLXFBConfig &bfbc, Display* &display);
    /*get display conection*/
    static void openConection(Display* &display);
    /*make opengl context*/
    bool makeContext();

    /*get native gl context*/
    void getContext(GLXContext &context);
};

#endif
