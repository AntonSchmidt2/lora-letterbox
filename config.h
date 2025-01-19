#ifndef CONFIG_H
#define CONFIG_H

// E-Paper Display Connections for LILYGO T3-S3 (from pinmap)
#define EPAPER_DC       16    // EDP_DC_PIN IO16
#define EPAPER_CS       15    // EDP_CS_PIN IO15
#define EPAPER_SCK      4     // EDP_CLK_PIN IO04
#define EPAPER_MOSI     11    // EDP_MOSI_PIN IO11
#define EPAPER_RESET    47    // EDP_RESET_PIN IO47
#define EPAPER_BUSY     48    // EDP_BUSY_PIN IO48

// Display Settings
#define DISPLAY_WIDTH        250     // 2.9 inch display
#define DISPLAY_HEIGHT      122
#define DISPLAY_ROTATION     0

// Debug Settings
#define DEBUG_ENABLED        true
#define DEBUG_BAUDRATE      115200

// Board-specific pins
#define BOOT_BUTTON         0        // Boot button
#define RESET_BUTTON        21       // Reset button
#define LED_PIN            16        // LED shares pin with EPAPER_DC

#endif // CONFIG_H