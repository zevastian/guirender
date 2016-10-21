#include"OGLWindow.h"
#include<stdexcept>
#include<string.h>

OGLWindow::OGLWindow(Setting settings, GLXFBConfig fbc, Display* &display)
{
    /*save display handle*/
    mDisplay = display;
    /*init window*/
    createWindow(settings, fbc);
    registerEvents();
    loadIcon();
    settingWindow(settings);
    setting = settings;
}

OGLWindow::~OGLWindow()
{
    if (mWindow){
        XDestroyWindow(mDisplay, mWindow);
        XFreeColormap(mDisplay, mColormap);
    }

    if (mDisplay) {
        XCloseDisplay(mDisplay);
    }
}

void OGLWindow::createWindow(Setting setting, GLXFBConfig fbc)
{
    /*get visual for best fbc*/
    XVisualInfo *vi = glXGetVisualFromFBConfig(mDisplay, fbc);
    if (!vi) {
        XCloseDisplay(mDisplay);
        throw std::runtime_error("glXGetVisualFromFBConfig failed");
    }

    /*get default window*/
    Window rootWindow = RootWindow(mDisplay, vi->screen);

    XSetWindowAttributes wndAttr;
    mColormap = XCreateColormap(mDisplay, rootWindow, vi->visual, AllocNone);

    wndAttr.colormap = mColormap;
    wndAttr.background_pixmap = None;
    wndAttr.border_pixel = 0;
    wndAttr.event_mask = ExposureMask | StructureNotifyMask | FocusChangeMask | KeyPressMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask | EnterWindowMask | LeaveWindowMask;
    wndAttr.backing_store = Always;

    /*mask window event*/
    unsigned long eventMask = CWBorderPixel | CWBackingStore | CWBackPixmap | CWColormap | CWEventMask;

    XColor color;
    if (setting.background.enable) {
        color.red = (long)setting.background.color.r << 8;
        color.green = (long)setting.background.color.g << 8;
        color.blue = (long)setting.background.color.b << 8;

        XAllocColor(mDisplay, mColormap, &color);
        wndAttr.background_pixel = color.pixel;
        eventMask |= CWBackPixel;
    }

    /*create window*/
    mWindow = XCreateWindow(mDisplay, rootWindow, 0, 0, setting.width, setting.height, 0, vi->depth, InputOutput, vi->visual, eventMask, &wndAttr);
    XFree(vi);

    if (setting.background.enable){
        XFreeColors(mDisplay, mColormap, &color.pixel, 1, 0);
    }

    if (!mWindow) {
        XFreeColormap(mDisplay, mColormap);
        XCloseDisplay(mDisplay);
        throw std::runtime_error("XCreateWindow failed");
    }
}

void OGLWindow::registerEvents()
{
    mCloseWindow = XInternAtom(mDisplay, "WM_DELETE_WINDOW", false);
    mProtocol = XInternAtom(mDisplay, "WM_PROTOCOLS", true);
    mNewState = XInternAtom(mDisplay, "_NET_WM_STATE", false);
    mNewMaximizeHor = XInternAtom(mDisplay, "_NET_WM_STATE_MAXIMIZED_HORZ", false);
    mNewMaximizeVer = XInternAtom(mDisplay, "_NET_WM_STATE_MAXIMIZED_VERT", false);
    /*for to get window close message*/
    XSetWMProtocols(mDisplay, mWindow, &mCloseWindow, 1);
}

void OGLWindow::loadIcon()
{
//    std::ifstream stream(setting.icon, std::ios::in | std::ios::binary);
//    if (!stream.fail()) {
//        std::vector<char> buffer((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
//
//        XVisualInfo *vi = glXGetVisualFromFBConfig(mDisplay, fbc);
//        if (!vi) {
//            throw std::runtime_error("glXGetVisualFromFBConfig failed");
//        }
//
//        XpmAttributes attr;
//        memset(&attr, 0, sizeof(attr));
//        attr.visual = vi->visual;
//        attr.colormap = mColormap;
//        attr.depth = vi->depth;
//        attr.valuemask = XpmDepth | XpmColormap | XpmVisual;
//
//        char *data = buffer.data();
//        Pixmap pixmap;
//        Pixmap mask;
//        if (XpmCreatePixmapFromData(mDisplay, mWindow, &data, &pixmap, &mask, &attr) == XpmSuccess) {
//
//            XWMHints* windowHints = XAllocWMHints();
//            if (!windowHints) {
//                XFreePixmap(mDisplay, pixmap);
//                throw std::runtime_error("XAllocWMHints failed");
//            }
//
//            windowHints->flags = IconPixmapHint | IconMaskHint;
//            windowHints->icon_pixmap = pixmap;
//            windowHints->icon_mask = mask;
//            XSetWMHints(mDisplay, mWindow, windowHints);
//
//            XFree(windowHints);
//            //XFreePixmap(mDisplay, pixmap);
//            //XFreePixmap(mDisplay, mask);
//        }
//    }
}

void OGLWindow::settingWindow(Setting setting)
{
    /*set minimun and maximun dimension*/
    XSizeHints* sizeHints = XAllocSizeHints();
    if (!sizeHints) {
        throw std::runtime_error("XAllocSizeHints failed");
    }

    sizeHints->flags = PMaxSize | PMinSize;
    sizeHints->min_width = setting.minWidth;
    sizeHints->min_height = setting.minHeight;
    sizeHints->max_width = setting.maxWidth;
    sizeHints->max_height = setting.maxHeight;
    XSetWMNormalHints(mDisplay, mWindow, sizeHints);
    XFree(sizeHints);

    /*set title*/
    XStoreName(mDisplay, mWindow, setting.title.c_str());
}

bool OGLWindow::pendingMessage()
{
    return (bool)XPending(mDisplay);
}

void OGLWindow::show()
{
    Display *display = XOpenDisplay(NULL);
    if (!display) {
        return;
    }

    XMapWindow(display, mWindow);
    XMoveWindow(display, mWindow, setting.x, setting.y);
    XSync(display, true);

    XCloseDisplay(display);
}

void OGLWindow::maximize()
{

}

void OGLWindow::minimize()
{

}

void OGLWindow::close()
{
    Display *display = XOpenDisplay(NULL);
    if (!display) {
        return;
    }

    XEvent event;
    memset(&event, 0, sizeof(event));
    event.type = ClientMessage;
    event.xclient.window = mWindow;
    event.xclient.message_type = mProtocol;
    event.xclient.format = 32;
    event.xclient.data.l[0] = mCloseWindow;
    event.xclient.data.l[1] = CurrentTime;

    XSendEvent(display, mWindow, false, NoEventMask, &event);
    XSync(display, true);

    XCloseDisplay(display);
}

void OGLWindow::getWindow(Window &window)
{
    window = mWindow;
}

void OGLWindow::getDisplay(Display* &display)
{
    display = mDisplay;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void OGLWindow::getMessage(Wmsg &msg)
{
    XEvent event;
    XNextEvent(mDisplay, &event);

    switch (event.type) {
    case Expose:
        if (!event.xexpose.count) {
            msg.id = WMSG_EXPOSE;
        }
        break;

    case ConfigureNotify:
        msg.id = WMSG_SIZE;
        msg.data.size.width = event.xconfigure.width;
        msg.data.size.height = event.xconfigure.height;
        break;

    case MotionNotify:
        msg.id = WMSG_MOUSEMOVE;
        msg.data.mouseMove.x = event.xmotion.x;
        msg.data.mouseMove.y = event.xmotion.y;
        break;

    case EnterNotify:
        msg.id = WMSG_MOUSEENTER;
        break;

    case LeaveNotify:
        msg.id = WMSG_MOUSELEAVE;
        break;

    case KeyPress:
        //TODO
        msg.id = WMSG_KEYPRESS;
        msg.data.keyPress.key = event.xkey.keycode;
        break;

    case ButtonPress:
    case ButtonRelease:
        switch(event.xbutton.button) {
        case 1:
            //TODO: double click
            msg.id = WMSG_MOUSECLICK;
            msg.data.mouseClick.x = event.xbutton.x;
            msg.data.mouseClick.y = event.xbutton.y;
            msg.data.mouseClick.doubleClick = false;
            msg.data.mouseClick.press = event.xbutton.type == ButtonPress;
            break;

        case 4:
        case 5:
            msg.id = WMSG_MOUSEWHEEL;
            msg.data.mouseWheel.delta = event.xbutton.button == 4 ? 1 : -1;
            break;
        }
        break;

    case FocusIn:
        msg.id = WMSG_SETFOCUS;
        break;

    case FocusOut:
        msg.id = WMSG_KILLFOCUS;
        break;

    case ClientMessage:
        if (event.xclient.data.l[0] == mCloseWindow) {
            msg.id = WMSG_CLOSEWINDOW;
        }
        break;

    default:
        msg.id = WMSG_NONE;
    }
}

