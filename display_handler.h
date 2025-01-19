#include <GxEPD2_BW.h>
#include <Fonts/FreeMonoBold9pt7b.h>  // Include the font definition
#include <config.h>


class DisplayHandler {
private:
    GxEPD2_BW<GxEPD2_290_T94_V2, GxEPD2_290_T94_V2::HEIGHT> display;

public:
    // Use the correct constructor for GxEPD2_BW
    DisplayHandler() : display(GxEPD2_290_T94_V2(EPAPER_CS, EPAPER_DC, EPAPER_RESET, EPAPER_BUSY)) {}

    void init() {
      // Initialize SPI first
      pinMode(-1, INPUT_PULLUP);  // MISO pin is not used but set to pullup
      SPI.begin(EPAPER_SCK, -1, EPAPER_MOSI, EPAPER_CS);

      // Rest of the initialization
      display.init(115200, true, 2, false);
      display.setRotation(DISPLAY_ROTATION);
      display.setTextColor(GxEPD_BLACK);
      display.setFont(&FreeMonoBold9pt7b);
    }

    void showText(const char* text, bool partial = true) {
      Serial.println("Starting display update...");
      if (partial) {
          Serial.println("Using partial update");
          display.setPartialWindow(0, 0, display.width(), display.height());
      } else {
          Serial.println("Using full update");
          display.setFullWindow();
      }

      display.firstPage();
      do {
          display.fillScreen(GxEPD_WHITE);
          display.setCursor(10, 30);
          display.print(text);
      } while (display.nextPage());
      Serial.println("Display update complete");
    }

    void sleep() {
        display.powerOff();
    }
};
