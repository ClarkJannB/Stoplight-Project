
#include "RTClib.h"
//#define PERIOD 2
//#define WARNING 3
//#define PASSING 4
#define GREEN 2
#define YELLOW 3
#define RED 4
#define LIGHT_ON  0
#define LIGHT_OFF 1
#define WARNING_TIME 5
#define SWITCHA 5
#define SWITCHB 6
RTC_DS1307 rtc;


int schStart = 16;
int schEnd = 29;
int schJump = 7; //how many periods there are in the day, how many times it jumps
int lunch = 10;

//int currentPeriod = 0;
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
  {7, 59},  //469
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



  //EARLY RELEASE SCHEDULE:

  //Index (16-29)
  // Block 0 (ADVISORY)
  {7, 45}, //465
  {7, 50}, //470
  // Block 1 (PERIOD 1)
  {7, 53}, //473
  {8, 35}, //515
  // Block 2 (PERIOD 2)
  {8, 38}, //518
  {9, 16}, //556
  // Block 3 (PERIOD 3)
  {9, 19}, //559
  {9, 57}, //597
  // Block 4 (PERIOD 4)
  {10, 00}, //600
  {10, 38}, //638
  // Block 5 (PERIOD 5)
  {10, 41}, //641
  {11, 19}, //679
  // Block 6 (PERIOD 6)
  {11, 22}, //682
  {12, 00}, //720


  //ADVISORY ACTIVITY SCHEDULE:

  //Index (30-47)
  // Block 0 (ADVISORY)
  {7, 45}, //465
  {7, 50}, //470
  // Block 1 (PERIOD 1)
  {7, 53}, //473
  {8, 41}, //521
  // Block 2 (PERIOD 2)
  {8, 44}, //524
  {9, 32}, //572
  // Block 3 (ADVISORY ACTIVITY)
  {9, 35}, //575
  {10, 19}, //619
  // Block 4 (PERIOD 3)
  {10, 22}, //622
  {11, 10}, //670
  // Block 5 (PERIOD 4)
  {11, 15}, //675
  {12, 03}, //723
  // Block 6 (LUNCH)
  {12, 06}, //726
  {12, 36}, //756
  // Block 7 (PERIOD 5)
  {12, 39}, //759
  {13, 27}, //807
  // Block 8 (PERIOD 6)
  {13, 30}, //810
  {14, 18}, //858


  //EXTENDED ADVISORY ACTIVITY:

  //Index (48-65)
  // Block 0 (ADVISORY)
  {7, 45}, //
  {8, 15}, //
  // Block 1 (PERIOD 1)
  {8, 18}, //
  {9, 10}, //
  // Block 2 (PERIOD 2)
  {9, 13}, //
  {10, 05}, //
  // Block 3 (PERIOD 3)
  {10, 8}, //
  {11, 00}, //
  // Block 4 (PERIOD 4)
  {11, 03}, //
  {11, 55}, //
  // Block 5 (LUNCH)
  {11, 58}, //
  {12, 28}, //
  // Block 6 (PERIOD 5)
  {12, 31}, //
  {13, 23}, //
  // Block 7 (PERIOD 6)
  {13, 26}, //
  {14, 18}, //






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

  pinMode(GREEN, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(SWITCHA, INPUT_PULLUP);
  pinMode(SWITCHB, INPUT_PULLUP);

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    abort();
  }

  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));


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
  int switcha = digitalRead(SWITCHA);
  int switchb = digitalRead(SWITCHB);

  DateTime now = rtc.now();
  int hrs = now.hour();
  int mins = now.minute();
  int secs = now.second();
  int nowTime = convert_time(hrs, mins);

  // Gets time values for current period

  int startPeriod = convert_time(schReg[period][0], schReg[period][1]); //green is on between startPeriod and endPeriod - 5
  int endPeriod = convert_time(schReg[period + 1][0], schReg[period + 1][1]); // yellow is on between endPeriod - 5 and endPeriod
  int nextPeriod = convert_time(schReg[period + 2][0], schReg[period + 2][1]);  //red is on between endPeriod and nextPeriod

  Serial.print("nowTime: ");
  Serial.print(nowTime);
  Serial.print("  ");
  Serial.print("StartP: ");
  Serial.print(startPeriod);
  Serial.print("  ");
  Serial.print("EndP: ");
  Serial.print(endPeriod);
  Serial.print("  ");
  Serial.print("NextP: ");
  Serial.print(nextPeriod);
  Serial.print("  ");
  Serial.print("Period ");
  Serial.print(period / 2);
  Serial.print("  ");
  Serial.print("hrs:");
  Serial.print(hrs);
  Serial.print("  ");
  Serial.print("mins:");
  Serial.print(mins);
  Serial.print("  ");
  Serial.print("secs:");
  Serial.print(secs);

  //Controls the schedules

  if  (switcha == HIGH && switchb == HIGH) {
    //Regular Schedule
    schStart = 0;
    schEnd = 15;
    schJump = 8;
    lunch = 10;

    schIndex = 0;
    Serial.print(" REGULAR SCHEDULE");

  } else if (switcha == LOW && switchb == LOW) {
    //Early Release Schedule
    schStart = 16;
    schEnd = 29;
    schJump = 7;
    lunch = -1;

    schIndex = 16;
    Serial.print(" EARLY RELEASE");

  } else if (switcha == HIGH && switchb == LOW) {
    //Advisory Activity Schedule
    schStart = 30;
    schEnd = 47;
    schJump = 9;
    lunch = 42;

    schIndex = 30;
    Serial.print(" ADVISORY ACTIVITY ");

  } else if (switcha == LOW && switchb == HIGH) {
    //Extended Advisory Schedule
    schStart = 48;
    schEnd = 63;
    schJump = 8;
    lunch = 58;

    schIndex = 48;
    Serial.print(" EXTENDED ADVISORY");
  }

  // Function to turn off the lights when theres no school
  if (nowTime < convert_time(schReg[schStart][0], schReg[schStart][1]) || nowTime > convert_time(schReg[schEnd][0], schReg[schEnd][1])) {
    digitalWrite(GREEN, LIGHT_OFF);
    digitalWrite(YELLOW, LIGHT_OFF);
    digitalWrite(RED, LIGHT_OFF);
    Serial.println(" ALL OFF");

    //Turn only red light on during lunch time
  } else if ((nowTime >= convert_time(schReg[lunch][0], schReg[lunch][1])) && nowTime <= convert_time(schReg[lunch + 1][0], schReg[lunch + 1][1])) {
    digitalWrite(GREEN, LIGHT_OFF);
    digitalWrite(YELLOW, LIGHT_OFF);
    digitalWrite(RED, LIGHT_ON);
    Serial.println("LUNCH");

    // Class time
  } else if (nowTime <= endPeriod - WARNING_TIME) {
    digitalWrite(GREEN, LIGHT_ON);
    digitalWrite(YELLOW, LIGHT_OFF);
    digitalWrite(RED, LIGHT_OFF);
    Serial.println(" GREEN ON");

    // Five minutes before period ends
  } else if (nowTime <= endPeriod) {
    digitalWrite(YELLOW, LIGHT_ON);
    digitalWrite(GREEN, LIGHT_OFF);
    digitalWrite(RED, LIGHT_OFF);
    Serial.println(" YELLOW ON");

    // Passing time
  } else if (nowTime >= endPeriod && nowTime <= nextPeriod) {
    Serial.println(" RED ON");
    digitalWrite(RED, LIGHT_ON);
    digitalWrite(YELLOW, LIGHT_OFF);

    // Cycle the light testing and go to next period
  } else if (nowTime >= nextPeriod) {
    digitalWrite(RED, LIGHT_OFF);
    digitalWrite(GREEN, LIGHT_ON);
    Serial.println("!NEW PERIOD!");
    period += 2;
  }

  delay(1000);

}
