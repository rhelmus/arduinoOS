#include <SPI.h>
#include <GD.h>

#include <stdint.h>

uint16_t atxy(uint8_t x, uint8_t y)
{
    // Copied from frogger tutorial
    return RAM_PIC + 64 * y + x;
}

