#include "button.h"
#include "gui.h"
#include "utils.h"
#include "window.h"

#include <SPI.h>
#include <GD.h>

void CButton::coreDraw()
{
    const SDimensions dim(getDimensions());
    const uint8_t right = dim.x + dim.w - 1;
    const uint8_t bottom = dim.y + dim.h - 1;

    for (uint8_t x=dim.x+1; x<right; ++x)
    {
        GD.wr(atxy(x, dim.y),
              (highlight) ? CHAR_BUTTON_HORIZ_TOP_HL : CHAR_BUTTON_HORIZ_TOP);
        GD.wr(atxy(x, bottom),
              (highlight) ? CHAR_BUTTON_HORIZ_BOTTOM_HL : CHAR_BUTTON_HORIZ_BOTTOM);
    }

    for (uint8_t y=dim.y+1; y<bottom; ++y)
    {
        GD.wr(atxy(dim.x, y),
              (highlight) ? CHAR_BUTTON_VERT_LEFT_HL : CHAR_BUTTON_VERT_LEFT);
        GD.wr(atxy(right, y),
              (highlight) ? CHAR_BUTTON_VERT_RIGHT_HL : CHAR_BUTTON_VERT_RIGHT);
    }

    if (flashText)
        putstr_P(dim.x + 2, dim.y + 1, text);
    else
        putstr(dim.x + 2, dim.y + 1, text);
}

bool CButton::coreHandleMouseClick(EMouseButton button)
{
    if (getParentWindow())
        getParentWindow()->callback(this);

    return true;
}

void CButton::coreHandleMouseMove(uint8_t mx, uint8_t my)
{
    const bool oldh = highlight;
    highlight = inWidget(mx, my);
    if (oldh != highlight)
        GUI.redrawDesktop();
}

void CButton::setText(const char *t, bool ft)
{
    text = t;
    flashText = ft;

    if (ft)
        setWidth(strlen_P(t) + 4);
    else
        setWidth(strlen(t) + 4);

    GUI.redrawDesktop();
}
