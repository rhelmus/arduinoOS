#ifndef LABEL_H
#define LABEL_H

#include "widget.h"

class CLabel : public CWidget
{
public:
    enum EAlignment { ALIGN_LEFT, ALIGN_CENTER, ALIGN_RIGHT };

private:
    const char *text;
    bool flashText;
    EAlignment alignment;

    virtual void coreDraw(void);

public:
    CLabel(uint8_t x, uint8_t y, const char *t, bool ft=false);
    CLabel(uint8_t x, uint8_t y, uint8_t w, const char *t, EAlignment a=ALIGN_CENTER,
           bool ft=false) : CWidget(x, y, w, 1), text(t), flashText(ft), alignment(a) { }

    void setText(const char *t, bool ft=false);
};

#endif // LABEL_H
