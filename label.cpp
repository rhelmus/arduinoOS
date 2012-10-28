#include "gui.h"
#include "label.h"
#include "utils.h"

void CLabel::coreDraw()
{
    uint8_t x;

    switch (alignment)
    {
    case ALIGN_LEFT: x = getDimensions().x; break;
    case ALIGN_CENTER: x = center(getDimensions().x, strlen(text), getDimensions().w); break;
    case ALIGN_RIGHT: x = getDimensions().x + (getDimensions().w - strlen(text)); break;
    }

    if (flashText)
        putstr_P(x, getDimensions().y, text);
    else
        putstr(x, getDimensions().y, text);
}

void CLabel::setAlignment(EAlignment a)
{
    alignment = a;
    GUI.redrawDesktop();
}

void CLabel::setText(const char *t, bool aw, bool ft)
{
    text = t;
    flashText = ft;

    if (aw)
    {
        if (ft)
            setWidth(strlen_P(t));
        else
            setWidth(strlen(t));
    }

    GUI.redrawDesktop();
}
