/*
  Replace the total with sizeOf in the for loop
  Move the period type out of the conditional
*/
// comment out the following lines if you are using fona or ethernet
#include "AdafruitIO_WiFi.h"

#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ESP8266WiFi.h>

///******************************* WIFI **************************************///
#define WIFI_SSID "TP-Link_51CA"
#define WIFI_PASS "password"

///************************ Adafruit IO Config *******************************///
// visit io.adafruit.com if you need to create an account,
// or if you need your Adafruit IO key.
#define IO_USERNAME "tisnotgonnawork"
#define IO_KEY "aio_YECe518ozUaevBk2vkEd9qMfggNC" // <- Adafruit might reset this from time to time so make sure to check this is the same
#if defined(USE_AIRLIFT) || defined(ADAFRUIT_METRO_M4_AIRLIFT_LITE) ||         \
    defined(ADAFRUIT_PYPORTAL)
// Configure the pins used for the ESP32 connection
#if !defined(SPIWIFI_SS) // if the wifi definition isnt in the board variant
// Don't change the names of these #define's! they match the variant ones
#define SPIWIFI SPI
#define SPIWIFI_SS 10 // Chip select pin
#define NINA_ACK 9    // a.k.a BUSY or READY pin
#define NINA_RESETN 6 // Reset pin
#define NINA_GPIO0 -1 // Not connected
#endif
AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS, SPIWIFI_SS,
                   NINA_ACK, NINA_RESETN, NINA_GPIO0, &SPIWIFI);
#else
AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);
#endif


///***************************** FEED(S) *************************************///
//trying to use only one variable to control all other vals
AdafruitIO_Feed *scheduleControl = io.feed("scheduleControl");


///*******  DEBUG MODE  *******///
//change from true to false to turn debug mode on or off
#define DEBUG false

#define DAYLIGHTSAVINGS   1 //Change either to 1 or 0 depending on daylight savings. 0 = fall back (no daylight savings)
#define GREEN_PIN         4
#define YELLOW_PIN        15
#define RED_PIN           5
#define LIGHT_ON          1
#define LIGHT_OFF         0

//Advisory, Lunch, Per 1-6,
//Period defines
#define ADV               0
#define PR1               1
#define PR2               2
#define PR3               3
#define PR4               4
#define PR5               5
#define PR6               6
#define LUN               7
#define ACT               8

//Schedule defines
#define REG               0
#define HLF               1
#define AAC               2
#define EXT               3

//Array defines
#define HOURS             0
#define MINUTES           1
#define DURATION          2
#define WARNINGTIME       3
#define PERIOD            4
#define SCHEDULE          5

//State defines
#define BEFORE            0
#define ENDDAY            1
#define CLASS             2
#define WARNIN            3
#define PASSIN            4
#define LUNCH             5

//Outschool time defines
#define DAYSTART          460
#define SCHOOLSTARTWARN   465
#define DAYEND            858
#define NOONEND           720
#define SATURDAY          6
#define SUNDAY            0

//time setup
//offset for time zone (in seconds)
const long utcOffsetInSeconds = -18000;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
char periodName[9][10] {"Advisory", "Period_1", "Period_2", "Period_3", "Period_4", "Period_5", "Period_6", "Lunch", "Activity"};
char scheduleName[4][12] {"Regular", "Half_Day", "AdvisoryAct", "ExtendedAdv"};
char stateName[6][7] {"BEFORE", "ENDDAY", "CLASS", "WARNIN", "PASSIN", "LUNCH"};

int scheduleType = REG; //<- variable that is changing

int lightState;

//DEBUG
int nowTime = 450;

//{HOURS, MINS, PERIOD LENGTH, WARNING TIME, PERIOD TYPE, SCHEDULE}
uint8_t schedule[32][6] {
  {7, 45, 4, 0, ADV, REG},
  {7, 52, 56, 5, PR1, REG},
  {8, 51, 56, 5, PR2, REG},
  {9, 50, 56, 5, PR3, REG},
  {10, 51, 56, 5, PR4, REG},
  {11, 50, 30, 5, LUN, REG},
  {12, 23, 56, 5, PR5, REG},
  {13, 22, 56, 13, PR6, REG},
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
  {11, 15, 48, 5, PR4, AAC},
  {12, 6, 30, 5, LUN, AAC},
  {12, 39, 48, 5, PR5, AAC},
  {13, 30, 48, 13, PR6, AAC},
  {7, 45, 30, 0, ADV, EXT},
  {8, 18, 52, 5, PR1, EXT},
  {9, 13, 52, 5, PR2, EXT},
  {10, 8, 52, 5, PR3, EXT},
  {11, 3, 55, 5, PR4, EXT},
  {11, 58, 30, 5, LUN, EXT},
  {12, 31, 52, 5, PR5, EXT},
  {13, 26, 52, 13, PR6, EXT},
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

  // connect to io.adafruit.com
  Serial.print("Connecting to Adafruit IO");
  io.connect();

  // set up a message handler for the feeds.
  // the handleMessage function (defined below)
  // will be called whenever a message is
  // received from adafruit io.
  scheduleControl->onMessage(scheduleControlValue);

  // wait for a connection (will print out . until connected to Adafruit IO)
  // make sure to change IO_USERNAME and IO_KEY  up top
  while (io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());

  //make sure all feeds get their current values right away
  scheduleControl->get();

  //Setup Pins (change value with defines up top)
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(RED_PIN, OUTPUT);

}

void loop() {
  int i;
  uint8_t periodValue = 0;
  io.run();

  //DEBUG
#if DEBUG == true
  nowTime++;
  int day = 4; //<- placeholder value for day
  int hrs = minTotalToHours(nowTime);
  int mins = minTotalToMins(nowTime);
  //resets nowTime counter
  if (nowTime > 900) {
    nowTime = 400;
  }
#else DEBUG == false
  timeClient.update();
  int day = timeClient.getDay();
  int hrs = timeClient.getHours() + DAYLIGHTSAVINGS;
  int mins = timeClient.getMinutes();
  int secs = timeClient.getSeconds();
  int nowTime = hoursMinToMinTotal(hrs, mins);
  delay(100);
#endif

  if ((day == SATURDAY) || (day == SUNDAY)) {
    digitalWrite(GREEN_PIN, LIGHT_OFF);
    digitalWrite(YELLOW_PIN, LIGHT_OFF);
    digitalWrite(RED_PIN, LIGHT_OFF);
  } else {
    //Determines if time is before school overall
    if (nowTime < DAYSTART) {
      lightState = BEFORE;
      //Turn on yellow before advisory
    } else if (nowTime >= DAYSTART && nowTime < SCHOOLSTARTWARN) {
      lightState = WARNIN;
      //First conditonal: check if time is outside of school for all schedules except half day
      //Second conditional: check if time is outside of school for noon release during half day
    } else if ((nowTime >= DAYEND && scheduleType != HLF) || (nowTime >= NOONEND && scheduleType == HLF)) {
      lightState = ENDDAY;
    } else {
      for (i = 0; i <= 31; i++) {
        if (schedule[i][SCHEDULE] == scheduleType) {
          int periodStartTime = hoursMinToMinTotal(schedule[i][HOURS], schedule[i][MINUTES]);
          int periodEndTime = periodStartTime + schedule[i][DURATION];
          int periodWarningTime = periodEndTime - schedule[i][WARNINGTIME];
          int nextPeriodTime = hoursMinToMinTotal(schedule[i + 1][HOURS], schedule[i + 1][MINUTES]);
          //Class time turn on Green
          if (nowTime >= periodStartTime && nowTime < periodWarningTime) {
            //Lunch time turn on Red
            if (schedule[i][PERIOD] == LUN) {
              lightState = LUNCH;
              periodValue = i;
            } else {
              //Green light
              lightState = CLASS;
              periodValue = i;
            }
            //Warning time turn on Yellow
          } else if (nowTime >= periodWarningTime && nowTime < periodEndTime) {
            lightState = WARNIN;
            periodValue = i;
            //Passing time
          } else if (nowTime >= periodEndTime && nowTime < nextPeriodTime) {
            lightState = PASSIN;
            periodValue = i;
          }
        }
      }
    }
  }

  switch (lightState) {
    case BEFORE:
      digitalWrite(GREEN_PIN, LIGHT_OFF);
      digitalWrite(YELLOW_PIN, LIGHT_OFF);
      digitalWrite(RED_PIN, LIGHT_OFF);
      break;
    case ENDDAY:
      digitalWrite(GREEN_PIN, LIGHT_OFF);
      digitalWrite(YELLOW_PIN, LIGHT_OFF);
      digitalWrite(RED_PIN, LIGHT_OFF);
      break;
    case CLASS:
      digitalWrite(GREEN_PIN, LIGHT_ON);
      digitalWrite(YELLOW_PIN, LIGHT_OFF);
      digitalWrite(RED_PIN, LIGHT_OFF);
      break;
    case WARNIN:
      digitalWrite(GREEN_PIN, LIGHT_OFF);
      digitalWrite(YELLOW_PIN, LIGHT_ON);
      digitalWrite(RED_PIN, LIGHT_OFF);
      break;
    case PASSIN:
      digitalWrite(GREEN_PIN, LIGHT_OFF);
      digitalWrite(YELLOW_PIN, LIGHT_OFF);
      digitalWrite(RED_PIN, LIGHT_ON);
      break;
    case LUNCH:
      digitalWrite(GREEN_PIN, LIGHT_ON);
      digitalWrite(YELLOW_PIN, LIGHT_OFF);
      digitalWrite(RED_PIN, LIGHT_ON);
      break;
  }

  Serial.print(hrs);
  Serial.print(':');
  Serial.print(mins);
  Serial.print(':');
  Serial.print(nowTime);
  Serial.print(' ');
  Serial.print(stateName[lightState]);
  Serial.print(' ');
  Serial.print(periodName[schedule[periodValue][PERIOD]]);
  Serial.print(' ');
  Serial.print(periodValue);
  Serial.print(' ');
  Serial.print(scheduleName[scheduleType]);
  Serial.print(' ');
  Serial.print(daysOfTheWeek[day]);
  Serial.println();

}


// this function is called whenever an 'scheduleControl' feed message
// is received from Adafruit IO. it was attached to
// the 'scheduleControl' feed in the setup() function above.
void scheduleControlValue(AdafruitIO_Data * data) {

  //convert data to integer
  int scheduleControlRead = data->toInt();
  Serial.print("received <- ");

  // we can use feedName() in order to find out which feed the message came from.
  Serial.print(data->feedName());
  Serial.print(" ");

  // print out the received value
  Serial.println(scheduleControlRead);

  // set the read value equal to the variable to change which schedule
  scheduleType = scheduleControlRead;

}
