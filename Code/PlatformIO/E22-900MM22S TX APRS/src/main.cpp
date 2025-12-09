/*
  Dave Williams, DitroniX 2019-2025 (ditronix.net)

  Example Code, to demonstrate and test the ESPRanger - APRS TX Example 868 MHz
  APRS (Automatic Packet Reporting System) on SX1262 module by using an AFSK protocol
  Based on: https://github.com/jgromes/RadioLib/blob/master/examples/APRS/APRS_Position_LoRa/APRS_Position_LoRa.ino

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
APRSClient aprs(&radio);

// **************** SETUP ****************
void setup()
{
  Serial.begin(115200);

  SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI, SPI_CS);

  Serial.print(F("\n[SX1262] Initializing ... "));

  radio.XTAL = true;

  int state = radio.begin(868.0);

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

  Serial.print(F("[APRS] Initializing ... "));

  char source[] = "G8PUO";

  state = aprs.begin('>', source, 1);

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

  Serial.println("\nTest End");
}

// **************** LOOP ****************
void loop() {
  Serial.print(F("[APRS] Sending position ... "));
  
  // send a location with message and timestamp
  // SSID is set to 1, as APRS over LoRa uses WIDE1-1 path by default
  char destination[] = "GPS";
  char latitude[] = "51.5081N";
  char longitude[] = "0.0759W";
  char message[] = "Yo here I am!";
  char timestamp[] = "093045z";
  int state = aprs.sendPosition(destination, 1, latitude, longitude, message, timestamp);
  delay(500);

  // you can also send Mic-E encoded messages
  state |= state = aprs.sendMicE(49.1945, 16.6000, 120, 10, RADIOLIB_APRS_MIC_E_TYPE_EN_ROUTE);
  delay(500);

  if(state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
  }

  // wait one minute before transmitting again
  delay(60000);
}