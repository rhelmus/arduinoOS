#ifndef DESKTOPLAUNCHER_H
#define DESKTOPLAUNCHER_H

#include <avr/pgmspace.h>

#include "widget.h"

class CWindow;

class CDesktopLauncher : public CWidget
{
    enum { ICON_SIZE = 3 };

    uint8_t iconOffset;
    prog_uchar *iconPic;
    const char *text;
    CWindow *appWindow;

    virtual void coreDraw(void);
    virtual bool coreInWidget(uint8_t x, uint8_t y) const;
    virtual bool coreHandleMouseClick(EMouseButton button);

public:
    CDesktopLauncher(void) : iconOffset(0), iconPic(0), text(0), appWindow(0)
    { setWidth(ICON_SIZE); setHeight(ICON_SIZE + 1); }

    void setIcon(uint8_t i, prog_uchar *p);
    void setText(const char *t);
    void setAppWindow(CWindow *a) { appWindow = a; }
};

#endif // DESKTOPLAUNCHER_H
