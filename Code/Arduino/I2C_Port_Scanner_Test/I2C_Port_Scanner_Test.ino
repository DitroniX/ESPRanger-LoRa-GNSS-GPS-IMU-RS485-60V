/*
  Dave Williams, DitroniX 2019-2025 (ditronix.net)

  Example Code, to demonstrate and test the ESPRanger I2C Port Devices Address Scanner

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

// **************** USER VARIABLES / DEFINES / STATIC / STRUCTURES / CONSTANTS ****************

// Define I2C (Expansion Port)
#define I2C_SDA 6
#define I2C_SCL 7

// **************** FUNCTIONS AND ROUTINES ****************

// I2C Bus Scanner
void ScanI2CBus()
{

  // Useful Reference https://i2cdevices.org/addresses

  byte error, address;
  int nDevices;

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
        Serial.print(" TMP102 Temperature Sensor");
        break;
      case 0x4A:
        Serial.print(" TMP102 Temperature Sensor");
        break;
      case 0x4B:
        Serial.print(" ADS1115 16-bit ADC IPEM PiHat");
        break;
      case 0x3C ... 0x3D:
        Serial.print(" OLED");
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
        Serial.print(" MCP4728 12-bit DAC IPEM PiHat");
        break;
      case 0x68:
        Serial.print(" DS3231 RTC IPEM PiHat");
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

// **************** SETUP ****************
void setup()
{

  // Stabalise (Optional)
  delay(250);

  // Initialise UART
  Serial.begin(115200, SERIAL_8N1); // 115200
  while (!Serial)
    ;
  Serial.println("");

  // Initialize I2C
  Wire.begin(I2C_SDA, I2C_SCL);

  Serial.println("ESPRanger - Example Code");

  ScanI2CBus();
}

// **************** LOOP ****************
void loop()
{
}
