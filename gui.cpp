#include <SPI.h>
#include <GD.h>

#include "j1firmware/bg.h"
#include "gfx.h"
#include "gui.h"
#include "usbmouse.h"
#include "utils.h"
#include "window.h"

CGUI GUI;

namespace
{
CWindow window(20, 20, 20, 10);
CWindow w2(10, 10, 10, 10);
CWindow w3(30, 25, 20, 10);
CWindow w4(5, 25, 25, 5);

CWindow *getTopWindowFromPos(CWindow *botw, uint8_t x, uint8_t y)
{
    CWindow *w = botw, *ret = 0;

    while (w)
    {
        const CWindow::SDimensions dim = w->getDimensions();

        if ((x >= dim.x) && (x <= (dim.x + dim.w)) &&
            (y >= dim.y) && (y <= (dim.y + dim.h)))
            ret = w;

        w = w->nextWindow();
    }

    return ret;
}

int8_t getClosestHorizBorder(CWindow *botw, uint8_t col, uint8_t row)
{
    CWindow *w = botw;
    int8_t ret = -1;

    while (w)
    {
        const CWindow::SDimensions dim = w->getDimensions();

        if ((row >= dim.y) && (row <= (dim.y + dim.h)))
        {
            if (dim.x >= col)
            {
                if ((ret == -1) || (ret > dim.x))
                    ret = dim.x;
            }
            else if ((dim.x + dim.w) >= col)
            {
                if ((ret == -1) || (ret > (dim.x + dim.w)))
                    ret = dim.x + dim.w;
            }
        }

        w = w->nextWindow();
    }

    return ret;
}

}


void CUSBMouseParser::OnMouseMove(MOUSEINFO *mi)
{
    GUI.moveMouse(mi->dX, mi->dY);
}

void CUSBMouseParser::OnLeftButtonUp(MOUSEINFO *mi)
{
    GUI.setMouseButton(BUTTON_LEFT, BUTTON_UP);
}

void CUSBMouseParser::OnLeftButtonDown(MOUSEINFO *mi)
{
    GUI.setMouseButton(BUTTON_LEFT, BUTTON_DOWN);
}

void CUSBMouseParser::OnRightButtonUp(MOUSEINFO *mi)
{
    GUI.setMouseButton(BUTTON_RIGHT, BUTTON_UP);
}

void CUSBMouseParser::OnRightButtonDown(MOUSEINFO *mi)
{
    GUI.setMouseButton(BUTTON_RIGHT, BUTTON_DOWN);
}

void CUSBMouseParser::OnMiddleButtonUp(MOUSEINFO *mi)
{
    GUI.setMouseButton(BUTTON_MIDDLE, BUTTON_UP);
}

void CUSBMouseParser::OnMiddleButtonDown(MOUSEINFO *mi)
{
    GUI.setMouseButton(BUTTON_MIDDLE, BUTTON_DOWN);
}


void CGUI::initGD()
{
    GD.begin();

    GD.wr16(BG_COLOR, RGB(225, 225, 225));

    GD.wr16(RAM_PAL + (CHAR_BACKGROUND * 8), RGB(0, 0, 255));
    GD.wr16(RAM_PAL + (CHAR_TRANSPARENT * 8), TRANSPARENT);
    GD.wr16(RAM_PAL + (CHAR_WINDOW_TOP_INACTIVE * 8), RGB(180, 180, 180));
    GD.wr16(RAM_PAL + (CHAR_WINDOW_TOP_ACTIVE * 8), RGB(0, 255, 0));

    GD.copy(RAM_CHR + (CHAR_BORDER_VERT_LEFT * 16), borderChars, sizeof(borderChars));
    GD.copy(RAM_PAL + (CHAR_BORDER_VERT_LEFT * 8), borderPal, sizeof(borderPal));

    GD.copy(PALETTE4A, mouseArrowPal, sizeof(mouseArrowPal));
    GD.copy(RAM_SPRIMG, mouseArrowImg, sizeof(mouseArrowImg));

    GD.wr16(COMM+0, RGB(200, 200, 200)); // background top bar

    // Desktop gradient
    for (uint8_t i=2; i<40; i+=2)
    {
        GD.wr16(COMM+i, RGB(50-i, 100-i, 255-i));
    }

//    GD.wr16(COMM+4, RGB(0, 0, 255));

    GD.microcode(bg_code, sizeof(bg_code));
}

void CGUI::redrawWindows()
{
    CWindow *wit = bottomWindow;
    while (wit)
    {
        wit->draw();
        wit = wit->nextWindow();
    }
}

void CGUI::drawMouse()
{
    GD.__wstartspr(0);
    draw_mouseArrow(mouseX, mouseY, 0, 0);
    GD.__end();
}

void CGUI::setWindowPos(CWindow *w, uint16_t x, uint16_t y)
{
    CWindow::SDimensions olddim = w->getDimensions();
    w->setPos(x, y);

    const uint8_t wright = olddim.x + olddim.w;
    const uint8_t wbottom = olddim.y + olddim.h;

    for (uint8_t line=olddim.y; line<=wbottom; ++line)
    {
        int8_t startcol = olddim.x;

        while (getTopWindowFromPos(bottomWindow, startcol, line) != 0)
        {
            const int8_t b = getClosestHorizBorder(bottomWindow, startcol, line);
            if (b == -1)
            {
                ++startcol;
                break;
            }
            else
                startcol = b + 1;
        }

        while ((startcol != -1) && (startcol <= wright))
        {
            int8_t endcol = getClosestHorizBorder(bottomWindow, startcol+1, line);
            if (endcol == -1)
                endcol = wright + 1;
            /*Serial.print("start/endcol/x/right/line/y: "); Serial.print(startcol, DEC);
            Serial.print(", "); Serial.print(endcol, DEC);
            Serial.print(", "); Serial.print(olddim.x, DEC);
            Serial.print(", "); Serial.print(wright, DEC);
            Serial.print(", "); Serial.print(line, DEC);
            Serial.print(", "); Serial.println(olddim.y, DEC);*/
            GD.fill(atxy(startcol, line), CHAR_BACKGROUND, (endcol - startcol));
            startcol = getClosestHorizBorder(bottomWindow, endcol + 1, line);
            if (startcol == -1)
                break;
            ++startcol;

            while (getTopWindowFromPos(bottomWindow, startcol, line) != 0)
            {
                startcol = getClosestHorizBorder(bottomWindow, startcol, line);
                if (startcol == -1)
                    break;
                ++startcol;
            }
        }
    }

    redrawWindows();
}

void CGUI::init()
{
    initGD();

    memset(mouseButtonStates, BUTTON_UP, sizeof(mouseButtonStates));
    mouseX = 200;
    mouseY = 150;
    dragWindow = false;

    drawMouse();
    setUSBMouseParser(&mouseParser);

    addWindow(&window);
    addWindow(&w2);
    addWindow(&w3);
    addWindow(&w4);

    GD.fill(RAM_PIC, CHAR_BACKGROUND, 4096); // clear screen
    redrawWindows();
}

void CGUI::addWindow(CWindow *w)
{
    if (!bottomWindow)
        bottomWindow = topWindow = w;
    else
    {
        topWindow->setNextWindow(w);
        topWindow->setActive(false);
        w->setActive(true);
        topWindow = w;
    }
}

void CGUI::moveMouse(int8_t dx, int8_t dy)
{
    if ((mouseX + dx) < 400)
        mouseX += dx;
    if ((mouseY + dy) < 300)
        mouseY += dy;

    drawMouse();

    if (dragWindow)
    {
        uint8_t newx = convertPxToChar(mouseX) + winDragXOffset;
        uint8_t newy = convertPxToChar(mouseY);
        const CWindow::SDimensions dim(topWindow->getDimensions());

        if ((newx + dim.w) >= 50)
            newx = dim.x;
        if ((newy + dim.h) >= 37)
            newy = dim.y;

        setWindowPos(topWindow, newx, newy);
    }
}

void CGUI::setMouseButton(EMouseButton button, EMouseButtonState state)
{
    mouseButtonStates[button] = state;

    if ((button == BUTTON_LEFT) && (state == BUTTON_DOWN))
    {
        const uint8_t chx = convertPxToChar(mouseX);
        const uint8_t chy = convertPxToChar(mouseY);
        CWindow *w = getTopWindowFromPos(bottomWindow, chx, chy);

        if (w)
        {
            if (w != topWindow)
            {
                if (w != bottomWindow)
                {
                    // Find old previous window
                    CWindow *oldpw = bottomWindow;
                    while (oldpw->nextWindow() != w)
                        oldpw = oldpw->nextWindow();
                    oldpw->setNextWindow(w->nextWindow()); // Unlink
                }
                else
                    bottomWindow = bottomWindow->nextWindow();

                w->setActive(true);
                topWindow->setActive(false);

                topWindow->setNextWindow(w);
                w->setNextWindow(0);
                topWindow = w;

                redrawWindows();
            }

            if (chy == topWindow->getDimensions().y) // Clicked at window top?
            {
                dragWindow = true;
                winDragXOffset = topWindow->getDimensions().x - convertPxToChar(mouseX);
            }
        }
    }
    else if (dragWindow)
        dragWindow = false;
}
