#include <SPI.h>
#include <GD.h>

#include "j1firmware/bg.h"
#include "desktoplauncher.h"
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

CDesktopLauncher launcher(3, 3, CHAR_APP_ICON_START, appIconPic, "App");

CWindow *getTopWindowFromPos(CWindow *botw, uint8_t x, uint8_t y)
{
    CWindow *w = botw, *ret = 0;

    while (w)
    {
        const CWindow::SDimensions dim = w->getDimensions();

        if ((x >= dim.x) && (x <= (dim.x + dim.w)) &&
            (y >= dim.y) && (y <= (dim.y + dim.h)))
            ret = w;

        w = w->getNextWindow();
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

        w = w->getNextWindow();
    }

    return ret;
}

// Font loading from assets, see http://excamera.com/sphinx/gameduino/samples/assetlibrary/index.html
void assetASCII()
{
    uint8_t stretch[16] =
    {
        0x00, 0x03, 0x0c, 0x0f,
        0x30, 0x33, 0x3c, 0x3f,
        0xc0, 0xc3, 0xcc, 0xcf,
        0xf0, 0xf3, 0xfc, 0xff
    };

#ifdef __AVR__
    Asset a;
    a.open("fonts", "font8x8", NULL);
#endif

    for (uint16_t i=0; i<768; ++i)
    {
        uint8_t b;

#ifndef __AVR__
        b = pgm_read_byte(font8x8 + i);
#else
        a.read(&b, 1);
#endif
        uint8_t h = stretch[b >> 4];
        uint8_t l = stretch[b & 0xf];
        GD.wr(0x1000 + (16 * ' ') + (2 * i), h);
        GD.wr(0x1000 + (16 * ' ') + (2 * i) + 1, l);
    }

    for (uint8_t i = 0x20; i < 0x80; ++i)
    {
        GD.setpal(4 * i + 0, TRANSPARENT);
        GD.setpal(4 * i + 3, RGB(0, 0, 0));
    }

//    GD.fill(RAM_PIC, ' ', 4096);
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

    assetASCII();

    GD.wr16(BG_COLOR, RGB(225, 225, 225));

    GD.wr16(RAM_PAL + (CHAR_BACKGROUND * 8), RGB(0, 0, 255));
    GD.wr16(RAM_PAL + (CHAR_TRANSPARENT * 8), TRANSPARENT);
    GD.wr16(RAM_PAL + (CHAR_WINDOW_TOP_INACTIVE * 8), RGB(180, 180, 180));
    GD.wr16(RAM_PAL + (CHAR_WINDOW_TOP_ACTIVE * 8), RGB(0, 255, 0));

    GD.copy(RAM_CHR + (CHAR_BORDER_VERT_LEFT * 16), borderChars, sizeof(borderChars));
    GD.copy(RAM_PAL + (CHAR_BORDER_VERT_LEFT * 8), borderPal, sizeof(borderPal));

    GD.copy(RAM_CHR + (CHAR_APP_ICON_START * 16), appIconChars, sizeof(appIconChars));
    GD.copy(RAM_PAL + (CHAR_APP_ICON_START * 8), appIconPal, sizeof(appIconPal));

    GD.copy(PALETTE4A, mouseArrowPal, sizeof(mouseArrowPal));
    GD.copy(RAM_SPRIMG, mouseArrowImg, sizeof(mouseArrowImg));

    GD.wr16(COMM+0, RGB(200, 200, 200)); // background top bar

    // Desktop gradient
    for (uint8_t i=2; i<40; i+=2)
    {
        GD.wr16(COMM+i, RGB(50-i, 100-i, 255-i));
    }

    GD.microcode(bg_code, sizeof(bg_code));
}

void CGUI::redrawDesktop()
{
    while (GD.rd(COMM+42))
        ; // Wait till j1 is done with drawing framebuffer

    CWidget *wit = firstDesktopLauncher;
    while (wit)
    {
        wit->draw();
        wit = wit->getNextWidget();
    }

    wit = bottomWindow;
    while (wit)
    {
        wit->draw();
        wit = wit->getNextWidget();
    }

    GD.wr(COMM+42, 1); // Copy framebuffer to char memory
}

void CGUI::drawMouse()
{
    GD.__wstartspr(0);
    // Slight offset for pointer not being in center
    draw_mouseArrow(mouseX+2, mouseY+4, 0, 0);
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

    // UNDONE: doesn't keep window order right
#if 0
    CWindow *wit = bottomWindow;
    while (wit)
    {
#define inDimX(d1, d2) ((d1.x >= d2.x) && (d1.x <= (d2.x + d2.w)))
#define inDimY(d1, d2) ((d1.y >= d2.y) && (d1.y <= (d2.y + d2.h)))

        const CWindow::SDimensions dim = wit->getDimensions();

        if ((inDimX(dim, olddim) || inDimX(olddim, dim)) &&
            (inDimY(dim, olddim) || inDimY(olddim, dim)))
            wit->draw();

        /*if (((olddim.x >= dim.x) && (olddim.x <= (dim.x + dim.w)) &&
            (olddim.y >= dim.y) && (olddim.y <= (dim.y + dim.h))) ||
           ((dim.x >= olddim.x) && (dim.x <= (olddim.x + olddim.w)) &&
            (dim.y >= olddim.y) && (dim.y <= (olddim.y + olddim.h))))
            wit->draw();*/

        wit = wit->getNextWidget();
    }
#else
    redrawDesktop();
#endif
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

    addDesktopLauncher(&launcher);

    GD.fill(/*RAM_PIC*/FRAMEBUFFER, CHAR_BACKGROUND, 4096); // clear screen
    redrawDesktop();
}

void CGUI::addWindow(CWindow *w)
{
    if (!bottomWindow)
        bottomWindow = topWindow = w;
    else
    {
        topWindow->setNextWidget(w);
        topWindow->setActive(false);
        w->setActive(true);
        topWindow = w;
    }
}

void CGUI::addDesktopLauncher(CDesktopLauncher *l)
{
    if (!firstDesktopLauncher)
        firstDesktopLauncher = l;
    else
    {
        CWidget *lit = firstDesktopLauncher;
        while (lit->getNextWidget())
            lit = lit->getNextWidget();
        lit->setNextWidget(l);
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
        if ((newy == 0) || (newy + dim.h) >= 37)
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
                    while (oldpw->getNextWidget() != w)
                        oldpw = oldpw->getNextWindow();
                    oldpw->setNextWidget(w->getNextWidget()); // Unlink
                }
                else
                    bottomWindow = bottomWindow->getNextWindow();

                w->setActive(true);
                topWindow->setActive(false);

                topWindow->setNextWidget(w);
                w->setNextWidget(0);
                topWindow = w;

                redrawDesktop();
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
