#include <display_handler.h>
#include <config.h>

DisplayHandler display;

void setup() {
    Serial.begin(115200);
    delay(1000);  // Give serial time to initialize
    
    Serial.println("\n\n=== Starting E-Paper Display Test ===");
    Serial.println("Starting initialization...");

    // Initialize display
    display.init();
    
    // Show test message
    Serial.println("Showing test message...");
    display.showText("Hello E-Paper!");
    
    Serial.println("Setup complete!");
}

void loop() {
    delay(1000);
}