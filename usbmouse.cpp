#include "usbmouse.h"

#include <avr/pgmspace.h>
#include <avrpins.h>
#include <max3421e.h>
#include <usbhost.h>
#include <usb_ch9.h>
#include <Usb.h>
#include <usbhub.h>
#include <avr/pgmspace.h>
#include <address.h>
#include <hidboot.h>
#include <printhex.h>
#include <message.h>
#include <hexdump.h>
#include <parsetools.h>

namespace
{
USB Usb;
HIDBoot<HID_PROTOCOL_MOUSE> Mouse(&Usb);
}

void initUSBMouse()
{
    if (Usb.Init() == -1)
        Serial.println("Failed to initialize USB!");

    delay(200);
}

void setUSBMouseParser(HIDReportParser *p)
{
    Mouse.SetReportParser(0, p);
}

void updateUSBMouse()
{
    Usb.Task();
}
