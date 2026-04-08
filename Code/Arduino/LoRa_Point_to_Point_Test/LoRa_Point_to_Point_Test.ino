/*
  Dave Williams, DitroniX 2019-2026 (ditronix.net)

  Example Code, to demonstrate and test the ESPRanger - Basic LoRa (Point-to-Point)

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

// **************** USER VARIABLES / DEFINES / STATIC / STRUCTURES / CONSTANTS ****************

// **************** INPUTS ****************
#define User_Button 9  // GPIO 9

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

//Example Values.  Leave 00 between P2P testing.  Just compile as is, to all devices under test.
// uint8_t appEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
// uint8_t devEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
// uint8_t appKey[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

uint32_t counter = 0;
uint8_t payload[64];  // buffer for receive

// **************** SETUP ****************
void setup() {
  Serial.begin(115200);

  // Configures the specified LED GPIO as outputs
  pinMode(LED_Red, OUTPUT);

  // LED Default Off State
  digitalWrite(LED_Red, LOW);

  // Configure GPIO Inputs
  pinMode(User_Button, INPUT_PULLUP);

  // WS2812B Initialise
  ws2812b.begin();  // Initialize WS2812B
  ws2812b.clear();  // Turn off RGB

  // initialize SPI
  SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI, SPI_CS);

  // initialize SX1262 FSK modem with default settings
  Serial.print(F("\n[SX1262] Initializing ... "));

  // radio.XTAL = true;

  // Initialize the radio
  Serial.print(F("[SX1262] Initializing ... "));

  radio.XTAL = true;

  // int state = radio.begin(868.0);

  int state = radio.begin(868.0,  // frequency (MHz)
                          125.0,  // bandwidth (kHz)
                          9,      // spreading factor
                          5,      // coding rate
                          RADIOLIB_SX126X_SYNC_WORD_PRIVATE,
                          20,    // TX power (dBm)
                          8,     // preamble length
                          0.0);  // TCXO voltage (use 0.0 if no TCXO)

  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("Initialise Success!"));
    ws2812b.setPixelColor(0, ws2812b.Color(0, 150, 0));  // GREEN
    ws2812b.show();
  } else {
    ws2812b.setPixelColor(0, ws2812b.Color(150, 0, 0));  // RED
    ws2812b.show();
    Serial.print(F("Initialise Failed, Code "));
    Serial.println(state);
    while (true) {
      delay(10);
    }
  }

  Serial.println(F("Ready!"));

  Serial.println("\nESPRanger - LoRa Point-to-Point Messaging");
  Serial.println("Press USER Button to send Message\n");
}

// **************** LOOP ****************
void loop() {

  // Turn off RGB
  ws2812b.clear();  // Turn off RGB
  ws2812b.show();

  // Check for Button Press
  if (digitalRead(User_Button) == LOW) {

    Serial.print("Button Pressed\t");
    sendPacket();
  }

  // Receive Packet if Payload Available
  receivePacket();

  // Heartbeat & Turn off RGB
  ws2812b.clear();  // Turn off RGB
  ws2812b.show();
  digitalWrite(LED_Red, HIGH);
  delay(50);
  digitalWrite(LED_Red, LOW);
}

// ====================== RECEIVE PACKET ======================

void receivePacket() {

  // Try to receive a packet
  int state = radio.receive(payload, sizeof(payload) - 1);  // leave 1 byte for null terminator

  if (state == RADIOLIB_ERR_NONE) {

    ws2812b.setPixelColor(0, ws2812b.Color(0, 150, 0));  // GREEN
    ws2812b.show();

    // Packet received successfully
    payload[sizeof(payload) - 1] = '\0';  // ensure null termination

    Serial.print(F("Received: "));
    Serial.print(F(" RSSI: "));
    Serial.print(radio.getRSSI());
    Serial.print(F(" dBm   SNR: "));
    Serial.print(radio.getSNR());
    Serial.print(F(" dB\t Msg: "));
    Serial.println((char*)payload);
    delay(500);
  }
}

// ====================== SEND PACKET ======================
void sendPacket() {

  // Turn off RGB
  ws2812b.clear();  // Turn off RGB
  ws2812b.show();
  delay(250);

  // Build a simple text message
  snprintf((char*)payload, sizeof(payload), "Hello from ESPRanger #%lu", counter);

  Serial.print(F("Sending: "));
  Serial.println((char*)payload);

  int state = radio.transmit(payload, strlen((char*)payload));

  if (state == RADIOLIB_ERR_NONE) {
    ws2812b.setPixelColor(0, ws2812b.Color(0, 0, 150));  // BLUE
    ws2812b.show();
  } else {
    ws2812b.setPixelColor(0, ws2812b.Color(150, 0, 0));  // RED
    ws2812b.show();
    Serial.print(F("Sending Failed. Code "));
    Serial.println(state);
    while (true) {
      delay(10);
    }
  }

  counter++;
  delay(500);
}