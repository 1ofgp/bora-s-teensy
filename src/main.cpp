#include <Arduino.h>
#include <Wire.h>
#include <TimeLib.h>
#include <SD.h>
#include <SPI.h>

//Click here to get the library: http://librarymanager/All#SparkFun_VCNL4040
#include "SparkFun_VCNL4040_Arduino_Library.h"
VCNL4040 proximitySensor;
#define NUMBER_OF_SENSORS 4
#define TIME_HEADER  "T"   // Header tag for serial time sync message
#define TIME_REQUEST  7    // ASCII bell character requests a time sync message

#define MUX_ADDR 0x70 //7-bit unshifted default I2C Address
int LED_pin = 13;

char filename[15];
File logfile;
char CardExists;
boolean SDStat = false;
const int chipSelect = BUILTIN_SDCARD;

time_t getTeensy3Time()
{
  return Teensy3Clock.get();
}

void enableMuxPort(byte portNumber)
{
  if (portNumber > 7) portNumber = 7;

  Wire.beginTransmission(MUX_ADDR);
  //Read the current mux settings
  Wire.requestFrom(MUX_ADDR, 1);
  if (!Wire.available()) return; //Error
  byte settings = Wire.read();

  //Set the wanted bit to enable the port
  settings |= (1 << portNumber);

  Wire.write(settings);
  Wire.endTransmission();
} 

//Disables a specific port number
void disableMuxPort(byte portNumber)
{
  if (portNumber > 7) portNumber = 7;

  Wire.beginTransmission(MUX_ADDR);
  //Read the current mux settings
  Wire.requestFrom(MUX_ADDR, 1);
  if (!Wire.available()) return; //Error
  byte settings = Wire.read();

  //Clear the wanted bit to disable the port
  settings &= ~(1 << portNumber);

  Wire.write(settings);
  Wire.endTransmission();
} 

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.println("SparkFun VCNL4040 Example");
  setSyncProvider(getTeensy3Time);
  // **************** Create an output file. ****************************
  int j;
  strcpy(filename, "BS_000.TXT");

  if (SD.begin(chipSelect))
  {
    Serial.println("QQQQQ");
    for (uint8_t i = 0; i < 1000; i++) 
    {
      j=i/100;
      filename[3] = '0' + j;
      j=i%100;
      filename[4] = '0' + j/10;
      j=j%10;
      filename[5] = '0' + j%10;
      // create if does not exist, do not open existing, write, sync after write
      if (! SD.exists(filename)) break;
    }
  } 

}


void loop() {
  unsigned int proxValue; 
  logfile = SD.open(filename, FILE_WRITE);
 //------time--------------
  Serial.print(now());
  Serial.print(" ");
  if (logfile)
  {
    logfile.print(now());
    logfile.print(" ");
    digitalWrite(LED_BUILTIN, HIGH);
  }
  //--------------------------
  enableMuxPort(0);
  proximitySensor.begin();
  proxValue = proximitySensor.getProximity();
  Serial.print(proxValue);
  Serial.print(" ");
  if (logfile)
  {
    logfile.print(proxValue);
    logfile.print(" ");
  }
  disableMuxPort(0);
  //-------------------
  enableMuxPort(1);
  proximitySensor.begin();
  proxValue = proximitySensor.getProximity();
  Serial.print(proxValue);
  Serial.print(" ");
  if (logfile)
  {
    logfile.print(proxValue);
    logfile.print(" ");
  }
  disableMuxPort(1);
//------------------------
  enableMuxPort(2);
  proximitySensor.begin();
  proxValue = proximitySensor.getProximity();
  Serial.print(proxValue);
  Serial.print(" ");
  if (logfile)
  {
    logfile.print(proxValue);
    logfile.print(" ");
  }
  disableMuxPort(2);
//------------------
  enableMuxPort(3);
  proximitySensor.begin();
  Serial.print(proxValue);
  Serial.println();
  if (logfile)
  {
    logfile.print(proxValue);
    logfile.println();
  }
  disableMuxPort(3); 



  logfile.flush();
  logfile.close();
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);

  }











