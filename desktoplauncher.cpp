#include "desktoplauncher.h"
#include "utils.h"

#include <SPI.h>
#include <GD.h>

CDesktopLauncher::CDesktopLauncher(uint8_t x, uint8_t y, uint8_t i,
                                   prog_uchar *p, const char *n, CWindow *aw)
    : CWidget(x, y), iconOffset(i), iconPic(p), name(n), appWindow(aw)
{
    setWidth(max(strlen(n), ICON_SIZE));
    setHeight(ICON_SIZE + 1);
}

void CDesktopLauncher::coreDraw()
{
    uint8_t i = 0;
    const uint8_t ileft = center(getDimensions().x, ICON_SIZE, getDimensions().w);
    const uint8_t iright = ileft + ICON_SIZE;
    for (uint8_t y=getDimensions().y; y<(getDimensions().y + ICON_SIZE); ++y)
    {
        for (uint8_t x=ileft; x<iright; ++x, ++i)
        {
            GD.wr(atxy(x, y), iconOffset + pgm_read_byte_near(iconPic + i));
        }
    }

    putstr(center(getDimensions().x, strlen(name), getDimensions().w),
           getDimensions().y + getDimensions().h-1, name);
}

bool CDesktopLauncher::coreInWidget(uint8_t x, uint8_t y) const
{
    const SDimensions d(getDimensions());
    if ((y >= d.y) && (y < (d.y + d.h))) // Within y range?
    {
        uint8_t left = center(d.x, ICON_SIZE, d.w);

        if (y < (d.y + d.h - 1)) // Above title?
            return ((x >= left) && (x < (left + ICON_SIZE)));

        const uint8_t len = strlen(name);
        left = center(d.x, len, d.w);
        return (x >= left) && (x < (left + len));
    }

    return false;
}

bool CDesktopLauncher::coreHandleMouseClick(EMouseButton button)
{
    if (button == BUTTON_LEFT)
    {
        GUI.openWindow(appWindow);
        return true;
    }

    return false;
}
