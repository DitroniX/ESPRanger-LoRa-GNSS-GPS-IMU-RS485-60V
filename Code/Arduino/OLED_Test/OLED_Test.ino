/*
  Dave Williams, DitroniX 2019-2025 (ditronix.net)

  Example Code, to demonstrate and test the ESPRanger, OLED and RGB Test and Demo

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
#include <GyverOLED.h>
#include <Adafruit_NeoPixel.h>

// **************** USER VARIABLES / DEFINES / STATIC / STRUCTURES / CONSTANTS ****************

// Define I2C (Expansion Port)
#define I2C_SDA 6
#define I2C_SCL 7

// Hardware Serial 0 GPIO Pins
#define RXD0 17
#define TXD0 16

// RGB
#define PIN_WS2812B 8  // The ESP32-C6 pin GPIO8 connected to WS2812B
Adafruit_NeoPixel ws2812b(1, PIN_WS2812B, NEO_GRB + NEO_KHZ800);

// **************** OUTPUTS ****************
#define LED_Red 22  // Red LED

// OLED Instance. You will need to select your OLED Display. 
// Uncomment/Comment as needed.
GyverOLED<SSD1306_128x32, OLED_BUFFER> oled;  //0.6"
//GyverOLED<SSD1306_128x32, OLED_NO_BUFFER> oled;
//GyverOLED<SSD1306_128x64, OLED_BUFFER> oled;
//GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> oled;
// GyverOLED<SSH1106_128x64> oled; //1.1"

// Draw Battery
void drawBattery(byte percent) {
  oled.drawByte(0b00111100);
  oled.drawByte(0b00111100);
  oled.drawByte(0b11111111);
  for (byte i = 0; i < 100 / 8; i++) {
    if (i < (100 - percent) / 8) oled.drawByte(0b10000001);
    else oled.drawByte(0b11111111);
  }
  oled.drawByte(0b11111111);
}

// **************** SETUP ****************
void setup() {
  // Stabalise
  delay(250);

  // Initialise UART
  Serial.begin(115200, SERIAL_8N1, RXD0, TXD0);  // U0
  while (!Serial)
    ;
  Serial.println("");

  // Initialise RED LED on GP22
  pinMode(LED_Red, OUTPUT);

  // Initialize I2C
  Wire.begin(I2C_SDA, I2C_SCL);

  ws2812b.begin();  // Initialize WS2812B
  ws2812b.clear();  // Turn off RGB

  Serial.println("ESPRanger - Example Code");
}

void loop() {

  // Turn off RGB
  ws2812b.clear();
  ws2812b.show();

  // OLED
  oled.init();
  oled.clear();
  oled.setCursor(15, 0);
  oled.setScale(2);
  oled.print("ESPRanger");
  oled.update();
  delay(500);

  // OLED
  oled.init();
  oled.clear();
  oled.setCursor(15, 1);
  oled.setScale(2);
  oled.print("ESPRanger");
  oled.update();
  delay(500);

  // OLED
  oled.init();
  oled.clear();
  oled.setCursor(15, 2);
  oled.setScale(2);
  oled.print("ESPRanger");
  oled.update();
  delay(500);

  // RED
  ws2812b.setPixelColor(0, ws2812b.Color(50, 0, 0));
  ws2812b.show();
  delay(500);

  // OLED
  oled.init();
  oled.clear();
  oled.setCursor(30, 1);
  oled.setScale(3);
  oled.print("LoRa");
  oled.update();

  // Turn off RGB
  ws2812b.clear();
  ws2812b.show();  // update to the WS2812B Led Strip

  // BLUE
  ws2812b.setPixelColor(0, ws2812b.Color(0, 0, 50));
  ws2812b.show();
  delay(500);

  // OLED
  oled.init();
  oled.clear();
  oled.setCursor(35, 1);
  oled.setScale(3);
  oled.print("IMU");
  oled.update();

  // Turn off RGB
  ws2812b.clear();
  ws2812b.show();

  // GREEN
  ws2812b.setPixelColor(0, ws2812b.Color(0, 50, 0));
  ws2812b.show();
  delay(500);

  // Turn off RGB
  ws2812b.clear();
  ws2812b.show();

  // OLED
  oled.init();
  oled.clear();
  oled.setCursor(25, 1);
  oled.setScale(3);
  oled.print("GNSS");
  oled.update();

  delay(500);

  // OLED
  oled.init();
  oled.clear();
  oled.setCursor(20, 1);
  oled.setScale(3);
  oled.print("RS485");
  oled.update();

  delay(500);

  // OLED
  oled.init();
  oled.clear();
  oled.setCursor(19, 1);
  oled.setScale(3);
  oled.print("5-60V");
  oled.update();

  delay(500);

  // OLED
  oled.init();
  oled.clear();
  oled.setCursor(13, 1);
  oled.setScale(2);
  oled.print("ESP32-C6");
  oled.update();

  delay(500);

  // Heatbeat LED
  digitalWrite(LED_Red, HIGH);
  delay(100);
  digitalWrite(LED_Red, LOW);
}
