/*
  Dave Williams, DitroniX 2019-2025 (ditronix.net)

  Example Code, to demonstrate and test the ESPRanger - LoRa Spectral Scan

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

// REMEMBER TO PUT THE LORA ENABLE JUMPER ON - OR THIS CODE WILL NOT WORK.

// IMPORTANT: ENSURE AN ANTENNA, OR RF LOAD, IS CONNECTED TO THE LORA MODULE - BEFORE TX.  OTHERWISE DAMAGE TO THE LORA MODULE COULD RESULT.

// Libraries
#include <RadioLib.h>
#include <SPI.h>
#include <Adafruit_NeoPixel.h>

/*
  RadioLib SX126x Spectrum Scan Example

  This example shows how to perform a spectrum power scan using SX126x.

  The output is in the form of scan lines, each line has 33 power bins.

  First power bin corresponds to -11 dBm, the second to -15 dBm and so on.

  Higher number of samples in a bin corresponds to more power received at that level.

  To show the results in a plot, run the Python script
  RadioLib/extras/SX126x_Spectrum_Scan/SpectrumScan.py

  WARNING: This functionality is experimental and requires a binary patch to be uploaded to the SX126x device. There may be some undocumented side effects!

  For default module settings, see the wiki page
  https://github.com/jgromes/RadioLib/wiki/Default-configuration#sx126x---lora-modem

  For full API reference, see the GitHub Pages
  https://jgromes.github.io/RadioLib/
*/

// this file contains binary patch for the SX1262
#include <modules/SX126x/patches/SX126x_patch_scan.h>

// **************** USER VARIABLES / DEFINES / STATIC / STRUCTURES / CONSTANTS ****************

// **************** OUTPUTS ****************
#define LED_Red 22  // Red LED

// RGB
#define PIN_WS2812B 8  // The ESP32-C6 pin GPIO8 connected to WS2812B
Adafruit_NeoPixel ws2812b(1, PIN_WS2812B, NEO_GRB + NEO_KHZ800);

// Pin definitions
#define SPI_MOSI 18
#define SPI_MISO 20
#define SPI_SCK 19
#define SPI_CS 14

// ESPRanger
#define E22_IRQ 1
#define E22_NRST 21
#define E22_BUSY 3

SX1262 radio = new Module(SPI_CS, E22_IRQ, E22_NRST, E22_BUSY);

// **************** SETUP ****************
void setup() {
  Serial.begin(115200);

  // Configures the specified LED GPIO as outputs
  pinMode(LED_Red, OUTPUT);

  // LED Default Off State
  digitalWrite(LED_Red, LOW);

  // WS2812B Initialise
  ws2812b.begin();  // Initialize WS2812B
  ws2812b.clear();  // Turn off RGB for two seconds


  SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI, SPI_CS);

  // initialize SX1262 FSK modem with default settings
  Serial.print(F("\n[SX1262] Initializing ... "));

  // radio.XTAL = true;

  int state = radio.beginFSK();

  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
    ws2812b.setPixelColor(0, ws2812b.Color(0, 150, 0));
    ws2812b.show();
  } else {
    ws2812b.setPixelColor(0, ws2812b.Color(150, 0, 0));
    ws2812b.show();
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true) {
      delay(10);
    }
  }

  // Upload a patch to the SX1262 to enable spectral scan

  // NOTE: this patch is uploaded into volatile memory, and must be re-uploaded on every power up

  Serial.print(F("[SX1262] Uploading patch ... "));

  state = radio.uploadPatch(sx126x_patch_scan, sizeof(sx126x_patch_scan));

  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
    ws2812b.setPixelColor(0, ws2812b.Color(0, 150, 0));
    ws2812b.show();
  } else {
    ws2812b.setPixelColor(0, ws2812b.Color(150, 0, 0));
    ws2812b.show();
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true) {
      delay(10);
    }
  }

  // Configure scan bandwidth to 234.4 kHz and disable the data shaping
  Serial.print(F("[SX1262] Setting scan parameters ... "));

  state = radio.setRxBandwidth(234.3);

  state |= radio.setDataShaping(RADIOLIB_SHAPING_NONE);

  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
    ws2812b.setPixelColor(0, ws2812b.Color(0, 150, 0));
    ws2812b.show();
  } else {
    ws2812b.setPixelColor(0, ws2812b.Color(150, 0, 0));
    ws2812b.show();
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true) {
      delay(10);
    }
  }

  Serial.println("\nESPRanger - LoRa Spectral Scan");
}

// **************** LOOP ****************
void loop() {

  // Turn off RGB
  ws2812b.clear();
  ws2812b.show();
  delay(250);

  Serial.print(F("[SX1262] Starting Spectral Scan ... "));
  ws2812b.setPixelColor(0, ws2812b.Color(150, 0, 0));
  ws2812b.show();

  // start spectral scan
  // number of scans in each line is 2048
  // number of samples: 2048 (fewer samples = better temporal resolution)
  int state = radio.spectralScanStart(2048);
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true) { delay(10); }
  }

  // wait for spectral scan to finish
  while (radio.spectralScanGetStatus() != RADIOLIB_ERR_NONE) {
    delay(10);
  }

  // read the results
  uint16_t results[RADIOLIB_SX126X_SPECTRAL_SCAN_RES_SIZE];
  state = radio.spectralScanGetResult(results);
  if (state == RADIOLIB_ERR_NONE) {
    // we have some results, print it
    Serial.print("SCAN ");
    for (uint8_t i = 0; i < RADIOLIB_SX126X_SPECTRAL_SCAN_RES_SIZE; i++) {
      Serial.print(results[i]);
      Serial.print(',');
    }
    Serial.println(" END");
  }

  ws2812b.clear();
  ws2812b.show();

  // Blue
  ws2812b.setPixelColor(0, ws2812b.Color(0, 0, 80));
  ws2812b.show();

  // Flash RED LED
  digitalWrite(LED_Red, HIGH);
  delay(250);
  digitalWrite(LED_Red, LOW);

  delay(2000);
}