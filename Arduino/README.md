# IoT-Based Smart Dustbin

---

## Features

- **Lid Automation:** Automatically opens the lid when a user is detected.
- **Waste Level Detection:** Measures the fill level of the bin using ultrasonic sensors.
- **Real-Time Alerts:** Sends SMS notifications when the dustbin is full, using a GSM module.
- **Location Sharing (v2):** Includes GPS for location sharing.
- **LED Indicators (v2):** Provides visual feedback on the bin's status:
  - Green: Empty
  - Red: Full
- **Buzzer Alert (v2):** Alerts users when the bin is full and another person attempts to use it.

---

## Hardware Requirements

- **Microcontroller:** Arduino (e.g., Arduino Uno or Mega)
- **Ultrasonic Sensors:** HC-SR04 (for detecting the lid and waste levels)
- **Servo Motor:** SG90 or equivalent
- **GSM Module:** SIM800 or similar
- **GPS Module (v2):** NEO-6M or equivalent
- **LEDs (v2):** Green, Red
- **Buzzer (v2):** Piezoelectric buzzer
- **Power Supply:** 5V regulated power source
- **Miscellaneous:** Breadboard, jumper wires, resistors, etc.

---

## Libraries:
  - `Servo.h`
  - `SoftwareSerial.h`
  - `TinyGPSPlus.h` (for v2)

---

   **Circuit Diagram v1**
   


   **Circuit Diagram v2**



   **Hardware Assembly:**
   - Connect the ultrasonic sensors to the designated pins on the Arduino as per the circuit diagram of the selected version.
   - Position one ultrasonic sensor in front of the bin to detect humans and stick the second under the lid of the bin to measure trash level. 
   - Attach the servo motor to control the lid.
   - Connect the GSM module and GPS module (for v2).
   - Connect LEDs and buzzer to their respective pins.

   **Configuration:**

   - Replace the placeholder phone numbers (`+91xxxxxxxxxx`) in the code with the actual phone numbers for SMS notifications.

---

## File Version Descriptions

- **v1:**
  - Implements basic lid automation and waste level detection.
  - Sends SMS notifications when the bin is full.

- **v2:**
  - v1 with GPS integration for location-based alerts.
  - Adds LED and buzzer indicators for visual status.

---

## How It Works

1. **Automat Lid:**
   - The ultrasonic sensor detects the presence of a user near the bin.
   - The servo motor opens the lid when the user is detected.

2. **Detect Trash Level:**
   - The second ultrasonic sensor measures the trash level inside the bin.
   - Depending on the distance measured, the system classifies the bin status as empty, half full, or full.

3. **SMS Notifications:**
   - When the bin is full, an SMS is sent to the configured phone number.
   - In v2, the notification includes the bin’s GPS location.

4. **LED and Audio Alerts (v2):**
   - LEDs provide a quick visual indicator of the bin’s status.
   - The buzzer alerts users if the bin is full and another user approaches.
