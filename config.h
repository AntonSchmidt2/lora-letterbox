#ifndef CONFIG_H
#define CONFIG_H

// E-Paper Display Connections
#define EPAPER_SCK      13    // SPI Clock
#define EPAPER_MISO     12    // SPI MISO (might not be used by some displays)
#define EPAPER_MOSI     11    // SPI MOSI
#define EPAPER_CS       14    // SPI Chip Select
#define EPAPER_DC       15    // Data/Command control pin
#define EPAPER_RESET    16    // Reset pin
#define EPAPER_BUSY     17    // Busy pin

// Display Settings
#define DISPLAY_WIDTH        296    // Change according to your display
#define DISPLAY_HEIGHT       128    // Change according to your display
#define DISPLAY_ROTATION     0      // 0 = normal, 1 = 90°, 2 = 180°, 3 = 270°

// Debug Settings
#define DEBUG_ENABLED        true
#define DEBUG_BAUDRATE      115200

#endif // CONFIG_H