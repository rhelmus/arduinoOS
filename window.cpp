#include "gui.h"
#include "utils.h"
#include "window.h"

#include <SPI.h>
#include <GD.h>

void CWindow::draw()
{
    // Top
    GD.fill(atxy(dim.x, dim.y), CHAR_WINDOW_TOP, dim.w+1);

//    GD.putstr(dim.x + ((dim.w - 6)/2), dim.y, "Window");

    // Corners
    GD.wr(atxy(dim.x, dim.y + dim.h), CHAR_BORDER_CORNER_BOTTOM_LEFT);
    GD.wr(atxy(dim.x + dim.w, dim.y + dim.h), CHAR_BORDER_CORNER_BOTTOM_RIGHT);

    //         GD.fill(atxy(dim.x + 1, dim.y), CHAR_LINE_HORIZ, dim.w-1);
    GD.fill(atxy(dim.x + 1, dim.y + dim.h), CHAR_BORDER_HORIZ_BOTTOM, dim.w-1);

    for (uint8_t y=(dim.y + 1); y<(dim.y + dim.h); ++y)
    {
        GD.wr(atxy(dim.x, y), CHAR_BORDER_VERT_LEFT);
        GD.wr(atxy(dim.x + dim.w, y), CHAR_BORDER_VERT_RIGHT);
        GD.fill(atxy(dim.x + 1, y), CHAR_TRANSPARENT, dim.w-1);
    }
}
