# 🍇 Smart Wine Cellar Monitoring System

[![Project Status](https://img.shields.io/badge/status-active-success.svg)]()
[![License](https://img.shields.io/badge/license-MIT-blue.svg)]()

## Overview

A sophisticated Arduino-based environmental monitoring system designed for traditional wine cellars transitioning into the digital age. This system provides real-time monitoring of critical factors affecting wine quality: **light intensity**, **temperature**, and **humidity**.

## Features

### Core Functionality
- Real-time environmental monitoring
- Visual and audio alerts for critical conditions
- LCD display for data visualization
- Remote monitoring capabilities

### Technical Specifications
- **Display**: 16x2 LCD with sequential data presentation
  - Temperature status (Low / Optimal / High)
  - Humidity levels (Low / Optimal / High)
  - Light conditions (Dark / Moderate / Bright)

- **Visual Indicators**
  - Green LED: Optimal conditions
  - Yellow LED: Moderate light or temperature deviation
  - Red LED: Excessive light or humidity issues

- **Audio Alerts**
  - Buzzer activation for critical conditions

- **Data Processing**
  - 5-reading moving average for stable measurements
  - Enhanced accuracy through data smoothing

## Hardware Requirements

- Arduino UNO
- TMP36 Temperature Sensor
- Potentiometer (Humidity Simulation)
- LDR Light Sensor
- 16x2 LCD Display
- RGB LED Indicators
- Active Buzzer

## Development Team

- Everson Pereira
- [Your Team Members]

## Contributing

We welcome contributions! Please feel free to submit issues and pull requests.

## License

This project is licensed under the MIT License - see the LICENSE file for details.

---

*Built with ❤️ for wine enthusiasts*
