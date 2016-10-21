#include"Widget.h"
#include"Frame.h"

#ifndef _IMAGE_H_
#define _IMAGE_H_

/*properties*/
enum IMG_SCALE {
    IMG_STRETCH,
    IMG_FILL,
    IMG_ADJUST
};

class Image : public Widget
{
private:
    /*member vars*/
    GLuint mTexture;
    IMG_SCALE mScale;
    int mWidthTexture;
    int mHeightTexture;

    /**/
    void drawImage(int x, int y, int width, int height, float uvLeft, float uvTop, float uvRight, float uvBottom);

public:
    /*create image*/
    Image(std::shared_ptr<Frame> parent);
    /*destroy image*/
    ~Image();

    /*process size message*/
    void onMessage(Wmsg message);
};

#endif
