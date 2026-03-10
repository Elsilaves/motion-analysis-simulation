# Motion Analysis Simulation

This project is an embedded system demonstration that uses ultrasonic echolocation to detect motion and estimate velocity from distance changes, illustrating the fundamental principles behind Doppler-based motion sensing.

The system measures distance changes over time using an ultrasonic sensor to estimate movement direction and basic velocity, providing both visual and audio feedback.

Features
--------

- Ultrasonic distance measurement using HC-SR04
- Velocity estimation from distance changes
- Motion direction detection (approaching / receding)
- LED motion indicator
- Audio feedback using a piezo buzzer (pitch and click rate variation)
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
   - Blue LEDs → approaching (visual analogy to blueshift)
   - Red LEDs → receding (visual analogy to redshift)
5. A piezo buzzer provides audio feedback by generating clicks, where the pitch and click rate change with object distance.
6. The LCD displays distance, velocity and motion state.

## Theory

Echolocation is a sensing method in which a sound pulse is emitted and the reflected echo is used to detect objects. By measuring the time delay between the emitted pulse and the returning echo, the distance to the object can be determined. Ultrasonic sensors such as the HC-SR04 operate using this principle.

The Doppler effect describes how the frequency of a wave changes when there is relative motion between the source and the observer. When an object moves toward the observer, the observed frequency increases, while it decreases when the object moves away.

This project demonstrates motion detection using ultrasonic distance measurements. The system repeatedly measures the distance to an object and estimates velocity from the change in distance over time. By analyzing whether the measured distance is increasing or decreasing, the system determines whether the object is approaching or moving away.

Although the system does not directly measure Doppler frequency shifts, it illustrates the fundamental concept behind Doppler-based motion sensing: detecting motion by analyzing changes in reflected waves over time. In this implementation, motion is inferred from distance changes instead of frequency shifts, but the principle of determining movement direction remains similar.

Visual and audio feedback (LED indicators and buzzer signals) are used to represent approaching and receding motion, helping demonstrate the concept in an intuitive way. The system can be extended in future versions with microwave Doppler radar sensors that measure velocity directly from frequency shifts.

## Demo
