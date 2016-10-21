#include"../render/Utils.h"
#include"Button.h"
#include<cmath>

Button::Button(std::shared_ptr<Frame> parent) :Widget(parent)
{
    /*nothing*/
}

Button::~Button()
{
    /*nothing*/
}

void Button::drawArc(int x, int y, float delta, float arc, int steps)
{
    Widget::mOglLayer->glBegin(GL_TRIANGLE_FAN);
    /*center vertex*/
    Widget::mOglLayer->glVertex2f(x, y);
    /*arc vertexs*/
    for (int i = 0; i <= steps; i++) {
        float angle = delta + arc*(i/(float)steps);
        float xArc = x + mRadius*cos(angle);
        float yArc = y - mRadius*sin(angle);
        Widget::mOglLayer->glVertex2f(xArc, yArc);
    }
    Widget::mOglLayer->glEnd();
}

void Button::drawButton()
{
    Widget::mOglLayer->beginCommands();

    Widget::mOglLayer->glBegin(GL_QUADS);
    /*draw center rectangle*/
    Widget::mOglLayer->glVertex2f(Widget::mX, Widget::mY + mRadius);
    Widget::mOglLayer->glVertex2f(Widget::mX + Widget::mWidth, Widget::mY + mRadius);
    Widget::mOglLayer->glVertex2f(Widget::mX + Widget::mWidth, Widget::mY + Widget::mHeight - mRadius);
    Widget::mOglLayer->glVertex2f(Widget::mX, Widget::mY + Widget::mHeight - mRadius);
    /*draw top rectangle*/
    Widget::mOglLayer->glVertex2f(Widget::mX + mRadius, Widget::mY);
    Widget::mOglLayer->glVertex2f(Widget::mX + Widget::mWidth - mRadius , Widget::mY);
    Widget::mOglLayer->glVertex2f(Widget::mX + Widget::mWidth - mRadius, Widget::mY + mRadius);
    Widget::mOglLayer->glVertex2f(Widget::mX + mRadius, Widget::mY + mRadius);
    /*draw bottom rectangle*/
    Widget::mOglLayer->glVertex2f(Widget::mX + mRadius, Widget::mY + Widget::mHeight - mRadius);
    Widget::mOglLayer->glVertex2f(Widget::mX + Widget::mWidth - mRadius, Widget::mY + Widget::mHeight - mRadius);
    Widget::mOglLayer->glVertex2f(Widget::mX + Widget::mWidth - mRadius, Widget::mY + Widget::mHeight);
    Widget::mOglLayer->glVertex2f(Widget::mX + mRadius, Widget::mY + Widget::mHeight);
    Widget::mOglLayer->glEnd();

    /*calcule arc vertex count*/
    /*std::ceil(2.0f*sqrt(radius)*(arc/PI)*/
    int steps = std::ceil(sqrt(mRadius));

    /*draw right top corner*/
    drawArc(Widget::mX + Widget::mWidth - mRadius, Widget::mY + mRadius, 0.0f, PI_DIV_2, steps);
    /*draw left top corner*/
    drawArc(Widget::mX + mRadius, Widget::mY + mRadius, PI_DIV_2, PI_DIV_2, steps);
    /*draw left bottom corner*/
    drawArc(Widget::mX + mRadius, Widget::mY + Widget::mHeight - mRadius, PI, PI_DIV_2, steps);
    /*draw right bottom corner*/
    drawArc(Widget::mX + Widget::mWidth - mRadius, Widget::mY + Widget::mHeight - mRadius, PI + PI_DIV_2, PI_DIV_2, steps);

    Widget::mOglLayer->endCommands();
}

void Button::onMessage(Wmsg message)
{
    switch (message.id) {
    case WMSG_MOUSEMOVE:
        /*check basic rectangle*/
        if (common::inToRectangle(message.data.mouseMove.x, message.data.mouseMove.y, Widget::mX, Widget::mY, Widget::mWidth, Widget::mHeight)) {
            /*in to center rectangle*/
            if (common::inToRectangle(message.data.mouseMove.x, message.data.mouseMove.y, Widget::mX, Widget::mY + mRadius, Widget::mWidth, Widget::mHeight - 2*mRadius) ||
                /*in to top rectangle*/
                common::inToRectangle(message.data.mouseMove.x, message.data.mouseMove.y, Widget::mX + mRadius, Widget::mY, Widget::mWidth - 2*mRadius, mRadius) ||
                /*in to buttom rectangle*/
                common::inToRectangle(message.data.mouseMove.x, message.data.mouseMove.y, Widget::mX + mRadius, Widget::mHeight - mRadius, Widget::mWidth - 2*mRadius, mRadius) ||
                /*in to left top corner*/
                common::inToCircle(message.data.mouseMove.x, message.data.mouseMove.y, Widget::mX + mRadius, Widget::mY + mRadius, mRadius) ||
                /*in to left bottom corner*/
                common::inToCircle(message.data.mouseMove.x, message.data.mouseMove.y, Widget::mX + mRadius, Widget::mHeight - mRadius, mRadius) ||
                /*in to right bottom corner*/
                common::inToCircle(message.data.mouseMove.x, message.data.mouseMove.y, Widget::mWidth - mRadius, Widget::mHeight - mRadius, mRadius) ||
                /*in to right top corner*/
                common::inToCircle(message.data.mouseMove.x, message.data.mouseMove.y, Widget::mWidth - mRadius, Widget::mY + mRadius, mRadius)) {

                if (!mHover) {
                    mHover = true;
                    drawButton();
                }
                break;
            }
        }

    case WMSG_MOUSELEAVE:
    case WMSG_KILLFOCUS:
        if (mHover) {
            mHover = false;
            drawButton();
        }
        break;

    case WMSG_MOUSECLICK:
        if (mHover) {
            mOnClick();
        }
        break;

    case WMSG_SIZE:
        Widget::onMessage(message);
        drawButton();
        break;
    }
}
