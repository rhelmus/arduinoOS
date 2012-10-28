#ifndef CALCULATOR_H
#define CALCULATOR_H

#include "button.h"
#include "label.h"
#include "window.h"


class CCalculator : public CWindow
{
    CLabel resLabel;
    CButton numButtons[10];

public:
    CCalculator(void);
};

#endif // CALCULATOR_H
