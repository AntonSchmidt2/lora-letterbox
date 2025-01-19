#include "display_handler.h"
#include "config.h"

DisplayHandler display;

void setup() {
    Serial.begin(115200); // Board communication
    Serial.println("Starting E-Paper Display Test");

    // Initialize display
    display.init();
    
    // Show test message
    display.showText("Hello E-Paper!");
    
    Serial.println("Text displayed. Check the screen!");
}

void loop() {
    // Empty loop - we just need to verify the display works
    delay(1000);
}