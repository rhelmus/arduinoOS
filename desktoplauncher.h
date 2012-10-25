#ifndef DESKTOPLAUNCHER_H
#define DESKTOPLAUNCHER_H

#include <avr/pgmspace.h>

#include "widget.h"

class CDesktopLauncher : public CWidget
{
    uint8_t iconOffset;
    prog_uchar *iconPic;
    const char *name;

    virtual void coreDraw(void);
    virtual bool coreInWidget(uint8_t x, uint8_t y) const;

public:
    CDesktopLauncher(uint8_t x, uint8_t y, uint8_t i, prog_uchar *p, const char *n)
        : CWidget(x, y, 7, 4), iconOffset(i), iconPic(p), name(n) { }
};

#endif // DESKTOPLAUNCHER_H
