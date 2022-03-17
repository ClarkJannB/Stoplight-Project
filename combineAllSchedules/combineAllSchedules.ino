
#include "RTClib.h"
#define PERIOD 2
#define WARNING 3
#define PASSING 4
#define GREEN 2
#define YELLOW 3
#define RED 4
RTC_PCF8523 rtc;

int schState = 0;

int schStart = 0;
int schEnd = 6;


int currentPeriod = 0;
int period = 0;

int schBlk [5][5] {
  {0, 6},  //schedule regular
  {7, 13}, //schedule advisory
};

// Starting/end times of the period
int schReg[16][2] {
  //REGULAR
  // Block 0
  {7, 45}, //465
  {7, 50}, //470
  // Block 1
  {7, 53}, //473
  {8, 35}, //515
  // Block 2
  {8, 38}, //518
  {9, 16}, //556
  // Block 3
  {9, 19}, //559
  {9, 57}, //597
  // Block 4
  {10, 00}, //600
  {10, 38}, //638
  // Block 5
  {10, 41}, //641
  {11, 19}, //679
  // Block 6
  {11, 22}, //682
  {12, 00}, //720

  //ADVISORY

  // Block 0
  {7, 45}, //465
  {7, 50}, //470
  // Block 1
  {7, 53}, //473
  {8, 35}, //515
};



// Converts the hours and minutes of the array into # of minutes
int convert_time(int hrs, int mns) {
  int conv_time;
  conv_time = hrs * 60;
  conv_time = conv_time + mns;
  return conv_time;
};

void setup() {
  int conv_time;
  int i;
  Serial.begin(9600);

  if (schState == 0) {
    schStart = 0;
    schEnd = 6;
  } else {
    schStart = 6;
    schEnd = 13;
  }


  pinMode(GREEN, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(RED, OUTPUT);

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    abort();
  }

  DateTime now = rtc.now();
  int hrs = now.hour();
  int mins = now.minute();
  int secs = now.second();
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
  int conv_time;

  DateTime now = rtc.now();
  int hrs = now.hour();
  int mins = now.minute();
  int secs = now.second();
  int nowTime = convert_time(hrs, mins);

  Serial.print("nowTime: ");
  Serial.println(nowTime);



  // Gets time values for current period

  int startPeriod = convert_time(schReg[period][0], schReg[period][1]); //green is on between startPeriod and endPeriod - 5
  int endPeriod = convert_time(schReg[period + 1][0], schReg[period + 1][1]); // yellow is on between endPeriod - 5 and endPeriod
  int nextPeriod = convert_time(schReg[period + 2][0], schReg[period + 2][1]);  //red is on between endPeriod and nextPeriod

  Serial.print("startPeriod: ");
  Serial.println(startPeriod);
  Serial.print("endPeriod: ");
  Serial.println(endPeriod);
  Serial.print("nextPeriod: ");
  Serial.println(nextPeriod);
  Serial.print("block: ");
  Serial.println(period);
  Serial.print("nowTime: ");
  Serial.println(nowTime);
  Serial.println("-------------------");
  Serial.println();

  // In class time
  if (nowTime <= endPeriod - 5) {
    digitalWrite(GREEN, HIGH);
    digitalWrite(YELLOW, LOW);
    digitalWrite(RED, LOW);

    // Five minutes before period ends
  } else if (nowTime < endPeriod) {
    digitalWrite(YELLOW, HIGH);
    digitalWrite(GREEN, LOW);
    digitalWrite(RED, LOW);

    // Passing time
  } else if (nowTime >= endPeriod && nowTime <= nextPeriod) {
    //Serial.print("RED IS ON");
    digitalWrite(RED, HIGH);
    digitalWrite(YELLOW, LOW);

    // Cycle the light testing and go to next period
  } else if (nowTime >= nextPeriod) {
    digitalWrite(RED, LOW);
    digitalWrite(GREEN, HIGH);
    period += 2;
  }


  // Function to turn off the lights when theres no school
  if (nowTime < 465 || nowTime > 720) {
    digitalWrite(GREEN, LOW);
    digitalWrite(YELLOW, LOW);
    digitalWrite(RED, LOW);
  }

  delay(1000);

}
