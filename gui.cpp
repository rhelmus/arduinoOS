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

CDesktopLauncher launcher(3, 3, CHAR_APP_ICON_START, appIconPic, "App", &window);

void clearScreen(void)
{
    GD.fill(FRAMEBUFFER, CHAR_BACKGROUND, 4096);
}


CWidget *getTopWidgetFromPos(CWidget *botw, uint8_t x, uint8_t y)
{
    CWidget *w = botw, *ret = 0;

    while (w)
    {
        if (w->inWidget(x, y))
            ret = w;

        w = w->getNextWidget();
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

void CGUI::drawWindows()
{
    CWidget *wit = bottomWindow;
    while (wit)
    {
        wit->draw();
        wit = wit->getNextWidget();
    }
}

void CGUI::redrawDesktop()
{
    clearScreen();

    CWidget *wit = firstDesktopLauncher;
    while (wit)
    {
        wit->draw();
        wit = wit->getNextWidget();
    }

    drawWindows();
    updateCharScreen();
}

void CGUI::updateCharScreen()
{
    GD.wr(COMM+42, 1); // Copy framebuffer to char memory
    while (GD.rd(COMM+42))
        ; // Wait till j1 is done
}

void CGUI::drawMouse()
{
    GD.__wstartspr(0);
    // Slight offset for pointer not being in center
    draw_mouseArrow(mouseX+2, mouseY+4, 0, 0);
    GD.__end();
}

void CGUI::closeWindow(CWindow *w)
{
    if (bottomWindow != w)
    {
        // Find previous window
        CWindow *prevw = bottomWindow;
        while ((prevw->getNextWidget() != w) && prevw->getNextWidget())
            prevw = prevw->getNextWindow();

        if (prevw) // 0 if window not present
        {
            prevw->setNextWidget(w->getNextWidget()); // Unlink
            if (topWindow == w)
                topWindow = prevw;
        }
    }
    else
        bottomWindow = bottomWindow->getNextWindow();

    w->setNextWidget(0);
}

void CGUI::init()
{
    initGD();

    memset(mouseButtonStates, BUTTON_UP, sizeof(mouseButtonStates));

    drawMouse();
    setUSBMouseParser(&mouseParser);

//    activateWindow(&window);
    openWindow(&w2);
    openWindow(&w3);
    openWindow(&w4);

    addDesktopLauncher(&launcher);

    redrawDesktop();
}

void CGUI::openWindow(CWindow *w)
{
    if (w == topWindow)
        return;

    if (!bottomWindow)
        bottomWindow = topWindow = w;
    else
    {
        closeWindow(w); // remove from window list if present
        topWindow->setNextWidget(w);
        topWindow->setActive(false);
        topWindow = w;
    }

    w->setActive(true);
    redrawDesktop();
}

void CGUI::addDesktopLauncher(CDesktopLauncher *l)
{
    if (!firstDesktopLauncher)
        firstDesktopLauncher = l;
    else
    {
        CWidget *wit = firstDesktopLauncher;
        while (wit->getNextWidget())
            wit = wit->getNextWidget();
        wit->setNextWidget(l);
    }
}

void CGUI::moveMouse(int8_t dx, int8_t dy)
{
    if ((mouseX + dx) < 400)
        mouseX += dx;
    if ((mouseY + dy) < 300)
        mouseY += dy;

    drawMouse();

    if (dragWidget)
    {
        uint8_t newx = convertPxToChar(mouseX) + dragXOffset;
        uint8_t newy = convertPxToChar(mouseY) + dragYOffset;
        const CWidget::SDimensions dim(dragWidget->getDimensions());

        if ((newx + dim.w - 1) >= 50)
            newx = dim.x;
        if ((newy == 0) || ((newy + dim.h - 1) >= 37))
            newy = dim.y;

        if ((newx != dim.x) || (newy != dim.y))
        {
            dragged = true;
            dragWidget->setPos(newx, newy);
            redrawDesktop();
        }
    }
}

void CGUI::setMouseButton(EMouseButton button, EMouseButtonState state)
{
    mouseButtonStates[button] = state;

    if ((button == BUTTON_LEFT) && (state == BUTTON_DOWN))
    {
        const uint8_t chx = convertPxToChar(mouseX);
        const uint8_t chy = convertPxToChar(mouseY);
        CWidget *w = getTopWidgetFromPos(bottomWindow, chx, chy);

        if (w)
        {
            openWindow(static_cast<CWindow *>(w)); // Put window on top
            /*if (w != topWindow)
            {
                closeWindow(static_cast<CWindow *>(w));

                w->setActive(true);
                topWindow->setActive(false);

                topWindow->setNextWidget(w);
                w->setNextWidget(0);
                topWindow = static_cast<CWindow *>(w);

                drawWindows();
                updateCharScreen();
            }*/

            if (chy == topWindow->getDimensions().y) // Clicked at window top?
                dragWidget = w;
        }
        else // otherwise check desktop launchers
            dragWidget = getTopWidgetFromPos(firstDesktopLauncher, chx, chy);

        if (dragWidget)
        {
            dragXOffset = dragWidget->getDimensions().x - convertPxToChar(mouseX);
            dragYOffset = dragWidget->getDimensions().y - convertPxToChar(mouseY);
        }
    }
    else
    {
        if (dragWidget)
        {
            if (!dragged)
                dragWidget->handleMouseClick(button);
            else
                dragged = false;

            dragWidget = 0;
        }
    }
}
