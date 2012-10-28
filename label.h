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
    CLabel(void) : text(0), flashText(false), alignment(ALIGN_LEFT) { setHeight(1); }

    void setAlignment(EAlignment a);
    void setText(const char *t, bool aw=true, bool ft=false);
};

#endif // LABEL_H
