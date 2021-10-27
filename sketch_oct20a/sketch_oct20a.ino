/*
This is a demo for the Microchip MCP9808 Temperature Sensor. We used the Adafruit
MCP9808 breakout board. Click on this link to go to the Adafruit website for more 
details about this board.
http://www.adafruit.com/products/1782

For information about hooking up this board to the Arduino then click on this link 
to visit the hookup guide on the cactus.io website.
http://cactus.io/hookups/sensors/temperature-humidity/mcp9808/hookup-arduino-to-mcp9808-temperature-sensor
*/

#include <Wire.h>
#include "cactus_io_MCP9808.h"

// Create a MCP9808 sensor object
MCP9808 mcp = MCP9808();

void setup() {
  Serial.begin(9600);
  Serial.println("Microchip MCP9808 Temperature Sensor | cactus.io");
  Serial.println("Temp (C)\tTemp (F)");
  
  // Make sure the sensor is found
  if (!mcp.begin()) {
    Serial.println("Couldn't find MCP9808!");
    while (1);
  }
}

void loop() {
  
    // wake up MSP9808
    mcp.sensorPowerUp();
  
    mcp.readSensor();
  
    Serial.print(mcp.getTemperature_C()); Serial.print(" *C\t"); 
    Serial.print(mcp.getTemperature_F()); Serial.println(" *F");
    delay(250);
  
    // Shutdown MCP9808 to reduce power consumption
    mcp.sensorPowerDown();
    
    delay(2000);
   
}
