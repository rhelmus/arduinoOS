#include "desktoplauncher.h"
#include "utils.h"

#include <SPI.h>
#include <GD.h>

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

    putstr(center(getDimensions().x, strlen(text), getDimensions().w),
           getDimensions().y + getDimensions().h-1, text);
}

bool CDesktopLauncher::coreInWidget(uint8_t x, uint8_t y) const
{
    const SDimensions d(getDimensions());
    if ((y >= d.y) && (y < (d.y + d.h))) // Within y range?
    {
        uint8_t left = center(d.x, ICON_SIZE, d.w);

        if (y < (d.y + d.h - 1)) // Above title?
            return ((x >= left) && (x < (left + ICON_SIZE)));

        const uint8_t len = strlen(text);
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

void CDesktopLauncher::setIcon(uint8_t i, prog_uchar *p)
{
    iconOffset = i;
    iconPic = p;
    GUI.redrawDesktop();
}

void CDesktopLauncher::setText(const char *t)
{
    text = t;
    setWidth(max(strlen(t), ICON_SIZE));
    GUI.redrawDesktop();
}
