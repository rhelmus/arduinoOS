#include "gui.h"

#include <SPI.h>
#include <GD.h>

// Copied from frogger tutorial
uint16_t atxy(uint8_t x, uint8_t y)
{
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

// As above, but for flash based strings
void putstr_P(uint8_t x, uint8_t y, const char *s)
{
    char c;

    GD.__wstart(FRAMEBUFFER + (y << 6) + x);
    while ((c = pgm_read_byte(s++)))
        SPI.transfer(c);
    GD.__end();
}

// From http://jeelabs.org/2011/05/22/atmega-memory-use/
int freeRam()
{
    extern int __heap_start, *__brkval;
    int v;
    return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}
