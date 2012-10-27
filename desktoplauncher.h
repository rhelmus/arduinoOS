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
    const char *name;
    CWindow *appWindow;

    virtual void coreDraw(void);
    virtual bool coreInWidget(uint8_t x, uint8_t y) const;
    virtual bool coreHandleMouseClick(EMouseButton button);

public:
    CDesktopLauncher(uint8_t x, uint8_t y, uint8_t i, prog_uchar *p, const char *n,
                     CWindow *aw);
};

#endif // DESKTOPLAUNCHER_H
