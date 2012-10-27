#ifndef UTILS_H
#define UTILS_H

uint16_t atxy(uint8_t x, uint8_t y);
void putstr(uint8_t x, uint8_t y, const char *s);
inline int8_t convertPxToChar(int16_t px) { return px / 8; }
inline uint16_t center(uint16_t pos, uint16_t width, uint16_t totw)
{ return pos + ((totw - width) / 2); }

#endif // UTILS_H
