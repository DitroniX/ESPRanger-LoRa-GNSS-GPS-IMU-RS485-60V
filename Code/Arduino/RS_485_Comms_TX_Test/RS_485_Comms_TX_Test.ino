/*
  Dave Williams, DitroniX 2019-2025 (ditronix.net)

  Example Code, to demonstrate and test the ESPRanger, TWO UART Ports Basic Test. 0 as Terminal UART and 1 as RS485 UART

  NB. To test the RS-485:
  * Simply connect another RS-485 device
  * Wiring A to A and B to B
    * Monitor in computer terminal if using USB-RS485
    * Use the 'RS485 RX' Code Example on another ESPuno Pi Zero

  Further information, details and examples can be found on our website and also GitHub wiki pages:
  * ditronix.net
  * github.com/DitroniX
  * github.com/DitroniX/ESPRanger-LoRa-GNSS-GPS-IMU-RS485-60V
  * github.com/DitroniX/ESPRanger-LoRa-GNSS-GPS-IMU-RS485-60V/wiki
  * hackster.io/DitroniX/espranger-lora-gnss-imu-bt-wifi-rs485-5-60v-stem-board-4a3fa4

  This test code is OPEN SOURCE and formatted for easier viewing.  Although is is not intended for real world use, it may be freely used, or modified as needed.
  It is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.

  For board configuration, see https://github.com/DitroniX/ESPRanger-LoRa-GNSS-GPS-IMU-RS485-60V/wiki/Arduino-IDE

  Further information, details and examples maybe found on github.com/DitroniX
*/

// Libraries

// **************** USER VARIABLES / DEFINES / STATIC / STRUCTURES / CONSTANTS ****************

// Constants

#include <Wire.h>

// Hardware Serial 0 pins
#define RXD0 17
#define TXD0 16

// Hardware Serial 1 pins - RS485
#define RXLP 4
#define TXLP 5
#define ENLP 23  // RS485 UART Enable

// **************** OUTPUTS ****************
#define LED_Red 22      // Red LED
#define UART_Select 15  // UART Matrix

// **************** FUNCTIONS AND ROUTINES ****************

// **************** SETUP ****************
void setup() {

  // Stabalise
  delay(250);

  // Initialise UART 0 - USB COM Port
  Serial.begin(115200, SERIAL_8N1, RXD0, TXD0);  // U0
  while (!Serial)
    ;
  Serial.println("UART 0 Opened (USB COM Port)");

  // Initialise UART 1 - RS485 Port
  Serial1.begin(9600, SERIAL_8N1, RXLP, TXLP);  //LP
  while (!Serial)
    ;
  Serial.println("UART 1 Opened (RS485 Port)");

  // Initialise RS485 UART Enable on GP23
  pinMode(ENLP, OUTPUT);

  // Initialise and Configure UART Matrix Select
  pinMode(UART_Select, OUTPUT);
  digitalWrite(UART_Select, HIGH);

  // LED
  pinMode(LED_Red, OUTPUT);

  Serial.println("ESPRanger - Example Code");
  Serial.println("Running RS485 TX Test");
}

// **************** LOOP ****************
void loop() {

  // Force RS485 UART Driver Enable and Receiver is Disabled
  digitalWrite(ENLP, HIGH);

  Serial1.write("Hello World - Test\n");

  // Cycle RED LED
  digitalWrite(LED_Red, LOW);
  delay(1000);
  digitalWrite(LED_Red, HIGH);
  delay(1000);
}
