#ifndef UTILS_H
#define UTILS_H

uint16_t atxy(uint8_t x, uint8_t y);
inline int8_t convertPxToChar(int16_t px) { return px / 8; }

#endif // UTILS_H
