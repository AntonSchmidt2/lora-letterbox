#ifndef CONFIG_H
#define CONFIG_H

// Pin Definitions for ESP32-S3
// LoRa SX1262 Connections
#define LORA_SCK        5     // SPI Clock
#define LORA_MISO       6     // SPI MISO
#define LORA_MOSI       7     // SPI MOSI
#define LORA_CS         10    // SPI Chip Select
#define LORA_RESET      9     // Reset pin
#define LORA_BUSY       8     // Busy pin
#define LORA_DIO1       4     // DIO1 pin for IRQ

// E-Paper Display Connections
#define EPAPER_SCK      13    // SPI Clock
#define EPAPER_MISO     12    // SPI MISO (might not be used by some displays)
#define EPAPER_MOSI     11    // SPI MOSI
#define EPAPER_CS       14    // SPI Chip Select
#define EPAPER_DC       15    // Data/Command control pin
#define EPAPER_RESET    16    // Reset pin
#define EPAPER_BUSY     17    // Busy pin

// Button & Power Management
#define WAKE_BUTTON     3     // RTC GPIO capable pin for wake-up
#define BATTERY_ADC     1     // Battery voltage monitoring (ADC pin)
#define POWER_ENABLE    2     // Optional power control for peripherals

// SPI Bus Configurations
#define LORA_SPI_FREQ   10000000  // 10 MHz
#define EPAPER_SPI_FREQ 10000000  // 10 MHz

// Time Slot Settings (in milliseconds)
#define TIME_SLOT_DURATION      60000   // 60 seconds
#define RECEIVE_WINDOW          2000    // 2 seconds
#define TRANSMIT_WINDOW         2000    // 2 seconds
#define SYNC_INTERVAL          300000   // 5 minutes between sync attempts

// LoRa Radio Settings
#define LORA_FREQUENCY         915.0f   // MHz (region-specific)
#define LORA_BANDWIDTH         125.0f   // kHz
#define LORA_SPREADING_FACTOR  7
#define LORA_CODING_RATE       5
#define LORA_TX_POWER         20        // dBm
#define LORA_PREAMBLE_LEN     8

// Wi-Fi AP Settings
#define AP_SSID              "LoRaMessenger_AP"
#define AP_PASSWORD          "messenger123"     // Change this!
#define AP_CHANNEL           1
#define AP_MAX_CONNECTIONS   1
#define AP_TIMEOUT_MS        60000    // 60 seconds timeout

// RTC Memory Structure (must be 32-bit aligned)
struct RTCData {
    uint32_t timeSlotCounter;    // Current time slot number
    uint32_t lastSyncTime;       // Last successful sync time
    bool messagePending;         // Message waiting to be sent
    char message[128];           // Message buffer
    uint8_t messageLength;       // Length of pending message
    uint8_t padding[3];         // Padding for 32-bit alignment
};

// Display Settings
#define MAX_MESSAGES_SHOWN    5
#define DISPLAY_WIDTH        296    // Change according to your display
#define DISPLAY_HEIGHT       128    // Change according to your display
#define DISPLAY_ROTATION     0

// Debug Settings
#define DEBUG_ENABLED        true
#define DEBUG_BAUDRATE      115200

// Battery Settings
#define BATTERY_MIN_VOLTAGE  3.3f    // Minimum operating voltage
#define BATTERY_MAX_VOLTAGE  4.2f    // Maximum battery voltage
#define BATTERY_WARN_LEVEL   20      // Warning level percentage

#endif // CONFIG_H
