# Firmware Over-the-Air (FOTA) Update System

## Table of Contents
- [Abstract](#abstract)
- [Introduction](#introduction)
  - [Project Overview](#project-overview)
  - [Objective](#objective)
  - [Document Structure](#document-structure)
- [IoT](#iot)
  - [GUI Tool in Node-RED](#gui-tool-in-node-red)
  - [MQTT Communication](#mqtt-communication)
  - [Mosquitto Broker](#mosquitto-broker)
  - [Challenges](#challenges)
- [ESP32 Dev Module](#esp32-dev-module)
  - [Handling Hex File in ESP32](#handling-hex-file-in-esp32)
- [ECU Bootloader](#ecu-bootloader)
- [V-model](#v-model)
- [Integration Test Plan](#integration-test-plan)
- [Low Level Design](#low-level-design)
- [Components Design](#components-design)

## Abstract

In our rapidly advancing technological era, embedded systems and the Internet of Things (IoT) have seamlessly intertwined with various aspects of our lives, ushering in a new wave of innovation...

## Introduction

### Project Overview
![Pasted image 20240310113956.png](https://github.com/Islam-Alabassy/FOTA.git/Pasted image 20240310113956.png)
  ![[Pasted image 20240310113956.png]]
*Figure 1: Project layout*  

The FOTA update system comprises a robust architecture with distinct components...

### Objective

The primary objective of this project is to enable the remote and secure updating of firmware in embedded systems...

### Document Structure

This documentation is structured to provide a comprehensive understanding of the FOTA update system...

## IoT
![[Pasted image 20240310114947.png]]
Figure 2: Relation between IoT components in details
### GUI Tool in Node-RED

The GUI tool in Node-RED provides users with a user-friendly interface for initiating the firmware update process...

### MQTT Communication

MQTT, a lightweight IoT protocol, seamlessly integrates with Node-RED for Over-The-Air (OTA) firmware updates on ESP8266...

### Mosquitto Broker

In the context of Over-The-Air (OTA) firmware updates, the Mosquitto MQTT broker plays a pivotal role...

### Challenges

#### NODE-RED

- **Limitation in Uploading Blocks**: The Upload node faces a limitation when attempting to send the entire hex file...

#### Mosquitto Broker

- **Connection Issues with Clients**: The MQTT-out node fails to connect to the broker...

## ESP32 Dev Module

### Handling Hex File in ESP32

One of the critical tasks performed by the ESP32 in our FOTA project is the handling of the hex file...

## ECU Bootloader

The ECU Bootloader plays a crucial role in the firmware update process...
![[Pasted image 20240310114331.png]]
Figure 3: ECU Bootloader State Diagram

### Bootloader state :

#### Step 1 :
It initialize the system clock  and peripherals (UART, GPIO,..,and SysTick).

#### Step 2 :
It checks the status of the system by comparing the ROM_Marker with the predefined states of the system ( NO_APP , APP1_Running , APP2_Running , APP1_request, and APP2_request).

#### Step 3:
 It takes a decission according to  the state of the system:

* NO_APP → The system will remain in the bootloader until receiving application for the first time.

* APP1-Running → The system will jump to application 1 and application 1 will work until receiving an       update request from the NodeMCU ESP82 .

* APP2-Running → The system will jump to application 2 and application 2 will work until receiving an update request from the NodeMCU ESP82 .

* APP1_request → The system will start communication with NodeMCU to receive new application and flashing it in the application 2 sectors then jumping to run application 2 if the flashing has been done successfully and let application 1 as a backup application but if the flashing is faild , the system will jump to run application 1 .

* APP2_request → The system will start communication with NodeMCU to receive new application and flashing it in the application 1 sectors then jumping to run application 1 if the flashing has been done successfully and let application 2 as a backup application but if the flashing is faild , the system will jump to run application 2.

### APP1_Running state :

The application 1 is running until receiving request from NodeMCU ESP82 .

After receiving request , it will change the ROM_Marker to APP1_request and jump to the bootloader .

### APP2_Running state :

The application 2 is running until receiving request from NodeMCU ESP82 .

After receiving request , it will change the ROM_Marker to APP2_request and jump to the bootloader .

![[Pasted image 20240310115128.png]]
Figure 4: Bootloader Flow chart

## V-model

The V-model is a crucial part of our project, outlining the steps from design to testing...

## Integration Test Plan

- Test GPIO With RCC
- Test Bootloader Code
- Test Server With ESP82

## Low Level Design

The low-level design section details the implementation of the system's components...

## Components Design

In low-level design, all functions are implemented as flow charts...

