/*
  Dave Williams, DitroniX 2019-2025 (ditronix.net)

  Example Code, to demonstrate and test the ESPRanger - RTTY TX Example
  Based on: https://github.com/jgromes/RadioLib/tree/master/examples/RTTY

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
#include <RadioLib.h>
#include <SPI.h>

// **************** USER VARIABLES / DEFINES / STATIC / STRUCTURES / CONSTANTS ****************

// Pin definitions
#define SPI_MOSI 18
#define SPI_MISO 20
#define SPI_SCK 19
#define SPI_CS 14

#define E22_IRQ 1
#define E22_NRST 21
#define E22_BUSY 3

SX1262 radio = new Module(SPI_CS, E22_IRQ, E22_NRST, E22_BUSY);
RTTYClient rtty(&radio);

// **************** SETUP ****************
void setup()
{
  Serial.begin(115200);

  SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI, SPI_CS);

  Serial.print(F("\n[SX1262] Initializing ... "));

  radio.XTAL = true;

  int state = radio.beginFSK();

  if (state == RADIOLIB_ERR_NONE)
  {
    Serial.println(F("success!"));
  }
  else
  {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true)
    {
      delay(10);
    }
  }

  // initialize RTTY client
  // NOTE: RTTY frequency shift will be rounded
  //       to the nearest multiple of frequency step size.
  //       The exact value depends on the module:
  //         SX127x/RFM9x - 61 Hz
  //         RF69 - 61 Hz
  //         CC1101 - 397 Hz
  //         SX126x - 1 Hz
  //         nRF24 - 1000000 Hz
  //         Si443x/RFM2x - 156 Hz
  //         SX128x - 198 Hz

  Serial.print(F("[RTTY] Initializing ... "));
  // low ("space") frequency:     868.00 MHz
  // frequency shift:             183 Hz
  // baud rate:                   45 baud
  // encoding:                    ASCII (7-bit)
  // stop bits:                   1
  state = rtty.begin(868.0, 183, 45);

  if (state == RADIOLIB_ERR_NONE)
  {
    Serial.println(F("success!"));
  }
  else
  {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true)
    {
      delay(10);
    }
  }
}

// **************** LOOP ****************
void loop()
{
  Serial.print(F("[RTTY] Sending RTTY data ... "));

  // send out idle condition for 500 ms
  rtty.idle();
  delay(500);

  // RTTYClient supports all methods of the Serial class

  // Arduino String class
  String aStr = "Arduino String";
  rtty.println(aStr);

  // character array (C-String)
  rtty.println("C-String");

  // string saved in flash
  rtty.println(F("Flash String"));

  // character
  rtty.println('c');

  // byte
  // formatting DEC/HEX/OCT/BIN is supported for
  // any integer type (byte/int/long)
  rtty.println(255, HEX);

  // integer number
  int i = 1000;
  rtty.println(i);

  // floating point number
  float f = -3.1415;
  rtty.println(f, 3);

  // turn the transmitter off
  rtty.standby();

  Serial.println(F("done!"));

  // wait for a second before transmitting again
  delay(1000);
}