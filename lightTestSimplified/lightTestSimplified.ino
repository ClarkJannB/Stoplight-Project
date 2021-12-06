
#include "RTClib.h"
#define PERIOD 2
#define WARNING 3
#define PASSING 4
#define GREEN 2
#define YELLOW 3
#define RED 4
RTC_PCF8523 rtc;

int state = PERIOD;
int nowTime = 0;

int schReg[15][2] {
  {7, 52},
  {8, 48},
  {8, 51},
  {9, 47},
  {9, 50},
  {10, 46},
  {10, 51},
  {11, 47},
  {12, 23},
  {13, 19},
  {13, 22},
  {14, 18},
};

int convert_time(int hrs, int mns) {
  int conv_time;
  conv_time = hrs * 60;
  conv_time = conv_time + mns;
  return conv_time;
};

void setup() {
  Serial.begin(57600);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);


}

void loop() {

  Serial.print(" nowTime: ");
  Serial.print(nowTime);

  if (nowTime <= 515) {
    delay(5);
  } else {
    delay(1000);
  }

  if (nowTime >= 900 ) {
    nowTime = 450;
  }


  nowTime++;


  switch (state) {
    case PERIOD:
      digitalWrite(GREEN, HIGH);
      Serial.println(" GREEN");  //print statement when green light is on
      for (int i = 0; i <= 13; i++) {
        //int p_begin = convert_time(schReg[i][0], schReg[i][1]); //checks the schedule
        int p_end = convert_time(schReg[i + 1][0], schReg[i + 1][1]) ; //checking the end times of the schedule minus 7 for passing time
        if (((nowTime >=  (p_end - 5))  &&  nowTime <= p_end )) { //this measures the last 5 minutes of class
          state = WARNING;
          digitalWrite(GREEN, LOW);
        }
      }
      break;
    case WARNING:
      digitalWrite(YELLOW, HIGH); //turn on yellow light
      Serial.println(" (YELLOW)"); //print statement when yellow light is on

      for (int x = 0; x <= 13; x++) {
        //int startWarning = convert_time(schReg[x][0], schReg[x][1]) ; //the start of the period + 51 minutes
        int endWarning = convert_time(schReg[x + 1][0], schReg[x + 1][1]);  //the end of the period
        // print statements so I know the start of the period
        //  if (nowTime == endWarning)
        Serial.println("end of Period");
        Serial.print(endWarning);
        if (nowTime >= endWarning) {
          state = PASSING;
          digitalWrite(YELLOW, LOW);
        }
      }
      break;
    //maybe delete case passing if yellow still doesn't work
    case PASSING:
      digitalWrite(RED, HIGH);
      Serial.print("  (RED) ");

      for (int x = 0; x  <= 13; x++) {
        int endPeriod = convert_time(schReg[x + 1][0], schReg[x + 1][1]);
        int startPeriod = convert_time(schReg[x][0], schReg[x + 1] [1]);

        if (nowTime >= endPeriod && nowTime <= startPeriod) { 
          state = PERIOD; 
          digitalWrite(RED, LOW);
        }
      }

}

}
