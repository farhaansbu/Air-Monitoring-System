# Air Monitoring System

This repository contains the firmware and hardware interface code for an Air Monitoring System built with an AVR128DB48 microcontroller, Sensirion SCD41 sensor, and SparkFun 20x4 SerLDC screen.

## Project Overview

The Air Monitoring System captures real-time environmental data, including CO₂ concentration, temperature, and humidity. It utilizes embedded C firmware to interface with the Sensirion SCD41 sensor via the I²C protocol and employs SPI communication to display sensor readings on a SerLCD screen.

## Features

- Real-time data acquisition of CO₂, temperature, and humidity with 90% data reliability
- Embedded firmware developed in C for precise sensor interfacing and control
- SPI-based display integration for accessible user monitoring


## Technologies Used

- AVR128DB48 microcontroller
- Sensirion SCD41 sensor
- Communication protocols: I²C, SPI
- Firmware written in C
- Tools: Doxygen for documentation

## Getting Started

1. Connect Sensirion SCD41 sensor to AVR128DB48 microcontroller via I²C.
2. Connect SerLCD to the AVR128DB48 for SPI communication.
3. Upload the embedded C firmware to the microcontroller using compatible programming tools (Microchip Studio).
4. Monitor sensor data live on the display.

