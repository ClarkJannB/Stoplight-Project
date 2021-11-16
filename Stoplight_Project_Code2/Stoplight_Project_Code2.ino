
#include "RTClib.h"

#define NOSCHOOL 1
#define PERIOD 2
#define WARNING 3
#define PASSING 4

#define GREEN 2
#define YELLOW 3
#define RED 4
RTC_PCF8523 rtc;

int state = NOSCHOOL;

int nowTime = 0;

int schReg[15][2] {
  {7, 45},
  {7, 49},
  {7, 52},
  {8, 48},
  {8, 51},
  {9, 47},
  {9, 50},
  {10, 46},
  {10, 51},
  {11, 47},
  {12, 20},
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


void setup () {
  Serial.begin(57600);

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    abort();
  }

  if (! rtc.initialized() || rtc.lostPower()) {
    Serial.println("RTC is NOT initialized, let's set the time!");
    rtc.adjust(DateTime(2021, 1, 1, 7, 7, 0));
    rtc.start();
  }


  rtc.start();

  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
}

void loop () {
  DateTime now = rtc.now();

  //int hrs = now.hour();
  //  int mins = now.minute();

  // TESTING TIMES
  int hrs = now.minute();
  int mins = now.second();
  //int secs = now.second();

  //int nowTime = convert_time(hrs, mins);
  nowTime++;
  if (nowTime <= 450) {
    delay(10);
  } else {
    delay(200);
  }


  //prints time readout
  Serial.print("The current time is ");
  Serial.print(hrs);
  Serial.print(" h ");
  Serial.print(mins);
  Serial.print(" m ");
  Serial.println();
  /*
    Serial.print(secs);
    Serial.print(" s ");
    Serial.println();
  */


  Serial.print("nowTime:");
  Serial.println(nowTime);
  /*
    // Checks for lunch
    if ( nowTime >= convert_time(schReg[9][0], schReg[9][1]) && nowTime <= convert_time(schReg[10][0], schReg[10][1])) {
      digitalWrite(4, HIGH);
    } else  {
      digitalWrite(4, LOW);
    }
  */

  if (nowTime >= 900 ) {
    nowTime = 0;
  }

  switch (state) {
    case PERIOD:
      //set led green
      digitalWrite(GREEN, HIGH);  //turn on green light
      Serial.println("Green");  //print statement when green light is on 
      for (int i = 0; i <= 13; i++) {
        int p_begin = convert_time(schReg[i][0], schReg[i][1]); //checks the schedule
        int p_end = convert_time(schReg[i + 1][0], schReg[i + 1][1]) - 7; //checking the end times of the schedule minus 7 for passing time
        if (nowTime >=  p_begin &&  nowTime <= p_end ) {
          state = WARNING;  //move on to Yellow state
          digitalWrite(GREEN, LOW); //turn green light off
        }
      }
      break;
    case WARNING:
      digitalWrite(YELLOW, HIGH); //turn on yellow light
      Serial.println("Yellow"); //print statement when yellow light is on 
      for (int x = 0; x <= 13; x++) {
        int startPassing = convert_time(schReg[x][0], schReg[x][1]) + 51;
        int endPassing = convert_time(schReg[x + 1][0], schReg[x + 1][1]);
        if (nowTime >=  startPassing &&  nowTime <= endPassing ) {
          state = PASSING;

        }
      }
      break;
    case PASSING:
      digitalWrite(RED, HIGH);
      Serial.print("Red");
      //for (int x = 0; x <= 13; x++) {
      //if (((nowTime <= convert_time(schReg[x][0], schReg[x][1])) &&  (nowTime >= convert_time(schReg[x + 1][0], schReg[x + 1][1])))) {
      state = PERIOD;
      digitalWrite(RED, LOW);
      break;
    case NOSCHOOL:
    int preSchool = convert_time(schReg[0][0], schReg[0][1]);
    int postSchool = convert_time(schReg[14][0], schReg[14][1]);
    Serial.print("No School"); 
    if (nowTime <= preSchool && nowTime >= postSchool){ 
      digitalWrite(GREEN, LOW);
      digitalWrite(YELLOW, LOW); 
      digitalWrite(RED, LOW); 
      Serial.println("no school");
    } else { 
      state = PERIOD; 
    }
      break;
      


  }
}



/*
  if (state == NOTSCHOOL) {
    digitalWrite(GREEN, LOW);
    digitalWrite(YELLOW, LOW);
    digitalWrite(RED, LOW);
  } else if (state = PASSING) {

  }
*/
