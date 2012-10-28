#ifndef WIDGET_H
#define WIDGET_H

#include "gui.h"

class CWidget
{
public:
    struct SDimensions
    {
        uint8_t x, y, w, h;
        SDimensions(uint8_t _x, uint8_t _y, uint8_t _w, uint8_t _h)
            : x(_x), y(_y), w(_w), h(_h) { }
    };

private:
    CWidget *nextWidget;
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
    CWidget(uint8_t x, uint8_t y, uint8_t w, uint8_t h) :
        dim(x, y, w, h), active(false) { }
    CWidget(uint8_t x, uint8_t y) :
        dim(x, y, 0, 0), active(false) { }

    void setWidth(uint8_t w) { dim.w = w; }
    void setHeight(uint8_t h) { dim.h = h; }

public:
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
};

#endif // WIDGET_H
