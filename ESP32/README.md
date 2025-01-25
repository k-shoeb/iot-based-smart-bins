# IoT-Based Smart Bin using ESP32

---

## Features
- **Automatic Lid Operation**: The lid opens when a user is detected (within 35 cm).
- **Trash Level Monitoring**: Measures the trash level and categorizes it as `Empty`, `Low`, or `Full`.
- **Alerts**: Activates a buzzer when the bin is full to prevent overflow.
- **LED Indicators**:
  - Green LED: Indicates sufficient capacity.
  - Yellow LED: Indicates low capacity
  - Red LED: Indicates bin is full.
- **Arduino IoT Cloud**: Updates the bin status with the cloud for remote monitoring.

## Components
- ESP32 Microcontroller
- Ultrasonic Sensors(2):
  - Human Presence Detection
  - Trash Level Detection
- Servo Motor
- LEDs (Green, Yellow and Red)
- Buzzer
- Arduino IoT Cloud Account

  **Circuit Diagram**
  


## Setup
1. **Hardware Setup**:
   - Connect the components as per diagram.
   - Position one ultrasonic sensor in front of the bin to detect humans and stick the second under the lid of the bin to measure trash level. 
   - Attach the servo motor to the lid.
2. **Software Setup**:
   - Install the Arduino IDE.
   - Install the required libraries (`WiFi`, `thingProperties.h`, `ESP32Servo`).
   - Configure the `thingProperties.h` file for your Arduino IoT Cloud account.
   - Upload the provided code to the ESP32 board.
3. **Configuration**:
   - Update `binHeight` to match your bin's height (in cm).
   - Assign a unique `binId` for each smart bin if using multiple bins.
   - Update `SSID` and `Password` of your WiFi.
   - Configure the `DEVICE_LOGIN_NAME` and `DEVICE_KEY` from your Arduino IoT Cloud account.

## How It Works
1. **User Detection**: When a user approaches (distance < 35 cm), the lid opens automatically unless the bin is full.
2. **Trash Level Monitoring**: The bin periodically measures the trash level and updates the status:
   - `Empty`: Less than 50% filled.
   - `Low`: Between 50% and 90% filled.
   - `Full`: Over 90% filled.
3. **Alerts and Indicators**:
   - Activates a buzzer and red LED when the bin is full.
   - Syncs the status with the Arduino IoT Cloud.
