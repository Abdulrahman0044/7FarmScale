# Soil Monitoring System

## Overview
This project involves the development of a soil monitoring system using ESP32 and a variety of sensors to measure parameters like Nitrogen, Phosphorus, Potassium, Humidity, Temperature, Conductivity, pH, and Moisture. The system displays these values on an LCD screen and sends the data to a remote server for further processing. This setup is designed for agricultural applications, allowing farmers to monitor soil conditions in real-time.

---

## Features
- **Real-Time Monitoring**: Measures soil parameters such as NPK (Nitrogen, Phosphorus, Potassium), pH, temperature, humidity, and electrical conductivity.
- **LCD Display**: Displays the real-time values of the soil parameters on a 16x4 LCD screen.
- **Wireless Connectivity**: Connects to WiFi to send data to a remote server.
- **Data Logging**: Sends the collected data to a server for further analysis or logging.
- **Random Data Generation**: In case the actual values are not available, random values for NPK can be generated and displayed.
  
---

## Hardware Required
- **ESP32 Dev Board**
- **7-in-1 Soil Sensor** (Measures NPK, pH, Temperature, EC, etc.)
- **16x4 LCD Display (I2C)**
- **Relay Module**
- **WiFi Connection**
- **Jumper Wires**
- **Power Supply (e.g., 5V USB adapter)**

---

## Software Required
- **Arduino IDE**: For programming the ESP32 and uploading the code.
- **WiFi Library**: For handling the WiFi connection.
- **LiquidCrystal_I2C Library**: For controlling the I2C LCD display.
- **hd44780 Library**: For advanced LCD functionalities.
- **HTTPClient Library**: To send data to a remote server.

---

## Setup Instructions

1. **Install the Required Libraries**:
   - LiquidCrystal_I2C
   - hd44780
   - HTTPClient
   - WiFi

   You can install these libraries through the Arduino IDE's Library Manager.

2. **Wire the Hardware**:
   - Connect the soil sensor to the ESP32 according to your sensor's pinout.
   - Connect the LCD display using I2C pins (SDA, SCL) to the ESP32.
   - Connect the relay module to control the sensor reading switch.

3. **Update WiFi Credentials**:
   - In the code, replace the `ssid` and `password` with your WiFi network credentials.
   ```cpp
   const char* ssid = "YOUR_WIFI_SSID"; 
   const char* password = "YOUR_WIFI_PASSWORD";

