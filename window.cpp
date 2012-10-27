#include "gui.h"
#include "utils.h"
#include "window.h"

#include <SPI.h>
#include <GD.h>

void CWindow::coreDraw()
{
    // Top
    GD.fill(atxy(getDimensions().x, getDimensions().y),
            (isActive()) ? CHAR_WINDOW_TOP_ACTIVE : CHAR_WINDOW_TOP_INACTIVE,
            getDimensions().w);

    // UNDONE
    putstr(getDimensions().x + ((getDimensions().w - 6)/2), getDimensions().y, "Window");

    const uint8_t right = getDimensions().x + getDimensions().w - 1;
    const uint8_t bottom = getDimensions().y + getDimensions().h - 1;

    // Corners
    GD.wr(atxy(getDimensions().x, bottom), CHAR_BORDER_CORNER_BOTTOM_LEFT);
    GD.wr(atxy(right, bottom), CHAR_BORDER_CORNER_BOTTOM_RIGHT);

    GD.fill(atxy(getDimensions().x + 1, bottom), CHAR_BORDER_HORIZ_BOTTOM,
            getDimensions().w-2);

    for (uint8_t y=(getDimensions().y + 1); y<bottom; ++y)
    {
        GD.wr(atxy(getDimensions().x, y), CHAR_BORDER_VERT_LEFT);
        GD.wr(atxy(right, y), CHAR_BORDER_VERT_RIGHT);
        GD.fill(atxy(getDimensions().x + 1, y), CHAR_TRANSPARENT, getDimensions().w-2);
    }
}
