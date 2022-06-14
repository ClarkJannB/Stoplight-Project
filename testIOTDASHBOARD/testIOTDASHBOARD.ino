#include "thingProperties.h"

void setup() {
  // Initialize serial and wait for port to open:
  Serial.begin(9600);
  // This delay gives the chance to wait for a Serial Monitor without blocking if none is found
  delay(1500);

  // Defined in thingProperties.h
  initProperties();

  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);

  /*
     The following function allows you to obtain more information
     related to the state of network and IoT Cloud connection and errors
     the higher number the more granular information you’ll get.
     The default is 0 (only errors).
     Maximum is 4
  */
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();


  pinMode(GREEN, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(RED, OUTPUT);
}

void loop() {
  ArduinoCloud.update();
  // Your code here
  Serial.print("GREEN: "); 
  Serial.print(digitalRead(GREEN));
  Serial.print(" YELLOW: ");
  Serial.print(digitalRead(YELLOW));
  Serial.print(" RED: ");
  Serial.println(digitalRead(RED)); 

  delay(1000);

}

void onGreenChange()  {
  //Serial.print(" GREEN ON");
  digitalWrite(GREEN, !digitalRead(GREEN));
}

void onRedChange()  {
  //Serial.print(" RED ON");
  digitalWrite(RED, !digitalRead(RED));
}

void onYellowChange()  {
  //Serial.print(" YELLOW ON ");
  digitalWrite(YELLOW, !digitalRead(YELLOW));
}
