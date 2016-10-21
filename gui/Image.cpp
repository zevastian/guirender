#include"Image.h"

Image::Image(std::shared_ptr<Frame> parent) :Widget(parent)
{
    /*nothing*/
}

Image::~Image()
{
    /*nothing*/
}

void Image::drawImage(int x, int y, int width, int height, float uvLeft, float uvTop, float uvRight, float uvBottom)
{
    Widget::mOglLayer->beginCommands();

    Widget::mOglLayer->glEnable(GL_TEXTURE_2D);
    Widget::mOglLayer->glBindTexture(GL_TEXTURE_2D, mTexture);
    Widget::mOglLayer->glBegin(GL_QUADS);
    /*left-top*/
    Widget::mOglLayer->glTexCoord2f(uvLeft, uvTop);
    Widget::mOglLayer->glVertex2f(x, y);
    /*right-top*/
    Widget::mOglLayer->glTexCoord2f(uvRight, uvTop);
    Widget::mOglLayer->glVertex2f(x + width, y);
    /*rigth-buttom*/
    Widget::mOglLayer->glTexCoord2f(uvRight, uvBottom);
    Widget::mOglLayer->glVertex2f(x + width, y + height);
    /*left-buttom*/
    Widget::mOglLayer->glTexCoord2f(uvLeft, uvBottom);
    Widget::mOglLayer->glVertex2f(x, y + height);
    Widget::mOglLayer->glEnd();

    Widget::mOglLayer->endCommands();
}

void Image::onMessage(Wmsg message)
{
    if (message.id == WMSG_SIZE) {
        Widget::onMessage(message);

        switch (mScale) {
        case IMG_STRETCH:
            drawImage(Widget::mX, Widget::mY, Widget::mWidth, Widget::mHeight, 0.0f, 0.0f, 1.0f, 1.0f);
            break;

        case IMG_ADJUST:
            if ((Widget::mWidth*mHeightTexture)/mWidthTexture > Widget::mHeight) {
                int imageWidth = (Widget::mHeight*mWidthTexture)/mHeightTexture;
                int imageX = Widget::mX + (Widget::mWidth - imageWidth)*0.5f;
                drawImage(imageX, Widget::mY, imageWidth, Widget::mHeight, 0.0f, 0.0f, 1.0f, 1.0f);
            } else {
                int imageHeight = (Widget::mWidth*mHeightTexture)/mWidthTexture;
                int imageY = Widget::mY + (Widget::mHeight - imageHeight)*0.5f;
                drawImage(Widget::mX, imageY, Widget::mWidth, imageHeight, 0.0f, 0.0f, 1.0f, 1.0f);
            }
            break;

        case IMG_FILL:
            if ((Widget::mWidth*mHeightTexture)/mWidthTexture > Widget::mHeight) {
                int imageHeight = (Widget::mWidth*mHeightTexture)/mWidthTexture;
                float uvOffset = (0.5f*(imageHeight - Widget::mHeight))/imageHeight;
                drawImage(Widget::mX, Widget::mY, Widget::mWidth, Widget::mHeight, 0.0f, uvOffset, 1.0f, 1.0f - uvOffset);
            } else {
                int imageWidth = (Widget::mHeight*mWidthTexture)/mHeightTexture;
                float uvOffset = (0.5f*(imageWidth - Widget::mWidth))/imageWidth;
                drawImage(Widget::mX, Widget::mY, Widget::mWidth, Widget::mHeight, uvOffset, 0.0f, 1.0f - uvOffset, 1.0f);
            }
            break;
        }
    }
}
