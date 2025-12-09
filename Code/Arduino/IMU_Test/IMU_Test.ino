/*
  Dave Williams, DitroniX 2019-2025 (ditronix.net)

  Example Code, to demonstrate and test the ESPRanger, Basic IMU Test

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
#include <Wire.h>
#include "FastIMU.h"
#include <Adafruit_NeoPixel.h>

// **************** USER VARIABLES / DEFINES / STATIC / STRUCTURES / CONSTANTS ****************

// Define I2C (Expansion Port)
#define I2C_SDA 6
#define I2C_SCL 7

// Hardware Serial 0 GPIO Pins
#define RXD0 17
#define TXD0 16

// IMU
#define PERFORM_CALIBRATION  // Comment Out - to disable startup calibration
#define IMU_ADDRESS 0x6A
LSM6DS3 IMU;
calData calib = { 0 };  //Calibration Data
AccelData accelData;    //Sensor Data
GyroData gyroData;      // Gyro Data
MagData magData;        // Magnetometer Data

// RGB
#define PIN_WS2812B 8  // The ESP32-C6 pin GPIO8 connected to WS2812B
Adafruit_NeoPixel ws2812b(1, PIN_WS2812B, NEO_GRB + NEO_KHZ800);

int CycleCount = 0;

// **************** OUTPUTS ****************
#define LED_Red 22  // Red LED

// Calibration
void Calibration() {

  // Turn off RGB
  ws2812b.clear();
  ws2812b.show();

  Serial.println("FastIMU calibration & data example");
  
  if (IMU.hasMagnetometer()) {
    delay(1000);
    Serial.println("Move IMU in figure 8 pattern until done.");
    delay(3000);
    IMU.calibrateMag(&calib);
    Serial.println("Magnetic calibration done!");
  } else {
    delay(5000);
  }

  delay(5000);
  Serial.println("Keep IMU level. Waiting 5 seconds before calibration");
  delay(5000);

  IMU.calibrateAccelGyro(&calib);
  Serial.println("Calibration done!");
  Serial.println("Accel biases X/Y/Z: ");
  Serial.print(calib.accelBias[0]);
  Serial.print(", ");
  Serial.print(calib.accelBias[1]);
  Serial.print(", ");
  Serial.println(calib.accelBias[2]);
  Serial.println("Gyro biases X/Y/Z: ");
  Serial.print(calib.gyroBias[0]);
  Serial.print(", ");
  Serial.print(calib.gyroBias[1]);
  Serial.print(", ");
  Serial.println(calib.gyroBias[2]);

  if (IMU.hasMagnetometer()) {
    Serial.println("Mag biases X/Y/Z: ");
    Serial.print(calib.magBias[0]);
    Serial.print(", ");
    Serial.print(calib.magBias[1]);
    Serial.print(", ");
    Serial.println(calib.magBias[2]);
    Serial.println("Mag Scale X/Y/Z: ");
    Serial.print(calib.magScale[0]);
    Serial.print(", ");
    Serial.print(calib.magScale[1]);
    Serial.print(", ");
    Serial.println(calib.magScale[2]);
  }
  delay(5000);
  IMU.init(calib, IMU_ADDRESS);
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
  Wire.setClock(400000);  //400 kHz Clock

  ws2812b.begin();  // Initialize WS2812B
  ws2812b.clear();  // Turn off RGB



  // IMU
  int err = IMU.init(calib, IMU_ADDRESS);
  if (err != 0) {
    Serial.print("Error initializing IMU: ");
    Serial.println(err);
    while (true) {
      ;
    }
  }

#ifdef PERFORM_CALIBRATION
  Serial.println("ESPRanger - IMU Calibration");
  Calibration();
#endif

  //err = IMU.setGyroRange(500);      //USE THESE TO SET THE RANGE, IF AN INVALID RANGE IS SET IT WILL RETURN -1
  //err = IMU.setAccelRange(2);       //THESE TWO SET THE GYRO RANGE TO ±500 DPS AND THE ACCELEROMETER RANGE TO ±2g

  if (err != 0) {
    Serial.print("Error Setting range: ");
    Serial.println(err);
    while (true) {
      ;
    }
  }

  Serial.println("ESPRanger - IMU Ready");
}

void loop() {

  IMU.update();
  IMU.getAccel(&accelData);

  Serial.print(accelData.accelX);
  Serial.print("\t");
  Serial.print(accelData.accelY);
  Serial.print("\t");
  Serial.print(accelData.accelZ);
  Serial.print("\t");
  IMU.getGyro(&gyroData);
  Serial.print(gyroData.gyroX);
  Serial.print("\t");
  Serial.print(gyroData.gyroY);
  Serial.print("\t");
  Serial.print(gyroData.gyroZ);

  if (IMU.hasMagnetometer()) {
    IMU.getMag(&magData);
    Serial.print("\tM\t");
    Serial.print(magData.magX);
    Serial.print("\t");
    Serial.print(magData.magY);
    Serial.print("\t");
    Serial.print(magData.magZ);
  }

  if (IMU.hasTemperature()) {
    Serial.print("\tT\t");
    Serial.println(IMU.getTemp());
  } else {
    Serial.println();
  }
  delay(50);

  // Just some fun

  if (accelData.accelX < 0.02 || accelData.accelY < 1 || accelData.accelZ < 1 || gyroData.gyroX < 1 || gyroData.gyroY < 1 || gyroData.gyroZ) {
    // Turn off RGB
    ws2812b.clear();
    ws2812b.show();
  }

  if (accelData.accelX > 1) {
    // RED
    ws2812b.setPixelColor(0, ws2812b.Color(50, 0, 0));
    ws2812b.show();
    delay(250);
  }

  if (accelData.accelX < -0.1) {
    // BLUE
    ws2812b.setPixelColor(0, ws2812b.Color(0, 0, 50));
    ws2812b.show();
    delay(250);
  }

  if (gyroData.gyroZ > 1) {
    // GREEN
    ws2812b.setPixelColor(0, ws2812b.Color(0, 50, 0));
    ws2812b.show();
    delay(250);
  }

  if (gyroData.gyroZ < -0.1) {
    // RED
    ws2812b.setPixelColor(0, ws2812b.Color(50, 0, 0));
    ws2812b.show();
    delay(250);
  }


  // Heartbeat RED LED
  if (CycleCount == 0) {
    digitalWrite(LED_Red, HIGH);
    delay(50);
    digitalWrite(LED_Red, LOW);
  }

  // Heartbeat Loop Counter
  CycleCount++;
  if (CycleCount >= 10) CycleCount = 0;
}
