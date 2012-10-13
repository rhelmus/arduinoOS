#ifndef USBMOUSE_H
#define USBMOUSE_H

class HIDReportParser;

void initUSBMouse(void);
void setUSBMouseParser(HIDReportParser *p);
void updateUSBMouse(void);

#endif // USBMOUSE_H
