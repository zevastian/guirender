#include<GL/gl.h>
#include<GL/glx.h>
#include<climits>
#include<string>

#ifndef _OGLWINDOW_H_
#define _OGLWINDOW_H_

/*window description*/
struct Setting {
	std::string title;
	std::string icon;
	unsigned int width;
	unsigned int minWidth;
	unsigned int maxWidth;
	unsigned int height;
	unsigned int minHeight;
	unsigned int maxHeight;
	int x;
	int y;
	struct {
        struct {
            unsigned char r;
            unsigned char g;
            unsigned char b;
        } color;
        bool enable;
	} background;
	Setting() {
        background.enable = false;
        minWidth = 0;
        maxWidth = INT_MAX;
        minHeight = 0;
        maxHeight = INT_MAX;
	}
};

/*window message*/
enum WMSG_ENUM {
    WMSG_NONE,
    WMSG_EXPOSE,
    WMSG_SIZE,
    WMSG_MOUSEMOVE,
    WMSG_MOUSEENTER,
    WMSG_MOUSELEAVE,
    WMSG_KEYPRESS,
    WMSG_MOUSECLICK,
    WMSG_MOUSEWHEEL,
    WMSG_KILLFOCUS,
    WMSG_SETFOCUS,
    WMSG_CLOSEWINDOW
};

/*message struct*/
struct Wmsg {
    WMSG_ENUM id;
    union {
        struct {
            int width;
            int height;
        } size;
        struct {
            int x;
            int y;
        } mouseMove;
        struct {
            int key;
        } keyPress;
        struct {
            bool doubleClick;
            bool press;
            int x;
            int y;
        } mouseClick;
        struct {
            int delta;
        } mouseWheel;
    } data;
};

class OGLWindow
{
private:
    /*member var*/
    Display *mDisplay;
    Window mWindow;
    Colormap mColormap;
    Atom mNewState;
    Atom mNewMaximizeHor;
    Atom mNewMaximizeVer;
    Atom mCloseWindow;
    Atom mProtocol;
    Setting setting;

    /*init window*/
    void createWindow(Setting settings, GLXFBConfig fbc);
    /*register event*/
    void registerEvents();
    /*set icon*/
    void loadIcon();
    /**/
    void settingWindow(Setting setting);

public:
    /*create window*/
    OGLWindow(Setting settings, GLXFBConfig fbc, Display* &display);
    /*destroy window*/
    ~OGLWindow();

    /*get next window message*/
    void getMessage(Wmsg &msg);
    /*check if has message*/
    bool pendingMessage();

    /*show window*/
    void show();
    /*maximize window*/
    void maximize();
    /*minimze window*/
    void minimize();
    /*close window*/
    void close();

    /*get native window handle*/
    void getWindow(Window &window);
    /*get native display handle*/
    void getDisplay(Display* &display);
};

#endif
