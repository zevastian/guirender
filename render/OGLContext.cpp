#include"OGLContext.h"
#include<stdexcept>
#include<iostream>
#include<string.h>

OGLContext::OGLContext(std::unique_ptr<OGLWindow> &oglWindow, GLXFBConfig &fbc, Display* &display) : mContext(NULL)
{
    /*save display handle*/
    mDisplay = display;
    /*save window handle*/
    oglWindow->getWindow(mWindow);
    /*init context*/
    createContext(fbc);
    /*check extension*/
    checkExtension();
}

OGLContext::~OGLContext()
{
    if (mContext) {
        /*destroy*/
        glXMakeCurrent(mDisplay, 0, 0);
        glXDestroyContext(mDisplay, mContext);
    }
}

void OGLContext::createContext(GLXFBConfig &fbc)
{
    /*get string extensions*/
    const char *glxExts = glXQueryExtensionsString(mDisplay, DefaultScreen(mDisplay));

    bool newStyle = true;
    if (!strstr(glxExts, "GLX_ARB_create_context")) {
         newStyle = false;
    }

    if (!strstr(glxExts, "GLX_ARB_create_context_profile")) {
        newStyle = false;
    }

    //FIXME
    typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);
    //FIXME
    glXCreateContextAttribsARBProc glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)glXGetProcAddressARB((const GLubyte*)"glXCreateContextAttribsARB");
    if (!glXCreateContextAttribsARB) {
        newStyle = false;
    }

    if (newStyle) {
        int contextAttr[] = {
            GLX_CONTEXT_MAJOR_VERSION_ARB, 1,
            GLX_CONTEXT_MINOR_VERSION_ARB, 1,
            GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
            GLX_RENDER_TYPE, GLX_RGBA_TYPE,
            None
        };

        mContext = glXCreateContextAttribsARB(mDisplay, fbc, 0, True, contextAttr);
        if (!mContext) {
            throw std::runtime_error("glXCreateContextAttribsARB failed");
        }
    }
    else {
        //VALGRIND
        mContext = glXCreateNewContext(mDisplay, fbc, GLX_RGBA_TYPE, 0, True);
        if (!mContext) {
            throw std::runtime_error("glXCreateNewContext failed");
        }
    }
}

void OGLContext::openConection(Display* &display)
{
    display = XOpenDisplay(NULL);
    if (!display) {
        throw std::runtime_error("cant open display");
    }
}

void OGLContext::checkExtension()
{
//    if (!glewIsSupported("GL_ARB_texture_non_power_of_two")) {
//        throw std::runtime_error("GL_ARB_texture_non_power_of_two not found");
//        glXDestroyContext(mDisplay, mContext);
//    }
//
//    if (!glewIsSupported("GL_ARB_vertex_buffer_object")) {
//        throw std::runtime_error("GL_ARB_vertex_buffer_object not found");
//        glXDestroyContext(mDisplay, mContext);
//    }
//
//    if (!glewIsSupported("GL_ARB_pixel_buffer_object")) {
//        throw std::runtime_error("GL_ARB_pixel_buffer_object not found");
//        glXDestroyContext(mDisplay, mContext);
//    }
//
//    if (!glewIsSupported("GL_EXT_framebuffer_object") && !glewIsSupported("GL_ARB_framebuffer_object")) {
//        throw std::runtime_error("GL_EXT_framebuffer_object/GL_EXT_framebuffer_object not found");
//        glXDestroyContext(mDisplay, mContext);
//    }
}

bool OGLContext::makeContext()
{
    //VALGRIND
    return (bool)glXMakeCurrent(mDisplay, mWindow, mContext);
}

void OGLContext::chooseVisual(GLXFBConfig &bfbc, Display* &display)
{
    int glxMajor;
    int glxMinor;
    if (!glXQueryVersion(display, &glxMajor, &glxMinor)){
        XCloseDisplay(display);
        throw std::runtime_error("glXQueryVersion failed");
    }

    if ((glxMajor == 1 && glxMinor < 3) || glxMajor < 1) {
        XCloseDisplay(display);
        throw std::runtime_error("glX 1.3 or higher version is required");
    }

    int attr[] = {
        GLX_X_RENDERABLE, True,
        GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
        GLX_RENDER_TYPE, GLX_RGBA_BIT,
        GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
        GLX_RED_SIZE, 8,
        GLX_GREEN_SIZE, 8,
        GLX_BLUE_SIZE, 8,
        GLX_ALPHA_SIZE, 8,
        GLX_DOUBLEBUFFER, True,
        None
    };

    int fbcount = 0;
    //VALGRIND
    GLXFBConfig *fbc = glXChooseFBConfig(display, DefaultScreen(display), attr, &fbcount);

    if (!fbc || !fbcount) {
        XCloseDisplay(display);
        throw std::runtime_error("glXChooseFBConfig failed");
    }

    int i;
    for (i = 0; i < fbcount; i++) {
        int caveat;

        glXGetFBConfigAttrib(display, fbc[i], GLX_CONFIG_CAVEAT, &caveat);
        if (caveat == GLX_NONE) {
            bfbc = fbc[i];
            XFree(fbc);
            return;
        }
    }

    std::cout << "Warning: possible bad fbconfig\n";
    bfbc = fbc[0];
    XFree(fbc);
    return;
}

void OGLContext::getContext(GLXContext &context)
{
    context = mContext;
}
