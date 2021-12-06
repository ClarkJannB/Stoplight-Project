#include "RTClib.h"
#define PERIOD 2
#define WARNING 3
#define PASSING 4
#define GREEN 2
#define YELLOW 3
#define RED 4
RTC_PCF8523 rtc;
int state = PERIOD;
int nowTime = 547;

int currentPeriod = 0;


//loops through all thirteen and finds exact period

int schReg[16][2] {
  // Block 0
  {7, 45},  //465
  {7, 59},  //469
  // Block 1
  {7, 52},  //472 
  {8, 48},  //528
  // Block 2
  {8, 51},  //531
  {9, 47},  //587
  // Block 3
  {9, 50},  //590
  {10, 46}, //640
  // Block 4
  {10, 51}, //651
  {11, 47}, //707
  // Block 5
  {11, 50}, //710
  {12, 18}, //738
  // Block 6
  {12, 23}, //743
  {13, 19}, //799
  // Block 7
  {13, 22}, //802
  {14, 18}, //858
};

int period = 0;

int convert_time(int hrs, int mns) {
  int conv_time;
  conv_time = hrs * 60;
  conv_time = conv_time + mns;
  return conv_time;
};

void setup() {
  int i;
  Serial.begin(57600);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);



  // Gets initial block
  for ( i = 0; i < 8; i += 2) {
    int startPeriod = convert_time(schReg[i][0], schReg[i][1]);
    int nextPeriod = convert_time(schReg[i + 2][0], schReg[i + 2][1]);

    if ((nowTime >= startPeriod) && (nowTime <= nextPeriod)) {
      break;
    }

  }

  period = i;




  /*
      Serial.print("currentPeriod: ");
      Serial.println(currentPeriod);
      Serial.print("startPeriod: ");
      Serial.println(startPeriod);
      Serial.print("endPeriod: ");
      Serial.println(endPeriod);
      Serial.print("nextPeriod: ");
      Serial.println(nextPeriod);
      Serial.print("i: ");
      Serial.println(i);
      Serial.println("-------------------");
      Serial.println();
  */

  // if (nowTime >= startPeriod && nowTime <= nextPeriod) {

  //Serial.print("The Current Period: ");
  //    Serial.println(i);

  // }
}







void loop() {

  // Gets time values for current period
  int startPeriod = convert_time(schReg[period][0], schReg[period][1]); //green is on between startPeriod and endPeriod - 5
  int endPeriod = convert_time(schReg[period + 1][0], schReg[period + 1][1]); // yellow is on between endPeriod - 5 and endPeriod
  int nextPeriod = convert_time(schReg[period + 2][0], schReg[period + 2][1]);  //red is on between endPeriod and nextPeriod


  //Serial.print("currentPeriod: ");
  //Serial.println(currentPeriod);
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
  delay(1000);

  nowTime++;

  /*
    if (nowTime >= startPeriod && nowTime <= endPeriod) {
    Serial.println("It is PERIOD 1");
    currentPeriod = 1;
    }
  */

/*
  if (period = 0) {
    currentPeriod = 1;
  }
  else if (period = 2) {
    currentPeriod = 2;
  }
  else if (period = 4) {
    currentPeriod = 3;
  }
  else if (period = 6) {
    currentPeriod = 4;
  }
  else if (period = 8) {
    currentPeriod = 5;
  }
  else if (period = 10) {
    currentPeriod =  6;
  }
*/


  //period = 3;

  // In class time
  if (nowTime <= endPeriod - 5) {
    digitalWrite(GREEN, HIGH);
    digitalWrite(YELLOW, LOW);
    digitalWrite(RED, LOW);

    // Five minutes before period ends
  } else if (nowTime <= endPeriod) {
    digitalWrite(YELLOW, HIGH);
    digitalWrite(GREEN, LOW);
    digitalWrite(RED, LOW);

    // Passing time
  } else if (nowTime > endPeriod && nowTime <= nextPeriod) {
    //Serial.print("RED IS ON");
    digitalWrite(RED, HIGH);
    digitalWrite(YELLOW, LOW);

    // Cycle back and go to next period
  } else if (nowTime >= nextPeriod) {
    digitalWrite(RED, LOW);
    digitalWrite(GREEN, HIGH);
    period += 2;
  }


  // Function to turn off the lights when theres no school
  if (nowTime < 472 || nowTime > 858) {
    digitalWrite(GREEN, LOW);
    digitalWrite(YELLOW, LOW);
    digitalWrite(RED, LOW);
  }

  /*
    //Turn only red light on during lunch time
    if ((nowTime >= convert_time(schReg[10][0], schReg[10][1])) && nowTime <= convert_time(schReg[11][0], schReg[11][1]))) {
    digitalWrite(GREEN, LOW);
    digitalWrite(YELLOW, LOW);
    digitalWrite(RED, HIGH);
    }
  */



  /*
    //reset counter timer
    if (nowTime >= 890) {
      nowTime = 0;
    }
  */

}
