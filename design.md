# LoRa E-Paper Messenger with On-Demand Wi-Fi AP  
**Software Design Documentation**

## 1. Overview
The **LoRa E-Paper Messenger** is a low-power, asynchronous messaging system built around two ESP32-S3 microcontrollers with LoRa SX1262 modules and E-Paper displays. Each device can exchange messages via LoRa and display them on the E-Paper. To keep power consumption low, both devices spend most of their time in **deep sleep** and only wake during synchronized time slots to **receive** or **transmit** messages.

A unique feature of this design is the **on-demand Wi-Fi Access Point**: when a user wants to compose or edit messages more conveniently than using onboard buttons, pressing a dedicated button **wakes** the ESP32, **enables Wi-Fi**, and serves an HTML interface. Users can connect via their smartphone or laptop to type messages directly, after which the system returns to deep sleep.

---

## 2. Hardware Components

1. **ESP32-S3 Microcontroller**  
   - Deep Sleep capability (~10µA)  
   - Integrated Wi-Fi/BLE (for AP mode)  
   - RTC memory for state persistence  
2. **SX1262 LoRa Module**  
   - Long-range, low-power radio communication  
   - Programmable transmit power, bandwidth, etc.  
3. **E-Paper Display**  
   - Ultra-low power (only draws current when updating)  
   - Retains image without power  
4. **LiPo Battery** (e.g., 2000 mAh)  
   - Supplies the system with autonomy of several days  
5. **Button(s)**  
   - **Wake/Mode Button**: Wakes the device from deep sleep and optionally triggers Wi-Fi AP mode  
   - (Optional) Additional buttons for simple navigation or partial text entry  
6. **Power Management Circuitry**  
   - Onboard regulators  
   - Possibly MOSFET switches to fully cut power to the LoRa or E-Paper modules during sleep

---

## 3. Power Management

### 3.1 Power States

1. **Deep Sleep Mode** (~10–50µA total system, depending on circuitry)
   - ESP32-S3 in deep sleep with RTC timer or GPIO wake-up  
   - LoRa module powered off (or in sleep mode)  
   - E-Paper powered off or in deep sleep mode  
   - RTC memory is maintained  
   - Wake sources:
     - RTC timer (for synchronized LoRa receive/transmit window)  
     - **Wake/Mode Button** (for user input / Wi-Fi AP launch)

2. **Active Mode** (~80–120mA depending on radio usage and E-Paper updates)
   - Full microcontroller awake  
   - LoRa module can be enabled for TX/RX  
   - E-Paper updates occur here  
   - Typically used during short bursts (1–4s) for LoRa comms or user interactions

3. **Wi-Fi AP Mode** (~150–200mA)
   - ESP32-S3 with Wi-Fi active (Access Point)  
   - Device hosts a simple web server for user input  
   - Typically runs for user-defined timeout (e.g., 30–60s) to allow message entry  
   - After inactivity, transitions back to deep sleep

---

## 4. Synchronized Time Slot System

To exchange messages asynchronously and save power, each device follows a timed cycle:

```
60-second cycle example:
0-2s:   Receive Window (LoRa RX)
2-4s:   Transmit Window (LoRa TX if any message is queued)
4-60s:  Deep Sleep
```

- Each device wakes up exactly at the start of the cycle to **listen** for incoming messages in the first 2 seconds.
- If a message is pending in RTC memory, the device uses the **Transmit Window** (seconds 2–4) to send it.
- After the time slot, the device returns to deep sleep for the remainder of the cycle.

> **Note**: The 60-second cycle is an example. You can adjust the interval and window lengths (e.g., 2–5 minutes) to further reduce power at the expense of message latency.

### 4.1 Time Synchronization & Drift
- The internal RTC can drift over time. Use **periodic resync**: each device can send a small “timestamp” or sync packet in a known slot, and the other device adjusts its timer as needed.  
- If a device misses multiple wake-ups or resets, it can attempt a **fast resync** or fallback to a secondary method (e.g., button-initiated Wi-Fi sync, if available).

---

## 5. Software Architecture

```
LoRaMessenger/
├── LoRaMessenger.ino      # Main program entry
├── config.h               # Configuration constants & pin definitions
├── rtc_memory.h           # RTC memory management
├── radio_handler.h        # LoRa operations (TX/RX, power modes)
├── display_handler.h      # E-Paper control & rendering
├── power_manager.h        # Power state transitions (sleep, wake)
└── wifi_ap_handler.h      # On-demand Wi-Fi AP & web server
```

### 5.1 Core Components

1. **Power Manager**  
   - Schedules wake/sleep intervals  
   - Chooses power state transitions based on events (RTC timer, button interrupt, etc.)  
   - Controls peripheral power domains if possible

2. **Radio Handler (LoRa)**  
   - Initializes the SX1262  
   - Handles receiving and transmitting messages in each slot  
   - Manages frequency, spreading factor, bandwidth, coding rate, etc.

3. **Display Handler**  
   - Initializes and updates the E-Paper display  
   - Supports partial and full refresh modes  
   - Renders new messages and status indicators  
   - Minimizes refresh time to reduce power consumption

4. **RTC Memory Manager**  
   - Stores essential data during deep sleep, e.g.:  
     ```cpp
     struct RTCData {
         uint32_t timeSlotCounter;  
         bool messagePending;  
         char message[128];  
         uint8_t messageLength;  
         uint32_t lastSyncTime;  
     };
     ```  
   - Ensures the device can resume correct state after wake-up

5. **Wi-Fi AP Handler**  
   - Initializes Wi-Fi in AP mode on demand  
   - Hosts a minimal web server to collect user input (HTML form)  
   - Persists the user-entered text in RTC memory or a queue for transmission  
   - Times out after N seconds of inactivity and returns the ESP32 to deep sleep

---

## 6. New: On-Demand Wi-Fi AP Flow

This is the primary user interface flow for entering new messages:

1. **User Presses Button**  
   - A dedicated **Wake/Mode Button** is tied to an RTC GPIO interrupt.  
   - ESP32 transitions from deep sleep to active mode.

2. **Enable Wi-Fi AP**  
   - If the user holds or double-presses the button (configurable logic), the firmware starts a Wi-Fi Access Point.  
   - The device obtains a default IP (e.g., 192.168.4.1) and starts a small HTTP server.

3. **User Connects**  
   - From a phone or laptop, the user connects to the AP (“LoRaMessenger_AP”).  
   - The user navigates to `http://192.168.4.1/` in a browser to load a simple webpage.

4. **HTML Form Entry**  
   - The webpage contains a text field for typing the message, plus a “Send” button.  
   - Once submitted, the ESP32 stores the message in RTC memory (e.g., `message[]`) and sets `messagePending = true`.

5. **Closing the AP**  
   - The device either:
     - **Automatically times out** after X seconds of inactivity (e.g., 30–60s).  
     - Or the user can press the button again to stop the AP.  
   - The firmware shuts down Wi-Fi and transitions to deep sleep.

6. **Next LoRa Transmit**  
   - When the next synchronized time slot arrives, the device wakes up, checks `messagePending`, and transmits the stored message via LoRa.

> **Power Note**: While Wi-Fi AP is active, current can spike to 150–200mA. To preserve battery, keep the AP session short.

---

## 7. Typical Operation Flows

### 7.1 Normal Time Slot (Receive/Transmit)
1. **Wake (RTC Timer)**  
   - Wake at the start of a 60-second cycle.  
2. **LoRa Receive Window (0–2s)**  
   - Power up LoRa, listen for incoming messages.  
   - If received, parse and store them; optionally update the display.  
3. **LoRa Transmit Window (2–4s)**  
   - If `messagePending == true`, transmit the buffered message.  
   - Wait for optional ACK if implemented.  
4. **Return to Deep Sleep (4s–end)**  
   - Stop LoRa, power down display if needed.  
   - Sleep until the next cycle.

### 7.2 User Input via AP
1. **Button Press**  
   - Immediately wakes the ESP32.  
   - If the user triggers “AP Mode,” start Wi-Fi AP.  
2. **User Connects & Types**  
   - The user accesses the webpage, enters text, and clicks send.  
   - ESP32 saves the message to RTC memory.  
3. **AP Shutdown**  
   - After inactivity or user action, Wi-Fi is turned off.  
   - Device returns to deep sleep.  
4. **LoRa Transmission**  
   - At the next scheduled time slot, the device transmits the new message.

---

## 8. Configuration Parameters

```cpp
// Time Slot Settings
#define TIME_SLOT_DURATION      60000  // 60 seconds
#define RECEIVE_WINDOW          2000   // 2 seconds
#define TRANSMIT_WINDOW         2000   // 2 seconds

// LoRa Configuration
#define LORA_FREQUENCY         915.0f  // MHz (region-specific)
#define LORA_BANDWIDTH         125.0f  // kHz
#define LORA_SPREADING_FACTOR  7
#define LORA_CODING_RATE       5

// E-Paper Settings
#define DISPLAY_REFRESH_PARTIAL true
#define MAX_MESSAGES_SHOWN      5

// Wi-Fi AP Settings
#define AP_SSID                 "LoRaMessenger_AP"
#define AP_PASSWORD             ""      // or some password
#define AP_TIMEOUT_MS           60000   // 60s inactivity before AP off

// Button / Power
#define BUTTON_PIN              10      // Example
#define SLEEP_TIMEOUT_MS        5000    // After normal slot, go back to sleep
```

---

## 9. Power Consumption Estimates

1. **Deep Sleep**: ~10–50µA (depending on quiescent currents of regulators and modules)  
2. **LoRa Active**: ~80–120mA during 2–4s windows (depending on TX power and E-Paper usage)  
3. **Wi-Fi AP Active**: ~150–200mA, typically for 30–60s when a user is entering messages  

### Example Calculation (60-Second Cycle, No AP Use)

- **Active Time**: 4s (6.67% of the cycle)  
- **Sleep Time**: 56s (93.33%)  
- **Estimated Average Current**: ~18mA (including overhead)  
- **Battery Life (2000mAh)**: ~4.6 days  

> Actual results vary based on how often the Wi-Fi AP is used. Each AP session significantly increases average current but for a short duration. If used rarely, it won’t drastically impact total battery life.

---

## 10. Error Handling & Reliability

1. **Missed Windows**  
   - Devices may get out of sync due to drift or missed wake-ups. Implement a resync mechanism.  
2. **Message Acknowledgments**  
   - Optional handshake ensures reliable delivery. If ACK not received, requeue message.  
3. **User Experience**  
   - If the user tries to connect to the AP while the device is in deep sleep, they must press the button again.  
   - Document these “press for AP” instructions for clarity.

---

## 11. Future Enhancements

1. **Encryption**  
   - Implement AES encryption for LoRa payloads if security is needed.  
2. **Adaptive Time Slots**  
   - Increase or decrease cycle length based on traffic or battery level.  
3. **Enhanced Web UI**  
   - Add message history, device status (battery level, last received message, etc.)  
4. **BLE Alternative**  
   - Instead of Wi-Fi, a user might prefer BLE for phone connectivity.  

---

## 12. Summary

1. **Synchronized LoRa Windows** for low-power, asynchronous messaging.  
2. **Deep Sleep** dominates the device’s lifecycle, ensuring long battery life.  
3. **On-Demand Wi-Fi AP** for convenient message entry via a smartphone or laptop.  
4. **Button-Based Wake** to avoid continuous radio listening, minimizing idle current.  
5. **RTC Memory** persists message queues and time slot counters between sleep cycles.  

This design provides a versatile, low-power communication system that balances usability (via a simple HTML interface) with energy efficiency (deep sleep plus short LoRa windows).