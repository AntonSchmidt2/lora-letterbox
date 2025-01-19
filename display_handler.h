#include <GxEPD2_BW.h>
#include <Fonts/FreeMonoBold9pt7b.h>  // Include the font definition

class DisplayHandler {
private:
    GxEPD2_BW<GxEPD2_290_T94_V2, GxEPD2_290_T94_V2::HEIGHT> display;

public:
    // Use the correct constructor for GxEPD2_BW
    DisplayHandler() : display(GxEPD2_290_T94_V2(EPAPER_CS, EPAPER_DC, EPAPER_RESET, EPAPER_BUSY)) {}

    void init() {
        display.init(115200, true, 2, false); // Initialize with baud rate and reset
        display.setRotation(DISPLAY_ROTATION);
        display.setTextColor(GxEPD_BLACK);
        display.setFont(&FreeMonoBold9pt7b); // Set the font
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
