#include <Arduino.h>
#include <Wire.h>
#include <TimeLib.h>

//Click here to get the library: http://librarymanager/All#SparkFun_VCNL4040
#include "SparkFun_VCNL4040_Arduino_Library.h"
VCNL4040 proximitySensor;
#define NUMBER_OF_SENSORS 4
#define TIME_HEADER  "T"   // Header tag for serial time sync message
#define TIME_REQUEST  7    // ASCII bell character requests a time sync message

#define MUX_ADDR 0x70 //7-bit unshifted default I2C Address
int LED_pin = 13;
//Enables a specific port number

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
  // put your setup code here, to run once:
   Serial.begin(115200);
   pinMode(LED_BUILTIN, OUTPUT);
  Serial.println("SparkFun VCNL4040 Example");
  setSyncProvider(getTeensy3Time);

//  Wire.begin(); //Join i2c bus

 /* if (proximitySensor.begin() == false)
  {
    Serial.println("Device not found. Please check wiring.");
    while (1); //Freeze!
  } */
  
/*for (byte x = 0; x < NUMBER_OF_SENSORS; x++)
  {
    enableMuxPort(x); //Tell mux to connect to port X
    disableMuxPort(x);
} */

}


void loop() {
 unsigned int proxValue; 
  // put your main code here, to run repeatedly:
   //Get proximity value. The value ranges from 0 to 65535
  //so we need an unsigned integer or a long.
  //unsigned int proxValue = proximitySensor.getProximity(); 
Serial.print(now());
Serial.print(" ");
 enableMuxPort(0);
  proximitySensor.begin();
  proxValue = proximitySensor.getProximity();
    Serial.print(proxValue);
  Serial.print(" ");
  disableMuxPort(0);

  enableMuxPort(1);
  proximitySensor.begin();
  proxValue = proximitySensor.getProximity();
    Serial.print(proxValue);
  Serial.print(" ");
  disableMuxPort(1);

  enableMuxPort(2);
  proximitySensor.begin();
  proxValue = proximitySensor.getProximity();
   Serial.print(proxValue);
    Serial.print(" ");
  disableMuxPort(2);

  enableMuxPort(3);
  proximitySensor.begin();
    Serial.print(proxValue);
  Serial.println();
  disableMuxPort(3); 


  digitalWrite(LED_pin, HIGH);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  digitalWrite(LED_pin, LOW);
   digitalWrite(LED_BUILTIN, LOW);
  delay(500); 
  

}












