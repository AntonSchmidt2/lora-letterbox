#include <display_handler.h>
#include <config.h>

DisplayHandler display;

void setup() {
    Serial.begin(115200);
    Serial.println("=== Starting Program ==="); // Clear marker for program start

    Serial.println("Initializing display...");
    display.init();
    Serial.println("Display initialized");
    
    Serial.println("Showing test message...");
    display.showText("Hello E-Paper!");
    Serial.println("Text display command sent");
    
    Serial.println("Setup complete!");
}

void loop() {
    // Empty loop - we just need to verify the display works
    delay(1000);
}