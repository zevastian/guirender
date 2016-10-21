#include"Scrollbar.h"
#include"../render/Utils.h"

Scrollbar::Scrollbar(std::shared_ptr<Frame> parent) :Widget(parent)
{
    /*nothings*/
}

Scrollbar::~Scrollbar()
{
    /*nothings*/
}

void Scrollbar::drawScrollBar()
{
    Widget::mOglLayer->beginCommands();

    Widget::mOglLayer->glBegin(GL_QUADS);
    /*background*/
    Widget::mOglLayer->glVertex2f(Widget::mX, Widget::mY);
    Widget::mOglLayer->glVertex2f(Widget::mX + Widget::mWidth, Widget::mY);
    Widget::mOglLayer->glVertex2f(Widget::mX + Widget::mWidth, Widget::mY + Widget::mHeight);
    Widget::mOglLayer->glVertex2f(Widget::mX, Widget::mY + Widget::mHeight);
    /*bar*/
    if (!mDisable) {
        Widget::mOglLayer->glVertex2f(Widget::mX, Widget::mY + mBarY);
        Widget::mOglLayer->glVertex2f(Widget::mX + Widget::mWidth, Widget::mY + mBarY);
        Widget::mOglLayer->glVertex2f(Widget::mX + Widget::mWidth, Widget::mY + mBarY + mBarHeight);
        Widget::mOglLayer->glVertex2f(Widget::mX, Widget::mY + mBarY + mBarHeight);
    }
    Widget::mOglLayer->glEnd();

    Widget::mOglLayer->endCommands();
}

float Scrollbar::adjustY(float newY)
{
    if (newY < 0.0f) {
        return 0.0f;
    } else if (newY > Widget::mHeight - mBarHeight) {
        return Widget::mHeight - mBarHeight;
    } else {
        return newY;
    }
}

void Scrollbar::onMessage(Wmsg message)
{
    switch (message.id) {
    case WMSG_MOUSEMOVE:
        if (!mDisable) {
            if (mBarPress) {
                float newY = adjustY(mBarY + message.data.mouseMove.y - mYLastMouse);
                if (mBarY != newY) {
                    mBarY = newY;
                    mYLastMouse = message.data.mouseMove.y;
                    mOnChangeOffset(((mMaxValue - Widget::mHeight)*mBarY)/(Widget::mHeight - mBarHeight));
                    drawScrollBar();
                }
            }
            if (common::inToRectangle(message.data.mouseMove.x, message.data.mouseMove.y, Widget::mX - 1.5f*Widget::mWidth, Widget::mY + mBarY, 3.0f*Widget::mWidth, mBarHeight)) {
                if (!mBarHover) {
                    mBarHover = true;
                    if (!mBarPress) {
                        drawScrollBar();
                    }
                }
            } else {
                if (mBarHover) {
                    mBarHover = false;
                    if (!mBarPress) {
                        drawScrollBar();
                    }
                }
            }
        }
        break;

    case WMSG_MOUSECLICK:
        if (!mDisable) {
            if (message.data.mouseClick.press) {
                if (common::inToRectangle(message.data.mouseClick.x, message.data.mouseClick.y, Widget::mX -1.5f*Widget::mWidth, Widget::mY, 3.0f*Widget::mWidth, Widget::mHeight)) {

                    mYLastMouse = message.data.mouseClick.y;
                    mBarPress = true;
                    if (!common::inToRectangle(message.data.mouseClick.x, message.data.mouseClick.y, Widget::mX - 1.5f*Widget::mWidth, Widget::mY + mBarY, 3.0f*Widget::mWidth, mBarHeight)) {
                        float newY = adjustY(message.data.mouseClick.y - Widget::mY - 0.5f*mBarHeight);
                        if (mBarY != newY) {
                            mBarY = newY;
                            mBarHover = true;
                            mOnChangeOffset(((mMaxValue - Widget::mHeight)*mBarY)/(Widget::mHeight - mBarHeight));
                            drawScrollBar();
                        }
                    }
                }
            } else {
                if (mBarPress) {
                    mBarPress = false;
                    if (!mBarHover) {
                        drawScrollBar();
                    }
                }
            }
        }
        break;

    case WMSG_MOUSEWHEEL:
        if (!mBarPress && !mDisable) {
            float newY = adjustY(mBarY - (message.data.mouseWheel.delta*SCROLL_ADVANCE*(Widget::mHeight - mBarHeight))/mMaxValue);
            if (mBarY != newY) {
                mBarY = newY;
                mOnChangeOffset(((mMaxValue - Widget::mHeight)*mBarY)/(Widget::mHeight - mBarHeight));
                drawScrollBar();
            }
        }
        break;

    case WMSG_MOUSELEAVE:
        if (mBarHover && !mBarPress) {
            mBarHover = false;
            drawScrollBar();
        }
        break;

    case WMSG_KILLFOCUS:
        if (mBarPress || mBarHover) {
            mBarPress = false;
            mBarHover = false;
            drawScrollBar();
        }
        break;

    case WMSG_KEYPRESS:
        break;

    case WMSG_SIZE: {
        float offset = Widget::mHeight != mBarHeight ? ((mMaxValue - Widget::mHeight)*mBarY)/(Widget::mHeight - mBarHeight) : 0.0f;

        Widget::onMessage(message);
        /*update bar state*/
        mBarHeight = (Widget::mHeight*Widget::mHeight)/mMaxValue;
        mBarPress = false;
        mBarHover = false;

        if (mBarHeight >= Widget::mHeight) {
            mBarHeight = Widget::mHeight;
            mDisable = true;
            drawScrollBar();
            mOnChangeOffset(0.0f);
        } else {
            if (mDisable) {
                offset = 0.0f;
            }
            if (mMaxValue - offset < Widget::mHeight) {
                offset = mMaxValue - Widget::mHeight;
            }
            mBarY = adjustY((offset*(Widget::mHeight - mBarHeight))/(mMaxValue - Widget::mHeight));
            mDisable = false;
            drawScrollBar();
            mOnChangeOffset(((mMaxValue - Widget::mHeight)*mBarY)/(Widget::mHeight - mBarHeight));
        }
        break;
    }
    }
}
