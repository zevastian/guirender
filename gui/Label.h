#include"Widget.h"
#include"Frame.h"

#ifndef _LABEL_H_
#define _LABEL_H_

class Label : public Widget
{
private:


public:
    /*create label*/
    Label(std::shared_ptr<Frame> parent);
    /*destroy label*/
    ~Label();

    /*process size message*/
    void onMessage(Wmsg message);
};

#endif
