/*
  Dave Williams, DitroniX 2019-2025 (ditronix.net)

  Example Code, to demonstrate and test the ESPRanger, Bring Up Test

  This code is wip and drafted together to test the board.  

  #### LoRa Disabled by default. Remarked Out.
      If LoRa TX Enabled. PLEASE REMEMBER TO PUT a LoRa Antenna on before running this!

  I2C_Port_Scanner_Test
  RGB LED Basic Test - Multiple RGB LEDs Connected
  Basic EEPROM Test
  Basic ATGM336H Comms Test with OLED
  Basic IMU Test
  Basic TMP102 Board Temperature Test
  Basic LoRa Comms TX Test

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
#include <Arduino.h>
#include <Wire.h>
#include <GyverOLED.h>
#include <Adafruit_NeoPixel.h>
#include "I2C_eeprom.h"
#include <SparkFunTMP102.h> // Used to send and recieve specific information from the sensor
#include "FastIMU.h"
#include <RadioLib.h>
#include <Adafruit_TLA202x.h>

// **************** USER VARIABLES / DEFINES / STATIC / STRUCTURES / CONSTANTS ****************

// Hardware Serial 0 pins
#define RXD0 17
#define TXD0 16

// Hardware Serial 1 pins - UART / RS485
#define RXLP 4
#define TXLP 5
#define ENLP 23 // RS485 UART Enable

// Define I2C (Expansion Port)
#define I2C_SDA 6
#define I2C_SCL 7

// SPI E22 Pin definitions
#define SPI_MOSI 18
#define SPI_MISO 20
#define SPI_SCK 19
#define SPI_CS 14

#define E22_IRQ 1
#define E22_NRST 21
#define E22_BUSY 3

// **************** OUTPUTS ****************
#define LED_Red 22     // Red LED
#define FET 2          // FET
#define UART_Select 15 // UART Matrix High=RS485  Low=GNSS (Defaults to GNSS)

// RGB
#define PIN_WS2812B 8 // The ESP32-C6 pin GPIO8 connected to WS2812B
#define NUM_PIXELS 1  // The number of LEDs (pixels) on WS2812B LED strip

// adc
Adafruit_TLA202x tla;
float ADC_Raw;

// LoRa
SX1262 radio = new Module(SPI_CS, E22_IRQ, E22_NRST, E22_BUSY);

// WS2812
Adafruit_NeoPixel ws2812b(NUM_PIXELS, PIN_WS2812B, NEO_GRB + NEO_KHZ800);

// EEPROM
I2C_eeprom ee(0x50, I2C_DEVICESIZE_24LC64);

uint32_t start, duration, totals = 0;

struct
{
  float temperature;
  float humidity;
  float pressure;
} measurement;

// Variables TMP102
TMP102 TempSensor;
float TemperatureC; // TMP102 Temperature C
float TemperatureF; // TMP102 Temperature F

// OLED Instance. You will need to select your OLED Display. 
// Uncomment/Comment as needed.
GyverOLED<SSD1306_128x32, OLED_BUFFER> oled; // 0.6"
// GyverOLED<SSD1306_128x32, OLED_NO_BUFFER> oled;
// GyverOLED<SSD1306_128x64, OLED_BUFFER> oled;
//  GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> oled;
//  GyverOLED<SSH1106_128x64> oled; // 1.1"

boolean OLEDEnabled = false;

// GNSS
struct
{
  char GPS_Buffer[80];
  bool isGetData;     // GPS Aquasition
  bool isParseData;   // Parsing Complete
  char UTCTime[11];   // UTC Time
  char latitude[11];  // Latitude
  char N_S[2];        // N/S
  char longitude[12]; // Longitude
  char E_W[2];        // E/W
  char UTCDate[11];   // UTC Date
  bool isUsefull;     // Location Valid
} Save_Data;

const unsigned int gpsRxBufferLength = 600;
char gpsRxBuffer[gpsRxBufferLength];
unsigned int ii = 0;

int CycleCount = 0;

// IMU
#define PERFORM_CALIBRATION // Comment Out - to disable startup calibration
#define IMU_ADDRESS 0x6A
LSM6DS3 IMU;
calData calib = {0}; // Calibration Data
AccelData accelData; // Sensor Data
GyroData gyroData;   // Gyro Data
MagData magData;     // Magnetometer Data

// **************** FUNCTIONS AND ROUTINES ****************

// I2C Bus Scanner
void ScanI2CBus()
{

  // Useful Reference https://i2cdevices.org/addresses

  byte error, address;
  int nDevices;
  OLEDEnabled = false;

  Serial.println("Scanning I2C Bus for Devices for Auto Configuration ...\n");

  nDevices = 0;
  for (address = 1; address < 127; address++)
  {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    nDevices++;
    if (nDevices == 1)
      Serial.println(" I2C Possible Device(s) found at Address:");
    if (error == 0)
    {
      Serial.print(" * Hex 0x");
      Serial.print(address, HEX);

      Serial.print(" (");
      if (address < 16)
      {
        Serial.print("0");
      }
      Serial.print(address);
      Serial.print(")\t");

      switch (address)
      {
      case 0x10 ... 0x1F:
        Serial.print(" PCA9671 GPIO Expander");
        break;
      case 0x20:
        Serial.print(" PCA9671 GPIO Expander (Default)");
        break;
      case 0x21 ... 0x2F:
        Serial.print(" PCA9671 GPIO Expander");
        break;
      // case 0x50 ... 0x68: // Commented Out to Stop 'Case' Conflicts
      //   Serial.print(" PCA9671 GPIO Expander");
      //   break;
      case 0x48:
        Serial.print(" TLA2024 12-bit ADC");
        break;
      case 0x4A:
        Serial.print(" TMP102 Temperature Sensor");
        break;
      case 0x4B:
        Serial.print(" ADS1115 16-bit ADC");
        break;
      case 0x3C ... 0x3D:
        Serial.print(" OLED");
        OLEDEnabled = true;
        break;
      case 0x50:
        Serial.print(" EEPROM");
        break;
      case 0x55:
        Serial.print(" EEPROM IPEM PiHat");
        break;
      case 0x58:
        Serial.print(" EEPROM (Reserved Address)");
        break;
      case 0x60 ... 0x62:
        Serial.print(" MCP4728 12-bit DAC");
        break;
      case 0x68:
        Serial.print(" DS3231 RTC IPEM PiHat");
        break;
      case 0x6A:
        Serial.print(" LSM6DSL IMU MEMS");
        break;
      case 0x71 ... 0x77:
        Serial.print(" PCA9671 GPIO Expander");
        break;
      case 0x7C:
        Serial.print(" PCA9671 GPIO Expander (Reserved Address)");
        break;
      default:
        Serial.print(" ?");
        break;
      }
      Serial.println();
    }
    else if (error == 4)
    {
      Serial.print(" * Unknown error at address Decimal ");
      if (address < 16)
      {
        Serial.print("0");
      }
      Serial.print(address);
      Serial.print(" = Hex 0x");
      Serial.println(address, HEX);
    }
  }
  if (nDevices == 0)
  {
    Serial.println(" * No I2C devices found. Possible Hardware Issue?");
  }
  else
  {
    Serial.println(" I2C Bus Scan Complete\n");
  }
} // ScanI2CBus

// Initialize TLA2024
void Initialise_TLA2024()
{
  // TLA20024
  if (!tla.begin())
  {
    Serial.println("Failed to find TLA2024 chip");
    while (1)
    {
      delay(10);
    }
  }
  Serial.println("TLA2024 Found!");

  //  tla.setDataRate(TLA202x_RATE_1600_SPS);
  Serial.print("Data rate set to: ");
  switch (tla.getDataRate())
  {
  case TLA202x_RATE_128_SPS:
    Serial.println("128 SPS");
    break;
  case TLA202x_RATE_250_SPS:
    Serial.println("250 SPS");
    break;
  case TLA202x_RATE_490_SPS:
    Serial.println("490 SPS");
    break;
  case TLA202x_RATE_920_SPS:
    Serial.println("920 SPS");
    break;
  case TLA202x_RATE_1600_SPS:
    Serial.println("1600 SPS");
    break;
  case TLA202x_RATE_2400_SPS:
    Serial.println("2400 SPS");
    break;
  case TLA202x_RATE_3300_SPS:
    Serial.println("3300 SPS");
    break;
  }

  // tla.setRange(TLA202x_RANGE_6_144_V);
  Serial.print("Measurement range set to: ");
  switch (tla.getRange())
  {
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
  switch (tla.getMux())
  {
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
  switch (tla.getMode())
  {
  case TLA202x_MODE_ONE_SHOT:
    Serial.println("One-shot");
    break;
  case TLA202x_MODE_CONTINUOUS:
    Serial.println("Continuous");
    break;
  }
}

void EEPROM_Test()
{
  // Initialize EEPROM
  ee.begin();
  if (!ee.isConnected())
  {
    Serial.println("ERROR: Can't find eeprom (stopped)...");
  }

  Serial.print("Size of Measurement: \t");
  Serial.println(sizeof(measurement));

  Serial.print("\n");

  // Clear EEPROM part
  ee.setBlock(0, 0, sizeof(measurement));

  // Mock Measurements
  measurement.temperature = 22.5;
  measurement.humidity = 53.1;
  measurement.pressure = 1000.9;

  // Store Measurements in EEPROM
  start = micros();
  ee.writeBlock(0, (uint8_t *)&measurement, sizeof(measurement));
  duration = micros() - start;
  Serial.print("EEPROM Write: \t");
  Serial.println(duration);
  delay(10);

  // Clear Measurements Struct
  measurement.temperature = 0;
  measurement.humidity = 0;
  measurement.pressure = 0;

  // Read Measurements from EEPROM
  start = micros();
  ee.readBlock(0, (uint8_t *)&measurement, sizeof(measurement));
  duration = micros() - start;
  Serial.print("EEPROM Read: \t");
  Serial.println(duration);
  delay(10);

  Serial.println("\nMock Example Measurements\n");

  Serial.print("Temperature:\t");
  Serial.println(measurement.temperature);
  Serial.print("Humidity:\t");
  Serial.println(measurement.humidity);
  Serial.print("Pressure:\t");
  Serial.println(measurement.pressure);

  Serial.print("\n");

  // Write Test
  totals = 0;
  Serial.print("Test timing writeByte\t");
  start = micros();
  ee.writeByte(10, 1);
  duration = micros() - start;
  Serial.print("TIME: ");
  Serial.println(duration);
  totals += duration;

  // Read Test
  Serial.print("Test timing readByte\t");
  start = micros();
  ee.readByte(10);
  duration = micros() - start;
  Serial.print("TIME: ");
  Serial.println(duration);
  totals += duration;

  Serial.print("Test Timing Total\tTIME: ");
  Serial.println(totals);

} // EEPROM_Test

// GNSS Test
void errorLog(int num)
{
  Serial.print("ERROR");
  Serial.println(num);
  while (1)
  {
    digitalWrite(LED_Red, HIGH);
    delay(500);
    digitalWrite(LED_Red, LOW);
    delay(500);
  }
}

void printGpsBuffer()
{

  if (Save_Data.isParseData)
  {
    Save_Data.isParseData = false;

    Serial.print("Save_Data.UTCTime = \t");
    Serial.println(Save_Data.UTCTime);

    // if (Save_Data.isUsefull) {
    Save_Data.isUsefull = false;
    Serial.print("Save_Data.latitude = \t");
    Serial.println(Save_Data.latitude);
    Serial.print("Save_Data.N_S = \t");
    Serial.println(Save_Data.N_S);
    Serial.print("Save_Data.longitude = \t");
    Serial.println(Save_Data.longitude);
    Serial.print("Save_Data.E_W = \t");
    Serial.println(Save_Data.E_W);
    Serial.print("Save_Data.UTCDate = \t");
    Serial.println(Save_Data.UTCDate);

    if (OLEDEnabled == true)
    {
      // oled.clear();
      oled.setCursor(5, 0);
      oled.setScale(1);
      oled.print("ESPRanger GNSS Test");

      oled.setCursor(0, 1);
      oled.setScale(1);
      oled.print("Raw Time: ");
      oled.print(Save_Data.UTCTime);

      oled.setCursor(0, 2);
      oled.setScale(1);
      oled.print("Raw Date: ");
      oled.print(Save_Data.UTCDate);

      // oled.update();

      oled.setCursor(0, 3);
      oled.setScale(1);
      oled.print("  * GPS DATA OK *   ");
      oled.update();
    }
    else
    {
      Serial.println("Awaiting Signal Buffer");
      if (OLEDEnabled == true)
      {
        oled.clear();
        oled.setCursor(5, 0);
        oled.setScale(1);
        oled.print("ESPRanger GNSS Test");

        oled.setCursor(20, 2);
        oled.setScale(1);
        oled.print("+ Please Wait +");

        oled.setCursor(0, 3);
        oled.setScale(1);
        oled.print("Awaiting Signal Buffer");

        Serial.println("Awaiting Signal Buffer");
        oled.update();
      }
    }
  }
} // printGpsBuffer

void parseGpsBuffer()
{
  char *subString;
  char *subStringNext;
  if (Save_Data.isGetData)
  {
    Save_Data.isGetData = false;
    Serial.println("**************");
    Serial.println(Save_Data.GPS_Buffer);

    for (int i = 0; i <= 9; i++)
    {
      if (i == 0)
      {
        if ((subString = strstr(Save_Data.GPS_Buffer, ",")) == NULL)
          errorLog(1);
      }
      else
      {
        subString++;
        if ((subStringNext = strstr(subString, ",")) != NULL)
        {
          char usefullBuffer[2];
          switch (i)
          {
          case 1:
            memcpy(Save_Data.UTCTime, subString, subStringNext - subString);
            break;
          case 2:
            memcpy(usefullBuffer, subString, subStringNext - subString);
            break;
          case 3:
            memcpy(Save_Data.latitude, subString, subStringNext - subString);
            break;
          case 4:
            memcpy(Save_Data.N_S, subString, subStringNext - subString);
            break;
          case 5:
            memcpy(Save_Data.longitude, subString, subStringNext - subString);
            break;
          case 6:
            memcpy(Save_Data.E_W, subString, subStringNext - subString);
            break;
          case 9:
            memcpy(Save_Data.UTCDate, subString, subStringNext - subString);
            break;
          default:
            break;
          }

          subString = subStringNext;
          Save_Data.isParseData = true;
          if (usefullBuffer[0] == 'A')
            Save_Data.isUsefull = true;
          else if (usefullBuffer[0] == 'V')
            Save_Data.isUsefull = false;
        }
        else
        {
          errorLog(2);
        }
      }
    }
  }
} // parseGpsBuffer

void clrGpsRxBuffer(void)
{
  memset(gpsRxBuffer, 0, gpsRxBufferLength);
  ii = 0;
} // clrGpsRxBuffer

void gpsRead()
{
  while (Serial1.available())
  {
    gpsRxBuffer[ii++] = Serial1.read();
    if (ii == gpsRxBufferLength)
      clrGpsRxBuffer();
  }

  char *GPS_BufferHead;
  char *GPS_BufferTail;
  if ((GPS_BufferHead = strstr(gpsRxBuffer, "$GPRMC,")) != NULL || (GPS_BufferHead = strstr(gpsRxBuffer, "$GNRMC,")) != NULL)
  {
    if (((GPS_BufferTail = strstr(GPS_BufferHead, "\r\n")) != NULL) && (GPS_BufferTail > GPS_BufferHead))
    {
      memcpy(Save_Data.GPS_Buffer, GPS_BufferHead, GPS_BufferTail - GPS_BufferHead);
      Save_Data.isGetData = true;

      clrGpsRxBuffer();
    }
  }
} // gpsRead

// IMU Calibration
void IMUCalibration()
{

  // Turn off RGB
  ws2812b.clear();
  ws2812b.show();

  Serial.println("FastIMU calibration & data example");

  if (IMU.hasMagnetometer())
  {
    delay(1000);
    Serial.println("Move IMU in figure 8 pattern until done.");
    delay(3000);
    IMU.calibrateMag(&calib);
    Serial.println("Magnetic calibration done!");
  }
  else
  {
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

  if (IMU.hasMagnetometer())
  {
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
} // IMUCalibration

// Initialise IMU
void InitialiseIMU()
{
  // IMU
  int err = IMU.init(calib, IMU_ADDRESS);
  if (err != 0)
  {
    Serial.print("Error initializing IMU: ");
    Serial.println(err);
    while (true)
    {
      ;
    }
  }

#ifdef PERFORM_CALIBRATION
  Serial.println("ESPRanger - IMU Calibration");
  IMUCalibration();
#endif

  // err = IMU.setGyroRange(500);      //USE THESE TO SET THE RANGE, IF AN INVALID RANGE IS SET IT WILL RETURN -1
  // err = IMU.setAccelRange(2);       //THESE TWO SET THE GYRO RANGE TO ±500 DPS AND THE ACCELEROMETER RANGE TO ±2g

  if (err != 0)
  {
    Serial.print("Error Setting range: ");
    Serial.println(err);
    while (true)
    {
      ;
    }
  }
} // InitialiseIMU

// **************** SETUP ****************
void setup()
{
  // Stabalise
  delay(250);

  // Initialise UART 0 - USB COM Port
  Serial.begin(115200, SERIAL_8N1, RXD0, TXD0); // U0
  while (!Serial)
    ;
  Serial.println("UART 0 Opened (USB COM Port)");

  // Initialise UART 1 - RS485 Port
  Serial1.begin(9600, SERIAL_8N1, RXLP, TXLP); // LP
  while (!Serial)
    ;
  Serial.println("UART 1 Opened (RS485 Port)");

  // Initialise RS485 UART Enable on GP23
  pinMode(ENLP, OUTPUT);

  // Initialise and Configure UART Matrix Select
  pinMode(UART_Select, OUTPUT);
  digitalWrite(UART_Select, HIGH);

  // Initialise RED LED on GP22
  pinMode(LED_Red, OUTPUT);

  // Initialise I2C
  Wire.begin(I2C_SDA, I2C_SCL);
  Wire.setClock(400000); // 400 kHz Clock

  // Initialise SPI and TX
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

  Serial.print("\nSending Packet\n");

  radio.setOutputPower(10);

  // Initialise FET on GP2
  pinMode(FET, OUTPUT);

  ws2812b.begin(); // Initialize WS2812B
  ws2812b.clear(); // Set all pixel colors OFF

  // Initialise TMP102
  if (!TempSensor.begin(0x4A))
  {
    Serial.println("Failed to initialize TMP102");
    while (1)
      ;
  }

  Serial.println("Temperature Sensor (TMP102) Initialised\n");

  // I2C Scan
  ScanI2CBus();

  // OLED
  if (OLEDEnabled == true)
  {
    oled.init();
    oled.clear();
    oled.setCursor(5, 0);
    oled.setScale(1);
    oled.print("ESPRanger OLED Test");
    oled.update();
  }

  Serial.println("ESPRanger - Example Code");

  // One Off Test
  EEPROM_Test();

  // Read ATGM336H
  // Force RS485 UART Driver Enable and Receiver is Enabled
  digitalWrite(UART_Select, LOW);
  gpsRead();
  parseGpsBuffer();
  printGpsBuffer();

  // Initialise ADC TLA2024
  Initialise_TLA2024();

  // Initialise IMU
  InitialiseIMU();
}

// **************** LOOP ****************
void loop()
{

  // TMP102 PCB Temperature
  TemperatureC = TempSensor.readTempC(); // Centigrade
  TemperatureF = TempSensor.readTempF(); // Fahrenheit

  Serial.println("TMP102 PCB Temperature: " + String(TemperatureC) + " °C");
  Serial.println("TMP102 PCB Temperature: " + String(TemperatureF) + " °F\n");

  // TLA2024 VDC
  Serial.print("Channel 0 VDC: ");
  Serial.print(tla.readOnce(TLA202x_CHANNEL_0));
  Serial.println(" Volts (VDC)");

  // Return sensor to sleep
  TempSensor.sleep();

  // Read GNSS
  Serial.print("\nReading GNSS");
  digitalWrite(UART_Select, LOW);
  gpsRead();
  parseGpsBuffer();
  printGpsBuffer();

  // Cycle FET
  digitalWrite(FET, LOW);
  delay(1000);
  digitalWrite(FET, HIGH);
  delay(1000);

  // Uncomment out below to enable TX Test
  // Serial.print("\nSending Packet\n");
  // Serial.print(F("startTransmit code "));
  // int transmissionState = radio.startTransmit("Hello World!");
  // Serial.println(transmissionState);
  // Serial.println("Test End\n");
  // digitalWrite(UART_Select, HIGH);
  // Serial1.write("Hello World - Test\n");

  // WS2812 RGB Test
  // Turn off all pixels for two seconds
  ws2812b.clear();
  ws2812b.show(); // update to the WS2812B Led Strip
  delay(250);

  // turn on all pixels to RED at the same time for two seconds
  for (int pixel = 0; pixel < NUM_PIXELS; pixel++)
  {                                                         // for each pixel
    ws2812b.setPixelColor(pixel, ws2812b.Color(150, 0, 0)); // it only takes effect if pixels.show() is called
  }
  ws2812b.show(); // update to the WS2812B Led Strip
  delay(250);

  // Turn off all pixels for two seconds
  ws2812b.clear();
  ws2812b.show(); // update to the WS2812B Led Strip
  delay(250);

  // turn on all pixels to BLUE at the same time for two seconds
  for (int pixel = 0; pixel < NUM_PIXELS; pixel++)
  {                                                         // for each pixel
    ws2812b.setPixelColor(pixel, ws2812b.Color(0, 0, 150)); // it only takes effect if pixels.show() is called
  }
  ws2812b.show(); // update to the WS2812B Led Strip
  delay(250);

  // Turn off all pixels for two seconds
  ws2812b.clear();
  ws2812b.show(); // update to the WS2812B Led Strip
  delay(250);

  // turn pixels to GREEN one-by-one with delay between each pixel
  for (int pixel = 0; pixel < NUM_PIXELS; pixel++)
  {                                                         // for each pixel
    ws2812b.setPixelColor(pixel, ws2812b.Color(0, 150, 0)); // it only takes effect if pixels.show() is called
  }
  ws2812b.show(); // update to the WS2812B Led Strip
  delay(250);     // 500ms pause between each pixel

  // IMU Test
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

  if (IMU.hasMagnetometer())
  {
    IMU.getMag(&magData);
    Serial.print("\tM\t");
    Serial.print(magData.magX);
    Serial.print("\t");
    Serial.print(magData.magY);
    Serial.print("\t");
    Serial.print(magData.magZ);
  }

  if (IMU.hasTemperature())
  {
    Serial.print("\tT\t");
    Serial.println(IMU.getTemp());
  }
  else
  {
    Serial.println();
  }
  Serial.println();
  delay(50);

  // Just some fun

  if (accelData.accelX < 0.02 || accelData.accelY < 1 || accelData.accelZ < 1 || gyroData.gyroX < 1 || gyroData.gyroY < 1 || gyroData.gyroZ)
  {
    // Turn off RGB
    ws2812b.clear();
    ws2812b.show();
  }

  if (accelData.accelX > 1)
  {
    // RED
    ws2812b.setPixelColor(0, ws2812b.Color(50, 0, 0));
    ws2812b.show();
    delay(250);
  }

  if (accelData.accelX < -0.1)
  {
    // BLUE
    ws2812b.setPixelColor(0, ws2812b.Color(0, 0, 50));
    ws2812b.show();
    delay(250);
  }

  if (gyroData.gyroZ > 1)
  {
    // GREEN
    ws2812b.setPixelColor(0, ws2812b.Color(0, 50, 0));
    ws2812b.show();
    delay(250);
  }

  if (gyroData.gyroZ < -0.1)
  {
    // RED
    ws2812b.setPixelColor(0, ws2812b.Color(50, 0, 0));
    ws2812b.show();
    delay(250);
  }

  // Flash RED LED
  digitalWrite(LED_Red, LOW);
  delay(250);
  digitalWrite(LED_Red, HIGH);
  delay(250);
  digitalWrite(LED_Red, LOW);
  delay(250);
  digitalWrite(LED_Red, HIGH);
}
