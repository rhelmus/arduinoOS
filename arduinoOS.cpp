#include "gui.h"
#include "usbmouse.h"

#include "Arduino.h"

void setup()
{
    Serial.begin(115200);
    Serial.println("Start");

    initUSBMouse();
    GUI.init();
}

void loop()
{
    updateUSBMouse();
}

