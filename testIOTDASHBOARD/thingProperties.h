
#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>
#define GREEN 2
#define YELLOW 21
#define RED 13
#define LIGHT_ON HIGH
#define LIGHT_OFF LOW

const char SSID[]     = "OpenWrt";    // Network SSID (name)
const char PASS[]     = "mvthsstudent";    // Network password (use for WPA, or use as key for WEP)

void onGreenChange();
void onRedChange();
void onYellowChange();

CloudLight green;
CloudLight red;
CloudLight yellow;

void initProperties(){

  ArduinoCloud.addProperty(green, READWRITE, ON_CHANGE, onGreenChange);
  ArduinoCloud.addProperty(red, READWRITE, ON_CHANGE, onRedChange);
  ArduinoCloud.addProperty(yellow, READWRITE, ON_CHANGE, onYellowChange);

}

WiFiConnectionHandler ArduinoIoTPreferredConnection(SSID, PASS);
