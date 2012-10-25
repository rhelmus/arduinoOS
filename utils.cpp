#include "gui.h"

#include <SPI.h>
#include <GD.h>

uint16_t atxy(uint8_t x, uint8_t y)
{
    // Copied from frogger tutorial
    return /*RAM_PIC*/FRAMEBUFFER + 64 * y + x;
}

