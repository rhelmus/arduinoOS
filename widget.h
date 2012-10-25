#ifndef WIDGET_H
#define WIDGET_H

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
    virtual bool coreInWidget(uint8_t x, uint8_t y) const
    { return ((x >= dim.x) && (x < (dim.x + dim.w)) &&
              (y >= dim.y) && (y < (dim.y + dim.h))); }

protected:
    CWidget(uint8_t x, uint8_t y, uint8_t w, uint8_t h) :
        dim(x, y, w, h), active(false) { }

public:
    void draw(void) { coreDraw(); }
    bool inWidget(uint8_t x, uint8_t y) const { return coreInWidget(x, y); }
    bool isActive(void) const { return active; }
    void setActive(bool a) { active = a; }
    void setPos(uint8_t x, uint8_t y) { dim.x = x; dim.y = y; }
    const SDimensions &getDimensions(void) const { return dim; }
    CWidget *getNextWidget(void) const { return nextWidget; }
    void setNextWidget(CWidget *w) { nextWidget = w; }
};

#endif // WIDGET_H
