#ifndef WINDOW_H
#define WINDOW_H

#include "widget.h"

class CWindow : public CWidget
{
    CWidget *firstChildWidget;

    virtual void coreDraw(void);
    virtual void coreUpdatePos(const SDimensions &olddim);
    virtual bool coreHandleMouseClick(EMouseButton button);
    virtual void coreHandleMouseMove(uint8_t mx, uint8_t my);
    virtual void coreCallback(CWidget *child) { }

public:
    // UNDONE: Make protected
    CWindow(void) : firstChildWidget(0) { }

    void addChild(CWidget *w);
    void callback(CWidget *child) { coreCallback(child); }
    CWindow *getNextWindow(void) const { return static_cast<CWindow *>(getNextWidget()); }
};

#endif // WINDOW_H
