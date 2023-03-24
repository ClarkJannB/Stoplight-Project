/*
  Replace the total with sizeOf in the for loop
  Move the period type out of the conditional
  Debug mode
  Double check all greater than and less than or equal to signs
*/

#include "AdafruitIO_WiFi.h"

#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ESP8266WiFi.h>

///******************************* WIFI **************************************/
#define WIFI_SSID "TP-Link_51CA"
#define WIFI_PASS "password"

#define DAYLIGHTSAVINGS 1 //Change either to 1 or 0 depending on daylight savings. 0 = fall back (no daylight savings)

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

//Array defines
#define HOURS         0
#define MINUTES       1
#define DURATION      2
#define WARNINGTIME   3
#define PERIOD        4
#define SCHEDULE      5



const long utcOffsetInSeconds = -18000;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

char periodName[9][10] {"Advisory", "Period 1", "Period 2", "Period 3", "Period 4", "Period 5", "Period 6", "Lunch", "Activity"};


int scheduleType = REG; //<- variable that is changing


int nowTime = 460;
int i;

//{HOURS, MINS, PERIOD LENGTH, WARNING TIME, PERIOD TYPE, SCHEDULE}
uint8_t schedule[32][6] {
  {7, 45, 4, 0, ADV, REG},
  {7, 52, 56, 5, PR1, REG},
  {8, 51, 56, 5, PR2, REG},
  {9, 50, 56, 5, PR3, REG},
  {10, 51, 56, 5, PR4, REG},
  {11, 50, 30, 5, LUN, REG},
  {12, 23, 56, 5, PR5, REG},
  {13, 22, 56, 15, PR6, REG},

  {7, 45, 5, 0, ADV, HLF},
  {7, 53, 42, 5, PR1, HLF},
  {8, 38, 38, 5, PR2, HLF},
  {9, 19, 38, 5, PR3, HLF},
  {10, 0, 38, 5, PR4, HLF},
  {10, 41, 36, 5, PR5, HLF},
  {11, 22, 38, 15, PR6, HLF},

  {7, 45, 5, 0,  ADV, AAC},
  {7, 53, 48, 5, PR1, AAC},
  {8, 44, 48, 5, PR2, AAC},
  {9, 35, 44, 5, ACT, AAC},
  {10, 22, 48, 5, PR3, AAC},
  {11, 48, 48, 5, PR4, AAC},
  {12, 6, 30, 5, LUN, AAC},
  {12, 39, 48, 5, PR5, AAC},
  {13, 30, 48, 15, PR6, AAC},

  {7, 45, 30, 0, ADV, EXT},
  {8, 18, 52, 5, PR1, EXT},
  {9, 13, 52, 5, PR2, EXT},
  {10, 8, 52, 5, PR3, EXT},
  {11, 3, 55, 5, PR4, EXT},
  {11, 58, 30, 5, LUN, EXT},
  {12, 31, 52, 5, PR5, EXT},
  {13, 26, 52, 15, PR6, EXT},



};

// Converts the hours and minutes of the array into # of minutes since 12:00 am
int hoursMinToMinTotal(int hrs, int mns) {
  return hrs * 60 + mns;
};

uint8_t minTotalToHours (uint16_t minTotal) {
  return minTotal /= 60;
}

uint8_t minTotalToMins (uint16_t minTotal) {
  return minTotal %= 60;
}


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

  //  timeClient.update();
  //  int hrs = timeClient.getHours() + DAYLIGHTSAVINGS;
  //  int mins = timeClient.getMinutes();
  //  int secs = timeClient.getSeconds();
  //  int nowTime = hoursMinToMinTotal(hrs, mins);

  uint8_t periodValue = 0;
  nowTime++;
  int hrs = minTotalToHours(nowTime);
  int mins = minTotalToMins(nowTime);
  delay(100);



  Serial.print(hrs);
  Serial.print(":");
  Serial.print(mins);
  Serial.print(":");
  //  Serial.print(secs);
  //  Serial.println(" ");

  for (i = 0; i < 31; i++) {

    if (schedule[i][SCHEDULE] == scheduleType) {

      int periodStartTime = hoursMinToMinTotal(schedule[i][HOURS], schedule[i][MINUTES]);
      int periodEndTime = periodStartTime + schedule[i][DURATION];
      int periodWarningTime = periodEndTime - schedule[i][WARNINGTIME];
      int nextPeriodTime = hoursMinToMinTotal(schedule[i + 1][HOURS], schedule[i + 1][MINUTES]);

      //Class time turn on Green
      if (nowTime > periodStartTime && nowTime <= periodWarningTime) {
        //Lunch time turn on Red
        if (schedule[i][PERIOD] == LUN) {
          Serial.print("LUNCH RED");
          periodValue = i;
        }
        //Green light
        Serial.print("GREEN ");
        periodValue = i;
        
        //Warning time turn on Yellow
      } else if (nowTime > periodWarningTime && nowTime <= periodEndTime) {
        Serial.print("YELLOW ");
        periodValue = i;



        //Passing time
      } else if (nowTime > periodEndTime && nowTime <= nextPeriodTime) {
        Serial.print("RED ");
        periodValue = i;



      }

    }


  }
  Serial.print(nowTime);
  Serial.print(" ");
  Serial.print(periodName[schedule[periodValue][PERIOD]]);
  Serial.print(" ");
  Serial.print(periodValue);
  Serial.println(" ");


  if (nowTime > 900) {
    nowTime = 400;
  }

  //  delay(500);
}
