/*
  Dave Williams, DitroniX 2019-2025 (ditronix.net)

  Example Code, to demonstrate and test the ESPRanger - Morse FM TX Example
  Based on: https://github.com/jgromes/RadioLib/tree/master/examples/Morse/Morse_Transmit_FM

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

#define FET 3  // Place holder

SX1262 radio = new Module(SPI_CS, E22_IRQ, E22_NRST, E22_BUSY);
AFSKClient audio(&radio, FET);

// create Morse client instance using the AFSK instance
MorseClient morse(&audio);

// **************** SETUP ****************
void setup() {
  Serial.begin(115200);

  SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI, SPI_CS);

  Serial.print(F("\n[SX1262] Initializing ... "));

  radio.XTAL = true;

  int state = radio.beginFSK();

  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true) { delay(10); }
  }

  // initialize Morse client
  Serial.print(F("[Morse] Initializing ... "));
  // tone frequency:              400 Hz
  // speed:                       20 words per minute
  state = morse.begin(400);
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true) { delay(10); }
  }
}


// **************** LOOP ****************
void loop() {
  Serial.print(F("[Morse] Sending Morse data ... "));

  // MorseClient supports all methods of the Serial class
  // NOTE: Characters that do not have ITU-R M.1677-1
  //       representation will not be sent! Lower case
  //       letters will be capitalized.

  // send start signal first
  morse.startSignal();

  // Arduino String class
  String aStr = "Arduino String";
  morse.print(aStr);

  // character array (C-String)
  morse.print("C-String");

  // string saved in flash
  morse.print(F("Flash String"));

  // character
  morse.print('c');

  // byte
  // formatting DEC/HEX/OCT/BIN is supported for
  // any integer type (byte/int/long)
  morse.print(255, HEX);

  // integer number
  int i = 1000;
  morse.print(i);

  // floating point number
  // NOTE: When using println(), the transmission will be
  //       terminated with end-of-work signal (...-.-).
  float f = -3.1415;
  morse.println(f, 3);

  Serial.println(F("done!"));

  // wait for a second before transmitting again
  delay(1000);
}