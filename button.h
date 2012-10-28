#ifndef BUTTON_H
#define BUTTON_H

#include "widget.h"

class CButton : public CWidget
{
    const char *text;
    bool flashText;
    bool highlight;

    virtual void coreDraw(void);
    virtual bool coreHandleMouseClick(EMouseButton button);
    virtual void coreHandleMouseMove(uint8_t mx, uint8_t my);

public:
    CButton(void) : text(0), flashText(false), highlight(false) { setHeight(3); }

    void setText(const char *t, bool ft=false);
};

#endif // BUTTON_H
