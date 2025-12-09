/*
  Dave Williams, DitroniX 2019-2025 (ditronix.net)

  Example Code, to demonstrate and test the ESPRanger Basic ATGM336H Comms Test with OLED
  This code does not use external libraries for GNSS

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

// **************** USER VARIABLES / DEFINES / STATIC / STRUCTURES / CONSTANTS ****************

// OLED Instance. You will need to select your OLED Display. 
// Uncomment/Comment as needed.
GyverOLED<SSD1306_128x32, OLED_BUFFER> oled;  // 0.6"
//GyverOLED<SSD1306_128x32, OLED_NO_BUFFER> oled;
//GyverOLED<SSD1306_128x64, OLED_BUFFER> oled;
// GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> oled;
// GyverOLED<SSH1106_128x64> oled; // 1.1"

struct
{
  char GPS_Buffer[80];
  bool isGetData;      // GPS Aquasition
  bool isParseData;    // Parsing Complete
  char UTCTime[11];    // UTC Time
  char latitude[11];   // Latitude
  char N_S[2];         // N/S
  char longitude[12];  // Longitude
  char E_W[2];         // E/W
  char UTCDate[11];    // UTC Date
  bool isUsefull;      // Location Valid
} Save_Data;

const unsigned int gpsRxBufferLength = 600;
char gpsRxBuffer[gpsRxBufferLength];
unsigned int ii = 0;

int CycleCount = 0;

// **************** OUTPUTS ****************
#define LED_Red 22      // Red LED
#define UART_Select 15  // UART Matrix High=RS485  Low=GNSS (Defaults to GNSS)

// Define I2C (Expansion Port)
#define I2C_SDA 6
#define I2C_SCL 7

// Hardware Serial 0 pins
#define RXD0 17
#define TXD0 16

// Hardware Serial 1 pins - UART
#define RXLP 4
#define TXLP 5

// **************** STRUCTURES, FUNCTIONS AND ROUTINES ****************

void errorLog(int num) {
  Serial.print("ERROR");
  Serial.println(num);
  while (1) {
    digitalWrite(LED_Red, HIGH);
    delay(500);
    digitalWrite(LED_Red, LOW);
    delay(500);
  }
}

void printGpsBuffer() {

  // OLED
  // oled.clear();
  oled.setCursor(5, 0);
  oled.setScale(1);
  oled.print("ESPRanger GNSS Test");

  if (Save_Data.isParseData) {
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

    // } else {

    //   oled.clear();
    //   oled.setCursor(5, 0);
    //   oled.setScale(1);
    //   oled.print("ESPRanger GNSS Test");

    //   oled.setCursor(20, 2);
    //   oled.setScale(1);
    //   oled.print("+ Please Wait +");

    //   oled.setCursor(0, 3);
    //   oled.setScale(1);
    //   oled.print("No GPS DATA OR ERROR");

    //   Serial.println("No GPS DATA OR ERROR");
    // }
  }

  oled.update();
}

void parseGpsBuffer() {
  char* subString;
  char* subStringNext;
  if (Save_Data.isGetData) {
    Save_Data.isGetData = false;
    Serial.println("**************");
    Serial.println(Save_Data.GPS_Buffer);

    for (int i = 0; i <= 9; i++) {
      if (i == 0) {
        if ((subString = strstr(Save_Data.GPS_Buffer, ",")) == NULL)
          errorLog(1);
      } else {
        subString++;
        if ((subStringNext = strstr(subString, ",")) != NULL) {
          char usefullBuffer[2];
          switch (i) {
            case 1: memcpy(Save_Data.UTCTime, subString, subStringNext - subString); break;
            case 2: memcpy(usefullBuffer, subString, subStringNext - subString); break;
            case 3: memcpy(Save_Data.latitude, subString, subStringNext - subString); break;
            case 4: memcpy(Save_Data.N_S, subString, subStringNext - subString); break;
            case 5: memcpy(Save_Data.longitude, subString, subStringNext - subString); break;
            case 6: memcpy(Save_Data.E_W, subString, subStringNext - subString); break;
            case 9: memcpy(Save_Data.UTCDate, subString, subStringNext - subString); break;
            default: break;
          }

          subString = subStringNext;
          Save_Data.isParseData = true;
          if (usefullBuffer[0] == 'A')
            Save_Data.isUsefull = true;
          else if (usefullBuffer[0] == 'V')
            Save_Data.isUsefull = false;

        } else {
          errorLog(2);
        }
      }
    }
  }
}

void gpsRead() {
  while (Serial1.available()) {
    gpsRxBuffer[ii++] = Serial1.read();
    if (ii == gpsRxBufferLength) clrGpsRxBuffer();
  }

  char* GPS_BufferHead;
  char* GPS_BufferTail;
  if ((GPS_BufferHead = strstr(gpsRxBuffer, "$GPRMC,")) != NULL || (GPS_BufferHead = strstr(gpsRxBuffer, "$GNRMC,")) != NULL) {
    if (((GPS_BufferTail = strstr(GPS_BufferHead, "\r\n")) != NULL) && (GPS_BufferTail > GPS_BufferHead)) {
      memcpy(Save_Data.GPS_Buffer, GPS_BufferHead, GPS_BufferTail - GPS_BufferHead);
      Save_Data.isGetData = true;

      clrGpsRxBuffer();
    }
  }
}

void clrGpsRxBuffer(void) {
  memset(gpsRxBuffer, 0, gpsRxBufferLength);
  ii = 0;
}

// **************** SETUP ****************
void setup() {

  // Stabalise
  delay(250);

  // Initialise UART 0 - USB COM Port
  Serial.begin(115200, SERIAL_8N1, RXD0, TXD0);  // U0
  while (!Serial)
    ;
  Serial.println("UART 0 Opened (USB COM Port)");

  // Initialise UART 1 - RS485 Port
  Serial1.begin(9600, SERIAL_8N1, RXLP, TXLP);  //LP
  while (!Serial1)
    ;
  Serial.println("UART 1 Opened (GNSS Port)");

  // Initialize I2C
  Wire.begin(I2C_SDA, I2C_SCL);

  // Initialise RED LED on GP22
  pinMode(LED_Red, OUTPUT);
  digitalWrite(LED_Red, LOW);

  // Initialise and Configure UART Matrix Select.  Optional as ESPRanger defaults to GNSS with pull down.
  pinMode(UART_Select, OUTPUT);
  digitalWrite(UART_Select, LOW);  // GNSS ATGM336H

  // States
  Save_Data.isGetData = false;
  Save_Data.isParseData = false;
  Save_Data.isUsefull = false;

  // OLED
  oled.init();
  oled.clear();

  Serial.println("ESPRanger - Example Code\n");
}

// **************** LOOP ****************
void loop() {

  // Read ATGM336H
  gpsRead();
  parseGpsBuffer();
  printGpsBuffer();

  // Heartbeat RED LED
  if (CycleCount == 0) {
    digitalWrite(LED_Red, HIGH);
    delay(50);
    digitalWrite(LED_Red, LOW);
  }

  // Heartbeat Loop Counter
  CycleCount++;
  if (CycleCount >= 50) CycleCount = 0;
}
