/*
  Dave Williams, DitroniX 2019-2025 (ditronix.net)

  Example Code, to demonstrate and test the ESPRanger Initialise all Ports Example to Assist Your Development

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
#include <SPI.h>
#include <Wire.h>
#include <GyverOLED.h>          // OLED
#include <Adafruit_NeoPixel.h>  // OLED
#include <Adafruit_TLA202x.h>   // ADC

// **************** USER VARIABLES / DEFINES / STATIC / STRUCTURES / CONSTANTS ****************

Adafruit_TLA202x tla;

// Hardware Serial 0 GPIO - Serial Print
#define RXD0 17
#define TXD0 16

// Hardware Serial 1 pins - UART (485/GNSS)
#define RXLP 4
#define TXLP 5

// Define I2C (Expansion Port)
#define I2C_SDA 6
#define I2C_SCL 7

// SPI (LORA)
#define SPI_SCK 19
#define SPI_MISO 20
#define SPI_MOSI 18

// LoRa
#define CS_LORA 14
#define INT_LORA 1
#define RST_LORA 21
#define BUSY_LORA 3

// IMU
#define INT_IMU 0

// ########################################################

// Outputs
#define LED_Red 22      // Red LED.  Heatbeat or User LED
#define UART_Select 15  // UART Matrix.  High=RS485  Low=GNSS (Default)
#define FET 2           // FET Output
#define ENLP 23         // RS485 UART Enable.  High=TX (Default) Low=RX

// Inputs
#define User_Button 9  // User Button

// RGB
#define PIN_WS2812B 8  // The ESP32-C6 pin GPIO8 connected to WS2812B
Adafruit_NeoPixel ws2812b(1, PIN_WS2812B, NEO_GRB + NEO_KHZ800);

// OLED Instance. You will need to select your OLED Display. 
// Uncomment/Comment as needed.
GyverOLED<SSD1306_128x32, OLED_BUFFER> oled;  //0.6"
//GyverOLED<SSD1306_128x32, OLED_NO_BUFFER> oled;
//GyverOLED<SSD1306_128x64, OLED_BUFFER> oled;
//GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> oled;
// GyverOLED<SSH1106_128x64> oled; //1.1"

// **************** SETUP ****************
void setup() {
  // Stabalise
  delay(250);

  // Initialise UART Port
  Serial.begin(115200, SERIAL_8N1, RXD0, TXD0);  // U0
  while (!Serial)
    ;

  // Initialise UART 1 - GNSS/RS485 Port
  Serial1.begin(9600, SERIAL_8N1, RXLP, TXLP);  //LP
  while (!Serial)
    ;

  // Initialise and Configure UART Matrix Select
  pinMode(UART_Select, OUTPUT);
  // digitalWrite(UART_Select, LOW); // GNSS ATGM336H (Default)
  // digitalWrite(UART_Select, HIGH); // RS485

  // Initialise SPI
  SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);

  // Initialize I2C
  Wire.begin(I2C_SDA, I2C_SCL);

  // Configure GPIO Inputs
  pinMode(LED_Red, OUTPUT);   // Initialise RED LED
  pinMode(FET, OUTPUT);       // Initialise FET
  pinMode(ENLP, OUTPUT);      // Initialise RS485 UART Enable.  Low = RX  High = TX (Default)
  pinMode(RST_LORA, OUTPUT);  // Initialise Reset LoRa

  // Configure GPIO Inputs
  pinMode(User_Button, INPUT_PULLUP);  // Initialise User Button
  pinMode(INT_IMU, INPUT_PULLUP);      // Initialise Interupt for IMU
  pinMode(INT_LORA, INPUT_PULLUP);     // Initialise Interupt for LoRa
  pinMode(BUSY_LORA, INPUT_PULLUP);    // Initialise BUSY for LoRa

  // TLA20024
  if (!tla.begin()) {
    Serial.println("Failed to find TLA2024 chip");
    while (1) { delay(10); }
  }
  Serial.println("TLA2024 Found!");

  // OLED
  oled.init();
  oled.clear();
  oled.setCursor(15, 1);
  oled.setScale(2);
  oled.print("ESPRanger");
  oled.update();
  delay(500);

  // Initialise RGB and Turn Off
  ws2812b.clear();
  ws2812b.show();

  // Hello World
  Serial.println("\nWelcome to the ESPRanger\n");
}

// **************** LOOP ****************
void loop() {

  // Check for Button Press
  if (digitalRead(User_Button) == LOW) {

    Serial.println("User Button Pressed");

    // BLUE LED
    ws2812b.setPixelColor(0, ws2812b.Color(0, 0, 150));
    ws2812b.show();
    delay(500);
    ws2812b.clear();
    ws2812b.show();
  }

  // Heartbeat RED LED
  digitalWrite(LED_Red, HIGH);
  delay(50);
  digitalWrite(LED_Red, LOW);
  delay(1000);
}