/*
  Dave Williams, DitroniX 2019-2025 (ditronix.net)

  Example Code, to demonstrate and test the ESPRanger Basic ATGM336H GNSS GPS Comms Test

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

#include <Wire.h>

// **************** OUTPUTS ****************
#define LED_Red 22      // Red LED
#define UART_Select 15  // UART Matrix High=RS485  Low=GNSS

// Hardware Serial 0 pins
#define RXD0 17
#define TXD0 16

// Hardware Serial 1 pins - UART
#define RXLP 4
#define TXLP 5

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
  while (!Serial1)
    ;
  Serial.println("UART 1 Opened (GNSS Port)");

  // Initialise RED LED on GP22
  pinMode(LED_Red, OUTPUT);

  // Initialise and Configure UART Matrix Select
  pinMode(UART_Select, OUTPUT);
  digitalWrite(UART_Select, LOW); // GNSS ATGM336H

  Serial.println("ESPRanger - Example Code\n");
}

// **************** LOOP ****************
void loop() {

  // Serial1.write("AT?");

  if (Serial1.available() > 0) {

    // Read Incomming Bytes from GNSS ATGM336H and Output to the Serial Monitor
    char incomingbyte = Serial1.read();
    Serial.print(incomingbyte);

    // Heartbeat RED LED
    digitalWrite(LED_Red, LOW);
    delay(250);
    digitalWrite(LED_Red, HIGH);
  }
}
