#ifndef WIDGET_H
#define WIDGET_H

#include "gui.h"

class CWidget
{
public:
    struct SDimensions { uint8_t x, y, w, h; };

private:
    CWidget *nextWidget;
    CWindow *parentWindow;
    SDimensions dim;
    bool active;

    virtual void coreDraw(void) = 0;
    virtual void coreUpdatePos(const SDimensions &olddim) { }
    virtual bool coreInWidget(uint8_t x, uint8_t y) const
    { return ((x >= dim.x) && (x < (dim.x + dim.w)) &&
              (y >= dim.y) && (y < (dim.y + dim.h))); }
    virtual bool coreHandleMouseClick(EMouseButton button) { return false; }
    virtual void coreHandleMouseMove(uint8_t mx, uint8_t my) { }

protected:
    CWidget(void) : nextWidget(0), parentWindow(0), active(false) { }

public:
    void setDimensions(uint8_t x, uint8_t y, uint8_t w, uint8_t h)
    { dim.x = x; dim.y = y; dim.w = w; dim.h = h; }
    void setSize(uint8_t w, uint8_t h) { dim.w = w; dim.h = h; }
    void setWidth(uint8_t w) { dim.w = w; }
    void setHeight(uint8_t h) { dim.h = h; }
    void draw(void) { coreDraw(); }
    bool inWidget(uint8_t x, uint8_t y) const { return coreInWidget(x, y); }
    bool handleMouseClick(EMouseButton button) { return coreHandleMouseClick(button); }
    void handleMouseMove(uint8_t mx, uint8_t my) { coreHandleMouseMove(mx, my); }
    bool isActive(void) const { return active; }
    void setActive(bool a) { active = a; }
    void setPos(uint8_t x, uint8_t y)
    { SDimensions od(dim); dim.x = x; dim.y = y; coreUpdatePos(od); }
    const SDimensions &getDimensions(void) const { return dim; }
    CWidget *getNextWidget(void) const { return nextWidget; }
    void setNextWidget(CWidget *w) { nextWidget = w; }
    CWindow *getParentWindow(void) const { return parentWindow; }
};

#endif // WIDGET_H
