#include "desktoplauncher.h"
#include "utils.h"

#include <SPI.h>
#include <GD.h>

void CDesktopLauncher::coreDraw()
{
    uint8_t i = 0;
    const uint8_t ileft = getDimensions().x + 2, iright = ileft + 3;
    for (uint8_t y=getDimensions().y; y<(getDimensions().y + 3); ++y)
    {
        for (uint8_t x=ileft; x<iright; ++x, ++i)
        {
            GD.wr(atxy(x, y), /*iconOffset + pgm_read_byte_near(iconPic + i)*/1);
        }
    }

    GD.putstr(getDimensions().x + ((getDimensions().w - strlen(name)) / 2),
              getDimensions().y + getDimensions().h-1, name);
}
