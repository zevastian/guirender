#include"Utils.h"
#include<X11/Xlib.h>
#include<stdexcept>

namespace common
{

Display *openDisplay()
{
    Display *display = XOpenDisplay(NULL);
    if (!display) {
        throw std::runtime_error("cant't open x11 display");
    }
    return display;
}

unsigned int desktopWidth()
{
    Display *display = openDisplay();
    int width = DefaultScreenOfDisplay(display)->width;
    XCloseDisplay(display);
    return width;
}

unsigned int desktopHeight()
{
    Display *display = openDisplay();
    int height = DefaultScreenOfDisplay(display)->height;
    XCloseDisplay(display);
    return height;
}

unsigned int screenWidth()
{
    Display *display = openDisplay();
    int width = DefaultScreenOfDisplay(display)->width;
    XCloseDisplay(display);
    return width;
}

unsigned int screenHeight()
{
    Display *display = openDisplay();
    int height = DefaultScreenOfDisplay(display)->height;
    XCloseDisplay(display);
    return height;
}

bool inToRectangle(int x, int y, int xRectangle, int yRectangle, int widthRectangle, int heightRectangle)
{
    return x > xRectangle &&
           x < xRectangle + widthRectangle &&
           y > yRectangle &&
           y < yRectangle + heightRectangle;
}

bool inToCircle(int x, int y, int xCenter, int yCenter, int radius)
{
    /*(x-x0)*(x-x0) + (y-yo)*(y-yo) <= r*r*/
    int cx = x - xCenter;
    int cy = y - yCenter;
    return cx*cx + cy*cy <= radius*radius;
}

}
