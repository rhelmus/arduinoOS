#ifndef BUTTON_H
#define BUTTON_H

#include "widget.h"

class CButton : public CWidget
{
    const char *text;
    bool highlight;

    virtual void coreDraw(void);
    virtual bool coreHandleMouseClick(EMouseButton button);
    virtual void coreHandleMouseMove(uint8_t mx, uint8_t my);

public:
    CButton(uint8_t x, uint8_t y, const char *t);
};

#endif // BUTTON_H
