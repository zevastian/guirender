#include"Frame.h"
#include<algorithm>

Frame::Frame(std::shared_ptr<OGLLayer> oglLayer) :Widget(oglLayer)
{
    /*nothing*/
}

Frame::Frame(std::shared_ptr<Frame> parent) :Widget(parent)
{
    /*nothing*/
}

Frame::~Frame()
{
    /*nothing*/
}

void Frame::onMessage(Wmsg message)
{
    if (message.id == WMSG_SIZE) {
        Widget::onMessage(message);
        message.data.size.width = Widget::mWidth;
        message.data.size.height = Widget::mHeight;

        if (Widget::mBackground.enable) {
            Widget::mOglLayer->beginCommands();

            Widget::mOglLayer->glColor4f(Widget::mBackground.r, Widget::mBackground.g, Widget::mBackground.b, Widget::mBackground.a);
            Widget::mOglLayer->glBegin(GL_QUADS);
            Widget::mOglLayer->glVertex2f(Widget::mX, Widget::mY);
            Widget::mOglLayer->glVertex2f(Widget::mX, Widget::mY + Widget::mHeight);
            Widget::mOglLayer->glVertex2f(Widget::mX + Widget::mWidth, Widget::mY + Widget::mHeight);
            Widget::mOglLayer->glVertex2f(Widget::mX + Widget::mWidth, Widget::mY);
            Widget::mOglLayer->glEnd();

            Widget::mOglLayer->endCommands();
        }
    }

    /*notify child*/
    std::for_each(Widget::mChildrens.begin(), Widget::mChildrens.end(), [&message](std::shared_ptr<Widget> widget) {
        widget->onMessage(message);
    });
}
