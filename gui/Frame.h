#include"Widget.h"

#ifndef _FRAME_H_
#define _FRAME_H_

class Frame : public Widget
{
private:


public:
    /*create parent frame*/
    Frame(std::shared_ptr<OGLLayer> oglLayer);
    /*create child frame*/
    Frame(std::shared_ptr<Frame> parent);
    /*destroy frame*/
    ~Frame();

    /*process size message*/
    void onMessage(Wmsg message);
};

#endif
