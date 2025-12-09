/*
  Dave Williams, DitroniX 2019-2025 (ditronix.net)

  Example Code, to demonstrate and test the ESPRanger ADC (DC Voltage)12-bit Σ-Δ ADC
  Based on https://github.com/adafruit/Adafruit_TLA202x 

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
  
  Espressif References:
  * docs.espressif.com/projects/arduino-esp32/en/latest/api/adc.html
  * espressif.com/sites/default/files/documentation/esp32-c6_technical_reference_manual_en.pdf#sensor
*/

// Libraries
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_TLA202x.h>

// **************** USER VARIABLES / DEFINES / STATIC / STRUCTURES / CONSTANTS ****************

Adafruit_TLA202x tla;

// Variables
float ADC_Raw;

// Define I2C (Expansion Port)
#define I2C_SDA 6
#define I2C_SCL 7

// **************** FUNCTIONS AND ROUTINES ****************

// **************** SETUP ****************
void setup() {

  // Stabalise
  delay(250);

  // Initialise UART
  Serial.begin(115200);
  while (!Serial)
    ;

  // Initialize I2C
  Wire.begin(I2C_SDA, I2C_SCL);

  // TLA20024
  if (!tla.begin()) {
    Serial.println("Failed to find TLA2024 chip");
    while (1) { delay(10); }
  }
  Serial.println("TLA2024 Found!");

  //  tla.setDataRate(TLA202x_RATE_1600_SPS);
  Serial.print("Data rate set to: ");
  switch (tla.getDataRate()) {
    case TLA202x_RATE_128_SPS: Serial.println("128 SPS"); break;
    case TLA202x_RATE_250_SPS: Serial.println("250 SPS"); break;
    case TLA202x_RATE_490_SPS: Serial.println("490 SPS"); break;
    case TLA202x_RATE_920_SPS: Serial.println("920 SPS"); break;
    case TLA202x_RATE_1600_SPS: Serial.println("1600 SPS"); break;
    case TLA202x_RATE_2400_SPS: Serial.println("2400 SPS"); break;
    case TLA202x_RATE_3300_SPS: Serial.println("3300 SPS"); break;
  }

  // tla.setRange(TLA202x_RANGE_6_144_V);
  Serial.print("Measurement range set to: ");
  switch (tla.getRange()) {
    case TLA202x_RANGE_6_144_V:
      Serial.println("+6.144 V to -6.144 V");
      break;
    case TLA202x_RANGE_4_096_V:
      Serial.println("+4.096 V to -4.096 V");
      break;
    case TLA202x_RANGE_2_048_V:
      Serial.println("+2.048 V to -2.048 V");
      break;
    case TLA202x_RANGE_1_024_V:
      Serial.println("+1.024 V to -1.024 V");
      break;
    case TLA202x_RANGE_0_512_V:
      Serial.println("+0.512 V to -0.512 V");
      break;
    case TLA202x_RANGE_0_256_V:
      Serial.println("+0.256 V to -0.256 V");
      break;
  }

  //  tla.setMux(TLA202x_MUX_AIN0_GND);
  Serial.print("Multiplexer set to: ");
  switch (tla.getMux()) {
    case TLA202x_MUX_AIN0_AIN1:
      Serial.println("AINp = AIN 0, AINn = AIN 1");
      break;
    case TLA202x_MUX_AIN0_AIN3:
      Serial.println("AINp = AIN 0, AINn = AIN 3");
      break;
    case TLA202x_MUX_AIN1_AIN3:
      Serial.println("AINp = AIN 1, AINn = AIN 3");
      break;
    case TLA202x_MUX_AIN2_AIN3:
      Serial.println("AINp = AIN 2, AINn = AIN 3");
      break;
    case TLA202x_MUX_AIN0_GND:
      Serial.println("AINp = AIN 0, AINn = GND");
      break;
    case TLA202x_MUX_AIN1_GND:
      Serial.println("AINp = AIN 1, AINn = GND");
      break;
    case TLA202x_MUX_AIN2_GND:
      Serial.println("AINp = AIN 2, AINn = GND");
      break;
    case TLA202x_MUX_AIN3_GND:
      Serial.println("AINp = AIN 3, AINn = GND");
      break;
  }
  // tla.setMode(TLA202x_MODE_CONTINUOUS);
  Serial.print("Reading mode: ");
  switch (tla.getMode()) {
    case TLA202x_MODE_ONE_SHOT: Serial.println("One-shot"); break;
    case TLA202x_MODE_CONTINUOUS: Serial.println("Continuous"); break;
  }


  Serial.println("ESPRanger - Example Code");
}

// **************** LOOP ****************
void loop() {

  Serial.print("Channel 0: ");
  Serial.print(tla.readOnce(TLA202x_CHANNEL_0));
  Serial.println(" Volts (VDC)");

  // Read ADC 1-3.  Remember these inputs are floating by default.
  // Serial.print("Channel 1: ");
  // Serial.print(tla.readOnce(TLA202x_CHANNEL_1));
  // Serial.println(" Volts");
  // Serial.print("Channel 2: ");
  // Serial.print(tla.readOnce(TLA202x_CHANNEL_2));
  // Serial.println(" Volts");
  // Serial.print("Channel 3: ");
  // Serial.print(tla.readOnce(TLA202x_CHANNEL_3));
  // Serial.println(" Volts");

  Serial.println("");
  Serial.println("\n----------------------------------------------------");

  // Loop Delay
  delay(1000);
}
