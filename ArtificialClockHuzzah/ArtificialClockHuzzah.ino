#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#define DAYLIGHTSAVINGS 1
#define GREEN_PIN 5
#define YELLOW_PIN 0
#define RED_PIN 2
#define LIGHT_ON 1
#define LIGHT_OFF 0
#define WARNING_TIME 5

int currentPeriod = 0;
int block = 0;
int blockStart = 7; 
int nowTime = 466;
int schStart = 14; //Example: for Advisory Activity Schedule change the value to 30
int schEnd = 27; //Example: for Advisory Activity Schedule change the value to 47
int schJump = 7; //how many blocks there are in the day, how many times it jumps
int lunch = 10;  //Change this value to the position in the schedule of the start of the lunch period. Example: for Advisory Activity Schedule change the value to 42


int schReg[58][2] {

  //REGULAR TIME SCHEDULE:
  //Index (0 - 13)
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
  
  // Block 4.5 (LUNCH)
  //{11, 50}, //710
  //{12, 18}, //738
  
  // Block 5 (PERIOD 5)
  {12, 23}, //743
  {13, 19}, //799
  // Block 6 (PERIOD 6)
  {13, 22}, //802
  {14, 18}, //858


  //EARLY RELEASE SCHEDULE:

  //Index (14-27)
  // Block 7 (ADVISORY)
  {7, 45}, //465
  {7, 50}, //470
  // Block 8 (PERIOD 1)
  {7, 53}, //473
  {8, 35}, //515
  // Block 9 (PERIOD 2)
  {8, 38}, //518
  {9, 16}, //556
  // Block 10 (PERIOD 3)
  {9, 19}, //559
  {9, 57}, //597
  // Block 11 (PERIOD 4)
  {10, 00}, //600
  {10, 38}, //638
  // Block 12 (PERIOD 5)
  {10, 41}, //641
  {11, 19}, //679
  // Block 13 (PERIOD 6)
  {11, 22}, //682 
  {12, 00}, //720

  //ADVISORY ACTIVITY SCHEDULE:

  //Index (28 - 43)
  // Block 14 (ADVISORY)
  {7, 45}, //465
  {7, 50}, //470
  // Block 15 (PERIOD 1)
  {8, 35}, //515
  {8, 41}, //521
  // Block 16 (PERIOD 2)
  {8, 44}, //524
  {9, 32}, //572
  // Block 17 (ADVISORY ACTIVITY)
  {9, 35}, //575
  {10, 19}, //619
  // Block 18 (PERIOD 3)
  {10, 22}, //622
  {11, 10}, //670
  // Block 19 (PERIOD 4)
  {11, 15}, //675
  {12, 03}, //723
  
  // Block 19.5 (LUNCH)
  //{12, 06}, //726
  //{12, 36}, //756
  
  // Block 20 (PERIOD 5)
  {12, 39}, //759
  {13, 27}, //807
  // Block 21 (PERIOD 6)
  {13, 30}, //810
  {14, 18}, //858


  //EXTENDED ADVISORY ACTIVITY:

  //Index (45-57)
  // Block 22 (ADVISORY)
  {7, 45}, //465
  {8, 15}, //495
  // Block 23 (PERIOD 1)
  {8, 18}, //498
  {9, 10}, //550
  // Block 24 (PERIOD 2)
  {9, 13}, //553
  {10, 05}, //605
  // Block 25 (PERIOD 3)
  {10, 8}, //608
  {11, 00}, //660
  // Block 26 (PERIOD 4)
  {11, 03}, //663
  {11, 55}, //715
  
  // Block 26.5 (LUNCH)
  //{11, 58}, //718
  //{12, 28}, //748
  
  // Block 27 (PERIOD 5)
  {12, 31}, //751
  {13, 23}, //803
  // Block 28 (PERIOD 6)
  {13, 26}, //806
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
  int i;
  Serial.begin(115200);

  pinMode(GREEN_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(RED_PIN, OUTPUT);


  // Gets initial block
  for ( i = 0; i < 16; i += 2) {
    int startPeriod = convert_time(schReg[i][0], schReg[i][1]);
    int nextPeriod = convert_time(schReg[i + 2][0], schReg[i + 2][1]);

    if (nowTime >= startPeriod && nowTime <= nextPeriod) {
      break;
    }
  }
  block = i ;
  Serial.print("block=");
  Serial.println(block);
}

void loop() {
  nowTime += 1;
  // Gets time values for current period

  int startPeriod = convert_time(schReg[block][0], schReg[block][1]); //green is on between startPeriod and endPeriod - 5
  int endPeriod = convert_time(schReg[block + 1][0], schReg[block + 1][1]); // yellow is on between endPeriod - 5 and endPeriod
  int nextPeriod = convert_time(schReg[block + 2][0], schReg[block + 2][1]);  //red is on between endPeriod and nextPeriod

  Serial.print("nowTime: ");
  Serial.print(nowTime);
  Serial.print(" block = ");
  Serial.print(block);

  // Function to turn off the lights when theres no school
  if (nowTime < convert_time(schReg[schStart][0], schReg[schStart][1]) || nowTime > convert_time(schReg[schEnd][0], schReg[schEnd][1])) {
    digitalWrite(GREEN_PIN, LIGHT_OFF);
    digitalWrite(YELLOW_PIN, LIGHT_OFF);
    digitalWrite(RED_PIN, LIGHT_OFF);
    //block = schIndex;
    Serial.print(" (OUTSIDE SCHOOL)");
/*
    //Turn only red light on during lunch time
  } else if ((nowTime >= convert_time(schReg[lunch][0], schReg[lunch][1])) && nowTime <= convert_time(schReg[lunch + 1][0], schReg[lunch + 1][1])) {
    digitalWrite(GREEN_PIN, LIGHT_OFF);
    digitalWrite(YELLOW_PIN, LIGHT_OFF);
    digitalWrite(RED_PIN, LIGHT_ON);
    Serial.print(" (LUNCH)");
*/
    // Class time
  } else if (nowTime < endPeriod - WARNING_TIME) {
    digitalWrite(GREEN_PIN, LIGHT_ON);
    digitalWrite(YELLOW_PIN, LIGHT_OFF);
    digitalWrite(RED_PIN, LIGHT_OFF);
    Serial.print(" (CLASS)");

    // Five minutes before block ends
  } else if (nowTime < endPeriod) {
    digitalWrite(YELLOW_PIN, LIGHT_ON);
    digitalWrite(GREEN_PIN, LIGHT_OFF);
    digitalWrite(RED_PIN, LIGHT_OFF);
    Serial.print(" (WARNING)");

    // Passing time
  } else if (nowTime >= endPeriod && nowTime < nextPeriod) {
    Serial.print(" (PASSING)");
    digitalWrite(RED_PIN, LIGHT_ON);
    digitalWrite(YELLOW_PIN, LIGHT_OFF);

    // Cycle the light and go to next block
  } else if (nowTime >= nextPeriod) {
    digitalWrite(RED_PIN, LIGHT_OFF);
    digitalWrite(GREEN_PIN, LIGHT_ON);
    Serial.print(" !NEW PERIOD!");
    block += 2;
  } 

  //Indicates on the Serial monitor which light is on

  if (digitalRead(GREEN_PIN) ==  LIGHT_ON) {
    Serial.print(" |GREEN|");
  }
  if (digitalRead(YELLOW_PIN) == LIGHT_ON) {
    Serial.print(" |YELLOW|");
  }
  if (digitalRead(RED_PIN) ==  LIGHT_ON) {
    Serial.print(" |RED|");
  }

  if (nowTime >= 870) {
    nowTime = 450;
    block = blockStart;
  }
  Serial.println();
  delay(100);


}
