#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#define DAYLIGHTSAVINGS 1
#define GREEN_PIN 5
#define YELLOW_PIN 15
#define RED_PIN 16
#define LIGHT_ON 1
#define LIGHT_OFF 0
#define WARNING_TIME 5

//Wifi Credentials (edit as required)
const char *ssid     = "TP-Link_51CA";
const char *password = "password";

const long utcOffsetInSeconds = -18000;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

// The times of the schedules

int schStart = 0;
int schEnd = 15;
int schJump = 7; //how many periods there are in the day, how many times it jumps
int lunch = 10;

int schIndex = 16;
int period = schIndex;
int nowTime = 460;

//int currentPeriod = 0;
//int period = 0;

// Starting/end times of the period
int schReg[65][2] {

  //REGULAR TIME SCHEDULE:
  //Index (0 - 15)
  // Block 0 (ADVISORY)
  {7, 45},  //465 
  {7, 49},  //469
  // Block 1 (PERIOD 1)
  {7, 52},  //472
  {8, 48},  //528
  // Block 2 (PERIOD 2)
  {8, 51},  //531
  {9, 47},  //587
  // Block 3 (PERIOD 3)
  {9, 50},  //590
  {10, 46}, //640
  // Block 4 (PERIOD 4)
  {10, 51}, //651
  {11, 47}, //707
  // Block 5 (LUNCH)
  {11, 50}, //710
  {12, 18}, //738
  // Block 6 (PERIOD 5)
  {12, 23}, //743
  {13, 19}, //799
  // Block 7 (PERIOD 6)
  {13, 22}, //802
  {14, 18}, //858

};

// Converts the hours and minutes of the array into # of minutes
int convert_time(int hrs, int mns) {
  int conv_time;
  conv_time = hrs * 60;
  conv_time = conv_time + mns;
  return conv_time;
};


void setup() {

  Serial.begin(115200);
  int conv_time;
  int i;

  WiFi.begin(ssid, password);
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }
  timeClient.begin();

  //Setup Pins:
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(RED_PIN, OUTPUT);


  int hrs = timeClient.getHours() + DAYLIGHTSAVINGS;
  int mins = timeClient.getMinutes();
  int secs = timeClient.getSeconds();
  convert_time(hrs, mins);
  int nowTime = conv_time;



  // Gets initial block
  for ( i = 0; i < 8; i += 2) {
    int startPeriod = convert_time(schReg[i][0], schReg[i][1]);
    int nextPeriod = convert_time(schReg[i + 2][0], schReg[i + 2][1]);

    if (nowTime >= startPeriod && nowTime <= nextPeriod) {
      break;
    }

  }

  period = i;

}

void loop() {
  timeClient.update();

  Serial.print(daysOfTheWeek[timeClient.getDay()]);
  Serial.print(", ");
  Serial.print(timeClient.getHours() + DAYLIGHTSAVINGS);
  Serial.print(":");
  Serial.print(timeClient.getMinutes());
  Serial.print(":");
  Serial.println(timeClient.getSeconds());
  //Serial.println(timeClient.getFormattedTime());

  delay(1000);

  int conv_time;
  int hrs = timeClient.getHours() + DAYLIGHTSAVINGS;
  int mins = timeClient.getMinutes();
  int sec = timeClient.getSeconds();
  int nowTime = convert_time(hrs, mins);

  // Gets time values for current period
  int startPeriod = convert_time(schReg[period][0], schReg[period][1]); //green is on between startPeriod and endPeriod - 5
  int endPeriod = convert_time(schReg[period + 1][0], schReg[period + 1][1]); // yellow is on between endPeriod - 5 and endPeriod
  int nextPeriod = convert_time(schReg[period + 2][0], schReg[period + 2][1]);  //red is on between endPeriod and nextPeriod

  // Function to turn off the lights when theres no school
  if (nowTime < convert_time(schReg[schStart][0], schReg[schStart][1]) || nowTime > convert_time(schReg[schEnd][0], schReg[schEnd][1])) {
    digitalWrite(GREEN_PIN, LIGHT_OFF);
    digitalWrite(YELLOW_PIN, LIGHT_OFF);
    digitalWrite(RED_PIN, LIGHT_OFF);
    period = schIndex;
    Serial.print(" (OUTSIDE SCHOOL)");

    //Turn only red light on during lunch time
  } else if ((nowTime >= convert_time(schReg[lunch][0], schReg[lunch][1])) && nowTime <= convert_time(schReg[lunch + 1][0], schReg[lunch + 1][1])) {
    digitalWrite(GREEN_PIN, LIGHT_OFF);
    digitalWrite(YELLOW_PIN, LIGHT_OFF);
    digitalWrite(RED_PIN, LIGHT_ON);
    Serial.print(" (LUNCH)");

    // Class time
  } else if (nowTime < endPeriod - WARNING_TIME) {
    digitalWrite(GREEN_PIN, LIGHT_ON);
    digitalWrite(YELLOW_PIN, LIGHT_OFF);
    digitalWrite(RED_PIN, LIGHT_OFF);
    Serial.print(" (CLASS)");

    // Five minutes before period ends
  } else if (nowTime < endPeriod) {
    digitalWrite(YELLOW_PIN, LIGHT_ON);
    digitalWrite(GREEN_PIN, LIGHT_OFF);
    digitalWrite(RED_PIN, LIGHT_OFF);
    Serial.print(" (WARNING)");

    // Passing time
  } else if (nowTime >= endPeriod && nowTime <= nextPeriod) {
    Serial.print(" (PASSING)");
    digitalWrite(RED_PIN, LIGHT_ON);
    digitalWrite(YELLOW_PIN, LIGHT_OFF);

    // Cycle the light testing and go to next period
  } else if (nowTime >= nextPeriod) {
    digitalWrite(RED_PIN, LIGHT_OFF);
    digitalWrite(GREEN_PIN, LIGHT_ON);
    Serial.print("!NEW PERIOD!");
    period += 2;
  }


  //Indicate on Serial monitor which light is on

  if (digitalRead(GREEN_PIN) ==  LIGHT_ON) {
    Serial.print(" |GREEN|");
  }
  if (digitalRead(YELLOW_PIN) == LIGHT_ON) {
    Serial.print(" |YELLOW|");
  }
  if (digitalRead(RED_PIN) ==  LIGHT_ON) {
    Serial.print(" |RED|");
  }

  Serial.println();
  delay(1000);

}
