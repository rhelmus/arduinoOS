#ifndef GUI_H
#define GUI_H

#include <hidboot.h>

class CDesktopLauncher;
class CWidget;
class CWindow;

enum
{
    CHAR_BACKGROUND=0,
    CHAR_TRANSPARENT,

    CHAR_WINDOW_TOP_INACTIVE,
    CHAR_WINDOW_TOP_ACTIVE,

    CHAR_BORDER_VERT_LEFT=128, // Start after fonts
    CHAR_BORDER_VERT_RIGHT,
    CHAR_BORDER_HORIZ_TOP,
    CHAR_BORDER_HORIZ_BOTTOM,
    CHAR_BORDER_CORNER_BOTTOM_LEFT,
    CHAR_BORDER_CORNER_BOTTOM_RIGHT,

    CHAR_APP_ICON_START
};

#define FRAMEBUFFER 0x6FFF

enum EMouseButton { BUTTON_LEFT=0, BUTTON_RIGHT, BUTTON_MIDDLE, BUTTON_MAX };
enum EMouseButtonState { BUTTON_DOWN, BUTTON_UP };

class CUSBMouseParser : public MouseReportParser
{
protected:
    virtual void OnMouseMove(MOUSEINFO *mi);
    virtual void OnLeftButtonUp(MOUSEINFO *mi);
    virtual void OnLeftButtonDown(MOUSEINFO *mi);
    virtual void OnRightButtonUp(MOUSEINFO *mi);
    virtual void OnRightButtonDown(MOUSEINFO *mi);
    virtual void OnMiddleButtonUp(MOUSEINFO *mi);
    virtual void OnMiddleButtonDown(MOUSEINFO *mi);
};

class CGUI
{
    CUSBMouseParser mouseParser;
    uint16_t mouseX, mouseY;
    EMouseButtonState mouseButtonStates[BUTTON_MAX];
    CWindow *bottomWindow, *topWindow;
    CDesktopLauncher *firstDesktopLauncher;
    CWidget *dragWidget;
    uint8_t dragXOffset, dragYOffset;
    bool dragged;

    void initGD(void);
    void drawWindows(void);
    void redrawDesktop(void);
    void updateCharScreen(void);
    void drawMouse(void);
    void closeWindow(CWindow *w);

public:
    CGUI(void) : mouseX(200), mouseY(150), bottomWindow(0), topWindow(0),
        firstDesktopLauncher(0), dragWidget(0) { }

    void init(void);
    void openWindow(CWindow *w);
    void addDesktopLauncher(CDesktopLauncher *l);
    void moveMouse(int8_t dx, int8_t dy);
    void setMouseButton(EMouseButton button, EMouseButtonState state);
};

extern CGUI GUI;

#endif // GUI_H
