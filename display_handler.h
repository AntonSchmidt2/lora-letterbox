#include <GxEPD2_BW.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <config.h>

class DisplayHandler {
private:
    GxEPD2_BW<GxEPD2_290_T94_V2, GxEPD2_290_T94_V2::HEIGHT> display;

public:
    DisplayHandler() : display(GxEPD2_290_T94_V2(EPAPER_CS, EPAPER_DC, EPAPER_RESET, EPAPER_BUSY)) {}

    void init() {
        // Initialize GPIO pins first
        pinMode(EPAPER_CS, OUTPUT);
        pinMode(EPAPER_DC, OUTPUT);
        pinMode(EPAPER_RESET, OUTPUT);
        pinMode(EPAPER_BUSY, INPUT);
        pinMode(EPAPER_SCK, OUTPUT);
        pinMode(EPAPER_MOSI, OUTPUT);

        // Initialize SPI
        SPI.begin(EPAPER_SCK, -1, EPAPER_MOSI, EPAPER_CS);

        // Now initialize display
        Serial.println("Initializing display...");
        display.init(115200, true, 2, false);
        Serial.println("Display initialized");

        display.setRotation(DISPLAY_ROTATION);
        display.setTextColor(GxEPD_BLACK);
        display.setFont(&FreeMonoBold9pt7b);
    }

    void showText(const char* text, bool partial = true) {
        Serial.println("Starting to show text...");
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
        Serial.println("Text display completed");
    }

    void sleep() {
        display.powerOff();
    }
};