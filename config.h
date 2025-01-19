#ifndef CONFIG_H
#define CONFIG_H

// E-Paper Display Connections for LILYGO T3-S3
#define EPAPER_BUSY     14    // BUSY Signal
#define EPAPER_RESET    21    // RESET Control
#define EPAPER_DC       7     // Data/Command Control
#define EPAPER_CS       6     // Chip Select
#define EPAPER_SCK      18    // SPI Clock
#define EPAPER_MOSI     17    // SPI MOSI
#define EPAPER_MISO     -1    // Not used for most E-paper displays

// Display Settings
// For 2.9 inch display. Adjust if you have a different size
#define DISPLAY_WIDTH        296
#define DISPLAY_HEIGHT      128
#define DISPLAY_ROTATION     0

// Debug Settings
#define DEBUG_ENABLED        true
#define DEBUG_BAUDRATE      115200

// Board-specific extra pins
#define BUTTON_1            0     // Boot Button
#define BUTTON_2            21    // User Button (shares with RESET)
#define LED_PIN            16     // Onboard LED
#define BAT_ADC            4     // Battery monitoring ADC pin

#endif // CONFIG_H