# Smart Home Automation System with ESP32 & Arduino Uno

![Smart Home Project Demo](https://via.placeholder.com/600x300.png?text=Your+Project+Demo+GIF/Video+Here!)

## 📝 Table of Contents
* [Short Description](#short-description)
* [Key Features](#key-features)
* [Technologies & Hardware Used](#technologies--hardware-used)
* [Setup & Configuration](#setup--configuration)
  * [Prerequisites](#prerequisites)
  * [Hardware Connections](#hardware-connections)
  * [Blynk App Setup](#blynk-app-setup)
  * [Telegram Bot Setup](#telegram-bot-setup)
* [Author](#author)

## Short Description

This project is a comprehensive Smart Home Automation system built using an ESP32 and an Arduino Uno. It features remote control and monitoring via the Blynk mobile app and Telegram notifications for critical events. The system manages environmental conditions (temperature, humidity, air quality, rain detection), controls windows, a fan, RGB lighting, a bathroom light, and includes a keypad-based secure door lock system.

This project was developed as a demonstration of IoT capabilities, sensor integration, microcontroller programming, and remote system management.

## Key Features

**ESP32 Module (Networked Control & Monitoring):**
* **Remote Control via Blynk App:**
    * Individually control left and right windows (servo-operated).
    * Turn a DC fan ON/OFF.
    * Control RGB LED colors and state (ON/OFF, Yellow, Green, Red).
    * Toggle a bathroom LED ON/OFF.
* **Real-time Sensor Monitoring (via Blynk & Serial):**
    * Temperature and Humidity (DHT11).
    * Air Quality/Gas Concentration (MQ2 Sensor).
    * Rain Detection.
* **Automated Actions:**
    * **High Temperature:** Automatically turns on the fan if the temperature exceeds 34°C.
    * **Gas Leak Detection:** If MQ2 sensor detects dangerous gas levels:
        * Sends urgent warning notifications via Telegram.
        * Activates a local warning LED and buzzer.
        * Opens windows and turns on the fan for ventilation.
    * **Rain Detection:** If rain is detected:
        * Sends a notification via Telegram.
        * Automatically closes windows.
* **Telegram Notifications:**
    * System online status.
    * Critical gas alerts.
    * Rain detection alerts and window status.

**Arduino Uno Module (Local Access Control):**
* **Keypad Door Lock System:**
    * Secure PIN-based door access using a 4x4 keypad.
    * Servo-controlled door mechanism (open/close).
* **LCD Display:**
    * Provides user feedback ("Welcome!", "Enter Pin:", "Correct Pin!", "Incorrect!").
    * Displays entered PIN digits.
    * Timeout feature resets PIN entry and secures the door.


## Technologies & Hardware Used

**Hardware Components:**
* ESP32 Development Board
* Arduino Uno R3
* DHT11 Temperature & Humidity Sensor
* MQ2 Gas Sensor Module
* Rain Drop Sensor Module
* Servo Motors (e.g., SG90 or MG996R for windows and door)
* DC Fan (and L298N Motor Driver or similar)
* Common Cathode/Anode RGB LED
* Standard LEDs (for warning and bathroom light)
* Buzzer
* 4x4 Matrix Keypad
* 16x2 I2C LCD Display Module
* Resistors, Jumper Wires, Breadboards
* Power Supplies for ESP32 and Arduino Uno

**Software, Cloud Services & Libraries:**
* **Programming Language:** C/C++ (Arduino Framework)
* **IDE:** Arduino IDE or PlatformIO
* **Cloud Platform:** Blynk IoT (for app interface and cloud communication)
* **Messaging Service:** Telegram Bot API (for notifications)
* **Key ESP32 Libraries:**
    * `WiFi.h`
    * `BlynkSimpleEsp32.h`
    * `UniversalTelegramBot.h`
    * `ESP32Servo.h`
    * `DHT.h`
* **Key Arduino Uno Libraries:**
    * `LiquidCrystal_I2C.h`
    * `Keypad.h`
    * `Servo.h`

## Setup & Configuration

### Prerequisites
1.  **Arduino IDE:** Install the latest version of the Arduino IDE.
2.  **Board Support:**
    * Add ESP32 board support to your Arduino IDE.
    * Ensure Arduino Uno is selectable.
3.  **Libraries:** Install all the libraries mentioned above via the Arduino Library Manager or by adding them manually. *Refer to each library's documentation for specific installation instructions if needed.*

### Hardware Connections
* Connect all sensors, actuators, LCD, keypad, and modules to the ESP32 and Arduino Uno as per the pin definitions in their respective `.ino` files.
* **(Recommended):** For clarity, consider adding a Fritzing diagram, a simple circuit schematic image, or a table detailing the pin connections (e.g., `DHT11 Data Pin -> ESP32 GPIO XX`) in a separate `WIRING.md` file or a sub-section here. This is especially helpful for complex projects.

### Blynk App Setup
1.  Create a new project in the Blynk app.
2.  Add the required widgets (buttons for windows, fan, lights; labeled value displays for sensors) and assign them to the virtual pins (V0-V4, V6-V8) as defined in the ESP32 code.
3.  Your Blynk Auth Token will be sent to your email; use this in your local `credentials.h`.

### Telegram Bot Setup
1.  Create a new bot by talking to the "BotFather" on Telegram.
2.  Get your Chat ID. You can do this by:
    * Sending a message to your bot and then accessing `https://api.telegram.org/botYOUR_TELEGRAM_BOT_TOKEN/getUpdates` (replace `YOUR_TELEGRAM_BOT_TOKEN` with your actual token). Look for the `chat` object and its `id`.
    * Alternatively, message a bot like `@RawDataBot` or `@userinfobot` to get your user ID (if sending notifications to yourself) or a group ID. Add this to your local `credentials.h`.

## Author
* **HONESTO E. VICENTE**
* GitHub: [hozestooo](https://github.com/hozestooo)
