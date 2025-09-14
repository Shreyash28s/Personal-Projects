# Automated Parking Toll System

This project implements an **Arduino-based vehicle access control system** that simulates an automated parking toll gate. The system integrates sensors, actuators, and real-time monitoring to control access and ensure safety.

---

## Demo
🎥 [Watch the Demo on YouTube](https://youtu.be/Pv_plqxakyM?si=m8oi-l5YfwkzZAFX)

---

## Objectives
- Design and build an **IoT-style parking control system** with Arduino.  
- Automate barrier operations using sensors and servos.  
- Provide **auditory and visual feedback** (buzzers, LEDs) to drivers.  
- Enable **real-time monitoring** via serial communication with Linux terminal.  

---

## System Components
- **Arduino Uno** (microcontroller)  
- **Ultrasonic sensor** (vehicle detection & distance measurement)  
- **Servo motor** (barrier gate control)  
- **LEDs** (traffic light indicators for access approval/denial)  
- **Passive buzzer** (auditory alerts)  
- **Serial communication** (Minicom terminal integration)  

---

## Implementation Highlights
- **Event-driven logic**:  
  - Detects incoming vehicles with the ultrasonic sensor.  
  - Approves or denies entry based on system conditions.  
  - Operates servo to raise/lower gate.  
- **Feedback system**:  
  - Green LED + gate open = access approved.  
  - Red LED + buzzer = access denied.  
- **Safety features**:  
  - Time delays prevent abrupt barrier movements.  
  - Logs vehicle events through terminal for monitoring.  
- **Iterative enhancements**:  
  - Added traffic light sequence for smoother driver experience.  
  - Implemented persistent vehicle logs for system reliability.  

---

## Example Workflow
1. Vehicle approaches gate → ultrasonic sensor detects presence.  
2. System checks access conditions → green LED if approved, red LED if denied.  
3. Servo raises gate; buzzer signals entry or warning.  
4. After delay, gate lowers and system resets.  
5. All events are logged on Linux terminal via Minicom.  

---

## Tech Stack
- **Hardware:** Arduino Uno, Ultrasonic sensor, Servo motor, LEDs, Passive buzzer  
- **Software:** Arduino IDE, Minicom (Linux serial terminal)  
- **Concepts:** Event-driven programming, Hardware-software integration, IoT systems  

---

## Run Locally
1. Clone this repo:  
   ```bash
   git clone https://github.com/<your-username>/Personal-Projects.git
   cd Personal-Projects/Parking-Toll-System
