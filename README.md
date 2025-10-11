# guard-sentry
# Arduino Home Alarm System with Raspberry Pi & Firebase Backend

## Overview

A modular, cloud-connected home alarm system built with an Arduino Uno (sensors, LCD), Raspberry Pi (IoT backend), Firebase Realtime Database, and SMS/Text alert functionality using Firebase Cloud Functions and Twilio.

- Armed/disarmed state with pushbutton
- Intruder alert when 2+ sensors are triggered
- LCD shows status & alerts
- SMS notification to owner via Twilio API
- Easy extensibility (add/replace sensors, integrate camera, swap backend)

---


## Hardware

- Arduino Uno x1 (can be extended to multiple boards)
- HW416B PIR motion sensor
- Sound sensor module
- IR sensor module
- LDR (Light sensor + voltage divider)
- LCD I2C 16x2 or 20x4
- Buzzer and LED
- Pushbutton for arming/disarming
- Raspberry Pi (any model with USB, Python 3)
- Optional: OV7670 camera (see modular instructions)

---

## Wiring

- Sensors to digital/analog pins as described in `main.ino`
- LCD to A4 (SDA), A5 (SCL)
- Buzzer/LED to D6/D7 (with 220Ω resistor)
- Button for arming to D4 (active LOW)
- Arduino TX (Serial) to Pi USB, GND shared



---

## Software

**Arduino (`main.ino`)**
- Reads sensor states
- Armed/unarmed toggle
- Sends sensor/alert status to Pi via Serial
- LCD status/alerts

**Raspberry Pi Python (`main.py`)**
- Reads Arduino serial state
- Updates Firebase Realtime Database
- Logs intruder alerts

**Firebase Cloud Function (`index.js`)**
- Sends SMS texts to owner via Twilio when intruder detected
- Can be extended for email/push/other alerts

---

---

## Alert Workflow

- System is armed via button.
- If 2 or more sensors are triggered:
- Arduino sets `intruderAlert = true`
- LCD displays "INTRUDER ALERT!"
- Buzzer and LED are activated
- Serial sends state to Pi
- Pi pushes event to Firebase DB
- Cloud Function sends SMS to owner

---

## Modularity

- Camera logic, additional sensors or outputs can be added independently
- Easily replace or extend backend (other alerts, dashboards, etc.)

---

## Dependencies

- Arduino IDE (LiquidCrystal_I2C library)
- Python 3 (pyserial, firebase-admin)
- Node.js 18+ (firebase-functions, firebase-admin, twilio)
- Firebase CLI (`npm install -g firebase-tools`)


## License

MIT

---

## Credits

Project by Shivansh Alwadhi


### 2. Raspberry Pi
- Install Python 3, pip packages:
