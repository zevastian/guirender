#include"Widget.h"
#include"Frame.h"

#ifndef _BUTTON_H_
#define _BUTTON_H_

#define PI 3.141592654f
#define PI_DIV_2 1.570796327f
#define PI_DIV_4 0.785398163f

/*type callback function*/
typedef void (*OnClickCallback)(void);

class Button : public Widget
{
private:
    /*member var*/
    int mRadius;
    int mHover;
    OnClickCallback mOnClick;

    /**/
    void drawButton();
    /**/
    void drawArc(int x, int y, float delta, float arc, int steps);

public:
    /*create button*/
    Button(std::shared_ptr<Frame> parent);
    /*destroy button*/
    ~Button();

    /*process message*/
    void onMessage(Wmsg message);
};

#endif
