#include "AdafruitIO_WiFi.h"

#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ESP8266WiFi.h>

///******************************* WIFI **************************************/
#define WIFI_SSID "TP-Link_51CA"
#define WIFI_PASS "password"

//Advisory, Lunch, Per 1-6,

//Period defines
#define ADV      0
#define PR1      1
#define PR2      2
#define PR3      3
#define PR4      4
#define PR5      5
#define PR6      6
#define LUN      7
#define ACT      8


//Schedule defines
#define REG     0
#define HLF     1
#define AAC     2
#define EXT     3


const long utcOffsetInSeconds = -18000;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);


int schedule[35][5] {
  {7, 45, 4, ADV, REG},
  {7, 52, 56, PR1, REG},
  {8, 51, 56, PR2, REG},
  {9, 50, 56, PR3, REG},
  {10, 51, 56, PR4, REG},
  {11, 50, 30, LUN, REG},
  {12, 23, 56, PR5, REG},
  {13, 22, 56, PR6, REG},
  {7, 45, 5, ADV, HLF},
  {7, 53, 42, PR1, HLF},
  {8, 38, 38, PR2, HLF},
  {9, 19, 38, PR3, HLF},
  {10, 0, 38, PR4, HLF},
  {10, 41, 36, PR5, HLF},
  {11, 22, 38, PR6, HLF},
  {7, 45, 5, ADV, AAC},
  {7, 53, 48, PR1, AAC},
  {8, 44, 48, PR2, AAC},
  {9, 35, 44, ACT, AAC},
  {10, 22, 48, PR3, AAC},
  {11, 48, 48, PR4, AAC},
  {12, 6, 30, LUN, AAC},
  {12, 39, 48, PR5, AAC},
  {13, 30, 48, PR6, AAC},
  {7, 45, 30, ADV, EXT},
  {8, 18, 52, PR1, EXT},
  {9, 13, 52, PR2, EXT},
  {10, 8, 52, PR3, EXT},
  {11, 3, 55, PR4, EXT},
  {11, 58, 30, LUN, EXT},
  {12, 31, 52, PR5, EXT},
  {13, 26, 52, PR6, EXT},



};

void setup() {
  Serial.begin(115200);
  
  // start the wifi connection
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  // will print out . until connected
  // make sure to change WIFI_SSID and WIFI_PASS up top
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }
  
  timeClient.begin();
}

void loop() {
  
  timeClient.update();
  Serial.print(timeClient.getHours());
  Serial.print(":");
  Serial.print(timeClient.getMinutes());
  Serial.print(":");
  Serial.print(timeClient.getSeconds());
  Serial.println(" ");

}
