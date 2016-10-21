#ifndef _COMMON_H_
#define _COMMON_H_

namespace common
{
unsigned int desktopWidth();
unsigned int desktopHeight();
unsigned int screenWidth();
unsigned int screenHeight();

bool inToRectangle(int x, int y, int xRectangle, int yRectangle, int widthRectangle, int heightRectangle);
bool inToCircle(int x, int y, int xCenter, int yCenter, int radius);
}

#endif
