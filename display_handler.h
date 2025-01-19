#ifndef DISPLAY_HANDLER_H
#define DISPLAY_HANDLER_H

#include <GxEPD2_BW.h>  // E-Paper display library
#include "config.h"

class DisplayHandler {
private:
    // Choose the correct display class for your display
    // This example uses a 2.9" b/w display (296x128)
    GxEPD2_BW<GxEPD2_290_T94_V2, GxEPD2_290_T94_V2::HEIGHT> display;

public:
    DisplayHandler() : display(
        EPAPER_CS,     // CS pin
        EPAPER_DC,     // DC pin
        EPAPER_RESET,  // Reset pin
        EPAPER_BUSY    // Busy pin
    ) {}

    void init() {
        display.init(115200, true, 2, false); // Init with reset
        display.setRotation(DISPLAY_ROTATION);
        display.setTextColor(GxEPD_BLACK);
        display.setFont(&FreeMonoBold9pt7b);
    }

    void showText(const char* text, bool partial = true) {
        if (partial) {
            display.setPartialWindow(0, 0, display.width(), display.height());
        } else {
            display.setFullWindow();
        }
        
        display.firstPage();
        do {
            display.fillScreen(GxEPD_WHITE);
            display.setCursor(10, 30);
            display.print(text);
        } while (display.nextPage());
    }

    void sleep() {
        display.powerOff();
    }
};

#endif
