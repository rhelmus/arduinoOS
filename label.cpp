#include "gui.h"
#include "label.h"
#include "utils.h"

CLabel::CLabel(uint8_t x, uint8_t y, const char *t, bool ft)
    : CWidget(x, y), text(t), flashText(ft), alignment(ALIGN_LEFT)
{
    if (ft)
        setWidth(strlen_P(t));
    else
        setWidth(strlen(t));

    setHeight(1);
}

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

void CLabel::setText(const char *t, bool ft)
{
    text = t;
    flashText = ft;
    GUI.redrawDesktop();
}
