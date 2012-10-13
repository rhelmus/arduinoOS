#include <SPI.h>
#include <GD.h>

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

bool inWindow(CWindow *topw, uint8_t x, uint8_t y)
{
    CWindow *w = topw;

    while (w)
    {
        const CWindow::SDimensions dim = w->getDimensions();

        if ((x >= dim.x) && (x <= (dim.x + dim.w)) &&
            (y >= dim.y) && (y <= (dim.y + dim.h)))
            return true;

        w = w->nextWindow();
    }

    return false;
}

int8_t getClosestHorizBorder(CWindow *topw, uint8_t col, uint8_t row)
{
    CWindow *w = topw;
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
    GD.wr16(RAM_PAL + (CHAR_WINDOW_TOP * 8), RGB(0, 255, 0));

    GD.copy(RAM_CHR + (CHAR_BORDER_VERT_LEFT * 16), borderChars, sizeof(borderChars));
    GD.copy(RAM_PAL + (CHAR_BORDER_VERT_LEFT * 8), borderPal, sizeof(borderPal));

    GD.copy(PALETTE4A, mouseArrowPal, sizeof(mouseArrowPal));
    GD.copy(RAM_SPRIMG, mouseArrowImg, sizeof(mouseArrowImg));
}

void CGUI::redrawScreen()
{
    // clear screen
    GD.fill(RAM_PIC, CHAR_BACKGROUND, 4096);

    CWindow *wit = topWindow;
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

        while (inWindow(topWindow, startcol, line))
        {
            const int8_t b = getClosestHorizBorder(topWindow, startcol, line);
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
            int8_t endcol = getClosestHorizBorder(topWindow, startcol+1, line);
            if (endcol == -1)
                endcol = wright + 1;
            /*Serial.print("start/endcol/x/right/line/y: "); Serial.print(startcol, DEC);
            Serial.print(", "); Serial.print(endcol, DEC);
            Serial.print(", "); Serial.print(olddim.x, DEC);
            Serial.print(", "); Serial.print(wright, DEC);
            Serial.print(", "); Serial.print(line, DEC);
            Serial.print(", "); Serial.println(olddim.y, DEC);*/
            GD.fill(atxy(startcol, line), CHAR_BACKGROUND, (endcol - startcol));
            startcol = getClosestHorizBorder(topWindow, endcol + 1, line);
            if (startcol == -1)
                break;
            ++startcol;

            while (inWindow(topWindow, startcol, line))
            {
                startcol = getClosestHorizBorder(topWindow, startcol, line);
                if (startcol == -1)
                    break;
                ++startcol;
            }
        }
    }

    CWindow *wit = topWindow->nextWindow();

    while (wit)
    {
        wit->draw();
        wit = wit->nextWindow();
    }

    topWindow->draw();
}

void CGUI::init()
{
    initGD();

    memset(mouseButtonStates, BUTTON_UP, sizeof(mouseButtonStates));

    mouseX = 200;
    mouseY = 150;
    drawMouse();

    setUSBMouseParser(&mouseParser);

    addWindow(&window);
    addWindow(&w2);
    addWindow(&w3);
    addWindow(&w4);

    redrawScreen();
}

void CGUI::addWindow(CWindow *w)
{
    if (!topWindow)
        topWindow = w;
    else
    {
        CWindow *wit = topWindow;
        while (wit->nextWindow())
            wit = wit->nextWindow();
        wit->setNextWindow(w);
    }
}

void CGUI::moveMouse(int8_t dx, int8_t dy)
{
    if ((mouseX + dx) < 400)
        mouseX += dx;
    if ((mouseY + dy) < 300)
        mouseY += dy;

    drawMouse();

    if (mouseButtonStates[BUTTON_LEFT] == BUTTON_DOWN)
    {
        setWindowPos(&window, convertPxToChar(mouseX), convertPxToChar(mouseY));
    }
}

void CGUI::setMouseButton(EMouseButton button, EMouseButtonState state)
{
    mouseButtonStates[button] = state;
}
