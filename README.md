<meta name="google-site-verification" content="ku4yMglGLmTthuafIARrGHCC-FFy13kET4uCa3HkqAs" />

## ESPRanger, LoRa GNSS-GPS IMU RS485 Wide Operating Voltage 5-60V SDK

**Supporting STEM Electronic Internet of Things & Home Automation Technology for Smart Energy Monitoring**

[**For Latest Project Updates - Click Here**](https://github.com/DitroniX/ESPRanger-LoRa-GNSS-GPS-IMU-RS485-60V?tab=readme-ov-file#updates)

![ESPRanger Preview](https://github.com/DitroniX/ESPRanger-LoRa-GNSS-GPS-IMU-RS485-60V/blob/main/Datasheets%20and%20Information/ESPRanger%20-%20Features.png?raw=true)

## Updates

-   25-09-25    - Preliminary Information
-   25-09  - Alpha Testing, Schematics and Layout Topology

## Availability

**ESPRanger** will be beta testing late September and will be available to purchase in October 2025.

## Feature Overview

### **What is ESPRanger?**

**ESPRanger** is a compact and powerful STEM Community board which is designed for LoRa (Long Range wireless communication), with GNSS (Global Navigation Satellite System) and IMU (Accelerometer and Gyroscope).

![Lora Topology Overview](https://github.com/DitroniX/ESPRanger-LoRa-GNSS-GPS-IMU-RS485-60V/blob/main/Datasheets%20and%20Information/DitroniX.net%20Lora%20Topology%20Overview.png)

### **Top Level Devices**

The purpose of **ESPRanger**, is to provide a very compact board which includes the below features:

 - **Espressif ESP32-C6**
	 - WiFi, Bluetooth, Zigbee and Thread
 - **EByte E22-900MM22S**
	 - LoRa SX1262 module for 868MHz through to 915MHz
 - **ATGM336H-5N**
	 - High performance multi-constellation GNSS receiver for GPS, Galileo, QZSS and BDS
 - **ST LSM6DSL**
	 - IMU with 3D Digital Accelerometer and 3D Digital Gyroscope
 - **Chipanalog CS48520D**
	 - Industry Standard RS-485 interface
 - **TI TMP102**
	 -Digital Temperature
 - **Siproin SSP9481**
	 -5 to 60V DC SMPS for wide ranging power input, providing a stable 5V
 - **OnSemi NCP167BMX330**
	 -5V to 3V3 DC SMPS	 
 - **EEPROM P24C64C**
	 -64Kbit storage
	 -Two of these are included, one for ATGM336H and one for the ESP32-C6.

### **Where can ESPRanger be used?**

The **ESPRanger** board can be used in a wide range of projects and installations such as:

-   Automation
-   Commercial
-   Drones
-   Factories
-   Farms
-   Greenhouses
-   Homes
-   Industry
-   Makers
-   Modelling
-   Off Grid Systems
-   Robitics
-   Security
-   Shipping
-   Smart systems
-   Solar Farms
-   STEM Education
-   Wind Farms

## Features (Preliminary)

### **Radio and U.FL**

**ESPRanger** main MCU is the ESP32-C6 which includes a powerful radio module, opening up a range of IoT applications that include:

#### Wi-Fi 6

-   2.4 GHz (2400 ~ 2483.5 MHz)
-   802.11ax (20 MHz bandwidth)
-   802.11b/g/n (20/40 MHz bandwidth)
-   CE Max EIRP 19.81 dBm

#### Bluetooth

-   2.4 GHz (2400 ~ 2483.5 MHz)
-   Bluetooth LE
-   Bluetooth 5.3
-   Bluetooth Mesh
-   CE Max EIRP 18.46 dBm

#### [Thread](https://www.threadgroup.org/) 1.4

-   2.4 GHz (2405 ~ 2480 MHz)
-   802.15.4
-   Thread Mesh
-   CE Max EIRP 10.29 dBm

#### [Zigbee](https://csa-iot.org/) 3.0

-   2.4 GHz (2405 ~ 2480 MHz)
-   802.15.4
-   Zigbee Mesh
-   CE Max EIRP 10.40 dBm

Zigbee and mesh are expanding in an range of home, farming and industrial applications, so will be interested to see how this develops over time.

Using a mix of Ethernet and Zigbee, installations could offer remote mesh systems, with a mix of communications technologies.

### **LoRa and LoRaWAN**

The main focus of the board was sensor flexibility and the ability to send this data to the outside world.  LoRa provides a Long Range wireless communication technology.

The **ESPRanger** includes an EByte E22-900MM22S LoRa module, with Semtech SX1262.  This module covers 850 to 930MHz, so allowing the ESPRanger to work in many countries around the World.  

Example [bands](https://www.thethingsnetwork.org/docs/lorawan/frequencies-by-country/):

-   AS923 (AS1) band (920-923 MHz) in Japan, Malaysia, Singapore
-   AS923 (AS2) band (915–928 MHz) in Asia
-   AU915 band (915–928 MHz) in South America
-   CN779 band (779-787 MHz) in China
-   EU868 band (863–870 MHz) in Europe 
-   IN865 band (865-867 MHz) in India
-   KR920 band (920-923 MHz) in Korea
-   RU864 band (864-870 MHz) in Russia
-   US915 band (902–928 MHz) in North America

LoRa networks supported include [TTN](https://www.thethingsnetwork.org/), [Helium](https://www.helium.com/iot), [Meshtastic](https://meshtastic.org/) and [ChirpStack](https://www.chirpstack.io/).

An U.FL antenna connector is provided to allow internal or external antennas to be fitted.

### **GNSS**

The  ATGM336H-5NR32 is a high-performance, low-power, multi-constellation GNSS module, which features 32 tracking channels to fully support positioning and navigation via Beidou (BDS

 - American GPS
 - EU Galileo
 - Chinese BDS (Beidou)
 - Japanese QZSS
 - Satellite enhanced system SBAS (WAAS,EGNOS,GAGAN, MSAS)

A dedicated EEPROM has been included to allow the module to to store the almanac, which is a inventory of currently available satellites and their orbits.

An external optional plug-in CR2302 3V coin battery maybe added.

An U.FL antenna connector is provided to allow internal or external antennas to be fitted.

### **IMU**

A requested feature by some users is an on-board Digital Accelerometer and Gyroscope.   The LSM6DSLTR has been included on the board.

This IMU is an always-on compact 3D device and offers low power but with excellent and extensive features.


### **RS-485**

To interfacing to other devices shich Smart Meters, Devices and Smart Batteries which have an RS-485 MODBUS interface, the **ESPRanger** includes a standard industry interface which allows you to collect, or control, other devices on the RS-485 bus.

This RS-485 interface could also be used to control local CCTV on remote installations, or even send data from the **ESPRanger** board over a single twisted pair, up to around 1,200 meters (4,000 feet), in distance. Ideal for farms and remote installations.

### **Board Configuration**

[Two jumpers](https://github.com/DitroniX/ESPRanger-LoRa-GNSS-GPS-IMU-RS485-60V?tab=readme-ov-file#board-configuration) are provided, which allow you to enable/disable the power to the GNSS and/or LoRa modules.

### **Outline Features and Specifications**

The **ESPRanger** board main features and Outline Specifications:

#### **Board**

-   **Compact and Flexible Proven Design**
    -   Only 65mm x 30mm (Pi Zero size)
    -   Fully Populated and Tested
-   **Easy to interface and Program**
-   **Board Jumpers for Easy 'No Solder' Configuration**

#### **Devices and Interfaces**

-   **Espressif ESP32 C6**
    -   ESP32-C6-MINI-1U-N4 (MHF3 Antenna Connector)
    -   32-bit RISC-V MCU
    -   Single Core MCU up to 160 MHz
    -   Wi-Fi 6 | 2.4 GHz | 802.11ax | 802.11b/g/n
    -   Bluetooth | LE | 5.3 | Mesh
    -   Thread 1.4 | 802.15.4 | Mesh
    -   Zigbee 3.0 | 802.15.4 | Mesh
    -   4 MB Flash
    -   3 UARTS
    -   I2C
    -   SPI (2xCS)
    -   USB (Type C)
    -   ADC
    -   User LED
-   **TYPE C Interfaces (x2)**
    -   ESP32-C6 USB | JTAG | Debugging | 5V Supply
    -   UART (esp32-c6-devkitm-1 compatible) | CH343P | Serial Monitor | 5V Supply
-   **RS-485 CS48520D Interface**
    -   MODBUS or Other Protocol
    -   Selectable 120R Load Resistor
    -   Up to 20Mbps (limited only by the ESP32-C6)
    -   Driver with Current Limiter and Thermal Shutdown Protection
    -   Open, Short and Idle Bus Failsafe Protection
    -   Optional Ground for Shielded Cable to Asymmetrical TVS Diode
    -   Chipanalog Half-Duplex Transceiver
    -   EN/TXD LEDs
-   **24C64 EEPROM**
    -   Memory  64kbit (8192x8)
    -   Parameter Settings
    -   Logging
    -   P24C64C
-   **TMP102 Temperature**
    -   Ambient PCB Temperature Sensor
    
-   **OLED I2C Interface**
    -   I2C Interface
    -   OLED I2C Connector
    -   Configurable Power Pins (Solder Pads)
-   **WS2812B RGB LED**
    -   RGB Status LED
-   **SSP9481 Onboard SMPS Board Power Supply**
    -   5V From Either Type C USB
    -   Power taken from DC Input (4.5V to 60V)
-   **Tactile Buttons**
    -   Reset Button
    -   User Programmable Button / PGM

### **Programming and Development**

Development is primarily supported through Visual Studio Code (VSCode and PlatformIO) and Arduino IDE. This also includes platforms like Home Assistant via ESPHome.

A range of libraries are already available which support the devices on the **ESPRanger** such as the:

-   ESP32-C6
-   E22-900MM22S (SX1262)
-   LSM6DSL
-   ATGM336H
-   24C64
-   TMP102
-   WS2812

Code support is available in my existing [GitHub](https://github.com/DitroniX)  [code](https://github.com/DitroniX/ESPRanger-LoRa-GNSS-GPS-IMU-RS485-60V/tree/main/Code).

This is a worldwide community project and is able to be integrated in so many systems.



## **Further Information**

Additional information, and other technical details on this project, maybe found in the related repository pages.

**Repository Folders**

 - **Code** *(Code examples for Arduino  IDE, PlatformIO)*
 -  **Datasheets and Information** *(Component Datasheets, Schematics, Board Layouts, Photos, Technical Documentation)*
 - **Certification** *(Related Repository Project or Part, Certification Information)*

**Repository Tabs**

 - **Wiki** *(Related Repository Wiki pages and Technical User Information)*
 - **Discussions** *(Related Repository User Discussion Forum)*
 - **Issues** *(Related Repository Technical Issues and Fixes)*

***

We value our Customers, Users of our designs and STEM Communities, all over the World . Should you have any other questions, or feedback to share to others, please feel free to:

* Visit the related [Project](https://github.com/DitroniX?tab=repositories) *plus the related* **Discussions** and **Wiki** Pages.  See tab in each separate repository.
* **Project Community Information** can be found at https://www.hackster.io/DitroniX
* [DitroniX.net Website - Contact Us](https://ditronix.net/contact/)
* **Twitter**: [https://twitter.com/DitroniX](https://twitter.com/DitroniX)
* [Supporting the STEM Projects - BuyMeACoffee](https://www.buymeacoffee.com/DitroniX)
*  **LinkedIN**: [https://www.linkedin.com/in/g8puo/](https://www.linkedin.com/in/g8puo/)

***Dave Williams, Maidstone, UK.***

Electronics Engineer | Software Developer | R&D Support | RF Engineering | Product Certification and Testing | STEM Ambassador

## STEM

**Supporting [STEM Learning](https://www.stem.org.uk/)**
