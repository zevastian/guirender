#include"../render/OGLLayer.h"
#include"../render/OGLWindow.h"
#include<memory>
#include<vector>

#ifndef _WIDGET_H_
#define _WIDGET_H_

class Widget
{
protected:
    /*member vars*/
    std::shared_ptr<OGLLayer> mOglLayer;
    std::vector<std::shared_ptr<Widget>> mChildrens;
    /*properties*/
    int mWidth;
    int mHeight;
    int mX;
    int mY;
    struct {
        bool enable;
        float r;
        float g;
        float b;
        float a;
    } mBackground;

public:
    /*create parent widget*/
    Widget(std::shared_ptr<OGLLayer> oglLayer);
    /*create child widget*/
    Widget(std::shared_ptr<Widget> parent);
    /*destroy widget*/
    ~Widget();

    /*add children*/
    void addChildren(std::shared_ptr<Widget> children);
    /*get parent layer*/
    void getLayer(std::shared_ptr<OGLLayer> &oglLayer);

    /*window message proc*/
    void onMessage(Wmsg message);
};

#endif
