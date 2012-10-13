#ifndef WINDOW_H
#define WINDOW_H

class CWindow
{
public:
    struct SDimensions
    {
        uint8_t x, y, w, h;
        SDimensions(uint8_t _x, uint8_t _y, uint8_t _w, uint8_t _h)
            : x(_x), y(_y), w(_w), h(_h) { }
    };

private:
    SDimensions dim;
    CWindow *next;

    virtual void coreDraw(void) { } // UNDONE: make pure virtual

public:
    // UNDONE: Make protected
    CWindow(uint8_t x, uint8_t y, uint8_t w, uint8_t h) : dim(x, y, w, h), next(0) { }

    void draw(void);

    void setPos(uint8_t x, uint8_t y) { dim.x = x; dim.y = y; }
    const SDimensions &getDimensions(void) const { return dim; }

    CWindow *nextWindow(void) const { return next; }
    void setNextWindow(CWindow *w) { next = w; }
};

#endif // WINDOW_H
