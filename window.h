#ifndef WINDOW_H
#define WINDOW_H

#include "widget.h"

class CWindow : public CWidget
{
    CWindow *next;

    virtual void coreDraw(void);
    virtual bool coreHandleMouseClick(EMouseButton button);

public:
    // UNDONE: Make protected
    CWindow(uint8_t x, uint8_t y, uint8_t w, uint8_t h) :
        CWidget(x, y, w, h), next(0) { }

    CWindow *getNextWindow(void) const { return static_cast<CWindow *>(getNextWidget()); }
};

#endif // WINDOW_H
