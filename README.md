# Motion Analysis Simulation

Motion detection and velocity estimation using an ultrasonic distance sensor, 
with planned integration of a Doppler radar (HB100) for true Doppler measurements.

This project demonstrates motion detection and basic velocity estimation 
using an ultrasonic distance sensor. The system measures distance changes 
over time to estimate movement direction and speed.

Features
--------

- Ultrasonic distance measurement (HC-SR04)
- Velocity estimation from distance change
- Direction detection (approaching / receding)
- LED motion indicator
- Bat-style audio feedback using a piezo buzzer
- 16x2 LCD real-time telemetry display
- Planned Doppler radar integration (HB100)

## Hardware Setup

<img width="738" height="322" alt="image" src="https://github.com/user-attachments/assets/20a807d7-6620-4360-8139-5ec82489d15b" />

## Circuit Diagram

<img width="1233" height="954" alt="image" src="https://github.com/user-attachments/assets/e8aea9a5-6e99-4f15-8d7e-51c21ceda779" />


## How it Works

1. The ultrasonic sensor measures the distance to an object.
2. The system compares the current distance with the previous measurement.
3. From the distance change and time difference, velocity is estimated.
4. LEDs indicate motion direction:
   - Blue LEDs → approaching
   - Red LEDs → receding
5. A piezo buzzer generates bat-like clicks based on distance.
6. The LCD displays distance, velocity and motion state.
