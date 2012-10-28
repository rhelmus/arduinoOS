#include "gui.h"
#include "utils.h"
#include "window.h"

#include <SPI.h>
#include <GD.h>

void CWindow::coreDraw()
{
    const uint8_t right = getDimensions().x + getDimensions().w - 1;
    const uint8_t bottom = getDimensions().y + getDimensions().h - 1;

    // Top
    GD.fill(atxy(getDimensions().x, getDimensions().y),
            (isActive()) ? CHAR_WINDOW_TOP_ACTIVE : CHAR_WINDOW_TOP_INACTIVE,
            getDimensions().w-1);

    // Close button
    GD.wr(atxy(right, getDimensions().y), CHAR_WINDOW_CLOSE);

    // Title
    // UNDONE
    putstr(getDimensions().x + ((getDimensions().w - 6)/2), getDimensions().y, "Window");

    // Corners
    GD.wr(atxy(getDimensions().x, bottom), CHAR_WINDOW_CORNER_BOTTOM_LEFT);
    GD.wr(atxy(right, bottom), CHAR_WINDOW_CORNER_BOTTOM_RIGHT);

    GD.fill(atxy(getDimensions().x + 1, bottom), CHAR_WINDOW_HORIZ_BOTTOM,
            getDimensions().w-2);

    for (uint8_t y=(getDimensions().y + 1); y<bottom; ++y)
    {
        GD.wr(atxy(getDimensions().x, y), CHAR_WINDOW_VERT_LEFT);
        GD.wr(atxy(right, y), CHAR_WINDOW_VERT_RIGHT);
        GD.fill(atxy(getDimensions().x + 1, y), CHAR_TRANSPARENT, getDimensions().w-2);
    }

    CWidget *wit = firstChildWidget;
    while (wit)
    {
        wit->draw();
        wit = wit->getNextWidget();
    }
}

void CWindow::coreUpdatePos(const SDimensions &olddim)
{
    const uint8_t dx = getDimensions().x - olddim.x;
    const uint8_t dy = getDimensions().y - olddim.y;

    CWidget *wit = firstChildWidget;
    while (wit)
    {
        wit->setPos(wit->getDimensions().x + dx, wit->getDimensions().y + dy);
        wit = wit->getNextWidget();
    }
}

bool CWindow::coreHandleMouseClick(EMouseButton button)
{
    const uint8_t mx = convertPxToChar(GUI.getMouseX());
    const uint8_t my = convertPxToChar(GUI.getMouseY());

    // First check child widgets
    CWidget *wit = firstChildWidget;
    while (wit)
    {
        if (wit->inWidget(mx, my) && wit->handleMouseClick(button))
            return true;
        wit = wit->getNextWidget();
    }

    if (button != BUTTON_LEFT)
        return false;

    // Clicked close button? (top-right corner)
    if ((mx == (getDimensions().x + getDimensions().w - 1)) && (my == getDimensions().y))
    {
        GUI.closeWindow(this);
        return true;
    }

    return false;
}

void CWindow::coreHandleMouseMove(uint8_t mx, uint8_t my)
{
    CWidget *wit = firstChildWidget;
    while (wit)
    {
        wit->handleMouseMove(mx, my);
        wit = wit->getNextWidget();
    }
}

void CWindow::addChild(CWidget *w)
{
    if (!firstChildWidget)
        firstChildWidget = w;
    else
    {
        CWidget *wit = firstChildWidget;
        while (wit->getNextWidget())
            wit = wit->getNextWidget();
        wit->setNextWidget(w);
    }
}
