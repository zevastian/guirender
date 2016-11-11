#include"Widget.h"
#include"Frame.h"

#ifndef _SCROLLBAR_H_
#define _SCROLLBAR_H_
#define SCROLL_ADVANCE 64

typedef void (*OnChangeOffset)(float);

class Scrollbar :public Widget
{
private:
    /*member var*/
    float mBarY;
    float mBarHeight;
    int mYLastMouse;
    bool mBarHover;
    bool mBarPress;
    float mMaxValue;
    bool mDisable;
    OnChangeOffset mOnChangeOffset;

    /**/
    void drawScrollBar();

public:
    /*create scrollbar*/
    Scrollbar(std::shared_ptr<Frame> parent);
    /*destroy scrollbar*/
    ~Scrollbar();

    /*process message*/
    void onMessage(Wmsg message);
};

#endif
