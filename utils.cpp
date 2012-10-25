#include "gui.h"

#include <SPI.h>
#include <GD.h>

uint16_t atxy(uint8_t x, uint8_t y)
{
    // Copied from frogger tutorial
    return /*RAM_PIC*/FRAMEBUFFER + 64 * y + x;
}

// Basically GD.putstr, but using framebuffer
void putstr(uint8_t x, uint8_t y, const char *s)
{
    GD.__wstart(FRAMEBUFFER + (y << 6) + x);
    while (*s)
        SPI.transfer(*s++);
    GD.__end();
}
