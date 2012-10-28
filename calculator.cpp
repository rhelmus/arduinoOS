#include "calculator.h"

CCalculator::CCalculator()
{
    setSize(15, 11);

    resLabel.setDimensions(1, 1, 13, 1);
    addChild(&resLabel);

    uint8_t i = 0;
    for (uint8_t y=0; y<3; ++y)
    {
        for (uint8_t x=0; x<3; ++x, ++i)
        {
            numButtons[i].setPos(1 + (x * 3) , 1 + (y * 3));

            char s[2];
            itoa(9 - i, s, 10);
            numButtons[i].setText(/*s*/"0");

            addChild(&numButtons[i]);
        }
    }
}
