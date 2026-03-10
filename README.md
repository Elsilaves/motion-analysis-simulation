# Motion Analysis Simulation

Motion detection and velocity estimation using an ultrasonic distance sensor.

This project demonstrates motion detection and basic velocity estimation 
using an ultrasonic distance sensor. The system measures distance changes 
over time to estimate movement direction and speed.

Features
--------

- Ultrasonic distance measurement (HC-SR04)
- Velocity estimation from distance change
- Direction detection (approaching / receding)
- LED motion indicator
- Audio feedback using a piezo with pitch and click rate change
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
5. Piezo buzzer simulates echolocation sound by generating clicks, where the pitch and click rate change with object distance.
6. The LCD displays distance, velocity and motion state.

## Theory

Echolocation is a sensing method in which a sound pulse is emitted and the reflected echo is used to detect objects. By measuring the time delay between the emitted pulse and the returning echo, the distance to the object can be determined. Ultrasonic sensors such as the HC-SR04 operate using this principle.

The Doppler effect describes how the frequency of a wave changes when there is relative motion between the source and the observer. When an object moves toward the observer, the observed frequency increases, while it decreases when the object moves away.

In this project, motion is detected by repeatedly measuring distance using ultrasonic echolocation and estimating velocity from the change in distance over time. This allows the system to determine whether an object is approaching or receding. While the current implementation estimates motion from distance measurements, the concept relates to Doppler-based motion sensing and can be extended using microwave Doppler radar sensors.
